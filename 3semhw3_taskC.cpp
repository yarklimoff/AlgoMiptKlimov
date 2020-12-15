#include <iostream>
#include <vector>
#include <algorithm>

int Mex(std::vector <int>& to_mex) {
    int i = 0;
    while (find(to_mex.begin(), to_mex.end(), i)!=to_mex.end()) {
        ++i;
    }
    return i;
}

int SpragueGrandy(int n, std::vector<int>& sg) {
    if (sg[n] != -1) {
        return sg[n];
    } else {
        std::vector<int> to_mex;
        to_mex.push_back(SpragueGrandy(n - 1, sg));
        for (int i = 1; i <= n - 2; ++i) {
            to_mex.push_back(SpragueGrandy(i, sg) ^ SpragueGrandy(n - i - 1, sg));
        }
        sg[n] = Mex(to_mex);
        return sg[n];
    }
}

void WinSchtirlitz(int n, std::vector<int>& sg) {
    for (int i = 1; i <= n; ++i) {
        if ((sg[i-1]^sg[n-i]) == 0) {
            std::cout << i << std::endl;
        }
    }
}
int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> sg(n+1, -1);
    sg[0] = 0;
    sg[1] = 0;
    sg[2] = 1;
    if (SpragueGrandy(n, sg) > 0) {
        std::cout << "Schtirlitz\n";
        WinSchtirlitz(n, sg);

    } else {
        std::cout << "Mueller\n";
    }
    return 0;
}
