#include "rsa.h"
#include "utils.h"

int check(size_t fi_n, size_t d, size_t e);
/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{

	*primes_sz = 0;
	size_t *primes = malloc(limit * sizeof(size_t));
	size_t *primes_leq_limit = malloc(limit * sizeof(size_t));
	// init primes with value of 1
	for (size_t i = 2; i < limit; i++)
		primes[i] = 1;

	// ApplySieve of Eratosthenes Algorithm
	for (size_t i = 2; i * i <= limit; i++)
		if (primes[i] == 1)
			for (size_t j = i * i; j <= limit; j += i)
				primes[j] = 0;

	for (size_t i = 2; i < limit; i++)
		if (primes[i] == 1)
		{
			primes_leq_limit[*primes_sz] = i;
			*primes_sz = *primes_sz + 1;
		}

	return primes_leq_limit;
}

/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int gcd(int a, int b)
{
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n)
{
	size_t e;
	int primes_sz;
	int primes_gcd1_sz = 0;
	printf("fi_n -> %d \n", fi_n);
	// get all the primes from 2 to fi_n by calling sieve_of_eratosthenes
	size_t *primes = sieve_of_eratosthenes(fi_n, &primes_sz);
	size_t *primes_gcd1 = malloc(primes_sz * sizeof(size_t));

	// create an array for the primes have gcd(prime, fi_n) == 1)
	for (size_t i = 0; i < primes_sz; i++)
		if ((gcd(primes[i], fi_n) == 1))
		{
			primes_gcd1[i] = primes[i];
			primes_gcd1_sz = primes_gcd1_sz + 1;
		}
	// choose randomly from the created array
	e = primes_gcd1[rand() % primes_gcd1_sz];
	return e;
}

/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)
{
	//Implement the extended euclidean algorithm
	size_t x0 = 0, x1 = 1, y0 = 1, y1 = 0;
	while (r0 > 0)
	{
		r = r1 % r0;
		q = r1 / r0;
		r1 = r0;
		r0 = r;
		x = x1 - (q * x0);
		x1 = x0;
		x0 = x;
		y = y1 - (q * y0);
		y1 = y0;
		y0 = y;
	}
	
	return x1;
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void rsa_keygen(void)
{
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;
	srand(time(0));

	print("Start generation...", info);
	int primes_sz;
	size_t *primes = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, &primes_sz);
	p = primes[rand() % primes_sz];
	q = primes[rand() % primes_sz];
	n = p * q;
	fi_n = (p - 1) * (q - 1);
	e = choose_e(fi_n);
	d = mod_inverse(e, fi_n);
	printf("\n\npublic key conists of n=%d, d=%d\n", n, d);
	printf("public key conists of n=%d, e=%d\n", n, e);
	if (check(fi_n,d,e) == 1)
		print("OK", success);
}

/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_encrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */
}

/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_decrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */
}

int check(size_t fi_n, size_t d, size_t e)
{
	size_t m = (d * e) - 1;
	if (abs(m) % fi_n == 0)
		return 1;
	return 0;
}
