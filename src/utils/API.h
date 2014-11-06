#include <vector.h>
#include <string.h>

using namespace std;

class API{
public:
	void createTable(InfoRequest request);
	void dropTable(NameRequest reqeust);
	void createIndex(Request request);
	void dropIndex(NameRequest request);
	void select(FromRequest request);
	void select(ConditionReqeust request);
	void insert(InsertRequest reqeust);
	void delete(FromRequest reqeuse);
	void delete(ConditionReqeust requset);
	
}
