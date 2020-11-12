// lab_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "matrix.h"
#include <random>
#include <ctime>
#include <intrin.h>

typedef short int Word;

void fillRandom(Matrix<Word>& mat);
void time_experiment();

using namespace::std;

int main()
{
    std::cout << "Hello World!\n";

    cout << "_________________________________\n";

    size_t m = 3, n = 5, q = 2;
    Matrix<Word> A(m, n);
    Matrix<Word> B(n, q);
    fillRandom(B);
    fillRandom(A);
    //cout << B << endl;
    cout << "Source matrices: \n" << A << endl << B << "--------------------------------------\n";

    try {
        cout << "\n\nspecial\n";
        cout << A.multiplication(B) << endl;
        cout << A.multiplicationVinograd(B) << endl;
        cout << A.multiplicationVinogradOptimised(B);
        cout << "\n" << (0 << 1) << "\n";
    } catch (exception& e) { cout << "ups " << e.what(); }
    char nothing;
    cout << (long double) 380000000 << endl;
    cout << "Press enter to start if you ready to wait...\n";
    cin >> nothing;
    time_experiment();
    return 0;
}

void fillRandom(Matrix<Word>& mat) {
    std::random_device rd;
    std::mt19937 mersenne(rd());

    for (Word& i : mat) {
        i = mersenne() % 33;
    }
}

void time_experiment() {
    Matrix<Word> array{ { 10, 100, 200, 300, 400, 500 }, { 11, 101, 201, 301, 401, 501 } };
    typedef unsigned long long proc_time;
    cout << "t1 - default, t2 - Vinograd, t3 - Vinograd optimised\n";
    cout << "N    t1       t2       t3       t2/t1%  t3/t2%  t3/t1%     repeats\n";

    for (size_t i = 0; i < array.rows(); ++i)
        for (size_t j = 0; j < array.columns(); ++j) {
            size_t n = array[i][j];
            Matrix<Word> A(n, n), B(n, n);
            fillRandom(A), fillRandom(B);

            proc_time t1 = 0, t2 = 0, t3 = 0;
            int repeats = 7500 / n + 5;
            for (int i = 0; i < repeats; ++i) {
                unsigned long long time_in = __rdtsc();
                A.multiplication(B);
                unsigned long long time_out = __rdtsc();
                t1 += (time_out - time_in);

                time_in = __rdtsc();
                A.multiplicationVinograd(B);
                time_out = __rdtsc();
                t2 += (time_out - time_in);

                time_in = __rdtsc();
                A.multiplicationVinogradOptimised(B);
                time_out = __rdtsc();
                t3 += (time_out - time_in);
            }
            //cout << "\n" << n << "  ";
            //double coef = 100000.0;
            //cout << t1 / repeats / coef<< " ";
            //cout << t2 / repeats / coef << " ";
            //cout << t3 / repeats / coef << " ";
            //cout << (((long double) t2 / (long double) t1) * 100) << "% ";
            //cout << (((long double)t3 / (long double)t2) * 100) << "% ";
            //cout << (((long double)t3 / (long double)t1) * 100) << "%  " << repeats << "\n";
            cout << "\n" << n << " & ";
            double coef = 100000.0 * repeats;
            cout << t1 / coef << " & ";
            cout << t2 / coef << " & ";
            cout << t3 / coef << " & ";
            cout << (((long double)t3 / (long double)t1) * 100) << "%  \\\\ \n";
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
