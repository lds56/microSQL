#ifndef RECORDMANAGER_H_
#define RECORDMANAGER_H_

#include<vector>
#include<string>
#include<map>
#include"models/Condition.h"
#include"models/TableInfo.h"
#include"models/TableRow.h"
#include"models/Table.h"

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
    void registerTable(TableInfo tableInfo);
    void deregisterTable(TableInfo tableInfo);
    void commit();
private:
    vector<TableInfo> tables;
	//int delete(vector<Address> addresses); 
};

#endif