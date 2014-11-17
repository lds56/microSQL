#include <string>
#include "utils/Request.h"

using namespace std;

class Interpreter {
public:
    static Request& interpret(string command);
};