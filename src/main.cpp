#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "roles/RecordManager.h"
#include "models/TableInfo.h"
#include "models/Condition.h"
#include "models/tableRow.h"
#include "roles/BufferManager.h"

using namespace std;

int main() {

    chdir("src/data");

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


    TableInfo tb2;
    tb2.rowSize = 15;
    tb2.fieldNum = 3;
    Field fd3; fd3.fieldLength = 6; fd1.fieldName = "id";
    fd1.fieldType = FieldType::CHAR; fd1.isPrimeryKey = true; fd1.isUnique = true;
    Field fd4; fd4.fieldLength = 4; fd1.fieldName = "name";
    fd1.fieldType = FieldType::CHAR; fd1.isPrimeryKey = false; fd1.isUnique = false;
    Field fd5; fd5.fieldLength = 5; fd5.fieldName = "ooo";
    fd5.fieldType = FieldType::CHAR; fd5.isPrimeryKey = false; fd1.isUnique = false;
    tb2.fields.push_back(fd1);
    tb2.fields.push_back(fd2);
    tb2.fields.push_back(fd3);
    tb2.tableName = "test2";
    tb2.totalLength = -1;

    //Block::size = 4*10;
    BufferManager::status = "ENABLE";
    BufferManager::initBlocks();
    BufferManager bufferManager = BufferManager(tb);

    //char sss[100];
    //getcwd(sss, 100);
    //puts(sss);
    //cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();
    Condition cond1 = Condition(Comparison::gt, string("330000"), 0);
    Condition cond2 = Condition(Comparison::lt, string("370000"), 0);
    vector<Condition> conds = vector<Condition> ();
    conds.push_back(cond1); conds.push_back(cond2);
    RecordManager recordManager = RecordManager();
    //recordManager.registerTable(tb);
    //recordManager.createTable(tb2);

    recordManager.registerTable(tb2);
    vector<TableRowPtr> v = recordManager.select(tb, conds);
    cout << v.size() << endl;
    for (int i=0; i<v.size(); i++) {
        cout << v[i]->getValues()[0] << endl;
        cout << v[i]->getValues()[1] << endl;
    }

    //recordManager.createTable(tb);
    //recordManager.dropTable(tb);


    vector<Condition> conds2;
    conds.push_back(Condition(true));
    vector<TableRowPtr> v2 = recordManager.select(tb2, conds2);
    cout << v2.size() << endl;
    for (int i=0; i<v2.size(); i++) {
       cout << v2[i]->getValues()[0] << endl;
      cout << v2[i]->getValues()[1] << endl;
        cout << v2[i]->getValues()[2] << endl;
    }

    //char sss[100];
    //getcwd(sss, 100);
    //puts(sss);



    //getcwd(sss, 100);
    //puts(sss);

    //recordManager.dropTable(tb);

    //recordManager.registerTable(tb);
    //vector<TableRowPtr> v = recordManager.select(tb, conds);
    //bufferManager.printBlocks();
    //recordManager.del(tb, conds);
    //bufferManager.printBlocks();
    //vector<TableRowPtr> v = recordManager.select(tb, conds);
    //cout << v.size() << endl;
    //recordManager.commit();
    //bufferManager.printBlocks();

    /*
    vector<string> v0 = vector<string>();
    v0.push_back(string("990000"));
    v0.push_back(string("LDS1"));
    recordManager.insert(tb, v0);

    Condition cd0 = Condition(true);
    vector<Condition> vc;
    vc.push_back(cd0);
    vector<TableRowPtr> v = recordManager.select(tb, vc);
    cout << v.size() << endl;*/


//    vector<string> v2 = vector<string>();
//    v2.push_back(string("999000Ne2x"));
//    recordManager.insert(tb, v2);
/*
    cout << v.size() << endl;
    for (int i=0; i<v.size(); i++) {
        cout << v[i]->getValues()[0] << endl;
        cout << v[i]->getValues()[1] << endl;
    }*/

    //vector<string> v = vector<string>();
    //v.push_back(string("900000new9"));
    //recordManager.insert(tb, v);


    //BufferManager bufferManager = BufferManager(tb);
    //cout << bufferManager.getTailAddr().getOffset() << endl;
    //cout << bufferManager.read(Address("test", 12)) << endl; bufferManager.printBlocks();
    //bufferManager.write(Address("test", 12), string("920000ld12")); bufferManager.printBlocks();


    //bufferManager.write(Address("test", 13), string("313000ld13")); bufferManager.printBlocks();

/*

    cout << "read: " << bufferManager.read(Address("test", 4)) << endl; //bufferManager.printBlocks();


    cout << "read: " << bufferManager.read(Address("test", 3)) << endl; //bufferManager.printBlocks();

    bufferManager.write(Address("test", 3), string("380000ldx8")); // bufferManager.printBlocks();

    cout << "read: " << bufferManager.read(Address("test", 10)) << endl;// bufferManager.printBlocks();

    bufferManager.write(Address("test", 10), string("420000ld10"));// bufferManager.printBlocks();

    cout << "read: " << bufferManager.read(Address("test", 3)) << endl;// bufferManager.printBlocks();

    bufferManager.write(Address("test", 12), string("920000ld12")); //bufferManager.printBlocks();

*/

    //bufferManager.cleanupBlocks();


    //bufferManager.truncateFrom(Address("test", 10));


    //cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();

    return 0;

}