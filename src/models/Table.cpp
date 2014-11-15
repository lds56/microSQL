#include "models/table.h"
#include <string>
#include <queue>
#include <map>

using namespace std;

map<string, queue<TableRowPtr>> Table::blanksMap = map<string, queue<TableRowPtr>>();