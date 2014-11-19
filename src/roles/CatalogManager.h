#ifndef __CatalogManager_h__
#define __CatalogManager_h__
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Macro.h"
#include "Struct.h"
#include "models/TableInfo.h"

class CatalogManager{
public:
	vector<TableInfo> tables;
	int tableCount;          //the number of tables in the database, should be equels to tables.size()
	vector<Index> indexes;                	//�����б�
	int indexCount;                   	//������Ŀ
	void initTable();
	
	void initIndex();
	
	void saveTable();

	void saveIndex();

public: 
	CatalogManager();
	
	~CatalogManager();

	void createTable(TableInfo& table);

	void createIndex(Index index);
		
	void dropTable(TableInfo table){
		dropTable(table.tableName);
	}
	
	void dropIndex(Index index){
		dropIndex(index.indexName);
	}
	
	void dropTable(string tablename);
	
	void dropIndex(string index_name);
	
	void update(TableInfo& tableinfor);

	void update(Index& index);
	
	bool existTable(string tablename);
	
	bool existIndex(string tablename, int column);

	bool existIndex(string indexname);

	TableInfo getTable(string tablename);
	
	Index getIndex(string tablename, int column);

	Index getIndex(string indexName);

	void display();
	
	void displayTable();
	
	void displayIndex();

	int getColNum(TableInfo& tableinfo,string columnname);

	int getColAmo(TableInfo& tableinfo);
};
#endif
