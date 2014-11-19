#include<vector>
#include<string>
#include "models/Address.h"
#include "models/Condition.h"
//#include "models/TableRow.h"
#include "BTree.h"
#include "struct.h"

using namespace std;

class IndexManager {
public:
    IndexManager() {}
	void createIndex(Index indexInfo, vector<string> svector, vector<Address> avector);
	void dropIndex(Index indexInfo);
	void insert(string key, Address address);
	vector<Address> select(vector<Condition> cond);
	vector<Address> dele(vector<Condition> cond);
	//vector<TableRowPtr> select(Index& indexInfo, vector<Condition> conditions);

public:
    BTree bTree;
};
