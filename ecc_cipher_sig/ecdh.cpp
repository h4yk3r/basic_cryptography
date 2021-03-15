#include <iostream>
#include <string>
#include <gmpxx.h>

using namespace ::std;

mpz_class p;
int a, b;
mpz_class gx, gy;
mpz_class na, nb;

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

void dec (mpz_class &rx1, mpz_class &ry1, mpz_class &rx2, mpz_class &ry2)
{
    elliptic curve = elliptic(p, a, b);
    mpz_class k;

    mpz_class mx, my;
    mpz_class gx, gy;
    mpz_class kx, ky;
    curve.eccmul(k, gx, gy, kx, ky);

    mpz_class x1,y1,x2,y2;
    mpz_class xtemp, ytemp;

    mpz_class r;
    curve.eccmul(r, gx, gy, x2, y2);
    curve.eccmul(r, kx, ky, xtemp, ytemp);
    curve.eccplus(mx, my, xtemp, ytemp, x1, y1);

    rx1 = x1; ry1 = y1;
    rx2 = x2; ry2 = y2;

    return ;
}

void enc (mpz_class x1, mpz_class y1, mpz_class x2, mpz_class y2, mpz_class &mx, mpz_class &my)
{
    mpz_class k;
    mpz_class xtemp, ytemp;
    elliptic curve = elliptic(p, a, b);
    curve.eccmul(k, x2, y2, xtemp, ytemp);
    ytemp = -ytemp;
    mpz_class rx, ry;
    curve.eccplus(x1, y1, xtemp, ytemp, rx, ry);
    mx = xtemp; my = ytemp;

    return ;
}

void init_a ()
{
    elliptic curvea = elliptic(p, a, b);
    /*
    srand(int(time(0)));
    do {
        unsigned int r = rand();
        na = r; na = na % p;
    } while (na==0);
    */
    cout << "select na:" << endl;
    cin >> na;
    mpz_class pax, pay;
    curvea.eccmul(na, gx, gy, pax, pay);

    cout << "Pa: (" << pax << ", " << pay << ")" << endl;
    return ;
}

void init_b ()
{
    elliptic curveb = elliptic(p, a, b);
    /*
    srand(int(time(0)));
    do {
        unsigned int r = rand();
        nb = r; nb = nb % p;
    } while (nb==0);
    */
    cout << "select na:" << endl;
    cin >> na;
    mpz_class pbx, pby;
    curveb.eccmul(nb, gx, gy, pbx, pby);

    cout << "Pb: (" << pbx << ", " << pby << ")" << endl;
    return ;
}

void getKa ()
{
    mpz_class kx, ky;
    mpz_class pbx, pby;
    cout << "enter pk of B" << endl;
    cin >> pbx >> pby;
    elliptic curvea = elliptic(p, a, b);
    curvea.eccmul(na, pbx, pby, kx, ky);
    cout << "K: (" << kx << ", " << ky << ")" << endl;
    return ;
}

void getKb ()
{
    mpz_class kx, ky;
    mpz_class pax, pay;
    cout << "enter pk of A" << endl;
    cin >> pax >> pay;
    elliptic curvea = elliptic(p, a, b);
    curvea.eccmul(na, pax, pay, kx, ky);
    cout << "K: (" << kx << ", " << ky << ")" << endl;
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
    int flaga=0, flagb=0;
    while (1) {
        cout << "choose to  initialize:" << endl;
        cin >> op;
        if (op=='a') {
            if (flaga==1) {
                cout << "already initialized" << endl;
                continue;
            }
            init_a();
            flaga=1;
            continue;
        } else if (op=='b') {
            if (flagb==1) {
                cout << "already initialized" << endl;
                continue;
            }
            init_b();
            flagb=1;
            continue;
        } else if (op=='q') {
            break;
        }
    }

    getKa();
    getKb();

    return 0;
}
