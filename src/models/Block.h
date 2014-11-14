#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>
#include <iostream>
#include "models/Address.h"

using namespace std;

class Block {
public:
    Block() {
        this->used = clock();
        this->data = string("");
        //strcpy(this->data, "");
        //Address* nullAddr = NULL;
        this->startAddr = *(new Address()); //NULL
        this->dirty = false;
        //cout << "new Block!" << endl;
        //cout << data << endl;
    }

    Block(string data, Address address): used(clock()), dirty(false), data(data), startAddr(address) {
        //strcpy(this->data, data);
    }

    Block(const Block& anotherBlock) {
        this->used = anotherBlock.used;
        this->data = anotherBlock.data;
        //strcpy(this->data, anotherBlock.data);
        this->startAddr = anotherBlock.startAddr;
        this->dirty = anotherBlock.dirty;
    }

    clock_t getUsed() {return used;}
    Address getStartAddress() {return startAddr;}
    bool isDirty() { return dirty; }

    void modify(int start, int len, string newData) {
        //for (int i=0; i<len; i++) data[i+start] = newData[i];
        data.replace(start, len, newData);
        dirty = true;
    }

    string pickData(int start, int len) {
        //char* returnData = (char*) malloc(sizeof(char)*(len+1));
        //for (int i=0; i<len; i++) returnData[i] = data[i+start];
        used = clock();
        return data.substr(start, len);
    }

    string pickAllData() {
        return data;
    }

    bool contains(Address address) {
        return (startAddr.getfileName() == address.getfileName() &&
                startAddr.getOffset() <= address.getOffset() && address.getOffset() < startAddr.getOffset()+size );
    }

    static int size;
    ~Block() {
        //cout << "delete" << endl;
        //delete[] data;
    }

private:
    clock_t used;
    bool dirty;
    string data;
    Address startAddr;
};

#endif