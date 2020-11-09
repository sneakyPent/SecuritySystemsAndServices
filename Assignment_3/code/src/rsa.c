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
	// printf("fi_n -> %d \n", fi_n);
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
	size_t r, q, x, y;
	size_t r0 = b, r1 = a;
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
void rsa_keygen(char *public_key_file, char *private_key_file)
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
	if (((int)d) < 0)
	{
		print("negative", success);
		printf("%d", d);
		d += fi_n;
	}
	printf("\npublic key conists of n=%d, d=%d\n", n, d);
	printf("public key conists of n=%d, e=%d\n", n, e);
	if (check(fi_n, d, e) == 1)
		print("d,e are modular inverse", success);
	long len1 = sizeof(size_t);
	fileManager(public_key_file, "w", &n, &len1);
	fileManager(public_key_file, "a", &e, &len1);
	fileManager(private_key_file, "w", &n, &len1);
	fileManager(private_key_file, "a", &d, &len1);
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
	size_t e,n,ct;
	size_t *nde;

	unsigned char *publicKey, *plaintext;
	long len, plaintextLen;
	publicKey = fileManager(key_file, "r", publicKey, &len);
	plaintext = fileManager(input_file, "r", plaintext, &plaintextLen);
	
	print_string(plaintext, plaintextLen);

	nde = splitKey(publicKey);

	e=nde[0];
	n=nde[1];
	printf("e = %li\n", nde[0]);
	printf("n = %li\n", nde[1]);
	len = sizeof(ct);
	
	for (size_t i = 0; i < strlen(plaintext); i++)
	{
		ct = myPow(plaintext[i], e, n);
		if (((int) i) == 0 )
			fileManager(output_file, "w", &ct, &len);
		else
			fileManager(output_file, "a", &ct, &len);
	}
	plaintext = fileManager(output_file, "r", plaintext, &plaintextLen);
	printf("size of file is %d\n",plaintextLen);
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
	size_t d,n ;
	size_t *nde;

	unsigned char *privateKey, *cipherText;
	long len, cipherTextLen;
	privateKey = fileManager(key_file, "r", NULL, &len);
	cipherText = fileManager(input_file, "r", NULL, &cipherTextLen);

	print_hex(privateKey, len);
	print_hex(cipherText, cipherTextLen);
	nde = splitKey(privateKey);
	d=nde[0];
	n=nde[1];
	int bufsize = sizeof(size_t);
	char ct;
	unsigned char* plaintext = malloc(cipherTextLen/bufsize);
	int plaintextContent = 0;
	for (int i = 0; i < cipherTextLen; i++)
	{
		unsigned char buf[bufsize], d_e_str[2*sizeof(size_t)] = "", str1[80] ;
		buf[7-i%(bufsize)] = cipherText[i];
		if (i%bufsize == bufsize-1 ){
			for (size_t j = 0; j < sizeof(buf); j++)
			{
				sprintf(str1, "%02X", buf[j]);
				strcat(d_e_str, str1);
			}
			size_t x = strtoul(d_e_str, NULL, 16);
			char z = myPow(x, d, n);
			long len = 1;
			plaintext[plaintextContent]=z;
			plaintextContent+=1;
		}

	}
	print_string(plaintext,plaintextContent);
	long sz = cipherTextLen/bufsize;
	fileManager(output_file, "w", plaintext, &sz);
}

int check(size_t fi_n, size_t d, size_t e)
{
	size_t m = (d * e) - 1;
	if (abs(m) % fi_n == 0)
		return 1;
	return 0;
}
