/*
 * storage.h
 *
 *  Created on: Nov 14, 2014
 *      Author: Administrator
 */

#ifndef STORAGE_H_
#define STORAGE_H_
#include "string"
using namespace std;

class IndexBranch{
public:
	IndexBranch();
public:
	string key;  //20bytes
	int child; //block number <3bytes
};

class block{
public:
    block();
public:
	bool isLeaf;
	bool isRoot;
	int father;  //3
	int length;  //3
	IndexBranch b[100];
};

class storage{
public:
  int recordnumber;
  block blk[100];
};

#endif /* STORAGE_H_ */
