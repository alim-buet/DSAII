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

void solve(int tc)
{
    int n;
    cin >> n;

    unordered_map<string, vector<pair<string, int>>> graph;
    unordered_set<string> cities;
    for (int i = 0; i < n; i++)
    {
        string u, v;
        int wt;
        cin >> u >> v >> wt;
        graph[u].push_back({v, wt});
        graph[v].push_back({u, wt});
        cities.insert(u);
        cities.insert(v);
    }

    // apply prims
    unordered_map<string, bool> inMst;
    for (string s : cities)
    {
        inMst[s] = false;
    }
    unordered_map<string, int> minWt;
    for (string s : cities)
    {
        minWt[s] = INT32_MAX;
    }
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> minpq;
    minpq.push({0, *cities.begin()});
    int totalWt = 0;
    while (!minpq.empty())
    {
        auto p = minpq.top();
        minpq.pop();
        int wt = p.first;
        string city = p.second;
        if (inMst[city])
            continue;
        inMst[city] = true;
        totalWt += wt;
        for (auto nbr : graph[city])
        {
            string nbrCity = nbr.first;
            int cost = nbr.second;
            if (cost < minWt[nbrCity])
            {
                minWt[nbrCity] = cost;
                minpq.push({cost, nbrCity});
            }
        }
    }
    for (auto el : cities)
    {
        if (!inMst[el])
        {
            cout << "Case " << tc << ": " << "Impossible" << endl;
            return;
        }
    }
    cout << "Case " << tc << ": " << totalWt << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    string blank;
    getline(cin, blank); // consume newline after T

    for (int caseNum = 1; caseNum <= T; ++caseNum)
    {
        getline(cin, blank); // consume blank line before each case
        solve(caseNum);
    }

    return 0;
}