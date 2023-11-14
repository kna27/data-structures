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
/*
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
    cout << "\tTest 9: " << (j9 == vector<int>{3, 2, 5, 4, 1}) << '\n';

    cout << "loopTail tests\n";
    Link<int> *l1 = nullptr;
    vector<int> t1 = loopTail(l1);
    cout << "\tTest 1: " << (t1 == vector<int>{0, 0}) << '\n';

    // Test 2: List without loop
    Link<int> *l2 = new Link<int>(1);
    l2->next = new Link<int>(2);
    vector<int> t2 = loopTail(l2);
    cout << "\tTest 2: " << (t2 == vector<int>{0, 2}) << '\n';

    // Test 3: List with loop, no tail
    Link<int> *l3 = new Link<int>(1);
    l3->next = new Link<int>(2);
    l3->next->next = new Link<int>(3);
    l3->next->next->next = l3;
    vector<int> t3 = loopTail(l3);
    cout << "\tTest 3: " << (t3 == vector<int>{3, 0}) << '\n';

    // Test 4: List with loop and tail
    Link<int> *l4 = new Link<int>(1);
    l4->next = new Link<int>(2);
    l4->next->next = new Link<int>(3);
    l4->next->next->next = new Link<int>(4);
    l4->next->next->next->next = l4->next;
    vector<int> t4 = loopTail(l4);
    cout << "\tTest 4: " << (t4 == vector<int>{3, 1}) << '\n';

    Link<int> *l5 = new Link<int>(1);
    vector<int> t5 = loopTail(l5);
    cout << "\tTest 5: " << (t5 == vector<int>{0, 1}) << '\n';

    // Test 6: Single node, self-loop
    Link<int> *l6 = new Link<int>(1);
    l6->next = l6;
    vector<int> t6 = loopTail(l6);
    cout << "\tTest 6: " << (t6 == vector<int>{1, 0}) << '\n';

    // Test 7: Multiple nodes, self-loop
    Link<int> *l7 = new Link<int>(1);
    l7->next = new Link<int>(2);
    l7->next->next = new Link<int>(3);
    l7->next->next->next = new Link<int>(4);
    l7->next->next->next->next = new Link<int>(5);
    l7->next->next->next->next->next = l7->next->next;
    vector<int> t7 = loopTail(l7);
    cout << "\tTest 7: " << (t7 == vector<int>{3, 2}) << '\n';

    // Test 8: Two nodes, self-loop
    Link<int> *l8 = new Link<int>(1);
    l8->next = new Link<int>(2);
    l8->next->next = l8->next;
    vector<int> t8 = loopTail(l8);
    cout << "\tTest 8: " << (t8 == vector<int>{1, 1}) << '\n';

    // Test 9: Multiple nodes, loop, tail
    Link<int>* l9 = new Link<int>(1);
    l9->next = new Link<int>(2);
    l9->next->next = new Link<int>(3);
    l9->next->next->next = nullptr;
    vector<int> loopTailResult4 = loopTail(l9);
    cout << "\tTest 9: " << (loopTailResult4 == vector<int>{0, 3}) << '\n';
    return 0;
}
*/
