#include <iostream>
#include <vector>
#include <sstream>
#include "database.h"

int main()
{
	std::string input = "nUla sEst jedenact nula";
	std::string id = "ABC001";
	std::string name = "test";

	ArticleProcessing ap;
	ap.ProcessArticle(input, id, name);
	return 0;
}