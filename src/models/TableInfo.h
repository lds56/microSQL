#ifndef TABLEINFO_H_
#define TABLEINFO_H_

#include <string>
#include <vector>
using namespace std;

enum FieldType {
	INT,
	CHAR,
	FLOAT
};

struct Field {
	string fieldName;
	FieldType fieldType;
	int fieldLength;
	bool isPrimeryKey;
	bool isUnique;
};

struct TableInfo {
	string tableName;   // Table name
	int blockNum;	// Block number of table data in the file
	int fieldNum;	// Number of fields in the table
	int totalLength;	// Length of one record which equals to sum(fieldLfength)
	vector<Field> fields;  // Vector used to store all field
};



#endif
