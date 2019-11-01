#include <iostream>
#include <cstring>
#include <unistd.h> 
#include <pthread.h>
using namespace std;

typedef struct param {
    int row;
    int col;
}parameters;

int sudoku[9][9];
int check[11];
int itr;

void* row(void *param){
    for(int r = 0; r < 9; r++){
        int array[10];
        for(int a=0; a<10; a++) array[a] = 0;
        for(int i = 0; i < 9; i++){
            int temp = sudoku[r][i];
            if(temp < 0 || temp > 9)   
                continue;
            array[temp]++;
            if(array[temp] > 1){
                check[0] = 1;
                itr++;
                return 0;
            }
        }
    }
    check[0]=1;
    itr++;
    return 0;
}


void* col(void *param){
    for(int c = 0; c < 9; c++){
        int array[10];
        for(int a=0; a<10; a++) array[a] = 0;
        for(int i = 0; i < 9; i++){
            int temp = sudoku[i][c];
            if(temp < 0 || temp > 9)   
                continue;
            array[temp]++;
            if(array[temp] > 1){
                check[1] = 0;
                itr++;
                return 0;
            }
        }    
        
    }
    check[1]=1;
    itr++;
    return 0;
}

void *subGrid(void *pointer){
    parameters *p_ = (parameters *)pointer;
    int array[10];
    for(int a=0; a<10; a++) array[a] = 0;
    for(int i=p_->row; i<p_->row+3; i++){
        for(int j=p_->col; j<p_->col+3; j++){
            array[sudoku[i][j]]++;
        }
    }
    for(int i=1; i<10; i++){
        if(array[i]>1) {
            check[itr++]=0;
            return 0;
        }
    }
    check[itr++]=1;
    return 0;
}

int main(){
    cout<<"Please Enter Sudoku in row order : \n";
    pthread_t tid[9];
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            cin>>sudoku[i][j];
        }
    }
    parameters *param = (parameters *)malloc(sizeof(parameters));
    param->row=0;
    param->col=0;
    pthread_create(&tid[0], NULL, col, NULL);
    pthread_create(&tid[1], NULL, row, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    for(int i=0; i<3; i++){
        param->row=0;
        param->col=3*i;
        pthread_create(&tid[i+1], NULL, subGrid, param);
    }
    for(int i=0; i<3; i++) pthread_join(tid[i+1], NULL);
    for(int i=0; i<3; i++){
        param->row=3;
        param->col=3*i;
        pthread_create(&tid[i+1], NULL, subGrid, param);
    }
    for(int i=0; i<3; i++) pthread_join(tid[i+1], NULL);
    for(int i=0; i<3; i++){
        param->row=6;
        param->col=3*i;
        pthread_create(&tid[i+1], NULL, subGrid, param);
    }
    for(int i=0; i<3; i++) pthread_join(tid[i+1], NULL);
    for(int i=0; i<11; i++){
        if(check[i]!=1) {
            cout<<"Incorrect"<<endl;
            return 0;
        }
    }
    cout<<"Correct"<<endl;
    return 0;
}