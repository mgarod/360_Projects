#include <iostream>
#include <iomanip>
using namespace std;

const int L8 = 8;
const int K3 = 3;
const int R6 = 6;

void fillmatrix(int array[L8][L8]){
	int counter = 1;
	for(int i = 0; i < L8; i++)
		for(int j=0; j < L8; j++)
			array[i][j] = counter++;
}

void fillmatrix(int array[K3][K3]){
	int counter = 1;
	for(int i = 0; i < K3; i++)
		for(int j = 0; j < K3; j++)
			array[i][j] = counter++;
}

void printmatrix(int array[L8][L8]){
	for(int i = 0; i < L8; i++){
		for(int j = 0; j < L8; j++){
			cout << setw(6) << array[i][j];
		}
		cout << endl;
	}
}

void printmatrix(int array[K3][K3]){
	for(int i = 0; i < K3; i++){
		for(int j = 0; j < K3; j++){
			cout << setw(6) << array[i][j];
		}
		cout << endl;
	}
}

void printmatrix(int array[R6][R6]){
	for(int i = 0; i < R6; i++){
		for(int j = 0; j < R6; j++){
			cout << setw(6) << array[i][j];
		}
		cout << endl;
	}
}

int calc_space(int ai, int aj, int A[L8][L8], int K[K3][K3]){
	int sum = 0;
	for(int row = ai-1; row <= ai+1; row++){
		for (int col = aj-1; col <= aj+1; col++){
			//printf("Calculating a(%d,%d) * k(%d,%d)\n",
			//	row, col, row-ai+1, col-aj+1);
			sum += (A[row][col] * K[row-ai+1][col-aj+1]);
		}
	}
	return sum;
}

void convolution(int A[L8][L8], int K[K3][K3], int R[R6][R6]){
	for(int i = 1; i < L8-1; i++){
		for (int j = 1; j < L8-1; j++){
			//printf("Calculating R[%d][%d]\n", i-1, j-1);
			R[i-1][j-1] = calc_space(i, j, A, K);
		}
	}
}

int main(){
	int square[L8][L8];
	int kernel[K3][K3];// = { {0,0,1}, {0,0,0}, {0,0,0} };
	int result[R6][R6];

	printf("Input Matrix:\n");
	fillmatrix(square);
	printmatrix(square);

	printf("\nKernel Matrix\n");
	fillmatrix(kernel);
	printmatrix(kernel);

	printf("\nConvolution Matrix\n");
	convolution(square, kernel, result);
	printmatrix(result);
}

