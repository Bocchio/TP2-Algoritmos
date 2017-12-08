#include <stdio.h>
#include <stdlib.h>
#include "minitest.h"

#include "split_tests.h"
#include "starts_with_tests.h"

int main(int argc, char **argv)
{
	split_tests_run();
	starts_with_tests_run();

	return EXIT_SUCCESS;
}
