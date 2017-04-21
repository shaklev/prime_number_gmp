/*! Generate a 1024 bits prime using GMP library

HOW TO COMPILE : g++ gmp_is_prime.cpp -lgmp -o test_gmp
HOW TO RUN: ./test_gmp

 */

// explanation of the mpz_probab_prime_p https://gmplib.org/manual/Prime-Testing-Algorithm.html
#include <stdio.h>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <typeinfo>
using namespace std;


/* This function calculates (ab)%c */
int modulo(mpz_t a,mpz_t b,mpz_t c){
		mpz_t x,y;
		mpz_init_set_str (x, "1", 10);
		mpz_init_set(y,a);
		while(mpz_sgn(b))
			{
				if(mpz_odd_p(b))
				{
							mpz_mul(x,x,y);
							mpz_mod(x,x,c);
				}
				mpz_mul(y,y,y);
				mpz_mod(y,y,c);
				mpz_tdiv_q_ui(b,b,2);
			}
		mpz_mod(x,x,c);
		return mpz_get_si(x);
    }

/* Fermat's test for checking primality, the more iterations the more is accuracy */
bool Fermat(mpz_t p, int iterations){
		mpz_t one,a,p_minus_one;
		mpz_init_set_str (one, "1", 10);
		mpz_init_set_str (a, "0", 10);
		mpz_init_set_str (p_minus_one, "0", 10);
    if(mpz_cmp(p,one)==0){ // 1 isn't prime
        return false;
    }
		// mpz_urandomb(p, rstate, 1024);
    for(int i=0;i<iterations;i++){
				mpz_sub(p_minus_one,p,one);
				gmp_randstate_t rstate;
				gmp_randinit_default (rstate);
        // choose a random integer between 1 and p-1 ( inclusive )
				mpz_urandomm(a, rstate, p_minus_one);
				if (modulo(a,p_minus_one,p) != 1)
					{
						return false;
					}
		}
    return true; /* p is probably prime */
}

int main()
{
	int choice,iterations;
	bool isPrime;
	mpz_t bign;
	// mpz_init(bign);
	FILE *fp;
	fp = fopen("big_number.txt","r");
	mpz_inp_str(bign,fp,10);
	cout<<"\n********************************************"<<endl;
	cout<<"\nThe number that's being tested for primality: ";
	gmp_printf("%Zd\n\n",bign);
	cout<<"********************************************\n"<<endl;
	cout<<"Enter the number of iterations that you want to be used: ";
	cin>>iterations;
	cout << "Which algorith do you want to use to test-primality of a number?"<<endl;
	cout<<"----------------------------------"<<endl;
	cout<<"1) Fermat primality-test"<<endl;
	cout<<"2) Miller-Rabin primality-test"<<endl;
	cout<<"----------------------------------"<<endl;
	cout<<"Your choice:";
	cin >> choice;
	switch(choice) {
	case 1:
	    isPrime = Fermat(bign, iterations);
	    break;
	case 2:
	    isPrime = mpz_probab_prime_p(bign, iterations);
	    break;
	default:
	    cout<<"You didn't input correct number, so I chose Miller-Rabin's algorithm"<<endl;
			isPrime = mpz_probab_prime_p(bign, iterations);
	  }
	if (isPrime==2)
	  {
	    gmp_printf("The number you have chosen ( %Zd ) is prime.\n",bign);
	  }
	else if (isPrime==1)
	  {
			gmp_printf("The number you have chosen ( %Zd ) is probably prime.\n",bign);
	  }
	else
		{
			gmp_printf("The number you have chosen ( %Zd ) is composite.\n",bign);
		}

return 0;
}

/*
Function: int mpz_probab_prime_p (const mpz_t n, int reps)
Determine whether n is prime. Return 2 if n is definitely prime, return 1 if n is probably prime (without being certain), or return 0 if n is definitely non-prime.
This function performs some trial divisions, then reps Miller-Rabin probabilistic primality tests.
A higher reps value will reduce the chances of a non-prime being identified as “probably prime”.
A composite number will be identified as a prime with a probability of less than 4^(-reps). Reasonable values of reps are between 15 and 50.
*/

/* Generate random probable prime numbers https://asecuritysite.com/encryption/random3 */
