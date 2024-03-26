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
    if (!head)
    {
        return {0, 0};
    }
    // Detect if there is a loop
    Link<int> *slow = head->next;
    Link<int> *fast = (head->next) ? head->next->next : nullptr;
    while (fast && fast != slow)
    {
        slow = slow->next;
        fast = (fast->next) ? fast->next->next : nullptr;
    }
    // Find the length of the loop
    int loopLength = 0;
    if (fast)
    {
        do
        {
            fast = fast->next;
            loopLength++;
        } while (slow != fast);
    }
    // Find the length of the tail
    int tailLength = 0;
    slow = fast = head;
    for (int i = 0; i < loopLength; i++)
    {
        fast = fast->next;
    }
    while (fast)
    {
        if (slow == fast && loopLength)
        {
            break;
        }
        slow = slow->next;
        fast = fast->next;
        tailLength++;
    }
    return {loopLength, tailLength};
}
