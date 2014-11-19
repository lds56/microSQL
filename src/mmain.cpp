#include <iostream>
#include <unistd.h>
#include <string>
#include "Macro.h"
#include "Struct.h"
#include "roles/CatalogManager.h"
#include "Interpret.h"
#include <ctime>

using namespace std;

CatalogManager catalog;
Interpret parser;

int main() {

    BufferManager::status = "ENABLE";
    BufferManager::initBlocks();

    //char sss[100];
    //getcwd(sss, 100);
    //puts(sss);
	cout << "Welcome to MiniSQL !" << endl;
	vector<Status> conditions;
	TableInfo tableinfor;
	Index indexinfor;
	Row insertValue;
//	Data datas;
	char command[COMLEN] = "";
	char input[INPUTLEN] = "";
	char word[WORDLEN] = "";
	short int ComEnd = 0;

	while(1)
	{
		strcpy(command, "");
		ComEnd = 0;
		cout << "MiniSQL>>";
 		while(!ComEnd)
		{

			gets(input);
            if (input[0] == 'e') {
                string filename;
                for (int i=9; i< INPUTLEN; i++)
                    if (input[i] == ';') break;
                    else filename += input[i];
                vector<string> deal;
                //openfile();
                ifstream in(filename.c_str());
                string s;
                getline(in,s);
                while (s!=""){
                    deal.push_back(s);
                    getline(in,s);
                }
                //cout << "-----" << endl;
                for (int i=0; i<deal.size()-1; i++) {
                    deal[i][deal[i].length()-1] = ' ';
                    char command2[COMLEN];
                    memset(command2, 0, sizeof command2);
                    strcpy(command2, deal[i].c_str());
                    //unsigned len = strlen(command2);
                    //command2[len] = ' ';
                    //command2[len + 1] = '\0';
                    //deal[i].substr(0, deal[i].length()-1)
                    //cout << len <<endl;
                    //cout << string() << endl;
                    parser.parsing(command2);
                    parser.exec();
                }

               // if (BufferManager::status == "ENABLE")
               //cout << "total time: " << clock() << endl;

            } else {
                if (parser.isEnd(input))
                    ComEnd = 1;
                strcat(command, input);
                unsigned len = strlen(command);
                command[len] = ' ';
                command[len + 1] = '\0';
                //cout << string(command) << endl;
                parser.parsing(command);
                parser.exec();
            }
		}
	}
	getchar();
    return 0;
}
