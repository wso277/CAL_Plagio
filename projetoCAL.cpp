#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

/**
*	Splits a string according to the delimiter parameter, which defines the string delimiter, and stores them in a vector<string> - if they aren't empty -
	passed as the parameter elems.
*/
vector<string> splitString(const string &s, char delimiter, vector<string> &res)
{
    stringstream ss(s);
    string stringTokens;
    while (getline(ss, stringTokens, delimiter))
	{
    	//checks if the string isn't empty
		if (stringTokens != " ")
		{
			res.push_back(stringTokens);
		}
    }
    return res;
}


/**
*	Returns the splitted string as a vector<string>.
*/
vector<string> getSplittedString(const string &s, char delimiter)
{
    vector<string> res;
    splitString(s, delimiter, res);
    return res;
}

/*
*	Measures the difference between two strings using the Levenshtein method.
*/
int computeLevenshteinDistance( const string& s, const string& t)
{
  int n = s.size();
  int m = t.size();

  vector< vector<int> > d(n+1, vector<int>(m+1));

  if(s.empty())
  {
	return t.size();
  }

  if(t.empty())
  {
	return s.size();
  }

  for(int i = 0; i <= n; ++i)
  {
	d[i][0] = i;
  }

  for(int j = 0; j <= m; ++j)
  {
	d[0][j] = j;
  }

  for(int i = 1 ; i <= n ; ++i)
  {
      for (int j = 1; j <= m; j++)
      {
        int cost = s[ i - 1 ] == t[ j - 1 ] ? 0 : 1;
        d[i][j] = min(d[i-1][j] + 1,
                       min(d[i][j-1] + 1, d[i-1][j-1] + cost ));
      }
   }
   // TODO: minimum cost path(s) from d[0][0] to d[n][m]
   // gives the required alignment(s) between the two strings
   // a character by character comparison of the aligned
   // strings gives the difference between the two strings
   return d[n][m] ;
}

double computeSimilarityRatio(const string& s, const string& t)
{
	double res = 0;

	vector<string> sSplitted = getSplittedString(s,' ');
	vector<string> tSplitted = getSplittedString(t,' ');

	if (sSplitted.size() < tSplitted.size())
	{
		vector<string> temp = tSplitted;
		tSplitted = sSplitted;
		sSplitted = temp;
	}

	return res;
}

