/*
    Data Structures 2023-2024 Lab 01: Mod

    @author Krish Arora
*/

#include <iostream>
#include "mod.h"

long Mod::modulus = 17;

Mod::Mod(long t)
{
    x = t % modulus;
    if (x < 0)
    {
        x += modulus;
    }
}

Mod::Mod(const Mod &m)
{
    x = m.x;
}

Mod &Mod::operator=(const Mod &m)
{
    if (this != &m)
    {
        x = m.x;
    }
    return *this;
}

Mod &Mod::operator+=(const Mod &m)
{
    bool positiveOverflow = (x + m.x) > 0;
    long remainder = (m.x % modulus);
    if (x < 0 && m.x < 0 && positiveOverflow) // Overflow
    {
        x = (((x % modulus) + modulus) + remainder) % modulus;
    }
    else if (x > 0 && m.x > 0 && !positiveOverflow) // Underflow
    {
        x = (((x % modulus) - modulus) + remainder) % modulus;
    }
    else // No overflow
    {
        x = (x + m.x) % modulus;
    }

    // Handle negative values
    if (x < 0)
    {
        x += modulus;
    }
    return *this;
}

Mod &Mod::operator-=(const Mod &m)
{
    return *this += Mod(-m.x);
}

Mod &Mod::operator*=(const Mod &m)
{
    // Russian peasant algorithm
    Mod a(x);
    long b = m.x;
    Mod res(0);
    while (b > 0)
    {
        if (b & 1)
        {
            res += a;
        }
        b = b >> 1;
        a += a;
    }

    return *this = res;
}

Mod &Mod::operator/=(const Mod &m)
{
    Mod inverse(x);
    inverse *= m.pwr(-1);
    return *this = inverse;
}

Mod Mod::operator-() const
{
    return Mod(-x);
}

Mod Mod::pwr(long e) const
{
    // Binary exponentiation algorithm
    // Handle negative exponents
    if (e < 0)
    {
        return inv(x).pwr(-e);
    }

    Mod result(1);
    Mod base(x);

    while (e)
    {
        if (e % 2)
            result *= base;
        base *= base;
        e /= 2;
    }

    return result;
}

long Mod::val() const { return x; }

void Mod::set_modulus(long m)
{
    if (m <= 1)
    {
        std::cerr << "Modulus must be at least 2\n";
        exit(1);
    }
    modulus = m;
}

Mod Mod::inv(long r0)
{
    // Extended Euclidean algorithm
    long m = modulus;
    long x1 = 0, y1 = 1; // m coefficients
    long x2 = 1, y2 = 0; // r0 coefficients

    // r0 is non-negative and < m
    r0 %= m;
    if (r0 < 0)
        r0 += m;

    while (r0 > 0)
    {
        long q = m / r0;
        long t = m % r0;

        m = r0;
        r0 = t;

        long s = x1 - q * x2;
        x1 = x2;
        x2 = s;

        s = y1 - q * y2;
        y1 = y2;
        y2 = s;
    }

    if (m != 1)
    {
        std::cerr << "Inverse doesn't exist. r0 and modulus must be co-prime.\n";
        exit(1);
    }

    if (x1 < 0)
    {
        x1 += modulus;
    }

    return x1;
}

Mod operator+(const Mod &a, const Mod &b)
{
    Mod r(a);
    return r += b;
}

Mod operator+(long t, const Mod &m) { return Mod(t) += m; }

Mod operator-(const Mod &a, const Mod &b)
{
    Mod r(a);
    return r -= b;
}

Mod operator-(long t, const Mod &m) { return Mod(t) -= m; }

Mod operator*(const Mod &a, const Mod &b)
{
    Mod r(a);
    return r *= b;
}

Mod operator*(long t, const Mod &m) { return Mod(t) *= m; }

Mod operator/(const Mod &a, const Mod &b)
{
    Mod r(a);
    return r /= b;
}

Mod operator/(long t, const Mod &m) { return Mod(t) /= m; }

bool operator==(const Mod &a, const Mod &b) { return a.val() == b.val(); }

bool operator==(long t, const Mod &m) { return m.val() == t; }

bool operator!=(const Mod &a, const Mod &b) { return a.val() != b.val(); }

bool operator!=(long t, const Mod &m) { return m.val() != t; }

istream &operator>>(istream &is, Mod &m)
{
    long t;
    is >> t;
    m = Mod(t);
    return is;
}

ostream &operator<<(ostream &os, const Mod &m) { return os << m.val(); }
