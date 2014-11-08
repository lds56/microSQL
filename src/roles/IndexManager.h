#include<vector.h>
#include<string.h>

using namespace std;

class IndexManager {
public:
	bool createIndex(String indexName, String tableName, Field field);
	bool dropIndex(String indexName, String tableName, Field field);
	bool insert(String tableName, vector<String> values);
	bool delete(String indexName, vector<Condition> conditions);
	vector<TableRowPtr> select(String indexName, vector<Condition> conditions);
	
}
