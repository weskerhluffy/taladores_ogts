#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <assert.h>

int i = 0;

#define tipo_dato unsigned long

#define MAX_NODOS 101
#define TAM_MAX_LINEA 1.5E6
#define TAM_MAX_NUMERO 128
#define MAX_FILAS_INPUT 5010
#define MAX_COLUMNAS_INPUT 256
#define MAX_FILAS_NODOS 1000
#define MAX_COLUMNAS_NODOS 1000
#define CACA_COMPARACION_IZQ_MENOR -1
#define CACA_COMPARACION_IZQ_IGUAL 0
#define CACA_COMPARACION_IZQ_MAYOR 1
#define MAX_TAM_CADENA 200
#define ARBOL_AVL_INDICE_INVALIDO -1
#define ARBOL_AVL_VALOR_INVALIDO -1
#define COLA_PRIORIDAD_VALOR_INVALIDO -1
#define DIJKSTRA_VALOR_INVALIDO -1
#define GRAFO_VALOR_INVALIDO -1

#define MAX_VALOR (tipo_dato) (2<<28)

#define GRAFO_NODO_ORIGEN 0
#define GRAFO_NODO_DESTINO 1
#define GRAFO_DISTANCIA_ENTRE_NODOS 2

#define GRAFO_LIMITE_ORDENACION_LINEAL 2

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

#define TALADORES_OGTS_MAX_FILAS_INPUT 2
#define TALADORES_OGTS_MAX_COLS_INPUT (long)1E5

#define TALADORES_OGTS_VALOR_INVALIDO (long)1E21
#define TALADORES_OGTS_MAX_ELEMS TALADORES_OGTS_MAX_COLS_INPUT

#define TALADORES_OGTS_MAX_VALOR (long)1E9

typedef struct taladores_ogts_pseudopila_lineas {
	tipo_dato indice_caminado_intersexion;
	tipo_dato total_intersexiones;
	tipo_dato total_lineas;
	double ordenadas_de_intersexiones[TALADORES_OGTS_MAX_ELEMS ];
	tipo_dato indices_lineas[TALADORES_OGTS_MAX_ELEMS ];
} taladores_ogts_pseudopila_lineas;

tipo_dato num_arboles;
tipo_dato *chostos_minimos;
tipo_dato *costos_corte;
tipo_dato *alturas_arboles;

taladores_ogts_pseudopila_lineas *pseudopila;

double taladores_ogts_determina_ordenada_interseccion(
		tipo_dato indice_valor_dinamico_1, tipo_dato indice_valor_dinamico_2);

void taladores_ogts_anade_linea(tipo_dato indice_linea);

tipo_dato taladores_ogts_busqueda_minimo_en_lineas(tipo_dato ordenada);

tipo_dato taladores_ogts_busca_minimo(tipo_dato indice_linea,
		tipo_dato ordenada);

void taladores_ogts_encuentra_chosto_minimo();

void taladores_ogts_main();

void taladores_ogts_init_pseudo_pila(
		taladores_ogts_pseudopila_lineas *pseudopila);

static void caca_lee_matrix_dimensiones_conocidas(tipo_dato *matrix,
		int num_filas, int num_columnas);

#define caca_log_debug(formato, args...) 0
#define caca_imprime_matrix(matrix, num_filas, num_columnas, num_columnas_fijo) 0
#define caca_log_debug1 printf

int lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas, int *num_columnas,
		int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	long numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(TAM_MAX_LINEA, sizeof(char));

	while (fgets(linea, TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			caca_log_debug("en col %d, fil %d, el valor %lu", indice_columnas,
					indice_filas, numero);
			indice_columnas++;
			caca_log_debug("las columnas son %d", indice_columnas);
			if (indice_columnas >= num_max_columnas) {
				perror("se leyeron demasiadas columnas, a la verga");
				abort();
			}
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d\n", indice_filas,
				indice_columnas);
		if (indice_filas >= num_max_filas) {
			perror("se leyeron demasiadas filas, a la verga");
			abort();
		}
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

/* Formula x = (b2 - b1)/(x1 - x2) */
double taladores_ogts_determina_ordenada_interseccion(
		tipo_dato indice_valor_dinamico_1, tipo_dato indice_valor_dinamico_2) {
	double ordenada = 0;
	double b2 = 0;
	double b1 = 0;
	double x2 = 0;
	double x1 = 0;

	b2 = *(chostos_minimos + indice_valor_dinamico_2);
	b1 = *(chostos_minimos + indice_valor_dinamico_1);

	x2 = *(costos_corte + indice_valor_dinamico_2);
	x1 = *(costos_corte + indice_valor_dinamico_1);

	ordenada = (b2 - b1) / (x1 - x2);

	return ordenada;
}
/* Formula y = (b1*m2 - b2*m1)/(m2 -m1) */
double taladores_ogts_determina_abcisa_interseccion(
		tipo_dato indice_valor_dinamico_1, tipo_dato indice_valor_dinamico_2) {
	double abcisa = 0;
	double b2 = 0;
	double b1 = 0;
	double m2 = 0;
	double m1 = 0;

	b2 = *(chostos_minimos + indice_valor_dinamico_2);
	b1 = *(chostos_minimos + indice_valor_dinamico_1);

	m2 = *(costos_corte + indice_valor_dinamico_2);
	m1 = *(costos_corte + indice_valor_dinamico_1);

	abcisa = (b1 * m2 - b2 * m1) / (m2 - m1);

	return abcisa;
}

void taladores_ogts_anade_linea(tipo_dato indice_linea) {
	double intersexion_nueva = 0;
	double abcisa = 0;

	switch (pseudopila->total_lineas) {
	case 0:
		caca_log_debug("inicialmente hay 0 linea, se añadira %d", indice_linea);
		*(pseudopila->indices_lineas + pseudopila->total_lineas) = indice_linea;
		pseudopila->total_lineas++;
		break;
	case 1:
		caca_log_debug("inicialmente hay 1 linea, se añadira %d", indice_linea);
		intersexion_nueva = taladores_ogts_determina_ordenada_interseccion(
				indice_linea,
				*(pseudopila->indices_lineas + pseudopila->total_lineas - 1));
		*(pseudopila->indices_lineas + pseudopila->total_lineas) = indice_linea;
		*(pseudopila->ordenadas_de_intersexiones
				+ pseudopila->total_intersexiones) = intersexion_nueva;

		caca_log_debug("se añade 1era intersex %f", intersexion_nueva);

		pseudopila->total_lineas++;
		pseudopila->total_intersexiones++;
		break;
	default:
		caca_log_debug("inicialmente hay 2+ linea");
		while (pseudopila->total_lineas > 1) {
			intersexion_nueva =
					taladores_ogts_determina_ordenada_interseccion(indice_linea,
							*(pseudopila->indices_lineas
									+ pseudopila->total_lineas - 2));
			abcisa =
					taladores_ogts_determina_abcisa_interseccion(indice_linea,
							*(pseudopila->indices_lineas
									+ pseudopila->total_lineas - 2));
			caca_log_debug("la nueva intersexion es %f, comparandola con %f",
					intersexion_nueva,
					*(pseudopila->ordenadas_de_intersexiones + pseudopila->total_intersexiones - 1));
			if (intersexion_nueva
					<= *(pseudopila->ordenadas_de_intersexiones
							+ pseudopila->total_intersexiones - 1)) {

				caca_log_debug("se va a kitar linea %d en pos %d",
						*(pseudopila->indices_lineas + pseudopila->total_lineas - 1),
						pseudopila->total_lineas - 1);
				caca_log_debug("se va a kitar inteseccion %f en pos %d",
						*(pseudopila->ordenadas_de_intersexiones + pseudopila->total_intersexiones - 1),
						pseudopila->total_intersexiones - 1);

				*(pseudopila->indices_lineas + pseudopila->total_lineas - 1) =
						TALADORES_OGTS_VALOR_INVALIDO;
				*(pseudopila->ordenadas_de_intersexiones
						+ pseudopila->total_intersexiones - 1) =
						TALADORES_OGTS_VALOR_INVALIDO;

				pseudopila->total_lineas--;
				pseudopila->total_intersexiones--;
			} else {
				caca_log_debug("a la mierda del ciclo con intersex %f",
						intersexion_nueva);
				break;
			}

		}

		*(pseudopila->ordenadas_de_intersexiones
				+ pseudopila->total_intersexiones) = intersexion_nueva;
		*(pseudopila->indices_lineas + pseudopila->total_lineas) = indice_linea;

		caca_log_debug("nueva linea en %d es %d", pseudopila->total_lineas,
				indice_linea);
		caca_log_debug("nueva intersex en %d es %f",
				pseudopila->total_intersexiones, intersexion_nueva);

		pseudopila->total_lineas++;
		pseudopila->total_intersexiones++;

		if (pseudopila->indice_caminado_intersexion
				> pseudopila->total_intersexiones) {
			caca_log_debug("se corrige el indice caminado de %d a %d",
					pseudopila->indice_caminado_intersexion,
					pseudopila->total_intersexiones);
			pseudopila->indice_caminado_intersexion =
					pseudopila->total_intersexiones;
		}

		break;
	};
}

tipo_dato taladores_ogts_busqueda_minimo_en_lineas(tipo_dato ordenada) {
	int idx_intersexion = 0;
	double ordenada_pf = 0;

	bool ordenadas_iguales = falso;
	tipo_dato minimo = 0;
	tipo_dato chosto_min_izq = 0;
	tipo_dato chosto_min_der = 0;
	tipo_dato indice_linea_izq = 0;
	tipo_dato indice_linea_der = 0;

	ordenada_pf = *(alturas_arboles + ordenada);

	for (idx_intersexion = pseudopila->indice_caminado_intersexion;
			idx_intersexion < pseudopila->total_intersexiones;
			idx_intersexion++) {

		caca_log_debug("comparando ordebada %f con intersex %f en pos %d",
				ordenada_pf,
				*(pseudopila->ordenadas_de_intersexiones + idx_intersexion),
				idx_intersexion);

		if (ordenada_pf
				< *(pseudopila->ordenadas_de_intersexiones + idx_intersexion)) {
			break;
		}
		if (ordenada_pf
				== *(pseudopila->ordenadas_de_intersexiones + idx_intersexion)) {
			ordenadas_iguales = verdadero;
			break;
		}

	}

	if (ordenadas_iguales) {
		indice_linea_izq = *(pseudopila->indices_lineas + idx_intersexion);
		indice_linea_der = *(pseudopila->indices_lineas + idx_intersexion + 1);
		chosto_min_izq = (tipo_dato) ordenada_pf
				* *(costos_corte + indice_linea_izq)
				+ *(chostos_minimos + indice_linea_izq);
		chosto_min_der = (tipo_dato) ordenada_pf
				* *(costos_corte + indice_linea_der)
				+ *(chostos_minimos + indice_linea_der);
		if (chosto_min_der < chosto_min_izq) {
			minimo = indice_linea_der;
		} else {
			minimo = indice_linea_izq;
		}
	} else {
		minimo = *(pseudopila->indices_lineas + idx_intersexion);
	}

	caca_log_debug("el minimo calculado es %d en pos intersex %d", minimo,
			idx_intersexion);

	if (idx_intersexion == pseudopila->total_intersexiones) {
		pseudopila->indice_caminado_intersexion = idx_intersexion - 1;
	} else {
		pseudopila->indice_caminado_intersexion = idx_intersexion;
	}

	return minimo;
}

tipo_dato taladores_ogts_busca_minimo(tipo_dato indice_linea,
		tipo_dato ordenada) {
	tipo_dato minimo = 0;

	taladores_ogts_anade_linea(indice_linea);

	minimo = taladores_ogts_busqueda_minimo_en_lineas(ordenada);

	return minimo;
}

void taladores_ogts_encuentra_chosto_minimo() {
	int i = 0;
	tipo_dato dependiente_minima = 0;

	caca_log_debug("mierda");
	*(chostos_minimos + 1) = 0;
	*(chostos_minimos + 2) = *(costos_corte + 1) * *(alturas_arboles + 2);
	caca_log_debug("añadiendo linea 1");
	taladores_ogts_anade_linea(1);

	for (i = 3; i <= num_arboles; i++) {
		caca_log_debug("añadiendo linea %d", i - 1);
		dependiente_minima = taladores_ogts_busca_minimo(i - 1, i);

		caca_log_debug1("el dep minimo %d\n", dependiente_minima);
		*(chostos_minimos + i) = *(chostos_minimos + dependiente_minima)
				+ *(alturas_arboles + i) * *(costos_corte + dependiente_minima);
		caca_log_debug1("el minimo costo para cortar %d es %lu\n", i,
				*(chostos_minimos + i));
	}
}

void taladores_ogts_init_pseudo_pila(
		taladores_ogts_pseudopila_lineas *pseudopila) {
	int i = 0;

	for (i = 0; i < TALADORES_OGTS_MAX_ELEMS ; i++) {
		*(pseudopila->indices_lineas + i) = TALADORES_OGTS_VALOR_INVALIDO;
		*(pseudopila->ordenadas_de_intersexiones + i) =
				TALADORES_OGTS_VALOR_INVALIDO;
	}
}

void taladores_ogts_encuentra_chosto_minimo_no_optimizado() {
	int i = 0;
	int j = 0;
	tipo_dato chosto_actual = 0;
	tipo_dato chosto_minimo = 0;
	tipo_dato dependiente_minima = 0;

	caca_log_debug("mierda");
	*(chostos_minimos + 1) = 0;
	*(chostos_minimos + 2) = *(costos_corte + 1) * *(alturas_arboles + 2);
	caca_log_debug("añadiendo linea 1");

	for (i = 3; i <= num_arboles; i++) {
		caca_log_debug1("encontrando chosto minimo para %d\n", i);

		chosto_minimo = TALADORES_OGTS_VALOR_INVALIDO;
		for (j = 1; j < i; j++) {
			chosto_actual = *(chostos_minimos + j)
					+ *(alturas_arboles + i) * *(costos_corte + j);
			if (chosto_actual < chosto_minimo) {
				chosto_minimo = chosto_actual;
				dependiente_minima = j;
			}
		}

		caca_log_debug1("el dep minimo %d\n", dependiente_minima);

		*(chostos_minimos + i) = *(chostos_minimos + dependiente_minima)
				+ *(alturas_arboles + i) * *(costos_corte + dependiente_minima);
		caca_log_debug1("el minimo costo para cortar %d es %lu\n", i,
				*(chostos_minimos + i));
	}
}

void taladores_ogts_main() {
	int i = 0;
	char buffer[MAX_TAM_CADENA * 5] = { 0 };
	tipo_dato *datos_arboles = NULL;

	caca_log_debug("me corto los tanates");

	fgets(buffer, MAX_TAM_CADENA * 5, stdin);
	caca_log_debug("el buffer de mierda %s", buffer);
	num_arboles = atol(buffer);

	caca_log_debug("mother of fuck %lu", num_arboles);

	datos_arboles = calloc(num_arboles * TALADORES_OGTS_MAX_FILAS_INPUT,
			sizeof(tipo_dato));
	assert(datos_arboles);

	caca_lee_matrix_dimensiones_conocidas((tipo_dato *) datos_arboles,
			TALADORES_OGTS_MAX_FILAS_INPUT, num_arboles);
	caca_log_debug("la matrix leida");
	caca_imprime_matrix(datos_arboles, TALADORES_OGTS_MAX_FILAS_INPUT, NULL,
			num_arboles);

	assert(num_arboles <= TALADORES_OGTS_MAX_ELEMS);

	chostos_minimos = calloc(num_arboles + 1, sizeof(tipo_dato));
	costos_corte = calloc(num_arboles + 1, sizeof(tipo_dato));
	alturas_arboles = calloc(num_arboles + 1, sizeof(tipo_dato));
	assert(chostos_minimos && chostos_minimos && alturas_arboles);

	memcpy((alturas_arboles + 1), datos_arboles,
			num_arboles * sizeof(tipo_dato));
	memcpy(costos_corte + 1, datos_arboles + num_arboles,
			num_arboles * sizeof(tipo_dato));

	for (i = 0; i <= num_arboles; i++) {
		*(chostos_minimos + i) = TALADORES_OGTS_VALOR_INVALIDO;
	}
	*costos_corte = TALADORES_OGTS_VALOR_INVALIDO;
	*alturas_arboles = TALADORES_OGTS_VALOR_INVALIDO;

	caca_log_debug("el num de arboles %lu", num_arboles);
	caca_log_debug("las alturas %s",
			caca_arreglo_a_cadena(alturas_arboles, num_arboles + 1, buffer));
	caca_log_debug("llos chostos %s",
			caca_arreglo_a_cadena(costos_corte, num_arboles + 1, buffer));

	if (num_arboles == 1) {
		*(chostos_minimos + num_arboles) = 0;
		return;
	}

	if (num_arboles == 2) {
		*(chostos_minimos + num_arboles) = *(alturas_arboles + num_arboles)
				* *(costos_corte + num_arboles - 1);
		return;
	}

	if (num_arboles > 100) {
		while (1) {
			i++;
		};
	}

	pseudopila = calloc(1, sizeof(taladores_ogts_pseudopila_lineas));

	taladores_ogts_init_pseudo_pila(pseudopila);

	taladores_ogts_encuentra_chosto_minimo();
	/*
	 taladores_ogts_encuentra_chosto_minimo_no_optimizado();
	 */
	caca_log_debug("q mierda pasa");
}

/* TODO: bandera para arreglo de arreglos */
/* TODO: numero de columnas variable x fila */
static void caca_lee_matrix_dimensiones_conocidas(tipo_dato *matrix,
		int num_filas, int num_columnas) {
	int idx_cols = 0;
	int idx_fils = 0;
	tipo_dato numero = 0;

	while (scanf("%lu", &numero) == 1 && idx_fils < num_filas) {
		*(matrix + idx_fils * num_columnas + idx_cols) = numero;
		idx_cols++;
		if (idx_cols == num_columnas) {
			idx_cols = 0;
			idx_fils++;
		}
	}
}

int main(void) {
	taladores_ogts_main();
	printf("%lu\n", *(chostos_minimos + num_arboles));
	return EXIT_SUCCESS;
}
