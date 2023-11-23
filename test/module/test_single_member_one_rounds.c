#include "../../src/module/single_member.h"
#include "../test_utils.h"

StringBuilder* string_builder;
int return_value;










void beforeAll() {

}


void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterAll() {
    deleteStringBuilder(&string_builder);
}


void afterEach() {
}








bool testtheWinnerOneRound(){
    beforeEach();
    
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

int main(){
    beforeAll();

    test_fun(testtheWinnerOneRound, 1, "testtheWinnerOneRound");
}