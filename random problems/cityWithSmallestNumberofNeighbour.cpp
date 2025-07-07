#include <bits/stdc++.h>
using namespace std;
#define yes cout << "YES" << endl;
#define no cout << "NO" << endl;
#define vi vector<int>
#define ll long long
#define vl vector<long long>
#define fori                    \
    for (int i = 0; i < n; i++) \
    {                           \
        cin >> arr[i];          \
    }

class Solution
{
public:
    int findTheCity(int n, vector<vector<int>> &edges, int distanceThreshold)
    {
        // build the matrix from the list
        const int inf = 1e8;
        vector<vector<int>> matrix(n, vector<int>(n, inf));
        for (auto e : edges)
        {
            int u = e[0];
            int v = e[1];
            int wt = e[2];
            matrix[u][v] = wt;
            matrix[v][u] = wt;
        }
        // gotta make the diagonal element 0 if they are not already
        for (int i = 0; i < n; i++)
        {
            matrix[i][i] = (matrix[i][i] < 0) ? matrix[i][i] : 0;
        }
        // apply floyd warshall
        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                }
            }
        }
        int count = 0;
        int candidate = 0;
        for (int i = 1; i < n; i++)
        {
            if (matrix[0][i] <= distanceThreshold)
                count++;
        }
        for (int i = 1; i < n; i++)
        {
            int currcount = 0;
            for (int j = 0; j < n; j++)
            {
                if (matrix[i][j] <= distanceThreshold)
                {
                    currcount++;
                }
            }
            if (currcount < count)
            {
                count = currcount;
                candidate = i;
            }
        }
        return candidate;

    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}