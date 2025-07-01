#ifndef LANE_H
#define LANE_H

#include "car.h"
#include <stdint.h>

/**
 * @file lane.h
 * @brief API dla generycznej, bezpiecznej kolejki cyklicznej.
 *
 * Ta implementacja używa statycznie alokowanej tablicy, co jest kluczowe dla
 * systemów embedded, aby zapewnić przewidywalne zużycie pamięci i uniknąć
 * dynamicznej alokacji (malloc/free).
 */

// Maksymalny rozmiar kolejki. Zdefiniowany jako stała, aby alokacja pamięciodbyła się w czasie kompilacji.
#define MAX_TRAFFIC_SIZE 127 // INT8_MAX to 127

/** @brief Struktura danych reprezentująca kolejkę cykliczną. */
typedef struct laneQueue {
	Car cars[MAX_TRAFFIC_SIZE]; /**< Statyczna tablica przechowująca elementy. */

	int8_t head; /**< Indeks pierwszego elementu do odczytu. */
	int8_t tail; /**< Indeks następnego wolnego miejsca do zapisu. */

	uint8_t size; /**< Aktualna liczba elementów w kolejce. */
	uint8_t capacity; /**< Maksymalna pojemność kolejki (równa MAX_TRAFFIC_SIZE). */

} laneQueue;

void initializeLaneQueue(laneQueue* q);
void enqueue(laneQueue* q, Car value);
void dequeue(laneQueue* q);
Car first(laneQueue* q);
uint8_t size(laneQueue* q);

#endif //LANE_H