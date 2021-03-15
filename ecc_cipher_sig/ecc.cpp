#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <gmpxx.h>

using namespace ::std;

mpz_class p;
int a, b;
mpz_class gx, gy;
mpz_class og;
mpz_class k;

//a curve of y^2=x^3+ax+b(mod p)
class elliptic {
private:
    mpz_class p; int a, b;//curve

    //快速模幂, res = a^b mod c
    void modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res);

public:
    //constructor
    elliptic() = default;
    elliptic(mpz_class pin, int ain, int bin);

    void equallamda(mpz_class xp, mpz_class yp,
                    mpz_class xq, mpz_class yq, mpz_class &lamda);
    void unequallamda(mpz_class xp, mpz_class yp,
                    mpz_class xq, mpz_class yq, mpz_class &lamda);
    void eccplus(mpz_class xp, mpz_class yp,
                mpz_class xq, mpz_class yq, mpz_class &rx, mpz_class &ry);
    void eccmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry);
    bool econcurve(mpz_class x, mpz_class y);
};

elliptic::elliptic (mpz_class pin, int ain, int bin)
{
    p = pin;
    a = ain;
    b = bin;
}

void elliptic::equallamda(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &lamda)
{
    //up/down(mod p) = up*down^(p-2)(mod p)
    mpz_class nume;
    mpz_class deno;

    nume = 3*xp*xp+a;
    deno = 2*yp;

    mpz_class power;
    power = p-2;
    mpz_class invdeno;
    modpow(deno, power, p, invdeno);

    nume = nume % p;
    lamda = nume*invdeno;
    lamda = lamda % p;

    return ;
}

void elliptic::unequallamda(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &lamda)
{
    //up/down(mod p) = up*down^(p-2)(mod p)
    mpz_class nume;
    mpz_class deno;

    nume = yq - yp;
    deno = xq - xp;

    mpz_class power;
    power = p-2;
    mpz_class invdeno;
    modpow(deno, power, p, invdeno);

    nume = nume % p;
    lamda = nume*invdeno;
    lamda = lamda % p;

    return ;
}

void elliptic::eccplus(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &rx, mpz_class &ry)
{
    mpz_class lamda;
    if ( (xp==xq) && (yp==yq) ) {
        equallamda(xp, yp, xq, yq, lamda);
    } else {
        unequallamda(xp, yp, xq, yq, lamda);
    }

    mpz_class xtemp, ytemp;
    xtemp = lamda*lamda - xp - xq;
    while (xtemp<0) {
        xtemp = xtemp + p;
    }
    xtemp = xtemp % p;
    ytemp = lamda*xp - lamda*xtemp -yp;
    while (ytemp<0) {
        ytemp = ytemp + p;
    }
    ytemp = ytemp % p;

    rx = xtemp;
    while (rx<0) {
        rx = rx + p;
    }
    ry = ytemp;
    while (ry<0) {
        ry = ry + p;
    }

    return ;
}

void elliptic::eccmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry)
{
    mpz_class xtemp, ytemp;
    xtemp = x;
    ytemp = y;
    mpz_class i;
    for (i=1; i!=n; i=i+1) {
        eccplus(xtemp, ytemp, x, y, xtemp, ytemp);
    }
    rx = xtemp;
    while (rx<0) {
        rx = rx + p;
    }
    ry = ytemp;
    while (ry<0) {
        ry = ry + p;
    }

    return ;
}

//y^2=x^3+ax+b(mod p)
bool elliptic::econcurve(mpz_class x, mpz_class y)
{
    mpz_class l,r;
    l = (y*y) % p;
    r = (x*x*x + a*x + b) % p;
    if (l==r) {
        return true;
    } else if (l!=r) {
        return false;
    }
    return true;
}

//快速模幂, res = a^b mod c
void elliptic::modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res)
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

void enc ()
{
    elliptic curve = elliptic(p, a, b);
    //产生k
    cout << "select sk:" << endl;
    cin >> k;
    //k = 101;

    //得到m
    mpz_class mx, my;
    cout << "select M:" << endl;
    cin >> mx >> my;

    mpz_class kx, ky;
    curve.eccmul(k, gx, gy, kx, ky);

    mpz_class x1,y1,x2,y2;
    mpz_class xtemp, ytemp;

    mpz_class r;
    do {
        srand(int(time(0)*10));
        do {
            unsigned int ran = rand();
            r = ran; r = r % p;
        } while (r==0);
        curve.eccmul(r, gx, gy, x2, y2);
    } while (!curve.econcurve(x2, y2));
    //r = 41;

    curve.eccmul(r, kx, ky, xtemp, ytemp);
    curve.eccplus(mx, my, xtemp, ytemp, x1, y1);

    cout << "the cipher(C1, C2):" << endl;
    cout << "( (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ") )" << endl;

    return ;
}

void dec ()
{
    mpz_class x1;
    mpz_class y1;
    mpz_class x2;
    mpz_class y2;

    cout << "enter the cipher(C1, C2):" << endl;
    cin >> x1 >> y1 >> x2 >> y2;

    mpz_class xtemp, ytemp;
    elliptic curve = elliptic(p, a, b);
    curve.eccmul(k, x2, y2, xtemp, ytemp);
    ytemp = -ytemp;

    mpz_class rx, ry;
    curve.eccplus(x1, y1, xtemp, ytemp, rx, ry);
    cout << "m: ";
    cout << "(" << rx << ", " << ry << ")" << endl;

    return ;
}

int main ()
{
    cout << "initializing..." << endl;
    cout << "select the base point G:" << endl;
    cin >> gx >> gy;
    cout << "select the curve:" << endl;
    cin >> a >> b >> p;
    cout << endl;

    char op;
    while (1) {
        cout << "e to enc, d to dec, q to quit:" << endl;
        cin >> op;
        if (op=='d') {
            dec();
            continue;
        } else if (op=='e') {
            enc();
            continue;
        } else if (op=='q') {
            break;
        }
    }

    /*test ec
    mpz_class  xp, yp, xq, yq;
    cin >> xp >> yp >> xq >> yq;
    cin >> p >> a >> b;
    elliptic curve = elliptic(p, a, b);
    mpz_class xtemp, ytemp;
    curve.eccplus(xp, yp, xq, yq, xtemp, ytemp);
    cout << xtemp << " " << ytemp << endl;
    curve.eccmul(2, xp ,yp, xtemp, ytemp);
    cout << xtemp << " " << ytemp << endl;
    */

    return 0;
}
