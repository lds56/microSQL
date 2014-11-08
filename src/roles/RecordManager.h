#include<vector>
#include<string>
#include"models/table.h"
#include<map>

using namespace std;
class RecordManager {
public:
	bool createTable(string tableName);   //create file: *.table
	bool dropTable(string tableName);  //delete file: *.table
	vector<TableRowPtr> select(string tableName);
	vector<TableRowPtr> select(string tableName, Condition cond);
//	vector<TableRowPtr> select(vector<Address> addresses);
	bool insert(string tableName, vector<String> data);
	bool delete(string tableName);
	bool delete(string tableName, Condtion cond);
	//int delete(vector<Address> addresses); 
}

