#ifndef _BTREE_H_
#define _BTREE_H_

#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "models/storage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "struct.h"
#include <vector>
#include "models/Address.h"
#include "models/TableInfo.h"

using namespace std;
#define MAX 3

class BTree{
public:
	BTree(){
		blkn=1; bias=1; root=1;
		for (int i=1; i<100; i++)
			for (int j=1; j<100; j++)
				lazy[i][j]=0;

	}
	//void delete_file(Index& indexinfo); //drop index
	void create_file(Index indexin); //create index
	void ReadFromFile(Index indexin); // use after this
	void WriteToFile(); //close after this

	vector<Address> del(string left, string right, bool leftFlag, bool rightFlag);
	void insert(string s, int offset);

	//void create(Index& indexinfo);
    vector<Address> select_between(string low, string high, bool leftFlag, bool rightFlag);

    void search(string s, int bp);
	void l_split();
	void b_split();
	void print();

public:
	Index indexinfo;
	storage str;
	int blkn;
	int bias;
	int root;
	int lazy[100][100];
};

#endif
