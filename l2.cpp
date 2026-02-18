#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

#define ALGO_APPLIES 100000

// ~ ALOGRITHMS :
// Брут форс
int full_numerate_search (string source, string pattern) {
  for(int i = 0; i < source.length(); i++){
    for(int j = 0; j < pattern.length(); j++){
      if(i + j > source.length()) break;
      if (source[i + j] != pattern[j]) break;
      else if (j == pattern.length() - 1) return i;
    }
  }
  return -1;
}

// Алгоритм поиска при помощи z-функции O(source.length())
vector<int> z(1010, 0);
int z_function_find (string source, string pattern) {
  // левая и правая крайние посещенные границы
  int l = 0, r = 0;
  // Бежим по индексам где может найтись pattern
  for (int i = 0; i < source.length() - pattern.length() + 1; i++) {
    // Если уже были в этом индексе, попробуем восстановить значение
    //   но оно не может быть больше r-i+1 поскольку мы не были правее границы r
    if (i <= r && i > 0)
      z[i] = min(r - i + 1, z[i - l]);
    
    // Двигаем z[i] до тех пор пока подстрока совпадает с pattern
    while (
      i + z[i] < source.length() &&
      z[i] < pattern.length() &&
      pattern[z[i]] == source[i + z[i]]
    )
      z[i]++;
    
    // Если мы пересекли правую границу
    if (i + z[i] - 1 > r) {
      // Сдвинем правую границу
      r = i + z[i] - 1;
      // Левую границу поставим на текущий z-индекс
      l = i;
    }

    // Если нашли нужную подстроку, вернём индекс её начала
    if (z[i] == pattern.length())
      return i;
  }
  // Если так и не нашли подстроку, вернём -1
  return -1;
}

// ~ RANDOMIZERS :
// Функция возвращающая случайное целое число в диапазоне [min, max]
int getRandomNumber(int min = 0, int max = 10000) {
  return rand() % (max - min + 1) + min;
}
// Функция возвращающая случайный символ
int getRandomChar () {
  return char(getRandomNumber(int('a'), int('a')+6));
}

// ~ TESTERS :
// Функция запускающая тест при некотором n (количестве элементов) для обоих сортировок
void testAlgos (int n) {
  cout << "~~~~~~~~~ Test block for n = " << n << " ~~~~~~~~~~" << endl;
  double t1_sum = 0, t2_sum = 0;
  // Запускаем алгоритмы ALGO_APPLIES раз
  for(int start_time = 0; start_time < ALGO_APPLIES; start_time++){
    // Генерируем строку:
    // Левая граница pattern'a
    int len = getRandomNumber(0.3*n, 0.7*n);
    int l = getRandomNumber(0, n-len-1);
    int r = l + len;
    // Длина pattern'a
    string pattern = "", source = "";
    for(int j = 0; j < n; j++){
      char a = getRandomChar();
      source += a;
      if (l <= j && j <= r) pattern += a;
    }
    // запускаем алгоритмы поиска
    double t1 = clock();
    full_numerate_search(source, pattern);
    t1_sum += clock() - t1;
    //
    double t2 = clock();
    z_function_find(source, pattern);
    t2_sum += clock() - t2;
  }
  cout << "| Brute Force find took: " << t1_sum / CLOCKS_PER_SEC << " s." << endl;
  cout << "| Z-index find took: " << t2_sum / CLOCKS_PER_SEC << " s." << endl;
}

int main() {
  // Счетчик для генерации случайных чисел
  srand(time(0));
  cout << "~~~~~~~~~ TESTING ~~~~~~~~~" << endl;
  // Настраиваем вывод чисел с плавающей точкой
  cout << fixed; cout.precision(3);
  
  // Запускаем тесты для сортировок на n = 10, 100, 200, ..., 1000
  testAlgos(10);
  for(int i = 100; i <= 1000; i+=100) testAlgos(i);

  return 0;
}

