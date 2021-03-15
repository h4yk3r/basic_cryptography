//#include "minv.h"
#include <iostream>
//函数的实现
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
