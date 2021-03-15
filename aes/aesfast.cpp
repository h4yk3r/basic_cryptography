#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "aesfastutil.h"
using namespace ::std;

int te0[256];// 02 01 01 03
int te1[256];// 03 02 01 01
int te2[256];// 01 03 02 01
int te3[256];// 01 01 03 02

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

unsigned int s[16][16] = {
    //0     1     2      3    4     5     6     7      8    9     A     B     C     D     E     F
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, //0
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}, //1
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15}, //2
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75}, //3
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84}, //4
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf}, //5
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8}, //6
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2}, //7
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73}, //8
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb}, //9
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79}, //A
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08}, //B
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a}, //C
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e}, //D
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf}, //E
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16} }; //F

void te_init ()
{
    for (int i=0; i!=256; ++i) {
        int temp;

        int row = 0, col = 0;
        row = i >> 4;
        col = i - (row << 4);
        temp = s[row][col];

        int sig = temp;
        int dou = gf_mul(2, temp);
        int tri = gf_mul(3, temp);
        te0[i] = tri + (sig << 8) + (sig << 16) + (dou << 24);
        te1[i] = sig + (sig << 8) + (dou << 16) + (tri << 24);
        te2[i] = sig + (dou << 8) + (tri << 16) + (sig << 24);
        te3[i] = dou + (tri << 8) + (sig << 16) + (sig << 24);
    }
    return ;
}

void te_box (int round, unsigned int* state, unsigned int(&roundkey)[44])
{
    for (int i=0; i!=4; ++i) {
        unsigned int res = te0[state[i*4+0]]^te1[state[i*4+1]]^te2[state[i*4+2]]^te3[state[i*4+3]]^roundkey[4*round+i];
        //cout << "res: " << hex << res << endl;
        state[i*4+0] = (res & 0xFF000000) >> 24;
        state[i*4+1] = (res & 0x00FF0000) >> 16;
        state[i*4+2] = (res & 0x0000FF00) >> 8;
        state[i*4+3] = res & 0x000000FF;
    }
    return ;
}

int td0[256];// 14 09 13 11
int td1[256];// 11 14 09 13
int td2[256];// 13 11 14 09
int td3[256];// 09 13 11 14

unsigned int invs[16][16] = {
    /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
    {0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb}, /*0*/
    {0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb}, /*1*/
    {0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e}, /*2*/
    {0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25}, /*3*/
    {0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92}, /*4*/
    {0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84}, /*5*/
    {0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06}, /*6*/
    {0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b}, /*7*/
    {0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73}, /*8*/
    {0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e}, /*9*/
    {0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b}, /*a*/
    {0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4}, /*b*/
    {0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f}, /*c*/
    {0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef}, /*d*/
    {0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61}, /*e*/
    {0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}  /*f*/
};

void td_init ()
{
    for (int i=0; i!=256; ++i) {
        int temp;

        int row = 0, col = 0;
        row = i >> 4;
        col = i - (row << 4);
        temp = invs[row][col];

        int nin = gf_mul(9, temp);
        int ele = gf_mul(11, temp);
        int tri = gf_mul(13, temp);
        int frz = gf_mul(14, temp);
        td0[i] = ele + (tri << 8) + (nin << 16) + (frz << 24);
        td1[i] = tri + (nin << 8) + (frz << 16) + (ele << 24);
        td2[i] = nin + (frz << 8) + (ele << 16) + (tri << 24);
        td3[i] = frz + (ele << 8) + (tri << 16) + (nin << 24);
    }
    /*
    cout << hex;
    for (int j=0; j!=256; ++j) {
        cout << td0[j] << " ";
    }
    cout << endl;
    */
    return ;
}

int key0[256];// 14 09 13 11
int key1[256];// 11 14 09 13
int key2[256];// 13 11 14 09
int key3[256];// 09 13 11 14

void key_box_init ()
{
    for (int i=0; i!=256; ++i) {
        int nin = gf_mul(9, i);
        int ele = gf_mul(11, i);
        int tri = gf_mul(13, i);
        int frz = gf_mul(14, i);
        key0[i] = ele + (tri << 8) + (nin << 16) + (frz << 24);
        key1[i] = tri + (nin << 8) + (frz << 16) + (ele << 24);
        key2[i] = nin + (frz << 8) + (ele << 16) + (tri << 24);
        key3[i] = frz + (ele << 8) + (tri << 16) + (nin << 24);
    }
    return ;
}

void td_box (int round, unsigned int* state, unsigned int(&roundkey)[44])
{
    for (int i=0; i!=4; ++i) {
        unsigned int res = td0[state[i*4+0]]^td1[state[i*4+1]]^td2[state[i*4+2]]^td3[state[i*4+3]]^roundkey[4*round+i];
        state[i*4+0] = (res & 0xFF000000) >> 24;
        state[i*4+1] = (res & 0x00FF0000) >> 16;
        state[i*4+2] = (res & 0x0000FF00) >> 8;
        state[i*4+3] = res & 0x000000FF;
    }
    return ;
}

void enc()
{
    clock_t start_t, end_t;
    string key;
    cout << "enter the key: " << endl;
    //cin >> key; //初始密钥
    key = "0f1571c947d9e8590cb7add6af7f6798";
    cout << "test mode" << endl;
    string plain;
    cout << "enter the plain text: " << endl;
    //cin >> plain; //明文
    plain = "0123456789abcdeffedcba9876543210";
    cout << "test mode" << endl;
    cout << "start timing" << endl;
    start_t = clock();
    unsigned int subkey[44];
    get_subkey(key, subkey); //得到子密钥，16个字节（4个ui），一共11个子密钥，0-43
    //cout << endl;
    unsigned int state[16]; //状态方阵，16个数据
    initial_text(plain, state); //32个16进制数字变为16个数据的初始状态方阵
    subkey_xor(0, state, subkey); //第0轮轮密钥加
    //1-8
    for (int i=1; i!=5; ++i) {
        row_shift(state);
        te_box(2*i-1, state, subkey);
        row_shift(state);
        te_box(2*i, state, subkey);
    }
    //9
    row_shift(state);
    te_box(9, state, subkey);

    //第10轮
    row_shift(state); //行位移
    substitution(state); //字节替换，查找S盒进行
    subkey_xor(10, state, subkey); //第10轮轮密钥加
    end_t = clock();
    cout << "stop timing" << endl;
    cout << "the cipher is: " << endl; //得到16个字节的密文结果，将其化为32字节的16进制数字
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    cout << endl;
    //cout.setf(ios::fixed,ios::floatfield);
    cout << "enc time: " << (double)(end_t - start_t)*1000 / CLOCKS_PER_SEC << "ms" << endl;
    return;
}

void key_mix (unsigned int* mixkey, unsigned int(&subkey)[44])
{
    for (int i=0; i!=11; ++i) {
        if (i==0 || i==11) {
            continue;
        }
        unsigned int temp[16];
        for (int j=0; j!=4; ++j) {
            temp[j*4+0] = (subkey[i*4+j] & 0xFF000000) >> 24;
            temp[j*4+1] = (subkey[i*4+j] & 0x00FF0000) >> 16;
            temp[j*4+2] = (subkey[i*4+j] & 0x0000FF00) >> 8;
            temp[j*4+3] = subkey[i*4+j] & 0x000000FF;
        }
        for (int k=0; k!=4; ++k) {
            mixkey[i*4+k] =
            key0[temp[k*4+0]]^key1[temp[k*4+1]]^key2[temp[k*4+2]]^key3[temp[k*4+3]];
        }
    }
    return ;
}

void dec()
{
    clock_t start_t, end_t;
    string key;
    cout << "enter the key: " << endl;
    //cin >> key; //初始密钥
    key = "0f1571c947d9e8590cb7add6af7f6798";
    cout << "test mode" << endl;
    cout << endl;
    string cipher;
    cout << "enter the cipher text: " << endl;
    //cin >> cipher;
    cipher = "ff0b844a0853bf7c6934ab4364148fb9";
    cout << "test mode" << endl;
    cout << "start timing" << endl;
    start_t = clock();
    unsigned int subkey[44];
    get_subkey(key, subkey);
    unsigned int mixkey[44];
    key_mix(mixkey, subkey);
    unsigned int state[16];
    initial_text(cipher, state);// 将16进制序列转换为状态方阵
    //初始
    subkey_xor(10, state, subkey);// 初始密钥加
    //9,8,7,6,5,4,3,2
    for (int i=4; i!=0; --i) {
        inv_row_shift(state);
        td_box(2*i+1, state, mixkey);
        inv_row_shift(state);
        td_box(2*i, state, mixkey);
    }
    //1
    inv_row_shift(state);
    td_box(1, state, mixkey);
    //最后一轮，不变动
    inv_row_shift(state);// 逆向列混淆
    inv_substitution(state);// 逆向字节替换
    subkey_xor(0, state, subkey);// 密钥加

    end_t = clock();
    cout << "stop timing" << endl;
    cout << "the plain is: " << endl;
    for (int i = 0; i != 16; ++i) {
        unsigned int high = 0, low = 0;
        high = state[i] >> 4;
        low = state[i] - (high << 4);
        printHEX(high);
        printHEX(low);
    }
    cout << endl;
    //cout.setf(ios::fixed,ios::floatfield);
    cout << "dec time: " << (double)(end_t - start_t)*1000 / CLOCKS_PER_SEC << "ms" << endl;
    return;
}

int main()
{
	initial_gf();

    for (int i = 0; i < 64; ++i) {
		cout << "*";
	}
	cout << endl;
	cout << "		  " << "|AES enc&dec|" << endl;
	char option;
	cout << "enter option, e to encrypt, d to dectrypt: " << endl;
	cin >> option;
	if (option == 'e') {
        te_init();
		enc();
	}
	else if (option == 'd') {
        td_init();
        key_box_init();
		dec();
	}
    cout << endl;
    for (int i = 0; i < 64; ++i) {
        cout << "*";
    }
    cout << endl;
    return 0;
}
