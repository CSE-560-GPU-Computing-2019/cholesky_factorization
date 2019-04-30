#include <bits/stdc++.h>
#include <ctime>
#include <iostream>
#include <cstdlib> 
using namespace std; 
  
const int MAX = 100; 
 

__global__ void Cholesky_GPU(double *matrix, int n){
	
	//n threads running in parallel

	//int x = blockIdx.x;
	int y = threadIdx.x;
	//int i = x;
	int j = y;
	
	extern __device__ __shared__ double localMatrix[];
//	extern __device__ __shared__ double sum[];
	//matrix2d[x][y] = matrix1d[x*n+y]	
	
	//Copy to shared mem

	for(int i=0; i<n; i++)
		localMatrix[i*n+j] = matrix[i*n+j];

	localMatrix[n*n+j] = 0; // sum column
	__syncthreads();

	//Do the calc;
	#pragma unroll
	for(int i=0; i<n; i++){
		if(j<i){
			localMatrix[i*n+j] = 0;
		}
		if(j>=i) {
			localMatrix[n*n+j]=0;//initialize sum to 0
			for(int k=0; k<i; k++)
				localMatrix[n*n+j] +=localMatrix[k*n+i]*localMatrix[k*n+j]; // sums
			//if(j<i){
			//	localMatrix[i*n+j]=0;
			//}
			if(i == j){
				localMatrix[i*n+j] = sqrt(localMatrix[i*n+j] - localMatrix[n*n+j]);
			}if(j > i){
				localMatrix[i*n+j] = (localMatrix[i*n+j] - localMatrix[n*n+j])/localMatrix[i*n+i];
			}
		}
	}


	__syncthreads();

	
	for(int i=0; i<n; i++)
		matrix[i*n+j] = localMatrix[i*n+j];
	//Copy back



}

int main(){
	srand(time(0));
    float totalTime = 0;
    int n = 100;  //Matrix size
    //cout << n + "\n";
    double matrix1[MAX][MAX];
    double matrix2[MAX][MAX];
    for(int a = 0; a<5000; a++){
    	for(int j = 0; j<n; j++){
    		for(int k = 0; k<j+1; k++){
    			matrix1[k][j] = (rand()%100) + 1;
                matrix2[j][k] = matrix1[k][j];
    			//cout << matrix[j][k] + "\n";
    		}
            for(int k = 0; k<j; k++){
                matrix1[j][k] = 0;
                matrix2[k][j] = 0;
            }
    	}
		/*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) 
                cout << setw(6) << matrix1[i][j] << " ";  
            cout << endl;
            }
            cout << "__________________________________________________\n";
        for (int i = 0; i < n; i++) { 
            for (int j = 0; j < n; j++) 
                cout << setw(6) << matrix2[i][j] << " ";  
            cout << endl;
            }
            cout << "__________________________________________________\n";
        */
		double matrix[MAX][MAX];
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                for(int k = 0; k < n; ++k){
                    //cout << matrix1[i][k]<<"*"<<matrix2[k][j]<<"+";
					matrix[i][j] += matrix2[i][k] * matrix1[k][j];
                }
				//cout<<endl;
			}
		}
		//matrix={{2,-1,0},{-1,2,0},{0,-1,2}};
        /*
        for (int i = 0; i < n; i++) { 
          for (int j = 0; j < n; j++) 
            cout << setw(6) << matrix[i][j] << " ";  
        cout << endl;
        }
        cout << "__________________________________________________\n";
        */
		double *matrix1d = (double*)malloc(n*n*sizeof(double));
		
        for (int i = 0; i < n; i++) { 
        	for (int j = 0; j < n; j++){
        		matrix1d[i*n+j] = matrix[i][j];  
		}
        //cout << endl;
        }
		/*
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				cout<<matrix1d[i*n+j] << "  ";
		}
		cout<<endl;
		*/
		//Cuda 
		float time;
		cudaEvent_t start_kernel, stop_kernel;
		cudaEventCreate(&start_kernel);
		cudaEventCreate(&stop_kernel);
		//cudaEventRecord(start_kernel, 0);

		double *dev_matrix ;
		cudaMalloc(&dev_matrix,n*n*sizeof(double));
		cudaMemcpy(dev_matrix, matrix1d, n*n*sizeof(double), cudaMemcpyHostToDevice);

		dim3 dimGrid(1,1,1);
		dim3 dimBlock(n,1,1);
		
		cudaEventRecord(start_kernel,0);
		Cholesky_GPU<<<dimGrid, dimBlock, n*(n+1)>>>(dev_matrix, n);
		cudaEventRecord(stop_kernel,0);
		cudaEventSynchronize(stop_kernel);
		cudaEventElapsedTime(&time, start_kernel, stop_kernel);	
		cudaDeviceSynchronize();
		totalTime+=time;
		cudaMemcpy(matrix1d, dev_matrix, n*n*sizeof(double), cudaMemcpyDeviceToHost);
		/*
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				cout<<matrix1d[i*n+j]<<" ";
			}
			cout<<endl;
		}
		*/

    	//Cholesky_CPU(matrix,n);
    	//Cholesky_CPU_optimized(matrix,n);
    }
	totalTime=totalTime/1000.0;
    printf("%f\n", totalTime);
	//cout << totalTime;
    //cout << endl;
    return 0; 
}
