#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#define TAM 1000
#include <time.h>
#define TAG_SEND_DIR 11
#define TAG_SEND_ESQ 12

// int vetor[TAM] = {9,8,7,6,5,4,3,2,1,0,-1,-2};

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

void comparare_exchage(int *vetor,int i, int j) {
    int temp;
    if (vetor[i] > vetor[j]) {
        temp = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = temp;
    }
}

void order_vetor(int *vetor, int tam) {
    for (int i = 0 ; i < tam; i++)
        for (int j = 0; j < tam-1; j++){
            comparare_exchage(vetor,j,j+1);}
}

/*
    ##################################################################################################
*/


MPI_Status status;


void split_min(int *vetor, int tam_padrao, int tam_resto, int rank, int size) {
    int tam_min = tam_padrao;
    int tam_max = (rank == size-2)? tam_resto : tam_padrao;
    int tam_aux = tam_min + tam_max;
    int *vetor_aux = (int*) malloc((tam_aux)*sizeof(int));

    for (int i = 0; i < tam_min; i++) vetor_aux[i] = vetor[i];

    MPI_Send(vetor, tam_min, MPI_INT, rank+1, TAG_SEND_DIR, MPI_COMM_WORLD);
    MPI_Recv(&vetor_aux[tam_min], tam_max, MPI_INT, rank+1, TAG_SEND_ESQ, MPI_COMM_WORLD, &status);

    order_vetor(vetor_aux, tam_aux);

    for (int i = 0; i < tam_min; i++) vetor[i] = vetor_aux[i];
    free(vetor_aux);

    // int *vetor_aux = (int*) malloc((TAM+2)*sizeof(int)); 
    // vetor_aux[0] = fim-inicio;
    // vetor_aux[1] = inicio;
    // for(int i = 0; i < fim;i++) vetor_aux[2+i] = vetor[inicio+i];
    // MPI_Send(vetor_aux, TAM+2, MPI_INT, rank+1, TAG_SEND_DIR, MPI_COMM_WORLD);

    // MPI_Recv(vetor_aux, TAM+2, MPI_INT, rank+1, TAG_SEND_ESQ, MPI_COMM_WORLD, &status);
  
    // for (int i = 0 ;i < vetor_aux[0]; i++) vetor[ vetor_aux[1] + i] = vetor_aux[i+2];

    // order_vetor(vetor,inicio,fim+vetor_aux[0]);

    // free(vetor_aux);    
}


void split_max(int *vetor, int tam_padrao, int tam_resto,int rank, int size) {
    int tam_min = tam_padrao;
    int tam_max = (rank == size-1)? tam_resto : tam_padrao;
    int tam_aux = tam_min + tam_max;
    int *vetor_aux = (int*) malloc((tam_aux)*sizeof(int));

    for (int i = 0; i < tam_max; i++) vetor_aux[i] = vetor[i];

    MPI_Send(vetor, tam_max, MPI_INT, rank-1, TAG_SEND_ESQ, MPI_COMM_WORLD);
    MPI_Recv(&vetor_aux[tam_max], tam_min, MPI_INT, rank-1, TAG_SEND_DIR, MPI_COMM_WORLD, &status);

    order_vetor(vetor_aux, tam_aux);

    for (int i = 0; i < tam_max; i++) vetor[i] = vetor_aux[tam_min + i];
    free(vetor_aux);


    // int *vetor_aux = (int*) malloc((TAM+2)*sizeof(int)); 
    // vetor_aux[0] = fim-inicio;
    // vetor_aux[1] = inicio;
    // for(int i = 0; i < fim;i++) vetor_aux[2+i] = vetor[inicio+i];
    // MPI_Send(vetor_aux, TAM+2, MPI_INT, rank-1, TAG_SEND_ESQ, MPI_COMM_WORLD);

    // MPI_Recv(vetor_aux, TAM+2, MPI_INT, rank-1, TAG_SEND_DIR, MPI_COMM_WORLD, &status);

    // for (int i = 0 ;i < vetor_aux[0]; i++) vetor[ vetor_aux[1] + i] = vetor_aux[i+2];
    
    // order_vetor(vetor,vetor_aux[1],fim);      
    // free(vetor_aux); 


    // while(1){
    //     order_vetor(vetor,inicio,fim);
    //     int temp;
        
    //     MPI_Recv(&temp, 1, MPI_INT, rank-1, TAG_SEND_DIR, MPI_COMM_WORLD, &status);
    //     MPI_Send(&vetor[inicio], 1, MPI_INT, rank-1, TAG_SEND_ESQ, MPI_COMM_WORLD);
    //     if (temp > vetor[inicio]) vetor[inicio] = temp;
    //     else break;
    //     // inicio++;
    // }
}



int main(int argc, char** argv) {
    // int TAM;
    // fscanf(f,  "%d\n", )

    // return 0;
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

    int tam_padrao = TAM/size;
    int inicio = rank * tam_padrao;
    int fim = inicio + tam_padrao;
    int resto = TAM % size;
    if (rank == size-1) fim += resto;
    int tam = fim - inicio;
    // printf("%d  %d\n", inicio, fim);

    int *vetor_local = (int*) malloc(sizeof(int)*tam);
    if (rank == 0) {
        FILE *f = fopen("input.txt", "r");
        int *vetor = (int*) malloc(sizeof(int)*TAM);
        for(int i = 0 ; i < TAM;i++){
            int temp;
            fscanf(f,"%d", &temp);
            // printf("%d\n", temp);
            vetor[i] = temp;
        }

        int i;
        for (i = 1; i < size-1; i++) MPI_Send(&vetor[i*tam_padrao], tam_padrao, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(&vetor[i*tam_padrao], tam_padrao+resto, MPI_INT, i, 0, MPI_COMM_WORLD);

        for (int i = 0; i < tam; i++) vetor_local[i] = vetor[i];
        free(vetor);
    } 
    else {
        MPI_Recv(vetor_local, tam, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    /*
    ######################################################################################################
    */
    
    int extra = ceil((float)(resto) / tam_padrao);
    
    for (int interator_process = 0 ; interator_process < size + extra; interator_process++) {
        // printf("TESTE\n");
        if (size == 1) order_vetor(vetor_local, tam); // 1 processo

        if (interator_process % 2 == 1) {
            if (rank % 2 == 1) { //rank impar
                if (rank < size-1) 
                    split_min(vetor_local, tam_padrao, tam_padrao+resto, rank, size);
            }
            else {
                if (rank > 0)
                    split_max(vetor_local, tam_padrao, tam_padrao+resto, rank, size);
            }
        }
        else { // interador par
            if (rank % 2 == 0 ) {
                if (rank < size-1) 
                    split_min(vetor_local, tam_padrao, tam_padrao+resto, rank, size);
            }
            else {//rank impar
                split_max(vetor_local, tam_padrao, tam_padrao+resto, rank, size);
            }
        }
    }
    
    if (rank==0) {
        int *vetor = (int*) malloc(sizeof(int)*TAM);
        for (int i = 0; i < tam; i++) vetor[i] = vetor_local[i];
        free(vetor_local);

        for (int i = 1; i < size; i++) {
            int tam_rank = (i == size-1)? tam_padrao+resto : tam_padrao;
            MPI_Recv(&vetor[i * tam_padrao], tam_rank, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        
        // tempo de execução
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("The elapsed time is %f seconds\n", time_spent);
        // escrevendo resultado no arquivo
        FILE * f = fopen("output.txt", "w");
        for (int i = 0; i < TAM; i++) fprintf(f, "%d  ", vetor[i]);
        free(vetor); fclose(f);
    } 
    else {
        MPI_Send(vetor_local, tam, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(vetor_local);
    }

    MPI_Finalize();
    
    return 0;

    // printf()
}
