/*
    Data Structures 2023-2024 Lab 07: Orders

    @author Krish Arora
*/

#include <vector>

using std::vector;
using std::min;

#define VI vector<int>

vector<VI> graph;
long transporterCount;

void articulationPoints(int node, vector<bool> &visited, VI &discoveryTime, VI &lowTime, int &timeStamp, int parent, vector<bool> &isCritical)
{
    int childCount = 0;
    visited[node] = true;
    discoveryTime[node] = lowTime[node] = timeStamp++;

    for (int neighbor : graph[node])
    {
        if (!visited[neighbor])
        {
            childCount++;
            articulationPoints(neighbor, visited, discoveryTime, lowTime, timeStamp, node, isCritical);

            lowTime[node] = min(lowTime[node], lowTime[neighbor]);

            if (parent != -1 && lowTime[neighbor] >= discoveryTime[node])
                isCritical[node] = true;
        }
        else if (neighbor != parent)
        {
            lowTime[node] = min(lowTime[node], discoveryTime[neighbor]);
        }
    }

    if (parent == -1 && childCount > 1)
        isCritical[node] = true;
}

void DFS(int curr, vector<bool> &visited, vector<bool> &criticalPoints)
{
    if (criticalPoints[curr])
    {
        visited[curr] = true;
        return;
    }

    if (visited[curr])
        return;

    visited[curr] = true;

    for (int adjacentNode : graph[curr])
    {
        if (criticalPoints[adjacentNode] && !visited[adjacentNode])
        {
            transporterCount++;
        }
        if (!visited[adjacentNode])
        {
            DFS(adjacentNode, visited, criticalPoints);
        }
    }
}

int transporters(int n, VI &tubes)
{
    long min = 0;
    transporterCount = 0;
    graph = vector<VI>(n);
    for (int i = 0; i < tubes.size(); i += 2)
    {
        int from = tubes[i];
        int to = tubes[i + 1];
        graph[from].push_back(to);
        graph[to].push_back(from);
    }

    VI discoveryTime(n, 0), lowTime(n, 0);
    vector<bool> visited(n, false);
    vector<bool> criticalPoints(n, false);
    int time = 0, emptyParent = -1;

    articulationPoints(0, visited, discoveryTime, lowTime, time, emptyParent, criticalPoints);

    fill(visited.begin(), visited.end(), false);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            if (criticalPoints[i])
                continue;
            DFS(i, visited, criticalPoints);
            if (transporterCount < 2)
                min++;
            transporterCount = 0;
            for (int j = 0; j < n; j++)
            {
                if (criticalPoints[j])
                    visited[j] = false;
            }
        }
    }

    return min;
}
