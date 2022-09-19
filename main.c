#include <stdio.h>
#include <mpi.h>
#define TAM 12
#define TAG_SEND_MAX 11
#define TAG_SEND_MIN 12

int vetor[TAM] = {9,8,7,6,5,4,3,2,1,0,-1,-2};

int comparare_exchage(int *vetor,int i, int j){
    int temp;
    if (vetor[i] > vetor[j]){
        temp = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = temp;
        return 1;
    }
    else return 0;
}

void split_min(int vetor, int inicio, int fim,int rank){
    MPI_Send(&vetor[inicio], 1,MPI_INT, rank-1, TAG_SEND_MIN, MPI_COMM_WORLD);
}


void split_max(int vetor, int inicio, int fim, int rank){}




void order_vetor(int *vetor, int inicio, int fim){
    for (int i= inicio ; i < fim; i++)
        for (int j=  i ; j < fim; j++)
            comparare_exchage(vetor,i,j);
    
}
int main(int argc, char** argv ){


    /*
    ##################################################################################################
                  ___ _   _ ___ ____ ___    _    _     ___ _____   _    ____    _    ___                                 
                 |_ _| \ | |_ _/ ___|_ _|  / \  | |   |_ _|__  /  / \  / ___|  / \  / _ \ 
                  | ||  \| || | |    | |  / _ \ | |    | |  / /  / _ \| |     / _ \| | | |
                  | || |\  || | |___ | | / ___ \| |___ | | / /_ / ___ \ |___ / ___ \ |_| |
                 |___|_| \_|___\____|___/_/   \_\_____|___/____/_/   \_\____/_/   \_\___/ 
    #################################################################################################
    */
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    /*
    ######################################################################################################
    */
    MPI_Status status;
    int inicio = rank*(TAM/size);
    int fim = (rank+1)*(TAM/size);


    for (int interator_process = 0 ; interator_process < size; interator_process++){
        if (interator_process % 2 == 1){
            if (rank % 2 == 1)
        }

    }
    
    
    for (int n = 0 ; n < TAM/size; n++){
        order_vetor(vetor, inicio,fim);
            
        // primeiro processo
        if (rank == 0){
            MPI_Send(&vetor[fim-1], 1,MPI_INT, rank+1, TAG_SEND_MAX, MPI_COMM_WORLD);
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, rank+1, TAG_SEND_MIN,MPI_COMM_WORLD, &status);
            vetor[fim-1] = temp;
        }

        // ultimo processo
        else{
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, rank-1, TAG_SEND_MAX,MPI_COMM_WORLD, &status);
            MPI_Send(&vetor[inicio], 1,MPI_INT, rank-1, TAG_SEND_MIN, MPI_COMM_WORLD);
            vetor[inicio] = temp;
        }

        // for (int i= inicio ; i < fim; i++)
        //     for (int j=  i ; j < fim; j++)
        //      comparare_exchage(vetor,i,j);
       
    
    }
    MPI_Finalize();
    if (rank ==0 )
        for (int i=  0 ; i < TAM; i++) printf("%d\n", vetor[i]);
    return 0;
}