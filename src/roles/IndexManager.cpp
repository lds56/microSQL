#include "IndexManager.h"
#include "BTree.h"


void IndexManager::createIndex(Index indexInfo, vector<string> svector, vector<Address> avector) {
    bTree.create_file(indexInfo);
	for (int i=0; i<svector.size(); i++){
		string key=svector[i];
		int offset=avector[i].getOffset();
		bTree.insert(key, offset);
	}
}

void IndexManager::dropIndex(Index indexInfo) {

}

void IndexManager::insert(string key, Address address) {  //primary key
	int offset=address.getOffset();
    bTree.insert(key, offset);
}
/*
vector<Address> IndexManager::select(vector<Condition> cond) {
    bool leftFlag = cond[0].getLeftFlag();
    bool rightFlag = cond[1].getRightFlag();
    string left = cond[0].getValue();
    string right = cond[1].getValue();
    vector<Address> addrs = bTree.select_between(left, right, leftFlag, rightFlag);
    return addrs;
}

vector<Address> IndexManager::dele(vector<Condition> cond) {
    bool leftFlag = cond[0].getLeftFlag();
    bool rightFlag = cond[1].getRightFlag();
    string left = cond[0].getValue();
    string right = cond[1].getValue();
    vector<Address> addrs= bTree.del(left, right, leftFlag, rightFlag);
    return addrs;
}

bool del(Index& indexInfo, string key) {
    bTree.del(key);
}
vector<Address> select(Index& indexInfo, string key) {
    bTree.select(key);
}*/
