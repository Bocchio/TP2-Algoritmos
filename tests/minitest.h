#ifndef MINITEST__H
#define MINITEST__H

#include "../types.h"

#define RUN_TEST_MSG 		"Running test"
#define FAILURE_TEST_MSG 	"Error"
#define TESTS_FAILED_MSG	"Failed"
#define TESTS_SUCCEDED_MSG	"Succeded"
#define TESTS_RUN_MSG		"Total tests run"
#define MINI_TEST(test) { #test , test }
#define TEST_MSG_BUFFER_SIZE		100
#define run_tests(tests) _run_tests(tests, sizeof(tests)/sizeof(*tests), #tests )

typedef enum {
	TEST_FAILURE,
	TEST_SUCCESS
} test_status_t;

typedef test_status_t (*test_fun_t)(void);

typedef struct {
	char * test_name;
	test_fun_t test_fun;
} test_t;

test_status_t assert_int_eq(int a, int b);
test_status_t assert_int_eq_msg(int a, int b, string msg);
test_status_t assert_str_eq(string a, string b);
test_status_t assert_str_eq_msg(string a, string b, string msg);
void _run_tests(test_t *tests, size_t n, char *suite_name);
void _make_test(test_t *test, const char * name, test_fun_t fun);

#endif