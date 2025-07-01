#include "lights.h"

/**
 * @file lights.c
 * @brief Implementacja funkcji dla świateł.
 */

void turnGreenState0Lamps() {
	westStraightLight = ORANGE_RED_LIGHT;
	eastStraightLight = ORANGE_RED_LIGHT;

	westStraightLight = GREEN_LIGHT;
	eastStraightLight = GREEN_LIGHT;
}
void turnRedState0Lamps(){
	westStraightLight = RED_LIGHT;
	eastStraightLight = RED_LIGHT;
}
void turnOrangeState0Lamps(){
	westStraightLight = ORANGE_LIGHT;
	eastStraightLight = ORANGE_LIGHT;
}

void turnGreenState1Lamps(){
	northLeftLight = ORANGE_RED_LIGHT;
	southLeftLight = ORANGE_RED_LIGHT;

	northLeftLight = GREEN_LIGHT;
	southLeftLight = GREEN_LIGHT;
	eastRightLight = GREEN_LIGHT;
	westRightLight = GREEN_LIGHT;
}
void turnRedState1Lamps(){
	northLeftLight = RED_LIGHT;
	southLeftLight = RED_LIGHT;
	eastRightLight = RED_LIGHT;
	westRightLight = RED_LIGHT;
}
void turnOrangeState1Lamps(){
	northLeftLight = ORANGE_LIGHT;
	southLeftLight = ORANGE_LIGHT;
}

void turnGreenState2Lamps(){
	northStraightLight = ORANGE_RED_LIGHT;
	southStraightLight = ORANGE_RED_LIGHT;

	northStraightLight = GREEN_LIGHT;
	southStraightLight = GREEN_LIGHT;
}
void turnRedState2Lamps(){
	northStraightLight = RED_LIGHT;
	southStraightLight = RED_LIGHT;
}
void turnOrangeState2Lamps(){
	northStraightLight = ORANGE_LIGHT;
	southStraightLight = ORANGE_LIGHT;
}

void turnGreenState3Lamps(){
	westLeftLight = ORANGE_RED_LIGHT;
	eastLeftLight = ORANGE_RED_LIGHT;

	westLeftLight = GREEN_LIGHT;
	eastLeftLight = GREEN_LIGHT;
	northRightLight = GREEN_LIGHT;
	southRightLight = GREEN_LIGHT;
}
void turnRedState3Lamps(){
	westLeftLight = RED_LIGHT;
	eastLeftLight = RED_LIGHT;
	northRightLight = RED_LIGHT;
	southRightLight = RED_LIGHT;
}
void turnOrangeState3Lamps(){
	westLeftLight = ORANGE_LIGHT;
	eastLeftLight = ORANGE_LIGHT;
}

void turnAllRedLamps() {
	northStraightLight = RED_LIGHT;
	southStraightLight = RED_LIGHT;
	eastStraightLight = RED_LIGHT;
	westStraightLight = RED_LIGHT;
	northLeftLight = RED_LIGHT;
	southLeftLight = RED_LIGHT;
	eastLeftLight = RED_LIGHT;
	westLeftLight = RED_LIGHT;
	northRightLight = RED_LIGHT;
	southRightLight = RED_LIGHT;
	eastRightLight = RED_LIGHT;
	westRightLight = RED_LIGHT;
}