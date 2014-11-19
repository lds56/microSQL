#ifndef TABLEINFO_H_
#define TABLEINFO_H_

#include <string>
#include <vector>
using namespace std;


class Field {
public:
	string fieldName;
	int fieldType;
	int fieldLength;
	bool isPrimeryKey;
	bool isUnique;
    Field()
    {
        isPrimeryKey=false;
        isUnique=false;
    }
    Field(string n, int t, int l, bool isP, bool isU):
            fieldName(n), fieldType(t), fieldLength(l), isPrimeryKey(isP), isUnique(isU){}
};

class TableInfo {
public:
	string tableName;   // Table name
    int blockNum;
	int rowSize;	// Block number of table data in the file
	int fieldNum;	// Number of fields in the table
	// Length of one record which equals to sum(fieldLfength)
    void resetRowSize() {
        rowSize = 0;
        for (int i=0; i<fields.size(); i++)
            rowSize += fields[i].fieldLength;

    }
	vector<Field> fields;  // Vector used to store all field
    TableInfo(): tableName(""), fieldNum(0), rowSize(0) {}
};

static bool operator<(const TableInfo& a, const TableInfo& b)
{
    return a.tableName < b.tableName;
}

static bool operator==(const TableInfo& a, const TableInfo& b)
{
    return a.tableName == b.tableName;
}

static bool operator>(const TableInfo& a, const TableInfo& b)
{
    return a.tableName > b.tableName;
}



#endif
