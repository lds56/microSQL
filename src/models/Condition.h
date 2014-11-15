#ifndef  CONDITION_H_
#define CONDITION_H_

#include<string>
#include"models/TableRow.h"

using namespace std;

/*
class Conditions{
public:
    Conditions(vector<Condition> conds): conds(conds) {}
    bool check(string attri) {
        for (int i=0; i<conds.size(); i++)
            if (!conds[i].check(attri)) return false;
        return true;
    }
private:
    vector<Condition> conds;
};
*/

enum Comparison {
    gt,   //> value
    lt,
    eq,
    ne,
    ge,
    le
};

class Condition{
public:
    Condition(Comparison op, bool allTrue, string value, int kthAttri):
            op(op), allTrue(allTrue), value(value), kthAttri(kthAttri) {}
    Condition(Comparison op, string value, int kthAttri):
            op(op), allTrue(false), value(value), kthAttri(kthAttri) {}
    Condition(bool allTrue): allTrue(allTrue) {}
    //Condition(int kthAttri): allTrue(false), kthAttri(kthAttri) {}
    bool check(string attri) {
        if (allTrue) return true;
        else {
            switch (op) {
                case (Comparison::gt): return (attri > value);
                case (Comparison::lt): return (attri < value);
                case (Comparison::eq): return (attri == value);
                case (Comparison::ne): return (attri != value);
                case (Comparison::ge): return (attri >= value);
                case (Comparison::le): return (attri <= value);
            }
        }
    }
    int getKthAttri() { return kthAttri; }

private:
    Comparison op;
    string value;
    bool allTrue;
    int kthAttri;
};

#endif