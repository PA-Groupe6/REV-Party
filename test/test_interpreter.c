#include <stdbool.h>
#include "../src/interpreter.h"
#include "test_utils.h"

#define printsb(msg) addLineStringBuilder(string_builder, msg)

StringBuilder* string_builder;
int return_value;

void beforeAll() {
    string_builder = createStringBuilder();
}

void afterEach() {
    emptyStringBuilder(string_builder);
}

void testInterprete() {
    int argc;
    char* cmd = "interprete";
    char* file = "test/ressource/unit/d1.csv";
    char* dflag = "-d";
    char* iflag = "-i";
    char* jflag = "-j";
    char* oflag = "-o";
    char* offlag = "test/ressource/unit/res.log";

    printsb("\ntest sur \"interprete -d test/ressource/unit/d1.csv\"");
    argc = 3;
    char* argv_d[argc];

    Command cmd = interprete(3, argv_d);
}

void test_fun(bool(*f)(), int fnb, char* fname) {
    bool test_success = f();
    afterEach();
    if (!test_success) {
        return_value += fnb;
        printFailure(fname);
        printStringBuilder(string_builder);
    } else printSuccess(fname);
}

int main() {
    beforeAll();

    test_fun(testInterprete, 1, "testInterprete");

    return return_value;
}