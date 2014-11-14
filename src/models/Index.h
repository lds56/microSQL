#ifndef INDEX_H_
#define INDEX_H_

struct Index {
	string indexName;	// Name of Index
	string tableName;	// Name of the table on which the index is created
	string fieldName;			// Name of field on which the index is created
	int columnLength;
	int blockNum;		//number of block the datas of the index occupied in the file index_name.table
};

#endif
