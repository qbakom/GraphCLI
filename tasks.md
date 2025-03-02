# Podział zadań – Projekt generowania grafu w C

## Założenia projektu

- **Język:** C (wyłącznie C)
- **Typ programu:** Konsolowy
- **Funkcjonalność:** Generowanie grafu, gdzie graf może być dwukierunkowy, a z danego wierzchołka wychodzi dowolna liczba krawędzi.
- **Interakcja człowiek-komputer:**  
  1. **Tryb uporządkowany:** Użytkownik podaje parametry po uruchomieniu (np. liczba wierzchołków, typ generacji – losowy czy określony).  
  2. **Tryb czatowy:** Użytkownik komunikuje się przez styl rozmowy (np. "stwórz graf z 5 wierzchołkami, zrób mi graf losowy"), gdzie backend symuluje AI.

- **Podejścia do generowania grafu:**
  - Ekstrakcja parametrów z danych wejściowych i generowanie grafu przy pomocy algorytmu.
  - Generowanie grafu przy pomocy LLM i dostosowanie do określonej reprezentacji.
  - Wymagane: przeanalizowanie wad i zalet obu podejść oraz implementacja minimalnej wersji.

- **Dodatkowe wymagania:**
  - Walidacja danych wejściowych.
  - Wynik działania programu to określony graf zapisany w wybranej reprezentacji.
  - Możliwość dodania dodatkowych funkcji (zadanie generyczne).
  - Kod źródłowy umieszczony na GitHubie.

---

## Podział zadań na 2 osoby

### Kuba – Core Engine & Reprezentacja Grafu

1. **Reprezentacja grafu i struktury danych**
   - Opracowanie i implementacja struktur grafu (np. lista sąsiedztwa lub macierz sąsiedztwa).
   - Zapewnienie, że graf może być dwukierunkowy i z danego wierzchołka wychodzi dowolna liczba krawędzi.

2. **Generowanie grafu przy pomocy algorytmu**
   - Implementacja funkcji generujących graf losowy oraz określony przez użytkownika.
   - Implementacja walidacji danych wejściowych (np. poprawność liczby wierzchołków, spójność danych).

3. **Interfejs konsolowy – Tryb uporządkowany**
   - Zaprojektowanie menu i sposobu podawania parametrów przez użytkownika po uruchomieniu programu.
   - Prezentacja wyniku w formie tekstowej (np. wyświetlenie reprezentacji grafu na konsoli).

4. **Integracja i dokumentacja kodu**
   - Przygotowanie struktury repozytorium na GitHubie (komentarze, README, instrukcje kompilacji).
   - Testowanie i debugowanie głównego mechanizmu generowania grafu.

---

### Olaf – Interfejs Czatu & Integracja AI

1. **Interfejs komunikacji czatowej**
   - Implementacja trybu, w którym użytkownik podaje parametry na podstawie rozmowy (np. "stwórz graf z 5 wierzchołkami, zrób mi graf losowy").
   - Przygotowanie backendu symulującego interakcję czatową (może być uproszczona symulacja lub integracja z zewnętrznym API, jeśli to możliwe w C).

2. **Ekstrakcja parametrów i analiza podejść**
   - Stworzenie modułu ekstrakcji parametrów z danych wejściowych w trybie czatowym.
   - Porównanie dwóch podejść:
     - Ekstrakcja parametrów i generacja grafu przy pomocy algorytmu.
     - Generowanie grafu przez LLM.
   - Przygotowanie dokumentacji z analizą wad i zalet obu metod.
   - Implementacja minimalnej wersji podejścia opartego na LLM (np. symulacja „inteligentnej” rozmowy przekładającej się na parametry generowania grafu).

3. **Walidacja i dodatkowe funkcje**
   - Implementacja mechanizmów walidacji danych wejściowych specyficznych dla trybu czatowego.
   - Rozbudowa dodatkowych funkcji (np. generowanie statystyk, wyszukiwanie ścieżek w grafie) – zadanie generyczne, opcjonalne rozszerzenia.

4. **Integracja i dokumentacja**
   - Współpraca przy integracji obu trybów (uporządkowanego i czatowego) w jedną spójną aplikację.
   - Dokumentacja w repozytorium na GitHubie – opis poszczególnych modułów, instrukcje uruchomienia i przykłady użycia.

---

## Podsumowanie

- **Kuba** koncentruje się na solidnym fundamencie: reprezentacji grafu, logice generowania, klasycznym interfejsie konsolowym oraz testowaniu.
- **Olaf** buduje "inteligentną" warstwę komunikacji, implementuje tryb czatowy, analizuje oba podejścia do generowania grafu oraz zajmuje się dodatkowymi funkcjonalnościami.

Podział zadań zapewnia równoległą pracę nad kluczowymi komponentami projektu i pozwala na pełne pokrycie wymagań.

---

