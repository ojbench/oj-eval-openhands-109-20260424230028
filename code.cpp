
#include <iostream>
#include <vector>

using namespace std;

const int MOD = 1e9 + 7;

int main() {
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<int>> adj(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> adj[i][j];
        }
    }

    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }
    if (n == 1) {
        cout << 1 << endl;
        return 0;
    }

    // dp[i][j][0] : number of ways to form a set of non-crossing trees in range [i, j]
    // such that i and j are in the same tree.
    // dp[i][j][1] : number of ways to form a set of non-crossing trees in range [i, j]
    // such that i and j are NOT necessarily in the same tree, but the whole range is covered.
    // Actually, the standard way for this problem (non-crossing spanning trees on a circle)
    // is to use the property that any such tree can be decomposed.
    
    // Let f[i][j] be the number of ways to form a single tree on the nodes i, i+1, ..., j
    // such that (i, j) is an edge in this tree.
    // Let g[i][j] be the number of ways to form a single tree on the nodes i, i+1, ..., j.
    
    // This is still not quite right. Let's use the interval DP for non-crossing trees.
    // dp[i][j][0]: i and j are connected, and the nodes {i, ..., j} form a single tree.
    // dp[i][j][1]: {i, ..., j} form a single tree, but i and j are not necessarily directly connected.
    
    // Correct DP for non-crossing spanning trees:
    // dp[i][j][0]: nodes i...j form a single tree, and i, j are connected.
    // dp[i][j][1]: nodes i...j form a single tree.
    
    vector<vector<long long>> dp0(n, vector<long long>(n, 0));
    vector<vector<long long>> dp1(n, vector<long long>(n, 0));

    for (int i = 0; i < n; ++i) {
        dp1[i][i] = 1;
    }

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            // To compute dp0[i][j]: i and j are connected.
            // We can split at k (i <= k < j), where i...k is a tree and k+1...j is a tree.
            // And i is connected to j.
            if (adj[i][j]) {
                for (int k = i; k < j; ++k) {
                    dp0[i][j] = (dp0[i][j] + dp1[i][k] * dp1[k + 1][j]) % MOD;
                }
            }
            // To compute dp1[i][j]: i...j form a tree.
            // We can split at k (i <= k < j), where i...k form a tree and k is connected to j.
            // Wait, this is the standard decomposition.
            // dp1[i][j] = sum_{k=i}^{j-1} (dp1[i][k] * dp0[k][j])
            // This way we ensure that j is connected to some k in [i, j-1].
            for (int k = i; k < j; ++k) {
                dp1[i][j] = (dp1[i][j] + dp1[i][k] * dp0[k][j]) % MOD;
            }
        }
    }

    cout << dp1[0][n - 1] << endl;

    return 0;
}
