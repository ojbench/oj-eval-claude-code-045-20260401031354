#include <cstdio>
#include <iostream>
using namespace std;

const int MAXN = 500010;

// Min heap for floors above current position (for upward direction - we want nearest, i.e., smallest)
struct MinHeap {
    int arr[MAXN];
    int cnt;

    MinHeap() : cnt(0) {}

    void push(int x) {
        arr[++cnt] = x;
        int i = cnt;
        while (i > 1) {
            int parent = i >> 1;
            if (arr[parent] > arr[i]) {
                swap(arr[parent], arr[i]);
                i = parent;
            } else {
                break;
            }
        }
    }

    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        int i = 1;
        while (i * 2 <= cnt) {
            int left = i * 2;
            int right = i * 2 + 1;
            int minChild = left;
            if (right <= cnt && arr[right] < arr[left]) {
                minChild = right;
            }
            if (arr[i] > arr[minChild]) {
                swap(arr[i], arr[minChild]);
                i = minChild;
            } else {
                break;
            }
        }
    }

    int top() {
        return cnt > 0 ? arr[1] : -1;
    }

    bool empty() {
        return cnt == 0;
    }

    void remove(int x) {
        // Find and remove element x
        int pos = -1;
        for (int i = 1; i <= cnt; i++) {
            if (arr[i] == x) {
                pos = i;
                break;
            }
        }
        if (pos == -1) return;

        arr[pos] = arr[cnt--];

        // Bubble up
        while (pos > 1) {
            int parent = pos >> 1;
            if (arr[parent] > arr[pos]) {
                swap(arr[parent], arr[pos]);
                pos = parent;
            } else {
                break;
            }
        }

        // Bubble down
        while (pos * 2 <= cnt) {
            int left = pos * 2;
            int right = pos * 2 + 1;
            int minChild = left;
            if (right <= cnt && arr[right] < arr[left]) {
                minChild = right;
            }
            if (arr[pos] > arr[minChild]) {
                swap(arr[pos], arr[minChild]);
                pos = minChild;
            } else {
                break;
            }
        }
    }
};

// Max heap for floors below current position (for downward direction - we want nearest, i.e., largest)
struct MaxHeap {
    int arr[MAXN];
    int cnt;

    MaxHeap() : cnt(0) {}

    void push(int x) {
        arr[++cnt] = x;
        int i = cnt;
        while (i > 1) {
            int parent = i >> 1;
            if (arr[parent] < arr[i]) {
                swap(arr[parent], arr[i]);
                i = parent;
            } else {
                break;
            }
        }
    }

    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        int i = 1;
        while (i * 2 <= cnt) {
            int left = i * 2;
            int right = i * 2 + 1;
            int maxChild = left;
            if (right <= cnt && arr[right] > arr[left]) {
                maxChild = right;
            }
            if (arr[i] < arr[maxChild]) {
                swap(arr[i], arr[maxChild]);
                i = maxChild;
            } else {
                break;
            }
        }
    }

    int top() {
        return cnt > 0 ? arr[1] : -1;
    }

    bool empty() {
        return cnt == 0;
    }

    void remove(int x) {
        // Find and remove element x
        int pos = -1;
        for (int i = 1; i <= cnt; i++) {
            if (arr[i] == x) {
                pos = i;
                break;
            }
        }
        if (pos == -1) return;

        arr[pos] = arr[cnt--];

        // Bubble up
        while (pos > 1) {
            int parent = pos >> 1;
            if (arr[parent] < arr[pos]) {
                swap(arr[parent], arr[pos]);
                pos = parent;
            } else {
                break;
            }
        }

        // Bubble down
        while (pos * 2 <= cnt) {
            int left = pos * 2;
            int right = pos * 2 + 1;
            int maxChild = left;
            if (right <= cnt && arr[right] > arr[left]) {
                maxChild = right;
            }
            if (arr[pos] < arr[maxChild]) {
                swap(arr[pos], arr[maxChild]);
                pos = maxChild;
            } else {
                break;
            }
        }
    }
};

MinHeap upHeap;   // Floors above current position (min heap for nearest)
MaxHeap downHeap; // Floors below current position (max heap for nearest)

int currentFloor = 0;
bool goingUp = true; // true = up, false = down

void add(int x) {
    if (x > currentFloor) {
        upHeap.push(x);
    } else {
        downHeap.push(x);
    }
}

void cancel(int x) {
    if (x > currentFloor) {
        upHeap.remove(x);
    } else {
        downHeap.remove(x);
    }
}

void action() {
    if (goingUp) {
        if (!upHeap.empty()) {
            // Go to nearest floor above
            currentFloor = upHeap.top();
            upHeap.pop();
        } else if (!downHeap.empty()) {
            // No more floors above, reverse direction
            goingUp = false;
            currentFloor = downHeap.top();
            downHeap.pop();
        }
        // else: no requests, stay still
    } else { // going down
        if (!downHeap.empty()) {
            // Go to nearest floor below
            currentFloor = downHeap.top();
            downHeap.pop();
        } else if (!upHeap.empty()) {
            // No more floors below, reverse direction
            goingUp = true;
            currentFloor = upHeap.top();
            upHeap.pop();
        }
        // else: no requests, stay still
    }
}

void locate() {
    printf("%d\n", currentFloor);
}

int main() {
    int n;
    scanf("%d", &n);

    char op[10];
    for (int i = 0; i < n; i++) {
        scanf("%s", op);
        if (op[0] == 'a' && op[1] == 'd') { // add
            int x;
            scanf("%d", &x);
            add(x);
        } else if (op[0] == 'c') { // cancel
            int x;
            scanf("%d", &x);
            cancel(x);
        } else if (op[0] == 'l') { // locate
            locate();
        } else { // action
            action();
        }
    }

    return 0;
}
