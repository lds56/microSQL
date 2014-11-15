#include <iostream>
#include "roles/BTree.h"
#include "models/storage.h"
#include <fstream>//ofstream,ifstream
#include <sstream>//istringstream
#include "models/Index.h"

using namespace std;

int main(){
	BPlus bt;
	Index indexinfo;
    indexinfo.indexName="test";
	//string filename=indexinfo.indexName+".index";
	//ofstream out(filename.c_str());
    //bt.create(indexinfo);
     bt.ReadFromFile(indexinfo);
    /*cout << bt.blkn << " " << bt.bias << " " << bt.root << " " << bt.str.recordnumber << " " << endl;
	for (int i=1; i<=bt.str.recordnumber; i++){
		cout << bt.str.blk[i].isLeaf << " " << bt.str.blk[i].isRoot << " " << bt.str.blk[i].father << " " << bt.str.blk[i].length << " ";
		for (int j=1; j<=bt.str.blk[i].length; j++)  cout << bt.str.blk[i].b[j].key << " " << bt.str.blk[i].b[j].child << " ";
		cout << endl;
	}

	for (int i=1; i<100; i++){
		for (int j=1; j<100; j++){
			cout << bt.lazy[i][j] << " ";
		}
		cout << endl;
    }*//*
    bt.insert("1");
    bt.insert("2");
    bt.insert("3");
    bt.insert("4");
    bt.insert("5");
    bt.insert("6");
    bt.insert("7");
    bt.insert("8");
    bt.insert("9");
    bt.WriteToFile(indexinfo);
    */bt.print();
    //bt.search("3",bt.root);
    //cout << bt.bias << " " << bt.blkn << endl;*/
    //bt.delete_file(indexinfo);
	return 0;
}
