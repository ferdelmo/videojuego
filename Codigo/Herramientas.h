#ifndef HERRAMIENTAS_H_
#define HERRAMIENTAS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string leerStringFromFIle(string path) {
	ifstream inFile;
	inFile.open(path);//open the input file

	stringstream strStream;
	strStream << inFile.rdbuf();//read the file
	string str = strStream.str();//str holds the content of the file

	return str;
}

#endif