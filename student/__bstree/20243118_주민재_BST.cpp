// 입출력 x

/*
    화일 처리 BST Report

    대부분의 요구사항은 c++의 OOP를 살리기 위해 메서드 형태로 작성되었습니다.
    
    요구사항에 적힌 함수 원본 이름은 주석으로 달아두었습니다.
*/
#include <iostream>
#include <stack>


// key type define
using KeyType = int;


// Node class
class Node {

// Member value
private:
    int key;

    Node* left; //left Node
    Node* right; // right node
    
    int height;

    int size;

// Constructor
/*
    원본: getNodeBST()
    getNodeBST() 대신 C++ 의 생성자와 new 키워드 사용
*/
public:
    Node(KeyType key, Node* left, Node* right, int height, int size): 
        key(key), left(left), right(right), height(height), size(size) {}

    Node(KeyType key = 0): Node(key, nullptr, nullptr, 0, 0) {}

// Getter Setter
public:
    KeyType getKey() { return key; }
    void setKey(KeyType key) { this->key = key; }
    
    Node* getRight() { return right; }
    void setRight(Node* right) { this->right = right; }
    
    Node* getLeft() { return left; }
    void setLeft(Node* left) { this->left = left; }

    /*
        원본: height(T) 
        
        매번 높이를 계산하고 리턴 받으면 오버헤드 발생
        따라서 Node의 맴버 변수로서 관리함.
        private로 선언했으므로 Getter Setter 사용.
    */
    int getHeight() { return height; }
    void setHeight(int height) { this->height = height; }

    int getSize() { return size; }
    void setSize(int size) { this->size = size; }
};


/*
    원본: minNode(T)
    부모와 최소 값을 함께 리턴 받기 위해 pair 사용
*/
std::pair<Node*, Node*> minNode(Node* root) {
    Node* parent = nullptr;
    Node* p = root;

    // 왼쪽 리프를 만날때까지 반복
    while (p->getLeft() != nullptr) {
        parent = p;
        p = p->getLeft();
    }

    return {p, parent};  // 최솟값 노드, 해당 노드 부모
}


/*
    원본: maxNode(T)
    부모와 최대 값을 함께 리턴받기 위해 pair 사용
*/
std::pair<Node*, Node*> maxNode(Node* root) {
    Node* parent = nullptr;
    Node* p = root;

    // 우측 리프 노드를 만날때까지 반복
    while (p->getRight() != nullptr) {
        parent = p;
        p = p->getRight();
    }

    return {p, parent};  // 최댓값 노드, 해당 노드 부모
}


// BST class
/*
    특정 Node에 root 라벨을 붙인 Node* root 가 아닌 하나의 클래스로써 운영함 
*/
class BST {

// Member value 
private:
    Node* root; // root Node

// Constructor
public:
    BST(Node* root): root(root) {}
    BST(): BST(nullptr) {}

// Getter Setter
public:
    Node* getRoot() { return root; }
    void setRoot(Node* root) { this->root = root; }

// Method
public:

    /*
        원본: insertBST(T, key)
        
        BST 클래스 내부에 root 가 저장되어있으므로 T 값은 필요 없음
        
        재귀 스택을 초과한 케이스를 염두해 stack을 사용한 구현

        exception 라이브러리 사용이 금지되므로 try catch 대신 bool을 사용해 예외 처리
    */
    bool insertBST(KeyType newKey) {
        std::stack<Node*> stack;
        Node* p = root;
        Node* q = nullptr;

        // 삽입할 위치 탐색
        while (p != nullptr) { 
            if (newKey == p->getKey()) return false; // 이미 존하는 key면 예외 발생

            q = p;
            stack.push(q);
            
            // newKey의 대소 관계에 맞게 좌측 혹은 우측 Node로 이동
            if (newKey < p->getKey()) p = p->getLeft();
            else p = p->getRight();
        }

        // 새로운 노드 생성
        Node* newNode = new Node(); // new Node() == getNodeBST()
        newNode->setKey(newKey);

        if (root == nullptr) root = newNode; // root 없을 경우
        else if (newKey < q->getKey()) q->setLeft(newNode); // key가 부모보다 작음
        else q->setRight(newNode); // key가 부모보다 큼

        // stack 해제 (재귀 리턴 과정)
        while (stack.size()) {
            q = stack.top(); stack.pop();
            
            // 자식이 없을 때 높이는 0으로 간주
            int leftHeight = (q->getLeft() == nullptr) ? 
                                        (0) : (q->getLeft()->getHeight());
            int rightHeight = (q->getRight() == nullptr) ? 
                                        (0) : (q->getRight()->getHeight());

            // 높이 갱신
            q->setHeight(std::max(leftHeight, rightHeight) + 1);

            // 자식이 없을때는 노드 개수 0
            int leftSize = (q->getLeft() == nullptr) ? 0 : q->getLeft()->getSize();
            int rightSize = (q->getRight() == nullptr) ? 0 : q->getRight()->getSize();
            
            // 자식 개수 갱신
            q->setSize(leftSize + rightSize + 1);
        }

        return true; // 정상 종료됨
    }

    /*
        원본: eraseBST(T, key)

        BST맴버에 root가 있으므로 T 값 불필요

        재귀의 스택오버플로우 방지를 위한 stack을 사용한 구현

        exception 라이브러리 사용이 금지이므로 bool 리턴값으로 예외 처리
    */
    bool eraseBST(int deleteKey) {
        std::stack<Node*> stack;
        Node* p = root;
        Node* q = nullptr;

        // key 위치 탐색
        while (p != nullptr && deleteKey != p->getKey()) {
            q = p;
            stack.push(q);
            
            // target key 값과 대소 비교 후 좌측 우측 이동
            if (deleteKey < p->getKey()) p = p->getLeft();
            else p = p->getRight();
        }

        // 해당 key가 없을 경우 예외 발생
        if (p == nullptr) return false;

        // degree 2일 때
        if (p->getLeft() != nullptr && p->getRight() != nullptr) {
            Node* tempNode = p;
            stack.push(p);

            // successor, predecessor 를 높이와 노드 개수에 따라 선택 (좌우 균형을 맞추기 위한 용도)
            if (p->getLeft()->getHeight() < p->getRight()->getHeight() || (
                    p->getLeft()->getHeight() == p->getRight()->getHeight() &&
                    p->getLeft()->getSize() < p->getRight()->getSize()
                )
            ) {
                // 우측 최소 값
                auto [p1, p2] = minNode(p->getRight());
                p = p1;
                q = p2;
            } 
            else {
                // 좌측 최댓값 선택
                auto [p1, p2] = maxNode(p->getLeft());
                p = p1;
                q = p2;
            }

            tempNode->setKey(p->getKey()); // 키값 교체
        }

        // degree 1/0 일때
        Node* child = (p->getLeft() != nullptr) ? 
            (p->getLeft()) : (p->getRight());

        if (q == nullptr) { // root 삭제
            root = child;
        } 
        else {
            if (q->getLeft() == p) q->setLeft(child);
            else q->setRight(child);
        }

        delete p; // 목표 삭제

        // 스택 해제
        while (!stack.empty()) {
            q = stack.top(); stack.pop();
            
            // 자식이 없을 때 높이는 0으로 간주
            int leftHeight = (q->getLeft() == nullptr) ? 
                                        (0) : (q->getLeft()->getHeight());
            int rightHeight = (q->getRight() == nullptr) ? 
                                        (0) : (q->getRight()->getHeight());

            // 높이 갱신
            q->setHeight(std::max(leftHeight, rightHeight) + 1);

            // 자식이 없을때는 노드 개수 0
            int leftSize = (q->getLeft() == nullptr) ? 0 : q->getLeft()->getSize();
            int rightSize = (q->getRight() == nullptr) ? 0 : q->getRight()->getSize();
            
            // 자식 개수 갱신
            q->setSize(leftSize + rightSize + 1);
        }

        return true;
    }


    /*
        원본: clear(T)

        BST에 root 값이 있으므로 T 불필요.
        stack으로 리프부터 차근차근 제거
    */
    void clear() {
        if (!root) return; // 루트가 없으면 바로 종료

        std::stack<std::pair<Node*, Node*>> stack;
        std::pair<Node*, Node*> p = { root, nullptr }; // 현재, 그 부모

        stack.push(p); 

        while (stack.size()) {
            p = stack.top();

            // 좌측이 존재하면 좌측부터 내려감
            if (p.first->getLeft() != nullptr) { 
                p.second = p.first;
                p.first = p.second->getLeft();
                
                stack.push(p);
            }
            // 좌측 없을 경우 우측이 있다면 우측으로 내려감
            else if (p.first->getRight() != nullptr) { 
                p.second = p.first;
                p.first = p.second->getRight();
                
                stack.push(p);
            }
            // 리프일 경우 삭제
            else {
                stack.pop();

                // 삭제 대상이 root인 경우 
                if (p.second == nullptr) {} 
                // 삭제 대상이 부모의 좌측인 경우
                else if (p.second->getLeft() == p.first) 
                    p.second->setLeft(nullptr);
                //삭제 대상이 부모의 우측인 경우
                else 
                    p.second->setRight(nullptr);
                
                delete p.first;
            }
        }

        root = nullptr;
    }


public:
    /*
        원본: inorder(T)

        inorder은 출력용 함수이므로 연산자 오버로딩을 통해 구현함
        stack을 사용한 inorder traversal
    */
    friend std::ostream& operator<<(std::ostream& os, const BST& bst) {
        if (bst.root == nullptr) return os;

        std::stack<std::pair<Node*, bool>> stack; // 노드, 좌측 방문여부
        Node* p = bst.root;

        os << "<";

        while (p != nullptr || !stack.empty()) {

            //좌측 서브트리 끝까지 이동
            while (p != nullptr) {
                stack.push({p, false});

                // 자식이 있으면 < 출력
                if (p->getLeft()) os << "<";
                
                p = p->getLeft();
            }

            // 체커 갱신을 위해 레퍼런스 사용
            auto& [node, leftChecker] = stack.top(); 

            // 좌측 서브트리를 방문한적 없음
            if (!leftChecker) {
                os << " " << node->getKey() << " ";
        
                leftChecker = true;
        
                // 우측이 존재하면 우측으로 이동
                if (node->getRight()) {
                    os << "<";
                    p = node->getRight();
                } 
                // 우측 서브트리가 없을 시 해당 트리 출력 종료
                else {
                    os << ">";
                    stack.pop();
                }
            } 
            // 좌측을 방문한적 있음
            else {
                os << ">";
                stack.pop();
            }
        }

        return os;
    }

public:
    /*
        clear을 까먹었을 시를 대비한 소멸자
    */
    ~BST() {
        this->clear();
    }
};





int main() {
    BST bst; // 문제에 사용할 BST

    char cmd; // 쿼리
    KeyType key; // 쿼리에 대한 key

    // 입력 개수 제한이 없으므로 cin의 리턴값을 사용해 반복
    while (std::cin >> cmd >> key) {

        // insert 쿼리
        if (cmd == 'i') {
            if (!bst.insertBST(key)) // insert의 리턴이 false면 예외
                std::cerr << "i " << key << ": The key already exists" << std::endl;
            else // 정상 출력
                std::cout << bst << std::endl; 
        }
        // erase 쿼리
        else {
            if (!bst.eraseBST(key)) //erase의 리턴이 false면 예외 
                std::cerr << "d " << key << ": The key does not exist" << std::endl;
            else // 정상 출력
                std::cout << bst << std::endl;
            
        }
    }

    // Node가 차지하는 heap 해제
    bst.clear();

    return 0;
}