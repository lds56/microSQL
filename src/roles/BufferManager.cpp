#include <string>
#include <cstdio>
#include <ctime>
#include <memory>
#include "models/Address.h"
#include "models/Block.h"
#include "roles/BufferManager.h"
#include "models/TableInfo.h"

using namespace std;

vector<Block> BufferManager::blocks;
string BufferManager::status;

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
    fseek(fp, 0, SEEK_END);
    fclose(fp);
    //delete fp;
    //int i = 10;
}

void BufferManager::initBlocks(int num) {
    for (int i=0; i<num; i++)
        blocks.push_back(Block());
}

Address BufferManager::getHeadAddr() {
    return Address(tableInfo.tableName, 0);
}

Address BufferManager::getTailAddr() {
    return Address(tableInfo.tableName, fileSize/tableInfo.rowSize - 1);
}

Block& BufferManager::readFromFile(Address address) {
    char buffer[Block::size+1];
    memset(buffer, 0, sizeof buffer);
    //printf("buffer: %d", (int)buffer);
    //buffer[0] = '\0';
    cout << "buffer: " << (void*)buffer << endl;
    cout << string(buffer) << endl;
    //int blockNum = address.getOffset()*tableInfo.rowSize / Block::size;{
    int startOffset = (address.getOffset() / howManyRows * howManyRows);
    Address startAddress = Address(address.getfileName(), startOffset);
    fseek(fp, startOffset*tableInfo.rowSize, SEEK_SET);
    //cout << "start: " << startOffset << endl;
    //cout << "fileSize: " << fileSize << endl;
    if (startOffset*tableInfo.rowSize + Block::size > fileSize) {
        int remainRows = (fileSize - startOffset*tableInfo.rowSize) / tableInfo.rowSize;
        //buffer = new char(tableInfo.rowSize * remainRows);
        fread(buffer, tableInfo.rowSize, remainRows, fp);
        cout << "len: " << strlen(buffer) << endl;
        while (strlen(buffer)<Block::size) strcat(buffer, "0");
        cout << string(buffer) << endl;
    } else {
        //buffer = new char(tableInfo.rowSize * howManyRows);
        fread(buffer, tableInfo.rowSize, howManyRows, fp);
        cout << "len: " << strlen(buffer) << endl;
        cout << string(buffer) << endl;
    }
    //LRU
    clock_t leastUsed = blocks[0].getUsed();
    int leastIndex = 0;
    for (int i=1; i<blocks.size(); i++) {
        //cout << blocks[i].getUsed() << endl;
        if (leastUsed > blocks[i].getUsed()) {   //used min
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
    //delete[] buffer;
    //buffer = NULL;
    //cout << "mem: " << string(buffer) << endl;
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
        if (blocks[i].contains(address, howManyRows)) {
            no = i;  //hit
            //if (readFlag && blocks[i].isDirty()) {
            //    writeToFile(address, blocks[i]);
            //}
        }
    cout << no << endl;
    if (no == -1) return readFromFile(address);   //miss
    else return blocks[no];
}

string BufferManager::read(Address address) {
    if (status == "DISABLE") return readDirectly(address);
    else {
        int start = address.getOffset() % howManyRows * tableInfo.rowSize;
        int len = tableInfo.rowSize;
        return findBlock(address, true).pickData(start, len);
    }
}

bool BufferManager::write(Address address, string data) {
    if (status=="DISABLE") writeDirectly(address, data);
    else {
        int start = address.getOffset() % howManyRows * tableInfo.rowSize;
        int len = tableInfo.rowSize;
        findBlock(address, false).modify(start, len, data);
    }
    return true;
}

string BufferManager::readDirectly(Address address) {
    char* buffer = new char(tableInfo.rowSize);
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    fread(buffer, tableInfo.rowSize, 1, fp);
    string aString = string(buffer);
    delete[] buffer;
    return aString;
}

bool BufferManager::writeDirectly(Address address, string data) {
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    fwrite(data.c_str(), tableInfo.rowSize, 1, fp);
    return true;
}

void BufferManager::truncate(Address address) {

}

void BufferManager::printBlocks() {
    //cout << "print!" << endl;
    for (int i=0; i<blocks.size(); i++)
        if (blocks[i].getUsed() != 0) {
            cout << i << ": " << blocks[i].getStartAddress().getOffset() << " " ;
            cout << blocks[i].pickAllData() << " " ;
            cout << blocks[i].isDirty() << " ";
            cout << blocks[i].getUsed() << endl;
        }
}

