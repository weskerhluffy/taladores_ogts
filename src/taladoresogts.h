/*
 * escapecabron.h
 *
 *  Created on: 28/06/2015
 *      Author: ernesto
 */

#include<config.h>
#include<cacacomun.h>

#ifndef TALADORESOGTS_H_
#define TALADORESOGTS_H_

#define TALADORES_OGTS_MAX_FILAS_INPUT 3
#define TALADORES_OGTS_MAX_COLS_INPUT 100000

#define TALADORES_OGTS_VALOR_INVALIDO 10E12

typedef struct taladores_ogts_pseudopila_lineas {
	tipo_dato indice_caminado_intersexion;
	tipo_dato total_intersexiones;
	tipo_dato total_lineas;
	double *ordenadas_de_intersexiones;
	tipo_dato *indices_lineas;
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

double taladores_ogts_main();

#endif /* TALADORESOGTS_H_ */
