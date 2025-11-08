#include <iostream>
#include <stack>
using namespace std;

template <class _Tp> class Node;
template <class _Tp> class BST;

/*        Node 클래스
 트리의 각 노드를 표현하는 클래스예요.
 - __key_  : 노드의 키 값
 - __left_  : 왼쪽 자식 노드 포인터
 - __right_: 오른쪽 자식 노드 포인터
 - __height_: 현재 노드를 루트로 하는 서브트리의 높이
 - __size_ : 현재 노드를 루트로 하는 서브트리의 노드 개수 */
template <class _Tp>
class Node {
	public:
		typedef _Tp					__key_type;
		typedef Node<__key_type>*	__node_pointer_type;
        typedef std::size_t size_type; 

	public:
        __key_type			__key_;     // 노드 키 값
        __node_pointer_type	__left_;    // 왼쪽 자식 노드
        __node_pointer_type	__right_;   // 오른쪽 자식 노드
        size_type __height_;            // 서브트리 높이
        size_type __size_;              // 서브트리 노드 개수
	
	public:
		// 기본 생성자: 모든 포인터는 nullptr, 높이와 크기는 1로 초기화
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

		// 키 값이 주어진 경우의 생성자
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

		// 중위 순회 출력용 연산자 오버로딩
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
        if(!np) return os;
        os << np->__left_;
        os << np->__key_ << " ";
        os << np->__right_;
        return os;
	}
};

//        함수들: 

// 어떤 노드의 높이를 반환
template <class _NodePtr>
unsigned __height(_NodePtr temuujin) {
    if (!temuujin) return 0;
    return temuujin->__height_; 
}
// 어떤 노드를 루트로 하는 서브트리의 노드 개수 반환
template <class _NodePtr>
unsigned __size(_NodePtr temuujin) {
    if(!temuujin) return 0;
    return temuujin->__size_; 
}

// 트리의 최소값 
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr temuujin){
    if(!temuujin) return nullptr;
    while(temuujin->__left_) temuujin = temuujin->__left_;
    return temuujin;
}

// 최대값 노드 탐색
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr temuujin){
    if(!temuujin) return nullptr;
    while(temuujin->__right_) temuujin = temuujin->__right_;
    return temuujin;
}


// 트리의 노드들의 값을 출력힙니다.
template <class _NodePtr>
void __inorder(_NodePtr temuujin) {
    if (!temuujin) return;
    cout << "<";
    if(temuujin -> __left_) __inorder(temuujin -> __left_);
    cout << " " << temuujin -> __key_ << " ";
    if(temuujin -> __right_) __inorder(temuujin -> __right_);

    cout << ">";  
}

// 새로운 노드를 동적 할당하여 반환합니다.
template <class _Tp>
Node<_Tp>* getNodeBST() {
    Node<_Tp>* newNode = new Node<_Tp>();
    return newNode;
}

/*
        __insertBST() 함수.
 새로운 키(newKey)를 BST에 삽입합니다.

 알고리즘 절차:
 1. 루트부터 탐색하며 삽입 위치를 찾습니다.
 2. 이미 존재하는 키면 false 반환.
 3. 새 노드를 생성하고 부모의 왼쪽 또는 오른쪽에 연결.
 4. 삽입 경로를 따라 올라가며 height, size 갱신.
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __roott, const _Tp& key) {
    std::stack<_NodePtr> stack;
    _NodePtr p = __roott;  // 현재 탐색 중인 노드
    _NodePtr q = nullptr;  // 부모 노드 포인터

    // 삽입 위치 탐색
    while (p != nullptr) {
        if (key == p->__key_){
            return std::make_pair(p, false);
        }  // 중복 키 존재하니까 노드 삽입 실패.
        q = p;
        stack.push(q);
        if (key < p->__key_) p = p->__left_;
        else p = p->__right_;
    }
    // 새로운 노드 생성하고 트리랑 연결하고자 합니다.
    _NodePtr newNode = getNodeBST<_Tp>();
    newNode->__key_ = key;

    // 부모와 연결
    if (__roott == nullptr) __roott = newNode;  /// 트리가 비어 있다면, 루트로 만든다.
    else if (key < q->__key_) {
        q->__left_ = newNode;
    } else {
        q->__right_ = newNode;
    }
    // 부모 방향으로 height, size 갱신
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();
        // 노드의 높이와 사이즈를 update하고 있다.
        unsigned leftt = __height(q->__left_);
        unsigned rightt = __height(q->__right_);
        q->__height_ = 1 + (leftt > rightt ? leftt : rightt);
        q->__size_   = 1 + __size(q->__left_) + __size(q->__right_);
    }
    return std::make_pair(newNode, true);  // 노드 삽입 성공.
}

/*     __eraseBST() 함수.

 키(key)를 가진 노드를 BST에서 삭제합니다. (BST-pseudo.txt에 있는 것을 따하했음.)
 1. 루트에서부터 key 탐색
 2. 노드가 존재하지 않으면 nullptr 반환
 3. 삭제할 노드가
     - 자식이 0개 → 그냥 삭제해 버림.
     - 자식이 1개 → 자식을 부모에 직접 연결
     - 자식이 2개 → 높이, 크기를 기준으로 대체 노드 선택 후 삭제
 4. 스택에 저장된 부모 경로를 따라 height, size 갱신 */
template <class _NodePtr, class _Tp>
bool __eraseBST(_NodePtr& __root, const _Tp& key) {
    std::stack<_NodePtr> stack;
    _NodePtr p = __root;   // 현재 탐색 노드
    _NodePtr q = nullptr;   /// 부모 노드

    // 1. 삭제할 노드 탐색
    while (p != nullptr && key != p->__key_) {
        q = p;
        stack.push(q);
        if (key < p->__key_) p = p->__left_;
        else p = p->__right_;
    }
    if(p == nullptr) return false;  // 존재하지 않음

    // 2. 자식이 둘 다 있을 경우 (이럴 때는 대체할 노드를 탐색해야 합니다.)
    if (p->__left_ != nullptr && p->__right_ != nullptr) {
        _NodePtr tempNode = p;  // 삭제 대상 노드..
        stack.push(p);
        if (__height(p->__left_) < __height(p->__right_) ||
        (__height(p->__left_) == __height(p->__right_) &&
            __size(p->__left_) < __size(p->__right_))) {
            // 오른쪽 서브트리에서 successor 탐색
            p = p->__right_;
            while (p->__left_ != nullptr) {
                stack.push(p);
                p = p->__left_;
            }
        } else {
            // 왼쪽 서브트리에서 predecessor 탐색
            p = p->__left_;
            while (p->__right_ != nullptr) {
                stack.push(p);
                p = p->__right_;
            }
        }
        tempNode->__key_ = p->__key_; // 키 값 대체. --> 삭제 하는 것과 동일하죠.
        q = stack.top();
    }

    // 3. 실제 삭제 처리
    if (p->__left_ != nullptr) { 
        // 왼쪽 자식만 있는 경우
        if (q == nullptr){  __root = p->__left_;}
        else if (q->__left_ == p) q->__left_ = p->__left_;
        else q->__right_ = p->__left_;
    } 
    else if (p->__right_ != nullptr) { 
        // 오른쪽 자식만 있는 경우
        if (q == nullptr) __root = p->__right_; 
        else if (q->__left_ == p) {
            q->__left_ = p->__right_;
        }
        else q->__right_ = p->__right_;
    } 
    else { 
        if (q == nullptr) __root = nullptr; 
        else if (q->__left_ == p) {
            q->__left_ = nullptr;}
        else q->__right_ = nullptr;
    }
    delete p;  // 여기서 드디어 진짜로 삭제함.

    // 4. height, size 갱신
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();

        // 노드의 높이와 사이즈를 update하고 있다.
        unsigned lefttt = __height(q->__left_);
        unsigned righttt = __height(q->__right_);
        q->__height_ = 1 + (lefttt > righttt ? lefttt : righttt);
        q->__size_   = 1 + __size(q->__left_) + __size(q->__right_);
    }
    return true;
}

/*     __clear() 함수.
 트리 전체를 후위 순회 방식으로 재귀 삭제.
 모든 노드를 동적 해제하여 메모리 누수 방지. */
template <class _NodePtr>
void __clear(_NodePtr& temuujin) {
    if(!temuujin) return;
    __clear(temuujin->__left_);
    __clear(temuujin->__right_);
    delete temuujin;
    temuujin = nullptr;
}

/*        BST 클래스
 - 삽입, 삭제, 탐색, 순회 기능 제공
 - 내부적으로 루트 포인터(__root_) 관리 */
template <class _Tp>
class BST {
public:
    typedef _Tp						key_type;
    typedef std::size_t				size_type;
    typedef Node<key_type>*			pointer;
    typedef const Node<key_type>*	const_pointer;

    private:
        pointer	__root_;

public:
    BST(): __root_(nullptr) {}

	// 트리의 높이 반환
	size_type height() const {
		return __height(__root_);
	}

	// 트리의 전체 노드 수 반환
	size_type size() const {
		return __size(__root_);
	}
	// 중위 순회 출력
	void inorder() const {
		__inorder(__root_);
		std::cout << std::endl;
	}
		
	// 노드 삽입
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertBST(__root_, key);
	}
	// 노드 삭제
    bool erase(const key_type& key) {
        return __eraseBST(__root_, key);
    }

	// 전체 트리 삭제
	void clear() {
		__clear(__root_);
	}
	// << 연산자로 트리 출력
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		__inorder(tree.__root_);
		return os;
	}
};

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
	BST<int>	tree;
	char		command;
	int			key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key).second == false) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == false) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
		}
		std::cout << tree << std::endl;
	}

	tree.clear(); // 프로그램 종료 전 메모리 해제
	return (0);
}
///