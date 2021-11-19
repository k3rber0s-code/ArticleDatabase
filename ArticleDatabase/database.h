#pragma once
#include <string>
#include <vector>
#include <map>

class ArticleProcessing {
public:

	typedef	std::map<std::string, std::vector<std::string>> Database;
	typedef std::pair<std::string, std::vector<std::string>> Entry; // TODO - Keyword database Entry: vector<str(ID), vector(LIST OF ARTICLES)<vector(ID, vyskyt[0], vyskyt[1]...)<str>>>


	void ProcessArticle(std::string& article, std::string& id, std::string& title);
	void AddKeyword(std::string& article, std::string& id, std::string& title);
	bool IsDelimiter(int c) { return !isalpha(c); };
	void FindWord(std::string word);

	ArticleProcessing(){
		KeywordDatabase = CreateDatabase();
		ArticleDatabase = CreateDatabase();
	};
private:
	Database KeywordDatabase;
	Database ArticleDatabase;
	std::map<std::string, std::vector<std::string>> CreateDatabase();
};