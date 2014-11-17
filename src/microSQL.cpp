#include "utils/Interpreter.h"
#include "utils/Request.h"
#include "utils/API.h"
#include <iostream>

using namespace std;

int mian0() {
    string s, command;
    do {
        getline(cin, s);
        command = command + s;
    } while (s[s.length()-1] != ';');
    Request& request = Interpreter::interpret(command);
    request.excute();
}