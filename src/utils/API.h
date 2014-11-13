#include <vector>
#include <string>
#include "utils/Request.h"

using namespace std;

class API{
public:
	void createTable(TbInfoRequest request);
	void dropTable(NameRequest reqeust);
	void createIndex(IdxInfoRequest request);
	void dropIndex(NameRequest request);
	void select(FromRequest request);
	void select(ConditionReqeust request);
	void insert(InsertRequest reqeust);
	void del(FromRequest reqeuse);
	void del(ConditionReqeust requset);
	
};
