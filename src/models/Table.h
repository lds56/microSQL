#ifndef TABLE_H_
#define TABLE_H_

#include<queue>
#include<string>
#include"models/TableRow.h"
#include"models/Address.h"
#include"roles/BufferManager.h"
#include"models/TableInfo.h"
#include<memory>
#include <map>

using namespace std;

class Table{
public:
	Table(TableInfo tableInfo): tableInfo(tableInfo), bufferManager(BufferManager(tableInfo)), rowSize(tableInfo.rowSize) {
		//rowSize = tableInfo.rowSize; //API.askRowSize(tableName);
        //bufferManager = BufferManager(tableInfo);
		headAddr = bufferManager.getHeadAddr();
        //cout << "head" << bufferManager.read(headAddr) << endl;
        //cout << tableInfo.rowSize << " " << tableInfo.tableName << endl;
        //cout << headAddr.getfileName() << " " << headAddr.getOffset() << endl;
		tailAddr = bufferManager.getTailAddr();
        //cout << "tail" << tailAddr.getOffset() << endl;
        //cout << "tail" << bufferManager.read(tailAddr) << endl;

        if (blanksMap.count(tableInfo)==0) {
            queue<TableRowPtr> blankQueue = queue<TableRowPtr> ();
            blanksMap[tableInfo] = blankQueue;
            //blanksMap.insert( map<string, queue<TableRowPtr>::value_type(tableInfo.tableName, blankQueue) );
        }
	}
	TableRowPtr getHead() {
        vector<string> data = byteTo(bufferManager.read(headAddr));
        return TableRowPtr( new TableRow(headAddr, rowSize, data));
    }
	TableRowPtr getTail() {
        vector<string> data = byteTo(bufferManager.read(tailAddr));
        return TableRowPtr( new TableRow(tailAddr, rowSize, data));
    }
	TableRowPtr getNext(TableRowPtr rPtr) {
        Address nextAddr = rPtr->getAddr().add(1);
        //cout << "mx: " << nextAddr.getOffset() << endl;
        return TableRowPtr( new TableRow(nextAddr, rowSize, byteTo(bufferManager.read( nextAddr ))) );
    }
    TableRowPtr getPrevious(TableRowPtr rPtr) {
        Address prevAddr = rPtr->getAddr().add(-1);
        return TableRowPtr( new TableRow(prevAddr, rowSize, byteTo(bufferManager.read( prevAddr ))) );
    }
	bool isTail(TableRowPtr rPtr) {
        return rPtr->getAddr().equalsTo(tailAddr);
    }
	TableRowPtr getBlankRow() { return blanksMap[tableInfo].front(); }
	void popBlank() { blanksMap[tableInfo].pop(); }
    void pushBlank(TableRowPtr rPtr) { blanksMap[tableInfo].push(rPtr); }
	bool noBlank() { return blanksMap[tableInfo].empty(); }
	string toByte(vector<string> data) {
		string rowString = "";
		for (int i=0; i<data.size(); i++) {
			rowString = rowString + data[i];
			//if (i!=data.size()-1) rowString = rowString + "|";
		}
		return rowString;
	}
    vector<string> byteTo(string rawData) {
        //cout << "raw: " << rawData << endl;
        vector<string> data = vector<string>();
        int now = 0;
        for (int i=0; i<tableInfo.fields.size(); i++) {
            data.push_back(rawData.substr( now, tableInfo.fields[i].fieldLength) );
            now += tableInfo.fields[i].fieldLength;
        }
        return data;
    }
	void addNewRow(vector<string> data) {
		tailAddr.add(rowSize);
        bufferManager.write(tailAddr, toByte(data));
        //getTail()->setData(data);
	}
    void fillBlank(TableRowPtr rPtr, vector<string> data) {
        bufferManager.write(rPtr->getAddr(), toByte(data));
    }
    static map<TableInfo, queue<TableRowPtr>> blanksMap;

private:
    TableInfo tableInfo;
    BufferManager bufferManager;
	int rowSize;
    //shared_ptr<queue<TableRowPtr>> blankQueuePtr;
	Address headAddr;
	Address tailAddr;
};

typedef shared_ptr<Table> TablePtr;

#endif