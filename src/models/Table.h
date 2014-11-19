#ifndef TABLE_H_
#define TABLE_H_

#include<queue>
#include<string>
#include<memory>
#include <map>
#include"models/TableRow.h"
#include"models/Address.h"
#include"roles/BufferManager.h"
#include"models/TableInfo.h"

using namespace std;

class Table{
public:
	Table(TableInfo tableInfo): tableInfo(tableInfo), bufferManager(BufferManager(tableInfo)), rowSize(tableInfo.rowSize) {
		//rowSize = tableInfo.rowSize; //API.askRowSize(tableName);
        //bufferManager = BufferManager(tableInfo);
		headAddr = bufferManager.getHeadAddr();
        //cout << "head" << bufferManager.read(headAddr) << endl;
        bufferManager.printBlocks();
        //cout << tableInfo.rowSize << " " << tableInfo.tableName << endl;
        cout << headAddr.getfileName() << " " << headAddr.getOffset() << endl;
		tailAddr = bufferManager.getTailAddr();
        //cout << "tail" << tailAddr.getOffset() << endl;
        //cout << "tail" << bufferManager.read(tailAddr) << endl;
        for (int i=0; i<rowSize; i++) BLANK_ROW.append("-");

        if (blanksMap.count(tableInfo) == 0) {
            queue<TableRowPtr> blankQueue = queue<TableRowPtr> ();
            blanksMap[tableInfo] = blankQueue;
            //blanksMap.insert( map<string, queue<TableRowPtr>::value_type(tableInfo.tableName, blankQueue) );
        }
        //cout << "b: " << blanksMap[tableInfo].size() << endl;
	}
	TableRowPtr getHead() {
        string rawData = bufferManager.read(headAddr);
        vector<string> data = byteTo(rawData);
        return TableRowPtr( new TableRow(headAddr, rowSize, rawData == BLANK_ROW, data));
    }
	TableRowPtr getTail() {
        //cout << tailAddr.getOffset() << endl;
        string rawData = bufferManager.read(tailAddr);
        //cout << "done!!!!" << endl;
        vector<string> data = byteTo(rawData);
        return TableRowPtr( new TableRow(tailAddr, rowSize, rawData == BLANK_ROW, data));
    }
	TableRowPtr getNext(TableRowPtr rPtr) {
        Address nextAddr = rPtr->getAddr().add(1);
        string rawData = bufferManager.read( nextAddr );
        vector<string> data = byteTo(rawData);
        return TableRowPtr( new TableRow(nextAddr, rowSize, rawData == BLANK_ROW, data));
    }
    TableRowPtr getPrevious(TableRowPtr rPtr) {
        Address prevAddr = rPtr->getAddr().add(-1);
        //cout << "pre" << prevAddr.getOffset() << endl;
        string rawData = bufferManager.read( prevAddr );
        vector<string> data = byteTo(rawData);
        return TableRowPtr( new TableRow(prevAddr, rowSize, rawData == BLANK_ROW, data));
    }
	bool isTail(TableRowPtr rPtr) {
        return rPtr->getAddr().equalsTo(tailAddr);
    }
	TableRowPtr getBlankRow() {
        //cout << blanksMap[tableInfo].size() << endl;
        return blanksMap[tableInfo].front();
    }
	void popBlank() { blanksMap[tableInfo].pop(); }
    void pushBlank(TableRowPtr rPtr) { blanksMap[tableInfo].push(rPtr); }
	bool noBlank() { return blanksMap[tableInfo].empty(); }
    void setBlank(TableRowPtr rPtr) {
        bufferManager.write(rPtr->getAddr(), BLANK_ROW);
        bufferManager.printBlocks();
    }
    bool blankTable() {
        if (bufferManager.isNull()) return true;
        else return false;
    }
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
        bufferManager.incFileSize();
        bufferManager.write(tailAddr.add(1), toByte(data));
        //getTail()->setData(data);
	}
    void fillBlank(TableRowPtr rPtr, vector<string> data) {
        bufferManager.write(rPtr->getAddr(), toByte(data));
    }
    void fillBlankDirectly(TableRowPtr rPtr, vector<string> data) {
        bufferManager.writeDirectly(rPtr->getAddr(), toByte(data));
    }
    void notifyTruncate(Address address) {
        bufferManager.truncateFrom(address);
    }
    void notifyCleanup() {
        bufferManager.cleanupBlocks();
    }
    void printBlocks() {
        bufferManager.printBlocks();
    }
    static map<TableInfo, queue<TableRowPtr>> blanksMap;

private:
    TableInfo tableInfo;
    BufferManager bufferManager;
	int rowSize;
    //shared_ptr<queue<TableRowPtr>> blankQueuePtr;
	Address headAddr;
	Address tailAddr;
    string BLANK_ROW;
};

typedef shared_ptr<Table> TablePtr;

#endif