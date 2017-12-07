#include <stdio.h>
#include <stdlib.h>
#include "../types.h"
#include "../utils.h"
#include "minitest.h"

MINI_TEST(split_test_1)
{
	char *cadena = "holaccmundo";
	char *delim = "cc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "mundo", NULL};

	size_t i;
	char msg_buffer[100];

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		sprintf(msg_buffer, "%s != %s\n", sep[i], expected[i]);
		assert_str_eq_msg(sep[i], expected[i], msg_buffer);
	}
	assert_int_eq(number_of_substrings, 2);
}

MINI_TEST(split_test_2)
{
	char *cadena = "holaccccmundo";
	char *delim = "cc";
	char **sep;
	size_t number_of_substrings;

	char * expected[] = {"hola", "", "mundo", NULL};

	size_t i;
	char msg_buffer[100];

	split(cadena, &sep, delim, &number_of_substrings);

	for(i = 0; i < sizeof(expected)/sizeof(*expected); i++){
		sprintf(msg_buffer, "%s != %s\n", sep[i], expected[i]);
		assert_str_eq_msg(sep[i], expected[i], msg_buffer);
	}
	assert_int_eq(number_of_substrings, 3);
}

MINI_TEST(starts_with_test_1)
{
	char msg_buffer[100];
	char *s1 = "abcdef";
	char *s2 = "abc";

	sprintf(msg_buffer, "%s %s\n", s1, s2);
	assert_int_eq_msg(starts_with("abcdef", "abc"), TRUE, msg_buffer);
}

MINI_TEST(starts_with_test_2)
{
	char msg_buffer[100];
	char *s1 = "abcdef";
	char *s2 = "abcdef";

	sprintf(msg_buffer, "%s %s\n", s1, s2);
	assert_int_eq_msg(starts_with(s1, s2), TRUE, msg_buffer);
}

MINI_TEST(starts_with_test_3)
{
	char msg_buffer[100];
	char *s1 = "abcdef";
	char *s2 = "abcd";

	sprintf(msg_buffer, "%s %s\n", s1, s2);
	assert_int_eq_msg(starts_with(s1, s2), TRUE, msg_buffer);
}

MINI_TEST(starts_with_test_4)
{
	char msg_buffer[100];
	char *s1 = "abcdef";
	char *s2 = "abcdeff";

	sprintf(msg_buffer, "%s %s\n", s1, s2);
	assert_int_eq_msg(starts_with(s1, s2), FALSE, msg_buffer);
}

MINI_TEST(starts_with_test_5)
{
	char msg_buffer[100];
	char *s1 = "abcdef";
	char *s2 = "abd";

	sprintf(msg_buffer, "%s %s\n", s1, s2);
	assert_int_eq_msg(starts_with(s1, s2), FALSE, msg_buffer);
}

int main(int argc, char **argv)
{
	test_t split_tests[] = {
		split_test_1,
		split_test_2
	};

	test_t starts_with_tests[] = {
		starts_with_test_1,
		starts_with_test_2,
		starts_with_test_3,
		starts_with_test_4,
		starts_with_test_5
	};

	run_tests(starts_with_tests);
	run_tests(split_tests);

	return EXIT_SUCCESS;
}
