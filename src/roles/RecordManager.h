#include<vector>
#include<string>
#include"models/table.h"
#include<map>

using namespace std;

class RecordManager {
public:
	bool createTable(TableName tableName, map<string, FieldType> tableInfo);
	bool dropTable(TableName tableName);
	vector<TableRowPtr> select(TableName tableName, vector<Condition> conditions);
	vector<TableRowPtr> select(vector<Address> addresses);
	Address insert(TableName tableName, vector<String> data);
	int delete(TableName tableName, vector<Condition> conditions);
	int delete(vector<Address> addresses);  //Buffer manager?

}
