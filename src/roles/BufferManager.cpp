#include <string>
#include <cstdio>
#include <ctime>
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include "roles/BufferManager.h"

using namespace std;

vector<Block> BufferManager::blocks;
map<TableInfo, int> BufferManager::fileSizes;
string BufferManager::status;

BufferManager::BufferManager(TableInfo tableInfo){
    this->tableInfo = tableInfo;
    this->fp = fopen((tableInfo.tableName + ".table").c_str(), "r+");
    //cout << "open " << tableInfo.tableName + ".table" << endl;
    if (fp == NULL) cout << "No File Found!" << endl;
    else cout << "open file successfully!" << endl;

    //char sss[100];
    //getcwd(sss, 100);
    //puts(sss);

    fseek(fp, 0, SEEK_END);
    if (fileSizes.count(tableInfo) == 0) {
        fileSize = ftell(fp);
        fileSizes[tableInfo] = fileSize;
    }
    else fileSize = fileSizes[tableInfo];
    this->howManyRows = Block::size/tableInfo.rowSize;
    //cout << "howManyRows: " <<  howManyRows << endl;
}

BufferManager::~BufferManager() {
    fileSizes[tableInfo] = fileSize;
    fseek(fp, 0, SEEK_END);
    fclose(fp);
    //delete fp;
    //int i = 10;
}

bool BufferManager::isNull() {
    return (fileSize == 0);
}

void BufferManager::incFileSize() {
    fileSize += tableInfo.rowSize;
    //cout << "size: " << fileSize << endl;
}

void BufferManager::initBlocks() {
    for (int i=0; i<Block::number; i++)
        blocks.push_back(Block());
}

Address BufferManager::getHeadAddr() {
    return Address(tableInfo.tableName, 0);
}

Address BufferManager::getTailAddr() {
    //cout << "tail: " << fileSize/tableInfo.rowSize - 1 << endl;
    return Address(tableInfo.tableName, fileSize/tableInfo.rowSize - 1);
}

Block& BufferManager::readFromFile(Address address) {
    char buffer[Block::size+1];
    memset(buffer, 0, sizeof buffer);
    //printf("buffer: %d", (int)buffer);
    //buffer[0] = '\0';
    //cout << "buffer: " << (void*)buffer << endl;
    //cout << string(buffer) << endl;
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
        //cout << "len: " << strlen(buffer) << endl;
        while (strlen(buffer)<Block::size) strcat(buffer, "0");
        //cout << string(buffer) << endl;
    } else {
        //buffer = new char(tableInfo.rowSize * howManyRows);
        fread(buffer, tableInfo.rowSize, howManyRows, fp);
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
        //printBlocks();
        writeToFile(blocks[leastIndex]);
    }
    blocks[leastIndex] = Block(string(buffer), startAddress);
    //cout << blocks[leastIndex].pickAllData() << endl;
    //delete[] buffer;
    //buffer = NULL;
    //cout << "mem: " << string(buffer) << endl;
    return (blocks[leastIndex]);
}

bool BufferManager::writeToFile(Block& block) {
    //cout << "bw: " << block.pickAllData() << endl;
    return writeToFile(block.getStartAddress(), block.pickAllData());
}

bool BufferManager::writeToFile(Address address, string data) {
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    //cout << "write: " <<  data << endl;
    //cout << "adr: " << address.getOffset() << endl;
    if (address.getOffset()*tableInfo.rowSize + Block::size > fileSize) {
        int delta = fileSize - address.getOffset()*tableInfo.rowSize;
       // cout << "delta: " << data.substr(0, delta) << " " << tableInfo.rowSize << endl;
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
    //cout << no << endl;
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
    char buffer[Block::size+1] ;// = new char(tableInfo.rowSize);
    memset(buffer, 0, sizeof buffer);
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    fread(buffer, tableInfo.rowSize, 1, fp);
    string aString = string(buffer);
    //delete[] buffer;
    return aString;
}

bool BufferManager::writeDirectly(Address address, string data) {
    fseek(fp, address.getOffset()*tableInfo.rowSize, SEEK_SET);
    fwrite(data.c_str(), tableInfo.rowSize, 1, fp);
    return true;
}

void BufferManager::truncateFrom(Address address) {
    fflush(fp);
    int fd = fileno(fp);
    int size = address.getOffset()*tableInfo.rowSize;
    //cout << "size: " << size << endl;
    if (ftruncate(fd, size))
    {
        perror("truncate wrong");
    }
    rewind(fp);
}

void BufferManager::cleanupBlocks() {
    //cout << "-------------" << endl;
    //printBlocks();
    for (int i=0; i<Block::number; i++)
        if (blocks[i].isDirty()) writeToFile(blocks[i]);
}

void BufferManager::printBlocks() {
    //cout << "print!" << endl;
    /*for (int i=0; i<blocks.size(); i++)
        if (blocks[i].getUsed() != 0) {
            cout << i << ": " << blocks[i].getStartAddress().getOffset() << " " ;
            cout << blocks[i].pickAllData() << " " ;
            cout << blocks[i].isDirty() << " ";
            cout << blocks[i].getUsed() << endl;
        }
        */
}

