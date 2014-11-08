#include <boost/smart_ptr.hpp>
#include <iostream>
using namespace std;
using namespace boost;

typedef shared_ptr<Table> TablePtr;
typedef share_ptr<Index> IndexPtr;


class CatalogManager {
public:
	bool existsTable(String tableName);
  //  vector<TablePtr> getTable();
	vector<FieldType> getFileds(String tableName);
	vector<IndexPtr> getIndex(String tableName);
	int getFiledCount(String tableName);
	int getIndexCount(String tableName);
	int getRowLength(String tableName);
    String getPrimaryKey(String tableName);
	bool hasIndex(String tableName);
	
	bool isUnique(String tableName, String field);
	bool isPrimaryKey(String tableName, String field);
	//?
	
private:
	int fieldCount;
	int indexCount;
  
}
