#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <util.h>
#include <math.h>
#include <check.h>
#include <cacacomun.h>
#include "../src/taladoresogts.h"

static int *resultado_assestment = NULL;

#define NUM_ELEMENTOS  6
START_TEST( test_caca)
	{
		chostos_minimos = (tipo_dato[NUM_ELEMENTOS] ) {
						TALADORES_OGTS_VALOR_INVALIDO };
		costos_corte = (tipo_dato[NUM_ELEMENTOS] )
				{ TALADORES_OGTS_VALOR_INVALIDO, 5, 4, 3, 2, 0 };
		alturas_arboles = (tipo_dato[NUM_ELEMENTOS] ) {
						TALADORES_OGTS_VALOR_INVALIDO, 1, 2, 3, 4, 5 };
		pseudopila = calloc(1, sizeof(taladores_ogts_pseudopila_lineas));

		num_arboles = 5;

		taladores_ogts_init_pseudo_pila(pseudopila);

		taladores_ogts_encuentra_chosto_minimo();

		free(pseudopila);

		ck_assert_msg(*(chostos_minimos+NUM_ELEMENTOS-1)==25,
				"verga, el costo min final es %d",
				*(chostos_minimos+NUM_ELEMENTOS));
	}END_TEST

Suite *
taladoresogts_suite(void) {
	Suite *s = suite_create("taladoras de troncos");

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
	Suite *s = taladoresogts_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
