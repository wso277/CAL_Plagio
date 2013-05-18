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

double computeSimilarityRatio( string& s,  string& t) {
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

	for (unsigned int i = 0; i < sSplitted.size(); i++) {
		for (unsigned int i1 = 0; i1 < tSplitted.size(); i1++)
			scores[i][i1] = 1000;
		bestWord[i] = -1;
	}
	int WordsMatched = 0;
	for (unsigned int i = 0; i < sSplitted.size(); i++) {
		string String1 = sSplitted[i];
		for (unsigned int i1 = 0; i1 < tSplitted.size(); i1++) {
			string String2 = tSplitted[i1];
			int LevenshteinDistance = computeLevenshteinDistance(String1,
					String2);
			scores[i][i1] = LevenshteinDistance;
			if (bestWord[i] == -1
					|| scores[i][bestWord[i]] > LevenshteinDistance)
				bestWord[i] = i1;
		}
	}

	for (unsigned int i = 0; i < sSplitted.size(); i++) {
		if (scores[i][bestWord[i]] == 1000)
			continue;
		for (unsigned int i1 = i + 1; i1 < sSplitted.size(); i1++) {
			if (scores[i1][bestWord[i1]] == 1000)
				continue; //the worst score available, so there are no more words left
			if (bestWord[i] == bestWord[i1]) //2 words have the same best word
					{
				//The first in order has the advantage of keeping the word in equality
				if (scores[i, bestWord[i]] <= scores[i1, bestWord[i1]]) {
					scores[i1][bestWord[i1]] = 1000;
					int CurrentBest = -1;
					int CurrentScore = 1000;
					for (int i2 = 0; i2 < tSplitted.size(); i2++) {
						//Find next bestWord
						if (CurrentBest == -1
								|| CurrentScore > scores[i1][i2]) {
							CurrentBest = i2;
							CurrentScore = scores[i1][i2];
						}
					}
					bestWord[i1] = CurrentBest;
				} else //the latter has a better score
				{
					scores[i][bestWord[i]] = 1000;
					int CurrentBest = -1;
					int CurrentScore = 1000;
					for (int i2 = 0; i2 < tSplitted.size(); i2++) {
						//Find next bestWord
						if (CurrentBest == -1 || CurrentScore > scores[i][i2]) {
							CurrentBest = i2;
							CurrentScore = scores[i][i2];
						}
					}
					bestWord[i] = CurrentBest;
				}

				i = -1;
				break; //recalculate all
			}
		}
	}
	for (unsigned int i = 0; i < sSplitted.size(); i++) {
		if (scores[i][bestWord[i]] == 1000)
			res += sSplitted[i].size(); //All words without a score for best word are max failures
		else {
			res += scores[i][bestWord[i]];
			if (scores[i][bestWord[i]] == 0)
				WordsMatched++;
		}
	}
	int thesize = 0;

	s.erase( remove( s.begin(), s.end(), ' ' ), s.end() );
	t.erase( remove( t.begin(), t.end(), ' ' ), t.end() );

	if (s.size() > t.size())
		s.size();
	else
		t.size();
	if (res > thesize)
		res = thesize;
	res = (1 - (res / thesize)) * 100;
	double wordsRatio = ((double) WordsMatched / (double) tSplitted.size())
			* 100;
	double realWordsRatio = ((double) WordsMatched / (double) sSplitted.size())
			* 100;
	return realWordsRatio;
}

int main() {
	/*string fileName = "", line = "";
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
			cout << "Ficheiro n�o existe!" << endl << endl;
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
		if (!databaseFile.fail()) { //checks if file opened successfully
			while (databaseFile.good()) {
				getline(databaseFile, line);
				databaseFileString << line << endl;
			}
			databaseFileContent.push_back(databaseFileString.str());
		}
		databaseFile.close();
		cout << databaseFileContent[i] << endl;
	}*/
	string s = "teste";
	string t = "teste vinte dez";
	cout << computeSimilarityRatio( s, t);
}

