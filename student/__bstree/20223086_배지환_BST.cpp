// root가 삭제될 때 케이스 안 다룸 
// 20223086 배지환 화일처리 리포트1


#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;


template <class _Tp>
class Node {
public: // Member types
	typedef _Tp					__key_type;
	typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;
	unsigned height; // 멤버변수 height 추가
	unsigned size; // 멤버변수 size 추가

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(1), size(1) {}  // height size 1로 초기화
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(1), size(1) {} // height size 1로 초기화

};


// height, size update 하는함수
template <class _NodePtr>
void __update(_NodePtr __x) {
	if (__x == nullptr) {
		return;
	}

	unsigned lh = 0;
	unsigned rh = 0;
	unsigned ls = 0;
	unsigned rs = 0;

	if (__x->__left_ != nullptr) {
		lh = __x->__left_->height; // 왼쪽 자식있으면 왼쪽자식높이로
	}

	if (__x->__right_ != nullptr) {
		rh = __x->__right_->height; // 오른쪽 자식있으면 오른쪽자식높이로
	}

	if (__x->__left_ != nullptr) {
		ls = __x->__left_->size; // 왼쪽 자식있으면 왼쪽자식사이즈로
	}

	if (__x->__right_ != nullptr) {
		rs = __x->__right_->size; // 오른쪽 자식있으면 오른쪽자식사이즈로
	}

	__x->height = 1 + (lh > rh ? lh : rh); // 높이 중 큰거선택
	__x->size = 1 + ls + rs; // 사이즈계산
}



// height 구하는 함수
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) {
		return 0;
	}
	unsigned height = 0;
	unsigned left = 0; // 왼쪽 높이
	unsigned right = 0; // 오른쪽 높이
	if (__x->__left_ != nullptr) {
		left = __height(__x->__left_) + 1; // 재귀로 왼쪽 계산
	}
	if (__x->__right_ != nullptr) {
		right = __height(__x->__right_) + 1; // 재귀로 오른쪽 계산
	}
	if ((__x->__left_ == nullptr) && (__x->__right_ == nullptr)) {
		return height;
	}
	height = left < right ? right : left; // 큰거 선택
	return height;
}

// size 구하는 함수
template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) {
		return 0;
	}
	unsigned size = 1;
	unsigned left = 0; // 왼쪽 개수
	unsigned right = 0; // 오른쪽 개수
	if (__x->__left_ != nullptr) {
		left = __size(__x->__left_); // 재귀로 왼쪽 계산
	}
	if (__x->__right_ != nullptr) {
		right = __size(__x->__right_); // 재귀로 오른쪽 계산
	}
	if ((__x->__left_ == nullptr) && (__x->__right_ == nullptr)) {
		return size;
	}
	size += left + right;
	return size;
}

// 노드 동적할당하는 함수
template <class _Tp>
Node<_Tp>* getNodeBST() {
	Node<_Tp>* node = new Node<_Tp>(); 
	node->__left_ = nullptr; // nullptr로 초기화
	node->__right_ = nullptr; // nullptr로 초기화
	node->height = 1; // 높이1로 초기화
	node->size = 1; // 사이즈1로 초기화
	return node;
}



template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if (__x->__left_ != nullptr) {
		return __tree_min(__x->__left_); // 재귀 이용해서 왼쪽으로이동
	}
	else {
		return __x;
	}

}


template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (__x->__right_ != nullptr) {
		return __tree_max(__x->__right_); // 재귀 이용해서 오른쪽으로 이동
	}
	else {
		return __x;
	}
}

// inorder로 출력하는 함수
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) {
		std::cout << "<>";
		return;
	}

	std::cout << "<";

	if (__x->__left_ != nullptr) {
		__inorder(__x->__left_); 
	}

	std::cout << " " << __x->__key_ << " ";


	if (__x->__right_ != nullptr) {
		__inorder(__x->__right_);
	}

	std::cout << ">";
}

// 삽입 함수
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr;

	std::stack<_NodePtr> stack;
	// p이동
	while (p != nullptr) {
		if (key == p->__key_) {
			return std::pair<_NodePtr, bool>(p, false);
		}
		q = p;
		stack.push(q);
		//q stack에 push

		if (key < p->__key_) {
			p = p->__left_; // 왼쪽으로 이동
		}
		else {
			p = p->__right_; // 오른쪽으로 이동
		}
	}
	// 노드 생성
	_NodePtr newNode = getNodeBST<_Tp>();
	newNode->__key_ = key;


	// q의자식으로 노드삽입
	if (__root == nullptr) {
		__root = newNode;
	}
	else if (key < q->__key_) {
		q->__left_ = newNode; // 왼쪽자식에삽입
	}
	else {
		q->__right_ = newNode; // 오른쪽 자식에 삽입
	}

	while (!stack.empty()) {
		q = stack.top();
		stack.pop();
		__update(q); // 업데이트(height랑 size)
	}

	return std::pair<_NodePtr, bool>(newNode, true);
}


template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr p = __root;
	_NodePtr q = nullptr;

	std::stack<_NodePtr> stack;

	// p이동
	while ((p != nullptr) && (key != p->__key_)) {
		q = p;
		stack.push(q);

		if (key < p->__key_) {
			p = p->__left_; // 왼쪽으로 이동
		}
		else {
			p = p->__right_; // 오른쪽으로 이동
		}
	}
	// 못 찾은 경우
	if (p == nullptr) {
		return nullptr; 
	}

	// 자식 2개인 경우
	if ((p->__left_ != nullptr) && (p->__right_ != nullptr)) {
		_NodePtr tempNode = p;
		stack.push(p);

		if ((p->__left_->height < p->__right_->height) || ((p->__left_->height == p->__right_->height) && (p->__left_->size < p->__right_->size))) {
			p = p->__right_;
			while (p->__left_ != nullptr) {
				stack.push(p);
				p = p->__left_;
			}
		}

		else {
			p = p->__left_;
			while (p->__right_ != nullptr) {
				stack.push(p);
				p = p->__right_;
			}
		}

		tempNode->__key_ = p->__key_;
		q = stack.top();
	}

	// 왼쪽 자식만 있는경우
	if (p->__left_ != nullptr) {
		if (q == nullptr) {
			__root = __root->__left_;
		}
		else if (q->__left_ == p) {
			q->__left_ = p->__left_;
		}
		else {
			q->__right_ = p->__left_;
		}
	}
	// 오른쪽 자식만 있는경우
	else if (p->__right_ != nullptr) {
		if (q == nullptr) {
			__root = __root->__right_;
		}
		else if (q->__left_ == p) {
			q->__left_ = p->__right_;
		}
		else {
			q->__right_ = p->__right_;
		}
	}
	// 자식이 없는 경우
	else {
		if (q == nullptr) {
			__root = nullptr;
		}
		else if (q->__left_ == p) {
			q->__left_ = nullptr;
		}
		else {
			q->__right_ = nullptr;
		}
	}
	delete p;

	while (!stack.empty()) {
		q = stack.top();
		stack.pop();
		__update(q); // height랑 size update
	}
	return __root;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) return;

	__clear(__x->__left_);
	__clear(__x->__right_);

	delete __x;
	__x = nullptr;  
}


// 아래는 반드시 사용해야하는 BST 클래스입니다.
template <class _Tp>
class BST {
public: // Member types
	typedef _Tp						key_type;
	typedef std::size_t				size_type;
	typedef Node<key_type>* pointer;
	typedef const Node<key_type>* const_pointer;

private: // Member variables
	pointer	__root_;

public: // Constructor
	BST() : __root_(nullptr) {}

	/*
	* 아래는 반드시 구현해야하는 부분입니다.
	* 위의 추천드리는 설계 방식을 이용하여 구현하는 것을 추천합니다.
	* 추전드리는 설계 방식을 이용하지 않고 새로 구현하셔도 무방합니다.
	*/

public: // Capacity
	size_type height() const {
		// use __height or write your own code here
		return __height(__root_);
	}

	size_type size() const {
		// use __size or write your own code here
		return __size(__root_);
	}

public: // Lookup
	void inorder() const {
		// use __inorder or write your own code here
		__inorder(__root_);
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		// use __insertBST or write your own code here
		return __insertBST(__root_, key);

	}

	const_pointer erase(const key_type& key) {
		// use __eraseBST or write your own code here
		pointer __r = __eraseBST(__root_, key);

		// Client still needs to destruct/deallocate it
		// Or memory leak will occur
		//delete __r;

		return __r;
	}

	void clear() {
		// use __clear or write your own code here
		__clear(__root_);
	}


	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;
		return os;
	}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* ❗️새로 구현하실 경우, 출력 형식에 주의하세요.❗️
*/
int main(int argc, char** argv) {
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
			if (tree.erase(key) == nullptr) {
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			break;
		default:
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
			break;
		}
		//std::cout << tree << std::endl;
		tree.inorder(); // inorder순회로 출력
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}