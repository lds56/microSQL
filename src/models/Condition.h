#ifndef  CONDITION_H_
#define CONDITION_H_

#include<string>
#include"models/TableRow.h"

using namespace std;

class Condition{
public:
    Condition(string mask): mask(mask) {}
    bool check(TableRowPtr rPtr) {
        if (mask == "TRUE") return true;
        else return true;
    }

private:
    string mask;

};

#endif