// lab_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "matrix.h"
#include <random>
#include <ctime>
#include <intrin.h>
#include <string.h>


typedef short int Word;

void fillRandom(Matrix<Word>& mat);
void time_experiment();

using namespace::std;

Matrix<Word> multiplicationVinogradOptimised(const Matrix<Word>& matA, const Matrix<Word>& matB) {
    {   time_t _time = time(NULL);
    if (!matA.isSuitForMult(matB))
        throw MultiplicationException(__FILE__, typeid(matA).name(), __LINE__, ctime(&_time)); }

    const size_t m = matA.rows(), q = matB.columns();
    Matrix<Word> res(m, q);
    size_t temp_size = (matA.columns() >> 1) << 1;
    Matrix<Word>::MatrixLine rowFactor(m), columnFactor(q);
    Word temp;

    for (size_t i = 0; i < m; ++i) {
        temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += (matA(i, k) * matA(i, k + 1));
        rowFactor[i] = temp;
    }
    for (size_t i = 0; i < q; ++i) {
        temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += matB(k, i) * matB(k + 1, i);
        columnFactor[i] = temp;
    }

    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < q; ++j) {
            temp = -(rowFactor[i] + columnFactor[j]);
            for (size_t k = 0; k < temp_size; k += 2) {
                temp += (matA(i, k) + matB(k + 1, j))
                    * (matA(i, k + 1) + matB(k, j));
            }
            res(i, j) = temp;
        }

    if ((temp_size) != matA.columns())
        for (size_t i = 0, t = temp_size; i < m; ++i)
            for (size_t j = 0; j < q; ++j)
                res(i, j) += matA(i, t) * matB(t, j);
    return res;
}

void calcRow(Matrix<Word>::MatrixLine& rowFactor, const Matrix<Word>& mat, size_t temp_size, size_t i, size_t end) {
    for ( ; i < end; ++i) {
        Word temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += mat(i, k) * mat(i, k + 1);
        rowFactor[i] = temp;
    }
};

void calcColumn(Matrix<Word>::MatrixLine& columnFactor, const Matrix<Word>& mat, size_t temp_size, size_t j, size_t end) {
    for ( ; j < end; ++j) {
        Word temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += mat(k, j) * mat(k + 1, j);
        columnFactor[j] = temp;
    }
};

Matrix<Word> multiplicationVinogradParallel_1(const Matrix<Word>& matA, const Matrix<Word>& matB, size_t threadNum) {
    {   time_t _time = time(NULL);
    if (!matA.isSuitForMult(matB))
        throw MultiplicationException(__FILE__, typeid(matA).name(), __LINE__, ctime(&_time)); }

    const size_t m = matA.rows(), q = matB.columns();
    Matrix<Word> res(m, q);
    size_t temp_size = (matA.columns() >> 1) << 1;
    Matrix<Word>::MatrixLine rowFactor(m), columnFactor(q);

    shared_ptr<std::thread[]> threads = shared_ptr<std::thread[]>(new std::thread[threadNum]);

    // int m, int n, int q ||||| N a = m, M a = n, M b = q 
    if (threadNum > 1) {
        size_t proportion = threadNum * m / (m + q);
        if (proportion == 0)
            ++proportion;

        size_t rows_per_t = m / proportion, cols_per_t = q / (threadNum - proportion);

        size_t i = 0;
        for (size_t start_row = 0, end_row; i < proportion; i++, start_row = end_row) {
            end_row = (i == proportion - 1) ? m : start_row + rows_per_t;
            threads[i] = std::thread(&calcRow,
                                    std::ref(rowFactor), std::ref(matA), temp_size, start_row, end_row);
        }
        // i продолжает использоваться
        for (size_t start_col = 0, end_col; i < threadNum; ++i, start_col = end_col) {
            end_col = (i == threadNum - 1) ? q : start_col + cols_per_t;
            threads[i] = std::thread(&calcColumn,
                                    std::ref(columnFactor), std::ref(matB), temp_size, start_col, end_col);
        }

        for (size_t i = 0; i < threadNum; ++i)
            threads[i].join();

    }
    else {
        calcRow(std::ref(rowFactor), std::ref(matA), temp_size, 0, m);
        calcColumn(std::ref(columnFactor), std::ref(matB), temp_size, 0, q);
    }

    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < q; ++j) {
            Word temp = -(rowFactor[i] + columnFactor[j]);
            for (size_t k = 0; k < temp_size; k += 2) {
                temp += (matA(i, k) + matB(k + 1, j))
                    * (matA(i, k + 1) + matB(k, j));
            }
            res(i, j) = temp;
        }

    if ((temp_size) != matA.columns())
        for (size_t i = 0, t = temp_size; i < m; ++i)
            for (size_t j = 0; j < q; ++j)
                res(i, j) += matA(i, t) * matB(t, j);

    return res;
}

void heartOfVinograd(const Matrix<Word> &matA, const Matrix<Word> &matB, Matrix<Word> &res,
    Matrix<Word>::MatrixLine &rowFactor, Matrix<Word>::MatrixLine &columnFactor, size_t temp_size, size_t i, size_t end)
{
    for ( ; i < end; ++i)
        for (size_t j = 0; j < matB.columns(); ++j) {
            Word temp = -(rowFactor[i] + columnFactor[j]);
            for (size_t k = 0; k < temp_size; k += 2) {
                temp += (matA(i, k) + matB(k + 1, j))
                    * (matA(i, k + 1) + matB(k, j));
            }
            res(i, j) = temp;
        }
}

Matrix<Word> multiplicationVinogradParallel_2(const Matrix<Word>& matA, const Matrix<Word>& matB, size_t threadNum) {
    {   time_t _time = time(NULL);
    if (!matA.isSuitForMult(matB))
        throw MultiplicationException(__FILE__, typeid(matA).name(), __LINE__, ctime(&_time)); }

    const size_t m = matA.rows(), q = matB.columns();
    Matrix<Word> res(m, q);
    size_t temp_size = (matA.columns() >> 1) << 1;
    Matrix<Word>::MatrixLine rowFactor(m), columnFactor(q);

    calcRow(std::ref(rowFactor), std::ref(matA), temp_size, 0, m);
    calcColumn(std::ref(columnFactor), std::ref(matB), temp_size, 0, q);

    // int m, int n, int q ||||| N a = m, M a = n, M b = q 

    shared_ptr<std::thread[]> threads = shared_ptr<std::thread[]>(new std::thread[threadNum]);

    size_t rows_per_t = m / threadNum;  // сколько строк обрабатывает один поток

    for (size_t i = 0, start_row = 0, end_row; i < threadNum; ++i, start_row = end_row) {
        end_row = (i == threadNum - 1) ? m : start_row + rows_per_t;
        threads[i] = std::thread(&heartOfVinograd, std::ref(matA), std::ref(matB), std::ref(res),
                std::ref(rowFactor), std::ref(columnFactor), temp_size, start_row, end_row);
    }

    for (int i = 0; i < threadNum; ++i)
        threads[i].join();

    if ((temp_size) != matA.columns())
        for (size_t i = 0, t = temp_size; i < m; ++i)
            for (size_t j = 0; j < q; ++j)
                res(i, j) += matA(i, t) * matB(t, j);

    return res;
}

int main() {
    std::cout << "_________________________________\n";

    size_t m = 4, n = 6, q = 7;
    Matrix<Word> A(m, n);
    Matrix<Word> B(n, q);
    fillRandom(A), fillRandom(B);
    std::cout << "Source matrices: \n" << A << endl << B << "--------------------------------------\n";

    try {
        std::cout << "\n\nspecial\n";
        std::cout << A.multiplication(B) << endl;
        std::cout << multiplicationVinogradOptimised(A, B) << endl;
        std::cout << multiplicationVinogradParallel_1(A, B, 4) << endl;
        std::cout << multiplicationVinogradParallel_2(A, B, 4);
        std::cout << "\n" << (0 << 1) << "\n";
    } catch (exception& e) { std::cout << "ups " << e.what(); }
    char nothing;
    std::cout << "Press enter to start if you ready to wait...\n";
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
    Matrix<Word> array{
        { 10,  100, 200, 300, 400, 500 }, { 12, 12, 12, 12, 12, 12 }, 
        { 360, 360, 360, 360, 360, 360 }, { 1,  2,  6,  12, 24, 48 }
    };

    char buffer[255];

    typedef unsigned long long proc_time;

    {   FILE* temp = fopen("logTable.txt", "rt");
        if (temp == NULL) temp = fopen("logTable.txt", "wt");
        fclose(temp);
        temp = fopen("logPlot.txt", "rt");
        if (temp == NULL) temp = fopen("logPlot.txt", "wt");
        fclose(temp); }

    std::ofstream ofs;
    std::ofstream ofsII;

    std::cout << "t1 - default Vinograd, t2 - Vinograd I, t3 - Vinograd II\n";
    std::cout << "    N          t1           t2           t3          \n";
    ofs.open("logTable.txt", std::ofstream::app), ofsII.open("logPlot.txt", std::ofstream::app);
    ofs << "t1 - default Vinograd, t2 - Vinograd I, t3 - Vinograd II\n";
    ofs << "    N          t1           t2           t3          \n";
    ofsII << "t1 - default Vinograd, t2 - Vinograd I, t3 - Vinograd II\n";
    ofsII << "    N          t1           t2           t3          \n";
    ofs.close(), ofsII.close();

    for (size_t ind = 0; ind < array.rows(); ++ind, ++ind)
        for (size_t j = 0; j < array.columns(); ++j) {
            ofs.open("logTable.txt", std::ofstream::app), ofsII.open("logPlot.txt", std::ofstream::app);

            size_t n = array[ind][j];
            Matrix<Word> A(n, n), B(n, n);
            fillRandom(A), fillRandom(B);

            proc_time t1 = 0, t2 = 0, t3 = 0;
            int repeats = 25;
            for (int i = 0; i < repeats; ++i) {

                unsigned long long time_in = __rdtsc();
                multiplicationVinogradOptimised(A, B);
                unsigned long long time_out = __rdtsc();
                t1 += (time_out - time_in);

                time_in = __rdtsc();
                multiplicationVinogradParallel_1(A, B, array[ind + 1][j]);
                time_out = __rdtsc();
                t2 += (time_out - time_in);

                time_in = __rdtsc();
                multiplicationVinogradParallel_2(A, B, array[ind + 1][j]);
                time_out = __rdtsc();
                t3 += (time_out - time_in);
            }

            double coef = 100000.0 * repeats; // 10^-5

            size_t tempio = ind == 0 ? n : array[ind + 1][j];
            snprintf(buffer, 255, "%4lld & %12.3lf & %12.3lf & %12.3lf \\\\ \n",
                        tempio, t1 / coef, t2 / coef, t3 / coef);
            std::cout << buffer;
            ofs << buffer;

            snprintf(buffer, 255, "(%4lld, %12.3lf) (%4lld, %12.3lf) (%4lld, %12.3lf) \n",
                tempio, t1 / coef, tempio, t2 / coef, tempio, t3 / coef);
            // std::cout << buffer;
            ofsII << buffer;
            
            ofs.close(), ofsII.close();
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
