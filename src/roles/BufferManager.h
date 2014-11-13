#include "models/Address.h"
#include <string>
#include <models/Buffer.h>

class BufferManager {
public:
    void load(string tableName);
    Address getHeadAddr(string tableName);
    Address getTailAddr(string tableName);
	static char* read(Address address);
	static bool write(Address address, char data[]);
	static Address findFree(string tableName);
	static Block readFromFile(string fileName, int offset);
	static bool writeToFile(string fileName, int offset, Block block);

private:

    static Buffer buffer;
};
