#include "unistd.h"
#include "CatalogManager.h"

CatalogManager::CatalogManager(){
    chdir("src/data");
	initTable();
	initIndex();
}

CatalogManager::~CatalogManager(){
    //cout << "destruct" << endl;
	saveTable();
	saveIndex();
}

void CatalogManager::initTable(){
    //cout << "init" << endl;
    char sss[100];
    getcwd(sss, 100);
    puts(sss);
	const string filename = "table.catlog";
	fstream  fin(filename.c_str(), ios::in);
    //fin >> "hhh" ;
    //cout << tableCount << endl;
	fin >> tableCount;
	for(int i = 0; i < tableCount; i++)
	{//fill in the vector of tables
		TableInfo temp_table;
		fin >> temp_table.tableName;
		fin >> temp_table.fieldNum;
		fin >> temp_table.blockNum;
		for(int j = 0; j < temp_table.fieldNum; j++)
		{//fill in the vector of temp_table.fields
			Field temp_attri;
            //cout << temp_attri.fieldName << endl;
	 		fin >> temp_attri.fieldName;
	 		fin >> temp_attri.fieldType;
	 		fin >> temp_attri.fieldLength;
	 		fin >> temp_attri.isPrimeryKey;
	 		fin >> temp_attri.isUnique;
	 		temp_table.fields.push_back(temp_attri);
	 		temp_table.rowSize += temp_attri.fieldLength;
	 	}
	 	tables.push_back(temp_table);
	}
	fin.close();
}

void CatalogManager::initIndex(){
	const string filename = "index.catlog";
	fstream  fin(filename.c_str(), ios::in);
	fin >> indexCount;
	for(int i = 0; i < indexCount; i++)
	{//fill in the vector of indexes
		Index temp_index;
		fin >> temp_index.indexName;
		fin >> temp_index.tableName;
		fin >> temp_index.column;
		fin >> temp_index.columnLength;
		fin >> temp_index.blockNum;
	 	indexes.push_back(temp_index);
	}
	fin.close();
}

void CatalogManager::saveTable(){
    //cout << "save " << endl;
	string filename = "table.catlog";
	fstream  fout(filename.c_str(), ios::out);

    //cout << "cout: " << tableCount << endl;
	fout << tableCount << endl;
	for(int i = 0; i < tableCount; i++)
	{
		fout << tables[i].tableName << " ";
		fout << tables[i].fieldNum << " ";
		fout << tables[i].blockNum << endl;

		for(int j = 0; j < tables[i].fieldNum; j++)
		{
			fout << tables[i].fields[j].fieldName << " ";
			fout << tables[i].fields[j].fieldType << " ";
			fout << tables[i].fields[j].fieldLength << " ";
			fout << tables[i].fields[j].isUnique  << " ";
			fout << tables[i].fields[j].isPrimeryKey  << endl;
		}
	}
	fout.close();
}

void CatalogManager::saveIndex(){
	string filename = "index.catlog";
	fstream  fout(filename.c_str(), ios::out);
	fout << indexCount << endl;
	for(int i = 0; i < indexCount; i++)
	{
		fout << indexes[i].indexName << " ";
		fout << indexes[i].tableName << " ";
		fout << indexes[i].column << " ";
		fout << indexes[i].columnLength << " ";
		fout << indexes[i].blockNum << endl;
	}
	fout.close();
}

void CatalogManager::createTable(TableInfo& table){
	tableCount++;
	for(int i = 0; i < table.fields.size(); i++){
		table.rowSize += table.fields[i].fieldLength;
	}
	tables.push_back(table);
}

void CatalogManager::createIndex(Index index){
	indexCount++;
	indexes.push_back(index);
}

void CatalogManager::dropTable(string tablename){
	for(int i = tableCount -1; i >= 0; i--){
		if(tables[i].tableName == tablename){
			tables.erase (tables.begin()+i);
			tableCount--;
		}
	}
	for(int i = indexCount - 1; i >= 0; i--){//�?ɾ���ˣ�����������ϵ����е�indexҲҪ����ɾ��
		if(indexes[i].tableName == tablename){
			indexes.erase (indexes.begin()+i);
			indexCount--;
		}
	}
}

void CatalogManager::dropIndex(string indexName){
	for(int i = indexCount - 1; i >= 0; i--){
		if(indexes[i].indexName == indexName){
			indexes.erase (indexes.begin()+i);
			indexCount--;
		}
	}
}

void CatalogManager::update(TableInfo& tableinfor){
	for(int i = 0; i < tableCount; i++){
		if(tables[i].tableName == tableinfor.tableName){
			tables[i].fieldNum = tableinfor.fieldNum;
			tables[i].blockNum = tableinfor.blockNum;
			tables[i].rowSize = tableinfor.rowSize;
			tables[i].fields = tableinfor.fields;
		}
	}
}

void CatalogManager::update(Index& index){
	for(int i = 0; i< indexCount; i++){
		if(indexes[i].indexName == index.indexName){
			indexes[i].tableName = index.tableName;
			indexes[i].column = index.column;
			indexes[i].blockNum = index.blockNum;
		}
	}
}

bool CatalogManager::existTable(string tablename){
	int i;
	for(i=0;i<tables.size();i++){
		if(tables[i].tableName==tablename)
			return true;
	}
	return false;
}

bool CatalogManager::existIndex(string tablename, int column){
	int i;
	for(i = 0; i < indexes.size(); i++){
		if(indexes[i].tableName == tablename && indexes[i].column==column)
			break;//found it
	}
	if(i >= indexes.size()) return 0;
	else return 1;
}

bool CatalogManager::existIndex(string indexname){
	int i;
	for(i = 0; i <indexes.size(); i++){
		if(indexes[i].indexName == indexname)
			break;//found it
	}
	if(i >= indexes.size()) return 0;
	else return 1;
}

TableInfo CatalogManager::getTable(string tablename){
	int i;
	TableInfo temp;
	for(i=0;i<tableCount;i++){
		if((tables[i].tableName)==tablename){

			return tables[i];
		}
	}
	return temp;
}

Index CatalogManager::getIndex(string tablename, int column){
	int i;
	for(i = 0; i < indexes.size(); i++){
		if(indexes[i].tableName == tablename && indexes[i].column==column)
			break;//found it
	}
	if(i >= indexCount){
		Index tmpt;
		return tmpt;//indicate that table information not found
	}
	return indexes[i];
}

Index CatalogManager::getIndex(string indexName){
	int i;
	for(i = 0; i < tableCount; i++){
		if(indexes[i].indexName == indexName)
			break;//found it
	}
	if(i >= indexCount){
		Index tmpt;
		return tmpt;//indicate that table information not found
	}
	return indexes[i];
}

void CatalogManager::display(){
	displayTable();
	displayIndex();
}

void CatalogManager::displayTable(){//this method is for debug only
	cout << "##    Number of tables:" <<tableCount << endl;
	for(int i = 0; i < tableCount; i++)
	{
		cout << "TABLE " << i << endl;
		cout << "Table Name: " << tables[i].tableName << endl;
		cout << "Number of fields: "<< tables[i].fieldNum << endl;
		cout << "Number of blocks occupied in disk: " << tables[i].blockNum << endl;
		for(int j = 0; j < tables[i].fieldNum; j++)
		{
			cout << tables[i].fields[j].fieldName << "\t";
			switch(tables[i].fields[j].fieldType)
			{
				case CHAR:	cout << "CHAR("<<tables[i].fields[j].fieldLength << ")\t";	break;
				case INT:	cout << "INT\t";		break;
				case FLOAT:	cout << "FLOAT\t";	break;
				default:	cout << "UNKNOW TYPE\t";	break;
			}
			if(tables[i].fields[j].isUnique)	cout << "Unique\t";
			else cout << "NotUnique ";
			if(tables[i].fields[j].isPrimeryKey) cout << "PrimeryKey\t" << endl;
			else cout << endl;
	 	}
	}
}

void CatalogManager::displayIndex(){//this method is for debug also
	cout << "##    Number of indexes:" <<indexCount << endl;
	for(int i = 0; i < indexCount; i++)
	{
		cout << "INDEX " << i << endl;
		cout << "Index Name: " << indexes[i].indexName << endl;
		cout << "Table Name: "<< indexes[i].tableName << endl;
		cout << "Column Number: " << indexes[i].column << endl;
		cout << "Column Number of blocks occupied in disk: " << indexes[i].blockNum << endl;
	}
}

int CatalogManager::getColNum(TableInfo& tableinfo,string columnname)
{
	for(int i=0;i<tableinfo.fields.size();i++){
		if(tableinfo.fields[i].fieldName==columnname){
			return i;
		}
	}
	return -1;
}

int CatalogManager::getColAmo(TableInfo& tableinfo){
	return tableinfo.fields.size();
}
