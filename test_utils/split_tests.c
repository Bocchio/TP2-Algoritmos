#include <stdio.h>
#include <stdlib.h>

#include "../types.h"
#include "../utils.h"

#include "minitest.h"
#include "split_tests.h"

test_status_t split_test_1(void)
{
	char *cadena = "holaccmundo";
	char *delim = "cc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "mundo", NULL};

	size_t i;
	test_status_t st;

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		if((st = assert_str_eq(sep[i], expected[i])) != TEST_SUCCESS)
			return st;
	}
	return assert_int_eq_msg(number_of_substrings, 2, "Number of substrings\n");
}

test_status_t split_test_2(void)
{
	char *cadena = "holaccccmundo";
	char *delim = "cc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "", "mundo", NULL};

	size_t i;
	test_status_t st;

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		if((st = assert_str_eq(sep[i], expected[i])) != TEST_SUCCESS)
			return st;
	}
	return assert_int_eq_msg(number_of_substrings, 3, "Number of substrings\n");
}

test_status_t split_test_3(void)
{
	char *cadena = "holaccccmundo";
	char *delim = "ccc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "cmundo", NULL};

	size_t i;
	test_status_t st;

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		if((st = assert_str_eq(sep[i], expected[i])) != TEST_SUCCESS)
			return st;
	}
	return assert_int_eq_msg(number_of_substrings, 2, "Number of substrings\n");
}

test_status_t split_test_4(void)
{
	char *cadena = "holacccccmundo";
	char *delim = "ccc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "ccmundo", NULL};

	size_t i;
	test_status_t st;

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		if((st = assert_str_eq(sep[i], expected[i])) != TEST_SUCCESS)
			return st;
	}
	return assert_int_eq_msg(number_of_substrings, 2, "Number of substrings\n");
}

test_status_t split_test_5(void)
{
	char *cadena = "holaccccccmundo";
	char *delim = "ccc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "", "mundo", NULL};

	size_t i;
	test_status_t st;

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		if((st = assert_str_eq(sep[i], expected[i])) != TEST_SUCCESS)
			return st;
	}
	return assert_int_eq_msg(number_of_substrings, 3, "Number of substrings\n");
}

void split_tests_run(void)
{
	test_t split_tests[] = {
		MINI_TEST(split_test_1),
		MINI_TEST(split_test_2),
		MINI_TEST(split_test_3),
		MINI_TEST(split_test_4),
		MINI_TEST(split_test_5)
	};

	run_tests(split_tests);
}