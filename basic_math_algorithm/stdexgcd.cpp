#include <iostream>
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

int main ()
{
    int a, b, x, y, gcd;

    cin >> a >> b;

    exgcd(a, b, x, y, gcd);

    cout << "the result is " << a << "*" << x << "+" << b << "*" << y << "=gcd("
         << a << "," << b << ")=" << gcd ;

    return 0;
}