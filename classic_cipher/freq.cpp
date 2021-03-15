#include <iostream>
#include <string>
#include <vector>
#define LEAP 97
using namespace ::std;
int alp[26];

void read ()
{
    string cipher;
    cin >> cipher;
    for (auto i : cipher) {
        int temp = i-LEAP;
        ++alp[temp];
    }
}

int findmax ()
{
    int max = 0;
    for (int i=0; i<26; ++i) {
        if (alp[i]>=alp[max]) {
            max = i;
        }
    }
    return max;
}

int main ()
{
    read();
    int bp = findmax();
    bp+=22;
    bp %= 26;
    int key = bp;
    cout << key << endl;

    return 0;
}
