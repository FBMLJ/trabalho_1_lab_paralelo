#include <stdio.h>
#include <mpi.h>
#include<stdlib.h>
#define TAM 1000
#include<time.h>
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



void comparare_exchage(int *vetor,int i, int j){
    int temp;
    if (vetor[i] > vetor[j]){
        temp = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = temp;
    }
}

void order_vetor(int *vetor, int inicio, int fim){

    
    for (int i= 0 ; i < fim - inicio; i++)
        for (int j=  inicio ; j < fim-i-1; j++){

            comparare_exchage(vetor,j,j+1);}
        

}

/*
    ##################################################################################################

*/


MPI_Status status;


void split_min(int *vetor, int inicio, int fim,int rank){
    int *vetor_aux = (int*) malloc((TAM+2)*sizeof(int)); 
    vetor_aux[0] = fim-inicio;
    vetor_aux[1] = inicio;
    for(int i = 0; i < fim;i++) vetor_aux[2+i] = vetor[inicio+i];
    MPI_Send(vetor_aux, TAM+2, MPI_INT, rank+1, TAG_SEND_DIR, MPI_COMM_WORLD);

    MPI_Recv(vetor_aux, TAM+2, MPI_INT, rank+1, TAG_SEND_ESQ, MPI_COMM_WORLD, &status);

    
  
    for (int i = 0 ;i < vetor_aux[0]; i++) vetor[ vetor_aux[1] + i] = vetor_aux[i+2];

    

    order_vetor(vetor,inicio,fim+vetor_aux[0]);

    free(vetor_aux);    
}


void split_max(int *vetor, int inicio, int fim, int rank){
    int *vetor_aux = (int*) malloc((TAM+2)*sizeof(int)); 
    vetor_aux[0] = fim-inicio;
    vetor_aux[1] = inicio;
    for(int i = 0; i < fim;i++) vetor_aux[2+i] = vetor[inicio+i];
    MPI_Send(vetor_aux, TAM+2, MPI_INT, rank-1, TAG_SEND_ESQ, MPI_COMM_WORLD);

    MPI_Recv(vetor_aux, TAM+2, MPI_INT, rank-1, TAG_SEND_DIR, MPI_COMM_WORLD, &status);

    for (int i = 0 ;i < vetor_aux[0]; i++) vetor[ vetor_aux[1] + i] = vetor_aux[i+2];
    
    order_vetor(vetor,vetor_aux[1],fim);      
     free(vetor_aux); 


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



int main(int argc, char** argv ){
    FILE *f = fopen("input.txt", "r");
    int *vetor = (int*) malloc(sizeof(int)*TAM);
    for(int i = 0 ; i < TAM;i++){
        int temp;
        fscanf(f,"%d", &temp);
        // printf("%d\n", temp);
        vetor[i] = temp;
    }
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
   double time_spent  = 0.0;
   clock_t begin = clock();
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int inicio = rank*(TAM/size);
    int tam_padrao = TAM/size;
    int fim = (rank+1)*(TAM/size);
    
    if (rank == size-1) fim += TAM % size;
    int delta = TAM/size;
    // printf("%d  %d\n", inicio, fim);
    /*
    ######################################################################################################
    */

    for (int interator_process = 0 ; interator_process < size; interator_process++){
        // printf("TESTE\n");
        if (interator_process % 2 == 1){
            if (rank % 2 == 1){ //rank impar
                if (rank < size-1)
                split_min(vetor,inicio,fim, rank);
            }
            else {
                if (rank > 0){
                    split_max(vetor,inicio,fim,rank);
                }
            }
        }
        else { // interador par
            if (rank % 2 == 0 ) {
                if (size == 1) order_vetor(vetor,inicio, fim);
                if (rank < size-1) split_min(vetor,inicio,fim,rank);
            }
            else {//rank impar
                split_max(vetor,inicio,fim, rank);
            }

        }


    }
    

    int *aux = (int*)malloc(sizeof(int)*TAM);
    if (rank==0){
        for(int i=0; i < size-1;i++){

            MPI_Recv(aux, TAM, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&status  );
            int rank_recv = status.MPI_SOURCE;
            int aux_rest = 0;
            if (rank_recv == size-1) aux_rest += TAM % size;
            for(int j=0;j < delta+aux_rest;j++) vetor[ j+delta*rank_recv ] = aux[j];
        }
    }
    else {
        for(int i = inicio; i < fim; i++)aux[i-inicio] = vetor[i];
        MPI_Send(aux,fim-inicio,MPI_INT,0, 15,MPI_COMM_WORLD);
    }
    


    MPI_Finalize();
    if (rank ==0 ){
        clock_t end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
        printf("The elapsed time is %f seconds", time_spent);
        // printf("%d %d %d\n", rank, inicio, fim);
        FILE * f = fopen("output.txt", "w");
        for (int i=  0 ; i < TAM; i++) fprintf(f, "%d  ", vetor[i]);
        fclose(f);
        // printf("\n");
    }
    return 0;

    // printf()
}