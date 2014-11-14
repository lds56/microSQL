#include <roles/BufferManager.h>
#include <iostream>
#include <string>
#include <models/Block.h>

using namespace std;

int main() {
    TableInfo tb;
    tb.rowSize = 10;
    tb.fieldNum = 2;
    tb.fields.push_back("id");
    tb.fields.push_back("name");
    tb.tableName = "test";
    tb.totalLength = -1;
    Block::size = 4*10;
    BufferManager::initBlocks(3);
    BufferManager bufferManager = BufferManager(tb);
    //cout << bufferManager.getTailAddr().getOffset() << endl;
    cout << bufferManager.read(Address("test", 4)) << endl; bufferManager.printBlocks();

/*
    bufferManager.write(Address("test", 4), string("390000ldx9")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 4)) << endl; bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();

    bufferManager.write(Address("test", 3), string("380000ldx8")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 10)) << endl; bufferManager.printBlocks();

    bufferManager.write(Address("test", 10), string("420000ld10")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();

*/

    //cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();


}