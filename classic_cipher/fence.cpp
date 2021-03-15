#include <iostream>
#include <string>
#include <vector>
using namespace ::std;

void dec ()
{
    string plain;
    string cyc;
    int n = 0;
    cin >> n;
    cin >> cyc;
    int s = cyc.size();
    int len = 0;
    if (s%n==0) {
        len = s / n;
    } else {
        len = s / n + 1;
    }
    vector<vector<char>> table;
    for (int i=0; i<len; ++i) {
        vector<char> temp = {};
        table.push_back(temp);
    }
    int sig = 0;
    for (auto cc : cyc) {
        (table[sig]).push_back(cc);
        ++sig;
        sig %= len;
    }
    for (int i=0; i<len; ++i) {
        for (auto ch : table[i]) {
            cout << ch;
        }
    }
    cout << endl;

    return;
}

void enc ()
{
    vector<vector<char>> table;
    string plain;
    int n = 0;
    cin >> n;
    for (int i = 0; i<n; ++i) {
        vector<char> temp = {};
        table.push_back(temp);
    }
    cin >> plain;
    int sig = 0;
    for (auto pc : plain) {
        (table[sig]).push_back(pc);
        ++sig;
        sig %= n;
    }
    for (int i = 0; i<n; ++i) {
        for (auto ch : table[i]) {
            cout << ch;
        }
    }
    cout << endl;
    return;
}

int main ()
{
    char op;
    cout << "dec, enter d, else enter e:" << endl;
    cin >> op;
    if (op == 'd') {
        dec();
    } else if (op == 'e') {
        enc();
    }
    system("pause");
    return 0;
}
