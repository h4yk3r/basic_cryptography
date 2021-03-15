#include <iostream>
#include <gmp.h>
#include <time.h>
using namespace ::std;

void exgcd (mpz_t a, mpz_t b, mpz_t &x, mpz_t &y, mpz_t &gcd)
{
    if (mpz_cmp_ui(b, 0)==0) {
        mpz_set_ui(x, 1);
        mpz_set_ui(y, 0);
        mpz_set(gcd, a);
        return ;
    }
    //更换新的x和y
    mpz_t r;
    mpz_init(r);
    mpz_mod(r, a, b);
    exgcd(b, r, x, y, gcd);
    /*
    long long temp = y;
    y = x - (a / b) * y;// 使用不完全商往里面回代
    x = temp;
    */
    mpz_t temp;
    mpz_init(temp);
    mpz_set(temp, y);
    mpz_t q;
    mpz_init(q);
    //cdiv是向正无穷取整，fdiv是向负无穷取整，tdiv是向0取整
    mpz_fdiv_q(q, a, b);
    mpz_mul(q,q,y);
    mpz_sub(y, x, q);
    mpz_set(x, temp);

    return;
}

int main ()
{
    clock_t startTime, endTime;
    mpz_t a, b, x, y, gcd;
    mpz_init(a);
    mpz_init(b);
    mpz_init(x);
    mpz_init(y);
    mpz_init(gcd);

    gmp_scanf("%Zd %Zd", &a, &b);
    //cin >> a >> b;
    startTime = clock();
    exgcd(a, b, x, y, gcd);
    //cout << "the result is " << a << "*" << x << "+" << b << "*" << y << "=gcd(" << a << "," << b << ")=" << gcd ;
    gmp_printf("the result is %Zd*%Zd+%Zd*%Zd=gcd(a,b)=%Zd\n", a,x,b,y,gcd);
    endTime = clock();
    cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    system("pause");

    return 0;
}