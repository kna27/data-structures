/*
    Data Structures 2023-2024 Lab 05: Probab

    @author Krish Arora
*/

#include <random>

// Returns true with probability 0.5.
bool flip()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 1);

    return dis(gen) == 1;
}

// Returns true with probability a/b.
bool flip(unsigned long a, unsigned long b)
{
    // Edge cases
    if (a == 0)
    {
        return false;
    }
    if (b == 0 || a >= b)
    {
        return true;
    }

    const int MAX_ITERATIONS = 512; // Ensure O(1) constant runtime
    double target = static_cast<double>(a) / b;
    double prob = 0.5;
    double inc = 0.5;

     // Adjust probability towards target
    for (int i = 0; i < MAX_ITERATIONS; i++)
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
    }

    // After MAX_ITERATIONS, decide based on the current probability
    return prob >= target;
}
