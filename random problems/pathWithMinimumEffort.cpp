#include <bits/stdc++.h>
using namespace std;
#define yes     cout << "YES" << endl;
#define no      cout << "NO" << endl;
#define vi      vector<int>
#define ll      long long
#define vl      vector<long long>
#define fori    for (int i = 0; i < n; i++) { cin >> arr[i]; }
class Solution
{
public:
    int minimumEffortPath(vector<vector<int>> &heights)
    {
        int rows = heights.size();
        int colm = heights[0].size();
        const int inf = 1e8;
        vector<vector<int>> minDist(rows, vector<int>(colm, inf));
        priority_queue < pair<int, pair<int, int>>, vector < pair<int, pair<int, int>>>, greater<pair<int,pair<int,int>>>> minpq;
        minpq.push{0,{0,0}};
        minDist[0][0]=0;
        while(!minpq.empty()){
            pair<int, pair<int, int>> el = minpq.top();
            minpq.pop();
            int d = el.first;
            int r = el.second.first;
            int c = el.second.second;
            if(d> minDist[r][c]) continue;
            if(c-1>=0){
                int curreffort = abs(heights[r][c] - heights[r][c - 1]);
                if(max(curreffort,d) < minDist[r][c-1]){
                    minDist[r][c - 1] = max(curreffort, d);
                    minpq.push{max(curreffort, d), {r, c - 1}};
                }
                
            }
            if(c+1<colm){
                int curreffort = abs(heights[r][c] - heights[r][c + 1]);
                if (max(curreffort, d) < minDist[r][c + 1])
                {
                    minDist[r][c + 1] = max(curreffort, d);
                    minpq.push{max(curreffort, d), {r, c + 1}};
                }
            }
            if(r-1>=0){
                int curreffort = abs(heights[r][c] - heights[r-1][c ]);
                if (max(curreffort, d) < minDist[r-1][c])
                {
                    minDist[r-1][c] = max(curreffort, d);
                    minpq.push{max(curreffort, d), {r-1, c}};
                }
            }
            if (r + 1 <rows)
            {
                int curreffort = abs(heights[r][c] - heights[r + 1][c]);
                if (max(curreffort, d) < minDist[r + 1][c])
                {
                    minDist[r + 1][c] = max(curreffort, d);
                    minpq.push{max(curreffort, d), {r + 1, c}};
                }
            }
        }
        return minDist[rows - 1][colm - 1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}