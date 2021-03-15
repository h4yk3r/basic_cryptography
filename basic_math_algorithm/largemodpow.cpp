#include <iostream>
#include <time.h>
#include <gmp.h>
using namespace ::std;

//在快速幂算法的基础上加入取模运算，因为取模不会改变
void modpow(mpz_t a, mpz_t b, mpz_t c, mpz_t &res) {
  //mpz_t res = 1;
  while (mpz_cmp_ui(b, 0)>0) {
    //检测最后一位是否为1，是1则为真
    mpz_t temp;
    mpz_init(temp);
    mpz_mod_ui(temp, b, 2);
    if (mpz_cmp_ui(temp, 1)==0) { //使用奇偶判断来代替,如果是奇数则真
        mpz_mul(res, res, a);
        mpz_mod(res, res, c);
        //res = (res * a)%c;
    }
    mpz_mul(a, a, a);
    mpz_mod(a, a, c);
    //a = (a * a)%c;
    //幂次的幂次乘以2
    //b = b >> 1;
    mpz_fdiv_q_ui(b, b, 2);
    //使用除以2的方法代替位运算符舍弃最后一位
  }
  return ;
}
//如果是一个素数，可以用费马小定理加速算法（先判定素数再进行计算
//素性检测的复杂度也要计算在内
int main ()
{
    /*
    long long x, n, m, ret;
    cin >> x >> n >> m;
    x = x % m;
    */
    clock_t startTime, endTime;
    mpz_t x, n, m, ret;
    mpz_init(x);
    mpz_init(n);
    mpz_init(m);
    gmp_scanf("%Zd %Zd %Zd", &x, &n, &m);
    startTime = clock();
    mpz_init(ret);
    mpz_set_ui(ret, 1);
    /*
    ret = modpow(x, n, m);
    cout << ret;
    */
    modpow(x, n, m, ret);
    gmp_printf("the result is %Zd\n.", ret);
    endTime = clock();
    cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    system("pause");
    return 0;
}