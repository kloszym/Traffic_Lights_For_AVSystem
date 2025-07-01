#ifndef SIMULATION_LOGIC_H
#define SIMULATION_LOGIC_H

#include "lane.h"
#include <stdint.h>
#include "lights.h"
#include <stdbool.h>
#include <string.h>

/**
 * @file simulation_logic.h
 * @brief Publiczny interfejs (API) dla głównego modułu logiki symulacji.
 *
 * Ten plik deklaruje funkcje i zmienne globalne, które składają się na rdzeń symulatora świateł drogowych.
 */

// --- Definicje Stanów ---
// Stany reprezentują bezkolizyjne grupy ruchów na skrzyżowaniu.
// 0. East-West, West-East, West-South, East-North
// 1. North-East, East-North, South-West, West-South
// 2. North-South, South-North, North-West, South-East
// 3. West-North, North-West, East-South, South-East
// 4. All red

#define state0 0
#define state1 1
#define state2 2
#define state3 3
#define state4 4 // Stan bezpieczeństwa: wszystkie światła czerwone

#define MAX_LEFT_CARS_IN_ONE_STEP 8

#define STATE_FILE "simulation.state"

#define SOUTH_NORTH_EAST_LANE 0
#define SOUTH_WEST_LANE 1
#define NORTH_SOUTH_WEST_LANE 2
#define NORTH_EAST_LANE 3
#define EAST_WEST_NORTH_LANE 4
#define EAST_SOUTH_LANE 5
#define WEST_EAST_SOUTH_LANE 6
#define WEST_NORTH_LANE 7

// --- Zmienne Globalne Stanu Symulacji ---

/** @brief Tablica śledząca liczbę pojazdów oczekujących w każdym ze stanów (0-3). */
extern uint8_t currentOccupationOfStates[4];

/** @brief Tablica 8 kolejek, każda reprezentująca osobny pas ruchu na skrzyżowaniu. */
extern laneQueue twoLanesCrossroad[8];

/** @brief Proporcja obciążenia między dwoma najbardziej zajętymi stanami. Używana w algorytmie decyzyjnym. */
extern uint16_t proportionOfTwoMostOccupiedStates;

/** @brief Aktualnie aktywny stan świateł (0-4). */
extern uint8_t currentState;

/** @brief Poprzedni stan świateł, przydatny do debugowania i logiki przejść. */
extern uint8_t previousState;

// --- Deklaracje Funkcji Publicznych ---

/**
 * @brief Aktualizuje liczniki obciążenia stanów na podstawie trasy pojazdu.
 * @param car Pojazd, którego trasa jest analizowana.
 * @param modifier Wartość do dodania do licznika (+1 dla dodania, -1 dla usunięcia).
 */
void updateOccupation(const Car car, int modifier);

/**
 * @brief Dodaje pojazd do odpowiedniej kolejki (pasa ruchu).
 * @param car Pojazd do dodania.
 */
void addToLane(const Car car);

/**
 * @brief Funkcja do dodawania nowego pojazdu do symulacji.
 * @param vehicleId Identyfikator pojazdu.
 * @param startRoad Droga, z której pojazd wjeżdża.
 * @param endRoad Droga, na którą pojazd zjeżdża.
 */
void addVehicle(char vehicleId[], direction startRoad, direction endRoad);

/**
 * @brief Zmienia stan świateł, zarządzając przejściami między stanami.
 * @param state Nowy stan świateł do aktywacji (0-4).
 */
void change_lights(const uint8_t state);

/**
 * @brief Dunkcja wykonujące usunięcie pojedynczego samochodu z pasa ruchu.
 * @param lane Dany pas ruchu
 * @param car Dany samochód
 * @param returnCars Tablica samochodów, które w danym kroku symulacji opuszczając skrzyżowanie
 * @param index Indeks na którym miejscu w tablicy returnCars powinien być dany samochó
 */
void car_left_lane(laneQueue* lane, Car car, Car returnCars[], uint8_t* index);

/**
 * @brief Funkcja sprawdzająca, który samochód może opuścić skrzyżowanie.
 * @param returnCars Tablica samochodów, które w danym kroku symulacji opuszczając skrzyżowanie
 */
void cars_moves(Car returnCars[]);

/**
 * @brief Zmienia stan świateł, zarządzając przejściami między stanami.
 * @param state Nowy stan świateł do aktywacji (0-4).
 */
void step(Car leftCars[]);

#endif //SIMULATION_LOGIC_H
