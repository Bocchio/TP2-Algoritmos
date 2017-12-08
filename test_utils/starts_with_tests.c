#include <stdio.h>
#include <stdlib.h>

#include "../types.h"
#include "../utils.h"

#include "minitest.h"
#include "starts_with_tests.h"

test_status_t starts_with_test_1(void)
{
	char *s1 = "abcdef";
	char *s2 = "abc";

	return assert_int_eq(starts_with(s1, s2), TRUE);
}

test_status_t starts_with_test_2(void)
{
	char *s1 = "abcdef";
	char *s2 = "abcdef";

	return assert_int_eq(starts_with(s1, s2), TRUE);
}

test_status_t starts_with_test_3(void)
{
	char *s1 = "abcdef";
	char *s2 = "abcd";

	return assert_int_eq(starts_with(s1, s2), TRUE);
}

test_status_t starts_with_test_4(void)
{
	char *s1 = "abcdef";
	char *s2 = "abcdeff";

	return assert_int_eq(starts_with(s1, s2), FALSE);
}

test_status_t starts_with_test_5(void)
{
	char *s1 = "abcdef";
	char *s2 = "abd";

	return assert_int_eq(starts_with(s1, s2), FALSE);
}

void starts_with_tests_run(void)
{
	test_t starts_with_tests[] = {
		MINI_TEST(starts_with_test_1),
		MINI_TEST(starts_with_test_2),
		MINI_TEST(starts_with_test_3),
		MINI_TEST(starts_with_test_4),
		MINI_TEST(starts_with_test_5)
	};

	run_tests(starts_with_tests);
}