/*
    Data Structures 2023-2024 Lab 06: 2Sat

    @author Krish Arora
*/

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using std::vector;
using std::stack;
using std::queue;
using std::cin;
using std::cout;
using std::endl;
using std::fill;

const int MAX = 10001;

vector<int> graph[MAX], transposedGraph[MAX], condensedGraph[MAX];
vector<int> components[MAX];
bool visited[MAX], isActiveNode[MAX];
stack<int> vertexStack;
queue<int> reversedVertexStack;
int componentIndex[MAX];
int componentCount = 0;
int assignments[MAX];

void dfs(int var, vector<int> adj[], int dfsRun)
{
    if (visited[var])
    {
        return;
    }
    visited[var] = true;

    for (int v : adj[var])
    {
        if (!visited[v])
        {
            dfs(v, adj, dfsRun);
        }
    }

    switch (dfsRun)
    {
    case 0:
        vertexStack.push(var);
        break;
    case 1:
        componentIndex[var] = componentCount;
        components[componentCount].push_back(var);
        break;
    case 2:
        reversedVertexStack.push(var);
        break;
    }
}

void condense(int var, vector<int> adj[])
{
    if (visited[var])
    {
        return;
    }
    visited[var] = true;
    for (int v : adj[var])
    {
        if (componentIndex[var] == componentIndex[v])
        {
            continue;
        }
        else
        {
            vector<int> &componentEdges = condensedGraph[componentIndex[var]];
            if (find(componentEdges.begin(), componentEdges.end(), componentIndex[v]) == componentEdges.end())
            {
                componentEdges.push_back(componentIndex[v]);
            }
        }
        if (!visited[v])
        {
            condense(v, adj);
        }
    }
}

int negateVar(int var)
{
    return MAX - var;
}

int main()
{
    int nv, nc;
    cin >> nv >> nc;
    for (int i = 0; i < nc; i++)
    {
        int u_i, v_i;
        cin >> u_i >> v_i;
        u_i = (u_i > 0) ? u_i : u_i + MAX;
        v_i = (v_i > 0) ? v_i : v_i + MAX;
        graph[negateVar(u_i)].push_back(v_i);
        transposedGraph[v_i].push_back(negateVar(u_i));
        graph[negateVar(v_i)].push_back(u_i);
        transposedGraph[u_i].push_back(negateVar(v_i));
        isActiveNode[u_i] = isActiveNode[negateVar(u_i)] = true;
        isActiveNode[v_i] = isActiveNode[negateVar(v_i)] = true;
    }

    for (int i = 1; i < MAX; i++)
    {
        if (isActiveNode[i] && !visited[i])
        {
            dfs(i, graph, 0);
        }
    }
    fill(visited, visited + MAX, false);
    while (!vertexStack.empty())
    {
        int currentNode = vertexStack.top();
        vertexStack.pop();
        if (!visited[currentNode])
        {
            dfs(currentNode, transposedGraph, 1);
            componentCount++;
        }
    }
    for (int i = 1; i <= 5000; i++)
    {
        if (isActiveNode[i] && componentIndex[i] == componentIndex[negateVar(i)])
        {
            cout << "Not satisfiable" << endl;
            return 0;
        }
    }
    for (int i = 1; i < MAX; i++)
    {
        visited[i] = false;
        if (isActiveNode[i])
        {
            condense(i, graph);
        }
    }
    fill(visited, visited + MAX, false);
    for (int i = 0; i < componentCount; i++)
    {
        dfs(i, condensedGraph, 2);
    }
    while (!reversedVertexStack.empty())
    {
        int currentNode = reversedVertexStack.front();
        reversedVertexStack.pop();
        for (int v : components[currentNode])
        {
            if (assignments[v] == 0)
            {
                assignments[v] = 1;
                assignments[negateVar(v)] = -1;
            }
        }
    }

    for (int i = 1; i <= nv; i++)
    {
        cout << (isActiveNode[i] ? (assignments[i] == 1 ? "T" : "F") : "T") << endl;
    }

    return 0;
}
