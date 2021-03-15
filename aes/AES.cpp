#include <iostream>
#include <string>
#include <vector>
#include "aesutil.h"
using namespace ::std;

void printHEX(unsigned int i)
{
    char ch = 0;
    if (i >= 0 && i <= 9) {
        ch = i + '0';
    }
    else if (i >= 10 && i <= 15) {
        ch = i + 'a' -10;
    }
    cout << ch;
    return;
}

void enc()
{
    string key;
    cout << "enter the key: " << endl;
    cin >> key;
    unsigned int subkey[44];
    get_subkey(key, subkey);
    /*
    cout << "subkey: " << endl;
    for (int i = 0; i != 44; ++i) {
        cout << subkey[i] << " ";
    }
    */
    cout << endl;
    string plain;
    cout << "enter the plain text: " << endl;
    cin >> plain;
    unsigned int state[16];
    initial_text(plain, state);
    /*
    cout << "plain text initial: ";
    for (int j = 0; j != 16; ++j) {
        cout << state[j] << " ";
    }
    cout << endl;
    */
    subkey_xor(0, state, subkey);
    /*
    cout << "first time xor: ";
    for (int j = 0; j != 16; ++j) {
        cout << state[j] << " ";
    }
    cout << endl;
    */
    for (int i = 1; i != 10; ++i) {
        substitution(state);
        /*
        cout << "sub: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        row_shift(state);
        /*
        cout << "row: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        col_mix(state);
        /*
        cout << "col: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        subkey_xor(i, state, subkey);
        /*
        cout << "xor: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
    }
    substitution(state);
    /*
    cout << "sub: ";
    for (int i = 0; i != 16; ++i) {
        cout << state[i] << " ";
    }
    cout << endl;
    */
    row_shift(state);
    /*
    cout << "row: ";
    for (int i = 0; i != 16; ++i) {
        cout << state[i] << " ";
    }
    cout << endl;
    */
    subkey_xor(10, state, subkey);
    /*
    cout << "xor: ";
    for (int i = 0; i != 16; ++i) {
        cout << state[i] << " ";
    }
    cout << endl;
    */
    cout << endl;
    cout << "the cipher is: " << endl;
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    cout << endl;
    return;
}

void dec()
{
    string key;
    cout << "enter the key: " << endl;
    cin >> key;
    unsigned int subkey[44];
    get_subkey(key, subkey);
    /*
    cout << "subkey: " << endl;
    for (int i = 0; i != 44; ++i) {
        cout << subkey[i] << " ";
    }
    */
    cout << endl;
    string cipher;
    cout << "enter the cipher text: " << endl;
    cin >> cipher;
    unsigned int state[16];
    initial_text(cipher, state);
    /*
    for (int i = 0; i != 16; ++i) {
        cout << state[i] << " ";
    }
    cout << endl;
    */
    subkey_xor(10, state, subkey);
    /*
    for (int i = 0; i != 16; ++i) {
        cout << state[i] << " ";
    }
    cout << endl;
    */
    for (int i = 9; i != 0; --i) {
        inv_row_shift(state);
        /*
        cout << "row: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        inv_substitution(state);
        /*
        cout << "sub: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        subkey_xor(i, state, subkey);
        /*
        cout << "xor: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
        inv_col_mix(state);
        /*
        cout << "col: ";
        for (int i = 0; i != 16; ++i) {
            cout << state[i] << " ";
        }
        cout << endl;
        */
    }
    inv_row_shift(state);
    inv_substitution(state);
    subkey_xor(0, state, subkey);
    cout << endl;
    cout << "the plain is: " << endl;
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    cout << endl;
    return;
}

int main()
{
	for (int i = 0; i < 64; ++i) {
		cout << "*";
	}
	cout << endl;
	cout << "		  " << "|AES enc&dec|" << endl;
	char option;
	cout << "enter option, e to encrypt, d to dectrypt: " << endl;
	cin >> option;
	if (option == 'e') {
		enc();
	}
	else if (option == 'd') {
		dec();
	}
    cout << endl;
    for (int i = 0; i < 64; ++i) {
        cout << "*";
    }
    cout << endl;
    system("pause");
    return 0;
}
