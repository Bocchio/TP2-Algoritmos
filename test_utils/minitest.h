#include "../types.h"

#define RUN_TEST_MSG "Running test: "

typedef void (*test_fun_t)(void);

typedef struct {
	char * test_name;
	test_fun_t test_fun;
} test_t;

#define MINI_TEST(test) \
	void fun_##test (void);\
	test_t test = { #test , fun_##test } ; \
	void fun_##test (void)

void assert_int_eq(int a, int b);
void assert_int_eq_msg(int a, int b, string msg);
void assert_str_eq(string a, string b);
void assert_str_eq_msg(string a, string b, string msg);
void _run_tests(test_t *tests, size_t n);

#define run_tests(tests) _run_tests(tests, sizeof(tests)/sizeof(*tests))