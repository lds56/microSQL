#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_

#include <string>
#include <map>
#include "models/TableInfo.h"
#include "models/Address.h"
#include "models/Block.h"

using namespace std;

class BufferManager {
public:
    //BufferManager() {};
    BufferManager(TableInfo tableInfo);
    static void initBlocks();
    void incFileSize();
    void cleanupBlocks();
    void truncateFrom(Address address);
    Address getHeadAddr();
    Address getTailAddr();
	string read(Address address);
	bool write(Address address, string data);
    Block& readFromFile(Address address);
    Block& findBlock(Address address, bool readFlag);
	bool writeToFile(Address address, string data);
    bool writeToFile(Block& block);
    string readDirectly(Address address);
    bool writeDirectly(Address address, string data);
    void printBlocks();
    bool isNull();

    ~BufferManager();

    static string status;

private:
    static vector<Block> blocks;
    static map<TableInfo, int> fileSizes;
    TableInfo tableInfo;
    long int fileSize;
    int howManyRows;
    FILE* fp;
    //static Buffer buffer;
};

#endif