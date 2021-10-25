#include <stdlib.h>

#include "unity/unity.h"
#include "../src/fstack.h"
#include "../src/cal.h"

long d = -1;
FStack n, o;

#define D &d
#define S &str[0]
#define N &n
#define O &o

void setUp() {
    d = -1;
    n = new_fstack(3);
    o = new_fstack(3);
}

void tearDown() {}

static void test_empty_and_null_strings() {
    char str[1] = "";
    TEST_ASSERT_FALSE(parse_str_to_int(S, D));
    TEST_ASSERT_FALSE(parse_str_to_int(NULL, D));
}

static void test_basic_str_to_int() {
    char str[] = "1";

    TEST_ASSERT_TRUE(parse_str_to_int(S, D));
    TEST_ASSERT_EQUAL(1, d);
}

static void test_parses_number_just_fine() {
    char str[] = "123";

    TEST_ASSERT_TRUE(parse_str_to_int(S, D));
    TEST_ASSERT_EQUAL(123, d);
}

static void test_parses_number_just_fine_again() {
    char str[] = "7658";

    TEST_ASSERT_TRUE(parse_str_to_int(S, D));
    TEST_ASSERT_EQUAL(7658, d);
}

static void test_bigger_than_permitted_number() {
    char str[] = "174698273";

    TEST_ASSERT_FALSE(parse_str_to_int(S, D));
}

static void test_operate_plus() {
    push(N, 1);
    push(N, 2);
    push(O, OP_PLUS);

    TEST_ASSERT_EQUAL(STATE_OK, operate(N, O, D));
    TEST_ASSERT_EQUAL(3, d);
}

static void test_operate_minus() {
    push(N, 9);
    push(N, 6);
    push(O, OP_MINUS);

    TEST_ASSERT_EQUAL(STATE_OK, operate(N, O, D));
    TEST_ASSERT_EQUAL(3, d);
}

static void test_operate_product() {
    push(N, 10);
    push(N, -2);
    push(O, OP_PRODUCT);

    TEST_ASSERT_EQUAL(STATE_OK, operate(N, O, D));
    TEST_ASSERT_EQUAL(-20, d);
}

static void test_operate_division_ok() {
    push(N, 10);
    push(N, 2);
    push(O, OP_DIVISION);

    TEST_ASSERT_EQUAL(STATE_OK, operate(N, O, D));
    TEST_ASSERT_EQUAL(5, d);
}

static void test_operate_division_by_zero() {
    push(N, 10);
    push(N, 0);
    push(O, OP_DIVISION);

    TEST_ASSERT_EQUAL(STATE_DIVISION_BY_ZERO, operate(N, O, D));
}

static void test_evaluate_simple_sum() {
    char str[] = "1 + 1";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(2, d);
}

static void test_evaluate_simple_rest() {
    char str[] = "1 - 1";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(0, d);
}

static void test_evaluate_simple_product() {
    char str[] = "1 * 2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(2, d);
}

static void test_evaluate_simple_division() {
    char str[] = "1 / 2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(0, d);
}

static void test_evaluate_many_sums() {
    char str[] = "1 + 2 + 3";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(6, d);
}

static void test_evaluate_many_rests() {
    char str[] = "1 - 2 - 3";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(-4, d);
}

static void test_evaluate_many_products() {
    char str[] = "1 * 2 * 6";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(12, d);
}

static void test_evaluate_many_divisions() {
    char str[] = "80 / 2 / 4";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(10, d);
}

static void test_evaluate_negative_numbers() {
    char str[] = "1 - - 2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(3, d);
}

static void test_evaluate_more_negative_numbers() {
    char str[] = "-1 * 2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(-2, d);
}

static void test_double_operator_only_applies_to_negative_numbers() {
    char str[] = "1 - + 2";

    TEST_ASSERT_NOT_EQUAL(STATE_OK, evaluate(S, D));
}

static void test_double_operator_only_applies_to_negative_numbers_2() {
    char str[] = "1 + -2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(-1, d);
}

static void test_operator_precedence_1() {
    char str[] = "1 + 2 * 3";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(7, d);
}

static void test_operator_precedence_2() {
    char str[] = "-4 * 3 - 2";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(-14, d);
}

static void test_operator_precedence_3() {
    char str[] = "-4 * -3";

    TEST_ASSERT_EQUAL(STATE_OK, evaluate(S, D));
    TEST_ASSERT_EQUAL(12, d);
}

int main() {
    UnityBegin("tests/fstack_tests.c");

    RUN_TEST(test_basic_str_to_int);
    RUN_TEST(test_empty_and_null_strings);
    RUN_TEST(test_bigger_than_permitted_number);
    RUN_TEST(test_parses_number_just_fine);
    RUN_TEST(test_parses_number_just_fine_again);
    RUN_TEST(test_operate_plus);
    RUN_TEST(test_operate_minus);
    RUN_TEST(test_operate_product);
    RUN_TEST(test_operate_division_ok);
    RUN_TEST(test_operate_division_by_zero);
    RUN_TEST(test_evaluate_simple_sum);
    RUN_TEST(test_evaluate_simple_rest);
    RUN_TEST(test_evaluate_simple_product);
    RUN_TEST(test_evaluate_simple_division);
    RUN_TEST(test_evaluate_many_sums);
    RUN_TEST(test_evaluate_many_rests);
    RUN_TEST(test_evaluate_many_products);
    RUN_TEST(test_evaluate_many_divisions);
    RUN_TEST(test_evaluate_negative_numbers);
    RUN_TEST(test_evaluate_more_negative_numbers);
    RUN_TEST(test_double_operator_only_applies_to_negative_numbers);
    RUN_TEST(test_double_operator_only_applies_to_negative_numbers_2);
    RUN_TEST(test_operator_precedence_1);
    RUN_TEST(test_operator_precedence_2);
    RUN_TEST(test_operator_precedence_3);

    return UnityEnd();
}
