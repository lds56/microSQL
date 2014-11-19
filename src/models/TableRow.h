#ifndef TABLEROW_H_
#define TABLEROW_H_

#include<vector>
#include<string>
#include<memory>
#include"models/Address.h"

using namespace std;



class TableRow { //read-only
public:
    TableRow(Address addr, int size, bool blankFlag, vector<string> data): addr(addr), size(size), blankFlag(blankFlag), values(data) {}
    TableRow(Address addr, int size, vector<string> data): addr(addr), size(size), blankFlag(false), values(data) {}
    //TableRow(Address addr, int size): TableRow(addr, size, false) {} //require c++ 11
//	TableRow(Address addr, int size, vector<string> data);
	//TableRowPtr next() { return TableRowPtr( new TableRow(addr.add(size), size) ); }
	vector<string> getValues() { return values; }
	bool isBlank() { return blankFlag;}
	void setBlank(bool flag) { blankFlag = flag; }
    void setValues(vector<string> values) { this->values = values; }
    Address getAddr() { return addr; }
	
private:
	Address addr; //has table name
	int size;
	bool blankFlag;
	vector<string> values;
};

typedef shared_ptr<TableRow> TableRowPtr;

#endif