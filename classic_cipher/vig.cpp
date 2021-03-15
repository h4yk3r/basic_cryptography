#include <iostream>
#include <string>
using namespace ::std;
#define LEAP 97

void enc ()
{
    string key;
    cin >> key;
    string plain;
    cin >> plain;
    string cyc;
    auto ik = key.begin();
    for (auto pc : plain) {
        int i = pc - LEAP;
        int j = (*ik) - LEAP;
        i += j;
        i %= 26;
        char ch = i + LEAP;
        cyc.push_back(ch);
        ++ik;
        if (ik==key.end()) {
            ik = key.begin();
        }
    }
    cout << cyc << endl;
}

void dec ()
{
    string key;
    string plain;
    string cyc;
    cin >> key;
    cin >> cyc;
    auto ik = key.begin();
    for (auto cc : cyc) {
        int i = cc - LEAP;
        int j = (*ik) - LEAP;
        i += 26;
        i -= j;
        i %= 26;
        char ch = i + LEAP;
        plain.push_back(ch);
        ++ik;
        if (ik==key.end()) {
            ik = key.begin();
        }
    }
    cout << plain << endl;
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