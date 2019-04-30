
#include <iostream>
#include "char_to_num.h"
#include"open_file_chick_buffer.h"

#define bufferSizeForLine 50

void FileToArray1D(FILE *inputFile, double *arr, int dataNum, int dataDim) {
	char *saveptr = NULL;
	char *buffer = (char*)malloc(bufferSizeForLine * dataDim);
	chickBuffer(buffer);
	fgets(buffer, bufferSizeForLine * dataDim, inputFile);
	saveptr = buffer;
	for (int j = 0; j < dataDim; j++) {
		arr[j] = CharToDouble(&saveptr);
	}
}

void FileToArray2D_int(FILE *inputFile, int **arr, int dataNum, int dataDim) {
	char *saveptr = NULL;
	char *buffer = (char*)malloc(bufferSizeForLine*10000);
	chickBuffer(buffer);
	for (int i = 0; i < dataNum; i++) {
		fgets(buffer, bufferSizeForLine*10000, inputFile);
		saveptr = buffer;
		for (int j = 0; j < dataDim; j++) {
			arr[i][j] = CharToIntOnly(&saveptr);//¥[³t
		}
	}

}

void FileToArray2D_double(FILE *inputFile, double **arr, int dataNum, int dataDim) {
	char *saveptr = NULL;
	char *buffer = (char*)malloc(bufferSizeForLine*dataDim);
	chickBuffer(buffer);
	for (int i = 0; i < dataNum; i++) {
		fgets(buffer, bufferSizeForLine*dataDim, inputFile);
		saveptr = buffer;
		for (int j = 0; j < dataDim; j++) {
			arr[i][j] = CharToDouble(&saveptr);
		}
	}
}

void file_to_array_2D_uchar(FILE *inputFile, unsigned char **arr, int dataNum, int dataDim) {//0~255
	char *saveptr = NULL;
	char *buffer = (char*)malloc(bufferSizeForLine*dataDim);
	chickBuffer(buffer);
	for (int i = 0; i < dataNum; i++) {
		fgets(buffer, bufferSizeForLine*dataDim, inputFile);
		saveptr = buffer;
		for (int j = 0; j < dataDim; j++) {
			arr[i][j] = CharToDouble(&saveptr);
		}
	}
}
