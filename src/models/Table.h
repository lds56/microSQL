#include<queue>
#include<string>

typedef shared_ptr<Table> TablePtr;

class Table{
public:
	Table(string tableName) {
		int rowSize = API.askRowSize(tableName);
		BufferManager.load(tableName);
		Address headAddr = BufferManager.getHeadAddr(tableName);
		Address tailAddr = BufferManager.getTailAddr(tableName);
	}
	TableRowPtr getHead() { return TableRowPtr( new TableRow(headAddr, rowSize)); }
	TableRowPtr getTail() { return TableRowPtr( new TableRow(tailAddr, rowSize)); }
	bool isTail(TableRowPtr rPtr) { return rPtr.getAddr().equalsTo(tailAddr); }
	TableRowPtr getBlankRow() { return blankQueue.front(); }
	void popBlank() { blankQueue.pop(); }
	bool noBlank() { return blankQueue.empty(); }
	char[] toByte() {
		String rowString = "";
		for (int i=0; i<data.size(); i++) {
			rowString = rowString + data[i];
			if (i!=data.size()-1) rowString = rowString + "|";
		}
		return rowString.data();
	}
	void addNewRow(vector<string> data) {
		tailAddr += rowSize;
		BufferManager.write(tailAddr, toByte(data));
	}
	
private:
	int rowSize;
	queue<TableRowPtr> blankQueue;
	Address headAddr;
	Address tailAddr;
}
