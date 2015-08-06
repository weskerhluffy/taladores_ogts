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
const char EOT[] = { 4, '\0' };

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

#undef NUM_ELEMENTOS
#define NUM_ELEMENTOS  TALADORES_OGTS_MAX_ELEMS
#define TAM_ARRAY NUM_ELEMENTOS+1
START_TEST( test_caca_max)
	{
		tipo_dato saltito = 0;

		chostos_minimos = calloc(TAM_ARRAY, sizeof(tipo_dato));
		costos_corte = calloc(TAM_ARRAY, sizeof(tipo_dato));
		alturas_arboles = calloc(TAM_ARRAY, sizeof(tipo_dato));
		pseudopila = calloc(1, sizeof(taladores_ogts_pseudopila_lineas));

		*chostos_minimos = TALADORES_OGTS_VALOR_INVALIDO;
		*costos_corte = TALADORES_OGTS_VALOR_INVALIDO;
		*alturas_arboles = TALADORES_OGTS_VALOR_INVALIDO;

		num_arboles = NUM_ELEMENTOS;

		saltito = TALADORES_OGTS_MAX_VALOR / NUM_ELEMENTOS;

		for (int i = 1; i <= num_arboles; i++) {
			*(chostos_minimos + i) = TALADORES_OGTS_VALOR_INVALIDO;
			*(costos_corte + i) = TALADORES_OGTS_MAX_VALOR - i * saltito;
			*(alturas_arboles + i) = i * saltito;
		}
		*(costos_corte + num_arboles) = 0;
		*(alturas_arboles + 1) = 1;

		taladores_ogts_init_pseudo_pila(pseudopila);

		taladores_ogts_encuentra_chosto_minimo();

		ck_assert_msg(*(chostos_minimos+NUM_ELEMENTOS)==9510770918594932224ULL,
				"verga, el costo min final es %lu",
				*(chostos_minimos+NUM_ELEMENTOS));

		free(pseudopila);
		free(chostos_minimos);
		free(costos_corte);
		free(alturas_arboles);
	}END_TEST

#undef NUM_ELEMENTOS
#define NUM_ELEMENTOS 5
#define NUM_FILAS 3
START_TEST(test_basica)
	{
		const tipo_dato VALOR_ESPERADO = 25;
		const char VALORES_ENTRADA[NUM_FILAS][10] = {

		{ "5\n" },

		{ "1 2 3 4 5\n" },

		{ "5 4 3 2 0\n" },

		};

		int ptyfd = 0;
		int pid = 0;

		tipo_dato resultado_real = 0;

		printf("you were the last one \n");

		resultado_assestment = mmap(NULL, sizeof *resultado_assestment,
				PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

		printf("antes de forkear\n");
		pid = forkpty(&ptyfd, 0, 0, 0);
		if (pid < 0) {
			perror("forkpty no c pudo acer"), abort();
		}

		*resultado_assestment = -1;
		if (!pid) {

			taladores_ogts_main();
			resultado_real = *(chostos_minimos + NUM_ELEMENTOS);

			*resultado_assestment = (resultado_real == VALOR_ESPERADO);

		} else {
			for (int i = 0; i < NUM_FILAS - 1; i++) {
//				printf("escribiendo esta mierda %s I LIKE TO\n", *(VALORES_ENTRADA + i));
				write(ptyfd, *(VALORES_ENTRADA + i),
						strlen(*(VALORES_ENTRADA + i)));
			}
			write(ptyfd, EOT, 1);
		}

		if (pid) {
			while (*resultado_assestment < 0) {
				printf("puta madre, esperando...\n");
				sleep(5);
			}
			printf("cerrando todo\n");
			close(ptyfd);
			ck_assert_msg(*resultado_assestment > 0,
					"no corresponde ni madres");
		} else {

		}
	}END_TEST

#undef NUM_ELEMENTOS
//#define NUM_ELEMENTOS  TALADORES_OGTS_MAX_ELEMS
#define NUM_ELEMENTOS  50
START_TEST(test_stress)
	{
		const tipo_dato VALOR_ESPERADO = 9510770918594932224ULL;

		int ptyfd = 0;
		int pid = 0;
		char buf[1000] = { 0 };

		tipo_dato resultado_real = 0;
		tipo_dato saltito = 0;

		saltito = TALADORES_OGTS_MAX_VALOR / NUM_ELEMENTOS;

		printf("you were the last one \n");

		resultado_assestment = mmap(NULL, sizeof *resultado_assestment,
				PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

		printf("antes de forkear\n");
		pid = forkpty(&ptyfd, 0, 0, 0);
		if (pid < 0) {
			perror("forkpty no c pudo acer"), abort();
		}

		*resultado_assestment = -1;
		if (!pid) {

			taladores_ogts_main();
			resultado_real = *(chostos_minimos + NUM_ELEMENTOS );

			*resultado_assestment = (resultado_real == VALOR_ESPERADO);

		} else {
			sprintf(buf, "%lu\n", NUM_ELEMENTOS);
			write(ptyfd, buf, strlen(buf));

			sprintf(buf, "%lu", (long )1);
			write(ptyfd, buf, strlen(buf));
			for (int i = 1; i < NUM_ELEMENTOS - 1; i++) {
				sprintf(buf, " %lu", i * saltito);
				write(ptyfd, buf, strlen(buf));
			}
			sprintf(buf, " %lu\n", TALADORES_OGTS_MAX_VALOR);
			write(ptyfd, buf, strlen(buf));

			sprintf(buf, "%lu", TALADORES_OGTS_MAX_VALOR);
			write(ptyfd, buf, strlen(buf));
			for (int i = 1; i < NUM_ELEMENTOS - 1; i++) {
				sprintf(buf, " %lu", TALADORES_OGTS_MAX_VALOR - i * saltito);
				write(ptyfd, buf, strlen(buf));
			}
			sprintf(buf, " %lu\n", (long )0);
			write(ptyfd, buf, strlen(buf));

			write(ptyfd, EOT, 1);
		}

		if (pid) {
			while (*resultado_assestment < 0) {
				printf("puta madre, esperando...\n");
				sleep(5);
			}
			printf("cerrando todo\n");
			close(ptyfd);
			ck_assert_msg(*resultado_assestment > 0,
					"no corresponde ni madres");
		} else {

		}
	}END_TEST

Suite *
taladoresogts_suite(void) {
	Suite *s = suite_create("taladoras de troncos");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 5);
#ifdef DEBUG
	sleep(5);
#endif
//	tcase_add_test(tc_core, test_caca);
//	tcase_add_test(tc_core, test_caca_max);
//	tcase_add_test(tc_core, test_basica);
	tcase_add_test(tc_core, test_stress);

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
