#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

struct Node {
public:
    Node* left, * right;
    int key;
    int height;
    int size;
};

Node* getNodeBST(int newkey) {
    Node* newNode = new Node();
    newNode->key = newkey;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    newNode->size = 1;
    return newNode;
}

// height
int height(Node* p) {
    if (p == nullptr)
        return 0;
    return p->height;
}

// size
int size(Node* p) {
    if (p == nullptr)
        return 0;
    return p->size;
}

// minNode
Node* minNode(Node* p, stack<Node*>& stack) {
    while (p->left != nullptr) {
        stack.push(p);
        p = p->left;
    }
    return p;
}

// maxNode
Node* maxNode(Node* p, stack<Node*>& stack) {
    while (p->right != nullptr) {
        stack.push(p);
        p = p->right;
    }
    return p;
}


int insertBST(Node*& T, int newKey) {
    Node* p = T; // 루트노드를 받아옴
    Node* q = nullptr; // 부모노드를 임시저장 
    stack<Node*> stack; // 부모노드를 계속 저장

    // searchNode()
    while (p != nullptr) {
        // 이미 같은 키의 노드가 존재하는 경우
        if (newKey == p->key) {
            cerr << "i " << newKey << ": The key already exists\n";
            return 0;
        }

        q = p;
        stack.push(q);

        if (newKey < p->key) {
            p = p->left;
        }
        else {
            p = p->right;
        }
    }

    // 새로운 노드 생성
    Node* newNode = getNodeBST(newKey);

    // 루트노드가 비었을 경우
    if (T == nullptr)
        T = newNode;

    else if (newKey < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    // Height, Size update
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();
        
        int leftHeight = height(q->left);
        int rightHeight = height(q->right);
        q->height = 1 + max(leftHeight, rightHeight);

        q->size = 1 + size(q->left) + size(q->right);
    }
    return true;
}

int eraseBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
    stack<Node*> stack;

    // searchNode
    while (p != nullptr and deleteKey != p->key) {
        q = p;
        stack.push(q);

        if (deleteKey < p->key)
            p = p->left;
        else
            p = p->right;
    }

    // 삭제할 노드가 없는 경우
    if (p == nullptr) {
        cerr << "d " << deleteKey << ": The key does not exist\n";
        return 0;
    }
    // 타겟노드의 양옆 확인
    if (p->left != nullptr and p->right != nullptr) { // 차수가 2
        stack.push(p);
        Node* tempNode = p;
        

        if (height(p->left) < height(p->right) or (height(p->left) == height(p->right) and size(p->left) < size(p->right)))
        {
            p = minNode(p->right, stack); // p->right 서브트리에서 minNode를 찾기
        }
        else {
            p = maxNode(p->left, stack); // p->left 서브트리에서 maxNode를 찾기
        }

        tempNode->key = p->key;
        q = stack.top();
    }

    if (p->left == nullptr and p->right == nullptr) {  // 차수가 0
        if (q == nullptr)
            T = nullptr;
        else if (q->left == p)
            q->left = nullptr;
        else
            q->right = nullptr;
    }
    else { // 차수가 1
        if (p->left != nullptr) {
            if (q == nullptr)
                T = T->left;
            else if (q->left == p)
                q->left = p->left;
            else
                q->right = p->left;
        }
        else {
            if (q == nullptr)
                T = T->right;
            else if (q->left == p)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    //메모리 누수 방지
    delete p;

    // Height, Size update
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();

        int leftHeight = height(q->left);
        int rightHeight = height(q->right);
        q->height = 1 + max(leftHeight, rightHeight);

        q->size = 1 + size(q->left) + size(q->right);
    }
    return true;
}


// 순회 출력
void inorder(Node* T) {
    // 빈 트리
    if (T == nullptr) {
        cout << "";
        return;
    }
    cout << "<";
    // 왼쪽
    if (T->left != nullptr)
        inorder(T->left);
    // 현재 노드
    cout << " " << T->key << " ";
    // 오른쪽
    if (T->right != nullptr)
        inorder(T->right);
    cout << ">";
}

void clear(Node* T) {
    if (T == nullptr)
        return;
    clear(T->left);
    clear(T->right);

    delete T;

}

//main 함수
int main() {
    char com;
    int key;
    Node* T = nullptr;
    while (cin >> com >> key) {
        if (com == 'i') {
            if (insertBST(T, key)) {
                inorder(T);
                cout << "\n";
            }
        }
        else if (com == 'd') {
            if (eraseBST(T, key)) {
                inorder(T);
                cout << "\n";
            }
        }
    }
    clear(T);
    return (0);
}