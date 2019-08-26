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
    int i, j, k, l, b1, b2, b3, b4, b5, b6, b7, b8;

    //For M=N=32
    if(M == 32 && N == 32){
        for(k = 0; k < 4; k++){
            for(l = 0; l < 4; l++){
                if(k == l){
                    //In this case, we experience the diagonal problem
                    for(i = 0; i < 8; i++){
                        //Load values into buffers
                        b1 = A[8*k + i][8*k];
                        b2 = A[8*k + i][8*k + 1];
                        b3 = A[8*k + i][8*k + 2];
                        b4 = A[8*k + i][8*k + 3];
                        b5 = A[8*k + i][8*k + 4];
                        b6 = A[8*k + i][8*k + 5];
                        b7 = A[8*k + i][8*k + 6];
                        b8 = A[8*k + i][8*k + 7];
                        //Transfer into B
                        B[8*k][8*k + i] = b1;
                        B[8*k + 1][8*k + i] = b2;
                        B[8*k + 2][8*k + i] = b3;
                        B[8*k + 3][8*k + i] = b4;
                        B[8*k + 4][8*k + i] = b5;
                        B[8*k + 5][8*k + i] = b6;
                        B[8*k + 6][8*k + i] = b7;
                        B[8*k + 7][8*k + i] = b8;
                    }
                }
                else{
                    for(i = 0; i < 8; i++){
                        for(j = 0; j < 8; j++){
                            B[8*l + j][8*k + i] = A[8*k + i][8*l + j];
                        }
                    }
                }
            }
        }
    }

    //For M=N=64
    if (M == 64 && N == 64) {
	//SIMILAR to 32x32 matrix---> All the variables loaded into the cache utilized compeletely
	for (k = 0; k < M; k += 4) {
	    for (l = 0; l < N; l += 4) {
		for (i = l; i < l + 4; i++) {
		    for (j = k; j < k + 4; j++) {

			if (i != j)
			    B[j][i] = A[i][j];
			else {
			    b1=A[i][j];
				b2=i;
				b3=1; //Indicating that diagonal element is accessed
			}
		    }
            //Putting the diagonal elements in the B matrix after every diagonal 8x8 block access
			if(b3==1)
			B[b2][b2]=b1;
			b3=0;
		}
		
	    }
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
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char transpose_test_desc[] = "Transpose test";
void transpose_test(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l, b1, b2, b3, b4, b5, b6, b7, b8;

    //For M=N=32
    if(M == 32 && N == 32){
        for(k = 0; k < 4; k++){
            for(l = 0; l < 4; l++){
                if(k == l){
                    //In this case, we experience the diagonal problem
                    for(i = 0; i < 8; i++){
                        //Load values into buffers
                        b1 = A[8*k + i][8*k];
                        b2 = A[8*k + i][8*k + 1];
                        b3 = A[8*k + i][8*k + 2];
                        b4 = A[8*k + i][8*k + 3];
                        b5 = A[8*k + i][8*k + 4];
                        b6 = A[8*k + i][8*k + 5];
                        b7 = A[8*k + i][8*k + 6];
                        b8 = A[8*k + i][8*k + 7];
                        //Transfer into B
                        B[8*k][8*k + i] = b1;
                        B[8*k + 1][8*k + i] = b2;
                        B[8*k + 2][8*k + i] = b3;
                        B[8*k + 3][8*k + i] = b4;
                        B[8*k + 4][8*k + i] = b5;
                        B[8*k + 5][8*k + i] = b6;
                        B[8*k + 6][8*k + i] = b7;
                        B[8*k + 7][8*k + i] = b8;
                    }
                }
                else{
                    for(i = 0; i < 8; i++){
                        for(j = 0; j < 8; j++){
                            B[8*l + j][8*k + i] = A[8*k + i][8*l + j];
                        }
                    }
                }
            }
        }
    }

    //For M=N=64
    if(M == 64 && N == 64){
        for(k = 0; k < 8; k++){
	    for(l = 0; l < 8; l++){
                if(k == l){
                    //In this case, we experience the diagonal problem
                    for(i = 0; i < 2; i++){
                        //Load values into buffers
                        b1 = A[8*k + 2*i][8*l];
                        b2 = A[8*k + 2*i][8*l + 1];
                        b3 = A[8*k + 2*i][8*l + 2];
                        b4 = A[8*k + 2*i][8*l + 3];
			b5 = A[8*k + 2*i + 1][8*l];
			b6 = A[8*k + 2*i + 1][8*l + 1];
			b7 = A[8*k + 2*i + 1][8*l + 2];
			b8 = A[8*k + 2*i + 1][8*l + 3];
			//Transfer into B
                    	B[8*l][8*k + 2*i] = b1;
                    	B[8*l + 1][8*k + 2*i] = b2;
			B[8*l + 2][8*k + 2*i] = b3;
                    	B[8*l + 3][8*k + 2*i] = b4;
		    	B[8*l][8*k + 2*i + 1] = b5;
                    	B[8*l + 1][8*k + 2*i + 1] = b6;
			B[8*l + 2][8*k + 2*i + 1] = b7;
                    	B[8*l + 3][8*k + 2*i + 1] = b8;
		    }
		    for(i = 2; i < 4; i++){
                        //Load values into buffers
                        b1 = A[8*k + 2*i][8*l + 0];
                        b2 = A[8*k + 2*i][8*l + 1];
                        b3 = A[8*k + 2*i][8*l + 2];
                        b4 = A[8*k + 2*i][8*l + 3];
			b5 = A[8*k + 2*i + 1][8*l + 0];
			b6 = A[8*k + 2*i + 1][8*l + 1];
			b7 = A[8*k + 2*i + 1][8*l + 2];
			b8 = A[8*k + 2*i + 1][8*l + 3];
			//Transfer into B
                    	B[8*l + 0][8*k + 2*i] = b1;
                    	B[8*l + 1][8*k + 2*i] = b2;
			B[8*l + 2][8*k + 2*i] = b3;
                    	B[8*l + 3][8*k + 2*i] = b4;
		    	B[8*l + 0][8*k + 2*i + 1] = b5;
                    	B[8*l + 1][8*k + 2*i + 1] = b6;
			B[8*l + 2][8*k + 2*i + 1] = b7;
                    	B[8*l + 3][8*k + 2*i + 1] = b8;
		    }
 		    for(i = 2; i < 4; i++){
                        //Load values into buffers
                        b1 = A[8*k + 2*i][8*l + 4];
                        b2 = A[8*k + 2*i][8*l + 5];
                        b3 = A[8*k + 2*i][8*l + 6];
                        b4 = A[8*k + 2*i][8*l + 7];
			b5 = A[8*k + 2*i + 1][8*l + 4];
			b6 = A[8*k + 2*i + 1][8*l + 5];
			b7 = A[8*k + 2*i + 1][8*l + 6];
			b8 = A[8*k + 2*i + 1][8*l + 7];
			//Transfer into B
                    	B[8*l + 4][8*k + 2*i] = b1;
                    	B[8*l + 5][8*k + 2*i] = b2;
			B[8*l + 6][8*k + 2*i] = b3;
                    	B[8*l + 7][8*k + 2*i] = b4;
		    	B[8*l + 4][8*k + 2*i + 1] = b5;
                    	B[8*l + 5][8*k + 2*i + 1] = b6;
			B[8*l + 6][8*k + 2*i + 1] = b7;
                    	B[8*l + 7][8*k + 2*i + 1] = b8;
		    }
		    for(i = 0; i < 2; i++){
                        //Load values into buffers
                        b1 = A[8*k + 2*i][8*l + 4];
                        b2 = A[8*k + 2*i][8*l + 5];
                        b3 = A[8*k + 2*i][8*l + 6];
                        b4 = A[8*k + 2*i][8*l + 7];
			b5 = A[8*k + 2*i + 1][8*l + 4];
			b6 = A[8*k + 2*i + 1][8*l + 5];
			b7 = A[8*k + 2*i + 1][8*l + 6];
			b8 = A[8*k + 2*i + 1][8*l + 7];
			//Transfer into B
                    	B[8*l + 4][8*k + 2*i] = b1;
                    	B[8*l + 5][8*k + 2*i] = b2;
			B[8*l + 6][8*k + 2*i] = b3;
                    	B[8*l + 7][8*k + 2*i] = b4;
		    	B[8*l + 4][8*k + 2*i + 1] = b5;
                    	B[8*l + 5][8*k + 2*i + 1] = b6;
			B[8*l + 6][8*k + 2*i + 1] = b7;
                    	B[8*l + 7][8*k + 2*i + 1] = b8;
		    }

                }
                else{
                    for(i = 0; i < 4; i++){
                        for(j = 0; j < 4; j++){
                            B[8*l + j][8*k + i] = A[8*k + i][8*l + j];
                        }
                    }
		    for(i = 0; i < 4; i++){
                        for(j = 0; j < 4; j++){
                            B[8*l + 4 + j][8*k + i] = A[8*k + i][8*l + 4 + j];
                        }
                    }
		    for(i = 0; i < 4; i++){
                        for(j = 0; j < 4; j++){
                            B[8*l +4+ j][8*k +4+ i] = A[8*k +4+ i][8*l +4+ j];
                        }
                    }
		    for(i = 0; i < 4; i++){
                        for(j = 0; j < 4; j++){
                            B[8*l + j][8*k + 4+i] = A[8*k + 4+ i][8*l + j];
                        }
                    }
                }
            }
        }
    }

} 
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
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(transpose_test, transpose_test_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

