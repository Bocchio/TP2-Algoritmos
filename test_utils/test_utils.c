#include <stdio.h>
#include <stdlib.h>
#include "../types.h"
#include "../utils.h"
#include "minitest.h"

MINI_TEST(split_test_1)
{
	assert_str_eq("hola", "holaa");
}

MINI_TEST(split_test_2)
{
	assert_int_eq(5, 5);
}

int main(int argc, char **argv)
{
	test_t tests[] = {
		split_test_1,
		split_test_2
	};

	run_tests(tests);

	return EXIT_SUCCESS;
}
