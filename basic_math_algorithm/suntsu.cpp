#include <iostream>
//#include <vector>
#include "minv.h"
using namespace ::std;
typedef long long ll;

int main()
{
    cout << "enter every single a and n: (x=a mod n)" << endl;
    ll a[3], n[3], c[3];
    for (int i = 0; i < 3; ++i) {
        cin >> a[i] >> n[i];
    }
    ll M = 1;
    ll m[3];
    ll im[3];
    for (int i = 0; i < 3; ++i) {
        M *= n[i];
    }
    for (int i = 0; i < 3; ++i) {
        m[i] = M / n[i];
    }
    for (int i = 0; i < 3; ++i) {
        im[i] = mul_inv(m[i], n[i]);
    }
    for (int i = 0; i < 3; ++i) {
        c[i] = m[i] * im[i];
    }
    ll ret = 0;
    for (int i = 0; i < 3; ++i) {
        int temp = a[i] * c[i];
        ret += temp;
    }

    cout << ret << "(mod " << M << ")" << endl;
    system("pause");

    return 0;
}