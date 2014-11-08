#include<vector>
#include"boost/shared_ptr.hpp"
#include"roles/BufferManager.h"

typedef shared_ptr<TableRow> TableRowPtr;

class TableRow { //read-only
public:
TableRow(Address addr, int size, bool blankFlag): addr(addr), size(size), blankFlag(blankFlag) {
		values = byteTo( BufferManager.read(addr) );
	}  
//TableRow(Address addr, int size): TableRow(addr, size, false) {} //require c++ 11
//	TableRow(Address addr, int size, vector<string> data);
	TableRowPtr next() { return TableRowPtr( new TableRow(addr.add(size), size) ); }
	vector<string> getValues() { return values; }
	bool isBlank() { return blankFlag;}
	void setBlank(bool flag) { blankFlag = flag; }
	
private:
	Address addr; //has table name
	int size;
	bool blankFlag;
	vector<string> values;
}

