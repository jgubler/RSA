#include "keygen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
        x = rand();
        //printf("x is %d\n", x);
        if(testprime(x)!=0) x=0;
    }
    return x;
}

// does several tests to approximate whether p is probably prime
// (1 and 2 are excluded primes)
int testprime(int potPrime) {
    if(potPrime == 1 || potPrime%2 == 0) return 1;
    if(fermat(potPrime, 15) != 0) return 1;
    return 0;
}


// tests if potPrime is prime according to fermat's theorem, which 
// states that if b is a prime number and a is any number for which
// gcd(a,b) = 1 holds, a^(b-1) mod b = 1 must be true.
// this function tests the property as many times as precision specifies
int fermat(int potPrime, int precision) {
    long long res = 1;
    int coprime = 0;
    for(int i=0; i<precision; i++) {
        while(coprime == 0) {
            coprime = rand();
            if(gcd(potPrime, coprime) != 1) {
                coprime = 0;
            }            
        }
        if(modularExp(coprime, potPrime-1, potPrime) != 1) 
            return 1;
    }
    return 0;
}

// Implements a^b mod n, with fast exponentiation through square&multiply
int modularExp(int a, int b, int n) {
    // log2(b)
    int explog = log(b)/log(2);
    long long res = a;
    int currentBit = 0;
    // traverse the bits of b, square for every bit and if it is 1, multiply as well
    for(int i=explog-1; i>=0; i--) {
        res *= res;
        // to get the bit for the current SAM cycle, we need to right shift by i and extract the last bit
        currentBit = (b>>i) % 2;
        if(currentBit==1) res *=a;
        res %= n;
    }
    return res;
}

// Approximates whether x is a prime by calculating the miller-rabin test
// TODO: Actually make it do something
int miller(int potPrime) {
    return 0;
}

// Compute gcd with Euclidean algorithm
int gcd (int pa, int pb) {
    int a = pa, b = pb, c = 0;
    while(a != 0) {
        c = a;
        a = b%a;
        b = c;
    }
    return b;
}
