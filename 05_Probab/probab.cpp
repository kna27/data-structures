/*
    Data Structures 2023-2024 Lab 05: Prob

    @author Krish Arora
*/

#include <cstdlib>

// Returns true with probability 0.5.
bool flip()
{
    return rand() % 2;
}

// Returns true with probability a/b.
bool flip(unsigned long a, unsigned long b)
{
    if (a == 0 && b == 0)
    {
        return false;
    }
    double target = (double)a / b;
    double prob = 0.5;
    double inc = 0.5;

    while (true)
    {
        bool result = flip();

        inc /= 2;
        if ((prob < target && result) || (prob > target && !result))
        {
            prob += (result ? inc : -inc);
        }
        else
        {
            return !result;
        }

        if (prob == target)
        {
            return false;
        }
    }
}
