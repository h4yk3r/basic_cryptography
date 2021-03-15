#include <iostream>
using namespace ::std;

long long easygcd (long long a, long long b)
{
    if (b==0) {
        return a;
    }
    return (b, a % b);
}

int main ()
{
    long long a,b;
    cin >> a, b;
    long long gcd = 0;
    gcd = easygcd(a, b);

    return 0;
}