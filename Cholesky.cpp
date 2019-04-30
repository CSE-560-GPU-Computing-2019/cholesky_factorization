#include <bits/stdc++.h>
#include <ctime>
#include <iostream>
#include <cstdlib> 
using namespace std; 
  
const int MAX = 100; 
  
void Cholesky_CPU(double matrix[MAX][MAX],int n){
    double low[n][n];
    memset(low, 0, sizeof(low));
    for (int i = 0; i < n; i++){ 
        for (int j = 0; j <= i; j++){
            double sum = 0; 
            if (j == i){
                for (int k = 0; k < j; k++)
                    sum += pow(low[j][k], 2);
                low[j][j] = sqrt(matrix[j][j] - sum);
            }
            else {
                for (int k = 0; k < j; k++)
                    sum += (low[i][k] * low[j][k]);
                low[i][j] = (matrix[i][j] - sum) / low[j][j];
            }
        }
    }
	//for(int i=0; i<n; i++){
	//	for(int j=0; j<n; j++)
	//		cout<<low[i][j]<<" ";
	//	cout<<endl;
	//}
}

void Cholesky_CPU_optimized(double matrix[MAX][MAX], int n){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(j<i){
			matrix[i][j]=0;
			continue;
			}
			int sum = 0;
			for(int k=0; k<i; k++){
				sum += matrix[k][i]*matrix[k][j];
			}
			if(i==j)
				matrix[i][j] = sqrt(matrix[i][j]-sum);
			else
				matrix[i][j] = (matrix[i][j]-sum)/matrix[i][i];
		}
	}

	//for(int i=0; i<n; i++){
	//	for(int j=0; j<n; j++)
	//		cout<<matrix[j][i]<<" ";
	//	cout<<endl;
	//}
}

int main(){
	srand(time(0));
    double totalTime = 0;
    int n = 100; 
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
        clock_t begin = clock();
    	//Cholesky_CPU(matrix,n);
    	Cholesky_CPU_optimized(matrix,n);
        clock_t end = clock();
        totalTime+= (float)(end - begin) / CLOCKS_PER_SEC;
    }
    cout << totalTime;
    cout << endl;
    return 0; 
}
