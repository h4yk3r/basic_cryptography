#include <iostream>
#include "alprime.h"
using namespace ::std;
typedef long long ll;

bool seccheck (ll a, ll n, ll k, ll q)
{
    for (ll j = 0; j < k; ++j) {
        ll temp = binpow(2, j);
        temp *= q;
        if (modpow(a, temp, n)==(n-1)) {
            return true;
        }
    }
    return false;
}

int main ()
{
    ll n = 0;
    cout << "enter the number waiting for tset: ";
    cin >> n;

    ll k = 0, q = 0;
    seperate(n, k, q);
    bool isPrime = false;

    //?????a
    ll a = (n - 1) / 2;
    //ll a = 2;
    
    if (modpow(a, q, n)==1) {
        //cout << "is a prime";
        isPrime = true;
    } else {
        if (seccheck(a, n, k ,q)) {
            //cout << "is a prime";
            isPrime = true;
        } 
    }
    
    if (isPrime) {
        cout << "is a prime";
    } else {
        cout << "is not";
    }

    system("pause");

    return 0;
}