#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

/**
 *	Splits a string according to the delimiter parameter, which defines the string delimiter, and stores them in a vector<string> - if they aren't empty -
 passed as the parameter elems.
 */
vector<string> splitString(const string &s, char delimiter,
		vector<string> &res) {
	stringstream ss(s);
	string stringTokens;
	while (getline(ss, stringTokens, delimiter)) {
		//checks if the string isn't empty
		if (stringTokens != " ") {
			res.push_back(stringTokens);
		}
	}
	return res;
}

/**
 *	Returns the split string as a vector<string>.
 */
vector<string> getSplittedString(const string &s, char delimiter) {
	vector<string> res;
	splitString(s, delimiter, res);
	return res;
}

/*
 *	Measures the difference between two strings using the Levenshtein method.
 */
int computeLevenshteinDistance(const string& s, const string& t) {
	int n = s.size();
	int m = t.size();

	vector<vector<int> > d(n + 1, vector<int>(m + 1));

	if (s.empty()) {
		return t.size();
	}

	if (t.empty()) {
		return s.size();
	}

	for (int i = 0; i <= n; ++i) {
		d[i][0] = i;
	}

	for (int j = 0; j <= m; ++j) {
		d[0][j] = j;
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; j++) {
			int cost = s[i - 1] == t[j - 1] ? 0 : 1;
			d[i][j] = min(d[i - 1][j] + 1,
					min(d[i][j - 1] + 1, d[i - 1][j - 1] + cost));
		}
	}
	return d[n][m];
}

double computeSimilarityRatio(const string& s, const string& t) {
	double res = 0;

	vector<string> sSplitted = getSplittedString(s, ' ');
	vector<string> tSplitted = getSplittedString(t, ' ');

	if (sSplitted.size() < tSplitted.size()) {
		vector<string> temp = tSplitted;
		tSplitted = sSplitted;
		sSplitted = temp;
	}

	vector<vector<int> > scores(sSplitted.size(),
			vector<int>(tSplitted.size()));
	vector<int> bestWord(sSplitted.size());

	return res;
}

int main() {
	string fileName = "", line = "";
	//file to be tested for plagiarism
	ifstream testFile;
	//temporary databaseFile reader
	ifstream databaseFile;
	//tested file content and temporary database file content
	stringstream testFileString, databaseFileString;
	//database file names
	vector<string> databaseFileNames;
	//database contente
	vector<string> databaseFileContent;


	databaseFileNames.push_back("db/f1.txt");
	databaseFileNames.push_back("db/f2.txt");
	databaseFileNames.push_back("db/f3.txt");
	databaseFileNames.push_back("db/f4.txt");
	databaseFileNames.push_back("db/f5.txt");
	databaseFileNames.push_back("db/f6.txt");
	databaseFileNames.push_back("db/f7.txt");
	databaseFileNames.push_back("db/f8.txt");
	databaseFileNames.push_back("db/f9.txt");
	databaseFileNames.push_back("db/f10.txt");

	//chooses the file to be tested
	do {
		cout << "Escreva o nome do ficheiro a comparar com a base de dados: ";
		cin >> fileName;
		testFile.open(fileName.c_str());
		if (!testFile.fail()) {
			cout << "Ficheiro aberto com sucesso!" << endl << endl;
		} else {
			cout << "Ficheiro não existe!" << endl << endl;
		}
	} while (testFile.fail());

	//reads file to test and copies to a string
	if (testFile.is_open()) {
		while (testFile.good()) {
			getline(testFile, line);
			testFileString << line << endl;
		}
	}

//reads file database and stores it in a vector and copies each file's content to a stringstream vector
	for (unsigned int i = 0; i < databaseFileNames.size(); i++) {
		databaseFile.open(databaseFileNames[i].c_str());
		if (!databaseFile.fail()) {//checks if file opened successfully
			while (databaseFile.good()) {
				getline(databaseFile, line);
				databaseFileString << line << endl;
			}
			databaseFileContent.push_back(databaseFileString.str());
		}
		databaseFile.close();
		cout << databaseFileContent[i] << endl;
	}
}

