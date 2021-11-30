#include "database.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>


bool cmp(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
	bool x = (a.first.compare(b.first) < 0);

	return x;
}
std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
void ArticleProcessing::LoadData(std::istream& sourceStream, outputDatabaseType type) {
	if (type == outputDatabaseType::load) {
		std::string input;
		std::getline(sourceStream, input);
		while (input.size() != 0) {
			std::vector<std::string> data = {};
			data.push_back(input);
			std::getline(sourceStream, input);
			data.push_back(input);
			std::getline(sourceStream, input);
			data.push_back(input);
			articlesData.push_back(data);
			data.clear();
			input.clear();
			std::getline(sourceStream, input);
		}
	}
	else if (type == outputDatabaseType::search) {
		std::string input;
		std::getline(sourceStream, input);
		while (input.size() != 0) {
			queries.push_back(input);
			input.clear();
			std::getline(sourceStream, input);
		}
	}
}
void ArticleProcessing::ProcessAllArticles() {
	for (auto&& x : articlesData) {
		ProcessArticle(x[2], x[0], x[1]);
	}

}
void ArticleProcessing::ProcessArticle(std::string& article, std::string& id, std::string& title)
{
	std::vector<std::string> entryData = { title, article };
	ArticleDatabaseEntry e(id, entryData);
	ArticleDb.insert(e);
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

			KeywordDatabase::iterator i = KeywordDb.find(token);

			if (i == KeywordDb.end()) {
				std::pair<std::string, int> articleIdAndPosition(id, counter - token.length());
				std::vector<std::pair<std::string, int>> tokenData = { articleIdAndPosition };
				KeywordDb.insert(KeywordDatabaseEntry(token, tokenData));
			}
			else {
				std::pair<std::string, int> articleIdAndPosition(id, counter - token.length());
				auto it = std::find_if(KeywordDb[token].begin(), KeywordDb[token].end(),
					[id](const std::pair<std::string, int>& element) { return element.first == id;});
				if (it == KeywordDb[token].end()) {
					KeywordDb[token].push_back(articleIdAndPosition); // TODO - vyskyt nekolikrat ve stejnem clanku pridava id vickrat?
				}
			}
			token = "";
		}
		else {
			token += (char)c;
		}
		++counter;
	}
	//std::cout << token << counter - token.length() << article[counter - token.length()] << std::endl;
	KeywordDatabase::iterator i = KeywordDb.find(token);
	if (i == KeywordDb.end()) {
		std::pair<std::string, int> articleIdAndPosition(id, counter - token.length());
		std::vector<std::pair<std::string, int>> tokenData = { articleIdAndPosition };
		KeywordDb.insert(KeywordDatabaseEntry(token, tokenData));
	}
	else {
		std::pair<std::string, int> articleIdAndPosition(id, counter - token.length());
		KeywordDb[token].push_back(articleIdAndPosition);
	}
}
void ArticleProcessing::SortDatabase() {
	for (auto&& x : KeywordDb) {
		std::sort(x.second.begin(), x.second.end(), cmp);
	}
}
bool ArticleProcessing::SearchQuery(std::string& query)
{
	std::vector<std::string> keywords = GetKeywordsFromQuery(query);
	if (keywords.size() == 0) return false;

	std::vector<std::vector< std::pair<std::string, int>>> vectorsToCross = {};
	for (auto&& keyword : keywords) {
		vectorsToCross.push_back(KeywordDb[keyword]);
	}
	std::vector< std::pair<std::string, int>> intersection = IntersectVectors(vectorsToCross);
	if (intersection.size() == 0) {
		return false;
	}
	for (auto&& x : intersection) {
		int minimumIndex = GetMinimumIndex(x.first, x.second, { keywords[0] });

		if (x.second > minimumIndex) {
			x.second = minimumIndex;
		}
		GetArticleSnippet(x.first, x.second);
	}
	std::cout << std::endl;
	return true;
}
void ArticleProcessing::SearchAllQueries()
{
	for (auto&& q : queries) {
		if (!SearchQuery(q)) {
			std::cout << "No results" << std::endl;
			std::cout << std::endl;
		}
	}
}
int ArticleProcessing::GetMinimumIndex(std::string& articleId, int currentMin, std::vector<std::string> keywords) {
	for (auto&& x : keywords) {
		auto it = std::find_if(KeywordDb[x].begin(), KeywordDb[x].end(),
			[articleId](const std::pair<std::string, int>& element) { return element.first == articleId;});
		if (it != KeywordDb[x].end()) {
			if (it->second < currentMin) {
				currentMin = it->second;
			}
		}
	}
	return currentMin;
}


std::vector< std::pair<std::string, int>> ArticleProcessing::IntersectVectors(std::vector<std::vector< std::pair<std::string, int>>>& vecs) {

	auto last_intersection = vecs[0];
	std::vector< std::pair<std::string, int>> curr_intersection;

	for (std::size_t i = 1; i < vecs.size(); ++i) {
		std::set_intersection(last_intersection.begin(), last_intersection.end(),
			vecs[i].begin(), vecs[i].end(),
			std::inserter(curr_intersection, curr_intersection.begin()), cmp);
		std::swap(last_intersection, curr_intersection);
		curr_intersection.clear();
	}
	return last_intersection;
}

std::vector<std::string> ArticleProcessing::GetKeywordsFromQuery(std::string& query)
{
	int counter = 0;
	std::string token;
	std::stringstream tokenStream(query);
	std::vector<std::string> tokens = std::vector<std::string>();

	while (counter != query.size() + 1)
	{
		int c = tokenStream.get();

		if (IsDelimiter(c)) {
			if (token.size() != 0) {

				std::transform(token.begin(), token.end(), token.begin(),
					[](unsigned char c) { return std::tolower(c); });

				KeywordDatabase::iterator i = KeywordDb.find(token);

				if (i == KeywordDb.end()) {
					return std::vector<std::string>();
				}
				else {
					tokens.push_back(token);
				}
				token = "";
			}
		}
		else {
			token += (char)c;
		}
		++counter;
	}
	return tokens;
}

bool ArticleProcessing::GetArticleSnippet(std::string& articleId, const int startingPosition)
{
	const unsigned BUFFERSIZE = 80;

	std::string title = ArticleDb[articleId][0];
	std::string articleText = ArticleDb[articleId][1];
	std::stringstream ss(articleText);

	char buffer[BUFFERSIZE];

	ss.seekg(startingPosition);
	ss.getline(buffer, 76);

	std::string result(buffer);
	result += "...";

	std::cout << "[" << articleId << "]" << " " << title << std::endl;
	std::cout << result << std::endl;
	return true;
}

std::map<std::string, std::vector<std::string>> ArticleProcessing::CreateArticleDatabase() {
	return 	std::map<std::string, std::vector<std::string>>();
}
std::vector<std::vector<std::string>> ArticleProcessing::CreateDataset()
{
	return std::vector<std::vector<std::string>>();
}
std::vector<std::string> ArticleProcessing::CreateQueriesDataset()
{
	return std::vector<std::string>();
}
std::map<std::string, std::vector<std::pair< std::string, int >>> ArticleProcessing::CreateKeywordDatabase() {
	return std::map<std::string, std::vector<std::pair< std::string, int >>>();
}