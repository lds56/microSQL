#include "roles/BufferManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <models/Block.h>
#include "roles/RecordManager.h"
#include "models/Condition.h"
#include "models/tableRow.h"

using namespace std;

int main() {
    TableInfo tb;
    tb.rowSize = 10;
    tb.fieldNum = 2;
    Field fd1; fd1.fieldLength = 6; fd1.fieldName = "id";
    fd1.fieldType = FieldType::CHAR; fd1.isPrimeryKey = true; fd1.isUnique = true;
    Field fd2; fd2.fieldLength = 4; fd1.fieldName = "name";
    fd1.fieldType = FieldType::CHAR; fd1.isPrimeryKey = false; fd1.isUnique = false;
    tb.fields.push_back(fd1);
    tb.fields.push_back(fd2);
    tb.tableName = "test";
    tb.totalLength = -1;
    Block::size = 4*10;
    BufferManager::status = "DISABLE";
    BufferManager::initBlocks(3);

    Condition cond1 = Condition(Comparison::gt, string("330000"), 0);
    Condition cond2 = Condition(Comparison::lt, string("370000"), 0);
    vector<Condition> conds = vector<Condition> ();
    conds.push_back(cond1); conds.push_back(cond2);
    RecordManager recordManager = RecordManager();
    recordManager.del(tb, conds);
    vector<string> v = vector<string>();
    v.push_back(string("990000newx"));
    recordManager.insert(tb, v);
    vector<string> v2 = vector<string>();
    v2.push_back(string("999000Ne2x"));
    recordManager.insert(tb, v2);
//    cout << v.size() << endl;
//    for (int i=0; i<v.size(); i++) {
//        cout << v[i]->getValues()[0] << endl;
//        cout << v[i]->getValues()[1] << endl;
//    }

    //vector<string> v = vector<string>();
    //v.push_back(string("900000new9"));
    //recordManager.insert(tb, v);


    //BufferManager bufferManager = BufferManager(tb);
    //cout << bufferManager.getTailAddr().getOffset() << endl;
    //cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();


    //bufferManager.write(Address("test", 13), string("313000ld13")); bufferManager.printBlocks();
/*
    cout << bufferManager.read(Address("test", 4)) << endl; bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();

    bufferManager.write(Address("test", 3), string("380000ldx8")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 10)) << endl; bufferManager.printBlocks();

    bufferManager.write(Address("test", 10), string("420000ld10")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();

*/

    //cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();


}