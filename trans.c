// Li Guoxiang lgx 2018202135
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */


#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    int x1, x2, x3, x4, x5, x6, x7, x8;
    if (M == 32 && N == 32)
    {
        for (j = 0; j < 32; j += 8)
        {
            for (i = 0; i < 32; i++)
            {
                x1 = A[i][0 + j];
                x2 = A[i][1 + j];
                x3 = A[i][2 + j];
                x4 = A[i][3 + j];
                x5 = A[i][4 + j];
                x6 = A[i][5 + j];
                x7 = A[i][6 + j];
                x8 = A[i][7 + j];
                B[0 + j][i] = x1;
                B[1 + j][i] = x2;
                B[2 + j][i] = x3;
                B[3 + j][i] = x4;
                B[4 + j][i] = x5;
                B[5 + j][i] = x6;
                B[6 + j][i] = x7;
                B[7 + j][i] = x8;
            }
        }
    }
    else if (M == 64 && N == 64)
    {
        for (i = 0; i < N; i += 8)
        {
            for (j = 0; j < M; j += 8)
            {
                for (k = 0; k < 4; k++)
                {
                    x1 = A[i + k][j + 0];
                    x2 = A[i + k][j + 1];
                    x3 = A[i + k][j + 2];
                    x4 = A[i + k][j + 3];
                    x5 = A[i + k][j + 4];
                    x6 = A[i + k][j + 5];
                    x7 = A[i + k][j + 6];
                    x8 = A[i + k][j + 7];

                    B[j + 0][i + k] = x1;
                    B[j + 1][i + k] = x2;
                    B[j + 2][i + k] = x3;
                    B[j + 3][i + k] = x4;

                    B[j + 0][i + k + 4] = x5;
                    B[j + 1][i + k + 4] = x6;
                    B[j + 2][i + k + 4] = x7;
                    B[j + 3][i + k + 4] = x8;
                }
                for (k = 0; k < 4; k++)
                {
                    x1 = B[j + k][i + 4];
                    x2 = B[j + k][i + 5];
                    x3 = B[j + k][i + 6];
                    x4 = B[j + k][i + 7];

                    x5 = A[i + 4][j + k];
                    x6 = A[i + 5][j + k];
                    x7 = A[i + 6][j + k];
                    x8 = A[i + 7][j + k];

                    B[j + k][i + 4] = x5;
                    B[j + k][i + 5] = x6;
                    B[j + k][i + 6] = x7;
                    B[j + k][i + 7] = x8;

                    B[j + 4 + k][i + 0] = x1;
                    B[j + 4 + k][i + 1] = x2;
                    B[j + 4 + k][i + 2] = x3;
                    B[j + 4 + k][i + 3] = x4;
                }
                for (k = 4; k < 8; k++)
                {
                    for (l = 4; l < 8; l++)
                    {
                       B[j + l][i + k] = A[i + k][j + l];
                    }
                }
            }
        }
    }
    else if (M == 61 && N == 67)
    {
        for(i = 0; i + 8 < N; i += 8){
            for(j = 0; j + 8 < M; j += 8){
                for(k = 0; k < 8; k ++){
                    x1 = A[i + k][j + 0];
                    x2 = A[i + k][j + 1];
                    x3 = A[i + k][j + 2];
                    x4 = A[i + k][j + 3];
                    x5 = A[i + k][j + 4];
                    x6 = A[i + k][j + 5];
                    x7 = A[i + k][j + 6];
                    x8 = A[i + k][j + 7];

                    B[j + 0][i + k] = x1;
                    B[j + 1][i + k] = x2;
                    B[j + 2][i + k] = x3;
                    B[j + 3][i + k] = x4;
                    B[j + 4][i + k] = x5;
                    B[j + 5][i + k] = x6;
                    B[j + 6][i + k] = x7;
                    B[j + 7][i + k] = x8;
                }
            }
        }
        for (l = 0; l + 8 < M; l += 8)
        {
            for (k = i; k < N; k++)
            {
                x1 = A[k][l + 0];
                x2 = A[k][l + 1];
                x3 = A[k][l + 2];
                x4 = A[k][l + 3];
                x5 = A[k][l + 4];
                x6 = A[k][l + 5];
                x7 = A[k][l + 6];
                x8 = A[k][l + 7];

                B[l + 0][k] = x1;
                B[l + 1][k] = x2;
                B[l + 2][k] = x3;
                B[l + 3][k] = x4;
                B[l + 4][k] = x5;
                B[l + 5][k] = x6;
                B[l + 6][k] = x7;
                B[l + 7][k] = x8;
            }
        }
        for (k = 0; k < N; k++)
        { // handle the rest
            x1 = A[k][56];
            x2 = A[k][57];
            x3 = A[k][58];
            x4 = A[k][59];
            x5 = A[k][60];

            B[56][k] = x1;
            B[57][k] = x2;
            B[58][k] = x3;
            B[59][k] = x4;
            B[60][k] = x5;
        }
    }
    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
// char trans_desc[] = "Simple row-wise scan transpose";
// void trans(int M, int N, int A[N][M], int B[M][N])
// {
// }



/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
