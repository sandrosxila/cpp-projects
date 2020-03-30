#include <bits/stdc++.h>

using namespace std;
//for r-values
int kmp(string &&text, string &&pattern) {
    int ans = 0;
    const int len = pattern.size();
    int *aux = new int[len];
    aux[0] = 0;
    for (int idx = 0, auxIndex = 1; auxIndex < len; auxIndex++) {
        if (pattern[auxIndex] == pattern[idx]) {
            idx++;
            aux[auxIndex] = idx;
        }
        else {
            idx = aux[max(idx - 1, 0)];
            if (pattern[auxIndex] == pattern[idx]) idx++, aux[auxIndex] = idx;
            else aux[auxIndex] = 0;
        }
    }
    for (int idx = 0, patternIndex = 0; idx < text.size(); idx++) {
        if (text[idx] == pattern[patternIndex]) {
            patternIndex++;
        }
        else {
            patternIndex = aux[max(patternIndex - 1, 0)];
            if (text[idx] != pattern[patternIndex])patternIndex = 0;
            else patternIndex++;
        }
        if (patternIndex == len) {
            cout << pattern << " pattern in "<<"\""<<text<<"\""<<" found at: " << idx - len + 1 << endl;
            ans++;
            patternIndex = aux[patternIndex - 1];
        }
    }
    return ans;
}
// for l-values
int kmp(string &text, string &pattern) {
    int ans = 0;
    const int len = pattern.size();
    int *aux = new int[len];
    aux[0] = 0;
    for (int idx = 0, auxIndex = 1; auxIndex < len; auxIndex++) {
        if (pattern[auxIndex] == pattern[idx]) {
            idx++;
            aux[auxIndex] = idx;
        }
        else {
            idx = aux[max(idx - 1, 0)];
            if (pattern[auxIndex] == pattern[idx]) idx++, aux[auxIndex] = idx;
            else aux[auxIndex] = 0;
        }
    }
    for (int idx = 0, patternIndex = 0; idx < text.size(); idx++) {
        if (text[idx] == pattern[patternIndex]) {
            patternIndex++;
        }
        else {
            patternIndex = aux[max(patternIndex - 1, 0)];
            if (text[idx] != pattern[patternIndex])patternIndex = 0;
            else patternIndex++;
        }
        if (patternIndex == len) {
            cout << pattern << " pattern in "<<"\""<<text<<"\""<<" found at: " << idx - len + 1 << endl;
            ans++;
            patternIndex = aux[patternIndex - 1];
        }
    }
    return ans;
}

int main() {
//    r-value example
    int x = kmp("aaaaaa", "aa");
    cout << "number of pattern repetitions in the text: " << x << endl;
//    l-value example
    string p = "acabacacd", t = "acfacabacabacacdk";
    int reps = kmp(t, p);
    cout << "number of pattern repetitions in the text: " << reps << endl;
    return 0;
}
