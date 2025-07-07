#include <bits/stdc++.h>
using namespace std;
#define yes     cout << "YES" << endl;
#define no      cout << "NO" << endl;
#define vi      vector<int>
#define ll      long long
#define vl      vector<long long>
#define fori    for (int i = 0; i < n; i++) { cin >> arr[i]; }

void solve() {
    //there wont be any negative edge, and as dijkstra is a better option than bellman ford we 
    //will go on will dijkstra
    int n, m, k, start, end;
    cin >> n >> m >> k >> start >> end;
    vector<vector<pair<int, int>>> graph(n + 1);
    vector<int> visited(n + 1, 0);
    

    int main()
    {
        ios::sync_with_stdio(false);
        cin.tie(0);
        cout.tie(0);
        solve();
        return 0;
    }