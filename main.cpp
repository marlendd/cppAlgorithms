#include <iostream>
#include <vector>
#include <ctime>
using namespace std;
#define N 10000


// ���������� ��������
vector<int> insertionSort(vector<int> nums) {
    for (int i = 1; i < nums.size(); i++) {
        int k = i;
        while (k > 0 && nums[k - 1] > nums[k]) {
            swap(nums[k - 1], nums[k]);
            k--;
        }
    }
    return nums;
}
// ������� ����������
vector<int> quickSort(vector<int> nums) {
    if (nums.size() < 2) return nums;
    int base = nums[0];
    vector<int> left, mid, right;
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] < base) left.push_back(nums[i]);
        else if (nums[i] > base) right.push_back(nums[i]);
        else mid.push_back(nums[i]);
    }
    left = quickSort(left);
    right = quickSort(right);

    left.insert(left.end(), mid.begin(), mid.end());
    left.insert(left.end(), right.begin(), right.end());

    return left;
}
// ������� ������������ ��������� ����� ����� � ��������� [min, max]
int getRandomNumber(int min = 0, int max = 10000) {
    return rand() % (max - min + 1) + min;
}

//��������� ����������
void ShakerSort(vector<int> arr)
{
    int size = arr.size();
    int leftMark = 1;
    int rightMark = size - 1;
    while (leftMark <= rightMark)
    {
        for (int i = rightMark; i >= leftMark; i--)
            if (arr[i - 1] > arr[i]) swap(arr[i], arr[i-1]);
        leftMark++;


        for (int i = leftMark; i <= rightMark; i++)
            if (arr[i - 1] > arr[i]) swap(arr[i], arr[i - 1]);
        rightMark--;
    }
}


// ������� ����������� ���� ��� ��������� n (���������� ���������) ��� ����������
void Sorts(int n) {
    cout << "Sort for n = " << n << "\n";
    // ��������� ������
    vector<int> ar;
    for (int i = 0; i < n; i++) ar.push_back(getRandomNumber());

    //���������� ��������� ����������
    cout << "Shaker sort\n";
    double t1 = clock(); // <- ������� clock() ���������� ������� ������������ ����� � ����� (���������� ������ ���������� � ������� ������� ���������)
    for (int i = 0; i < N; i++) ShakerSort(ar);
    double t2 = clock();
    cout << N << " sort for " << (t2 - t1) / CLOCKS_PER_SEC << endl;
    cout << "Mediana time: " << (t2 - t1) / CLOCKS_PER_SEC / N << endl; // <- ������� ����� ���������� ����� ���������

    //���������� ��� ������� ����������
    cout << "Quick sort\n";
    t1 = clock();
    for (int i = 0; i < N; i++) quickSort(ar);
    t2 = clock();
    cout << N << " sort for " << (t2 - t1) / CLOCKS_PER_SEC << endl;
    cout << "Mediana time: " << (t2 - t1) / CLOCKS_PER_SEC / N << endl;
    

}


int main() {
    // ������� ��� ��������� ��������� �����
    srand(time(0));
    // ����������� ����� ����� � ��������� ������
    cout << fixed; // ������������� ������������� ������
    cout.precision(3); // ������������� �������� �� 3 ������ ����� �����
    // ��������� ����� ��� ���������� �� n = 10, 100, 200, ..., 1000
    Sorts(10);
    cout << "\n" << endl;

    for (int n = 100; n <= 1000; n += 100) {
        Sorts(n);
        cout << "\n" << endl;
    }
    return 0;
}
