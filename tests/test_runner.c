#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "special_math.h"

void test_big_add() {
    char* res = big_add("999", "1");
    assert(strcmp(res, "1000") == 0);
    free(res);
    printf("[TEST PASSED] big_add\n");
}

void test_big_sub() {
    char* res = big_sub("1000", "1");
    assert(strcmp(res, "999") == 0);
    free(res);
    printf("[TEST PASSED] big_sub\n");
}

void test_big_mul() {
    char* res = big_mul("50", "50");
    assert(strcmp(res, "2500") == 0);
    free(res);
    printf("[TEST PASSED] big_mul\n");
}

void test_big_factorial() {
    char* res = big_factorial(5);
    assert(strcmp(res, "120") == 0);
    free(res);
    printf("[TEST PASSED] big_factorial\n");
}

void test_big_power() {
    char* res = big_power("2", 8);
    assert(strcmp(res, "256") == 0);
    free(res);
    printf("[TEST PASSED] big_power\n");
}

int main() {
    printf("Запуск юнит-тестов...\n");
    test_big_add();
    test_big_sub();
    test_big_mul();
    test_big_factorial();
    test_big_power();
    printf("Все тесты успешно пройдены!\n");
    return 0;
}
