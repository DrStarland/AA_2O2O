// lab3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//%5.1lf%%  %5.1lf%%  %5.1lf%% 

#include <iostream>
#include <random>
#include <ctime>
#include <intrin.h>
#include <vector>

typedef short int Word;

void fillRandom(std::vector<Word> &arr);
void time_experiment();

void selectionSort(std::vector<Word>& arr);
void bubbleSort(std::vector<Word>& arr);
void insertionSort(std::vector<Word>& arr);

using namespace::std;

template <typename T>
void print_array(std::vector<T>& arr, bool endflag = true) {
    for (auto i : arr)
        cout << i << " ";
    if (endflag)
        cout << endl;
}

int main() {
    size_t n = 10;
    {
        vector<Word> A(n);
        cout << "Original:       ";
        fillRandom(A), print_array(A);
        {
            vector<Word> B(A);
            cout << "Bubble sort:    ";
            bubbleSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Insertion sort: ";
            insertionSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Selection sort: ";
            selectionSort(B), print_array(B);
        }
    }
    {
        vector<Word> A = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        cout << "Original:       ";
        print_array(A);
        {
            vector<Word> B(A);
            cout << "Bubble sort:    ";
            bubbleSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Insertion sort: ";
            insertionSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Selection sort: ";
            selectionSort(B), print_array(B);
        }
    }
    {
        vector<Word> A = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        cout << "Original:       ";
        print_array(A);
        {
            vector<Word> B(A);
            cout << "Bubble sort:    ";
            bubbleSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Insertion sort: ";
            insertionSort(B), print_array(B);
        } {
            vector<Word> B(A);
            cout << "Selection sort: ";
            selectionSort(B), print_array(B);
        }
    }

    time_experiment();
    return 0;
}

void fillRandom(std::vector<Word>& arr) {
    std::random_device rd;
    std::mt19937 mersenne(rd());

    for (Word& i : arr) {
        i = mersenne() % 12000;
    }
}

void fillRight(std::vector<Word>& arr) {
    Word j = -10000;
    for (Word& i : arr) {
        i = ++j;
    }
}

void fillBack(std::vector<Word>& arr) {
    size_t j = 10000;
    for (Word& i : arr) {
        i = --j;
    }
}

void bubbleSort(std::vector<Word>& arr) {
    size_t N = arr.size();
    bool swappedElements = true;
    while (swappedElements) {
        --N;
        swappedElements = false;
        for (size_t i = 0; i < N; ++i)
            if (arr[i] > arr[i + 1])
                swap(arr[i], arr[i + 1]), swappedElements = true;
    }
}

void selectionSort(std::vector<Word>& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        size_t min = i;

        for (size_t j = i + 1; j < arr.size(); ++j)
            if (arr[j] < arr[min])
                min = j;

        if (min != i)
            swap(arr[i], arr[min]);
    }
}

void insertionSort(std::vector<Word>& arr) {
    for (size_t i = 1, j; i < arr.size(); ++i) {
        Word key = arr[i];
        for (j = i; j > 0 && arr[j - 1] > key; --j)
            arr[j] = arr[j - 1];
        arr[j] = key;
    }
}

void time_experiment() {

    vector<Word> array{ { 10, 50, 100, 200, 500, 1000, 2000, 5000, 10000 }};
    typedef unsigned long long proc_time;
    cout << "t1 - bubble, t2 - insertion, t3 - selection\n";
    cout << "    N        t1           t2           t3          t2/t1%  t3/t2%  t3/t1%     repeats\n";

    for (size_t i = 0; i < array.size(); ++i) {
        size_t n = array[i];

        proc_time t1 = 0, t2 = 0, t3 = 0;
        int repeats;
        if (n < 501)
            repeats = 8000 / n;
        else
            repeats = 100000 / n;

        for (int i = 0; i < repeats; ++i) {
            vector<Word> A(n);
            fillRandom(A);
            //fillRight(A);
            //fillBack(A);

            {
                vector<Word> B(A);
                unsigned long long time_in = __rdtsc();
                bubbleSort(B);
                unsigned long long time_out = __rdtsc();
                t1 += (time_out - time_in);
            }

            {
                vector<Word> B(A);
                unsigned long long time_in = __rdtsc();
                insertionSort(B);
                unsigned long long time_out = __rdtsc();
                t2 += (time_out - time_in);
            }

            {
                vector<Word> B(A);
                unsigned long long time_in = __rdtsc();
                selectionSort(B);
                unsigned long long time_out = __rdtsc();
                t3 += (time_out - time_in);
            }
        }

        double coef = 10000.0 * repeats;
        //printf("%5d %12.3lf %12.3lf %12.3lf      %5.1lf%%  %5.1lf%%  %5.1lf%%   %5d\n",
        //    n, t1 / coef, t2 / coef, t3 / coef,
        //    (((long double)t2 / (long double)t1) * 100),
        //    (((long double)t3 / (long double)t2) * 100), (((long double)t3 / (long double)t1) * 100), repeats);
        //printf("%5d %12.3lf %12.3lf %12.3lf                                     %5d\n",
        //    n, t1 / coef, t2 / coef, t3 / coef, repeats);
        // printf("%5d %12.3lf %12.3lf %12.3lf\n", n, t1 / coef, t2 / coef, t3 / coef); // standart
        printf("%5d & %12.3lf & %12.3lf & %12.3lf \\\\ \n", n, t1 / coef, t2 / coef, t3 / coef);
        //printf("(%5d, %12.3lf) (%5d, %12.3lf) (%5d, %12.3lf)\n", n, t1 / coef, n, t2 / coef, n, t3 / coef);
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
