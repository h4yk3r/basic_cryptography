#include <iostream>
using namespace ::std;

//在快速幂算法的基础上加入取模运算，因为取模不会改变
long long modpow(long long a, long long b, long long c) {
  long long res = 1;
  while (b > 0) {
    if (b & 1) { //检测最后一位是否为1，是1则为真
        res = (res * a)%c;
    }
    a = (a * a)%c;//幂次的幂次乘以2
    b = b >> 1;//舍弃最后一位
  }
  return res;
}
//如果是一个素数，可以用费马小定理加速算法（先判定素数再进行计算
//素性检测的复杂度也要计算在内
int main ()
{
    long long x, n, m, ret;
    cin >> x >> n >> m;
    x = x % m;

    ret = modpow(x, n, m);
    cout << ret;

    return 0;
}