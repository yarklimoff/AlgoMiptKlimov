#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int Mex(vector <int>& to_mex) {
    int i = 0;
    while (find(to_mex.begin(), to_mex.end(), i)!=to_mex.end()) {
        ++i;
    }
    return i;
}

int SpragueGrandy(int n, vector<int>& sg) {
    if (sg[n] != -1) {
        return sg[n];
    } else {
        vector<int> to_mex;
        to_mex.push_back(SpragueGrandy(n - 1, sg));
        for (int i = 1; i <= n - 2; ++i) {
            to_mex.push_back(SpragueGrandy(i, sg) ^ SpragueGrandy(n - i - 1, sg));
        }
        sg[n] = Mex(to_mex);
        return sg[n];
    }
}

void WinSchtirlitz(int n, vector<int>& sg) {
    for (int i = 1; i <= n; ++i) {
        if ((sg[i-1] ^ sg[n-i]) == 0) {
            cout << i << endl;
        }
    }
}
int main() {
    int n = 0;
    cin >> n;
    vector<int> sg(n+1, -1);
    sg[0] = 0;
    sg[1] = 0;
    sg[2] = 1;
    if (SpragueGrandy(n, sg) > 0) {
        cout << "Schtirlitz\n";
        WinSchtirlitz(n, sg);
    } else {
        cout << "Mueller\n";
    }
    return 0;
}
