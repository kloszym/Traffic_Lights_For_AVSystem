#include "simulation_logic.h"

/**
 * @file simulation_logic.c
 * @brief Implementacja głównego modułu logiki symulacji.
 *
 * Ten moduł zawiera "mózg" systemu: algorytm decyzyjny, zarządzanie
 * stanem i koordynację innych modułów.
 * Zmienne globalne są zdefiniowane tutaj jako "jedno źródło prawdy".
 */

// --- Definicje Zmiennych Globalnych ---
// Zadeklarowane jako 'extern' w pliku .h

laneQueue twoLanesCrossroad[8];

uint8_t northStraightLight, southStraightLight, eastStraightLight, westStraightLight, northLeftLight, southLeftLight, eastLeftLight, westLeftLight, northRightLight, southRightLight, eastRightLight, westRightLight;

uint8_t currentOccupationOfStates[4] = {0,0,0,0};

uint16_t proportionOfTwoMostOccupiedStates = UINT16_MAX;
uint8_t currentState = state4;
uint8_t previousState = state4;

void updateOccupation(const Car car, int modifier) {
	switch (car.startRoad) {
		case NORTH:
			switch (car.endRoad) {
				case EAST: currentOccupationOfStates[state1] += modifier; break;
				case WEST: currentOccupationOfStates[state2] += modifier; currentOccupationOfStates[state3] += modifier; break;
				case SOUTH: currentOccupationOfStates[state2] += modifier; break;
				default: break;
			}
			break;
		case SOUTH:
			switch (car.endRoad) {
				case EAST: currentOccupationOfStates[state2] += modifier; currentOccupationOfStates[state3] += modifier; break;
				case WEST: currentOccupationOfStates[state1] += modifier; break;
				case NORTH: currentOccupationOfStates[state2] += modifier; break;
				default: break;
				}
			break;
		case WEST:
			switch (car.endRoad) {
				case EAST: currentOccupationOfStates[state0] += modifier; break;
				case NORTH: currentOccupationOfStates[state3] += modifier; break;
				case SOUTH: currentOccupationOfStates[state0] += modifier; currentOccupationOfStates[state1] += modifier; break;
				default: break;
			}
			break;
		case EAST:
			switch (car.endRoad) {
				case NORTH: currentOccupationOfStates[state0] += modifier; currentOccupationOfStates[state1] += modifier; break;
				case WEST: currentOccupationOfStates[state0] += modifier; break;
				case SOUTH: currentOccupationOfStates[state3] += modifier; break;
				default: break;
			}
			break;
		default:
			break;
	}
}

void addToLane(const Car car) {
	updateOccupation(car, 1);
	switch (car.startRoad) {
		case NORTH:
			switch (car.endRoad) {
				case EAST:
					enqueue(&twoLanesCrossroad[NORTH_EAST_LANE], car);
					break;
				case WEST:
				case SOUTH:
					enqueue(&twoLanesCrossroad[NORTH_SOUTH_WEST_LANE], car);
					break;
				default:
					break;
			}
			break;
		case SOUTH:
			switch (car.endRoad) {
				case WEST:
					enqueue(&twoLanesCrossroad[SOUTH_WEST_LANE], car);
					break;
				case EAST:
				case NORTH:
					enqueue(&twoLanesCrossroad[SOUTH_NORTH_EAST_LANE], car);
					break;
				default:
					break;
			}
			break;
		case WEST:
			switch (car.endRoad) {
				case NORTH:
					enqueue(&twoLanesCrossroad[WEST_NORTH_LANE], car);
					break;
				case EAST:
				case SOUTH:
					enqueue(&twoLanesCrossroad[WEST_EAST_SOUTH_LANE], car);
					break;
				default:
					break;
			}
			break;
		case EAST:
			switch (car.endRoad) {
				case NORTH:
				case WEST:
					enqueue(&twoLanesCrossroad[EAST_WEST_NORTH_LANE], car);
					break;
				case SOUTH:
					enqueue(&twoLanesCrossroad[EAST_SOUTH_LANE], car);
					break;
				default:
					break;
			}
			break;
		default:
			// no lane found
			break;
	}
}

void addVehicle(char vehicleId[], direction startRoad, direction endRoad) {
	Car car;
	// Użycie strncpy jest kluczowe dla bezpieczeństwa, aby uniknąć przepełnienia bufora.
	strncpy(car.vehicleId, vehicleId, sizeof(car.vehicleId) - 1);
	car.vehicleId[sizeof(car.vehicleId) - 1] = '\0';
	car.startRoad = startRoad;
	car.endRoad = endRoad;

	addToLane(car);
}

void change_lights(const uint8_t state) {

	switch (currentState) {
		case state0:
			turnOrangeState0Lamps();
			turnRedState0Lamps();
			break;
		case state1:
			turnOrangeState1Lamps();
			turnRedState1Lamps();
			break;
		case state2:
			turnOrangeState2Lamps();
			turnRedState2Lamps();
			break;
		case state3:
			turnOrangeState3Lamps();
			turnRedState3Lamps();
			break;
		default:
			turnAllRedLamps();
			break;
	}

	switch (state) {
		case state0:
			turnGreenState0Lamps();
			break;
		case state1:
			turnGreenState1Lamps();
			break;
		case state2:
			turnGreenState2Lamps();
			break;
		case state3:
			turnGreenState3Lamps();
			break;
		default:
			turnAllRedLamps();
			break;
	}

	previousState = currentState;
	currentState = state;
}

void car_left_lane(laneQueue* lane, Car car, Car returnCars[], uint8_t* index) {
	dequeue(lane);
	updateOccupation(car, -1);
	returnCars[*index] = car;
	(*index)++;
}

void cars_moves(Car returnCars[]) {

	uint8_t index = 0;

	for (uint8_t i = 0; i < 8; i++) {
		if (size(&twoLanesCrossroad[i]) == 0) continue;
		Car frontCar = first(&twoLanesCrossroad[i]);
		switch (i) {
			case SOUTH_NORTH_EAST_LANE:
				if (frontCar.endRoad == NORTH && southStraightLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				else if (frontCar.endRoad == EAST && (southStraightLight == GREEN_LIGHT || southRightLight == GREEN_LIGHT)) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case SOUTH_WEST_LANE:
				if (southLeftLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case NORTH_SOUTH_WEST_LANE:
				if (frontCar.endRoad == SOUTH && northStraightLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				else if (frontCar.endRoad == WEST && (northStraightLight == GREEN_LIGHT || northRightLight == GREEN_LIGHT)) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case NORTH_EAST_LANE:
				if (northLeftLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case EAST_WEST_NORTH_LANE:
				if (frontCar.endRoad == WEST && eastStraightLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				else if (frontCar.endRoad == NORTH && (eastStraightLight == GREEN_LIGHT || eastRightLight == GREEN_LIGHT)) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case EAST_SOUTH_LANE:
				if (eastLeftLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case WEST_EAST_SOUTH_LANE:
				if (frontCar.endRoad == EAST && westStraightLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				} else if (frontCar.endRoad == SOUTH && (westStraightLight == GREEN_LIGHT || westRightLight == GREEN_LIGHT)) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			case WEST_NORTH_LANE:
				if (westLeftLight == GREEN_LIGHT) {
					car_left_lane(&twoLanesCrossroad[i], frontCar, returnCars, &index);
				}
				break;
			default:
				break;
		}
	}
}

void step(Car leftCars[]) {

	// Główna funkcja logiki, wywoływana cyklicznie

	// 1. Zawsze analizuj sytuację na skrzyżowaniu: znajdź dwa najbardziej obciążone stany.

	uint8_t mostOccupiedStateIndex, secondMostOccupiedStateIndex;
	uint8_t mostOccupiedVal, secondOccupiedVal;

	// Inicjalizacja na podstawie pierwszych dwóch stanów

	if (currentOccupationOfStates[state0] > currentOccupationOfStates[state1]) {
        mostOccupiedStateIndex = state0; mostOccupiedVal = currentOccupationOfStates[state0];
        secondMostOccupiedStateIndex = state1; secondOccupiedVal = currentOccupationOfStates[state1];
    } else {
        mostOccupiedStateIndex = state1; mostOccupiedVal = currentOccupationOfStates[state1];
        secondMostOccupiedStateIndex = state0; secondOccupiedVal = currentOccupationOfStates[state0];
    }
	// Inicjalizacja na podstawie pierwszych dwóch stanów
    for (uint8_t i = 2; i < 4; i++) {
        if (currentOccupationOfStates[i] > mostOccupiedVal) {
            secondMostOccupiedStateIndex = mostOccupiedStateIndex;
            secondOccupiedVal = mostOccupiedVal;
            mostOccupiedStateIndex = i;
            mostOccupiedVal = currentOccupationOfStates[i];
        } else if (currentOccupationOfStates[i] > secondOccupiedVal) {
            secondMostOccupiedStateIndex = i;
            secondOccupiedVal = currentOccupationOfStates[i];
        }
    }

	// 2. Oblicz nową proporcję na podstawie świeżych danych.
	// Proporcja jest metryką, która może być użyta do bardziej złożonych decyzji.
    if (mostOccupiedVal > 0) {
        proportionOfTwoMostOccupiedStates = (secondOccupiedVal * 100) / mostOccupiedVal;
    } else {
    	// Jeśli nie ma aut, ustaw proporcję na maksymalną wartość.
        proportionOfTwoMostOccupiedStates = UINT16_MAX;
    }

    // 3. Podejmij decyzję o zmianie świateł na podstawie ULEPSZONEJ logiki
    bool shouldChangeLights = false;
    if (mostOccupiedVal > 0) {
        if (currentState != mostOccupiedStateIndex || proportionOfTwoMostOccupiedStates > 120 || currentState == state4) {
             shouldChangeLights = true;
        }
    } else if (currentState != state4) {
        // Jeśli nie ma żadnych aut, włącz wszystkie czerwone
        shouldChangeLights = true;
        mostOccupiedStateIndex = state4;
    }

    if (shouldChangeLights) {
        change_lights(mostOccupiedStateIndex);
    }

    // 4. Przesuń samochody
    cars_moves(leftCars);

}
