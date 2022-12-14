#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#define TAM 40000 // tamanho do vetor principal
#include <time.h>
#define TAG_SEND_DIR 11
#define TAG_SEND_ESQ 12

/*
    ##################################################################################################
                                               _                                  
                                  ___  _ __ __| | ___ _ __   __ _  ___ __ _  ___  
                                 / _ \| '__/ _` |/ _ \ '_ \ / _` |/ __/ _` |/ _ \ 
                                | (_) | | | (_| |  __/ | | | (_| | (_| (_| | (_) |
                                 \___/|_|  \__,_|\___|_| |_|\__,_|\___\__,_|\___/ 
                                                                                  
                                     _                                  
                                 ___(_)_ __   ___ _ __ ___  _ __   __ _ 
                                / __| | '_ \ / __| '__/ _ \| '_ \ / _` |
                                \__ \ | | | | (__| | | (_) | | | | (_| |
                                |___/_|_| |_|\___|_|  \___/|_| |_|\__,_|
    ##################################################################################################
                                     
*/

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// quicksort
void order_vetor(int *vetor, int tam) {
    qsort(vetor, tam, sizeof(int), compare);
}

/*
    ##################################################################################################
*/

MPI_Status status;

void split_min(int *vetor, int tam, int rank) {
    while(1) {
        int temp;

        // envia maior valor para o processo seguinte e recebe o menor valor deste
        MPI_Send(&vetor[tam-1], 1, MPI_INT, rank+1, TAG_SEND_DIR, MPI_COMM_WORLD);
        MPI_Recv(&temp, 1, MPI_INT, rank+1, TAG_SEND_ESQ, MPI_COMM_WORLD, &status);
        
        if (temp < vetor[tam-1]) {
            // ordena o novo valor, O(n)
            int i;
            for (i = tam-2; vetor[i] > temp && i>=0; i--) vetor[i+1] = vetor[i];
            vetor[i+1] = temp;
        }
        else break;
    }
}


void split_max(int *vetor, int tam, int rank) {
    while(1) {
        int temp;

        // envia menor valor para o processo anterior e recebe o maior valor deste
        MPI_Send(vetor, 1, MPI_INT, rank-1, TAG_SEND_ESQ, MPI_COMM_WORLD);
        MPI_Recv(&temp, 1, MPI_INT, rank-1, TAG_SEND_DIR, MPI_COMM_WORLD, &status);
        
        if (temp > vetor[0]) {
            // ordena o novo valor, O(n)
            int i;
            for (i = 1; temp > vetor[i] && i <= tam-1; i++) vetor[i-1] = vetor[i];
            vetor[i-1] = temp;
        }
        else break;
    }
}



int main(int argc, char** argv) {
    /*
    ##################################################################################################
                   ___ _   _ ___ ____ ___    _    _     ___ _____   _    ____    _    ___                                 
                  |_ _| \ | |_ _/ ___|_ _|  / \  | |   |_ _|__  /  / \  / ___|  / \  / _ \ 
                  | ||  \| || | |    | |  / _ \ | |    | |  / /  / _ \| |     / _ \| | | |
                  | || |\  || | |___ | | / ___ \| |___ | | / /_ / ___ \ |___ / ___ \ |_| |
                 |___|_| \_|___\____|___/_/   \_\_____|___/____/_/   \_\____/_/   \_\___/ 
    #################################################################################################
    */
    double time_spent = 0.0;
    clock_t begin = clock();
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    int tam_padrao = TAM/size; // tamanho do vetor local (sem resto)
    int inicio = rank * tam_padrao;
    int fim = inicio + tam_padrao;
    int resto = TAM % size;
    if (rank == size-1) fim += resto; // resto fica no ??ltimo processo
    int tam = fim - inicio; // tamanho do vetor local para cada processo

    int *vetor_local = (int*) malloc(sizeof(int)*tam);
    if (rank == 0) {
        // leitura dos TAM valores
        FILE *f = fopen("input.txt", "r");
        int *vetor = (int*) malloc(sizeof(int)*TAM);
        for(int i = 0 ; i < TAM;i++){
            int temp;
            fscanf(f, "%d", &temp);
            vetor[i] = temp;
        }

        // distribui????o do vetor principal pelos vetores locais dos processos
        for (int i = 1; i < size; i++) {
            // define o tamanho da mensagem de acordo com o seu destino
            int tamanho = (i == size-1)? tam_padrao+resto : tam_padrao;
            MPI_Send(&vetor[i*tam_padrao], tamanho, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < tam; i++) vetor_local[i] = vetor[i];
        free(vetor);
    } 
    else {
        // distribui????o do vetor principal pelos vetores locais dos processos
        MPI_Recv(vetor_local, tam, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    /*
    ######################################################################################################
    */
    
    // n??mero de itera????es extras (necess??rias quando tem resto)
    int extra = ceil((float)(resto) / tam_padrao);

    // ordena????o sequencial dos vetores locais
    order_vetor(vetor_local, tam);
    
    for (int interator_process = 0; interator_process < size+extra; interator_process++) {
        if (interator_process % 2 == 1) { // itera????o impar
            if (rank % 2 == 1) { //rank impar
                if (rank < size-1) 
                    split_min(vetor_local, tam, rank);
            }
            else { //rank par
                if (rank > 0)
                    split_max(vetor_local, tam, rank);
            }
        }
        else { // itera????o par
            if (rank % 2 == 0 ) { //rank par
                if (rank < size-1) 
                    split_min(vetor_local, tam, rank);
            }
            else { //rank impar
                split_max(vetor_local, tam, rank);
            }
        }
    }
    
    if (rank==0) {
        int *vetor = (int*) malloc(sizeof(int)*TAM);
        for (int i = 0; i < tam; i++) vetor[i] = vetor_local[i];
        free(vetor_local);

        // jun????o dos vetores locais no processo 0
        for (int i = 1; i < size; i++) {
            // define o tamanho da mensagem de acordo com a sua origem
            int tam_rank = (i == size-1)? tam_padrao+resto : tam_padrao;
            MPI_Recv(&vetor[i * tam_padrao], tam_rank, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        
        // tempo de execu????o
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        
        // escreve vetor ordenado em um arquivo
        FILE * f = fopen("output.txt", "w");
        for (int i = 0; i < TAM; i++) fprintf(f, "%d  ", vetor[i]);
        free(vetor); fclose(f);

        // escreve tempo de execu????o em um arquivo
        FILE *f1 = fopen("time_result.csv", "a");
        fprintf(f1, "%f\n", time_spent);
        fclose(f1);
    } 
    else {
        // jun????o dos vetores locais no processo 0
        MPI_Send(vetor_local, tam, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(vetor_local);
    }

    MPI_Finalize();
    
    return 0;
}
