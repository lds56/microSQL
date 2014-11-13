#include"roles/RecordManager.h"
#include"models/TableRow.h"
#include"models/Table.h"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include <tclDecls.h>

using namespace std;

bool RecordManager::createTable(string tableName) {
	ofstream file("data/" + tableName + ".table");
  	file.close();
}
	
bool RecordManager::dropTable(string tableName) {
    string fileName = "data/" + tableName + ".table";
	int ret_code = remove(fileName.data());
   	if (ret_code == 0) {
		cout << "File was successfully deleted\n";
	} else {
	   	cerr << "Error during the deletion: " << ret_code << '\n';
	}
}
	
vector<TableRowPtr> RecordManager::select(string tableName) {
	select(tableName, Condition("true"));
}
	
	//ensure fields!! 
	//vector<map<string, string>> select(string tableName, Condition cond) {
	//	select()
	//}
	
vector<TableRowPtr> RecordManager::select(string tableName, Condition cond) {
		TablePtr tPtr(new Table(tableName));
        vector<TableRowPtr> aVector;
		TableRowPtr rPtr = tPtr->getHead();
//        TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
		do {
			if (cond.check(rPtr) && !rPtr->isBlank()) aVector.push_back(rPtr);
			rPtr = rPtr->next();
		} while (!tPtr->isTail(rPtr));
		return aVector;
}
	
bool RecordManager::insert(string tableName, vector<string> data) {
		TablePtr tPtr(new Table(tableName));
		if (tPtr->noBlank()) {
            tPtr->addNewRow(data);  //need validate data in upper level
            TableRowPtr rPtr = tPtr->getTail();
        }
//TableRow tRow = new TableRow(tPtr->getTailAddr(), tPtr->getRowSize());
		else {
			TableRowPtr rPtr = tPtr->getBlankRow();
			rPtr->setBlank(false);
			rPtr->setData(data);
		}
}
	
bool RecordManager::del(string tableName) {
	delete(tableName, Conditon("true"));
}

bool RecordManager::del(string tableName, Conditon cond){
	TablaePtr tPtr(new Table(tableName));
	TableRowPtr rPtr = tPtr.getHead();
//TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
	do {
		if (!rPtr->isBlank() && cond.check(rPtr)) {
			rPtr->setBlank(true);
		}
		rPtr = rPtr->next()      
	} while (!tPtr->isTail(rPtr));
}
