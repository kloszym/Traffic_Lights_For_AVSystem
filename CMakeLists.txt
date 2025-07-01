# Wymagana minimalna wersja CMake
cmake_minimum_required(VERSION 3.20)

# Nazwa projektu. Włączamy obsługę C i C++ (dla testów).
project(Traffic_Lights C CXX)

# Ustawienie standardów języka
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# --- 1. Stworzenie biblioteki z rdzeniem logiki ---
# Kompilujemy całą logikę (BEZ main.c) do jednej, reużywalnej biblioteki statycznej.
# To jest serce naszego programu.
add_library(simulation_logic
        simulation_logic.c
        lane.c
        lights.c
        car.c
)

# Poinformuj CMake, że pliki nagłówkowe dla tej biblioteki znajdują się w głównym folderze.
# Użycie PUBLIC sprawia, że każdy cel, który linkuje tę bibliotekę, automatycznie
# uzyska dostęp do tych nagłówków.
target_include_directories(simulation_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# --- 2. Główny program wykonywalny ---
# Budujemy go z pliku main.c, który jest tylko "uruchamiaczem".
add_executable(Traffic_Lights main.c)

# Łączymy (linkujemy) nasz główny program z biblioteką logiki,
# aby miał on dostęp do funkcji takich jak step(), addVehicle() itd.
target_link_libraries(Traffic_Lights PRIVATE simulation_logic)


# ======================================================================
# --- 3. Konfiguracja testów z użyciem Google Test ---
# ======================================================================

# Włącz testowanie dla tego projektu
enable_testing()

# Pobierz Google Test z Gita
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG    v1.14.0
)
FetchContent_MakeAvailable(googletest)


# --- 4. Program testowy ---
# Budujemy oddzielny program wykonywalny dla naszych testów.
# Nie zawiera on main.c, więc nie ma konfliktu funkcji main().
add_executable(runTests
        tests/test_lane.cpp
        tests/test_simulation.cpp
        tests/test_lights.cpp
)

# Linkujemy testy z naszą biblioteką logiki (aby mogły ją testować)
# oraz z gtest_main (która dostarcza funkcję main() i framework testowy).
target_link_libraries(runTests PRIVATE simulation_logic gtest_main)


# --- 5. Rejestracja testów w CTest ---
# Informuje CMake i CLion, jak uruchomić testy.
include(GoogleTest)
gtest_add_tests(TARGET runTests)