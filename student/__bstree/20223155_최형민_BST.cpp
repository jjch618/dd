/*
* File Processing Report 2024 - Binary Search Tree skeleton code in C/C++
*
* 본 Skeleton code는 구현에 도움을 주기 위해 작성된 코드입니다.
* ❗️본 Skeleton code는 과제의 요구 조건을 완전히 충족시키지 않을 수 있습니다.❗️
* ❗️최종 점수 산정은 과제 PDF에 명시된 요구 사항을 기준으로 이루어집니다.❗️
*
* 미리 말씀드리면, skeleton code가 다소 어려울 수 있습니다.
* C++ 스러운 코딩 스타일과 코드 설계 방식에 관한 고찰이 담겨있으니, 충분한 시간을 들여 분석해보시기 바랍니다.
* 또, 재사용성을 고려하여 설계된 코드인 만큼 처음에는 이해하기 어려울 수 있습니다.
* 이후 AVL-Tree, B-Tree 과제에서도 그대로 사용되니, 이번 BST에서 잘 이해하고 넘어가시길 바랍니다.
*
*/

#include <stack>
#include <iostream>
#include <algorithm>



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
	unsigned height;

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(0) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(0){}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// write your own code here
		if (np == nullptr) return os;
		os << "<" << np->__left_;
		os << " " << np->__key_ << " ";
		os << np->__right_  << ">";
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/


template<class Node>
Node* getBSTNode() {
	return new Node();
}



template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return 0;
	return 1 + std::max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return 0;
	return 1 + __size(__x->__left_) + __size(__x->__right_); //노드의 총 갯수를 순환하여 더해줌
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x, std::stack<_NodePtr>& s) {
	// write your own code here
	while (__x->__left_ != nullptr) {
		s.push(__x);
		__x = __x->__left_;
	}

	return __x;

}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x, std::stack<_NodePtr>& s) {
	// write your own code here
	while (__x->__right_ != nullptr) {
		s.push(__x);
		__x = __x->__right_;
	}

	return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return;
	std::cout << "<";
	__inorder(__x->__left_);
	std::cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
	std::cout << ">";
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/ 
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr p, q;
	p = __root;
	q = nullptr;
	std::stack<_NodePtr> s;

	while (p != nullptr) {
		if (key == p->__key_) {
			return std::pair<_NodePtr, bool>(p, false); //newKey already exists in T
		}
		q = p;
		s.push(q);
		if (key < p->__key_) p = p->__left_;
		else p = p->__right_;
	}

	Node<_Tp>* newNode;
	newNode = getBSTNode< Node<_Tp> >();
	newNode -> __key_ = key;

	if (__root == nullptr) __root = newNode;
	else if (key < q->__key_) q->__left_ = newNode;
	else q->__right_ = newNode;
	while (!s.empty()) {
		q = s.top();
		s.pop();
		q->height = 1 + std::max(__height(q->__left_), __height(q->__right_));
	}
	return std::pair<_NodePtr, bool>(newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr p, q, tempNode;
	p = __root;
	q = nullptr;
	std::stack<_NodePtr> s;

	while (p != nullptr && key != p->__key_) {
		q = p;
		s.push(q);

		if (key < p->__key_) p = p->__left_;
		else p = p->__right_;

	}

	if (p == nullptr) return nullptr;

	if (p->__left_ != nullptr && p->__right_ != nullptr) {
		tempNode = p;
		s.push(p);

		if ((p->__left_) -> height < (p->__right_) -> height || (((p->__left_) -> height == (p->__right_) -> height) && (__size(p->__left_) < __size(p->__right_)))) {
			p = __tree_min(p ->__right_, s);
			
		}
		else {
			p = __tree_max(p -> __left_, s);
		}
		//높이차이 비교해서 삭제한 후 갖고 올 값 선정(높이가 큰 쪽, 높이가 같다면 사이즈가 큰 쪽에서의 최소값)

		tempNode->__key_ = p->__key_;
		q = s.top();
		s.pop();
	}

	if (p-> __left_ != nullptr) { //max -> size 1 or size 1
		if (q == nullptr) __root = __root->__left_; //delete root
		else if (q->__left_ == p) q->__left_ = p->__left_; //case : size 1
		else q->__right_ = p->__left_; //case: size 2 -> maxnode -> 1, another size 1
	}
	else if (p-> __right_ != nullptr) { //min -> size 1 or size 1
		if (q == nullptr) __root = __root -> __right_; //delete root
		else if (q -> __left_ == p) q -> __left_ = p->__right_; //size 1, size2 -> minnode -> 1
		else q->__right_ = p->__right_;//another size 1
	}
	else { //min max size 0, or size 0
		if (q == nullptr) __root = nullptr;
		else if (q->__left_ == p) q->__left_ = nullptr;
		else q->__right_ = nullptr;
	}

	while (!s.empty()) {
		q = s.top();
		s.pop();
		q->height = 1 + std::max(__height(q->__left_),__height(q->__right_));
	}

	return p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// write your own code here
	if (__x == nullptr) return;

	__clear(__x -> __left_);
	__clear(__x -> __right_);

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
int main() {
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
			BST<int>::const_pointer r;
			r = tree.erase(key); //삭제할 노드 반환
			if (r == nullptr) {
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			else {
				delete r; //delete 해줌으로써 메모리 누수 방지
			}
			break;
		default:
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
			break;
		}
		std::cout << tree << std::endl;
		//tree.inorder();
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}