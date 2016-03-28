#include <keygen.h>
#include <asn1_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <INTEGER.h>
#include <PubKey.h>


//keygen.c
//Will generate a RSA keypair, sometime in the distant future at least
//Right now, it only generates two (probable) primes and does nothing with them
int main() {
    srand(time(NULL));
    // Choose p and q as 16-Bit pseudo random numbers (WARNING: rand() is no CSPRNG)
    long p = (long) chooseprime();
    long q = (long) chooseprime();
    while(p==q) {
        q = chooseprime();
    }
    long n = p * q;
    long phi = (p-1)*(q-1);
    // RSA: Choose e as a any number < phi with gcd(e,phi)==1 
    // for fast encryption we will choose e<1000, as this does not reduce security
    long e, d;
    do {
        e = (rand() % 1000) % phi;
        d = modularInverse(e,phi);
    } while (e == 1 || e == p-1 || e == q-1 || d == -1);
    //printf("n is %ld\ne is %ld\nd is %ld\nphi is %ld\np is %ld\nq is %ld\n", n, e, d, phi, p, q);
    int success = writeKeyFiles(n, e, d, p, q);
    if(success != 0) {
        fprintf(stderr, "ERROR: error writing key files");
        return 1;
    }
    return 0;
}

// chooses a prime between 0 and max (0 equals SIZE_INT)
// be aware that introducing a max will make the distribution less random, 
// unless max is a factor of SIZE_INT
int chooseprime() {
    int potPrime = 0;
    while(potPrime == 0) {
        potPrime = rand();
        //printf("potPrime is %d\n", potPrime);
        if(testprime(potPrime)!=0) { 
            potPrime=0;
        }
    }
    return potPrime;
}

// does several tests to approximate whether p is probably prime
// (1 and 2 are excluded primes), returns 0 for primes, 1 for composites
int testprime(int potPrime) {
    if(potPrime < 3 || potPrime%2 == 0 || fermat(potPrime, 15) != 0) {
        return 1;
    }
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
        if(modularExp(coprime, potPrime-1, potPrime) != 1) {
            return 1;
        }
    }
    return 0;
}

// Implements a^b mod n, with fast exponentiation through square&multiply
int modularExp (int a, int b, int n) {
    // log2(b) = ln(b)/ln(2)
    int explog = log(b)/log(2);
    long long res = a;
    int currentBit = 0;
    // traverse the bits of b, square for every bit and if it is 1, multiply as well
    for(int i=explog-1; i>=0; i--) {
        res = (res * res) % n;
        // to get the bit for the current SAM cycle, we need to right shift by i and extract the last bit
        currentBit = (b>>i) % 2;
        if(currentBit==1) {
            res = (res * a) % n;
        }
    }
    return res;
}

// Approximates whether x is a prime by calculating the miller-rabin test
// TODO: Actually make it do something
int miller (int potPrime) {
    return 0;
}

// Compute gcd with Euclidean algorithm
long gcd (long a, long b) {
    long c = 0;
    while(a != 0) {
        c = a;
        a = b%a;
        b = c;
    }
    return b;
}

// Compute the modular inverse a^(-1) mod m with the Extended Euclidean Algorithm
// Returns the modular inverse (shifted into Zn, no negative results) or -1 if gcd(a,n) != 1
long modularInverse (long a, long m) {
    int step = 2;
    // Don't know how many steps gcd will take yet, so I just ballparked 100 as a max
    // TODO: Fix this crap
    long down[1000];
    down[0] = m;
    down[1] = a;
    // This loop is basically the ordinary Euclidean Algorithm, doing the gcd calculation and
    // saving all intermediate results in 'down' as this part is going 'downwards' in the table
    while(down[step-1] != 0) {
        down[step] = down[step-2] % down[step-1];
        step++;
    }

    // Sanity check, making sure down[step-2] (= gcd(a,m)) = 1
    if(down[step-2] != 1) {
        return -1L;
    }

    // Following now is the extended part of the EEA, computing the inverse 'upwards'
    long up[step-1];
    up[step-2] = 0;
    up[step-3] = 1;

    for(int i=step-4; i>=0; i--) {
        up[i] = up[i+2] - up[i+1] * (down[i] / down[i+1]);
    }
    
    long res = up[0];
    // if the inverse is negative, convert it into Zn by adding m
    if(res < 0) {
        res += m;
    }

    return res;
}

int writeKeyFiles(long n, long e, long d, long p, long q) {
    printf("the modulus is %ld\ne is %ld\n", n, e);

    PubKey_t *pubKey = calloc(1, sizeof(PubKey_t));
    INTEGER_t *mod = calloc(1, sizeof(INTEGER_t));
    INTEGER_t *pubExp = calloc(1, sizeof(INTEGER_t));
    if(!(pubKey && mod && pubExp)) {
        perror("calloc() failed");
        exit(1);
    }

    asn_long2INTEGER(mod, n);
    asn_long2INTEGER(pubExp, e);

    pubKey->modulus = *mod;
    pubKey->publicExponent = *pubExp;

    pubkey_encode("pub.key", pubKey);

    FILE *pr;
    pr = fopen("key.pr", "w");    
    if(pr != NULL) {
        fprintf(pr, "-----RSA PRIVATE KEY-----\nn=%ld\ne=%ld\nd=%ld\np=%ld\nq=%ld\n---------END KEY---------\n", n, e, d, p, q);
        fclose(pr);
    } else {
        return 1;
    }

    return 0;
}
