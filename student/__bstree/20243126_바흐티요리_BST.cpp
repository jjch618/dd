#include <iostream>
using namespace std;

template <class _Tp> class Node;
template <class _Tp> class BST;

// ---------------- Node 클래스 (트리의 각 노드를 나타냄0----------------
template <class _Tp>
class Node {
public:
	typedef _Tp	__key_type;              // 데이터의 자료형 (예: int, string 등)
	typedef Node<__key_type>* __node_pointer_type;     // Node를 가리키는 포인터 타입
	typedef std::size_t	size_type;               // 크기나 높이를 저장할 때 사용하는 타입

public: // 트리에서 자주 쓰는 변수들은 밑에 있음.
	__key_type	__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;
	size_type	__height_;
	size_type __size_;

public:
	// 기본 생성자: 빈 노드 생성
	Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}
	// 값(키)을 전달받아 노드 생성
	Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

	// 출력 연산자 오버로딩 (트리 전체를 중위 순회로 출력)
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (!np) return os;
		os << np->__left_;
		os << np->__key_ << " ";
		os << np->__right_;
		return os;
	}
};

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (!__x) return 0;
	return __x->__height_;
}


template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (!__x) return 0;
	return __x->__size_;
}


template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if (!__x) return nullptr;
	while (__x->__left_) __x = __x->__left_;
	return __x;
}

// 트리에서 최대값(가장 오른쪽)
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (!__x) return nullptr;
	while (__x->__right_) __x = __x->__right_;
	return __x;
}

// 노드의 높이와 크기를 갱신
template <class _NodePtr>
void __update(_NodePtr node) {
	if (!node) return;
	unsigned left = __height(node->__left_);
	unsigned right = __height(node->__right_);
	node->__height_ = 1 + (left > right ? left : right);
	node->__size_ = 1 + __size(node->__left_) + __size(node->__right_);
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (!__x) return;
	cout << "<";
	__inorder(__x->__left_);
	cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
	cout << ">";
}

template <class _Tp>
Node<_Tp>* getNodeBST(const _Tp& key) {
	Node<_Tp>* newNode = new Node<_Tp>(key);
	return newNode;
}


template <class _NodePtr, class _Tp>
_NodePtr __insertBST(_NodePtr& __x, const _Tp& key, bool& inserted) {
	if (!__x) {                               // 현재 위치가 비어있으면 새 노드 삽입해야 한
		inserted = true;
		__x = getNodeBST<_Tp>(key);
		return __x;
	}
	if (key < __x->__key_) {                  // 더 작은 값이면 왼쪽으로 이동하고
		__insertBST(__x->__left_, key, inserted);
	}
	else if (key > __x->__key_) {             // 더 큰 값이면 오른쪽으로 이동해야합니
		__insertBST(__x->__right_, key, inserted);
	}
	else {
		inserted = false;
		return __x;
	}

	__update(__x);
	return __x;
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __x, const _Tp& key, bool& erased) {
	if (!__x) {                               // 노드가 없으면 삭제 불가
		erased = false;
		return nullptr;
	}
	if (key < __x->__key_) {                  // 키가 작으면 왼쪽으로 이동
		__eraseBST(__x->__left_, key, erased);
	}
	else if (key > __x->__key_) {             // 키가 크면 오른쪽으로 이동
		__eraseBST(__x->__right_, key, erased);
	}
	else {
		erased = true;
		// 1) 자식이 없는 경우
		if (!__x->__left_ && !__x->__right_) {
			delete __x;
			__x = nullptr;
			return nullptr;
		}
		// 2) 오른쪽 자식만 있는 경우
		else if (!__x->__left_) {
			Node<_Tp>* tmp = __x->__right_;
			delete __x;
			__x = tmp;
			return __x;
		}
		// 3) 왼쪽 자식만 있는 경우
		else if (!__x->__right_) {
			Node<_Tp>* tmp = __x->__left_;
			delete __x;
			__x = tmp;
			return __x;
		}
		// 4) 두 자식이 모두 있는 경우 (오른쪽 서브트리에서 최소값 찾아 교체)
		else {
			Node<_Tp>* successor = __tree_min(__x->__right_);
			__x->__key_ = successor->__key_;
			__eraseBST(__x->__right_, successor->__key_, erased);
		}
	}
	__update(__x);                            // 삭제 후 높이/크기 갱신
	return __x;
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (!__x) return;
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}

template <class _Tp>
class BST {
public:
	typedef _Tp		key_type;
    typedef std::size_t		size_type;
	typedef Node<key_type>*	pointer;
	typedef const Node<key_type>* const_pointer;

private:
	pointer	__root_;

public:
	BST() : __root_(nullptr) {}
	size_type height() const { return __height(__root_); }   // 트리 높이 반환
	size_type size() const { return __size(__root_); }       // 트리 크기 반환

	void inorder() const {         // 중위 순회 출력
		__inorder(__root_);
		std::cout << std::endl;
	}

	std::pair<const_pointer, bool> insert(const key_type& key) {   // 삽입 함수
		bool inserted = false;
		pointer result = __insertBST(__root_, key, inserted);
		return std::make_pair(result, inserted);
	}

	bool erase(const key_type& key) {   // 삭제 함수
		bool erased = false;
		__eraseBST(__root_, key, erased);
		return erased;
	}

	void clear() { // 트리 초기화
		__clear(__root_);
	}


	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		__inorder(tree.__root_);
		return os;
	}
};

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	BST<int> tree;      // int 타입의 BST 생성
	char command;    // 명령어 (i = insert, d = delete)
	int key;      // 삽입 또는 삭제할 값


	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i': // 삽입 명령
				if (tree.insert(key).second == false) {
					std::cerr << "i " << key << ": 이미 존재하는 키입니다" << std::endl;
					continue;
				}
				break;
			case (int)'d': // 삭제 명령
				if (tree.erase(key) == false) {
					std::cerr << "d " << key << ": 존재하지 않는 키입니다" << std::endl;
					continue;
				}
				break;
			default: // 잘못된 명령
				std::cerr << "잘못된 명령입니다: " << command << std::endl;
				return (1);
		}
		std::cout << tree << std::endl; // 트리 출력
	}
	tree.clear();                      // 프로그램 종료 전 트리 메모리 해제
	return (0);
}
