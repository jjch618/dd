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

#include <iostream>
#include <stack>

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

	// 코드에서 사용할 높이와 크기를 구현해놓았습니다.
	unsigned __height_;
	unsigned __size_;


public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(0), __size_(0) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// write your own code here
		// Node::operator<< 에서 inorder의 기능을 포함해서 구현되어 있습니다.

		// 매개변수로 node를 받았을때, 비어있을 경우에 node의 왼쪽 노드와 오른쪽 노드, 키 값에 접근하면 오류가 발생합니다.
		// 따라서 이와 같은 경우를 해결하기 위해 node가 비어있을 경우에 바로 os를 반환합니다.
		if (np == nullptr) return os;

		// 아래는 inorder의 순서로 재귀함수처럼 왼쪽 노드의 operator<<를 출력시킵니다.
		os << "<";
		if (np->__left_ != nullptr) {
			os << np->__left_;
		}
		if (np != nullptr) {
			os << " " << np->__key_ << " ";
		}
		// 아래도 마찬가지로 오른쪽 노드의 operator<<를 출력시킵니다.
		if (np->__right_ != nullptr) {
			os << np->__right_;
		}
		os << ">";

		// 이렇게 되면 트리가 순서대로 출력됩니다.
		// 그렇게 저장한 출력 값인 os를 return 해줍니다.

		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	// write your own code here
	// 노드의 높이를 계산하는 역할입니다. 노드가 비어있으면 0을 반환합니다.
	if (__x == nullptr) return 0;
	// 노드가 존재한다면 그 노드의 높이를 반환합니다.
	else return __x->__height_;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	// write your own code here
	// 노드의 크기를 계산하는 역할입니다. 노드가 비어있으면 0을 반환합니다.
	if (__x == nullptr) return 0;
	// 노드가 존재한다면 그 노드의 크기를 반환합니다.
	else return __x->__size_;
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// write your own code here
	// 트리에서 특정 노드보다 큰 키 값을 가진 노드 중에서 가장 작은 값을 가진 노드를 반환하는 값입니다.
	// 노드가 비어 있으면 빈 포인터를 반환합니다.
	if (__x == nullptr) return nullptr;
	// 특정 노드의 오른쪽 노드에서 시작해 가장 왼쪽에 있는 노드로 접근합니다.
	while (__x->__left_ != nullptr) __x = __x->__left_;
	// 반복이 끝난 후, 저장된 노드를  반환합니다.
	return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	// write your own code here
	// 트리에서 특정 노드보다 작은 키 값을 가진 노드 중에서 가장 큰 값을 가진 노드를 반환하는 값입니다.
	// 노드가 비어 있으면 빈 포인터를 반환합니다.
	if (__x == nullptr) return nullptr;
	// 특정 노드의 오른쪽 노드에서 시작해 가장 왼쪽에 있는 노드로 접근합니다.
	while (__x->__right_ != nullptr) __x = __x->__right_;
	// 반복이 끝난 후, 저장된 노드를  반환합니다.
	return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// write your own code here
	// __inorder의 기능은 class Node::operator<< 에 구현했습니다.
}

// 새로운 노드를 생성할 때 사용되는 template입니다.
template <class _Tp>
Node<_Tp>* getBSTNode() {
	return new Node<_Tp>();
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
//__root를 수정하기 위해서 값을 복사하는 방식이 아닌 참조로 수정했습니다.
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	// p의 부모 노드
	_NodePtr q = nullptr;
	// 최상단의 노드 p
	_NodePtr p = __root;
	// 노드를 담아둘 스택
	std::stack<_NodePtr> nodeS;

	// 최상단의 노드가 존재할 경우
	while (p != nullptr) { 
		// 입력할 키 값이 이미 존재할 경우에 false를 반환합니다.
		if (key == p->__key_) return std::pair<_NodePtr, bool>(p, false);

		// 부모 노드에 노드p가 입력됩니다.
		q = p; 
		// 부모 노드를 스택에 push 합니다.
		nodeS.push(q);
		// 만약 현재 노드의 값이 입력할 키 값보다 크다면 노드 p에 현재 노드의 왼쪽 노드의 값을 입력합니다.
		if (key < p->__key_) p = p->__left_; 
		// 아니라면 노드 p에 현재 노드의 오른쪽 노드를 입력합니다.
		else p = p->__right_; 
	} 
	// 새로운 노드를 생성합니다.
	_NodePtr newN = getBSTNode<_Tp>();
	// 새로운 노드에 키 값과 사이즈, 높이를 생성합니다.
	newN->__key_ = key;
	newN->__size_ = 1;
	newN->__height_ = 1;

    // 최상단의 노드가 비어있었다면 새로운 노드를 최상단 노드로 설정합니다.
	if (__root == nullptr) __root = newN;
	// 현재 노드p가 비어있을 때, 반복문을 탈출했습니다.
	// 따라서 부모 노드q를 기준으로 키 값과 비교해서 작으면 왼쪽 노드에 새로운 노드,
	// 크면 오른쪽 노드에 새로운 노드를 집어넣어서 연결합니다.
	else if (key < q->__key_) q->__left_ = newN;
	else q->__right_ = newN; 

	// 새롭게 입력된 노드로 인해 높이와 크기를 변경합니다.
	while (!nodeS.empty()) {
		// 쌓아두었던 stack에서 마지막에 들어온 값을 순서대로 출력합니다.
		q = nodeS.top();
		nodeS.pop();
		// 그리고 높이는 왼쪽 노드와 오른쪽 노드중에서 더 큰 높이에 1을 더합니다.
		q->__height_ = 1 + (__height(q->__left_) > __height(q->__right_) ? __height(q->__left_) : __height(q->__right_));
		// 그리고 크기는 왼쪽 노드와 오른쪽 노드를 모두 더합니다.
		q->__size_ = 1 + __size(q->__left_) + __size(q->__right_);
	}
	// 그렇게 새로 입력된 노드와 true 값을 반환합니다.
	return std::pair<_NodePtr, bool>(newN, true);
}


/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	// 최상단의 노드 p와 부모 노드 q, 노드의 stack을 선언합니다.
	_NodePtr p = __root;
	_NodePtr q = nullptr;
	std::stack<_NodePtr> nodeS;

	// 만약 현재 노드가 비어있지 않고 현재 키 값이 입력된 키 값과 일치하지 않는다면
	while (p != nullptr && key != p->__key_) {
		// 부모 노드에 현재 노드를 입력합니다. 그리고 부모 노드를 stack에 push 합니다.
		q = p;
		nodeS.push(q);

		// 만약 키 값이 현재 노드보다 작으면 왼쪽 노드로 접근하고 크면 오른쪽 노드로 접근합니다.
		if (key < p->__key_) p = p->__left_;
		else p = p->__right_;
	}

	// 일련의 과정을 거치고 현재 노드가 존재하지 않으면 입력한 키 값이
	// 노드에 존재하지 않는 다는 뜻 입니다. 따라서 널 포인터를 반환합니다.
	if (p == nullptr) return nullptr;

	// 차수가 2인 경우입니다.
	if (p->__left_ != nullptr && p->__right_ != nullptr) { // case of degree 2
		// 임시 노드에 p 값을 입력합니다. 그리고 현재 노드 p를 stack에 push 합니다.
		_NodePtr tempNode = p;
		nodeS.push(p);

		// 만약 오른쪽 노드의 높이가 크거나, 좌우의 노드의 높이가 같을때, 노드의 크기가 오른쪽 노드의 크기가 더 큰 경우 입니다.
		if (__height(p->__left_) < __height(p->__right_) || (__height(p->__left_) == __height(p->__right_) && __size(p->__left_) < __size(p->__right_))) {
			// 노드 p에 현재 노드의 오른쪽 노드를 저장합니다..
			p = tempNode->__right_;
			// 노드 p 값이 현재 노드보다 큰 키 값 중에서 가장 작은 값을 가진 노드가 될때까지 반복합니다.
			while (p != __tree_min(tempNode->__right_)) {
				// 반복을 계속하면서 변경될 높이와 사이즈를 위해서 노드를 stack에 push 합니다.
				nodeS.push(p);
				p = p->__left_;
			}
		}
		else {
			// 노드 p에 현재 노드의 왼쪽 노드를 저장합니다..
			p = tempNode->__left_;
			// 노드 p 값이 현재 노드보다 작은 키 값 중에서 가장 큰 값을 가진 노드가 될때까지 반복합니다.
			while (p != __tree_max(tempNode->__left_)) {
				// 반복을 계속하면서 변경될 높이와 사이즈를 위해서 노드를 stack에 push 합니다.
				nodeS.push(p);
				p = p->__right_;
			}
		}
		// 삭제될 키 값이 있던 위치의 노드인 tempNode에 구해둔 노드 p의 키 값을 입력합니다.
		tempNode->__key_ = p->__key_;
		// 키 값이 삭제될 노드로 입력하고 불필요해진 노드의 부모 노드로 접근합니다.
		q = nodeS.top();
		
	}
	// now degree of p is 0 or 1
	// 삭제될 노드 p의 왼쪽 노드가 있는 경우
	if (p->__left_ != nullptr) { // case of degree 1 - left child
		// q가 빈 포인터이면 이는 삭제될 노드가 최상단의 노드라는 의미로 __root에 값을 저장합니다.
		if (q == nullptr) __root = __root->__left_;
		// q의 왼쪽 노드가 p인 경우에는 q의 왼쪽 노드에 p의 왼쪽 노드를 입력하면서 p의 노드의 연결을 끊어냅니다.
		else if (q->__left_ == p) q->__left_ = p->__left_;
		// q의 오른쪽 노드가 p인 경우에는 q의 오른쪽 노드에 p의 왼쪽 노드를 입력합니다.
		else q->__right_ = p->__left_;
	}
	// 삭제될 노드가 p의 오른쪽 노드가 있는 경우
	else if (p->__right_ != nullptr) { // case of degree 1 - right child
		// q가 빈 포인터라면 삭제될 노드가 최상단의 노드라는 의미로 __root에 값을 저장합니다.
		if (q == nullptr) __root = __root->__right_;
		// q의 왼쪽 노드가 p인 경우에는 q의 왼쪽 노드에 p의 오른쪽 노드를 입력하면서 p의 노드의 연결을 끊어냅니다.
		else if (q->__left_ == p) q->__left_ = p->__right_;
		// q의 오른쪽 노드가 p인 경우에는 q의 오른쪽 노드에 p의 오른쪽 노드를 입력하면서 p의 노드의 연결을 끊어냅니다.
		else q->__right_ = p->__right_;
	}
	// 삭제될 노드 p가 왼쪽, 오른쪽 노드가 전부 없는 경우
	else { //case of degree 0
		// 빈 트리가 됩니다.
		if (q == nullptr) __root = nullptr; 
		// 노드 q의 왼쪽 노드 연결 자체를 지워버립니다.
		else if (q->__left_ == p) q->__left_ = nullptr;
		// 노드 q의 오른쪽 노드 연결 자체를 지워버립니다.
		else q->__right_ = nullptr;
	}
	// 변경된 트리에서 노드들의 높이와 크기를 재조정해줍니다.
	while (!nodeS.empty()) {
		q = nodeS.top();
		nodeS.pop();
		q->__height_ = 1 + (__height(q->__left_) > __height(q->__right_) ? __height(q->__left_) : __height(q->__right_));
		q->__size_ = 1 + __size(q->__left_) + __size(q->__right_);
	}
	return p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
// 메모리 절약을 위해서 생성된 노드들을 지웁니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// write your own code here
	// 빈 트리이면 바로 반환합니다.
	if (__x == nullptr) return;

	//확실히 지우기 위해서 모든 노드를 s1에 push 합니다.
	std::stack<_NodePtr> s1, s2;
	s1.push(__x);

	while (!s1.empty()) {
		// stack s1에 입력된 값을 노드 p에 입력합니다.
		_NodePtr p = s1.top();
		s1.pop();
		// 입력된 노드 p 값은 stack s2에 입력됩니다.
		s2.push(p);

		// 노드 p의 왼쪽, 오른쪽 노드를 s1에 push 합니다.
		if (p->__left_ != nullptr) s1.push(p->__left_);
		if (p->__right_ != nullptr) s1.push(p->__right_);
	}

	// 일련의 과정을 거친 stack s2에서 값을 pop해서 하나씩 삭제합니다.
	while (!s2.empty()) {
		_NodePtr q = s2.top();
		s2.pop();
		delete q;
		// 연산을 모두 마쳐보면 postorder와 같은 형식으로 모든 노드를 지웠습니다.
	}

	// 마지막으로 입력된 노드를 널 포인터로 변경합니다.
	__x = nullptr;
	return;
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
		// __height로 구현했습니다.
		return __height(__root_);
	}

	size_type size() const {
		// use __size or write your own code here
		// __size로 구현했습니다.
		return __size(__root_);
	}

public: // Lookup
	void inorder() const {
		// use __inorder or write your own code here
		// Node::operator<<로 구현했습니다.
		__inorder(__root_);
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		// use __insertBST or write your own code here
		// __insertBST로 구현했습니다.
		return __insertBST(__root_, key);
	}

	const_pointer erase(const key_type& key) {
		// use __eraseBST or write your own code here
		// __eraseBST로 구현했습니다.
		pointer __r = __eraseBST(__root_, key);

		// Client still needs to destruct/deallocate it
		// Or memory leak will occur
		delete __r;

		return __r;
	}

	void clear() {
		// use __clear or write your own code here
		// __clear로 구현했습니다.
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
		std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}