#include "roles/IndexManager.h"
#include "roles/BTree.h"


bool IndexManager::createIndex(Index& indexInfo, string tableName, Field field) {

}
bool IndexManager::dropIndex(Index& indexInfo, string tableName, Field field) {

}
bool IndexManager::insert(Index& indexInfo, string key) {  //primary key
    bTree.insert(key);
}
bool del(Index& indexInfo, string key) {
    bTree.del(key);
}
vector<Address> select(Index& indexInfo, string key) {
    bTree.select
}