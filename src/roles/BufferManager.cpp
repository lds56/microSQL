#include <string>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include "models/Address.h"
#include "models/Block.h"
#include "roles/BufferManager.h"
#include "models/TableInfo.h"

using namespace std;

vector<Block> BufferManager::blocks;

BufferManager::BufferManager(TableInfo tableInfo){
    this->tableInfo = tableInfo;
    this->fp = fopen((tableInfo.tableName + ".tb").c_str(), "r+");
    //char* sss;
    //getcwd(sss, 100);
    //puts(sss);
    fseek(fp, 0, SEEK_END);
    this->fileSize = ftell(fp);
    cout << fileSize << endl;
    this->howManyRows = Block::size/tableInfo.rowSize;
}

BufferManager::~BufferManager() {
    fclose(fp);
}

void BufferManager::initBlocks(int num) {
    for (int i=0; i<num; i++)
        blocks.push_back(Block());
}

Address BufferManager::getHeadAddr() {
    return Address(tableInfo.tableName, 0);
}

Address BufferManager::getTailAddr() {
    return Address(tableInfo.tableName, fileSize/tableInfo.rowSize);
}

Block& BufferManager::readFromFile(Address address) {
    char* buffer = new char(Block::size);
    //int blockNum = address.getOffset()*tableInfo.rowSize / Block::size;{
    int startOffset = (address.getOffset() / howManyRows * howManyRows);
    Address startAddress = Address(address.getfileName(), startOffset);
    fseek(fp, startOffset*tableInfo.rowSize, SEEK_SET);
    if (startOffset*tableInfo.rowSize + Block::size > fileSize) {
        int remainRows = (fileSize - startOffset) / tableInfo.rowSize;
        //buffer = new char(tableInfo.rowSize * remainRows);
        fread(buffer, tableInfo.rowSize, remainRows, fp);
        while (strlen(buffer)<Block::size) strcat(buffer, "0");
    } else {
        //buffer = new char(tableInfo.rowSize * howManyRows);
        fread(buffer, tableInfo.rowSize, howManyRows, fp);
    }
    //LRU
    clock_t leastUsed = blocks[0].getUsed();
    int leastIndex = 0;
    for (int i=1; i<blocks.size(); i++) {
        if (leastUsed < blocks[i].getUsed()) {   //used maximum
            leastUsed = blocks[i].getUsed();
            leastIndex  = i;
        }
    }
    //cout << "leastindex: " << leastIndex << endl;
    if (blocks[leastIndex].isDirty()) {
        writeToFile(address, blocks[leastUsed]);
    }
    blocks[leastIndex] = Block(string(buffer), startAddress);
    //cout << blocks[leastIndex].pickAllData() << endl;
    delete[] buffer;
    return (blocks[leastIndex]);
}

bool BufferManager::writeToFile(Address address, Block block) {
    //int startOffset = (address.getOffset() / howManyRows * howManyRows);
    //Address startAddress = Address(address.getfileName(), startOffset);
    return writeToFile(block.getStartAddress(), block.pickAllData());
}

bool BufferManager::writeToFile(Address address, string data) {
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    //cout << data << endl;
    if (address.getOffset()*tableInfo.rowSize + Block::size > fileSize) {
        int delta = fileSize - address.getOffset()*tableInfo.rowSize;
        //cout << data.substr(0, delta) << endl;
        fwrite(data.substr(0, delta).c_str(), tableInfo.rowSize, (delta)/tableInfo.rowSize, fp);
    } else fwrite(data.c_str(), tableInfo.rowSize, howManyRows, fp);
    return true;
}

Block& BufferManager::findBlock(Address address, bool readFlag) { //read?true, false
    int no = -1;
    for (int i=0; i<blocks.size(); i++)
        if (blocks[i].contains(address, tableInfo.rowSize)) {
            no = i;  //hit
            //if (readFlag && blocks[i].isDirty()) {
            //    writeToFile(address, blocks[i]);
            //}
        }
    //cout << no << endl;
    if (no == -1) return readFromFile(address);   //miss
    else return blocks[no];
}

string BufferManager::read(Address address) {
    int start = address.getOffset() % howManyRows * tableInfo.rowSize;
    int len = tableInfo.rowSize;
    return findBlock(address, true).pickData(start, len);
}

bool BufferManager::write(Address address, string data) {
    int start = address.getOffset() % howManyRows * tableInfo.rowSize;
    int len = tableInfo.rowSize;
    findBlock(address, false).modify(start, len, data);
    return true;
}

void BufferManager::printBlocks() {
    //cout << "print!" << endl;
    for (int i=0; i<blocks.size(); i++)
        if (blocks[i].getUsed() != 0) {
            cout << i << ": " << blocks[i].getStartAddress().getOffset() << " " ;
            cout << blocks[i].pickAllData() << " " ;
            cout << blocks[i].isDirty() << endl;
        }
}

