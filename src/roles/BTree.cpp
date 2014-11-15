#include "BTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <models/TableInfo.h>
#include <models/Index.h>
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

void BTree::delete_file(Index& indexinfo){

	//string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
	//DeleteFile(filename.c_str());
}

void BTree::create_file(Index& indexinfo, TableInfo& tablein){
    //ofstream of;

    //string fileName = string(indexinfo.indexName + ".index");
    //FILE* fp;
    //fopen(fp, fileName, "");
    //fclose(fp);
	  //string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
	  //CreateFile(filename.c_str(),GENERIC_WRITE,FILE_SHARE_READ, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL );

	  /*  string key;
	   * for (each tablerow in table) {
	   * key=tablerow.field
	     insert(key, address)
	   }
	   */
}

void BTree::select_between(string low, string high){
	/* cond condition;   for select
	for (each block in index)
		if (block.leaf){
			for (each b in block)
		    condition.check(b.key)
		}*/
}

void BTree::select_larger(string s){
	select_between(s, "max");
}

void BTree::select_smaller(string s){
	select_between("min" ,s);
}

void BTree::select_qual(string s){
	search(s,root);
    int address=str.blk[blkn].b[bias].child;
}

void BTree::ReadFromFile(Index& indexinfo){
    //string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
    string filename = indexinfo.indexName + ".index";
    ifstream in(filename.c_str());
    string s;
    getline(in,s);
    str.recordnumber=StringToNumber(s);
    int j=0; int ind; string s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); blkn=StringToNumber(s1);; j++; s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); bias=StringToNumber(s1);; j++; s1="";
    ind=j; while (s[j]!=' ') j++; s1=s.substr(ind,(j-ind)); root=StringToNumber(s1);; j++; s1="";
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
	for (int i=1; i<100; i++){
		getline(in,s);
		for (int j=1; j<100; j++){
			lazy[i][j]=s[j*2-2]-'0';
		}
    }

}

void BTree::WriteToFile(Index& indexinfo){
	//string filename="C:\\Users\\Administrator\\Desktop\\"+indexinfo.indexName+".index";
    string filename = indexinfo.indexName+".index";
    cout << filename << endl;
	ofstream out(filename.c_str());
	out << blkn << " " << bias << " " << root << " " << str.recordnumber << " " << endl;
	for (int i=1; i<=str.recordnumber; i++){
		out << str.blk[i].isLeaf << " " << str.blk[i].isRoot << " " << str.blk[i].father << " " << str.blk[i].length << " ";
		for (int j=0; j<=str.blk[i].length; j++)  out << str.blk[i].b[j].key << " " << str.blk[i].b[j].child << " ";
		out << endl;
	}

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

void  BTree::insert(string s){
  search(s,root);
  str.blk[blkn].length++;
  for (int i=str.blk[blkn].length; i>bias; i--){
	  str.blk[blkn].b[i]=str.blk[blkn].b[i-1];
  }
  str.blk[blkn].b[bias].key=s;
  if (str.recordnumber==0) str.recordnumber++;
  if (str.blk[blkn].length>=MAX){
	  if (str.blk[blkn].isLeaf) l_split();
	  else b_split();
  }
}


void BTree::del(string s){
	search(s,root);
	if (lazy[blkn][bias]==1){
		cout << "Invalid Command!" << endl;
	}
	else {
		lazy[blkn][bias]=1;
	}
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
