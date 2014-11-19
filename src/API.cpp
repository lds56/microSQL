#include "API.h"
#include "struct.h"
#include "models/Address.h"

using namespace std;

API::API() {
    for (int i=0; i<catalog.tables.size(); i++)
        recordManager.registerTable(catalog.tables[i]);
}

void API::createTable(TableInfo tableInfo){
    recordManager.createTable(tableInfo);
}

void API::dropTable(TableInfo tableInfo){
    recordManager.dropTable(tableInfo);
}

/*
void API::createIndex(Index indexinfo, TableInfo tableInfo){
	vector<string> svector;
	vector<Address> avector;
    vector<TableRowPtr> v = recordManager.select(tableInfo);
    int k;
    for (int i=0; i<tableInfo.fields.size(); i++)
        if (tableInfo.fields[i].isPrimeryKey) k = i;
    for (int i=0; i<v.size(); i++) {
        svector.push_back(v[i]->getValues()[k]);
        avector.push_back(v[i]->getAddr());
    }
	indexManager.createIndex(indexinfo, svector,  avector);
}
*/


/*

void API::dropIndex(Index indexinfo){
	indexManager.dropIndex(indexinfo);
}

*/

void API::insert(TableInfo tableInfo, vector<string> data){
    tableInfo.resetRowSize();
    //cout << "data: "<< endl;
    //for (int i=0; i<data.size(); i++)
    //    cout << data[i] << " ";
    //cout << endl;
    //cout << "----------" << endl;
    //cout << "size: " << tableInfo.rowSize << endl;
    //cout << "num: " << tableInfo.fieldNum << endl;
	recordManager.insert(tableInfo, data);
    /*
	if (table.index.IndexName!=""){
	indexManager.bTree.WriteToFile();
	indexManager.bTree.ReadFromFile(indexinfo);
	Address addres;
	string key;
	indexManager.insert(key, addres);
	}
	*/
}


void API::select(TableInfo tableInfo){
    tableInfo.resetRowSize();
    vector<Condition> conds;
    conds.push_back(Condition(true));
    vector<TableRowPtr> v = recordManager.select(tableInfo, conds);
    //cout << "vsize: " << v.size() << endl;
    //cout << "va: " <<  v[0]->getValues()[0] << endl;
    for (int j = 0; j < tableInfo.fields.size(); j++) {
        if (j != tableInfo.fields.size())
            cout << tableInfo.fields[j].fieldName << " | ";
        else cout << tableInfo.fields[j].fieldName;
    }
    cout << endl << "------------------------" << endl;
    //cout << "vs: " << v.size() << endl;
    for (int i=0; i<v.size(); i++) {
        //cout << "***";
        for (int j=0; j < tableInfo.fields.size(); j++) {
            cout << reconvert(v[i]->getValues()[j]);
            if (j != tableInfo.fields.size()) cout << " | ";
        }
        if (i!=v.size()) cout << endl;
    }
    cout << "------------------------" << endl;
    cout <<" ";
    //cout << "vsize: " << v.size() << endl;
}

void API::select(TableInfo tableInfo, vector<Status> condition){
    tableInfo.resetRowSize();
    vector<Condition> conds;
    if (condition.size() == 0) ;//del_conds.push_back(Condition(true));
    else {
        for (int i = 0; i < condition.size(); i++)
            conds.push_back(Condition(condition[i]));
    }
    vector<TableRowPtr> v = recordManager.select(tableInfo, conds);
    //vector<TableRowPtr> v = reconvert(v0);
    for (int j = 0; j < tableInfo.fields.size(); j++) {
        if (j != tableInfo.fields.size())
            cout << tableInfo.fields[j].fieldName << " | ";
        else cout << tableInfo.fields[j].fieldName;
    }
    cout << endl << "------------------------" << endl;
    //cout << "vs: " << v.size() << endl;
    for (int i=0; i<v.size(); i++) {
        //cout << "***";
        for (int j=0; j < tableInfo.fields.size(); j++) {
            cout << reconvert(v[i]->getValues()[j]);
            if (j != tableInfo.fields.size()) cout << " | ";
        }
        if (i!=v.size()) cout << endl;
    }
    cout << "------------------------" << endl;
}

void API::del(TableInfo tableInfo){
    tableInfo.resetRowSize();
    recordManager.del(tableInfo);
    //recordManager.commit();
}

void API::del(TableInfo tableInfo, vector<Status> condition){
    tableInfo.resetRowSize();
    /*
    if (table.index.IndexName!=""){
        indexManager.bTree.WriteToFile();
        indexManager.bTree.ReadFromFile(indexinfo);

        vector<Address> del(cond); // for recordManager
    } */
    vector<Condition> del_conds;
    if (condition.size() == 0) ;//del_conds.push_back(Condition(true));
    else {
        for (int i = 0; i < condition.size(); i++)
            del_conds.push_back(Condition(condition[i]));
    }
    recordManager.del(tableInfo, del_conds);
    //recordManager.commit();
}


void API::del(TableInfo tableInfo, vector<Condition> conds){
	/*
    if (table.index.IndexName!=""){
		indexManager.bTree.WriteToFile();
		indexManager.bTree.ReadFromFile(indexinfo);

		vector<Address> del(cond); // for recordManager
	} */
    recordManager.del(tableInfo, conds);
}

string API::reconvert(string s) {
    //vector<string> row;

    //for(int j=0;j<v.size();j++) {
        string temp;
        temp = s.substr(s.find_last_of('!') + 1);
        //row.push_back(temp);
    //}

    return temp;
}


API::~API()  {
    cout << "~API" << endl;
    recordManager.commit();
}


