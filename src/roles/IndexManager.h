#include<vector>
#include<string>
#include "models/Condition.h"
#include "models/TableRow.h"
#include "roles/BTree.h"
#include "models/Index.h"

using namespace std;

class IndexManager {
public:
    IndexManager(BTree bTree): BTree(bTree) {}
	bool createIndex(Index& indexInfo, string tableName, Field field);
	bool dropIndex(Index& indexInfo, string tableName, Field field);
	bool insert(Index& indexInfo, string key);
	bool del(Index& indexInfo, vector<Condition> conditions);
	vector<TableRowPtr> select(Index& indexInfo, vector<Condition> conditions);

private:
    BTree bTree;
};
