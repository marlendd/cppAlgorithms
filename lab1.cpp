#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <locale.h>
using namespace std;

// Функция пузырьковой сортировки
void bubbleSort(const char* inputFilename, const char* outputFilename) {
    FILE* file = fopen(inputFilename, "rb");
    if (file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }
    vector<int> nums;
    int num;
    while (fscanf(file, "%d", &num) != EOF) {
        nums.push_back(num);
    }
    fclose(file);

    for (int i = 0; i < nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            if (nums[i] > nums[j]) swap(nums[i], nums[j]);
        }
    }

    file = fopen(outputFilename, "wb");
    if (file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }
    for (int i = 0; i < nums.size(); i++) {
        fprintf(file, "%d ", nums[i]);
    }
    fclose(file);
}

// Функция для быстрой сортировки
int partition(vector<int>& nums, int low, int high) {
    int pivot = nums[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (nums[j] <= pivot) {
            i++;
            swap(nums[i], nums[j]);
        }
    }
    swap(nums[i + 1], nums[high]);
    return i + 1;
}

void quickSort(vector<int>& nums, int low, int high) {
    if (low < high) {
        int pi = partition(nums, low, high);
        quickSort(nums, low, pi - 1);
        quickSort(nums, pi + 1, high);
    }
}

// Функция для чтения из файла, сортировки быстрой сортировкой и записи в файл
void quickSortFromFile(const char* inputFilename, const char* outputFilename) {
    FILE* file = fopen(inputFilename, "rb");
    if (file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }
    vector<int> nums;
    int num;
    while (fscanf(file, "%d", &num) != EOF) {
        nums.push_back(num);
    }
    fclose(file);

    quickSort(nums, 0, nums.size() - 1);

    file = fopen(outputFilename, "wb");
    if (file == NULL) {
        printf("Error: Failed to open file\n");
        return;
    }
    for (int i = 0; i < nums.size(); i++) {
        fprintf(file, "%d ", nums[i]);
    }
    fclose(file);
}

int main() {
    setlocale(LC_ALL, "Russian");
    const char* inputFilename = "input.txt";
    const char* outputFilename = "output.txt";
    int choice;

    cout << "Выберите алгоритм сортировки:\n";
    cout << "1. Сортировка пузырьком\n";
    cout << "2. Быстрая сортировка\n";
    cout << "0. Выход\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    if (choice == 0) {
        cout << "Программа завершена.\n";
        return 0;
    }

    clock_t start = clock();

    switch (choice) {
        case 1:
            bubbleSort(inputFilename, outputFilename);
            break;
        case 2:
            quickSortFromFile(inputFilename, outputFilename);
            break;
        default:
            cout << "Неверный выбор. Программа завершена.\n";
            return 1;
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    cout << "Время выполнения сортировки: " << time_taken << " секунд\n";
    cout << "Отсортированный массив записан в файл " << outputFilename << "\n";

    return 0;
}
