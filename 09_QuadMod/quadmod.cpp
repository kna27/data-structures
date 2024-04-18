/*
    Data Structures 2023-2024 Lab 09: Modular Quadratic Equations

    @author Krish Arora
*/

#include <vector>
#include <iostream>

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


long long modInv(long long a, long long n)
{
    long long m0 = n;
    long long y = 0, x = 1;
    if (n == 1)
    {
        return 0;
    }
    while (a > 1)
    {
        long long q = a / n;
        long long t = n;
        n = a % n, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0)
    {
        x += m0;
    }
    return x;
}

// eulersCriterion
bool eulersCriterion(long a, long p) {
    long result = modPow(a, (p - 1) / 2, p);
    return result == 1 || result == p - 1;
}

//modSqrt
long modSqrt(long a, long p) {
    if (p % 4 == 3) {
        return modPow(a, (p + 1) / 4, p);
    }

    long q = p - 1;
    long s = 0;
    while (q % 2 == 0) {
        q /= 2;
        s++;
    }

    long z = 2;
    while (eulersCriterion(z, p)) {
        z++;
    }

    long m = s;
    long c = modPow(z, q, p);
    long t = modPow(a, q, p);
    long r = modPow(a, (q + 1) / 2, p);

    while (t != 1) {
        long i = 0;
        long temp = t;
        while (temp != 1) {
            temp = (temp * temp) % p;
            i++;
        }

        long b = modPow(c, 1 << (m - i - 1), p);
        m = i;
        c = (b * b) % p;
        t = (t * c) % p;
        r = (r * b) % p;
    }

    return r;
}

std::vector<long> solveQuadraticModPrime(long a, long b, long c, long p) {
    std::vector<long> solutions;
    long invA = modInv(a, p); // Modular inverse of a
    long discriminant = (modPow(b, 2, p) - 4 * invA * c) % p;
    if (discriminant < 0) discriminant += p; // Ensure non-negative

    if (!eulersCriterion(discriminant, p)) {
        return solutions; // No solution exists
    }

    // Assuming we have a function to find square roots modulo p
    long sqrtD = modSqrt(discriminant, p);
    long x1 = (modInv(2 * a, p) * (-b + sqrtD)) % p;
    long x2 = (modInv(2 * a, p) * (-b - sqrtD)) % p;

    if (x1 < 0) x1 += p;
    if (x2 < 0) x2 += p;

    solutions.push_back(x1);
    if (x1 != x2) solutions.push_back(x2); // If two distinct solutions

    return solutions;
}

std::vector<long> chineseRemainderTheorem(const std::vector<long>& residues, const std::vector<long>& moduli) {
    long product = 1;
    for (long mod : moduli) {
        product *= mod;
    }

    std::vector<long> solutions;
    long result = 0;
    for (size_t i = 0; i < moduli.size(); ++i) {
        long pp = product / moduli[i];
        result += residues[i] * modInv(pp, moduli[i]) * pp;
        result %= product;
    }

    solutions.push_back(result); // For simplicity, we return one solution
    return solutions;
}


std::vector<long> quadSolve(long n, long a, long b, long c) {
    std::vector<long> solutions;

    // Factorize n
    std::vector<unsigned long> primeFactors = factor(n);
    // Containers for CRT
    std::vector<long> residues;
    std::vector<long> moduli;

    for (long p : primeFactors) {
        // Solve the equation modulo p
        std::vector<long> localSolutions = solveQuadraticModPrime(a, b, c, p);
        if (localSolutions.empty()) {
            return {}; // No solution for this prime factor
        }

        // If solutions exist, prepare them for CRT
        for (long sol : localSolutions) {
            residues.push_back(sol);
            moduli.push_back(p); // Assuming p is prime power
        }
    }

    // Use CRT to combine the solutions
    solutions = chineseRemainderTheorem(residues, moduli);

    // Ensure solutions are unique and sort them
    sort(solutions.begin(), solutions.end());
    solutions.erase(unique(solutions.begin(), solutions.end()), solutions.end());

    // Filter solutions to be non-negative and less than n
    std::vector<long> filteredSolutions;
    for (long sol : solutions) {
        if (sol >= 0 && sol < n) {
            filteredSolutions.push_back(sol);
        }
    }

    return filteredSolutions;
}

void printSolutions(const std::string& testCaseName, const std::vector<long>& solutions) {
    std::cout << testCaseName << ": ";
    if (solutions.empty()) {
        std::cout << "No solutions";
    } else {
        for (size_t i = 0; i < solutions.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << solutions[i];
        }
    }
    std::cout << std::endl;
}

int main() {
    // Test Case 1
    std::vector<long> solutions1 = quadSolve(7, 1, 0, -1);
    printSolutions("Test Case 1", solutions1);

    // Test Case 2
    std::vector<long> solutions2 = quadSolve(3, 1, 1, 1);
    printSolutions("Test Case 2", solutions2);

    // Test Case 3
    std::vector<long> solutions3 = quadSolve(11, 1, 0, 4);
    printSolutions("Test Case 3", solutions3);

    // Test Case 4
    std::vector<long> solutions4 = quadSolve(15, 1, 0, -1);
    printSolutions("Test Case 4", solutions4);

    // Test Case 5
    std::vector<long> solutions5 = quadSolve(10, 1, 0, 2);
    printSolutions("Test Case 5", solutions5);

    return 0;
}