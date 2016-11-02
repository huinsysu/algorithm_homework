#include <iostream>
using namespace std;

int main() {

    int n, total_w;
    int w[100], v[100];
    int M[100][100];

    cout << "背包的总容量为：" << endl;
    cin >> total_w;
    cout << "物品数目为：" << endl;
    cin >> n;

    cout << "每个物品的重量及其价值分别为：" << endl;

    for (int i = 1; i <= n; i++)
        cin >> w[i] >> v[i];

    for (int i = 0; i < n; i++)
        M[0][i] = 0;

    for (int i = 1; i <= n; i++) {
       for (int j = 0; j <= total_w; j++) {
           if (w[i] > j)
               M[i][j] = M[i - 1][j];
           else
               M[i][j] = max(M[i - 1][j - w[i]] + v[i], M[i - 1][j]);
       }
    }

    cout << "背包能容纳的最大价值为：" << M[n][total_w] << endl;
    cout << "所选的背包为：" << endl;

    int current_w = total_w;
    for (int i = n; i >= 1; i--) {
        if (current_w >= w[i]) {
            if (M[i-1][current_w - w[i]] + v[i] > M[i-1][current_w]) {
                current_w -= w[i];
                cout << "( " << w[i] << "," << v[i] << " )" << endl;
            }
        }
    }

}
