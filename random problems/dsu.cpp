#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <numeric>
#include <limits>
#include <cstring>
#include <cassert>
#include <list>
#include <tuple>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
#define yes cout << "YES" << endl;
#define no cout << "NO" << endl;
#define vi vector<int>
#define vl vector<long long>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define ll long long
class DisjointSet
{
    vector<int> parent, rank, size;

public:
    // Constructor to initialize DSU
    DisjointSet(int n)
    {
        parent.resize(n + 1);
        rank.resize(n + 1, 0); // Rank array for union by rank
        size.resize(n + 1, 1); // Size array for union by size

        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    // Find function with path compression
    int findParent(int node)
    {
        if (parent[node] == node)
            return node;
        return parent[node] = findParent(parent[node]); // Path compression
    }

    // Union by Rank
    void unionByRank(int u, int v)
    {
        int uSet = findParent(u);
        int vSet = findParent(v);

        if (uSet != vSet)
        {
            // Attach smaller tree to larger tree based on rank
            if (rank[uSet] < rank[vSet])
            {
                parent[uSet] = vSet;
            }
            else if (rank[uSet] > rank[vSet])
            {
                parent[vSet] = uSet;
            }
            else
            {
                parent[uSet] = vSet;
                rank[vSet]++; // Increase rank of root
            }
        }
    }

    // Union by Size
    void unionBySize(int u, int v)
    {
        int uSet = findParent(u);
        int vSet = findParent(v);

        if (uSet != vSet)
        {
            // Attach smaller tree to larger tree based on size
            if (size[uSet] < size[vSet])
            {
                parent[uSet] = vSet;
                size[vSet] += size[uSet]; // Update size of the new root
            }
            else
            {
                parent[vSet] = uSet;
                size[uSet] += size[vSet]; // Update size of the new root
            }
        }
    }

    // Get the size of the component that node belongs to
    int getSize(int u)
    {
        int uSet = findParent(u);
        return size[uSet];
    }

    // Get the rank of the component that node belongs to (for union by rank)
    int getRank(int u)
    {
        int uSet = findParent(u);
        return rank[uSet];
    }
};

void solve()
{
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    return 0;
}