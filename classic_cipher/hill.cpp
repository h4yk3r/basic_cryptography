#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#define LEAP 97
using namespace ::std;

void exgcd (int a, int b, int &x, int &y, int &gcd)
{
    if (b==0) {
        x = 1;
        y = 0;
        gcd = a;
        return ;
    }
    //更换新的x和y
    exgcd(b, a % b, x, y, gcd);
    int temp = y;
    y = x - (a / b) * y;// 使用不完全商往里面回代
    x = temp;
    return;
}

int mul_inv (int a, int n)
{
    int x, y, gcd;
    exgcd(a, n, x, y, gcd);
    while (x<0) {
        x += n;
    }
    return x;//返回a模n的乘法逆元
}

void enc ()
{
    string plain;
    string cipher;
    int n = 0;
    cin >> n;
    vector<vector<int>> key;
    for (int i = 0; i<n; ++i) {
        vector<int> temp = {};
        key.push_back(temp);
    }
    int sig = 0;
    for (int i=0; i<(n*n); ++i) {
        int in = 0;
        cin >> in;
        key[sig].push_back(in);
        ++sig;
        sig %= n;
    }
        /*
    for (int i=0; i<n; ++i) {
        vector<int> temp;
        for (int j=0; j<n; ++j) {
            int in = 0;
            cin >> in;
            temp.push_back(in);
        }
        key.push_back(temp);
    }
    */
    cin >> plain;
    for (auto i = plain.begin(); i!=plain.end(); i+=n) {
        vector<int> temp = {};
        for (int j=0; j<n; ++j) {
            int pc = (*(i+j)) -LEAP;
            temp.push_back(pc);
        }
        for (auto j=key.begin(); j!=key.end(); ++j) {// 对每一列密钥
            int c = 0;
            for (int k=0; k<n; ++k) {//
                c += ( (temp[k])*((*j)[k]) );
            }
            c %= 26;
            char ch = c + LEAP;
            cipher.push_back(ch);
        }
    }

    cout << cipher << endl;
    return;
}

void dec ()
{
    string plain;
    string cipher;
    int n = 0;
    cin >> n;
    vector<vector<int>> key;
    for (int i = 0; i<n; ++i) {
        vector<int> temp = {};
        key.push_back(temp);
    }
    int sig = 0;
    for (int i=0; i<(n*n); ++i) {
        int in = 0;
        cin >> in;
        key[sig].push_back(in);
        ++sig;
        sig %= n;
    }// key finish
    vector<vector<int>> invkey;
    for (int i = 0; i<n; ++i) {
        vector<int> temp(n, 0);
        invkey.push_back(temp);
    }
    int hls = 0;
    int temp = 1;
    if (n==2) {
        hls = (key[0])[0] * (key[1])[1] - (key[0])[1] * (key[1])[0];
        hls %= 26;
        if (hls<0) {
            temp = -1;
        }
        hls = mul_inv(abs(hls), 26);
        (invkey[0])[0] = (key[1])[1]*temp;
        (invkey[0])[0] %= 26;
        (invkey[1])[0] = (-1)*(key[1])[0]*temp;
        (invkey[1])[0] %= 26;
        (invkey[0])[1] = (-1)*(key[0])[1]*temp;
        (invkey[0])[1] %= 26;
        (invkey[1])[1] = (key[0])[0]*temp;
        (invkey[1])[1] %= 26;
    } else if (n==3) {
        hls = (key[0])[0] * (key[1])[1] * (key[2])[2] + (key[1])[0] * (key[2])[1] * (key[0])[2] + (key[2])[0] * (key[0])[1] * (key[1])[2] - (key[2])[0] * (key[1])[1] * (key[0])[2] - (key[0])[0] * (key[2])[1] * (key[1])[2] - (key[1])[0] * (key[0])[1] * (key[2])[2];
        hls %= 26;
        if (hls<0) {
            temp = -1;
        }
        hls = mul_inv(abs(hls), 26);
        (invkey[0])[0] = temp*((key[1])[1] * (key[2])[2] - (key[1])[2] * (key[2])[1]);
        (invkey[0])[0] %= 26;
        (invkey[0])[1] = temp*((key[2])[1] * (key[0])[2] - (key[2])[2] * (key[0])[1]);
        (invkey[0])[1] %= 26;
        (invkey[0])[2] = temp*((key[0])[1] * (key[1])[2] - (key[0])[2] * (key[1])[1]);
        (invkey[0])[2] %= 26;
        (invkey[1])[0] = temp*(-1)*((key[1])[0] * (key[2])[2] - (key[1])[2] * (key[2])[0]);
        (invkey[1])[0] %= 26;
        (invkey[1])[1] = temp*(-1)*((key[2])[0] * (key[0])[2] - (key[2])[2] * (key[0])[0]);
        (invkey[1])[1] %= 26;
        (invkey[1])[2] = temp*(-1)*((key[0])[0] * (key[1])[2] - (key[0])[2] * (key[1])[0]);
        (invkey[1])[2] %= 26;
        (invkey[2])[0] = temp*((key[1])[0] * (key[2])[1] - (key[2])[0] * (key[1])[1]);
        (invkey[2])[0] %= 26;
        (invkey[2])[1] = temp*((key[2])[0] * (key[0])[1] - (key[0])[0] * (key[2])[1]);
        (invkey[2])[1] %= 26;
        (invkey[2])[2] = temp*((key[0])[0] * (key[1])[1] - (key[1])[2] * (key[0])[1]);
        (invkey[2])[2] %= 26;
    }

    cin >> cipher;
    for (auto i = cipher.begin(); i!=cipher.end(); i+=n) {
        vector<int> temp = {};
        for (int j=0; j<n; ++j) {
            int pc = (*(i+j)) -LEAP;
            temp.push_back(pc);
        }
        for (auto j=invkey.begin(); j!=invkey.end(); ++j) {// 对每一列密钥
            int c = 0;
            for (int k=0; k<n; ++k) {//
                c += ( (temp[k])*((*j)[k]) );
            }
            c += 520;
            c *= hls;
            c %= 26;
            char ch = c + LEAP;
            plain.push_back(ch);
        }
    }
    cout << plain << endl;
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
