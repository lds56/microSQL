#ifndef ADDRESS_H_
#define ADDRESS_H_

#include<string>

using namespace std;


class Address {
public:
    Address() {}
    Address(string fileName, int offset): fileName(fileName), offset(offset) {}
	string getfileName() { return fileName; }
	int getOffset() { return offset; }
    Address add(int delta) { return Address(fileName, offset+delta); }
    bool equalsTo(Address addr) { return (addr.fileName == this-> fileName && addr.offset == this->offset); }
	
private:
	string fileName;
	int offset;
};

#endif
