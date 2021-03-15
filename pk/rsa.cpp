#include "rsa.h"

void menu ()
{
    cout << endl << "RSA menu" << endl;
    cout << "e: enc" << endl;
    cout << "d: dec" << endl;
    cout << "q: quit" << endl;
    return ;
}

void seperate (mpz_t n, mpz_t &k, mpz_t &q)
{
    mpz_set_ui(k, 0);
    mpz_set(q, n);
    mpz_sub_ui(q, q, 1);
    mpz_t r;
    mpz_init(r);
    mpz_mod_ui(r, q, 2);
    while (mpz_cmp_ui(r, 0)==0) {
        mpz_fdiv_q_ui(q, q, 2);
        mpz_add_ui(k, k, 1);
        mpz_mod_ui(r, q, 2);
    }
    return;
}

bool seccheck (mpz_t a, mpz_t n, mpz_t k, mpz_t q)
{
    mpz_t s;
    mpz_init_set(s, n);
    mpz_sub_ui(s, s, 1);
    mpz_t j;
    mpz_init(j);
    mpz_t p;
    mpz_init_set(p, q);
    mpz_t r;
    mpz_init(r);
    for (mpz_set_ui(j, 0); mpz_cmp(j,k)<0; mpz_add_ui(j, j, 1)) {
        mpz_powm(r, a, p, n);
        mpz_mul_ui(p, p, 2);
        if (mpz_cmp(r,s)==0) {
            return true;
        }
    }
    return false;
}

bool mr_isPrime (mpz_t n)
{
    mpz_t k;
    mpz_t q;
    mpz_init(k);
    mpz_init(q);
    seperate(n, k, q);

    mpz_t a;
    mpz_init_set(a, n);
    mpz_sub_ui(a, a, 1);
    mpz_fdiv_q_ui(a, a, 2);

    mpz_t temp;
    mpz_init(temp);
    mpz_powm(temp, a, q, n);

    bool isPrime = false;
    if (mpz_cmp_ui(temp, 1)==0) {
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

//得到512位的二进制随机数
//然后使用string的方式赋值给p, q
//在main函数当中进行素性检测
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

void easygcd (mpz_t a, mpz_t b, mpz_t &r)
{
    if (mpz_cmp_ui(b, 0)==0) {
        mpz_set(r, a);
        return;
    }
    mpz_t tr;
    mpz_init(tr);
    mpz_mod(tr, a, b);// tr = a mod b
    easygcd(b, tr, r);
    return ;
}

bool rp_jud (mpz_t a, mpz_t b)
{
    mpz_t r;
    mpz_init(r);
    easygcd(a, b, r);
    if (mpz_cmp_ui(r, 1)==0) {
        return true;//互素为真
    } else {
        return false;//不互素为假
    }
}

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

void mul_inv (mpz_t a, mpz_t n, mpz_t &inverse)
{
    mpz_t x, y, gcd;
    mpz_init(x);
    mpz_init(y);
    mpz_init(gcd);
    exgcd(a, n, x, y, gcd);
    while (mpz_cmp_ui(x, 0)<0) {
        mpz_add(x, x, n);
    }
    mpz_set(inverse, x);
    return ;//返回a模n的乘法逆元
}

//res = a^b mod c
void modpow(mpz_t a, mpz_t b, mpz_t c, mpz_t &res) {
  mpz_t loga, power;
  mpz_init_set(loga, a);
  mpz_init_set(power, b);

  mpz_set_ui(res, 1);
  while (mpz_cmp_ui(power, 0)>0) {
    //检测最后一位是否为1，是1则为真
    mpz_t temp;
    mpz_init(temp);
    mpz_mod_ui(temp, power, 2);
    if (mpz_cmp_ui(temp, 1)==0) { //使用奇偶判断来代替,如果是奇数则真
        mpz_mul(res, res, loga);
        mpz_mod(res, res, c);
        //res = (res * loga)%c;
    }
    mpz_mul(loga, loga, loga);
    mpz_mod(loga, loga, c);
    //loga = (loga * loga)%c;
    //幂次的幂次乘以2
    //power = power >> 1;
    mpz_fdiv_q_ui(power, power, 2);
    //使用除以2的方法代替位运算符舍弃最后一位
  }
  return ;
}
