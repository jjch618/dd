#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Node 구조체: key, left, right, height, size (알고리즘에서 필요하므로 추가)
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int size;
    Node() : key(0), left(nullptr), right(nullptr), height(0), size(1) {}
};

// 헬퍼 함수들
Node* getNodeBST() {
    return new Node();
}

int height(Node* node) {
    return node ? node->height : 0;
}

int size(Node* node) {
    return node ? node->size : 0;
}

Node* minNode(Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

Node* maxNode(Node* node) {
    while (node && node->right) node = node->right;
    return node;
}

// insertBST 함수
void insertBST(Node*& T, int newKey) {
    Node* p = T;
    Node* q = nullptr;
    stack<Node*> stk;
    while (p) {
        if (newKey == p->key) return; // 이미 존재
        q = p;
        stk.push(q);
        if (newKey < p->key) p = p->left;
        else p = p->right;
    }
    Node* newNode = getNodeBST();
    newNode->key = newKey;
    if (!T) T = newNode;
    else if (newKey < q->key) q->left = newNode;
    else q->right = newNode;
    while (!stk.empty()) {
        q = stk.top(); stk.pop();
        q->height = 1 + max(height(q->left), height(q->right));
        q->size = 1 + size(q->left) + size(q->right);
    }
}

// eraseBST 함수
void eraseBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
    stack<Node*> stk;
    while (p && deleteKey != p->key) {
        q = p;
        stk.push(q);
        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }
    if (!p) return; // 존재하지 않음
    if (p->left && p->right) {
        Node* tempNode = p;
        stk.push(p);
        if (height(p->left) < height(p->right) || (height(p->left) == height(p->right) && size(p->left) < size(p->right))) {
            p = p->right;
            while (p->left) {
                stk.push(p);
                p = p->left;
            }
        } else {
            p = p->left;
            while (p->right) {
                stk.push(p);
                p = p->right;
            }
        }
        tempNode->key = p->key;
        q = stk.top();
    }
    // degree 0 or 1
    Node* child = p->left ? p->left : p->right;
    if (!q) T = child;
    else if (q->left == p) q->left = child;
    else q->right = child;
    delete p;
    while (!stk.empty()) {
        q = stk.top(); stk.pop();
        q->height = 1 + max(height(q->left), height(q->right));
        q->size = 1 + size(q->left) + size(q->right);
    }
}

// inorder 함수: 트리 구조 출력
void inorder(Node* node) {
    if (!node) return;
    cout << "<";
    inorder(node->left);
    cout << " " << node->key << " ";
    inorder(node->right);
    cout << ">";
}

// clear 함수: 메모리 해제
void clear(Node*& T) {
    if (!T) return;
    clear(T->left);
    clear(T->right);
    delete T;
    T = nullptr;
}

int main() {
    Node* T = nullptr;
    string line;
    while (getline(cin, line)) {
        char cmd;
        int key;
        sscanf(line.c_str(), "%c %d", &cmd, &key);
        if (cmd == 'i') {
            // 삽입 전 존재 확인
            Node* check = T;
            bool exists = false;
            while (check) {
                if (key == check->key) { exists = true; break; }
                check = (key < check->key) ? check->left : check->right;
            }
            if (exists) {
                cerr << "i " << key << ": The key already exists" << endl;
            } else {
                insertBST(T, key);
            }
        } else if (cmd == 'd') {
            // 삭제 전 존재 확인
            Node* check = T;
            bool exists = false;
            while (check) {
                if (key == check->key) { exists = true; break; }
                check = (key < check->key) ? check->left : check->right;
            }
            if (!exists) {
                cerr << "d " << key << ": The key does not exist" << endl;
            } else {
                eraseBST(T, key);
            }
        }
        // 각 명령 후 inorder 출력
        inorder(T);
        cout << endl;
    }
    clear(T);
    return 0;
}