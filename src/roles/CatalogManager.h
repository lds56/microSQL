#include <boost/smart_ptr.hpp>
#include <iostream>
#include <string>
#include "models/Table.h"
#include "models/Index.h"

using namespace std;
using namespace boost;

//typedef shared_ptr<Table> TablePtr;
//typedef shared_ptr<Index> IndexPtr;


class CatalogManager {
public:
	bool existsTable(string tableName);
  //  vector<TablePtr> getTable();
	vector<FieldType> getFileds(string tableName);
	vector<IndexPtr> getIndex(string tableName);
	int getFiledCount(string tableName);
	int getIndexCount(string tableName);
	int getRowLength(string tableName);
    string getPrimaryKey(string tableName);
	bool hasIndex(string tableName);
	
	bool isUnique(string tableName, string field);
	bool isPrimaryKey(string tableName, string field);
	//?
	
private:
	int fieldCount;
	int indexCount;
  
}

