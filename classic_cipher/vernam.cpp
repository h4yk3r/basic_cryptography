#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace ::std;
#define LEAP 64

void enc (string in)
{
    string plain = in;
    string cipher;
    string key;
    cin >> key;
    int len = key.size();
    for (int i=0; i<len; ++i) {
        int pc = plain[i];
        if (pc>=LEAP) {
            pc -= LEAP;
        }
        int kc = key[i];
        if (kc>=LEAP) {
            kc -= LEAP;
        }
        int cc = pc ^ kc;
        char ch = cc + LEAP;
        cipher.push_back(ch);
    }
    ofstream outfile("output.txt");
    outfile << plain << endl;
    return;
}

void dec (string in)
{
    string key;
    string plain;
    string cipher = in;
    cin >> key;
    int len = key.size();
    for (int i=0; i<len; ++i) {
        int pc = cipher[i];
        if (pc>=LEAP) {
            pc -= LEAP;
        }
        int kc = key[i];
        if (kc>=LEAP) {
            kc -= LEAP;
        }
        int cc = pc ^ kc;
        char ch = cc + LEAP;
        plain.push_back(ch);
    }
    ofstream outfile("output.txt");
    outfile << plain << endl;
    return;
}

int main ()
{
    ifstream infile("input.txt");
    string in;
    getline(infile, in);
    char op;
    cout << "dec, enter d, else enter e:" << endl;
    cin >> op;
    if (op == 'd') {
        dec(in);
    } else if (op == 'e') {
        enc(in);
    }
    system("pause");
    return 0;
}
