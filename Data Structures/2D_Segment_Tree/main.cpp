#include <bits/stdc++.h>

using namespace std;
int n, i, j, k, m, q, X1, Y1, X2, Y2;
#define midx ((lx+rx)>>1)
#define midy ((ly+ry)>>1)
struct node {
    int data = 0;
    node *leftUp = nullptr;
    node *rightUp = nullptr;
    node *rightDown = nullptr;
    node *leftDown = nullptr;
} *ROOT;

void update(node *&root, int value, int x, int y, int lx = 1, int rx = m, int ly = 1, int ry = n) {
    if (root == nullptr)root = new node;
    root->data += value;
    if (lx == rx && ly == ry)return;
    if (x <= midx) {
        if (y <= midy) update(root->leftUp, value, x, y, lx, midx, ly, midy);
        else update(root->leftDown, value, x, y, lx, midx, midy + 1, ry);
    } else {
        if (y <= midy) update(root->rightUp, value, x, y, midx + 1, rx, ly, midy);
        else update(root->rightDown, value, x, y, midx + 1, rx, midy + 1, ry);
    }
}

int answer(node *&root, int startx, int endx, int starty, int endy, int lx = 1, int rx = m, int ly = 1, int ry = n) {
    if (root == nullptr)return 0;
    if ((rx < startx || endx < lx) || (ry < starty || endy < ly)) return 0;
    if ((startx <= lx && rx <= endx) && (starty <= ly && ry <= endy)) return root->data;
    return answer(root->leftUp, startx, endx, starty, endy, lx, midx, ly, midy) +
           answer(root->leftDown, startx, endx, starty, endy, lx, midx, midy + 1, ry) +
           answer(root->rightUp, startx, endx, starty, endy, midx + 1, rx, ly, midy) +
           answer(root->rightDown, startx, endx, starty, endy, midx + 1, rx, midy + 1, ry);
}

int main() {
    cin >> n >> m;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            cin >> k;
            update(ROOT, k, j, i);
        }
    }
    cin >> q;
    while (q--) {
        cin >> X1 >> Y1 >> X2 >> Y2;
        cout << answer(ROOT, X1, X2, Y1, Y2) << endl;
    }
}
//test:
// 9 12
// 1 2 3 4 5 6 7 8 9 10 11 12
// 1 2 3 4 5 6 7 8 9 10 11 12
// 1 3 4 5 7 6 1 2 8 3 9 1
// 8 8 1 3 3 7 1 4 6 1 3 5
// 4 4 2 3 4 8 1 4 5 1 4 5
// 2 2 3 7 9 9 1 4 4 3 5 4
// 1 1 4 9 12 9 1 4 3 4 6 4
// 3 3 5 12 3 8 1 3 3 5 4 3
// 5 5 6 2 3 7 2 3 2 6 2 1
// 3
// 2 3 5 6
// 8 7 10 8
// 4 6 8 6