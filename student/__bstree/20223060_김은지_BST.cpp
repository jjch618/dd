#include <iostream>
#include <stack>
using namespace std;

class Node {
    public:
        int key;
        Node *left;
        Node *right;
        int height;
};

// 서브트리의 높이 계산
int height(const Node *T) {
    if (T == nullptr)
        return -1;

    // 불필요한 함수 호출을 줄이기 위해 height 값 미리 계산 후 변수에 저장
    int l_height = height(T->left);
    int r_height = height(T->right);

    return 1 + (l_height > r_height ? l_height : r_height);
}

// 서브트리의 노드 개수 반환
int size(const Node *T) {
    if (T == nullptr)
        return 0;

    if (T->left == nullptr && T->right == nullptr)
        return 1;

    if (T->left == nullptr)
        return 1 + size(T->right);

    if (T->right == nullptr)
        return 1 + size(T->left);

    return 1 + size(T->left) + size(T->right);
}

// (오른쪽 노드의) 가장 왼쪽으로 내려가서 최소 키를 가진 노드 찾음
Node * minNode(Node *T) {
    while (T->left != nullptr)
        T = T->left;

    return T;
}

// (왼쪽 노드의) 가장 오른쪽으로 내려가서 최대 키를 가진 노드 찾음
Node * maxNode(Node *T) {
    while (T->right != nullptr)
        T = T->right;

    return T;
}

// 새로운 노드 할당 및 기본 초기화
Node * getNodeBST() {
    Node *newNode = new Node;

    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 0;

    return newNode;
}

// 노드 삽입 함수 (BST-pseudo.txt 기반)
bool insertBST(Node *&T, const int newKey) {
    Node *p = T;
    Node *q = nullptr;
    stack<Node *> s; // 삽입 경로 저장

    // 삽입 해야 할 위치 탐색
    while(p != nullptr) {
        if (newKey == p->key) // 중복 키 일 경우 false 반환
            return false;

        q = p;
        s.push(q);

        if(newKey < p->key) // newKey가 현재 키 보다 작을 경우 왼쪽
            p = p->left;
        else p = p->right; // newKey가 현재 키 보다 클 경우 오른쪽
    }

    // 새로운 노드 생성 및 key값 할당
    Node *newNode = getNodeBST();
    newNode->key = newKey;

    if (T == nullptr) // 빈 트리일 경우
        T = newNode;
    else if (newKey < q->key)
        q->left = newNode;
    else q->right = newNode;

    // 삽입 후 경로에 있던 조상 노드들의 height 계산 및 갱신
    while(!s.empty()) {
        q = s.top();
        s.pop();

        // 불필요한 함수 호출을 줄이기 위해 height 값 미리 계산 후 변수에 저장
        int l_height = height(q->left);
        int r_height = height(q->right);

        // 큰 쪽 기준으로 height 갱신 (같으면 왼쪽 기준)
        q->height = 1 + (l_height < r_height ? r_height : l_height);
    }

    return true; // 삽입 성공 시 true 반환
}

// 노드 삭제 함수 (ppt + BST-pseudo.txt 기반)
// minNode, maxNode 활용을 위해 stack 보다는 재귀형식으로 구현
bool eraseBST(Node *&T, const int deleteKey) {
    Node *p = T;
    Node *q = nullptr;

    while (p != nullptr && deleteKey != p->key) {
        q = p;

        if (deleteKey < p->key)
            p = p->left;
        else p = p->right;
    }

    if (p == nullptr)
        return false; // 삭제할 키가 없으면 false 반환

    if (p->left != nullptr && p->right != nullptr) { // 자식 2개일 경우
        Node *tempNode = nullptr;
        bool flag; // 0:LEFT 1:RIGHT

        int l_height = height(p->left);
        int r_height = height(p->right);

        // 높이가 큰 쪽 선택 (높이 같을 경우 size 큰 쪽, 기본 왼쪽)
        if (l_height < r_height || (l_height == r_height && size(p->left) < size(p->right))) {
            tempNode = minNode(p->right);
            flag = 1;
        } else {
            tempNode = maxNode(p->left);
            flag = 0;
        }

        // 키 복사 후 노드 제거 재귀로
        p->key = tempNode->key;
        if (flag == 0)
            eraseBST(p->left, tempNode->key);
        else eraseBST(p->right, tempNode->key);
    } else { // 자식이 1개나 0개일 경우
        if (p->left != nullptr){ // 자식이 왼쪽 자신 1개인 경우
            if (q == nullptr)
                T = T->left;
            else if (q->left == p)
                q->left = p->left;
            else q->right = p->left;
        } else if (p->right != nullptr) { // 자식이 오른쪽 자신 1개인 경우
            if (q == nullptr)
                T = T->right;
            else if (q->left == p)
                q->left = p->right;
            else q->right = p->right;
        } else { // 자식이 0개일 경우
            if (q == nullptr) // 루트 노드 삭제 -> null
                T = nullptr;
            else if (q->left == p)
                q->left = nullptr;
            else q->right = nullptr;
        }

        delete p; // 메모리 누수를 피하기 위해 p 삭제
    }

    return true; // 삭제 성공 시 true 반환
}

// 형식대로 중위순회 출력
void inorder(const Node *T) {
    if (T != nullptr) {
        cout << "<";
        inorder(T->left);
        cout << " " << T->key << " ";
        inorder(T->right);
        cout << ">";
    }
}

// 초기화 함수
void clear(Node *&T) {
    if (T == nullptr)
        return;

    clear(T->left);
    clear(T->right);

    delete T;
}

int main() {
    Node *Tree = nullptr;
    char command;
    int key;

    while (cin >> command >> key) { // 삽입/삭제 와 key 입력
        if (command == 'i') {
            if (insertBST(Tree, key) == false) { // 중복 키 일 경우
                cout << "i " << key << ": The key already exists" << "\n";
                continue;
            }
        } else if (command == 'd'){
            if (eraseBST(Tree, key) == false) { // 삭제할 키가 없을 경우
                cout << "d " << key << ": The key does not exist" << "\n";
                continue;
            }
        }

        inorder(Tree); // 삽입/삭제가 일어날 때마다 중위순회 출력
        cout << "\n";
    }

    clear(Tree);
    return 0;
}
