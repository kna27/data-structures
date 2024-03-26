/*
    Data Structures 2023-2024 Lab 08: Factor

    @author Krish Arora
*/

#include <vector>

using namespace std;

long long modPow(long long base, long long exponent, long long modulus)
{
    long long result = 1;
    base %= modulus;
    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = (result * base) % modulus;
        }
        exponent >>= 1;
        base = (base * base) % modulus;
    }
    return result;
}

long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long t = b;
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
    long long x = 2 + rand() % (n - 3);
    unsigned long long c = 1 + rand() % (n - 1);
    unsigned long long y = x;
    unsigned long long d = 1;
    while (d == 1)
    {
        x = (modPow(x, 2, n) + c + n) % n;
        y = (modPow(y, 2, n) + c + n) % n;
        long long diff = abs(static_cast<long long>(x) - static_cast<long long>(y));
        d = gcd(diff, n);
        if (d == n)
        {
            return pollardsRho(n);
        }
    }
    return d;
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
            if (!factors.empty() && factors[factors.size() - 2] == n)
            {
                factors.back()++;
            }
            else
            {
                addFactor(factors, n, 1);
            }
        }
        else
        {
            unsigned long long primeFactor = pollardsRho(n);
            vector<unsigned long> smallest = factor(primeFactor);
            vector<unsigned long> additional = factor(n / primeFactor);
            factors.insert(factors.end(), smallest.begin(), smallest.end());
            factors.insert(factors.end(), additional.begin(), additional.end());
        }
    }
    return factors;
}
