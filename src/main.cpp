#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


#define MAX_SIZE 2000

unsigned long int L[MAX_SIZE][MAX_SIZE];

string computeLCS(string s, string t) {

	if (s.size() > t.size())
		swap(s, t);

	for (unsigned long int i = 0; i < s.size(); i++)
		for (unsigned long int j = 0; j < t.size(); j++)
			L[i][j] = 0;
	unsigned long int z = 0;

	string ret;

	for (unsigned long int i = 0; i < s.size(); i++) {
		for (unsigned long int j = 0; j < t.size(); j++) {
			if (s[i] == t[j]) {
				if (i == 0 || j == 0) {
					L[i][j] = 1;
				} else {
					L[i][j] = L[i - 1][j - 1] + 1;
				}

				if (L[i][j] > z) {
					z = L[i][j];
					ret = s.substr(i - z + 1, z);
				} else if (L[i][j] == z) {
					ret += s.substr(i - z + 1, z);
				} else {
					L[i][j] = 0;
				}
			}
		}
	}
	return ret;
}

double computeSimilarity(string& s, string& t) {
	unsigned long int sSize = s.size();
	unsigned long int tSize = t.size();
	double res;

	if (sSize == 0 && tSize == 0) {
// Two empty strings are equal
		res = 1;
	} else {
		string resS;

		resS = computeLCS(s, t);

		res = resS.size();

// Get the final measure of the similarity
		res = (double) 2 * res / (sSize + tSize);

	}

	return res;
}

double divideText(string& s, string& t) {

	vector<string> sDiv;
	vector<string> tDiv;
	int sSize = s.size();
	int tSize = t.size();
	unsigned long int i = 0;
	double limSimilarity = 0;
	double finalSimilarity = 0;


	while (sSize > MAX_SIZE) {
		sDiv.push_back(s.substr(i, MAX_SIZE));
		i += MAX_SIZE;
		sSize-=MAX_SIZE;
	}

	sDiv.push_back(s.substr(i,MAX_SIZE));

	i = 0;

	while (tSize > MAX_SIZE) {
		tDiv.push_back(t.substr(i, MAX_SIZE));
		i += MAX_SIZE;
		tSize-=MAX_SIZE;
	}

	tDiv.push_back(s.substr(i,MAX_SIZE));
	unsigned int j;
	unsigned int k;
	for (j= 0; j < sDiv.size(); j++) {
		for (k = 0; k < tDiv.size(); k++) {
			limSimilarity += computeSimilarity(sDiv[j], tDiv[k]);
		}

	}

	finalSimilarity = limSimilarity/(j*k);

	return finalSimilarity;

}

int main() {
	int temp;
	string fileName = "", line = "", temp1 = "", temp2 = "";
//file to be tested for plagiarism
	ifstream testFile;
//temporary databaseFile reader
	ifstream databaseFile;
//tested file content and temporary database file content
	stringstream testFileString, databaseFileString;
//database file names
	vector<string> databaseFileNames;
//database content
	vector<string> databaseFileContent;
//file content
	string testFileContent;
//maximum similarity
	double maxSimilarity;
//similarity limit
	double limSimilarity;
//if a file exceeds maximum similarity
	bool exceeds = false;

	do {
		cout
				<< "Escolha a base de dados para o teste:\n1 - Textos com séries de números\n2 - Textos sobre Batatas, Guitarras e Nvidia Titan\n\nOpcao: ";
		getline(cin, temp1);
		temp = atoi(temp1.c_str());
		if (temp < 1 || temp > 2) {
			cout << "Opcao invalida!" << endl << endl;
		}
	} while (temp < 1 || temp > 2);

	if (temp == 1) {
		databaseFileNames.push_back("db1/f1.txt");
		databaseFileNames.push_back("db1/f2.txt");
		databaseFileNames.push_back("db1/f3.txt");
		databaseFileNames.push_back("db1/f4.txt");
		databaseFileNames.push_back("db1/f5.txt");
		databaseFileNames.push_back("db1/f6.txt");
		databaseFileNames.push_back("db1/f7.txt");
		databaseFileNames.push_back("db1/f8.txt");
		databaseFileNames.push_back("db1/f9.txt");
		databaseFileNames.push_back("db1/f10.txt");
	} else {
		databaseFileNames.push_back("db2/f1.txt");
		databaseFileNames.push_back("db2/f2.txt");
		databaseFileNames.push_back("db2/f3.txt");
		databaseFileNames.push_back("db2/f4.txt");
		databaseFileNames.push_back("db2/f5.txt");
	}

//chooses the file to be tested
	cout << endl << endl;
	do {
		cout << "Escreva o nome do ficheiro a comparar com a base de dados: ";
		cin >> fileName;
		testFile.open(fileName.c_str());
		if (!testFile.fail()) {
			cout << "Ficheiro aberto com sucesso!" << endl << endl;
		} else {
			cout << "Ficheiro nao existe!" << endl << endl;
		}
	} while (testFile.fail());

//reads file to test and copies to a string
	if (testFile.is_open()) {
		while (testFile.good()) {
			getline(testFile, line);
			temp1 = line[line.length() - 1];
			line.erase(line.end() - 1, line.end());
			testFileString << line;
		}
		testFileString << temp1;
	}
	testFileContent = testFileString.str();
	for (unsigned long int i = 0; i < testFileContent.length(); i++) {
		if (testFileContent[i] == ' ' || testFileContent[i] == '\n'
				|| testFileContent[i] == '\r\n')
				{
			testFileContent.erase(i, 1);
		}
	}

//chooses the maximum similarity before it's considered plagiarism
	do {
		cout << "Escreva o grau maximo de similaridade (1-100): ";
		cin >> maxSimilarity;
		if (maxSimilarity < 1 || maxSimilarity > 100) {
			printf("Insira um numero valido (1-100)!\n\n");
		}
	} while (maxSimilarity < 1 || maxSimilarity > 100);

//reads file database and stores it in a vector and copies each file's content to a stringstream vector

	for (unsigned long int i = 0; i < databaseFileNames.size(); i++) {
		databaseFile.open(databaseFileNames[i].c_str());
		if (!databaseFile.fail()) { //checks if file opened successfully
			while (databaseFile.good()) {
				getline(databaseFile, line);
				temp1 = line[line.length() - 1];
				line.erase(line.end() - 1, line.end());
				databaseFileString << line;
			}
			databaseFileString << temp1;
			databaseFileContent.push_back(databaseFileString.str());
		}
		for (unsigned long int j = 0; j < databaseFileContent[i].length();
				j++) {
			if (databaseFileContent[i][j] == ' '
					|| databaseFileContent[i][j] == '\n'
					|| databaseFileContent[i][j] == '\r\n')
					{
				databaseFileContent[i].erase(j, 1);
			}
		}
		databaseFileString.str(string());
		databaseFile.close();
	}

	for (unsigned long int k = 0; k < databaseFileNames.size(); k++) {
		temp1 = testFileContent;
		temp2 = databaseFileContent[k];
		cout << endl << databaseFileContent[k].size() << endl;
		cout << "Similaridade de " << fileName << " com "
				<< databaseFileNames[k] << " - ";

		if (temp1.size() > MAX_SIZE || temp2.size() > MAX_SIZE) {

			limSimilarity = divideText(temp1, temp2) * 100.0;

		} else {
			limSimilarity = computeSimilarity(temp1, temp2) * 100.0;

		}
		cout << limSimilarity << "% ";
		if (limSimilarity > maxSimilarity) {
			exceeds = true;
			cout << "- EXCEDE" << endl;
		} else {
			cout << "- NAO EXCEDE" << endl;
		}

	}

	if (exceeds) {
		cout << endl
				<< "O racio de semelhanca foi excedido. Este ficheiro pode ser plagio!"
				<< endl;
	} else {
		cout << endl
				<< "O racio de semelhanca nao foi excedido. Este ficheiro nao e plagio!"
				<< endl;
	}

}

