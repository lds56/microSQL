#include"roles/RecordManager.h"
#include"models/TableRow.h"
#include"models/Table.h"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

bool RecordManager::createTable(TableInfo tableInfo) {
	ofstream file("data/" + tableInfo.tableName + ".table");
  	file.close();
    return true;
}
	
bool RecordManager::dropTable(  TableInfo tableInfo) {
    string fileName = "data/" + tableInfo.tableName + ".table";
	int ret_code = remove(fileName.data());
   	if (ret_code == 0) {
		cout << "File was successfully deleted\n";
        return false;
	} else {
	   	cerr << "Error during the deletion: " << ret_code << '\n';
        return true;
	}
}
	
vector<TableRowPtr> RecordManager::select(TableInfo tableInfo) {
	return select(tableInfo, Condition("true"));
}
	
vector<TableRowPtr> RecordManager::select(TableInfo tableInfo, Condition cond) {
		TablePtr tPtr(new Table(tableInfo));
        vector<TableRowPtr> aVector;
		TableRowPtr rPtr = tPtr->getHead();
        //cout << rPtr->getAddr().getOffset() << endl;
//        TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
        while (true) {
			if (cond.check(rPtr) && !rPtr->isBlank()) aVector.push_back(rPtr);
            //cout << rPtr->getAddr().getOffset() << endl;
            if (tPtr->isTail(rPtr)) break;
			rPtr = tPtr->getNext(rPtr);
		}
		return aVector;
}
	
bool RecordManager::insert(TableInfo tableInfo, vector<string> data) {
		TablePtr tPtr(new Table(tableInfo));
		if (tPtr->noBlank()) {
            tPtr->addNewRow(data);  //need validate data in upper level
            TableRowPtr rPtr = tPtr->getTail();
        }
//TableRow tRow = new TableRow(tPtr->getTailAddr(), tPtr->getRowSize());
		else {
			TableRowPtr rPtr = tPtr->getBlankRow();
            tPtr->fillBlank(rPtr, data);
		}
    return true;
}
	
bool RecordManager::del(TableInfo tableInfo) {
	return del(tableInfo, Condition("true"));
}

bool RecordManager::del(TableInfo tableInfo, Condition cond){
	TablePtr tPtr(new Table(tableInfo));
	TableRowPtr rPtr = tPtr->getHead();
//TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
    while (true) {
		if (!rPtr->isBlank() && cond.check(rPtr)) {
			rPtr->setBlank(true);
		}
        if (tPtr->isTail(rPtr)) break;
		rPtr = tPtr->getNext(rPtr);
	}
    return true;
}
