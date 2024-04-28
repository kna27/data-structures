/*
    Data Structures 2023-2024 Lab 09: Modular Quadratic Equations

    @author Krish Arora
*/

#include <vector>

using std::pair;
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

long modInv(long a, long m)
{
    long m0 = m;
    long y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1)
    {
        long q = a / m;
        long t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}

static unsigned long gcd(unsigned long a, unsigned long b)
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

bool isQuadraticResidue(long a, long p)
{
    if (p == 2)
    {
        return true;
    }
    long amodp = a % p;
    if (amodp == 0)
    {
        return true;
    }
    a = (amodp + p) % p;
    return modPow(a, (p - 1) / 2, p) == 1;
}

vector<long> findRoots(long a, long b, long c, long p, long exp)
{
    vector<long> roots;
    long modulus = 1;
    for (int i = 0; i < exp; i++)
    {
        modulus *= p;
    }
    for (long x = 0; x < modulus; x++)
    {
        if ((modPow(x, 2, modulus) * a % modulus + b * x % modulus + c) % modulus == 0)
        {
            roots.push_back(x);
        }
    }
    return roots;
}

vector<long> chineseRemainderTheorem(vector<pair<long, vector<long>>> &congruences, long m)
{
    vector<long> solutions;
    for (long x = 0; x < m; x++)
    {
        bool valid = true;
        for (const auto &[mod, roots] : congruences)
        {
            bool localValid = false;
            for (long root : roots)
            {
                if (x % mod == root)
                {
                    localValid = true;
                    break;
                }
            }
            if (!localValid)
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            solutions.push_back(x);
        }
    }
    return solutions;
}

vector<long> quad_solve(unsigned long n, long a, long b, long c)
{
    vector<unsigned long> factors = factor(n);
    vector<pair<long, vector<long>>> solutions;
    long combinedModulus = 1;

    for (size_t i = 0; i < factors.size(); i += 2)
    {
        long p = factors[i];
        long exp = factors[i + 1];
        long primePower = 1;
        for (int j = 0; j < exp; j++)
        {
            primePower *= p;
        }

        if (!isQuadraticResidue((modPow(b, 2, p) - (4 * a % p) * c % p + p) % p, p))
        {
            return {};
        }

        vector<long> roots = findRoots(a, b, c, p, exp);
        solutions.emplace_back(make_pair(primePower, roots));
        combinedModulus *= primePower;
    }

    return chineseRemainderTheorem(solutions, combinedModulus);
}
