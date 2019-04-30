#pragma once
#include<iostream>
#include <string>
using namespace std;

FILE *ChickOpenFile(FILE *&file, char*floder, string fileName, const char* mode) {
	string url;
	url = floder + fileName;
	//file = fopen(Url.c_str(), mode);
	//while (file == NULL) {
	while (fopen_s(&file, url.c_str(), mode)!=0) {
		fputs(url.c_str(), stderr);
		fputs("\nfile missing, place chicking input floder, press any key when ready\n", stderr);
		//file = fopen(Url.c_str(), mode);
		system("pause");
	}
	return file;
}

void chickBuffer(char *buffer) {
	if (buffer == NULL) {
		fputs("Memory error", stderr);
		system("pause");
		exit(0);
	}
}