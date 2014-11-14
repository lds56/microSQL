#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_

#include "models/Address.h"
#include "models/Block.h"
#include <string>
#include "models/TableInfo.h"

class BufferManager {
public:
    BufferManager();
    BufferManager(TableInfo tableInfo);
    static void initBlocks(int num);
    Address getHeadAddr();
    Address getTailAddr();
	string read(Address address);
	bool write(Address address, string data);
    Block& readFromFile(Address address);
    Block& findBlock(Address address, bool readFlag);
	bool writeToFile(Address address, string data);
    bool writeToFile(Address address, Block block);

    void printBlocks();

    ~BufferManager();

private:
    static vector<Block> blocks;
    TableInfo tableInfo;
    long int fileSize;
    int howManyRows;
    FILE* fp;
    //static Buffer buffer;
};

#endif