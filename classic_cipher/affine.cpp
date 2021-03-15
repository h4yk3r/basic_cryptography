#include <iostream>
#include <string>
#include <cmath>
using namespace ::std;
#define LEAP 97

int inv (int a, int m)
{
    int x0 = 1, y0 = 1;
    int x1 = 0, y1 = 1;
    int q = a / m;
    int r0 = m;
    int r1 = a % m;
    int temp = 0;
    while (r1!=0) {
        temp = x1;
        x1 = x0 - q * x1;
        x0 = temp;
        temp = y1;
        y1 = y0 - q * y1;
        y0 = temp;
        q = r0 / r1;
        temp = r1;
        r1 = r0 % r1;
        r0 = temp;
    }
    return x1;
}

void enc ()
{
    int k, b, m;
    cin >> k >> b >> m;
    string p;
    cin >> p;
    string c;
    for (auto ch : p) {
        int i = ch;
        i -= LEAP;
        i *= k;
        i += b;
        i %= m;
        char cyc = i+LEAP;
        c.push_back(cyc);
    }
    cout << c << endl;
}

void dec ()
{
    int k, b, m;
    cin >> k >> b >> m;
    int ik = inv(k, m);
    string c;
    string p;
    cin >> c;
    for (auto cc : c) {
        int i = cc - LEAP;
        i += m;
        i -= b;
        i *= ik;
        i %= m;
        char pc = i + LEAP;
        p.push_back(pc);
    }
    cout << p << endl;
}

int main ()
{
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
