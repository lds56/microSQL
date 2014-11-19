#include<fstream>
#include<iostream>
#include<vector>
#include"roles/RecordManager.h"
#include"models/Block.h"

using namespace std;

bool RecordManager::createTable(TableInfo tableInfo) {
	ofstream file(tableInfo.tableName + ".table");
    //file << "aaa" << endl;
  	file.close();
    registerTable(tableInfo);
    return true;
}
	
bool RecordManager::dropTable(TableInfo tableInfo) {
    string fileName = tableInfo.tableName + ".table";
	int ret_code = remove(fileName.c_str());
   	if (ret_code == 0) {
		cout << "File was successfully deleted\n";
        return false;
	} else {
	   	cerr << "Error during the deletion: " << ret_code << '\n';
        deregisterTable(tableInfo);
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
            cout << "va: " << rPtr->getValues()[0] << endl;
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
            //TableRowPtr rPtr = tPtr->getTail();
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
                tPtr->setBlank(rPtr);
                rPtr->setBlank(true);
                tPtr->pushBlank(rPtr);
            }
		}
        //if (rPtr->getAddr().getOffset() == 11) break;
        if (tPtr->isTail(rPtr)) break;
		rPtr = tPtr->getNext(rPtr);
	}

    return true;
}

void RecordManager::registerTable(TableInfo tableInfo) {
    tables.push_back(tableInfo);
}

void RecordManager::deregisterTable(TableInfo tableInfo) {
    for (vector<TableInfo>::iterator iter=tables.begin(); iter!=tables.end(); iter++) {
        if (iter->tableName == tableInfo.tableName)
            tables.erase(iter);
    }
}

void RecordManager::commit() {
    if (BufferManager::status == "DISABLE") {}
    else {
        //BufferManager::clearBlocks();
        cout << "cm: " << tables.size() << endl;
        for (int i = 0; i < tables.size(); i++) {
            TablePtr tPtr = TablePtr(new Table(tables[i]));
            //tPtr->printBlocks();
            if (tPtr->blankTable()) break;
            cout << "ds: " << tables[i].tableName << endl;
            TableRowPtr lastRowPtr = tPtr->getTail();
            cout << "ls: " << lastRowPtr->getAddr().getOffset() << endl;
            //queue<TableRowPtr> & bQueue = iter->second;
            while (!tPtr->noBlank()) {
                TableRowPtr rPtr = tPtr->getBlankRow();
                cout << "tr: " << rPtr->getAddr().getOffset();
                tPtr->popBlank();
                //while (!lastRowPtr->isBlank()) lastRowPtr = tPtr->getPrevious(rPtr);
                cout << " ls: " << lastRowPtr->getAddr().getOffset() << endl;
                tPtr->fillBlank(rPtr, lastRowPtr->getValues());
                lastRowPtr = tPtr->getPrevious(lastRowPtr);
            }
            cout << "[]" << endl;
            tPtr->printBlocks();
            tPtr->notifyCleanup();
            //tPtr->printBlocks();
            cout << "last: " << lastRowPtr->getAddr().getOffset() << endl;
            tPtr->notifyTruncate(lastRowPtr->getAddr());
        }
    }
}

//vector<TableInfo> RecordManager::tables = vector<TableInfo>();