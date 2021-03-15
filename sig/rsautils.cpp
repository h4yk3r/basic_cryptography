#include "rsautils.h"

//接受一个seed, 得到cnt(-1)位的随机数，以string返回
string bbs_prng (unsigned long long s, unsigned int cnt)
{
    using ull = unsigned long long;
    using ui = unsigned int;
    ull p = 383, q = 503;

    //ull n = p*q;
    ull n = 192649;

    ull xp, xs, ss, b;
    ss = s*s;
    xs = ss%n;

    string keystring = {};

    for (ui i=0; i!=cnt; ++i) {
        xp = xs*xs;
        xp %= n;
        b  = xp%2;
        keystring.push_back(b+'0');
        //gmp_printf("%Zd", b);
        xs = xp;
    }

    return keystring;
}

//快速模幂, res = a^b mod c
void modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res)
{
    mpz_class loga, power;
    loga = a; power = b; res = 1;
    while (power > 0) {
        mpz_class temp;
        temp = power % 2;
        if (temp==1) {
            res = res * loga;
            res = res % c;
        }
        loga = loga * loga;
        loga = loga % c;
        power = power / 2;
    }
    return ;
}

void seperate (mpz_class n, mpz_class &k, mpz_class &q)
{
    k=0; q=n-1;
    mpz_class r;
    r = q % 2;
    while (r==0) {
        q = q / 2;
        k = k+1;
        r = q%2;
    }
    return ;
}

bool seccheck (mpz_class a, mpz_class n, mpz_class k, mpz_class q)
{
    mpz_class s;
    s = n-1;
    mpz_class j, p, r;
    p = q;
    for (j=0; j<k; j=j+1) {
        modpow(a, p, n, r);
        p = p*2;
        if (r==s) {
            return true;
        }
    }
    return false;
}

//素性测试
bool mr_isPrime (mpz_class n)
{
    mpz_class k ,q;
    seperate(n, k, q);

    mpz_class a;
    a=n-1;
    a = a/2;

    mpz_class temp;
    modpow(a, q, n, temp);

    bool isPrime = false;
    if (temp == 1) {
        isPrime = true;
    } else {
        //seccheck
        isPrime = seccheck(a, n, k, q);
    }
    if (isPrime) {
        return true;
    } else {
        return false;
    }
}

//gcd(a, b)
void easygcd (mpz_class a, mpz_class b, mpz_class &r)
{
    if (b==0) {
        r=a;
        return ;
    }
    mpz_class tr;
    tr = a % b;
    easygcd(b, tr, r);
    return ;
}

//if (a, b) = 1, =1 return true
bool rp_jud (mpz_class a, mpz_class b)
{
    mpz_class r;
    easygcd(a, b, r);
    if (r==1) {
        return true;
    } else {
        return false;
    }
}

//互素的a和b，得到ax+by=gcd的形式
void exgcd (mpz_class a, mpz_class b, mpz_class &x, mpz_class &y, mpz_class &gcd)
{
    /*
    mpz_class ta, tb;
    ta = a;
    tb = b;
    */
    if (b==0) {
        x=1;y=0;gcd=a;
        return ;
    }

    mpz_class r, temp, q;
    r = a % b;
    exgcd(b, r, x, y, gcd);
    temp = y;
    q = (a/b)*y;
    y = x-q;
    x = temp;

    return ;
}

//求a模n的乘法逆元（a, n互素的情况下）
void mul_inv (mpz_class a, mpz_class n, mpz_class &inv)
{
    mpz_class x, y, gcd;
    x=y=gcd=0;
    exgcd(a, n, x, y, gcd);
    while (x<0) {
        x = x+n;
    }
    inv = x;
    return ;
}

//获取二进制位数
int get_len (mpz_class n)
{
    mpz_class res;
    int cnt = 1;
    res = n;
    while (1) {
        if (res<2) {
            return cnt;
        } else {
            res = res / 2;
            ++cnt;
        }
    }
    return 0;
}
