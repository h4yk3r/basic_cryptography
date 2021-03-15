#include <iostream>
typedef long long ll;

ll binpow(ll a, ll b) {
  ll res = 1;
  while (b > 0) {
    if (b & 1) { //检测最后一位是否为1，是1则为真
        res = res * a;
    }
    a = a * a;//幂次的幂次乘以2
    b = b >> 1;//舍弃最后一位
  }
  return res;
}

//拆分算法
void seperate (ll n, ll &k, ll &q)
{
    ll temp = n - 1;
    k = 0;
    q = 0;
    ll cnt = 0;
    while ((temp%2)==0) {
        temp /= 2;
        ++cnt;
    }
    q = temp;
    k = cnt;
    return;
}

//循环结构快速模幂算法
//输出：a的b次幂模c的结果res
ll modpow(ll a, ll b, ll c) {
    a %= c;
    ll res = 1;
    while (b > 0) {
        if (b & 1) { //检测最后一位是否为1，是1则为真
            res = (res * a)%c;
        }
        a = (a * a)%c;//幂次的幂次乘以2
        b = b >> 1;//舍弃最后一位
    }

    return res;
}