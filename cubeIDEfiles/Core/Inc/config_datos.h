/*
 * config_datos.h
 *
 *  Created on: Dec 30, 2024
 *      Author: Usuario
 */

#ifndef INC_CONFIG_DATOS_H_
#define INC_CONFIG_DATOS_H_

// CONSTANTES MATEMATICAS
#define E 2.72 // Constante de Euler

// CARACTERISTICAS DEL MOTOR
#define PARES_POLOS 4  // PARES DE POLOS DEL MOTOR



//CONSTANTES DE CONVERSION
#define FACT_TIEMPO_CONMUTACION  5/PARES_POLOS // t_30° =  20/(RPS*PARES_POLOS)

// ARRANQUE

#define VEL_INICIAL 0 // RPM
#define VEL_FINAL 30 //RPM
#define TIEMPO_TOT 1000 // ms
#define RESOLUCION 5 // ms
#define K_SLOPE 0.1 // Pendiente de la rampa
#define DELTA_N  VEL_FINAL - VEL_INICIAL


#endif /* INC_CONFIG_DATOS_H_ */
