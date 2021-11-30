#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "database.h"

int main(int argc, char ** argv[])
{
	ArticleProcessing ap;
	//std::vector<std::string> arg(argv, argv + argc);
	std::vector<std::string> arg = { "1", "2" };


	if (arg.size() > 1) {
		std::ifstream f;
		//f.open(arg[1]);
		f.open("./a.txt");
		if (!f.good()) {
			return -1;
		}
		ap.LoadData(f, ArticleProcessing::outputDatabaseType::load);
	}
	ap.ProcessAllArticles();
	ap.SortDatabase();

	std::vector<std::string> queries = {};
	if (arg.size() == 2) { // TODO -> 3
		ap.LoadData(std::cin, ArticleProcessing::outputDatabaseType::search);
	}
	else {
		std::ifstream s;
		//f.open(arg[2]);
		s.open("./b.txt");
		if (!s.good()) {
			return -1;
		}
		ap.LoadData(s, ArticleProcessing::outputDatabaseType::search);
	}

	ap.SearchAllQueries();
	return 0;
}