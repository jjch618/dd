// 출력형식 틀림

/*
* Binary Search Tree code in C++
* 20243047 김형민
*/

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

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// Node 구조 출력
		// 왼쪽 자식 -> 현재 노드 -> 오른쪽 자식 순으로 출력

		if (np == nullptr) return os;

		os << "<";
		os << np->__left_;
		os << ' ' << np->__key_ << ' ';                 
		os << np->__right_;              
		os << ">";

		return os;
		return os;
	}
};

template <class _NodePtr>
unsigned __height(_NodePtr __x) {

	// 노드가 하나도 없으면 0 반환
	if (__x == nullptr) return 0;

	// 재귀적으로 자식 BST의 height를 확인
	unsigned __left = __height(__x->__left_);
	unsigned __right = __height(__x->__right_);

	// (루트 노드) 1 + (더 큰 자식 BST의 높이) 계산
	return 1 + (__left < __right ? __right : __left);
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {

	// 노드가 하나도 없으면 0 반환
	if (__x == nullptr) return 0;

	// 자식 BST의 size 확인
	unsigned __left = __size(__x->__left_);
	unsigned __right = __size(__x->__right_);   // ⚠️ __height → __size 로 수정

	// (루트 노드) 1 + (왼쪽 BST의 size) + (오른쪽 BST의 size) 계산
	return 1 + __left + __right;
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {

	// 노드가 하나도 없으면 null 반환
	if (__x == nullptr) return nullptr;

	// q = p의 부모 노드, p = 찾고자 하는 노드
	_NodePtr q = nullptr;
	_NodePtr p = __x;

	// minNode이므로 BST의 가장 왼쪽에 있는 자식 노드까지 탐색
	while (p->__left_ != nullptr) {   
		q = p;
		p = p->__left_;
	}

	return p;  
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {

	// 노드가 하나도 없으면 null 반환
	if (__x == nullptr) return nullptr;

	// q = p의 부모 노드, p = 찾고자 하는 노드
	_NodePtr q = nullptr;
	_NodePtr p = __x;

	// maxNode이므로 BST의 가장 오른쪽에 있는 자식 노드까지 탐색
	while (p->__right_ != nullptr) {  
		q = p;
		p = p->__right_;
	}

	return p;
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// 재귀적으로 중위 순회(inorder traversal) 수행
	if (__x == nullptr) return;

	__inorder(__x->__left_);                 // 왼쪽 자식
	std::cout << __x->__key_;         // 현재 노드 출력
	__inorder(__x->__right_);                // 오른쪽 자식
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {  // ⚠️ 참조(&)로 수정

	// __q: 찾고자 하는 노드, __p: newNode가 들어갈 자리
	_NodePtr __q = nullptr;
	_NodePtr __p = __root;

	// __q를 찾을 때 까지 자식 노드 탐색 ( __q <- __p 반복)
	while (__p != nullptr) {

		// 키에 해당하는 노드가 이미 있으면 false pair 반환
		if (key == __p->__key_) return std::pair<_NodePtr, bool>(__p, false);

		// 리프 노드 찾는 로직
		__q = __p;

		if (key < __p->__key_)
			__p = __p->__left_;
		else
			__p = __p->__right_;
	}

	// 리프 노드 찾았으니 새 노드 생성
	_NodePtr newNode = new Node<_Tp>(key);   // ⚠️ 변수명 수정, 타입 템플릿화

	// 찾은 __q의 자식에 새로 만든 노드 포인팅하는 로직
	if (__root == nullptr)
		__root = newNode;
	// 왼쪽 / 오른쪽 선택해서 집어넣음
	else if (key < __q->__key_)
		__q->__left_ = newNode;
	else
		__q->__right_ = newNode;

	return std::pair<_NodePtr, bool>(newNode, true);   // ⚠️ __p 대신 newNode 반환
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {

	// __q: 부모 노드, __p: 찾고자 하는 노드
	_NodePtr __q = nullptr;
	_NodePtr __p = __root;

	// key에 해당하는 노드를 찾을 때 까지 자식 노드 탐색 ( __q <- __p 반복)
	while (__p != nullptr && key != __p->__key_) {

		// 리프 노드 찾는 로직
		__q = __p;

		if (key < __p->__key_)
			__p = __p->__left_;
		else
			__p = __p->__right_;
	} // 여기까진 insertBST와 비슷함

	// 존재하지 않는 key를 erase하려고 할 때 nullptr 반환 
	if (__p == nullptr) return nullptr;

	// degree가 2일 때 (왼쪽 오른쪽 자식이 모두 있을 때)
	if (__p->__left_ != nullptr && __p->__right_ != nullptr) {

		_NodePtr tempNode = __p;

		// 자식 중 왼쪽이랑 오른쪽 높이 비교함
		// 높이가 같으면 size(노드 개수) 비교 후에 그 노드의 minNode 찾음
		// 그리고 그 트리의 minNode까지 탐색 (__tree_max, __tree_min이랑 로직 동일함)
		if ((__height(__p->__left_) < __height(__p->__right_)) ||
			(__height(__p->__left_) == __height(__p->__right_) &&
				__size(__p->__left_) < __size(__p->__right_))) {

			__p = __p->__right_;
			while (__p->__left_ != nullptr)
				__p = __p->__left_;
		}
		else {
			__p = __p->__left_;
			while (__p->__right_ != nullptr)
				__p = __p->__right_;
		}

		tempNode->__key_ = __p->__key_;
	}

	// degree(자식 branch)가 0 또는 1일 때
	// 오른쪽이 null일 때 
	if (__p->__left_ != nullptr) {

		if (__q == nullptr)
			__root = __p->__left_;
		else if (__q->__left_ == __p)
			__q->__left_ = __p->__left_;
		else
			__q->__right_ = __p->__left_;
	}
	// 왼쪽이 null일 때
	else if (__p->__right_ != nullptr) {
		if (__q == nullptr)
			__root = __p->__right_;
		else if (__q->__left_ == __p)
			__q->__left_ = __p->__right_;
		else
			__q->__right_ = __p->__right_;
	}
	// 자식이 없을 때
	else {
		if (__q == nullptr)
			__root = nullptr;
		else if (__q->__left_ == __p)  // ⚠️ '=' → '==' 로 수정
			__q->__left_ = nullptr;
		else
			__q->__right_ = nullptr;
	}

	return __p;
}


// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// 후위 순회(postorder traversal) 방식으로 노드 전체 삭제
	if (__x == nullptr) return;

	__clear(__x->__left_);       // 왼쪽부터 삭제
	__clear(__x->__right_);      // 오른쪽 삭제
	delete __x;                  
	__x = nullptr;
}

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

public: // Capacity
	size_type height() const {
		return __height(__root_);
	}

	size_type size() const {
		return __size(__root_);
	}

public: // Lookup
	void inorder() const {
		__inorder(__root_);
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertBST(__root_, key);
	}

	const_pointer erase(const key_type& key) {
		pointer __r = __eraseBST(__root_, key);

		delete __r;

		return __r;
	}

	void clear() {
		__clear(__root_);
	}

	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;
		return os;
	}
};

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
		std::cout << tree << std::endl;
	}

	tree.clear();

	return (0);
}
