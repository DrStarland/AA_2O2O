// lab1_Lev.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <intrin.h>
#include "matrix.h"

typedef short int Word;

using namespace std;

void print_table(Matrix<Word>& matr, string& s1, string& s2);
inline bool isNotEqual(char& a, char& b) { return  a != b; }
inline Word minLevenstein(Word a, Word b, Word c, char s1, char s2) {
    return (Word) min(min(a, b) + 1, c + isNotEqual(s1, s2));
}

Word findDistLev_matrix(string& word_vert, string& word_hor, bool print_flag = false);
Word findDistLev_rec(string& s1, string& s2, size_t n1, size_t n2);
Word findDistLev_rec_matr(string& s1, string& s2, bool print_flag = false);
Word findDistDamLev_matrix(string& s1, string& s2, bool print_flag = false);

Word calcMatr_rec(Matrix<Word>& matrix, string& s1, string& s2, size_t i, size_t j);


Word findDistLev_matrix(string& word_vert, string& word_hor, bool print_flag) {
    if (word_vert.length() == 0 || word_hor.length() == 0)
        return max(word_vert.length(), word_hor.length());

    size_t n = word_vert.length() + 1, m = word_hor.length() + 1;
    Matrix<Word> cost_matrix(n, m); {
        size_t i = 0;
        for (; i < min(n, m); cost_matrix[0][i] = cost_matrix[i][0] = i++);
        if (n < m) for (; i < m; cost_matrix[0][i] = i++);
        else for (; i < n; cost_matrix[i][0] = i++);
    }

    for (size_t i = 1; i < n; ++i)
        for (size_t j = 1; j < m; ++j)
            cost_matrix[i][j] = minLevenstein(cost_matrix[i][j - 1],
                cost_matrix[i - 1][j], cost_matrix[i - 1][j - 1],
                    word_vert[i - 1], word_hor[j - 1]);

    if (print_flag)
        print_table(cost_matrix, word_vert, word_hor);

    return cost_matrix[n - 1][m - 1];
}

Word findDistLev_rec(string& s1, string& s2, size_t n1, size_t n2) {
    if (n1 == 0) return n2;
    if (n2 == 0) return n1;

    return minLevenstein(findDistLev_rec(s1, s2, n1 - 1, n2), findDistLev_rec(s1, s2, n1, n2 - 1),
        findDistLev_rec(s1, s2, n1 - 1, n2 - 1), s1[n1 - 1], s2[n2 - 1]);
}

Word findDistLev_rec_matr(string& s1, string& s2, bool print_flag) {
    size_t n = s1.length() + 1, m = s2.length() + 1;
    Matrix<Word> cost_matrix(n, m);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            cost_matrix[i][j] = (i && j) ? -1 : (i + j);

    calcMatr_rec(cost_matrix, s1, s2, s1.length(), s2.length());

    if (print_flag)
        print_table(cost_matrix, s1, s2);

    return cost_matrix[n - 1][m - 1];
}
Word calcMatr_rec(Matrix<Word>& matrix, string& s1, string& s2, size_t i, size_t j) {
    if (matrix[i][j] != -1)  return matrix[i][j];

    matrix[i][j] = minLevenstein(calcMatr_rec(matrix, s1, s2, i - 1, j), calcMatr_rec(matrix, s1, s2, i, j - 1),
        calcMatr_rec(matrix, s1, s2, i - 1, j - 1), s1[i - 1], s2[j - 1]);
    return matrix[i][j];
}

Word findDistDamLev_matrix(string& s1, string& s2, bool print_flag) {
    if (s1.length() == 0 || s2.length() == 0)
        return max(s1.length(), s2.length());

    size_t n = s1.length() + 1, m = s2.length() + 1;
    Matrix<Word> cost_matrix(n, m); {
        size_t i = 0;
        for (; i < min(n, m); cost_matrix[0][i] = cost_matrix[i][0] = i++);
        if (n < m) for (; i < m; cost_matrix[0][i] = i++);
        else for (; i < n; cost_matrix[i][0] = i++);
    }

    for (size_t i = 1; i < n; ++i)
        for (size_t j = 1; j < m; ++j) {
            cost_matrix[i][j] = minLevenstein(cost_matrix[i][j - 1],
                cost_matrix[i - 1][j], cost_matrix[i - 1][j - 1],
                s1[i - 1], s2[j - 1]);
            if ((i > 1 && j > 1) && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1])
                cost_matrix[i][j] = min(cost_matrix[i][j], (Word) (cost_matrix[i - 2][j - 2] + 1));
        }

    if (print_flag)
        print_table(cost_matrix, s1, s2);

    return cost_matrix[n - 1][m - 1];
}

void time_experiment(string s1, string s2);

int main() {
    //string str1 = "KIT";
    //string str2 = "SKAT";

    string str1, str2;
    //cout << "Input the first word: ";
    //cin >> str1;// cout << endl;
    //cout << "Input the second word: ";
    //cin >> str2;

    cout << "first " << findDistLev_matrix(str1, str2, false) << endl;
    cout << "second " << findDistLev_rec(str1, str2, str1.length(), str2.length()) << endl;
    cout << "third " << findDistLev_rec_matr(str1, str2, false) << endl;
    cout << "last: " << findDistDamLev_matrix(str1, str2, false) << endl;

    int da = 1;
    while (da) {
        cout << "Input the first word: ";
        cin >> str1;// cout << endl;
        cout << "Input the second word: ";
        cin >> str2;
        cout << "first " << findDistLev_matrix(str1, str2, false) << endl;
        cout << "second " << findDistLev_rec(str1, str2, str1.length(), str2.length()) << endl;
        cout << "third " << findDistLev_rec_matr(str1, str2, false) << endl;
        cout << "last: " << findDistDamLev_matrix(str1, str2, false) << endl;
        time_experiment(str1, str2);
        cout << "da?\n";
        cin >> da;
    }
    getchar();
    return 0;
}

void time_experiment(string s1, string s2) {
    unsigned long long t1 = 0, t2 = 0, t3 = 0, t4 = 0;
    int repeats = 1000;
    for (int i = 0; i < repeats; ++i) {
        unsigned long long time_in = __rdtsc();
        int result = findDistLev_matrix(s1, s2);
        unsigned long long time_out = __rdtsc();
        t1 += (time_out - time_in);

        time_in = __rdtsc();
        result = findDistLev_rec(s1, s2, s1.length(), s2.length());
        time_out = __rdtsc();
        t2 += (time_out - time_in);

        time_in = __rdtsc();
        result = findDistLev_rec_matr(s1, s2);
        time_out = __rdtsc();
        t3 += (time_out - time_in);

        time_in = __rdtsc();
        result = findDistDamLev_matrix(s1, s2);
        time_out = __rdtsc();
        t4 += (time_out - time_in);
    }
    cout << "\n";
    cout << "T1: " << t1 / repeats << "\n";
    cout << "T2: " << t2 / repeats << "\n";
    cout << "T3: " << t3 / repeats << "\n";
    cout << "T4: " << t4 / repeats << "\n";
}

void print_table(Matrix<Word>& matr, string& s1, string& s2) {
    cout << "  Work table: \n";
    cout << " Vertical word: " << s1 << ", horyzontal: " << s2 << endl;
    cout << matr;
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
