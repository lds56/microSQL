#include<queue>
#include<string>
#include"models/TableRow.h"
#include"models/Address.h"
#include"roles/BufferManager.h"

using namespace std;


typedef shared_ptr<Table> TablePtr;

class Table{
public:
	Table(string tableName) {
		int rowSize = 10;//API.askRowSize(tableName);
        BufferManager::load(tableName);
		Address headAddr = BufferManager::getHeadAddr(tableName);
		Address tailAddr = BufferManager::getTailAddr(tableName);
	}
	TableRowPtr getHead() { return TableRowPtr( new TableRow(headAddr, rowSize)); }
	TableRowPtr getTail() { return TableRowPtr( new TableRow(tailAddr, rowSize)); }
//	TableRowPtr getNext() { return TableRowPtr(); }
	bool isTail(TableRowPtr rPtr) { return rPtr->getAddr().equalsTo(tailAddr); }
	TableRowPtr getBlankRow() { return blankQueue.front(); }
	void popBlank() { blankQueue.pop(); }
	bool noBlank() { return blankQueue.empty(); }
	char* toByte(vector<string> data) {
		string rowString = "";
		for (int i=0; i<data.size(); i++) {
			rowString = rowString + data[i];
			if (i!=data.size()-1) rowString = rowString + "|";
		}
		return rowString.data();
	}
	void addNewRow(vector<string> data) {
		tailAddr += rowSize;
        getTail()->setData(data);
        //BufferManager::write(tailAddr, toByte(data));
	}

private:
	int rowSize;
	queue<TableRowPtr> blankQueue;
	Address headAddr;
	Address tailAddr;
}
