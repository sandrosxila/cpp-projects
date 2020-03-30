#include <bits/stdc++.h>

using namespace std;

int kmp(string &&text, string &&pattern) {
    int ans = 0;
    const int len = pattern.size();
    int *aux = new int[len];
    aux[0] = 0;
    for (int idx = 0, auxIndex = 1; auxIndex < len; auxIndex++) {
        if (pattern[auxIndex] == pattern[idx]) {
            idx++;
            aux[auxIndex] = idx;
        } else {
            idx = aux[max(idx - 1, 0)];
            if (pattern[auxIndex] == pattern[idx]) idx++, aux[auxIndex] = idx;
            else aux[auxIndex] = 0;
        }
    }
//    cout<<text<<' '<<pattern<<endl;
    for (int idx = 0, patternIndex = 0; idx < text.size(); idx++) {
//        cout<<text[idx]<<' '<<pattern[patternIndex]<<endl;
        if (text[idx] == pattern[patternIndex]) {
//            cout<<text[idx]<<' '<<idx<<endl;
            patternIndex++;
        } else {
            patternIndex = aux[max(patternIndex - 1, 0)];
            if (text[idx] != pattern[patternIndex])patternIndex = 0;
            else patternIndex++;
        }
//        if(idx == text.size() - 2) cout<<endl<<patternIndex<<endl;
        if (patternIndex == len) {
            cout << idx - len + 1 << endl;
            ans++;
            patternIndex = aux[patternIndex - 1];
        }
    }
    return ans;
}

int main() {
    int x = kmp("aaaaaa", "aa");
    cout << x << endl;
    return 0;
}
