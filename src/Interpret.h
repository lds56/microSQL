#ifndef _interpret_h
#define _interpret_h

#include<iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "Macro.h"
#include "Struct.h"
#include "roles/CatalogManager.h"
#include "API.h"

using namespace std;

class Interpret {
public:
	Interpret();
	~Interpret();

	int isEnd(char *input);
	void parsing(char* command);
	void initColumn();
	void initCondition();
	void initValue();
	void initTable(){
		getTableInfo.tableName="";
		getTableInfo.fieldNum=getTableInfo.rowSize=0;
		getTableInfo.fields.clear();
	}
	void initIndex(){
		getIndexInfo.blockNum=getIndexInfo.column=-1;
		getIndexInfo.indexName="";
	}
	bool getWord(string & src, string & des);
	bool getStr(string & src, string & des);

	short int isInt(const char *);
	short int isFloat(char *input);

	void exec();

    vector<string> convert(Row r);

public:
	int operation;
	string tableName;
	string indexName;
	string colName;
	vector<Field> column;
	vector<Status> condition;
	//TableRowPtr tableRow;
    Row row;
	TableInfo getTableInfo;
	Index getIndexInfo;
	int primaryKey;
	int unique;
	vector<Status> uniqueCondition;
    API api;
};

#endif
