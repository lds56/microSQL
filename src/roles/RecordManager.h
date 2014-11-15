#ifndef RECORDMANAGER_H_
#define RECORDMANAGER_H_

#include<vector>
#include<string>
#include"models/table.h"
#include"models/Condition.h"
#include<map>

using namespace std;

class RecordManager {
public:
	bool createTable(TableInfo tableInfo);   //create file: *.table
	bool dropTable(TableInfo tableInfo);  //delete file: *.table
	vector<TableRowPtr> select(TableInfo tableInfo);
	vector<TableRowPtr> select(TableInfo tableInfo, vector<Condition> conds);
//	vector<TableRowPtr> select(vector<Address> addresses);
	bool insert(TableInfo tableInfo, vector<string> data);
    bool del(TableInfo tableInfo);
    //bool delete(string tableName);
    bool del(TableInfo tableInfo, vector<Condition> conds);
	//int delete(vector<Address> addresses); 
};

#endif