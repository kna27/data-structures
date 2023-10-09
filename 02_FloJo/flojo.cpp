/*
    Data Structures 2023-2024 Lab 02: FloJo

    @author Krish Arora
*/

#include <iostream>
#include <vector>

using std::cout;
using std::vector;

template <class T>
struct Link
{
    explicit Link(const T &info, Link *next = 0) : info(info), next(next) {}
    // This avoids stack overflow
    ~Link()
    {
        Link *p = next;
        while (p)
        {
            Link *q = p->next;
            p->next = nullptr;
            delete p;
            p = q;
        }
    }
    T info;
    Link *next;
};
std::vector<int> josephus(int n, int k)
{
    if (n <= 0 || k <= 0)
    {
        return {};
    }
    k = k % n;
    Link<int> *head = new Link<int>(1);
    Link<int> *tail = head;
    for (int i = 2; i <= n; i++)
    {
        tail->next = new Link<int>(i);
        tail = tail->next;
    }
    tail->next = head;
    std::vector<int> result;
    Link<int> *current = head;
    int elimsToDo = n;

    while (elimsToDo > 1)
    {
        for (int i = 0; i < (k % elimsToDo + elimsToDo) - 2; i++)
        {
            current = current->next;
        }

        Link<int> *eliminated = current->next;
        result.push_back(eliminated->info);
        current->next = eliminated->next;
        eliminated->next = nullptr;
        delete eliminated;

        current = current->next;
        elimsToDo--;
    }

    result.push_back(current->info);
    current->next = nullptr;
    delete current;
    return result;
}

vector<int> loopTail(Link<int> *head)
{
    return {}; // TODO
}

int main()
{
    cout << "josephus tests\n";
    vector<int> j1 = josephus(0, 0);
    cout << "\tTest 1: " << (j1 == vector<int>{}) << '\n';
    vector<int> j2 = josephus(0, 5);
    cout << "\tTest 2: " << (j2 == vector<int>{}) << '\n';
    vector<int> j3 = josephus(5, 0);
    cout << "\tTest 3: " << (j3 == vector<int>{}) << '\n';
    vector<int> j4 = josephus(-5, 2);
    cout << "\tTest 4: " << (j4 == vector<int>{}) << '\n';
    vector<int> j5 = josephus(-5, -3);
    cout << "\tTest 5: " << (j5 == vector<int>{}) << '\n';
    vector<int> j6 = josephus(10, 3);
    cout << "\tTest 6: " << (j6 == vector<int>{3, 6, 9, 2, 7, 1, 8, 5, 10, 4}) << '\n';
    vector<int> j7 = josephus(1, 5);
    cout << "\tTest 7: " << (j7 == vector<int>{1}) << '\n';
    vector<int> j8 = josephus(5, 1);
    cout << "\tTest 8: " << (j8 == vector<int>{1, 2, 3, 4, 5}) << '\n';
    vector<int> j9 = josephus(5, 8);
    cout << "\tTest 9: " << (j9 == vector<int>{3, 1, 5, 2, 4}) << '\n';

    cout << "loopTail tests\n";
    // TODO
    return 0;
}
