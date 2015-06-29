#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <cacacomun.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <util.h>
#include <math.h>
#include "../src/taladoresogts.h"

static int *resultado_assestment = NULL;

START_TEST( test_caca) {
	ck_assert_msg(verdadero, "verga, la matrix e");
}
END_TEST

Suite *
escapecabron_suite(void) {
	Suite *s = suite_create("Escape de mierda");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 600);
#ifdef DEBUG
	sleep(5);
#endif
	tcase_add_test(tc_core, test_caca);

	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int number_failed;
	Suite *s = escapecabron_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
