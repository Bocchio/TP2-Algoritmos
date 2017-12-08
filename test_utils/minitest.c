#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "minitest.h"

char msg_buffer[TEST_MSG_BUFFER_SIZE];

test_status_t assert_int_eq(int a, int b)
{
	if(a != b){
		sprintf(msg_buffer, "%d != %d\n", a, b);
		return TEST_FAILURE;
	}
	return TEST_SUCCESS;
}

test_status_t assert_int_eq_msg(int a, int b, string msg)
{
	if(a != b){
		sprintf(msg_buffer, "%d != %d\n", a, b);
		fputs(msg, stderr);
		return TEST_FAILURE;
	}
	return TEST_SUCCESS;
}

test_status_t assert_str_eq(string a, string b)
{
	int cmp;

	if(a == NULL && b == NULL)
		return TEST_SUCCESS;

	if((cmp = strcmp(a, b))){
		sprintf(msg_buffer, "%s != %s\n", a, b);
		return TEST_FAILURE;
	}
	
	return TEST_SUCCESS;
}

test_status_t assert_str_eq_msg(string a, string b, string msg)
{
	int cmp;

	if(a == NULL && b == NULL)
		return TEST_SUCCESS;

	if((cmp = strcmp(a, b))){
		sprintf(msg_buffer, "%s != %s\n", a, b);
		fputs(msg, stderr);
		return TEST_FAILURE;
	}
	
	return TEST_SUCCESS;
}

void show_test_failure(char *test_name)
{
	fprintf(stderr, "%s: %s: %s\n", FAILURE_TEST_MSG, test_name, msg_buffer);
}

void _run_tests(test_t *tests, size_t n, char *suite_name)
{
	size_t i;
	size_t test_failed = 0;
	size_t test_succeded = 0;

	putchar('\n');
	for(i = 0; i < n; i++){
		printf("\r%s%s %lu/%lu", RUN_TEST_MSG, suite_name, i+1, n);
		if(tests[i].test_fun() == TEST_FAILURE){
			show_test_failure(tests[i].test_name);
			test_failed++;
		}
		else
			test_succeded++;
	}
	putchar('\n');
	printf("%s: %lu\n"\
		   "%s: %lu\n"\
		   "%s: %lu\n", TESTS_RUN_MSG, i,
			            TESTS_FAILED_MSG, test_failed,
					    TESTS_SUCCEDED_MSG, test_succeded);
}