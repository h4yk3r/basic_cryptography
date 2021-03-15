#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "aesutil.h"
using namespace ::std;
ofstream outfile;

void iv_shift (string &iv, string temp, int shiftlen)
{
    for (int i=0; i!=(iv.size()-shiftlen); ++i) {
        iv[i] = iv[i+shiftlen];
    }
    for (int i=0; i!=shiftlen; ++i) {
        iv[iv.size()-shiftlen+i] = temp[i];
    }
    return ;
}

void iv_xor (string iv, string &subs, int sublen)
{
    //int sublen = subs.size();
    char temp[2];
    int lr[2];
    for (int i=0; i!=sublen; ++i) {
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

void encdec (string &iv, string key)
{
    unsigned int subkey[44];
    get_subkey(key, subkey);

    unsigned int state[16];
    initial_text(iv, state);
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

    int blocklen;//一个分组的字节数
    cout << "enter the length of block: " << endl;
    cin >> blocklen;
    cout << "read from infile" << endl;
    ifstream infile;
    infile.open("intext.txt", ios::in | ios::binary);

    //char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    string text = "";

    char c = '\0';
    int cnt = 0;//读取的文本的长度
    while (infile >> c) {
        ++cnt;
        text.push_back(c);
    }
    int len = cnt/2;//字节数

    //分组数量，所有正常字节长的分组
    int blocknum = len/blocklen;
    //正常长度的字节长度
    int nor = blocknum*blocklen;
    //最后一个字节的长度
    int last = 0;
    if (nor<len) {
        last = len - nor;
    }

    string subs = "";
    for (int i=0; i!=(blocklen*2); ++i) {
        subs.push_back('0');
    }

    //循环处理所有正常长度分组
    int count = blocklen*2;
    for (int i = 0; i != blocknum; ++i) {
        for (int j=0; j!=count; ++j) {
            subs[j] = text[i*count+j];
        }
        string tv = iv;

        encdec(tv, key);
        string temp = "";
        for (int k=0; k!=count; ++k) {
            temp.push_back(tv[k]);
        }
        iv_xor(temp, subs, count);
        outfile << subs;
        iv_shift(iv, temp, count);
    }
    //如果最后一个字节的分组长度不为0则处理
    if (last!=0) {
        string lastsub = "";
        int lastcount = last*2;
        for (int j=0; j!=lastcount; ++j) {
            lastsub.push_back(text[nor*2+j]);
        }
        string tv = iv;

        encdec(tv, key);
        string temp = "";
        for (int k=0; k!=lastcount; ++k) {
            temp.push_back(tv[k]);
        }
        iv_xor(temp, lastsub, lastcount);
        outfile << lastsub;
        iv_shift(iv, temp, lastcount);
    }

    cout << "print in outfile(OFB)" << endl;

    return 0;
}
