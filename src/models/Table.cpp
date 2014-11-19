#include <string>
#include <queue>
#include <map>
#include "models/Table.h"

using namespace std;

map<TableInfo, queue<TableRowPtr>> Table::blanksMap = map<TableInfo, queue<TableRowPtr>>();