#include <vector>
#include <sstream>
#include <iomanip>
#include "Interpret.h"

using namespace std;

extern CatalogManager catalog; 

Interpret::Interpret(){
	operation =UNKNOW;
	tableName = "";
	indexName = "";
	colName="";
	primaryKey=-1;
	unique=-1;
}

Interpret::~Interpret(){

}

void Interpret::initColumn()
{
	if(column.size()>0){
		column.clear();
	}
}

//This function is used to initiate the 'condition' structure
void Interpret::initCondition()
{
	if(condition.size()>0){
		condition.clear();
	}
}

//This function is used to initiate the 'insertvalue' structure
void Interpret::initValue()
{
	if(row.columns.size()>0){
		row.columns.clear();
	}
}

//This function is used to get a word ( maybe a 'token' is more appropriate )
//from the 'src' string and copy it into the 'des' string
bool Interpret::getWord(string & src, string & des)
{   
	des.clear();
	unsigned int srcpos = 0, despos = 0;
	char temp = ' ';

	for(;srcpos<src.length();srcpos++) {    
		if(temp == ' ' || temp == '\t' || temp == 10 || temp == 13)
			temp = src[srcpos];
		else break;
	}
	if(srcpos == src.length() && (temp == ' ' || temp == '\t' || temp == 10 || temp == 13))
		return false;
			
	switch(temp)
	{
	case ',':
	case '(':
	case ')':
	case '*':
	case '=':
	case '\'':
		des+=temp;
		src.erase(0,srcpos);
		break;
	case '<':
		des+=temp;
		temp = src[srcpos++];
		if(temp == '=' || temp == '>')
		{
			des+=temp;
			src.erase(0,srcpos);
		}
		else
		{
			src.erase(0,srcpos-1);
		}
		break;
	case '>':
		des+=temp;
		temp = src[srcpos++];
		if(temp == '=')
		{
			des+=temp;
			src.erase(0,srcpos);
		}
		else
		{
			src.erase(0,srcpos-1);
		}
		break;
	default:
		do{
			des+=temp;
			if (srcpos < src.length())
				temp = src[srcpos++];
			else  {
				src.erase(0,srcpos);
				des[despos++]='\0';
				return true;
			}
		}while(temp != '*' && temp != ',' && temp != '(' && temp != ')'
			&& temp != ' ' && temp != '\t' && temp != '=' && temp != '>' 
			&& temp != '<' && temp != '\'' && temp != 10 && temp != 13 );
		src.erase(0,srcpos-1);
	}
	return true;
}

bool Interpret::getStr(string& src, string& des)
{
	unsigned int pos=0;
	des.clear();
	char temp;
	if (src[0] == '\'') {
		des = "";
		return true;
	}
	else for(pos=0;pos<src.length();pos++) {
		if(src[pos] != '\''){
			temp=src[pos];
			des+=temp;
		}
		else {
            //cout << "des" << des << endl;
			src.erase(0,pos);
			return true;
		} 
	}
	return false;
}

//This function is used to parse the 'command' string which comes from user's 
//input, and return a corresponding parse parsetree
void Interpret::parsing(char* command)
{
	string word ="";
	string temp = command;
	short int FindPriKey = 0, FindUnique = 0;
	Field tempcol;
	Status tempcon;
	string   temprow;
	bool flag;
	
	operation = UNKNOW;
	tableName = "";
	indexName = "";
	initColumn();
	initCondition();
	initValue();
	initTable();
	initIndex();
	flag = getWord(temp, word);//��temp�ַ��е�һ���ո�ǰ�ĵ���ȥ���������õ��ʸ���WORD��Ϊ�ַ�,flag����1
	//���ǿղ�ѯ
	if(!flag)
	{
		operation = EMPTY;
		return;
	}

	//�����˳�����
	else if(strcmp(word.c_str(),"quit")==0)
	{
		flag = getWord(temp, word);
		if(!flag)
			operation = QUIT;
		return;
	}

	//����select����
	else if(strcmp(word.c_str(),"select")==0)
	{
		operation = SELERR;
		
		//����select�Ӿ�
		flag = getWord(temp, word);
		if(!flag)
			return;
		//initcol(column);
		/*tempcol.fieldName=word;
		column.push_back(tempcol);//�����￪ʼpushҪ���ص���
		flag = getWord(temp, word);
		if(!flag)
			return;
		
		while(strcmp(word.c_str(),",")==0)
		{
			flag = getWord(temp, word);
			if(!flag)
				return;
			tempcol.fieldName=word;
			column.push_back(tempcol);//ѭ��pushҪ���ص���
			flag = getWord(temp, word);
			if(!flag)
				return;
		}*/
        flag = getWord(temp, word);
        if(!flag)
            return;
	
		if(strcmp(word.c_str(),"from")!=0)
			return;
		//����from�Ӿ�
		flag = getWord(temp, word);
		if(!flag)
			return;
		tableName  = word;
		
		if(!catalog.existTable(word)){
			operation=TABLEERROR;
			return;
		}
		getTableInfo=catalog.getTable(tableName);//�ڱ���ڵ�ǰ���»�ȡ�����Ϣ

		//���û��where�Ӿ䣬��ȷ����
		flag = getWord(temp, word);
		if(!flag)
		{
			operation = SELECT_NOWHERE_CAULSE;
			return;
		}

		if(strcmp(word.c_str(),"where")!=0)
			return;
		//����where�Ӿ�
		flag = getWord(temp, word);
		if(!flag)
		{
			operation = SELERR;
		}
		tempcon.columnNum=catalog.getColNum(getTableInfo,word);
		colName=word;
		if(tempcon.columnNum==-1){
			operation=COLUMNERROR;
			return;
		}
		flag = getWord(temp, word);
		if(!flag)
			return;
		if(strcmp(word.c_str(),"<")==0)
			tempcon.op = Lt ;
		else if(strcmp(word.c_str(),"<=")==0)
			tempcon.op = Le;
		else if(strcmp(word.c_str(),">")==0)
			tempcon .op = Gt;
		else if(strcmp(word.c_str(),">=")==0)
			tempcon .op = Ge;
		else if(strcmp(word.c_str(),"=")==0)
			tempcon .op = Eq;
		else if(strcmp(word.c_str(),"<>")==0)
			tempcon .op = Ne;
		else return;
			
		flag = getWord(temp, word);
		if(!flag)
			return;
		if(strcmp(word.c_str(),"\'")==0)
		{
			flag = getStr(temp, word);
			if(!flag)
				return;
			tempcon.value=word;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"\'")!=0)
				return;
		}
		else
		{
			tempcon.value=word;
		}
		condition.push_back(tempcon);//��ʼ������������
		flag = getWord(temp, word);
		if(!flag)
		{
			operation = SELECT_WHERE_CAULSE;
			return;
		}
		while(strcmp(word.c_str(),"and")==0)
		{
			flag = getWord(temp, word);
			if(!flag)
				return;
			tempcon.columnNum=catalog.getColNum(getTableInfo,word);
			if(tempcon.columnNum==-1){
				operation=COLUMNERROR;
				return;
			}
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"<")==0)
				tempcon .op = Lt;
			else if(strcmp(word.c_str(),"<=")==0)
				tempcon .op = Le;
			else if(strcmp(word.c_str(),">")==0)
				tempcon .op = Gt;
			else if(strcmp(word.c_str(),">=")==0)
				tempcon .op = Ge;
			else if(strcmp(word.c_str(),"=")==0)
				tempcon .op = Eq;
			else if(strcmp(word.c_str(),"<>")==0)
				tempcon .op = Ne;
			else return;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"\'")==0)
			{
				flag = getStr(temp, word);
				if(!flag)
					return;
				tempcon.value=word;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"\'")!=0)
					return;
			}
			else
			{
				tempcon.value=word;
			}
			condition.push_back(tempcon);//ѭ����������
			flag = getWord(temp, word);
			if(!flag)
			{
				operation = SELECT_WHERE_CAULSE;
				return;
			}
		}
	}
	//����create table/create index����
	else if(strcmp(word.c_str(),"create")==0)
	{
		flag = getWord(temp, word);
		if(!flag)
			return;
		//����create table����
		if(strcmp(word.c_str(),"table")==0)
		{
			operation = CRETABERR;
			flag = getWord(temp, word);
			if(!flag)
				return;
			tableName  = word;
			if(catalog.existTable(tableName)){
				operation=TABLEEXISTED;
				return;
			}
			getTableInfo.tableName=word;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"(")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"unique")==0||strcmp(word.c_str(),"primary")==0)
				return;
			tempcol.fieldName=word;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"int")==0)
			{
				tempcol.fieldType = INT;
				tempcol.fieldLength = INTLEN;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"unique")==0)
				{
					tempcol.isUnique = 1;
					flag = getWord(temp, word);
					if(!flag)
						return;
				}
			}
			else if(strcmp(word.c_str(),"float")==0)
			{
				tempcol.fieldType = FLOAT;
				tempcol.fieldLength = FLOATLEN;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"unique")==0)
				{
					tempcol.isUnique = 1;
					flag = getWord(temp, word);
					if(!flag)
						return;
				}
			}
			else if(strcmp(word.c_str(),"char")==0)
			{
				tempcol.fieldType = CHAR;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"(")!=0)
					return;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(!isInt(word.c_str()))
					return;
				tempcol.fieldLength = atoi(word.c_str());
				if(tempcol.fieldLength > 255 || tempcol.fieldLength < 1)
				{
					operation = CHARBOUD;
					return;
				}
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),")")!=0)
					return;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"unique")==0)
				{
					tempcol.isUnique = 1;
					flag = getWord(temp, word);
					if(!flag)
						return;
				}
			}
			else return;
			getTableInfo.fields.push_back(tempcol);//��ʼ������Ϣ����
			tempcol.isUnique=tempcol.isPrimeryKey=0;
			while(strcmp(word.c_str(),",")==0)
			{
				flag = getWord(temp, word);
				if(!flag)
					return;
				//�����primary key����
				if(strcmp(word.c_str(),"primary")==0)
				{
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"key")!=0)
						return;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"(")!=0)
						return;
					flag = getWord(temp, word);
					if(!flag)
						return;
					for(int i=0;i<getTableInfo.fields.size();i++){
						if(strcmp(getTableInfo.fields[i].fieldName.c_str(),word.c_str())==0){
							FindPriKey=1;
							getTableInfo.fields[i].isPrimeryKey=1;
							getTableInfo.fields[i].isUnique=1;
						}
					}
					if(!FindPriKey)
					{
						operation = VOIDPRI;
						return;
					}
					FindPriKey = 0;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),")")!=0)
						return;
					flag = getWord(temp, word);
					if(!flag)
						return;
				}
				//�����unique����
				else if(strcmp(word.c_str(),"unique")==0)
				{
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"(")!=0)
						return;
					flag = getWord(temp, word);
					if(!flag)
						return;
						for(int i=0;i<getTableInfo.fields.size();i++){
						if(strcmp(getTableInfo.fields[i].fieldName.c_str(),word.c_str())==0){
							FindUnique=1;
							getTableInfo.fields[i].isUnique=1;
						}
					}
					if(!FindUnique)
					{
						operation = VOIDUNI;
						return;
					}
					FindUnique = 0;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),")")!=0)
						return;
					flag = getWord(temp, word);
					if(!flag)
						return;
				}
				//�����unique��primary key����
				else
				{
					tempcol.fieldName=word;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"int")==0)
					{
						tempcol.fieldType = INT;
						tempcol.fieldLength = INTLEN;
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(strcmp(word.c_str(),"unique")==0)
						{	
							tempcol.isUnique = 1;
							flag = getWord(temp, word);
							if(!flag)
								return;
						}
					}
					else if(strcmp(word.c_str(),"float")==0)
					{
						tempcol.fieldType = FLOAT;
						tempcol.fieldLength = FLOATLEN;
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(strcmp(word.c_str(),"unique")==0)
						{
							tempcol.isUnique = 1;
							flag = getWord(temp, word);
							if(!flag)
							return;
						}
					}
					else if(strcmp(word.c_str(),"char")==0)
					{
						tempcol.fieldType = CHAR;
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(strcmp(word.c_str(),"(")!=0)
							return;
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(!isInt(word.c_str()))
							return;
						tempcol.fieldLength = atoi(word.c_str());
						if(tempcol.fieldLength > 255 || tempcol.fieldLength < 1)
						{
							operation = CHARBOUD;
							return;
						}
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(strcmp(word.c_str(),")")!=0)
							return;
						flag = getWord(temp, word);
						if(!flag)
							return;
						if(strcmp(word.c_str(),"unique")==0)
						{
							tempcol.isUnique = 1;
							flag = getWord(temp, word);
							if(!flag)
								return;
						}
					}
					else return;
					getTableInfo.fields.push_back(tempcol);//����Ϊֹ���ѻ��table�е�name��attributes����
					//����Ҫ��api�м����attriNum��rowSize
					tempcol.isPrimeryKey=tempcol.isUnique=0;
				}
			}
			if(strcmp(word.c_str(),")")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)
				operation = CRETAB;
		}
		//����create index����
		else if(strcmp(word.c_str(),"index")==0)
		{
			operation = CREINDERR;
			flag = getWord(temp, word);
			if(!flag)
				return;
			indexName=word;
			getIndexInfo.indexName=word;
			if(catalog.existIndex(indexName)){
				operation=INDEXERROR;
				return;
			}
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"on")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)
				return;
			tableName  = word;
			if(!catalog.existTable(word)){
				operation=TABLEERROR;
				return;
			}
			getTableInfo=catalog.getTable(tableName);
			/*for(int i=0;i<getTableInfo.attributes.size();i++){
				if(getTableInfo.attributes[i].isPrimeryKey==true){
					getIndexInfo.columnLength=getTableInfo.attributes[i].length;
				}
			}*/
			getIndexInfo.tableName=word;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"(")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)
				return;
			int tempint;
			tempint=catalog.getColNum(getTableInfo,word);
			if(tempint==-1){
				operation=COLUMNERROR;
				return;
			}
			if(catalog.existIndex(tableName,tempint)){
				operation=INDEXERROR;
				return;
			}
			getIndexInfo.column=tempint;
			getIndexInfo.blockNum=0;
			getIndexInfo.columnLength=getTableInfo.fields[tempint].fieldLength;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),")")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)
				operation = CREIND;
		}
	}
	//����delete from����
	else if(strcmp(word.c_str(),"delete")==0)
	{
		flag = getWord(temp, word);
		if(!flag)
			return;
		if(strcmp(word.c_str(),"from")==0)
		{
			operation = DELETEERR;
			flag = getWord(temp, word);
			if(!flag)
				return;
			tableName  = word;
			if(!catalog.existTable(word)){
				operation=TABLEERROR;
				return;
			}
			flag = getWord(temp, word);
			//��û��where�Ӿ䣬��ȷ����
			getTableInfo=catalog.getTable(tableName);
			if(!flag)
			{
				operation = DELETE;
				return;
			}
			if(strcmp(word.c_str(),"where")!=0)
				return;
			//��ʼ����where�Ӿ�
			flag = getWord(temp, word);
			if(!flag)
				return;
			int tempint;
			tempint=catalog.getColNum(getTableInfo,word);
			if(tempint==-1){
				operation=COLUMNERROR;
				return;
			}
			tempcon.columnNum=tempint;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"<")==0)
				tempcon.op = Lt;
			else if(strcmp(word.c_str(),"<=")==0)
				tempcon.op = Le;
			else if(strcmp(word.c_str(),">")==0)
				tempcon.op = Gt;
			else if(strcmp(word.c_str(),">=")==0)
				tempcon.op = Ge;
			else if(strcmp(word.c_str(),"=")==0)
				tempcon.op = Eq;
			else if(strcmp(word.c_str(),"<>")==0)
				tempcon.op = Ne;
			else return;
				
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"\'")==0)
			{
				flag = getStr(temp, word);
				if(!flag)
					return;
				tempcon.value=word;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"\'")!=0)
					return;
			}
			else
			{
				tempcon.value=word;
			}
			condition.push_back(tempcon);
			flag = getWord(temp, word);
			if(!flag)
			{
				operation = DELETE;
				return;
			}
			while(strcmp(word.c_str(),"and")==0)
			{
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(catalog.getColNum(getTableInfo,word)==-1){
					operation=COLUMNERROR;
					return;
				}
				tempcon.columnNum=catalog.getColNum(getTableInfo,word);
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"<")==0)
					tempcon.op = Lt;
				else if(strcmp(word.c_str(),"<=")==0)
					tempcon.op = Le;
				else if(strcmp(word.c_str(),">")==0)
					tempcon.op = Gt;
				else if(strcmp(word.c_str(),">=")==0)
					tempcon.op = Ge;
				else if(strcmp(word.c_str(),"=")==0)
					tempcon.op = Eq;
				else if(strcmp(word.c_str(),"<>")==0)
					tempcon.op = Ne;
				else return;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"\'")==0)
				{
					flag = getStr(temp, word);
					if(!flag)
						return;
					tempcon.value=word;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"\'")!=0)
						return;
				}
				else
				{
					tempcon.value=word;
				}
				condition.push_back(tempcon);
				flag = getWord(temp, word);
				if(!flag)
				{
					operation = DELETE;
					return;
				}
			}	
		}
	}
	//����insert into����
	else if(strcmp(word.c_str(),"insert")==0)
	{
		flag = getWord(temp, word);
		if(!flag)
			return;			
		if(strcmp(word.c_str(),"into")==0)
		{
			operation = INSERTERR;
			flag = getWord(temp, word);
			if(!flag)	
				return;
			tableName  = word;
			if(!catalog.existTable(word)){
				operation=TABLEERROR;
				return;
			}
			getTableInfo=catalog.getTable(tableName);
			
			flag = getWord(temp, word);
			if(!flag)	
				return;
			if(strcmp(word.c_str(),"values")!=0)
				return;
			flag = getWord(temp, word);
			if(!flag)	
				return;
			if(word != "(")
				return;
			flag = getWord(temp, word);
			if(!flag)
				return;
			if(strcmp(word.c_str(),"\'")==0)
			{
				flag = getStr(temp, word);
                //cout << "WD" << word << endl;
				if(!flag)
					return;
				temprow=word;
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"\'")!=0)
					return;
			}
			else
			{
				temprow=word;
			}
			flag = getWord(temp, word);
			if(!flag)
				return;
			row.columns.push_back(temprow);
			while(strcmp(word.c_str(),",")==0)
			{
				
				flag = getWord(temp, word);
				if(!flag)
					return;
				if(strcmp(word.c_str(),"\'")==0)
				{
					flag = getStr(temp, word);
					if(!flag)
						return;
					temprow=word;
					flag = getWord(temp, word);
					if(!flag)
						return;
					if(strcmp(word.c_str(),"\'")!=0)
						return;
				}
				else
				{
					temprow=word;
				}
				row.columns.push_back(temprow);
				flag = getWord(temp, word);
				if(!flag)
					return;
			}
			if(word != ")")
				return;
			if(row.columns.size()!=catalog.getColAmo(getTableInfo)){
				operation=INSERTNUMBERERROR;
				return;
			}
			//if(primaryKey!=-1){
			//	tempcon.op=Eq;
			//	tempcon.columnNum=primaryKey;
			//	tempcon.value=row.columns[primaryKey];
			//	condition.push_back(tempcon);//���������ж��Ƿ����ظ���PrimaryKey
			//}
			flag = getWord(temp, word);
			for(int i=0;i<getTableInfo.fields.size();i++){
				if(getTableInfo.fields[i].isPrimeryKey){
					primaryKey=i;
					tempcon.columnNum=i;
					tempcon.op=Eq;
					tempcon.value=row.columns[i];
					condition.push_back(tempcon);
				}
				if(getTableInfo.fields[i].isPrimeryKey!=1&&getTableInfo.fields[i].isUnique){
					unique=i;
					tempcon.columnNum=i;
					tempcon.op=Eq;
					tempcon.value=row.columns[i];
					uniqueCondition.push_back(tempcon);
				}
			}
			if(!flag){
				operation  = INSERT;
				return;
			}
		}
	}
	//����drop table/drop index����
	else if(strcmp(word.c_str(),"drop")==0)
	{
		flag = getWord(temp, word);
		if(!flag)
			return;
		if(strcmp(word.c_str(),"table")==0)
		{
			operation = DRPTABERR;
			flag = getWord(temp, word);
			if(!flag)
				return;
			tableName  = word;
			if(!catalog.existTable(tableName)){
				operation=TABLEERROR;
				return;
			}
			getTableInfo=catalog.getTable(tableName);
			flag = getWord(temp, word);
			if(!flag)
				operation = DRPTAB;
		}
		else if(strcmp(word.c_str(),"index")==0)
		{
			operation = DRPINDERR;
			flag = getWord(temp, word);
			if(!flag)
				return;
			indexName=word;
			if(!catalog.existIndex(indexName)){
				operation=INDEXEROR;
				return;
			}
			flag = getWord(temp, word);
			if(!flag)
				operation = DRPIND;
			//getIndexInfo=catalog.getIndex(
		}	

	}
	return;
}

//This function is used to check is the string 'input' represented an int variable
//return 1 if is, 0 otherwise
short int Interpret::isInt(const char *input)
{
	int i;
	int length = strlen(input);
	if(!isdigit(input[0]) && !(input[0] == '-'))
		return 0;
	for(i = 1 ; i < length ; i ++)
	{
		if(!isdigit(input[i]))
			return 0;
	}
	return 1;
}

//This function is used to check is the string 'input' represented an float variable
//return 1 if is, 0 otherwise
short int Interpret::isFloat(char *input)
{
		int dot = 0;
		int i;
		int length = strlen(input);
		if(!isdigit(input[0]) && !(input[0] == '-'))
			return 0;
		for(i = 1 ; i < length ; i ++)
		{
			if(!isdigit(input[i]) &&  input[i] != '.')
				return 0;
			else if(input[i] == '.')
			switch(dot)
			{
				case 0:
					dot++;
					break;
				default:
					return 0;
			}
		}
		return 1;
}

int Interpret::isEnd(char *input)
{
	unsigned int next = strlen(input) - 1;
	char prev = ' ';
	while(next >= 0 && (prev == '\t' || prev ==' '))
	{
		prev = input[next];
		next --;
	}
	if(prev == ';')
	{
		input[next + 1] ='\0';
		return 1;
	}
	return 0;
}

vector<string> Interpret::convert(Row r) {
    vector<string> row;


    for(int j=0;j<r.columns.size();j++) {
        //for(int i=0;i<getTableInfo.fields.size();i++) {
            stringstream ss;
            ss << setw(getTableInfo.fields[j].fieldLength) << setfill('!') << r.columns[j];
            string temp;
            ss >> temp;
            row.push_back(temp);
        }

    return row;
}

void Interpret::exec()
{
	int i;
	int j;
	int k;
	TableInfo tableinfor;
	Index indexinfor;
	string tempKeyValue;
	int tempPrimaryPosition=-1;
	int rowCount=0;
//	Data data;
	switch(operation)
	{
	case CRETAB:
		getTableInfo.fieldNum=getTableInfo.fields.size();
		catalog.createTable(getTableInfo);

		api.createTable(getTableInfo);

		cout<<"Table "<<getTableInfo.tableName<<" has been successfully created !"<<endl;
		break;
	case DRPTAB:
//		record.dropTable(getTableInfo);
//		for(int i = 0; i < getTableInfo.attriNum; i++){
//			indexinfor = catalog.getIndex(getTableInfo.name, i);
//			if(indexinfor.index_name != "")
//				index.dropIndex(indexinfor);
//		}
		api.dropTable(getTableInfo);

		catalog.dropTable(getTableInfo);
		cout<<"Table "<<getTableInfo.tableName<<" has been successfully dropped !"<<endl;
		break;
	case INSERT:
		tableinfor = getTableInfo;
            cout << primaryKey << " " << unique << endl;
		if(primaryKey==-1&&unique==-1){  //???
//			record.insertValue(tableinfor, row);
			catalog.update(tableinfor);
			cout<<"Record has been successfully inserted !"<<endl;
			break;
		}
		if(primaryKey!=-1)
		{
//			data=record.select(tableinfor, condition);
//			if(data.rows.size()>0){
//				cout<<"Primary Key Redundancy occurs, thus insertion failed"<<endl;
//				break;
//			}
		}
		if(unique!=-1){

//			data=record.select(tableinfor, uniqueCondition);
//			if(data.rows.size()>0){
//				cout<<"Unique Value Redundancy occurs, thus insertion failed"<<endl;
//				break;
//			}
		}
//		record.insertValue(tableinfor,row);
            //vector<string> data = row.columns;
            //vector<string> data;// = row.getData();

           // for (int i=0; i<row.columns.size(); i++)
             //   cout << row.columns[i] << endl;
		//api.insert(getTableInfo, (vector<string>) row.columns);
            api.insert(getTableInfo, convert(row));

		catalog.update(tableinfor);
		cout<<"Record has been successfully inserted !"<<endl;
		break;

	case SELECT_NOWHERE_CAULSE:
		tableinfor = getTableInfo;
//		data=record.select(tableinfor);
//		if(data.rows.size()!=0)
//			ShowResult( data, tableinfor, column);
//		else{
//			cout << "No data is found!!!" << endl;
//		}
        //vector<Condition> conds0;
          //  conds0.push_back(Condition(true));
            //api.select(getTableInfo,  conds0);

            api.select(getTableInfo);
		break;
	case SELECT_WHERE_CAULSE:
		tableinfor=getTableInfo;
		if(condition.size()==1){
			for(int i=0;i<getTableInfo.fields.size();i++){
				if((getTableInfo.fields[i].isPrimeryKey==true||getTableInfo.fields[i].isUnique==true)&&colName==getTableInfo.fields[i].fieldName){
					tempPrimaryPosition=i;
					indexinfor=catalog.getIndex(tableinfor.tableName,i);
					break;
				}
			}/*
        vector<Condition> conds = vector<Condition> ();
            for (int i=0; i<condition.size(); i++)
                conds.push_back(Condition(condition[i]));
		api.select(getTableInfo,  conds);*/
        }
            api.select(getTableInfo, condition);

		break;
	case DELETE:
//		rowCount = record.deleteValue(getTableInfo,condition);
//		cout<< rowCount <<"  rows have been deleted."<<endl

       /*     vector<Condition> del_conds;
            if (condition.size() == 0) ;//del_conds.push_back(Condition(true));
            else {
                for (int i = 0; i < condition.size(); i++)
                    del_conds.push_back(Condition(condition[i]));
            }*/
		api.del(getTableInfo, condition);


		break;
	case CREIND:
		tableinfor = getTableInfo;
		indexinfor = getIndexInfo;
		if(!tableinfor.fields[indexinfor.column].isPrimeryKey&&!tableinfor.fields[indexinfor.column].isUnique){
			cout << "Column " << tableinfor.fields[indexinfor.column].fieldName <<"  is not unique."<< endl;
			break;
		}
		catalog.createIndex(indexinfor);

            //api.createIndex(indexinfor, tableinfor);

		catalog.update(indexinfor);
		cout<<"Index "<< indexinfor.indexName << "has been successfully created !"<<endl;
		break;

	case DRPIND:
		indexinfor = catalog.getIndex(indexName);
		if(indexinfor.indexName == ""){
			cout << "Index" << indexName << "does not exist !" << endl;
		}

		//API.dropIndex(indexinfor);

		catalog.dropIndex(indexName);
		cout<<"Index has been successfully dropped !"<<endl;
		break;

	case INSERTERR:
		cout << "\"Insert\" Error! Please re-enter!" << endl;
		break;
	case TABLEEXISTED:
		cout<<"The table has existed, please re-enter !"<<endl;
		break;
	case INDEXERROR:
		cout<<"The index has existed !"<<endl;
		break;
	case CREINDERR:
		cout << "\"Create index\" Error! Please re-enter !" << endl;
		break;
	case QUIT:
		cout << "Bye !Press any key to exit !" << endl;
		getchar();
		exit(0);
		break;
	case EMPTY:
		cout << "Query Empty ! Please re-enter!" << endl;
		break;
	case UNKNOW:
		cout << "Query UNKNOW ! Please re-enter !" << endl;
		break;
	case SELERR:
		cout << "\"Select\" Error! Please re-enter !" << endl;
		break;
	case CRETABERR:
		cout << "\"Create table\" Error! Please re-enter !" << endl;
		break;
	case DELETEERR:
		cout << "\"Delete from\" Error! Please re-enter !" << endl;
		break;
	case DRPTABERR:
		cout << "\"Drop table\" Error! Please re-enter !" << endl;
		break;
	case DRPINDERR:
		cout << "\"Drop index\" Error! Please re-enter !" << endl;
		break;
	case VOIDPRI:
		cout << "Primary key Error! Please re-enter !" << endl;
		break;
	case VOIDUNI:
		cout << "Unique key Error! Please re-enter !" << endl;
		break;
	case CHARBOUD:
		cout << "Char length Error! Please re-enter !" << endl;
		break;
	case NOPRIKEY:
		cout << "No primary key! Please re-enter !" << endl;
		break;
	case TABLEERROR:
		cout<<"Table not existed! Please re-enter !"<<endl;
		break;
	case INDEXEROR:
		cout<<"Index not existed! Please re-enter !"<<endl;
		break;
	case COLUMNERROR:
		cout<<"Column not existed! Please re-enter !"<<endl;
		break;
	case INSERTNUMBERERROR:
		cout<<"Column number Error! Please re-enter !"<<endl;
		break;
	}

}
