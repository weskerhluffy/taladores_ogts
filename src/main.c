#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <assert.h>

int i = 0;

#define tipo_dato unsigned long long

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

#define TALADORES_OGTS_MAX_FILAS_INPUT 3
#define TALADORES_OGTS_MAX_COLS_INPUT (long)1E5

#define TALADORES_OGTS_VALOR_INVALIDO (long)1E21
#define TALADORES_OGTS_MAX_ELEMS TALADORES_OGTS_MAX_COLS_INPUT

#define TALADORES_OGTS_MAX_VALOR (long)1E9

typedef struct taladores_ogts_pseudopila_lineas {
	tipo_dato indice_caminado_linea;
	tipo_dato total_intersexiones;
	tipo_dato total_intersexiones_abcisas;
	tipo_dato total_lineas;
	double ordenadas_de_intersexiones[TALADORES_OGTS_MAX_ELEMS ];
	double abcisas_de_intersexiones[TALADORES_OGTS_MAX_ELEMS ];
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
/*
 #define caca_log_debug1 printf
 */
#define caca_log_debug1(formato, args...) 0

int lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas, int *num_columnas,
		int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	long numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas && fgets(linea, TAM_MAX_LINEA, stdin)) {
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
			caca_log_debug1("en col %d, fil %d, el valor %lu\n",
					indice_columnas, indice_filas, numero);
			indice_columnas++;
			caca_log_debug1("las columnas son %d\n", indice_columnas);
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug1("las filas son %d, con clos %d\n", indice_filas,
				indice_columnas);
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
	bool nuevo_mejor = falso;
	double intersexion_nueva = 0;
	double abcisa_nueva = 0;
	double intersexion_ordenada_actual = 0;
	double abcisa_actual = 0;

	tipo_dato chosto_minimo_ordenada_actual = 0;
	tipo_dato chosto_minimo_ordenada_nueva = 0;
	tipo_dato indice_linea_actual = 0;

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
		abcisa_nueva = taladores_ogts_determina_abcisa_interseccion(
				indice_linea,
				*(pseudopila->indices_lineas + pseudopila->total_lineas - 1));
		*(pseudopila->indices_lineas + pseudopila->total_lineas) = indice_linea;
		*(pseudopila->ordenadas_de_intersexiones
				+ pseudopila->total_intersexiones) = intersexion_nueva;
		*(pseudopila->abcisas_de_intersexiones
				+ pseudopila->total_intersexiones_abcisas) = abcisa_nueva;

		caca_log_debug("se añade 1era intersex %f", intersexion_nueva);

		pseudopila->total_lineas++;
		pseudopila->total_intersexiones++;
		pseudopila->total_intersexiones_abcisas++;
		break;
	default:
		caca_log_debug("inicialmente hay 2+ linea");

		while (pseudopila->total_lineas > 1) {
			intersexion_ordenada_actual =
					*(pseudopila->ordenadas_de_intersexiones
							+ pseudopila->total_intersexiones - 1);

			abcisa_actual = *(pseudopila->abcisas_de_intersexiones
					+ pseudopila->total_intersexiones_abcisas - 1);

			indice_linea_actual = *(pseudopila->indices_lineas
					+ pseudopila->total_lineas - 1);

			intersexion_nueva =
					taladores_ogts_determina_ordenada_interseccion(indice_linea,
							*(pseudopila->indices_lineas
									+ pseudopila->total_lineas - 2));
			abcisa_nueva =
					taladores_ogts_determina_abcisa_interseccion(indice_linea,
							*(pseudopila->indices_lineas
									+ pseudopila->total_lineas - 2));

			caca_log_debug("la nueva intersexion es %f, comparandola con %f",
					intersexion_nueva, intersexion_ordenada_actual);
			assert(
					(intersexion_nueva < intersexion_ordenada_actual)
							|| (abcisa_nueva >= abcisa_actual));
			if (intersexion_nueva < intersexion_ordenada_actual) {
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
				*(pseudopila->abcisas_de_intersexiones
						+ pseudopila->total_intersexiones_abcisas - 1) =
						TALADORES_OGTS_VALOR_INVALIDO;

				pseudopila->total_lineas--;
				pseudopila->total_intersexiones--;
				pseudopila->total_intersexiones_abcisas--;
			} else {
				intersexion_nueva =
						taladores_ogts_determina_ordenada_interseccion(
								indice_linea,
								*(pseudopila->indices_lineas
										+ pseudopila->total_lineas - 1));
				abcisa_nueva = taladores_ogts_determina_abcisa_interseccion(
						indice_linea,
						*(pseudopila->indices_lineas + pseudopila->total_lineas
								- 1));
				break;
			}

		}

		*(pseudopila->ordenadas_de_intersexiones
				+ pseudopila->total_intersexiones) = intersexion_nueva;
		*(pseudopila->abcisas_de_intersexiones
				+ pseudopila->total_intersexiones_abcisas) = abcisa_nueva;
		*(pseudopila->indices_lineas + pseudopila->total_lineas) = indice_linea;

		caca_log_debug("nueva linea en %d es %d", pseudopila->total_lineas,
				indice_linea);
		caca_log_debug("nueva intersex en %d es %f",
				pseudopila->total_intersexiones, intersexion_nueva);

		pseudopila->total_lineas++;
		pseudopila->total_intersexiones++;
		pseudopila->total_intersexiones_abcisas++;

		if (pseudopila->indice_caminado_linea >= pseudopila->total_lineas) {
			caca_log_debug("se corrige el indice caminado de %d a %d",
					pseudopila->indice_caminado_lineas,
					pseudopila->total_lineas);
			pseudopila->indice_caminado_linea = pseudopila->total_lineas - 1;
		}

		break;
	};
}

tipo_dato taladores_ogts_busqueda_minimo_en_lineas(tipo_dato ordenada) {
	bool ordenadas_iguales = falso;
	tipo_dato minimo = 0;
	tipo_dato chosto_min = 0;
	tipo_dato chosto_min_actual = 0;
	tipo_dato linea_actual = 0;
	tipo_dato ordenada_pf = 0;
	tipo_dato idx_linea = 0;
	tipo_dato idx_linea_min = 0;

	ordenada_pf = *(alturas_arboles + ordenada);

	chosto_min = TALADORES_OGTS_VALOR_INVALIDO;
	for (idx_linea = pseudopila->indice_caminado_linea;
			idx_linea < pseudopila->total_lineas; idx_linea++) {

		linea_actual = *(pseudopila->indices_lineas + idx_linea);

		chosto_min_actual = (tipo_dato) ordenada_pf
				* *(costos_corte + linea_actual)
				+ *(chostos_minimos + linea_actual);

		if (chosto_min_actual < chosto_min) {
			minimo = linea_actual;
			chosto_min = chosto_min_actual;
			idx_linea_min = idx_linea;
		}

	}

	caca_log_debug("el minimo calculado es %d en ilnea %d", minimo,
			idx_linea_min);

	if (idx_linea_min == pseudopila->total_lineas) {
		pseudopila->indice_caminado_linea = idx_linea_min - 1;
	} else {
		pseudopila->indice_caminado_linea = idx_linea_min;
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
	*(chostos_minimos + 2) = *(costos_corte + 1) * *(alturas_arboles + 2)
			+ *(chostos_minimos + 1);
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

char *caca_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;

	memset(buffer, 0, 1000);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%ld",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	return ap_buffer;
}

void taladores_ogts_main() {
	int i = 0;
	int total_casos = 0;
	tipo_dato *datos_arboles = NULL;
	char buffer[1000];

	caca_log_debug("me corto los tanates");

	datos_arboles = calloc(
			TALADORES_OGTS_MAX_COLS_INPUT * TALADORES_OGTS_MAX_FILAS_INPUT,
			sizeof(tipo_dato));
	assert(datos_arboles);

	chostos_minimos = calloc(TALADORES_OGTS_MAX_ELEMS + 1, sizeof(tipo_dato));
	costos_corte = calloc(TALADORES_OGTS_MAX_ELEMS + 1, sizeof(tipo_dato));
	alturas_arboles = calloc(TALADORES_OGTS_MAX_ELEMS + 1, sizeof(tipo_dato));
	assert(chostos_minimos && costos_corte && alturas_arboles);

	pseudopila = calloc(1, sizeof(taladores_ogts_pseudopila_lineas));
	assert(pseudopila);

	lee_matrix_long_stdin(datos_arboles, &i, NULL,
			TALADORES_OGTS_MAX_FILAS_INPUT, TALADORES_OGTS_MAX_COLS_INPUT);
	total_casos++;

	while (i) {

		num_arboles = *datos_arboles;
		caca_log_debug1("los datos %lu\n", num_arboles);

		assert(num_arboles <= TALADORES_OGTS_MAX_ELEMS);

		/*		if (num_arboles >= 7 && num_arboles <= 100000) {
		 */
		/*
		 if (num_arboles == TALADORES_OGTS_MAX_ELEMS) {
		 abort();
		 while (1) {
		 total_casos++;
		 }
		 }*/

		memcpy(alturas_arboles + 1, datos_arboles+TALADORES_OGTS_MAX_COLS_INPUT,
				(TALADORES_OGTS_MAX_COLS_INPUT)* sizeof(tipo_dato));
		memcpy(costos_corte + 1,
				datos_arboles + 2*(TALADORES_OGTS_MAX_COLS_INPUT),
				(TALADORES_OGTS_MAX_COLS_INPUT)* sizeof(tipo_dato));

		for (i = 0; i <= num_arboles; i++) {
			*(chostos_minimos + i) = TALADORES_OGTS_VALOR_INVALIDO;
		}
		*costos_corte = TALADORES_OGTS_VALOR_INVALIDO;
		*alturas_arboles = TALADORES_OGTS_VALOR_INVALIDO;

		caca_log_debug1("el num de arboles %lu\n", num_arboles);
		caca_log_debug1("las alturas %s\n",
				caca_arreglo_a_cadena(alturas_arboles, num_arboles + 1, buffer));
		caca_log_debug1("llos chostos %s\n",
				caca_arreglo_a_cadena(costos_corte, num_arboles + 1, buffer));

		if (num_arboles == 1) {
			*(chostos_minimos + num_arboles) = 0;
			goto caca;
		}

		if (num_arboles == 2) {
			*(chostos_minimos + num_arboles) = *(alturas_arboles + num_arboles)
					* *(costos_corte + num_arboles - 1);
			goto caca;
		}

		taladores_ogts_init_pseudo_pila(pseudopila);

		/*
		 taladores_ogts_encuentra_chosto_minimo_no_optimizado();
		 */
		taladores_ogts_encuentra_chosto_minimo();
		caca_log_debug("q mierda pasa");

		caca:

		printf("%lu\n", *(chostos_minimos + num_arboles));

		i = 0;
		memset(alturas_arboles, 0, TALADORES_OGTS_MAX_COLS_INPUT+1);
		memset(costos_corte, 0, TALADORES_OGTS_MAX_COLS_INPUT+1);
		memset(chostos_minimos, 0, TALADORES_OGTS_MAX_COLS_INPUT+1);
		memset(datos_arboles, 0,
				(TALADORES_OGTS_MAX_COLS_INPUT+1)*(TALADORES_OGTS_MAX_FILAS_INPUT));
		memset(pseudopila, 0, sizeof(taladores_ogts_pseudopila_lineas));

		lee_matrix_long_stdin(datos_arboles, &i, NULL,
				TALADORES_OGTS_MAX_FILAS_INPUT, TALADORES_OGTS_MAX_COLS_INPUT);
		total_casos++;

	}

	/*
	 if (total_casos >= 7) {
	 abort();
	 while (1) {
	 total_casos++;
	 }
	 }
	 */
	free(alturas_arboles);
	free(costos_corte);
	free(chostos_minimos);
	free(datos_arboles);

}

int main(void) {
	taladores_ogts_main();
	return EXIT_SUCCESS;
}
