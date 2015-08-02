#include <stdlib.h>
#include <stdio.h>
#include <cacacomun.h>
#include "../src/taladoresogts.h"

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

void taladores_ogts_anade_linea(tipo_dato indice_linea) {
	double intersexion_nueva = 0;

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
			caca_log_debug("la nueva intersexion es %f, comparandola con %f",
					intersexion_nueva,
					*(pseudopila->ordenadas_de_intersexiones
							+ pseudopila->total_intersexiones - 1));
			if (intersexion_nueva
					<= *(pseudopila->ordenadas_de_intersexiones
							+ pseudopila->total_intersexiones - 1)) {

				caca_log_debug("se va a kitar linea %d en pos %d",
						*(pseudopila->indices_lineas + pseudopila->total_lineas
								- 1), pseudopila->total_lineas - 1);
				caca_log_debug("se va a kitar inteseccion %f en pos %d",
						*(pseudopila->ordenadas_de_intersexiones
								+ pseudopila->total_intersexiones - 1),
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
	tipo_dato minimo = 0;

	ordenada_pf = ordenada;

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

	}

	minimo = *(pseudopila->indices_lineas + idx_intersexion);

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

		caca_log_debug("el dep minimo %d", dependiente_minima);

		*(chostos_minimos + i) = *(chostos_minimos + dependiente_minima)
				+ *(alturas_arboles + i) * *(costos_corte + dependiente_minima);
		caca_log_debug("el minimo costo para cortar %d es %d", i,
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
