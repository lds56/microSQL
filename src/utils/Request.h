

class Request { //abstract class
public:
	Request(Action action, string objName){
		this->action = action;
		this->objName = objName;
	};
	virtual ~IRequest()=0;
	virtual ;
	Action getAction() { return action; } //Action should be enum
	String getObjName() { return objName; }
//	void setAction(Action action) { this->action = action; }
//	void setObjName(String objName) {
	
private:
	Action action;
	String objName;
}


class TbInfoRequest: public Request {
public:
	TbInfoRequest(Action action, string objName, map<string, FieldType> attri, string primaryKey) {
		Request(action, objName);
		this->attri = attri;
		this->primaryKey = primaryKey;
	}
	map<string,FieldType> getAttri() { return attri; }
	string getPrimaryKey() { return primaryKey; }
private:
	map<String, FieldType> attri;
	string primaryKey;
}

class NameRequest: public Request {
public:
	NameRequest(Action action, string objName) {
		Request(action, objName);
	}
}

class IdxInfoRequest: public Request {
public:
	IdxInfoRequest(Action action, string objName, string tableName, strinf=g field) {
		Request(action, objName);
		this->tableName = tableName;
		this->field = field;
	}
	string getTableName() { return tableName; }
	string getField() { return field; }
	
private:
	string tableName;
	string field;
}


class FromRequest: public Request {
public:
	FromRequest(Action action, string objName, string fromTable) {
		Request(action, objName);
		this->fromTable = fromTable;
	}
	string geFromTable() {return fromTable; }

private:
	string fromTable;
}

class ConditionReqeust: public FromRequest {
public:
	ConditionReqeust(Action action, string objName, string FromRequest, vector<
					 
}

class InsertRequest: public Request {}
