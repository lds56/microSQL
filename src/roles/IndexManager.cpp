#include "roles/IndexManager.h"
#include "roles/BTree.h"


bool IndexManager::createIndex(Index& indexInfo, string tableName, Field field) {

}
bool IndexManager::dropIndex(Index& indexInfo, string tableName, Field field) {

}
bool IndexManager::select(Index& indexInfo, Condition cond) {
    string left = cond.getLeft();
    string right = cond.getRight();
    vector<Address> addrs = bTree.select(left, right);
}
bool IndexManager::del(Index& indexInfo, Condition cond) {
    string left = cond.getLeft();
    string right = cond.getRight();
    bTree.del(left, right);
}
bool IndexManager::insert(Index& indexInfo, string key, Address address) {  //primary key
    bTree.insert(key, address);
}
bool del(Index& indexInfo, string key) {
    bTree.del(key);
}
vector<Address> select(Index& indexInfo, string key) {
    bTree.select(key);
}