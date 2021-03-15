#include <iostream>
#include <string>
using namespace ::std;
#define LEAP 97
/*
char table[26] = {'q','w','e','r','t','y','u','i','o','p',
                        'a','s','d','f','g','h','j','k','l',
                            'z','x','c','v','b','n','m'};
                            */
char table[26];
char arc_table[26];

void get_table ()
{
    string key;
    cin >> key;
    for (int i=0; i<26; ++i) {
        table[i] = key[i];
    }
    return;
}

void arc ()
{
    for (int i = 0; i<26; ++i) {
        int temp = table[i] - LEAP;
        arc_table[temp] = i + LEAP;
    }
}

void enc ()
{
    string p;
    string c;
    cin >> p;
    for (auto pc : p) {
        int i = pc - LEAP;
        char ch = table[i];
        c.push_back(ch);
    }
    cout << c << endl;
}

void dec ()
{
    string p;
    string c;
    cin >> c;
    for (auto cc : c) {
        int i = cc - LEAP;
        char ch = arc_table[i];
        p.push_back(ch);
    }
    cout << p << endl;
}


int main ()
{
    get_table();
    arc();
    char key;
    cout << "dec, enter d, else enter e:" << endl;
    cin >> key;
    if (key == 'd') {
        dec();
    } else if (key == 'e') {
        enc();
    }
    system("pause");

    return 0;
}
