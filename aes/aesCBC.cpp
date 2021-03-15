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
void iv_xor (string iv, string &subs)
{
    char temp[2];
    int lr[2];
    for (int i=0; i!=32; ++i) {
        temp[0] = iv[i];
        temp[1] = subs[i];
        for (int i=0; i!=2; ++i) {
            if (temp[i] >='0' && temp[i] <= '9') {
                lr[i] = temp[i] - '0';
            } else if (temp[i]>= 'a' && temp[i]<='f') {
                lr[i] = temp[i] - 'a' + 10;
            } else if (temp[i]>= 'A' && temp[i]<='F') {
                lr[i] = temp[i] - 'A' + 10;
            }
        }
        int res = lr[0] ^ lr[1];
        char ch = 0;
        if (res >= 0 && res <= 9) {
            ch = res + '0';
        }
        else if (res >= 10 && res <= 15) {
            ch = res + 'a' -10;
        }
        subs[i] = ch;
    }
    return;
}

char printHEX(unsigned int i)
{
    char ch = 0;
    if (i >= 0 && i <= 9) {
        ch = i + '0';
    }
    else if (i >= 10 && i <= 15) {
        ch = i + 'a' -10;
    }
    //outfile << ch;
    return ch;
}

void enc (string plain, string key, string &iv)
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

    string temp;
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        temp.push_back(printHEX(high));
        temp.push_back(printHEX(low));
    }
    iv = temp;

    return ;
}

void dec (string cipher, string key, string &iv)
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

    string temp;
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        temp.push_back(printHEX(high));
        temp.push_back(printHEX(low));
    }
    iv_xor(iv, temp);
    outfile << temp;
    iv = cipher;

    return;
}

int main ()
{
    outfile.open("outtext.txt", ios::out | ios::trunc);
    string key;//128bits; 16bytes
    cout << "enter the key: " << endl;
    cin >> key;
    string iv;
    cout << "enter the iv: " << endl;
    cin >> iv;
    cout << "read from infile" << endl;
    ifstream infile;
    infile.open("intext.txt", ios::in | ios::binary);

    char option;
    cout << "Enter the option, e to enc, d to dec" << endl;
    cin >> option;

    char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    string text = "";

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

    for (int i=0; i!=blocknum; ++i) {
        for (int j=0; j!=32; ++j) {
            subs[j] = text[i*32+j];
        }
        //cout << subs << endl;
        if (option=='e') {
            iv_xor(iv, subs);
            enc(subs, key, iv);
            outfile << iv;
        } else if (option=='d') {
            dec(subs, key, iv);
        }
    }

    cout << "print in outfile(CBC)" << endl;

    return 0;
}
