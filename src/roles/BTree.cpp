#include "BTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>

//#include <models/TableInfo.h>
#include "struct.h"
//#include <Foundation/Foundation.h>

using namespace std;

int StringToNumber(string s){
	int sum=0;
	int len=s.size();
	for (int i=0; i<len; i++){
		sum=sum*10+s[i]-'0';
	}
	return sum;
}

/*void BTree::delete_file(Index& indexinfo){

	//string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
	//DeleteFile(filename.c_str());
}
*/
void BTree::create_file(Index indexInfo){
    //ofstream of;
    indexinfo = indexInfo;
    ofstream file(indexinfo.indexName + ".index");
    //file << "aaa" << endl;
    file.close();
    //string fileName = string(indexinfo.indexName + ".index");

	  //string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
	  //CreateFile(filename.c_str(),GENERIC_WRITE,FILE_SHARE_READ, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL );

}

vector<Address> BTree::select_between(string low, string high, bool leftFlag, bool rightFlag){
	vector<Address> avector;
	search(low,root);
	if (low=="min"){
		blkn=1;
		bias=1;
	}
	int s_blk=blkn; int s_bia=bias;
	if (leftFlag && str.blk[blkn].b[bias].key==low || low=="min"){
		Address address(indexinfo.indexName, str.blk[blkn].b[bias].child);
		if (lazy[blkn][bias]==0) avector.push_back(address);
	}

	if (high=="max"){
		int i = s_bia;
		while (i<=str.blk[s_blk].length){
			Address address(indexinfo.indexName, str.blk[s_blk].b[i].child);
			if (avector[0].getOffset()==str.blk[s_blk].b[i].child){ i++;  continue;}
			if (lazy[s_blk][i]==0) avector.push_back(address);
			i++;
		}
		i = str.blk[s_blk].b[0].child;
		while (str.blk[i].length>0){
			for (int j=1; j<=str.blk[i].length; j++){
				Address address(indexinfo.indexName, str.blk[i].b[j].child);
				if (lazy[i][j]==0) avector.push_back(address);
			}
			i = str.blk[i].b[0].child;
		}
	}
	else {
	search(high,root);
	int i = s_bia;
	while (i<=str.blk[s_blk].length){
		Address address(indexinfo.indexName, str.blk[s_blk].b[i].child);
		if (avector[0].getOffset()==str.blk[s_blk].b[i].child){ i++;  continue;}
		if (lazy[s_blk][i]==0) avector.push_back(address);
		i++;
	}
	i = str.blk[s_blk].b[0].child;
	while (i<blkn){
		int j=1;
		while (j<=str.blk[i].length){
			Address address(indexinfo.indexName, str.blk[i].b[j].child);
			if (lazy[i][j]==0) avector.push_back(address);
			j++;
		}
		i = str.blk[i].b[0].child;
	}

	if (blkn!=s_blk){
	  int j=1;
	  while (j<bias){
		  Address address(indexinfo.indexName, str.blk[blkn].b[j].child);
		  if (lazy[blkn][j]==0) avector.push_back(address);
		  j++;
	  }
	}
	if (rightFlag && (str.blk[blkn].b[bias].key==high)){
		Address address(indexinfo.indexName, str.blk[blkn].b[bias].child);
		if (lazy[blkn][bias]==0) avector.push_back(address);
	}
	}
	return avector;
}

void BTree::ReadFromFile(Index indexin){
    //string filename="C:\\Users\\Administrator\\Desktop\\"+indexin.indexName+".index";
    string filename = indexinfo.indexName + ".index";
    //string filename = indexinfo.indexName + ".index";
    ifstream in(filename.c_str());
    string s;
    //getline(in,s);
	//indexinfo.fieldName =s;
	getline(in,s);
	indexinfo.indexName =s;
	getline(in,s);
	indexinfo.tableName =s;
	getline(in,s);
	int j=0; int ind; string s1="";
	ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); indexinfo.blockNum=StringToNumber(s1);; j++; s1="";
	ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); indexinfo.columnLength=StringToNumber(s1);; j++; s1="";

    getline(in,s);
     j=0;  s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); blkn=StringToNumber(s1); j++; s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); bias=StringToNumber(s1); j++; s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); root=StringToNumber(s1); j++; s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); str.recordnumber=StringToNumber(s1); j++;
    for (int i=1; i<=str.recordnumber; i++){
    	j=0;
    	s="";
    	getline(in,s);
    	// former 4
    	if (s[j]=='1') str.blk[i].isLeaf=true; else str.blk[i].isLeaf=false; j+=2;
    	if (s[j]=='1') str.blk[i].isRoot=true; else str.blk[i].isRoot=false; j+=2;
    	ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind));  str.blk[i].father=StringToNumber(s1); j++;
    	ind=j; s1="";  while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); str.blk[i].length=StringToNumber(s1); j++;

    	// indexbranch
    	for (int k=0; k<=str.blk[i].length; k++){
    		ind=j;  s1=""; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); j++;
    		str.blk[i].b[k].key=s1;
    		ind=j;  s1=""; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); j++;
    		str.blk[i].b[k].child=StringToNumber(s1);
    	}
    }

    s=""; s1=" "; getline(in,s);
    int i=1; j=0; ind =0;
	while (str.blk[i].length>0){
		ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind));  str.blk[i].b[0].child=StringToNumber(s1); j++;
		i = str.blk[i].b[0].child;
	}

	for (int i=1; i<100; i++){
		getline(in,s);
		for (int j=1; j<100; j++){
			lazy[i][j]=s[j*2-2]-'0';
		}
    }

}

void BTree::WriteToFile(){
    string filename = indexinfo.indexName + ".index";
	//string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
    //string filename = indexinfo.indexName+".index";
	ofstream out(filename.c_str());
	//out << indexinfo.fieldName << endl;
    cout << "ooooo" << indexinfo.indexName << endl;
	out << indexinfo.indexName << endl;
	out << indexinfo.tableName << endl;
	out << indexinfo.blockNum << " " << indexinfo.columnLength << " " <<  endl;
	out << blkn << " " << bias << " " << root << " " << str.recordnumber << " " << endl;
	for (int i=1; i<=str.recordnumber; i++){
		out << str.blk[i].isLeaf << " " << str.blk[i].isRoot << " " << str.blk[i].father << " " << str.blk[i].length << " ";
		for (int j=0; j<=str.blk[i].length; j++)  out << str.blk[i].b[j].key << " " << str.blk[i].b[j].child << " ";
		out << endl;
	}

	int i=1;
	while (str.blk[i].length>0){
		out << str.blk[i].b[0].child << " ";
		i = str.blk[i].b[0].child;
	}
	out << endl;

	for (int i=1; i<100; i++){
		for (int j=1; j<100; j++){
			out << lazy[i][j] << " ";
		}
		out << endl;
	}
}

void BTree::b_split(){
	bool flag=true;
	//this branch
    str.blk[blkn].length=MAX/2;
    //another branch
    str.recordnumber++;
    str.blk[str.recordnumber].length=MAX-str.blk[blkn].length-1;  str.blk[str.recordnumber].isLeaf=0; str.blk[str.recordnumber].isRoot=str.blk[blkn].isRoot;
	for (int i=1; i<=str.blk[str.recordnumber].length; i++){
		str.blk[str.recordnumber].b[i].key=str.blk[blkn].b[str.blk[blkn].length+i+1].key;
		int children=str.blk[blkn].b[str.blk[blkn].length+i+1].child;
		str.blk[str.recordnumber].b[i].child=children;
		str.blk[children].father=str.recordnumber;
	}
	int MissChild= str.blk[blkn].b[str.blk[blkn].length+1].child;
	str.blk[MissChild].father=str.recordnumber; str.blk[str.recordnumber].b[0].child=MissChild;

	string InsertKey=str.blk[blkn].b[str.blk[blkn].length+1].key;
	int f_bias=1;

	if  ( str.blk[blkn].father==0){
		  int f_blkn=str.recordnumber+1;
		  root=f_blkn;
		  str.blk[blkn].father=root; str.blk[str.recordnumber].father=root;
		  str.blk[blkn].isRoot=false; str.blk[str.recordnumber].isRoot=false;
		  str.blk[root].b[1].key=InsertKey;
		  str.blk[root].b[0].child=blkn; str.blk[root].b[1].child=str.recordnumber;
		  str.blk[root].length=1; str.blk[root].isLeaf=0;
		  str.recordnumber++;
	}
	else {
		 int f_blkn=str.blk[blkn].father;

		 str.blk[f_blkn].length++;
		 str.blk[str.recordnumber].father=f_blkn;
				  for (int i=1; i<str.blk[f_blkn].length; i++){
					  if (InsertKey < str.blk[f_blkn].b[i].key) {
						  f_bias=i;
						  flag=false;
						  break;
					  }
				  }
				  if (flag) f_bias=str.blk[f_blkn].length;
				  for (int i=str.blk[f_blkn].length; i>f_bias; i--){
					  str.blk[f_blkn].b[i]=str.blk[f_blkn].b[i-1];

				  }
				  str.blk[f_blkn].b[f_bias].key=InsertKey;
				  str.blk[f_blkn].b[f_bias].child=str.recordnumber;
				  if (str.blk[f_blkn].length>=MAX){
					  blkn=f_blkn;
					  b_split();
				  }
	}

}

void BTree::l_split(){
	  bool flag=true;
	  //this leaf
	  str.blk[blkn].length=MAX/2;
	  //another leaf
	  str.recordnumber++;
	  str.blk[str.recordnumber].length=MAX-str.blk[blkn].length;  str.blk[str.recordnumber].isRoot=str.blk[blkn].isRoot;
	  str.blk[blkn].b[0].child=str.recordnumber;
	  for (int i=1; i<=str.blk[str.recordnumber].length; i++){
		  str.blk[str.recordnumber].b[i]=str.blk[blkn].b[str.blk[blkn].length+i];
	  }

	  int f_bias=1;
	  string InsertKey=str.blk[str.recordnumber].b[1].key ;

	  if (str.blk[blkn].father==0){
		  int f_blkn=str.recordnumber+1;
		  root=f_blkn;
		  str.blk[blkn].father=root; str.blk[str.recordnumber].father=root;
		  str.blk[blkn].isRoot=false; str.blk[str.recordnumber].isRoot=false;
		  str.blk[root].b[1].key=InsertKey; str.blk[root].length=1; str.blk[root].isLeaf=false;
		  str.blk[root].b[0].child=blkn; str.blk[root].b[1].child=str.recordnumber;
		  str.recordnumber++;
	  }
	  else{
		  int f_blkn=str.blk[blkn].father;
		  str.blk[f_blkn].length++;
		  str.blk[str.recordnumber].father=f_blkn;
		  for (int i=1; i<str.blk[f_blkn].length; i++){
			  if (InsertKey < str.blk[f_blkn].b[i].key) {
				  f_bias=i;
				  flag=false;
				  break;
			  }
		  }
		  if (flag) f_bias=str.blk[f_blkn].length;
		  for (int i=str.blk[f_blkn].length; i>f_bias; i--){
			  str.blk[f_blkn].b[i]=str.blk[f_blkn].b[i-1];
		  }
		  str.blk[f_blkn].b[f_bias].key=InsertKey;
		  str.blk[f_blkn].b[f_bias].child=str.recordnumber;
		  if (str.blk[f_blkn].length>=MAX){
			  blkn=f_blkn;
			  b_split();
		  }
      }
}

void BTree::search(string s, int bp){
  bool flag=true;
  if (!str.blk[bp].isLeaf){
	  for (int i=1; i<=str.blk[bp].length; i++){
         if (s<str.blk[bp].b[i].key) {
        	 search(s,str.blk[bp].b[i-1].child);
        	 flag=false;
        	 break;
         }
	  }
	  if (flag) search(s,str.blk[bp].b[str.blk[bp].length].child);
  }
  else{
      blkn=bp;
      for (int i=1; i<=str.blk[bp].length; i++){
           if (s<=str.blk[bp].b[i].key) {
        	   bias=i;
        	   flag=false;
        	   break;
           }
      }
      if (flag) bias=str.blk[bp].length+1;
  }
}

void  BTree::insert(string s, int offset){
  search(s,root);
  str.blk[blkn].length++;
  for (int i=str.blk[blkn].length; i>bias; i--){
	  str.blk[blkn].b[i]=str.blk[blkn].b[i-1];
  }
  str.blk[blkn].b[bias].key=s;
  str.blk[blkn].b[bias].child=offset;
  if (str.recordnumber==0) str.recordnumber++;
  if (str.blk[blkn].length>=MAX){
	  if (str.blk[blkn].isLeaf) l_split();
	  else b_split();
  }
}


vector<Address> BTree::del(string low, string high, bool leftFlag, bool rightFlag){
	vector<Address> avector;


	search(low,root);
	if (low=="min"){
		blkn=1;
		bias=1;
	}

	int s_blk=blkn; int s_bia=bias;
	if (leftFlag && str.blk[blkn].b[bias].key==low || low=="min"){
		Address address(indexinfo.indexName, str.blk[blkn].b[bias].child);
		if (lazy[blkn][bias]==0) {
			lazy[blkn][bias]=1;
			avector.push_back(address);
		}
	}

	if (high=="max"){
		int i = s_bia;
		while (i<=str.blk[s_blk].length){
			Address address(indexinfo.indexName, str.blk[s_blk].b[i].child);
			if (avector[0].getOffset()==str.blk[s_blk].b[i].child){ i++;  continue;}
			if (lazy[s_blk][i]==0) {
				lazy[s_blk][i]=1;
				avector.push_back(address);
			}
			i++;
		}
		i = str.blk[s_blk].b[0].child;
		while (str.blk[i].length>0){
			for (int j=1; j<=str.blk[i].length; j++){
				Address address(indexinfo.indexName, str.blk[i].b[j].child);
				if (lazy[i][j]==0) {
					lazy[i][j]=1;
					avector.push_back(address);
				}
			}
			i = str.blk[i].b[0].child;
		}
	}
	else {
	search(high,root);
	int i = s_bia;
	while (i<=str.blk[s_blk].length){
		Address address(indexinfo.indexName, str.blk[s_blk].b[i].child);
		if (avector[0].getOffset()==str.blk[s_blk].b[i].child){ i++;  continue;}
		if (lazy[s_blk][i]==0) {
			lazy[s_blk][i]=1;
			avector.push_back(address);
		}
		i++;
	}

	i = str.blk[s_blk].b[0].child;
	while (i<blkn){
		int j=1;
		while (j<=str.blk[i].length){
			Address address(indexinfo.indexName, str.blk[i].b[j].child);
			if (lazy[i][j]==0) {
				lazy[i][j]=1;
				avector.push_back(address);
			}
			j++;
		}
		i = str.blk[i].b[0].child;
	}

	if (blkn!=s_blk){
	  int j=1;
	  while (j<bias){
		  Address address(indexinfo.indexName, str.blk[blkn].b[j].child);
		  if (lazy[blkn][j]==0) {
			  lazy[blkn][j]=0;
			  avector.push_back(address);
		  }
		  j++;
	  }
	}
	if (rightFlag && (str.blk[blkn].b[bias].key==high)){
		Address address(indexinfo.indexName, str.blk[blkn].b[bias].child);
		if (lazy[blkn][bias]==0) {
			lazy[blkn][bias]=1;
			avector.push_back(address);
		}
	}
	}
	return avector;
}

void BTree::print(){
	/*for (int i=1; i<=str.recordnumber; i++){
		for (int j=1; j<=str.blk[i].length; j++){
			cout << str.blk[i].b[j].key;
		}
		cout << endl;
	}*/
	int bk[100];
	int ind=1; int end=1; int nm=0;
    bk[ind]=root;
    while (ind<=end){
    	nm=bk[ind];
    	if (str.blk[nm].b[0].child!=0){
    		end++;
    		bk[end]=str.blk[nm].b[0].child;
    	}
     for (int i=1; i<=str.blk[nm].length; i++){
    	cout << str.blk[nm].b[i].key;
    	if (str.blk[nm].b[i].child!=0){
    	end++;
    	bk[end]=str.blk[nm].b[i].child;}
     }
     cout << endl;
     ind++;
    }
}
