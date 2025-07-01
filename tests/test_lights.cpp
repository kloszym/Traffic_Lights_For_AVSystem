// Plik: tests/test_lights.cpp

#include <gtest/gtest.h>

extern "C" {
#include "lights.h"
	// Dołączamy też definicje zmiennych globalnych, aby móc je sprawdzić
	// To jest wyjątek, normalnie byśmy tego nie robili, ale dla testów jest to potrzebne.
#include "simulation_logic.c"
}

// Test dla funkcji sterujących światłami w stanie 0
TEST(LightsTest, State0Functions) {
	// Ustaw stan początkowy
	turnAllRedLamps();

	// Sprawdź włączenie zielonego
	turnGreenState0Lamps();
	ASSERT_EQ(westStraightLight, GREEN_LIGHT);
	ASSERT_EQ(eastStraightLight, GREEN_LIGHT);
	ASSERT_EQ(northStraightLight, RED_LIGHT); // Sprawdź, czy inne są czerwone

	// Sprawdź włączenie pomarańczowego
	turnOrangeState0Lamps();
	ASSERT_EQ(westStraightLight, ORANGE_LIGHT);
	ASSERT_EQ(eastStraightLight, ORANGE_LIGHT);

	// Sprawdź włączenie czerwonego
	turnRedState0Lamps();
	ASSERT_EQ(westStraightLight, RED_LIGHT);
	ASSERT_EQ(eastStraightLight, RED_LIGHT);
}

// Test dla funkcji sterujących światłami w stanie 2
TEST(LightsTest, State2Functions) {
	turnAllRedLamps();

	turnGreenState2Lamps();
	ASSERT_EQ(northStraightLight, GREEN_LIGHT);
	ASSERT_EQ(southStraightLight, GREEN_LIGHT);
	ASSERT_EQ(westLeftLight, RED_LIGHT); // Sprawdź inne

	turnOrangeState2Lamps();
	ASSERT_EQ(northStraightLight, ORANGE_LIGHT);
	ASSERT_EQ(southStraightLight, ORANGE_LIGHT);

	turnRedState2Lamps();
	ASSERT_EQ(northStraightLight, RED_LIGHT);
	ASSERT_EQ(southStraightLight, RED_LIGHT);
}