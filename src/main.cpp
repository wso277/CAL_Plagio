#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int computeLCS(string s,string t)
{
     if (t.length() > s.length())
     {
        swap(s,t);
     }

     int m = s.length();
     int n=t.length();
     vector< vector<int> > res(2, vector<int>(n+1,0));

     for (int i=1;i<=m;i++)
     {
         for (int j=1;j<=n;j++)
         {
             if (s[i-1]==t[j-1])
                res[1][j]=res[0][j-1]+1;
             else
                 res[1][j]=max(res[1][j-1],res[0][j]);
         }
         for (int j=1;j<=n;j++)
             res[0][j]=res[1][j];
     }

     return (res[1][n]);
}

double computeSimilarity(string& s, string& t)
{
  unsigned int sSize = s.size();
  unsigned int tSize = t.size();
  double res;

  if (sSize == 0 && tSize == 0)
  {
    // Two empty strings are equal
    res = 1;
  }
  else
  {
    unsigned int size;
	string ss;
	ss = s+s;

    size = computeLCS(ss, t);
    if (size > sSize)
	{
		size = sSize;
	}

    size *= 2;

    if (size == sSize + tSize && strcmp(s.c_str(), t.c_str()) != 0)
    {
    	size--;
    }

    // Get the final measure of the similarity
    res = (double)size / (sSize + tSize);
  }

  return res;
}

int main() {
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

//	databaseFileNames.push_back("db1/f1.txt");
//	databaseFileNames.push_back("db1/f2.txt");
//	databaseFileNames.push_back("db1/f3.txt");
//	databaseFileNames.push_back("db1/f4.txt");
//	databaseFileNames.push_back("db1/f5.txt");
//	databaseFileNames.push_back("db1/f6.txt");
//	databaseFileNames.push_back("db1/f7.txt");
//	databaseFileNames.push_back("db1/f8.txt");
//	databaseFileNames.push_back("db1/f9.txt");
//	databaseFileNames.push_back("db1/f10.txt");

	databaseFileNames.push_back("db2/f1.txt");
	databaseFileNames.push_back("db2/f2.txt");
	databaseFileNames.push_back("db2/f3.txt");
	databaseFileNames.push_back("db2/f4.txt");

//chooses the file to be tested
//	do {
//		cout << "Escreva o nome do ficheiro a comparar com a base de dados: ";
//		cin >> fileName;
//		testFile.open(fileName.c_str());
//		if (!testFile.fail()) {
//			cout << "Ficheiro aberto com sucesso!" << endl << endl;
//		} else {
//			cout << "Ficheiro nao existe!" << endl << endl;
//		}
//	} while (testFile.fail());

	fileName = "teste_texto.txt";
	testFile.open(fileName.c_str());

//reads file to test and copies to a string
	if (testFile.is_open()) {
		while (testFile.good()) {
			getline(testFile, line);
			testFileString << line << " ";
		}
	}
	testFileContent = testFileString.str();
	cout << testFileContent << endl << endl;

//reads file database and stores it in a vector and copies each file's content to a stringstream vector
	for (unsigned int i = 0; i < databaseFileNames.size(); i++) {
		databaseFile.open(databaseFileNames[i].c_str());
		if (!databaseFile.fail()) { //checks if file opened successfully
			while (databaseFile.good()) {
				getline(databaseFile, line);
				databaseFileString << line << " ";
			}
			databaseFileContent.push_back(databaseFileString.str());
		}
		databaseFileString.str(string());
		cout << databaseFileContent[i] << endl;
		databaseFile.close();
	}

	for (unsigned int i = 0; i < databaseFileNames.size(); i++) {
		temp1 = testFileContent;
		temp2 = databaseFileContent[i];
		cout << "Similaridade de " << fileName << " com "
				<< databaseFileNames[i] << " - "
				<< computeSimilarity(temp1, temp2)*100 << "%" << endl;
	}

}

