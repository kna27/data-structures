/*
    Data Structures 2023-2024 Lab 08: Factor

    @author Krish Arora
*/

#include <vector>

using std::vector;

unsigned long modMul(unsigned long a, unsigned long b, unsigned long m)
{
    if (m == 1)
    {
        return 0;
    }
    unsigned long modA = a % m;
    unsigned long modB = b % m;
    unsigned long prod = modA * modB;
    if (modA != 0 && prod / modA != modB)
    {
        unsigned long result = 0;
        a = modA;
        while (b > 0)
        {
            if (b & 1)
            {
                if (result > m - a)
                {
                    result -= (m - a);
                }
                else
                {
                    result += a;
                }
            }
            if (a < m - a)
            {
                a <<= 1;
            }
            else
            {
                a = (a - (m - a)) % m;
            }
            b >>= 1;
        }
        return result;
    }
    else
    {
        return prod % m;
    }
}

unsigned long modPow(unsigned long base, unsigned long exponent, unsigned long modulus)
{
    if (modulus == 1)
    {
        return 0;
    }
    unsigned long result = 1;
    base %= modulus;
    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = modMul(result, base, modulus);
        }
        exponent >>= 1;
        base = modMul(base, base, modulus);
    }
    return result;
}

unsigned long gcd(unsigned long a, unsigned long b)
{
    while (b != 0)
    {
        unsigned long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

bool millerRabin(unsigned long n)
{
    if (n == 2 || n == 3)
    {
        return true;
    }
    if (n == 1 || n % 2 == 0)
    {
        return false;
    }

    int bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    unsigned long long d = n - 1;
    int s = 0;
    while (d % 2 == 0)
    {
        d /= 2;
        s++;
    }
    for (int base : bases)
    {
        if (base >= n)
        {
            break;
        }
        unsigned long long x = modPow(base, d, n);
        if (x == 1 || x == n - 1)
        {
            continue;
        }
        bool isComposite = true;
        for (int r = 1; r < s; r++)
        {
            x = modPow(x, 2, n);
            if (x == 1)
            {
                return false;
            }
            if (x == n - 1)
            {
                isComposite = false;
                break;
            }
        }
        if (isComposite)
        {
            return false;
        }
    }
    return true;
}

unsigned long long pollardsRho(unsigned long long n)
{
    if (n % 2 == 0)
    {
        return 2;
    }
    long long x = 2, y = 2, d = 1;
    unsigned long long c = 1;
    auto f = [n, c](long long x) -> long long
    { return (modPow(x, 2, n) + c) % n; };

    while (d == 1)
    {
        x = f(x);
        y = f(f(y));
        d = gcd(abs(x - y), n);
    }
    return d == n ? 1 : d;
}

void addFactor(vector<unsigned long> &factors, unsigned long p, unsigned long e)
{
    if (factors.empty() || factors[factors.size() - 2] != p)
    {
        factors.push_back(p);
        factors.push_back(e);
    }
    else
    {
        factors.back() += e;
    }
}

vector<unsigned long> factor(unsigned long n)
{
    vector<unsigned long> factors;
    while (n % 2 == 0)
    {
        addFactor(factors, 2, 1);
        n /= 2;
    }

    for (unsigned long i = 3; i <= sqrt(n); i += 2)
    {
        while (n % i == 0)
        {
            addFactor(factors, i, 1);
            n /= i;
        }
    }

    if (n > 2)
    {
        if (millerRabin(n))
        {
            addFactor(factors, n, 1);
        }
        else
        {
            unsigned long long nFactor = pollardsRho(n);
            if (nFactor == 1)
            {
                addFactor(factors, n, 1);
            }
            else
            {
                vector<unsigned long> smallerFactors = factor(nFactor);
                factors.insert(factors.end(), smallerFactors.begin(), smallerFactors.end());
            }
        }
    }
    return factors;
}
