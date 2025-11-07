#include <iostream>
#include <utility>

template <class _Tp> class Node;
template <class _Tp> class BST;

/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* height, size를 제외한 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
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

	static __node_pointer_type getNodeBST() {
		return new Node();
	}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* nullptr가 들어오면 공백을 출력하도록 하고, 정상적인 노드 포인터가 들어오면 해당 노드의 키값을 출력하도록 했습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (!np) return os << "\x20";
		return os << np->__key_;
	}
};

// __x를 루트 노드로 하는 BST의 높이를 구하는 함수
template <class _NodePtr>
unsigned __height(_NodePtr __x) {   // Recursive
	if (!__x) return 0;

	unsigned left = __height(__x->__left_);
	unsigned right = __height(__x->__right_);
	
	// 왼쪽 서브트리 height와 오른쪽 서브트리 height 중 더 큰 값에 1 더해주기(__x까지 height 계산에 포함)
	return 1 + (left >= right ? left : right);
}

// __x를 루트 노드로 하는 BST의 노드 개수를 구하는 함수
template <class _NodePtr>
unsigned __size(_NodePtr __x) {     // Recursive
	if (!__x) return 0;

	// 왼쪽 서브트리 size와 오른쪽 서브트리 size의 합에 1 더해주기(__x까지 size 계산에 포함)
	return 1 + __size(__x->__left_) + __size(__x->__right_);
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* __x를 루트 노드로 하는 BST에서 가장 작은 키값을 가지는 노드에 대한 포인터를 반환합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	_NodePtr p = __x;
	_NodePtr q = nullptr;      // p의 부모 노드를 가리킴. 즉, 마지막으로 유효했던 p
		while (p) {
			q = p;
			p = p->__left_;
		}
	return q;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* __x를 루트 노드로 하는 BST에서 가장 큰 키값을 가지는 노드에 대한 포인터를 반환합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	_NodePtr p = __x;
	_NodePtr q = nullptr;      // p의 부모 노드를 가리킵니다. 즉, 마지막으로 유효했던 p 입니다.
		while (p) {
			q = p;
			p = p->__right_;
		}
	return q;
}

// __x를 루트 노드로 하는 BST에서 중위 순회 진행합니다.
// <{left subtree}‘ ’{key}‘ ’{right subtree}> 형식으로 출력합니다.
template <class _NodePtr>
void __inorder(_NodePtr __x) {       //Recursive
	if (!__x) return;

	std::cout << "<";
	__inorder(__x->__left_);
	std::cout << " " << __x << " ";
	__inorder(__x->__right_);
	std::cout << ">";
}

/*
* 삽입되는 노드가 Root node가 되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr q = nullptr;     // q는 p의 부모 노드
	_NodePtr p = __root;

	// step1. key의 삽입 위치 q를 찾음
	while (p) {
		if (key == p->__key_) {             // 이미 존재하면, 기존 노드의 포인터와 false를 반환
			return std::pair<_NodePtr, bool>(p, false);
		}
		q = p;
		p = (key < p->__key_ ? p->__left_ : p->__right_);
	}

	// step2. newKey를 갖는 노드를 만듬
	_NodePtr newNode = Node<_Tp>::getNodeBST();
	newNode->__key_ = key;
	
	// step3. q의 자식으로 newKey를 삽입
	if (!__root) __root = newNode;
	else if (key < q->__key_) q->__left_ = newNode;
	else q->__right_ = newNode;

	return std::pair<_NodePtr, bool>(newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
* 메모리를 해제할 노드에 대한 포인터를 반환합니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr q = nullptr;            // q는 p의 부모 노드
	_NodePtr p = __root;


	// 삭제 위치 p를 찾음
	while (p && p->__key_ != key) {
		q = p;
		p = (key < p->__key_ ? p->__left_ : p->__right_);
	}
	if (!p) return nullptr;      // 삭제하려는 key가 존재하지 않으면 nullptr를 반환

	// 삭제할 노드가 루트인 경우
	if (!q) {
		if (!p->__left_ && !p->__right_) {       // 루트 노드의 차수가 0인 경우
			__root = nullptr;
			return p;
		}
		else if (!p->__left_ || !p->__right_) {  // 루트 노드의 차수가 1인 경우
			__root = (p->__left_ ? p->__left_ : p->__right_);
			return p;
		}
	}

	// (p != null) && (q != null) 경우. 즉, 삭제할 노드가 루트가 아닌 경우
	if (!p->__left_ && !p->__right_) {       // 삭제할 노드의 차수가 0인 경우
		if (q->__left_ == p) q->__left_ = nullptr;
		else q->__right_ = nullptr;
	}
	else if (!p->__left_ || !p->__right_) {  // 삭제할 노드의 차수가 1인 경우
		if (p->__left_) {     // p의 왼쪽 자식 노드가 존재할 경우
			if (q->__left_ == p) q->__left_ = p->__left_;
			else q->__right_ = p->__left_;
		}
		else {                // p의 오른쪽 자식 노드가 존재할 경우
			if (q->__left_ == p) q->__left_ = p->__right_;
			else q->__right_ = p->__right_;
		}
	}
	else {                                   // 삭제할 노드의 차수가 2인 경우
		bool flag;             // true: 왼쪽 서브트리에서 최대 키 노드 가져옴, false: 오른쪽 서브트리에서 최소 키 노드 가져옴
		_NodePtr r;            // 삭제할 노드 자리에 들어갈 노드
		if (__height(p->__left_) > __height(p->__right_)) {         // p의 왼쪽 서브트리 높이가 더 높은 경우
			r = __tree_max(p->__left_);
			flag = true;
		}
		else if (__height(p->__left_) < __height(p->__right_)) {    // p의 오른쪽 서브트리 높이가 더 높은 경우
			r = __tree_min(p->__right_);
			flag = false;
		}
		else {         // p의 양쪽 서브트리 높이가 같은 경우
			if (__size(p->__left_) >= __size(p->__right_)) {        // p의 왼쪽 서브트리의 노드 수가 크거나 같은 경우
				r = __tree_max(p->__left_);
				flag = true;
			}
			else {                                                  // p의 왼쪽 서브트리의 노드 수가 작은 경우
				r = __tree_min(p->__right_);
				flag = false;
			}
		}
		p->__key_ = r->__key_;             // 삭제할 노드 자리에 들어갈 노드의 키 값으로 덮어씌움
		if (flag) __eraseBST(p->__left_, r->__key_);
		else __eraseBST(p->__right_, r->__key_);
		p = r;              // 메모리 해제 해야하는 r의 노드를 p도 가리키도록 함
	}
	return p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
// 트리 전체 삭제(트리에 속한 모든 노드들의 메모리 해제)
// 왼쪽 서브트리 -> 오른쪽 서브트리 -> 현재 노드 순서로 삭제(메모리 해제)
template <class _NodePtr>
void __clear(_NodePtr& __x) {       // Recursive
	if (!__x) return;

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
	typedef Node<key_type>*			pointer;
	typedef const Node<key_type>*	const_pointer;

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
		delete __r;

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
		tree.inorder();
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}