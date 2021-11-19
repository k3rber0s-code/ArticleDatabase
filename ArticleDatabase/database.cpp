#include "database.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

void ArticleProcessing::ProcessArticle(std::string& article, std::string& id, std::string& title)
{
	std::vector<std::string> entryData = { title, article };
	Entry e(id, entryData);
	ArticleDatabase.insert(e);
	AddKeyword(article, id, title);
}
void ArticleProcessing::AddKeyword(std::string& article, std::string& id, std::string& title)
{
	int counter = 0;
	std::string token;
	std::stringstream tokenStream(article);

	while (counter != article.size())
	{
		int c = tokenStream.get();

		if (IsDelimiter(c)) {
			//std::cout << token << counter - token.length() << article[counter - token.length()] << std::endl;
			std::transform(token.begin(), token.end(), token.begin(),
				[](unsigned char c) { return std::tolower(c); });

			Database::iterator i = KeywordDatabase.find(token);

			if (i == KeywordDatabase.end()) {
				std::vector<std::string> v = { id };
				KeywordDatabase.insert(Entry(token, v));
			}
			else {
				KeywordDatabase[token].push_back(id); // TODO - vyskyt nekolikrat ve stejnem clanku pridava id vickrat
			}
			token = "";
		}
		else {
			token += (char)c;
		}
		++counter;
	}
	//std::cout << token << counter - token.length() << article[counter - token.length()] << std::endl;
	Database::iterator i = KeywordDatabase.find(token);
	if (i == KeywordDatabase.end()) {
		std::vector<std::string> v = { id };
		KeywordDatabase.insert(Entry(token, v));
	}
	else {
		KeywordDatabase[token].push_back(id);
	}
}
std::map<std::string, std::vector<std::string>> ArticleProcessing::CreateDatabase()
{
	return std::map<std::string, std::vector<std::string>>();
}
