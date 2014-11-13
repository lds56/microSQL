#include "CatalogManager.h"
#include "BufferManager.h"
#include <iostream>
using namespace std;

#define MAX_TABLEINFO_SIZE 10000
#define MAX_INDEXINFO_SIZE 10000

void CatalogManager::loadTableCatalog(string tableCatalog) {
	Address table(tableCatalog, 0);
	BufferManager buffer;
	iostream in;

	in << buffer.read(table);
	in >> tableCount;
	for(int i = 0; i < tableCount; i++) {
		TableInfo tempTable;
		in >> tempTable.tableName;
		in >> tempTable.blockNum;
		in >> tempTable.fieldNum;
		for(int j = 0; j < tempTable.fieldNum; j++) {
			Field tempField;
			in >> tempField.fieldName;
			in >> tempField.fieldType;
			in >> tempField.fieldLength;
			in >> tempField.isPrimeryKey;
			in >> tempField.isUnique;
			tempTable.fields.push_back(tempField);
			tempTable.totalLength +=tempField.fieldLength;
		}
		tables.push_back(tempTable);
	}
}

void CatalogManager::loadIndexCatalog(string indexCatalog) {
	Address index(indexCatalog, 0);
	BufferManager buffer;
	iostream in;

	in << buffer.read(index);
	in >> indexCount;
	for(int i = 0; i < indexCatalog; i++) {
		Index tempIndex;
		in >> tempIndex.indexName;
		in >> tempIndex.tableName;
		in >> tempIndex.fieldName;
		in >> tempIndex.columnLength;
		in >> tempIndex.blockNum;
		indexes.push_back(tempIndex);
	}
}

void CatalogManager::storeTableCatalog(string tableCatalog) {
	Address table(tableCatalog, 0);
	BufferManager buffer;
	string data;

	data += tableCount;
	for(int i = 0; i < tableCount; i++) {
		iostream out;
		out << tables[i].tableName << " ";
		out << tables[i].blockNum << " ";
		out << tables[i].fieldNum << endl;
		for(int j = 0; j < tables[i].fieldNum; i++) {
			out << tables[i].fields[j].fieldName << " ";
			out << tables[i].fields[j].fieldType << " ";
			out << tables[i].fields[j].fieldLength << " ";
			out << tables[i].fields[j].isPrimeryKey << " ";
			out << tables[i].fields[j].isUnique << endl;
		}
		string temp;
		out >> temp;
		data = data.insert(1 + i * MAX_TABLEINFO_SIZE, temp);
	}
	buffer.write(table, data);
}

void CatalogManager::storeIndexCatalog(string indexCatalog) {
	Address index(indexCatalog, 0);
	BufferManager buffer;
	string data;

	data += indexCount;
	for(int i = 0; i << indexCount; i++) {
		iostream out;
		out << indexes[i].indexName << " ";
		out << indexes[i].tableName << " ";
		out << indexes[i].fieldName << " ";
		out << indexes[i].columnLength << " ";
		out << indexes[i].blockNum << endl;
		string temp;
		out >> temp;
		data = data.insert(1 + i * MAX_INDEXINFO_SIZE, temp);
	}
	buffer.write(index, data);
}

CatalogManager::CatalogManager() {
	loadTableCatalog();
	loadIndexCatalog();
}

CatalogManager::~CatalogManager() {
	storeTableCatalog();
	storeIndexCatalog();
}

bool CatalogManager::existsTable(string tableName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName)
			return true;
	}
	return false;
}

bool CatalogManager::hasIndex(string tableName, string fieldName) {
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].tableName == tableName && indexes[i].fieldName == fieldName)
			return true;
	}
	return false;
}

bool CatalogManager::hasIndex(string indexName) {
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].indexName == indexName)
			return true;
	}
	return false;
}

vector<Field> CatalogManager::getFields(string tableName) {
	vector<Field> emptyFields;
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName)
			return tables[i].fields;
	}
	return emptyFields;
}

Index CatalogManager::getIndex(string tableName, string fieldName) {
	Index emptyIndex;
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].tableName == tableName && indexes[i].fieldName == fieldName)
			return indexes[i];
	}
	return emptyIndex;
}

Index CatalogManager::getIndex(string indexName) {
	Index emptyIndex;
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].indexName == indexName)
			return indexes[i];
	}
	return emptyIndex;
}

int CatalogManager::getFieldCount(string tableName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName)
			return tables[i].fields.size();
	}
	return 0;
}

int CatalogManager::getIndexCount(string tableName) {
	int cnt = 0;
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].tableName == tableName)
			cnt++;
	}
	return cnt;
}

int CatalogManager::getRowLength(string tableName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName)
			return tables[i].totalLength;
	}
	return 0;
}

string CatalogManager::getPrimaryKey(string tableName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName) {
			for(int j = 0; j < tables[i].fields.size(); j++)
				if(tables[i].fields[j].isPrimeryKey)
					return tables[i].fields[j].fieldName;
		}
	}
	return "NotFound";
}

bool CatalogManager::isUnique(string tableName, string fieldName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName) {
			for(int j = 0; j < tables[i].fields.size(); j++)
				if(tables[i].fields[j].fieldName == fieldName)
					return tables[i].fields[j].isUnique;
		}
	}
	return false;
}

bool CatalogManager::isPrimaryKey(string tableName, string fieldName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName) {
			for(int j = 0; j < tables[i].fields.size(); j++)
				if(tables[i].fields[j].fieldName == fieldName)
					return tables[i].fields[j].isPrimeryKey;
		}
	}
	return false;
}

void CatalogManager::addTable(TableInfo &table) {
	tableCount++;
	for(int i = 0; i < table.fields.size(); i++){
		table.totalLength += table.fields[i].fieldLength;
	}
	tables.push_back(table);
}

void CatalogManager::deleteTable(string tableName) {
	for(int i = 0; i < tableCount; i++) {
		if(tables[i].tableName == tableName) {
			tables.erase (tables.begin()+i);
			tableCount--;
			break;
		}
	}
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].tableName == tableName) {
			indexes.erase (indexes.begin()+i);
			indexCount--;
		}
	}
}

void CatalogManager::addIndex(Index &index) {
	indexCount++;
	indexes.push_back(index);
}

void CatalogManager::deleteIndex(string indexName) {
	for(int i = 0; i < indexCount; i++) {
		if(indexes[i].indexName == indexName){
			indexes.erase (indexes.begin()+i);
			indexCount--;
		}
	}
}

TableInfo CatalogManager::getTableInfo(string tableName) {
	TableInfo emptyTableInfo;
	for(int i = 0; i < tableCount; i++)
		if(tables[i].tableName == tableName)
			return tables[i];
	return emptyTableInfo;
}





















