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
    BufferManager::initBlocks(10);
    BufferManager bufferManager = BufferManager(tb);
    //cout << bufferManager.getTailAddr().getOffset() << endl;
    cout << bufferManager.read(Address("test", 4)) << endl; bufferManager.printBlocks();

    bufferManager.write(Address("test", 4), string("300000lds4")); bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 4)) << endl; bufferManager.printBlocks();

    cout << bufferManager.read(Address("test", 3)) << endl; bufferManager.printBlocks();
}