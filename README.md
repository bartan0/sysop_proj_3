Magazyn danych dowolnej wielkości oparty na stronicowaniu

Celem projektu jest stworzenie biblioteki zapewniającej magazyn danych, w którym
program korzystający z biblioteki ma możliwość przechowywania (zapisywania i
odczytywania) dowolnej liczby danych (ciągów bajtów) o dowolnej długości.

Założenia projektowe:
* dane przechowywane są w magazynie danych w oparciu o model stronicowania
	* każda dana (ciąg bajtów) umieszczana w magazynie dzielona jest na strony
	* magazyn zapewnia pewną liczbę logicznych ramek, w których strony mogą być
	przechowywane
	* nie wszystkie strony muszą być przechowywane w pamięci głównej - pewna ich
	liczba może znajdować się w pamięci drugorzędnej
* biblioteka jest bezpieczna w sensie używania przez wiele wątków danego procesu
* pamięć drugorzędna realizowana jest przy użyciu pliku
* dane mogą być przechowywane trwale (zakończenie programu używającego biblioteki
nie musi oznaczać utraty danych zgromadzonych w magazynie)
