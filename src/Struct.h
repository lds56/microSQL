#ifndef __Struct_h__
#define __Struct_h__

#include "Macro.h"
#include <string>
#include <vector>
//#include <models/Condition.h>

using namespace std;

enum Comparison {
    Gt,   //> value
    Lt,
    Eq,
    Ne,
    Ge,
    Le
};

class Index
{
public:
	string indexName;	//all the datas is store in file index_name.index
	string tableName;	//the name of the table on which the index is create
	int column;			//on which column the index is created
	int columnLength;
	int blockNum;		//number of block the datas of the index occupied in the file index_name.table
	Index(): column(0), blockNum(0){}
};

class Row
{
public:
    vector<string> getData() { return columns; }
	vector<string> columns;
};
/*
class Data//����Ͱ�Data�����һ����ά����
{
public:
	vector<Row> rows;
};
*/
//enum Operator{Lt, Le, Gt, Ge, Eq, Ne};//stants for less than, less equal, greater than, greater equal, equal, not equal respectivly
class Status{
public:
	Comparison op;
	int columnNum;
	string value;
};
/*
class buffer{
public:
	bool isWritten;
	bool isValid;
	string filename;
	int blockOffset;	//block offset in file, indicate position in file
	int LRUvalue;		//����ʵ��LRU�㷨,the lower, the better
	char values[BLOCKSIZE + 1];	//c++���÷���ô���ԣ� char* values = new char[BLOCKSIZE];
	buffer(){
		initialize();
	}
	void initialize(){
		isWritten  = 0;
		isValid = 0;
		filename = "NULL";
		blockOffset = 0;
		LRUvalue= 0;
		for(int i = 0; i< BLOCKSIZE; i++) values[i] = EMPTY;
		values[BLOCKSIZE] = '\0';
	}
	string getvalues(int startpos, int endpos){
		string tmpt = "";
		if(startpos >=0 && startpos <= endpos && endpos <= BLOCKSIZE)
			for(int i = startpos; i< endpos; i++)
				tmpt += values[i];
		return tmpt;
	}
	char getvalues(int pos){
		if(pos >=0 && pos <= BLOCKSIZE)
			return values[pos];
		return '\0';
	}
};

class insertPos{
public:
	int bufferNUM;
	int position;
};
*/
#endif
