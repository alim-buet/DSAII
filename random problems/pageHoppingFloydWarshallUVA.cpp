#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

const int MAX_N = 101;
const int INF = 1e9;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    int caseNumber = 1;

    while (true)
    {
        vector<vector<double>> adj(MAX_N, vector<double>(MAX_N, INF));
        const int inf = 1e7;
        for (int i = 0; i < MAX_N; i++)
        {
            for (int j = 0; j < MAX_N; j++)
            {
                adj[i][j] = inf;
            }
        }

        for (int i = 1; i < MAX_N; ++i)
            adj[i][i] = 0;

        bool hasInput = false;

        while (cin >> a >> b, a || b)
        {
            hasInput = true;
            adj[a][b] = 1;
        }

        if (!hasInput)
            break;

        // Place your Floyd-Warshall or other logic here]
        for (int k = 0; k < MAX_N; k++)
        {
            for (int i = 0; i < MAX_N; i++)
            {
                for (int j = 0; j < MAX_N; j++)
                {
                    if (adj[i][j] > adj[i][k] + adj[k][j])
                    {
                        adj[i][j] = adj[i][k] + adj[k][j];
                    }
                }
            }
        }

        // Example dummy output
        double avgLength = 0.0; // Replace with actual average
        


        cout << "Case " << caseNumber++ << ": average length between pages = ";
        cout << fixed << setprecision(3) << avgLength << " clicks\n";
    }

    return 0;
}
