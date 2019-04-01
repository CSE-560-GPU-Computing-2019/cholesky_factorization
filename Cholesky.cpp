#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib> 
using namespace std; 
  
const int MAX = 100; 
  
void Cholesky_CPU(int matrix[][MAX],int n){
    int low[n][n];
    memset(low, 0, sizeof(low));
    for (int i = 0; i < n; i++){ 
        for (int j = 0; j <= i; j++){
            int sum = 0; 
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
}

int main(){
	srand(time(0));
    double totalTime = 0;
    int n = 10; 
    cout << n + "\n";
    int matrix1[MAX][MAX];
    int matrix2[MAX][MAX];
    for(int a = 0; a<1000000; a++){
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
        int matrix[MAX][MAX];
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                for(int k = 0; k < n; ++k){
                    matrix[i][j] += matrix1[i][k] * matrix2[k][j];
                }
        /*for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) 
                cout << setw(6) << matrix2[i][j] << "\t";  
            cout << endl;
            }
            cout << "__________________________________________________\n";
        for (int i = 0; i < n; i++) { 
            for (int j = 0; j < n; j++) 
                cout << setw(6) << matrix1[i][j] << "\t";  
            cout << endl;
            }
            cout << "__________________________________________________\n";*/
        
        //for (int i = 0; i < n; i++) { 
          //  for (int j = 0; j < n; j++) 
            //    cout << setw(6) << matrix[i][j] << "\t";  
            //cout << endl;
            //}
           // cout << "__________________________________________________\n";
        
        clock_t begin = clock();
    	Cholesky_CPU(matrix,n);
        clock_t end = clock();
        totalTime+= double(end - begin) / double CLOCKS_PER_SEC;
    }
    cout << totalTime;
    cout << endl;
    return 0; 
}