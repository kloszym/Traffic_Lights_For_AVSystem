// Plik: tests/test_simulation.cpp

#include <gtest/gtest.h>

extern "C" {
    #include "simulation_logic.h" // Dołączamy main.c, aby mieć dostęp do jego funkcji i zmiennych globalnych
}

// Test Fixture - klasa, która przygotowuje środowisko dla testów
class SimulationTest : public ::testing::Test {
protected:
    // Ta funkcja będzie wywoływana automatycznie PRZED każdym testem w tym zestawie
    void SetUp() override {
        // Resetuj wszystkie globalne stany do wartości początkowych
        for (int i = 0; i < 8; i++) {
            initializeLaneQueue(&twoLanesCrossroad[i]);
        }
        memset(currentOccupationOfStates, 0, sizeof(currentOccupationOfStates));
        turnAllRedLamps();
        currentState = state4;
        proportionOfTwoMostOccupiedStates = UINT16_MAX;
    }
};

// Test oparty na scenariuszu z pliku input.json
TEST_F(SimulationTest, FullScenarioFromTask) {
    Car leftCars[MAX_TRAFFIC_SIZE];

    // Funkcja pomocnicza do czyszczenia bufora leftCars
    auto clearLeftCars = [&]() {
        for(int i = 0; i < MAX_TRAFFIC_SIZE; ++i) {
            leftCars[i] = defaultCar();
        }
    };

    // --- Krok 1 & 2: Dodanie pojazdów ---
    addVehicle((char*)"vehicle1", SOUTH, NORTH);
    addVehicle((char*)"vehicle2", NORTH, SOUTH);

    // Sprawdzenie stanu przed krokiem
    ASSERT_EQ(currentOccupationOfStates[state2], 2);

    // --- Krok 3: Pierwszy step ---
    clearLeftCars();
    step(leftCars);
    // Sprawdzenie, czy v1 i v2 odjechały
    ASSERT_STREQ(leftCars[0].vehicleId, "vehicle1");
    ASSERT_STREQ(leftCars[1].vehicleId, "vehicle2");
    ASSERT_EQ(leftCars[2].startRoad, NOTHING); // Upewnij się, że nie ma więcej aut
    ASSERT_EQ(currentOccupationOfStates[state2], 0); // Stan powinien być pusty

    // --- Krok 4: Pusty step ---
    clearLeftCars();
    step(leftCars);
    ASSERT_EQ(leftCars[0].startRoad, NOTHING); // Nikt nie powinien odjechać

    // --- Krok 5 & 6: Dodanie kolejnych pojazdów ---
    addVehicle((char*)"vehicle3", WEST, SOUTH);
    addVehicle((char*)"vehicle4", WEST, SOUTH);
    ASSERT_EQ(currentOccupationOfStates[state0], 2);
    ASSERT_EQ(currentOccupationOfStates[state1], 2);

    // --- Krok 7: Trzeci step ---
    clearLeftCars();
    step(leftCars);
    ASSERT_STREQ(leftCars[0].vehicleId, "vehicle3");
    ASSERT_EQ(leftCars[1].startRoad, NOTHING);

    // --- Krok 8: Czwarty step ---
    clearLeftCars();
    step(leftCars);
    ASSERT_STREQ(leftCars[0].vehicleId, "vehicle4");
    ASSERT_EQ(leftCars[1].startRoad, NOTHING);
}

// Test sprawdzający, co się dzieje, gdy nie ma żadnych aut
TEST_F(SimulationTest, StepWithNoCars) {
    Car leftCars[MAX_TRAFFIC_SIZE];
    for(int i = 0; i < MAX_TRAFFIC_SIZE; ++i) leftCars[i] = defaultCar();

    // Krok na pustym skrzyżowaniu
    step(leftCars);

    // Sprawdzenie: nikt nie powinien odjechać, a światła powinny być czerwone
    ASSERT_EQ(leftCars[0].startRoad, NOTHING);
    ASSERT_EQ(currentState, state4); // Oczekujemy, że przełączy się na wszystkie czerwone
}

// Test sprawdzający, czy system poprawnie zmienia priorytet
TEST_F(SimulationTest, PriorityChangesCorrectly) {
    Car leftCars[MAX_TRAFFIC_SIZE];
    auto clearLeftCars = [&]() {
        for(int i = 0; i < MAX_TRAFFIC_SIZE; ++i) leftCars[i] = defaultCar();
    };

    // 1. Stwórz mały ruch na kierunku N-S (state2)
    addVehicle((char*)"v_north1", NORTH, SOUTH);

    clearLeftCars();
    step(leftCars); // System powinien włączyć state2

    ASSERT_EQ(currentState, state2);
    ASSERT_STREQ(leftCars[0].vehicleId, "v_north1");

    // 2. Teraz stwórz DUŻY ruch na kierunku E-W (state0)
    addVehicle((char*)"v_east1", EAST, WEST);
    addVehicle((char*)"v_east2", EAST, WEST);
    addVehicle((char*)"v_west1", WEST, EAST);

    // Obecnie state0 ma 3 auta, a inne stany 0.
    // System MUSI zmienić priorytet.
    clearLeftCars();
    step(leftCars);

    // Sprawdzenie:
    ASSERT_EQ(currentState, state0); // Światła powinny się zmienić na state0!
    ASSERT_STREQ(leftCars[0].vehicleId, "v_east1"); // Pierwsze auto z nowego kierunku powinno przejechać
}