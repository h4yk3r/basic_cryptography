#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "aesutil.h"
using namespace ::std;
ofstream outfile;
/*
样例数据有25个字节，经过补位变成32个字节，64个二进制数字
*/
void printHEX(unsigned int i)
{
    char ch = 0;
    if (i >= 0 && i <= 9) {
        ch = i + '0';
    }
    else if (i >= 10 && i <= 15) {
        ch = i + 'a' -10;
    }
    outfile << ch;
    return;
}

void enc (string plain, string key)
{
    unsigned int subkey[44];
    get_subkey(key, subkey);

    unsigned int state[16];
    initial_text(plain, state);
    subkey_xor(0, state, subkey);
    for (int i = 1; i != 10; ++i) {
        substitution(state);
        row_shift(state);
        col_mix(state);
        subkey_xor(i, state, subkey);
    }
    substitution(state);
    row_shift(state);
    subkey_xor(10, state, subkey);
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    return;
}

void dec (string cipher, string key)
{
    unsigned int subkey[44];
    get_subkey(key, subkey);
    unsigned int state[16];
    initial_text(cipher, state);
    subkey_xor(10, state, subkey);
    for (int i = 9; i != 0; --i) {
        inv_row_shift(state);
        inv_substitution(state);
        subkey_xor(i, state, subkey);
        inv_col_mix(state);
    }
    inv_row_shift(state);
    inv_substitution(state);
    subkey_xor(0, state, subkey);
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    return;
}

int main ()
{
    outfile.open("outtext.txt", ios::out | ios::trunc);
    string key;//128bits; 16bytes
    cout << "enter the key: " << endl;
    cin >> key;

    cout << "read from infile" << endl;
    ifstream infile;
    infile.open("intext.txt", ios::in | ios::binary);

    string text = "";
    char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    char c = '\0';
    int cnt = 0;//读取的文本的长度
    while (infile >> c) {
        ++cnt;
        text.push_back(c);
    }
    int len = cnt/2;
    int lack = (16 - (len % 16));
    if (lack != 16) {
        len += lack;//字节数
        for (int i = 0; i != lack; ++i) {
            text.push_back('0');
            text.push_back(hex[lack]);
        }
    }
    int blocknum = len/16;
    string subs('0', 32);
    char option;
    cout << "Enter the option, e to enc, d to dec" << endl;
    cin >> option;
    for (int i=0; i!=blocknum; ++i) {
        for (int j=0; j!=32; ++j) {
            subs[j] = text[i*32+j];
        }
        //cout << subs << endl;
        if (option=='e') {
            enc(subs, key);
        } else if (option=='d') {
            dec(subs, key);
            //outfile << endl;
        }
    }

    cout << "print in outfile(ECB)" << endl;

    return 0;
}
