#ifndef CONDITION_H_
#define CONDITION_H_

#include<string>
#include "Struct.h"
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


class Condition{
public:
    Condition(Status st): op(st.op), value(st.value), kthAttri(st.columnNum) {}
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
                case (Comparison::Gt): return (attri > value);
                case (Comparison::Lt): return (attri < value);
                case (Comparison::Eq): return (attri == value);
                case (Comparison::Ne): return (attri != value);
                case (Comparison::Ge): return (attri >= value);
                case (Comparison::Le): return (attri <= value);
            }
        }
    }
    int getKthAttri() { return kthAttri; }
    Comparison op;
    string value;
    bool allTrue;
    int kthAttri;
};

#endif