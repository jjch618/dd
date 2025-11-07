#include <iostream>

struct Node {
    int   key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {} // 노드 생성자: 키만 채우고, 자식은 비워둠
};

// 새 노드 함수
static Node* getNodeBST(int key) {
    Node* y = new Node(key); // 동적할당
    return y; // 포인터 반환
}

// 높이 함수
static int height(Node* p) {
    if (!p) return 0; // 비었으면 0 
    int hl = height(p->left); // 왼쪽 서브트리 높이
    int hr = height(p->right); // 오른쪽 서브트리 높이
    return (hl > hr ? hl : hr) + 1; // 더 큰 쪽에 자기 자신(+1) 추가
}

// 서브트리 노드 개수 함수
static int size(Node* p) {
    if (!p) return 0;// 공백이면 0
    return 1 + size(p->left) + size(p->right); // 왼+오+자기 
}
// 최소 노드(가장 왼쪽). parent는 r부모
static Node* minNode(Node* p, Node*& parent) {
    parent = nullptr; // 시작 시 부모 없음
    while (p && p->left) { // 왼쪽 끝까지
        parent = p; // 부모 갱신
        p = p->left;
    }
    return p; // 최소 노드 포인터
}

// 최대 노드(가장 오른쪽). parent는 r부모
static Node* maxNode(Node* p, Node*& parent) {
    parent = nullptr; // 시작 시 부모 없음
    while (p && p->right) { // 오른쪽 끝까지
        parent = p; // 부모 갱신
        p = p->right;
    }
    return p; // 최대 노드 포인터
}

// insertBST - 이미 해당 key 값을 가진 노드가 존재할 때 삽입 실패 메세지를 띄우기 위해 bool& 함수인자를 추가함. 큰틀에서 다르지 않다면 변경 허용하신다고 하시길래.. 스켈레톤 코드 속 std::pair 와 고민하다 이 방법 선택
static Node* insertBST(Node* T, int newKey, bool& done) { 
    // 삽입할 자리 찾
    Node* q = nullptr; // 마지막 방문한 부모 찾
    Node* p = T; // 주행 포인터
    while (p != nullptr) { // 빈 자리 만날 때까지
        if (newKey == p->key) { // 중복 키면 삽입 x already exists
            done = false;
            return T; // 변화 없는 트리 반환
        }
        q = p; // 부모 갱신
        if (newKey < p->key) p = p->left; else p = p->right;   
    }

    // 노드 만들기
    Node* y = getNodeBST(newKey);

    // 트리에 연결 — 루트였던 경우도 
    if (T == nullptr) { // 처음 삽입이라면 새 노드가 루트
        T = y;
    } else if (newKey < q->key) { // 부모 왼쪽으로 붙이기
        q->left = y;
    } else { // 부모 오른쪽으로 붙이기
        q->right = y;
    } 
    done = true; // 삽입 성공
    return T;
}

// eraseBST - 삭제하고자 하는 key 값을 가진 노드가 존재하지 않을 때 삭제 실패 메세지를 띄우기 위해 bool& 함수인자를 추가함. 이유는 insertBST 와 동일..
static Node* eraseBST(Node* T, int delKey, bool& done) {
    // 삭제할 키 위치, 부모 찾
    Node* q = nullptr; // 부모
    Node* p = T; // 주행 포인터
    while (p != nullptr && p->key != delKey) {
        q = p;
        if (delKey < p->key) p = p->left; else p = p->right; // 평소처럼 내려감
    }
    if (p == nullptr) { // 없으면 삭제 실패 not exists
        done = false;
        return T;
    }
    done = true; // 실제 삭제로 넘어감

    // 자식 0개
    if (p->left == nullptr && p->right == nullptr) {
        if (q == nullptr) { // 루트 하나짜리 트리였음
            delete p;
            return nullptr; // 전부 비워짐
        }
        if (q->left == p) q->left = nullptr; else q->right = nullptr; // 부모 연결 끊기
        delete p;
        return T;
    }

    // 자식 1개 
    if (p->left == nullptr || p->right == nullptr) {
        Node* child  = (p->left ? p->left : p->right); // 남은 가지 하나
        if (q == nullptr) { // p가 루트였으면 자식이 새 루트
            delete p;
            return child;
        }
        if (q->left == p) q->left = child; else q->right = child; // 자식으로 대체
        delete p;
        return T;
    }

    // 자식 2개
    int hl = height(p->left); // 왼 높이
    int hr = height(p->right); // 오 높이

    Node* s = nullptr; // r의 부모
    Node* r = nullptr; // 실제 제거되는 노드

    if (hl < hr) {
        // 오른쪽 최소
        r = minNode(p->right, s); 
        p->key = r->key; // 키만 이동
        if (!s) p->right = r->right; // r이 p의 바로 오른쪽 자식
        else s->left  = r->right; // r의 오른쪽 가지를 올림
        delete r;
        return T;
    } else if (hl > hr) {
        // 왼쪽 최대
        r = maxNode(p->left, s);
        p->key = r->key; // 키만 이동
        if (!s) p->left = r->left; // r이 p의 바로 왼쪽 자식
        else s->right = r->left; // r의 왼쪽 가지를 올림
        delete r;
        return T;
    } else {
        // 높이가 같으면 사이즈 비교
        int sl = size(p->left);
        int sr = size(p->right);
        if (sl < sr) {
            // 오른쪽 최소
            r = minNode(p->right, s);
            p->key = r->key;
            if (!s) p->right = r->right; else s->left = r->right;
            delete r;
            return T;
        } else {
            // 왼쪽의 사이즈가 크거나 같을 경우도
            r = maxNode(p->left, s);
            p->key = r->key;
            if (!s) p->left = r->left; else s->right = r->left;
            delete r;
            return T;
        }
    }
}

// inorder
static void inorder(Node* p) {
    if (!p) return; // 공백 서브트리는 생략
    std::cout << '<';// 현재 서브트리 시작
    inorder(p->left); // 왼쪽 먼저
    std::cout << ' ' << p->key << ' '; // 키 출력 가운데 
    inorder(p->right); // 그리고 오른쪽
    std::cout << '>'; // 현재 서브트리 끝
}

// 동적 할당에 대한 delete
static void clear(Node* p) {
    if (!p) return; // 비었으면 할 일 없음
    clear(p->left); // 왼쪽
    clear(p->right); // 오른쪽
    delete p; // 마지막에 자기 자신
}

int main() {
    Node* T = nullptr; // 루트(초기엔 공백 트리)
    char cmd;
    int k;

    // 입력: "i 2" / "d 5" ...
    while (std::cin >> cmd >> k) {
        if (cmd == 'i') { // 삽입
            bool done = false;
            T = insertBST(T, k, done);
            if (!done) {
                std::cout << 'i' << ' ' << k << ": The key already exists\n"; // 이미 있음
            } else {
                inorder(T); 
                std::cout << '\n'; // 상태 확인용
            }
        } else if (cmd == 'd') { // 삭제
            bool done = false;
            T = eraseBST(T, k, done);
            if (!done) {
                std::cout << 'd' << ' ' << k << ": The key does not exist\n"; // 없음
            } else {
                if (T) { inorder(T); std::cout << '\n'; } // 남은 트리 출력
                else { std::cout << '\n'; } // 전부 삭제되면 빈 줄만
            }
        }
    }

    clear(T);                     
    return 0; // 종료
}
