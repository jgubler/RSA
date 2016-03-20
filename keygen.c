#include "keygen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//keygen.c
//Will generate a RSA keypair, sometime in the distant future at least
//Right now, it only generates two (probable) primes and does nothing with them
int main() {
    srand(time(NULL));
    int p = chooseprime();
    int q = chooseprime();
    while(p==q) q = chooseprime();
    long long n = ((long long)p) * ((long long)q);
    printf("p is %d\nq is %d\nn is %lld\n", p, q, n);
    return 0;
}

// chooses a prime between 0 and 9,999,999
int chooseprime() {
    int x=0;
    while(x==0) {
        x = rand()%10000000;
        //printf("x is %d\n", x);
        if(testprime(x)!=0) x=0;
    }
    return x;
}

// does several tests to approximate whether p is probably prime
int testprime(int p) {
    if(p==1 || p%2==0) return 1;
    if(fermat(p)!=0) return 1;
    return 0;
}


// tests if a number x is prime according to fermat's theorem, which 
// states that if x is a prime number and a is any number for which
// gcd(a,b) = 1 holds, a^(b-1) mod b = 1 must be true.
// this function tests the property for the bases {3,5,7,11,13}
// TODO: Optimize exponentiation
int fermat(int b) {
    int bases[5] = {3,5,7,11,13};
    long long res = 1;
    for(int i=0;i<sizeof(bases)/sizeof(bases[0]);i++) {
        for(int j=0;j<b-1 && b!=bases[i];j++) {
            res *= bases[i];
            res = res%b;
        }
        if(res!=1) return 1;
    }
    return 0;
}

// Approximates whether x is a prime by calculating the miller-rabin test
// TODO: Actually make it do something
int miller(int x) {
    return 0;
}
