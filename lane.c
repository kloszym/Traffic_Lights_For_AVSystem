#include "lane.h"
#include <stdbool.h>

/**
 * @file lane.c
 * @brief Implementacja kolejki cyklicznej.
 *
 * Wykorzystuje arytmetykę modulo do "zawijania" wskaźników head i tail,
 * co pozwala na efektywne ponowne wykorzystanie pamięci bufora.
 */

// Funkcje pomocnicze, 'static' ogranicza ich widoczność tylko do tego pliku.
static bool is_full(const laneQueue* q) {
    return q->size == q->capacity;
}
static bool is_empty(const laneQueue* q) {
    return q->size == 0;
}

void initializeLaneQueue(laneQueue* q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->capacity = MAX_TRAFFIC_SIZE;
}

void enqueue(laneQueue* q, Car value) {
    // Sprawdzenie, czy kolejka nie jest pełna, zapobiega nadpisaniu danych.
    if (is_full(q)) {
        return;
    }

    q->cars[q->tail] = value;

    q->tail = (q->tail + 1) % q->capacity;

    q->size++;
}

void dequeue(laneQueue* q) {
    // Sprawdzenie, czy kolejka nie jest pusta, zapobiega błędnemu stanowi.
    if (is_empty(q)) {
        return;
    }

    q->head = (q->head + 1) % q->capacity;

    q->size--;
}

Car first(laneQueue* q) {
    if (is_empty(q)) {
        return defaultCar();
    }

    return q->cars[q->head];
}

uint8_t size(laneQueue* q) {
    return q->size;
}