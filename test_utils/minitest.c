#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "minitest.h"

void assert_int_eq(int a, int b)
{
	assert(a == b);
}

void assert_int_eq_msg(int a, int b, string msg)
{
	if(a != b)
		fputs(msg, stderr);
	assert(a == b);
}

void assert_str_eq(string a, string b)
{
	int cmp = !strcmp(a, b);
	assert(cmp);
}

void assert_str_eq_msg(string a, string b, string msg)
{
	int cmp = !strcmp(a, b);
	if(cmp)
		fputs(msg, stderr);
	assert(cmp);
}

void _run_tests(test_t *tests, size_t n)
{
	size_t i;

	for(i = 0; i < n; i++){
		printf("%s%s\n", RUN_TEST_MSG, tests[i].test_name);
		tests[i].test_fun();
	}
}