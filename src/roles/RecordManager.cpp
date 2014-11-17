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
    vector<Condition> conds = vector<Condition> ();
    conds.push_back(Condition(true));
	return select(tableInfo, conds);
}
	
vector<TableRowPtr> RecordManager::select(TableInfo tableInfo, vector<Condition> conds) {
		TablePtr tPtr(new Table(tableInfo));
        vector<TableRowPtr> aVector;
		TableRowPtr rPtr = tPtr->getHead();
        //cout << rPtr->getAddr().getOffset() << endl;
//        TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
        while (true) {
            if (!rPtr->isBlank()) {
                //cout << rPtr->getValues()[0] << endl;
                bool chosen = true;
                for (int i = 0; i < conds.size(); i++) {
                    int k = conds[i].getKthAttri();
                    if (!conds[i].check(rPtr->getValues()[k])) {
                        chosen = false;
                        break;
                    }
                }
                if (chosen) aVector.push_back(rPtr);
            }
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
            tPtr->popBlank();
            cout << rPtr->getAddr().getOffset() << endl;
            tPtr->fillBlank(rPtr, data);
		}
    return true;
}
	
bool RecordManager::del(TableInfo tableInfo) {
    vector<Condition> conds = vector<Condition> ();
    conds.push_back(Condition(true));
	return del(tableInfo, conds);
}

bool RecordManager::del(TableInfo tableInfo, vector<Condition> conds){
	TablePtr tPtr(new Table(tableInfo));
	TableRowPtr rPtr = tPtr->getHead();
//TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
    while (true) {
		if (!rPtr->isBlank()) {
            bool chosen = true;
            for (int i=0; i<conds.size(); i++) {
                int k = conds[i].getKthAttri();
                if (!conds[i].check(rPtr->getValues()[k])) {
                    chosen = false;
                    break;
                }
            }
            //cout << chosen << endl;
            if (chosen) {
                rPtr->setBlank(true);
                tPtr->pushBlank(rPtr);
            }
		}
        if (tPtr->isTail(rPtr)) break;
		rPtr = tPtr->getNext(rPtr);
	}
    return true;
}

void RecordManager::commit() {
    for (map<TableInfo, queue<TableRowPtr>>::iterator iter=Table::blanksMap.begin(); iter!=Table::blanksMap.end(); ++iter) {
        TablePtr tPtr = new Table(iter->first);
        queue<TableRowPtr> & bQueue = iter->second;

        tPtr->notifyTruncate();
    }
}