#include <vector>
#include <string>
#include "struct.h"
#include "roles/RecordManager.h"
#include "roles/CatalogManager.h"
//#include "roles/IndexManager.h"

extern CatalogManager catalog;

using namespace std;

class API{
public:
    API();
	void createTable(TableInfo tableInfo);
	void dropTable(TableInfo tableInfo);
    void insert(TableInfo tableInfo, vector<string> data);

    //void createIndex(Index indexinfo, TableInfo tableinfo);
	//void dropIndex(Index indexinfo);
	void select(TableInfo getTableInfo, vector<Status> cond);
    void select(TableInfo getTableInfo);
	//void select(ConditionReqeust request);
	/*void insert(TableInfo getTableInfo, TableRowPtr tableRow);
	//void del(FromRequest reqeuse); */
    void del(TableInfo getTableInfo, vector<Status> cond);
    void del(TableInfo getTableInfo);

    void del(TableInfo getTableInfo, vector<Condition> cond);
    string reconvert(string v);
    ~API();

public:
	//IndexManager indexManager;
	RecordManager recordManager;
};
