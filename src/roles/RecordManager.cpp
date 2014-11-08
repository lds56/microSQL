#include"models/RecordManager.h"
#include<fstream.h>

using namespace std;

bool RecordManager::createTable(string tableName) {
	ofstream file("data/" + tableName + ".table");
  	file.close();
}
	
bool RecordManager::dropTable(string tableName) {
	int ret_code = remove("data/" + tableName + ".table");
   	if (ret_code == 0) {
		cout << "File was successfully deleted\n";
	} else {
	   	cerr << "Error during the deletion: " << ret_code << '\n';
	}
}
	
vector<string> RecordManager::select(vector<string> fields, string tableName) {
	select(vector<string> fields, string tableName, new Condition("true"));
}
	
	//ensure fields!! 
	//vector<map<string, string>> select(string tableName, Condition cond) {
	//	select()
	//}
	
vector<string> RecordManager::select(vector<string> fields, string tableName, Conditon cond) {
		TablePtr tPtr(new Table(tableName));
        Vector<map<string, string>> aVector = new vector<TableRowPtr>;
		TableRowPtr rPtr = tPtr->getHead();
//        TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
		do {
			if (cond.check(rPtr) && !rPtr->isBlank()) aVector.push_back(rPtr.getValues()); 
			rPtr = rPtr->next();
		} while (!tPtr->isTail(rPtr));
		return aVector;
}
	
bool RecordManager::insert(string tableName, vector<string> data) {
		TablePtr tPtr(new Table(tableName));
		if (tPtr->noBlank())
			tPtr->addNewRow(data);  //need validate data in upper level 
			TableRowPtr rPtr = tPtr->getTail();
//TableRow tRow = new TableRow(tPtr->getTailAddr(), tPtr->getRowSize());
		else {
			TableRowPtr rPtr = tPtr->getBlankRow();
			rPtr->setBlank(false);
			rPtr->setData(data);
		}
}
	
bool RecordManager::delete(string tableName) {
	delete(tableName, new Conditon("true"));
}

bool RecordManager::delete(string tableName, Conditon cond){
	TablaePtr tPtr(new Table(tableName));
	TableRowPtr rPtr = tPtr.getHead();
//TableRowPtr rPtr( new TableRow(tPtr->getHeadAddr(), tPtr->getRowSize()) );
	do {
		if (!rPtr->isBlank() && cond.check(rPtr)) {
			rPtr.setBlank(true);
		}
		rPtr = rPtr->next()      
	} while (!tPtr->isTail(rPtr));
}
