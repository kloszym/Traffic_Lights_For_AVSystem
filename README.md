# Symulator Inteligentnych Świateł Drogowych

## Wprowadzenie

Projekt jest realizacją zadania rekrutacyjnego dla firmy AVSystem. Celem było stworzenie symulacji inteligentnych świateł drogowych, która adaptuje cykle świateł w zależności od natężenia ruchu.

Rozwiązanie zostało zaimplementowane w języku **C** z myślą o **łatwej przenośności na platformy embedded**, co było jednym z kluczowych wymagań. Jako interfejs do obsługi plików JSON został wykorzystany skrypt w języku **Scala**.

## Kluczowe cechy i funkcjonalności

*   **Adaptacyjny algorytm sterowania:** System dynamicznie reaguje na zmieniające się warunki na drodze, minimalizując czas oczekiwania pojazdów.
*   **Architektura Embedded-Ready:** Rdzeń logiki napisany w C jest w pełni niezależny od systemu operacyjnego. Unika dynamicznej alokacji pamięci i używa statycznie alokowanych buforów, co zapewnia przewidywalność i stabilność.
*   **Modularna budowa:** Kod jest podzielony na logiczne, niezależne moduły (logika symulacji, obsługa pasów, sterowanie światłami), co ułatwia jego rozwój, testowanie i utrzymanie.
*   **Obsługa wielu pasów:** Model skrzyżowania uwzględnia osiem oddzielnych pasów ruchu, w tym dedykowane pasy do skrętu w lewo oraz pasy współdzielone (jazda prosto i w prawo).
*   **Sterowanie przez JSON:** Symulacja jest w pełni kontrolowana przez zewnętrzny plik JSON, co umożliwia łatwe definiowanie i odtwarzanie złożonych scenariuszy drogowych.
*   **Pokrycie testami:** Projekt zawiera zestaw testów jednostkowych i integracyjnych napisanych przy użyciu frameworka **Google Test**, co gwarantuje poprawność działania kluczowych komponentów.

## Architektura rozwiązania

Projekt został zaprojektowany w oparciu o architekturę warstwową, aby zapewnić wysoką spójność i niskie powiązanie między modułami.

1.  **Rdzeń logiki (C):**
    *   **`simulation_logic.c/h`**: Mózg operacji. Zawiera główną pętlę symulacji (`step()`) oraz algorytm decyzyjny. Nie ma żadnych zależności od systemu operacyjnego (brak `stdio.h`, `stdlib.h` w logice).
    *   **`lane.c/h`**: Implementacja bezpiecznej, wydajnej kolejki cyklicznej do zarządzania pojazdami na pasach.
    *   **`lights.c/h`**: Symuluje interfejs do sterowania fizycznymi światłami.
    *   **`car.c/h`**: Prosta struktura danych reprezentująca pojazd.

2.  **Interfejs (Scala):**
    *   **`simulation.sc`**: Skrypt konsolowy, który parsuje wejściowy plik JSON, wywołuje skompilowany program w C z odpowiednimi argumentami i formatuje jego wyjście do wynikowego pliku JSON.

3.  **Testy (C++/gtest):**
    *   Zestaw testów weryfikujących poprawność działania poszczególnych modułów (kolejka, światła) oraz całej logiki symulacji w różnych scenariuszach.

Komunikacja między Scalą a C odbywa się poprzez wywoływanie skompilowanego programu C jako zewnętrznego procesu i przechwytywanie jego standardowego wyjścia. Stan symulacji jest utrwalany między wywołaniami w pliku binarnym (`simulation.state`).

## Działanie algorytmu

Algorytm sterowania światłami opiera się na koncepcji **ważonej adaptacji priorytetów**. W każdym kroku symulacji (`step`):

1.  **Analiza obciążenia:** System oblicza liczbę pojazdów oczekujących w ramach czterech bezkolizyjnych grup (stanów) ruchu.
2.  **Identyfikacja priorytetów:** Znajdowane są dwa stany o największym natężeniu ruchu (`mostOccupiedState` i `secondMostOccupiedState`).
3.  **Obliczenie proporcji:** Obliczana jest proporcja natężenia ruchu między drugim a pierwszym stanem. Daje to metrykę "pilności" dla drugiego kierunku.
4.  **Podjęcie decyzji:** Decyzja o zmianie świateł jest podejmowana, jeśli spełniony jest co najmniej jeden z poniższych warunków:
    *   Światła są w stanie początkowym (wszystkie czerwone) i pojawiły się jakiekolwiek pojazdy.
    *   Obecnie aktywny stan nie jest już najbardziej obciążonym kierunkiem (pojawił się nowy "lider korka").
    *   Proporcja obciążenia przekroczyła ustalony próg (domyślnie 120%), co oznacza, że drugi w kolejności kierunek staje się relatywnie bardzo obciążony.
5.  **Przejazd pojazdów:** Po ewentualnej zmianie świateł, pierwszy pojazd z każdego pasa, który ma zielone światło, opuszcza skrzyżowanie.

Ten algorytm zapewnia, że system nie tylko reaguje na najbardziej zatłoczony kierunek, ale także zapobiega "zagłodzeniu" mniej uczęszczanych pasów, jednocześnie unikając zbyt częstych, nieefektywnych zmian świateł.

## Jak zbudować i uruchomić projekt?

### Wymagania
*   Kompilator C (np. GCC, Clang)
*   CMake (wersja min. 3.20)
*   Scala CLI
*   (Opcjonalnie) CLion lub inne IDE wspierające CMake

### Krok 1: Budowa rdzenia w C

Projekt wykorzystuje CMake do zarządzania procesem budowy.

1.  **Konfiguracja CMake (jeśli robione ręcznie):**
    ```bash
    cmake -S . -B cmake-build-debug
    ```
2.  **Kompilacja głównego programu:**
    ```bash
    cmake --build cmake-build-debug --target Traffic_Lights
    ```
    Alternatywnie, można użyć bezpośrednio GCC:
    ```bash
    gcc main.c simulation_logic.c lane.c lights.c car.c -o simulation.exe -Wall
    ```
    Skompilowany plik `simulation.exe` powinien znajdować się w głównym folderze projektu.

### Krok 2: Uruchomienie symulacji (wersja konsolowa)

Aby uruchomić symulację na podstawie pliku `input.json` i wygenerować `output.json`, użyj następującej komendy:
```bash
scala-cli simulation.sc -- input.json output.json
```

### Krok 3: Uruchomienie testów

Aby zbudować i uruchomić zestaw testów, użyj CMake:
```bash
# Zbuduj cel testowy
cmake --build cmake-build-debug --target runTests

# Uruchom testy
cd cmake-build-debug
ctest
```
W środowisku CLion wystarczy wybrać cel `runTests` i kliknąć przycisk "Run".
