#ifndef LIGHTS_H
#define LIGHTS_H

#include <stdint.h>

/**
 * @file lights.h
 * @brief API dla warstwy abstrakcji sprzętowej dla świateł.
 *
 * Ten moduł symuluje interfejs do sterowania fizycznymi diodami LED na skrzyżowaniu.
 */

// Definicje stanów świateł.
#define RED_LIGHT 0
#define ORANGE_RED_LIGHT 1
#define GREEN_LIGHT 2
#define ORANGE_LIGHT 3

// Zmienne globalne reprezentujące stan logiczny każdego światła.
extern uint8_t northStraightLight;
extern uint8_t southStraightLight;
extern uint8_t eastStraightLight;
extern uint8_t westStraightLight;
extern uint8_t northLeftLight;
extern uint8_t southLeftLight;
extern uint8_t eastLeftLight;
extern uint8_t westLeftLight;
extern uint8_t northRightLight;
extern uint8_t southRightLight;
extern uint8_t eastRightLight;
extern uint8_t westRightLight;

// Zmienne globalne reprezentujące stan logiczny każdego światła.
void turnGreenState0Lamps();
void turnRedState0Lamps();
void turnOrangeState0Lamps();

void turnGreenState1Lamps();
void turnRedState1Lamps();
void turnOrangeState1Lamps();

void turnGreenState2Lamps();
void turnRedState2Lamps();
void turnOrangeState2Lamps();

void turnGreenState3Lamps();
void turnRedState3Lamps();
void turnOrangeState3Lamps();

void turnAllRedLamps();

#endif //LIGHTS_H
