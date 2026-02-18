#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <string>
#include <locale.h>
#include <iomanip>
#include <random> // Для случайного порядка
#include <thread> // Для задержки

using namespace std;

// =============== НАСТРОЙКИ ПРОГРАММЫ ===============
const int NUM_ITERATIONS = 100;    
const int NUM_TESTS = 10000;       // Увеличено количество тестов
const double START_EPS = 1.0;      
const double MIN_EPS = 0.0001;     
const double BEG = -10.0;          
const double END = 10.0;           

// =============== ОБЪЯВЛЕНИЕ ФУНКЦИЙ ================
double f(double x);
double dichotomy(double eps, double a = BEG, double b = END);
double bruteForce(double eps, double a = BEG, double b = END);

// ============== ЗАПИСЬ В ФАЙЛ =====================
template<typename... T>
void write_data(ofstream& file, const string& filename, 
               const string& sep, T&&... args) {
    file.open(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Ошибка записи в файл!" << endl;
        return;
    }
    
    file << fixed << scientific << setprecision(10);
    
    int dummy[] = { (file << args << sep, 0)... };
    (void)dummy;
    
    file << "\n";
    file.close();
}

// ============== ЦЕЛЕВАЯ ФУНКЦИЯ ====================
double f(double x) {
    return 3 * x * x + 2 * x - 2;
}

// ============== АЛГОРИТМ ДИХОТОМИИ ================
double dichotomy(double eps, double a, double b) {
    while (b - a > eps) {
        double x1 = (a + b - eps/3) / 2;
        double x2 = (a + b + eps/3) / 2;
        f(x1) < f(x2) ? b = x2 : a = x1;
    }
    return (a + b)/2;
}

// ============== ЛИНЕЙНЫЙ ПОИСК ====================
double bruteForce(double eps, double a, double b) {
    double min_x = a;
    double min_val = f(a);
    const long steps = static_cast<long>((b - a)/eps) + 1;
    
    for (long i = 0; i <= steps; ++i) {
        double x = a + i*eps;
        double val = f(x);
        if (val < min_val) {
            min_val = val;
            min_x = x;
        }
    }
    return min_x;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ofstream file;
    volatile double result;

    cout << "Запуск программы...\n";
    cout << "Параметры:\n"
         << "Количество измерений: " << NUM_ITERATIONS << "\n"
         << "Тестов на измерение: " << NUM_TESTS << "\n"
         << "Диапазон поиска: [" << BEG << ", " << END << "]\n\n";

    // Проверка точности часов
    auto period = std::chrono::steady_clock::period::num * 1e9 / std::chrono::steady_clock::period::den;
    cout << "Точность часов: " << period << " нс\n";

    // Создаем файл с заголовками
    write_data(file, "results.csv", ";", "Точность", "Перебор (нс)", "Дихотомия (нс)");

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        const double current_eps = START_EPS * pow(0.95, i);
        if (current_eps < MIN_EPS) break;

        volatile long long time_brute = 0;
        volatile long long time_dichotomy = 0;

        cout << "Измерение " << i+1 << "/" << NUM_ITERATIONS 
             << " (?=" << current_eps << ")\n";
        
        for (int j = 0; j < NUM_TESTS; ++j) {
            bool first = rand() % 2;
            
            if (first) {
                // Перебор сначала
                auto start = std::chrono::steady_clock::now();
                result = bruteForce(current_eps);
                auto end = std::chrono::steady_clock::now();
                time_brute += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                // Дихотомия потом
                start = std::chrono::steady_clock::now();
                result = dichotomy(current_eps);
                end = std::chrono::steady_clock::now();
                time_dichotomy += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            } else {
                // Дихотомия сначала
                auto start = std::chrono::steady_clock::now();
                result = dichotomy(current_eps);
                auto end = std::chrono::steady_clock::now();
                time_dichotomy += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                // Перебор потом
                start = std::chrono::steady_clock::now();
                result = bruteForce(current_eps);
                end = std::chrono::steady_clock::now();
                time_brute += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }

            this_thread::sleep_for(std::chrono::microseconds(1));
        }

        const double avg_brute = static_cast<double>(time_brute) / NUM_TESTS;
        const double avg_dichotomy = static_cast<double>(time_dichotomy) / NUM_TESTS;
        
        write_data(file, "results.csv", ";", 
            current_eps, 
            avg_brute,
            avg_dichotomy
        );
    }

    cout << "\nПрограмма успешно завершена!\n";
    cout << "Результаты сохранены в файл: results.csv" << endl;
    return 0;
}

