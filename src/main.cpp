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
vector<string> getSplitString(const string &s, char delimiter) {
	vector<string> res;
	splitString(s, delimiter, res);
	return res;
}

/*
 *	Measures the difference between two strings using the levenshtein method.
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

double computeSimilarityRatio(string& s, string& t) {
	double res = 0;

	vector<string> sSplit = getSplitString(s, ' ');
	vector<string> tSplit = getSplitString(t, ' ');

	if (sSplit.size() < tSplit.size()) {
		vector<string> temp = tSplit;
		tSplit = sSplit;
		sSplit = temp;
	}

	vector<vector<int> > scores(sSplit.size(), vector<int>(tSplit.size()));
	vector<int> bestWord(sSplit.size());

	for (unsigned int i = 0; i < sSplit.size(); i++) {
		for (unsigned int j = 0; j < tSplit.size(); j++)
			scores[i][j] = 1000;
		bestWord[i] = -1;
	}
	int wordsMatched = 0;
	for (unsigned int i = 0; i < sSplit.size(); i++) {
		string temp1 = sSplit[i];
		for (unsigned int j = 0; j < tSplit.size(); j++) {
			string temp2 = tSplit[j];
			int levenshteinDistance = computeLevenshteinDistance(temp1, temp2);
			scores[i][j] = levenshteinDistance;
			if (bestWord[i] == -1
					|| scores[i][bestWord[i]] > levenshteinDistance)
				bestWord[i] = j;
		}
	}

	for (unsigned int i = 0; i < sSplit.size(); i++) {
		if (scores[i][bestWord[i]] == 1000)
			continue;
		for (unsigned int j = i + 1; j < sSplit.size(); j++) {
			if (scores[j][bestWord[j]] == 1000)
				continue; //the worst score available, so there are no more words left
			if (bestWord[i] == bestWord[j]) //2 words have the same best word
					{
				//The first in order has the advantage of keeping the word in equality
				if (scores[i][bestWord[i]] <= scores[j][bestWord[j]]) {
					scores[j][bestWord[j]] = 1000;
					int currentBest = -1;
					int currentScore = 1000;
					for (unsigned int k = 0; k < tSplit.size(); k++) {
						//Find next bestWord
						if (currentBest == -1 || currentScore > scores[j][k]) {
							currentBest = k;
							currentScore = scores[j][k];
						}
					}
					bestWord[j] = currentBest;
				} else //the latter has a better score
				{
					scores[i][bestWord[i]] = 1000;
					int currentBest = -1;
					int currentScore = 1000;
					for (unsigned int k = 0; k < tSplit.size(); k++) {
						//Find next bestWord
						if (currentBest == -1 || currentScore > scores[i][k]) {
							currentBest = k;
							currentScore = scores[i][k];
						}
					}
					bestWord[i] = currentBest;
				}

				i = -1;
				break; //recalculate all
			}
		}
	}
	for (unsigned int i = 0; i < sSplit.size(); i++) {
		if (scores[i][bestWord[i]] == 1000)
			res += sSplit[i].size(); //All words without a score for best word are max failures
		else {
			res += scores[i][bestWord[i]];
			if (scores[i][bestWord[i]] == 0)
				wordsMatched++;
		}
	}
	int thesize = 0;

	s.erase(remove(s.begin(), s.end(), ' '), s.end());
	t.erase(remove(t.begin(), t.end(), ' '), t.end());

	if (s.size() > t.size())
		s.size();
	else
		t.size();
	if (res > thesize)
		res = thesize;
	res = (1 - (res / thesize)) * 100;
	double wordsRatio = ((double) wordsMatched / (double) tSplit.size())
			* 100;
	double realWordsRatio = ((double) wordsMatched / (double) sSplit.size())
			* 100;
	return ;
}

int main() {
	string fileName = "", line = "", temp = "";
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
//	do {
//		cout << "Escreva o nome do ficheiro a comparar com a base de dados: ";
//		cin >> fileName;
//		testFile.open(fileName.c_str());
//		if (!testFile.fail()) {
//			cout << "Ficheiro aberto com sucesso!" << endl << endl;
//		} else {
//			cout << "Ficheiro n�o existe!" << endl << endl;
//		}
//	} while (testFile.fail());

	fileName = "teste.txt";
	testFile.open(fileName.c_str());

//reads file to test and copies to a string
	if (testFile.is_open()) {
		while (testFile.good()) {
			getline(testFile, line);
			testFileString << line << endl;
		}
	}
	testFileContent = testFileString.str();

//reads file database and stores it in a vector and copies each file's content to a stringstream vector
	for (unsigned int i = 0; i < databaseFileNames.size(); i++) {
		databaseFile.open(databaseFileNames[i].c_str());
		if (!databaseFile.fail()) { //checks if file opened successfully
			while (databaseFile.good()) {
				getline(databaseFile, line);
				databaseFileString << line << endl;
			}
			databaseFileContent.push_back(databaseFileString.str());
		}
		databaseFile.close();
	}

	for (unsigned int i = 0; i < databaseFileNames.size(); i++) {
		temp = databaseFileContent[i];
		cout << "Similaridade de " << fileName << " com "
				<< databaseFileNames[i] << " - "
				<< computeSimilarityRatio(testFileContent, temp) << endl;
	}

}

