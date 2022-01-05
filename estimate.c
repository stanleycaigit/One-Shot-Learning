#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void transpose(int rows, int cols, double** X, double** tX){
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            tX[j][i] = X[i][j];
        }
    }
}

void multiply(int rows1, int cols1, int rows2, int cols2, double** X, double** secondX, double** productX){  
    //initialize product matrix with 0.0
    for(int i = 0; i < rows1; ++i){
        for(int j = 0; j < cols2; ++j){
            productX[i][j] = 0.0;
        }
    }

    for(int i = 0; i < rows1; ++i){
        for(int j = 0; j < cols2; ++j){
            for(int k = 0; k < cols1; ++k){
                productX[i][j] += (X[i][k] * secondX[k][j]);
            }
        }
    }
}

void inverse(int dimension, double** pX, double** iX){
    for(int p = 0; p < dimension; p++){
        double f = pX[p][p];
        for(int a = 0; a < dimension; a++){
            pX[p][a] = pX[p][a] / f;
            iX[p][a] = iX[p][a] / f;
        }
        for(int i = p+1; i < dimension; i++){
            f = pX[i][p];
            for(int a = 0; a < dimension; a++){
                double subtract = f * pX[p][a];
                pX[i][a] = pX[i][a] - subtract;

                double subtract2 = f * iX[p][a];
                iX[i][a] = iX[i][a] - subtract2;
            }
        }
    }

    for(int p = dimension-1; p >= 0; p--){
        for(int i = p-1; i >= 0; i--){
            double f = pX[i][p];
            for(int a = 0; a < dimension; a++){
                double subtract = f * pX[p][a];
                pX[i][a] = pX[i][a] - subtract;

                double subtract2 = f * iX[p][a];
                iX[i][a] = iX[i][a] - subtract2;
            }
        }
    }
}

int main(int argc, char **argv){

    //read training values
    FILE *training = fopen(argv[1], "r");
    if(training == NULL){
        return 1;
    }

    //store first word "train" of file ???
    char train[6];
    fscanf(training, "%s", train);

    //get number of attributes
    int attr;
    fscanf(training, "%d", &attr);
    attr++;

    //get number of houses
    int houses;
    fscanf(training, "%d", &houses);

    //allocate 2d arrays X and Y
    double **X, **Y;
    X = (double **)malloc(sizeof(double *) * houses);
    X[0] = (double *)malloc(sizeof(double) * houses * attr);
    for(int i = 1; i < houses; ++i){
        X[i] = X[i-1] + attr;
    }
    Y = (double **)malloc(sizeof(double *) * houses);
    Y[0] = (double *)malloc(sizeof(double) * houses * 1);
    for(int i = 1; i < houses; ++i){
        Y[i] = Y[i-1] + 1;
    }

    //fill Matrix X and Y with values
    for(int i = 0; i < houses; ++i){
        X[i][0] = 1.0;
        for(int j = 1; j < attr; ++j){
            fscanf(training, "%lf", &X[i][j]);
        }
        fscanf(training, "%lf", &Y[i][0]);
    }

    fclose(training);

    //find transpose of Matrix X
    double **tX;
    tX = (double **)malloc(sizeof(double *) * attr);
    tX[0] = (double *)malloc(sizeof(double) * attr * houses);
    for(int i = 1; i < attr; ++i){
        tX[i] = tX[i-1] + houses;
    }
    transpose(houses, attr, X, tX);

    //multiply Matrix X with transposed of Matrix X
    double **pX;
    pX = (double **)malloc(sizeof(double *) * attr);
    pX[0] = (double *)malloc(sizeof(double) * attr * attr);
    for(int i = 1; i < attr; ++i){
        pX[i] = pX[i-1] + attr;
    }
    multiply(attr, houses, houses, attr, tX, X, pX);

    //find inverse of productX Matrix

    //make identity Matrix
    double **iX;
    iX = (double **)malloc(sizeof(double *) * attr);
    iX[0] = (double *)malloc(sizeof(double) * attr * attr);
    for(int i = 1; i < attr; ++i){
        iX[i] = iX[i-1] + attr;
    }
    for(int i = 0; i < attr; ++i){
        for(int j = 0; j < attr; ++j){
            if(i == j){
                iX[i][j] = 1.0;
            }else{
                iX[i][j] = 0.0;
            }
        }
    }

    inverse(attr, pX, iX);
    free(pX[0]);
    free(pX);

    //multiply inverse with transpose of X
    double **pX2;
    pX2 = (double **)malloc(sizeof(double *) * attr);
    pX2[0] = (double *)malloc(sizeof(double) * attr * houses);
    for(int i = 1; i < attr; ++i){
        pX2[i] = pX2[i-1] + houses;
    }
    multiply(attr, attr, attr, houses, iX, tX, pX2);
    free(tX[0]);
    free(tX);
    free(iX[0]);
    free(iX);

    //multiply remainder of matrix (pX2) with y to get W
    double **W;
    W = (double **)malloc(sizeof(double *) * attr);
    W[0] = (double *)malloc(sizeof(double) * attr * 1);
    for(int i = 1; i < attr; ++i){
        W[i] = W[i-1] + 1;
    }
    multiply(attr, houses, houses, 1, pX2, Y, W);
    free(pX2[0]);
    free(pX2);
    free(X[0]);
    free(X);
    free(Y[0]);
    free(Y);

    //read data values
    FILE *data = fopen(argv[2], "r");
    if(data == NULL){
        return 1;
    }

    //store first word "data" of file ???
    char dataWord[5];
    fscanf(data, "%s", dataWord);

    //get number of attributes
    int attr2;
    fscanf(data, "%d", &attr2);
    attr2++;

    if(attr != attr2){
        return 1;
    }

    //get number of houses
    int houses2;
    fscanf(data, "%d", &houses2);

    //read in values of data
    double **X2;
    X2 = (double **)malloc(sizeof(double *) * houses2);
    X2[0] = (double *)malloc(sizeof(double) * houses2 * attr2);
    for(int i = 1; i < houses2; ++i){
        X2[i] = X2[i-1] + attr2;
    }
    for(int i = 0; i < houses2; ++i){
        X2[i][0] = 1.0;
        for(int j = 1; j < attr2; ++j){
            fscanf(data, "%lf", &X2[i][j]);
        }
    }
    fclose(data);

    //multiply X2 with W to get Y
    double **prices;
    prices = (double **)malloc(sizeof(double *) * houses2);
    prices[0] = (double *)malloc(sizeof(double) * houses2 * 1);
    for(int i = 1; i < houses2; ++i){
        prices[i] = prices[i-1] + 1;
    }
    multiply(houses2, attr2, attr, 1, X2, W, prices);
    free(X2[0]);
    free(X2);
    free(W[0]);
    free(W);

    for(int i = 0; i < houses2; ++i){
        printf("%.0f\n", prices[i][0]);
    }
    free(prices[0]);
    free(prices);

    return 0;
}
