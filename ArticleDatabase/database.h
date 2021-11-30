#pragma queronce
#include <string>
#include <vector>
#include <map>
#include <iostream>
class ArticleProcessing {
public:

	typedef std::pair< std::string, std::vector<std::string >> ArticleDatabaseEntry;
	typedef std::pair<std::string, std::vector<std::pair< std::string, int >>> KeywordDatabaseEntry; // TODO - Keyword database Entry: vector<str(ID), vector(LIST OF ARTICLES)<vector(ID, vyskyt[0], vyskyt[1]...)<str>>>
	typedef	std::map<std::string, std::vector<std::pair< std::string, int >>> KeywordDatabase; // map<kw, <id, index>>
	typedef std::map<std::string, std::vector<std::string>> ArticleDatabase;

	enum outputDatabaseType { load, search };
	void LoadData(std::istream& sourcePath, outputDatabaseType type);

	void ProcessAllArticles();
	void ProcessArticle(std::string& article, std::string& id, std::string& title);
	void AddKeyword(std::string& article, std::string& id, std::string& title);

	void SortDatabase();

	bool IsDelimiter(int c) { return !isalpha(c); };
	int GetMinimumIndex(std::string& articleId, int curretnMin, std::vector<std::string> keywords);
	bool SearchQuery(std::string& query);
	void SearchAllQueries();

	std::vector<std::string> GetKeywordsFromQuery(std::string& query);
	std::vector< std::pair<std::string, int>> IntersectVectors(std::vector<std::vector< std::pair<std::string, int>>>& vecs);
	//bool Compare(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);
	bool GetArticleSnippet(std::string& articleId, int startingPosition);


	ArticleProcessing() {
		KeywordDb = CreateKeywordDatabase();
		ArticleDb = CreateArticleDatabase();
		articlesData = CreateDataset();
		queries = CreateQueriesDataset();
	};

private:
	KeywordDatabase KeywordDb;
	ArticleDatabase ArticleDb;
	std::vector<std::vector<std::string>> articlesData;
	std::vector<std::string> queries;


	std::vector<std::vector<std::string>> CreateDataset();
	std::vector<std::string> CreateQueriesDataset();
	std::map<std::string, std::vector<std::pair< std::string, int >>> CreateKeywordDatabase();
	std::map<std::string, std::vector<std::string>> CreateArticleDatabase();


};
