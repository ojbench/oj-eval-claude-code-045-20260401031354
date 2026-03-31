#include <cstdio>
#include <iostream>
using namespace std;

const int MAXN = 500010;
const int HASHSIZE = 1000003;  // Prime number for better hashing

// Simple hash map to track active request count for each floor
struct HashMap {
    int keys[HASHSIZE];
    int counts[HASHSIZE];  // Count of active requests for this floor
    bool used[HASHSIZE];

    HashMap() {
        for (int i = 0; i < HASHSIZE; i++) {
            used[i] = false;
            counts[i] = 0;
        }
    }

    int hash(int key) {
        return ((key % HASHSIZE) + HASHSIZE) % HASHSIZE;
    }

    void increment(int key) {
        int h = hash(key);
        while (used[h] && keys[h] != key) {
            h = (h + 1) % HASHSIZE;
        }
        if (!used[h]) {
            keys[h] = key;
            counts[h] = 0;
            used[h] = true;
        }
        counts[h]++;
    }

    void decrement(int key) {
        int h = hash(key);
        int start = h;
        while (used[h]) {
            if (keys[h] == key) {
                counts[h]--;
                return;
            }
            h = (h + 1) % HASHSIZE;
            if (h == start) break;
        }
    }

    bool isActive(int key) {
        int h = hash(key);
        int start = h;
        while (used[h]) {
            if (keys[h] == key) {
                return counts[h] > 0;
            }
            h = (h + 1) % HASHSIZE;
            if (h == start) break;
        }
        return false;
    }
} requestCount;

// Min heap for floors above current position
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
        while (cnt > 0 && !requestCount.isActive(arr[1])) {
            pop();
        }
        return cnt > 0 ? arr[1] : -1;
    }

    bool empty() {
        while (cnt > 0 && !requestCount.isActive(arr[1])) {
            pop();
        }
        return cnt == 0;
    }
};

// Max heap for floors below current position
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
        while (cnt > 0 && !requestCount.isActive(arr[1])) {
            pop();
        }
        return cnt > 0 ? arr[1] : -1;
    }

    bool empty() {
        while (cnt > 0 && !requestCount.isActive(arr[1])) {
            pop();
        }
        return cnt == 0;
    }
};

MinHeap upHeap;
MaxHeap downHeap;

int currentFloor = 0;
bool goingUp = true;

void add(int x) {
    requestCount.increment(x);
    if (x > currentFloor) {
        upHeap.push(x);
    } else {
        downHeap.push(x);
    }
}

void cancel(int x) {
    requestCount.decrement(x);
}

void action() {
    if (goingUp) {
        if (!upHeap.empty()) {
            currentFloor = upHeap.top();
            upHeap.pop();
            requestCount.decrement(currentFloor);  // Decrement when visiting
        } else if (!downHeap.empty()) {
            goingUp = false;
            currentFloor = downHeap.top();
            downHeap.pop();
            requestCount.decrement(currentFloor);  // Decrement when visiting
        }
    } else {
        if (!downHeap.empty()) {
            currentFloor = downHeap.top();
            downHeap.pop();
            requestCount.decrement(currentFloor);  // Decrement when visiting
        } else if (!upHeap.empty()) {
            goingUp = true;
            currentFloor = upHeap.top();
            upHeap.pop();
            requestCount.decrement(currentFloor);  // Decrement when visiting
        }
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
        if (op[0] == 'a' && op[1] == 'd') {
            int x;
            scanf("%d", &x);
            add(x);
        } else if (op[0] == 'c') {
            int x;
            scanf("%d", &x);
            cancel(x);
        } else if (op[0] == 'l') {
            locate();
        } else {
            action();
        }
    }

    return 0;
}
