// Plik: tests/test_lane.cpp

#include <gtest/gtest.h>

extern "C" {
#include "lane.h"
}

// Test Fixture dla kolejki, aby unikać powtarzania inicjalizacji
class LaneQueueTest : public ::testing::Test {
protected:
	laneQueue q;

	void SetUp() override {
		initializeLaneQueue(&q);
	}
};

// Test, który już miałeś, teraz używa Fixture
TEST_F(LaneQueueTest, BasicOperations) {
	Car car1 = {"v1", SOUTH, NORTH};
	Car car2 = {"v2", NORTH, SOUTH};

	ASSERT_EQ(size(&q), 0);

	enqueue(&q, car1);
	ASSERT_EQ(size(&q), 1);
	ASSERT_STREQ(first(&q).vehicleId, "v1");

	enqueue(&q, car2);
	ASSERT_EQ(size(&q), 2);
	ASSERT_STREQ(first(&q).vehicleId, "v1");

	dequeue(&q);
	ASSERT_EQ(size(&q), 1);
	ASSERT_STREQ(first(&q).vehicleId, "v2");

	dequeue(&q);
	ASSERT_EQ(size(&q), 0);
}

// NOWY TEST: Sprawdzenie zachowania na krawędziach
TEST_F(LaneQueueTest, EdgeCases) {
	// 1. Działanie na pustej kolejce
	ASSERT_EQ(size(&q), 0);
	dequeue(&q); // Nie powinno nic zepsuć
	ASSERT_EQ(size(&q), 0);
	Car default_car = first(&q); // Powinno zwrócić domyślne auto
	ASSERT_EQ(default_car.startRoad, NOTHING);

	// 2. Działanie na pełnej kolejce
	for (int i = 0; i < MAX_TRAFFIC_SIZE; ++i) {
		Car temp_car = {"temp", NORTH, SOUTH};
		enqueue(&q, temp_car);
	}
	ASSERT_EQ(size(&q), MAX_TRAFFIC_SIZE);

	// Próba dodania do pełnej kolejki
	Car extra_car = {"extra", EAST, WEST};
	enqueue(&q, extra_car);
	ASSERT_EQ(size(&q), MAX_TRAFFIC_SIZE); // Rozmiar nie powinien się zmienić
}
