/*
* File Processing Report 2024 - Binary Search Tree skeleton code in C/C++
*
* 본 Skeleton code는 구현에 도움을 주기 위해 작성된 코드입니다.
* 본 Skeleton code는 과제의 요구 조건을 완전히 충족시키지 않을 수 있습니다.
* 최종 점수 산정은 과제 PDF에 명시된 요구 사항을 기준으로 이루어집니다.
*
* 미리 말씀드리면, skeleton code가 다소 어려울 수 있습니다.
* C++ 스러운 코딩 스타일과 코드 설계 방식에 관한 고찰이 담겨있으니, 충분한 시간을 들여 분석해보시기 바랍니다.
* 또, 재사용성을 고려하여 설계된 코드인 만큼 처음에는 이해하기 어려울 수 있습니다.
* 이후 AVL-Tree, B-Tree 과제에서도 그대로 사용되니, 이번 BST에서 잘 이해하고 넘어가시길 바랍니다.
*
*/

#include <iostream>

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

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		//노드 자체가 존재 하지 않으면 바로 리턴
		if (np == nullptr) {
			return os;
		}

		os << '<';
		//왼쪽 자식이 존재할 경우 왼쪽 출력
		if (np->__left_ != nullptr) {
			os << np->__left_;
		}
		//현재 key 출력
		os << ' ';
		os << np->__key_;
		os << ' ';
		//오른쪽 자식이 존재할 경우 오른쪽 출력
		if (np->__right_ != nullptr) {
			os << np->__right_;
		}
		os << '>';
		return os;

	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	//트리가 존재하지 않을 경우
	if (__x == nullptr) {
		return 0;
	}
	//존재하는 경우
	unsigned l = __height(__x->__left_);
	unsigned r = __height(__x->__right_);
	unsigned x;

	//왼쪽과 오른쪽 중 더 큰 것을 x로 설정하여 +1하여 리턴하면 그 값이 곧 높이
	//max() 쓰고 싶었지만, 그러려면 헤더파일 algorithm이 필요해서 안 씀
	if (l > r) {
		x = l;
	}
	else {
		x = r;
	}
	return x + 1;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) {
		return 0;
	}
	//왼쪽 노드 갯수 + 오른쪽 노드 갯수 + 1(현재 노드)를 하면 총 노드 갯수
	unsigned l = __size(__x->__left_);
	unsigned r = __size(__x->__right_);
	return l + r + 1;
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if (__x == nullptr) {
		return nullptr;
	}

	//왼쪽으로 쭉 가기
	while (__x->__left_ != nullptr) {
		__x = __x->__left_;
	}
	return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (__x == nullptr) {
		return nullptr;
	}
	//오른쪽으로 쭉 가기
	while (__x->__right_ != nullptr) {
		__x = __x->__right_;
	}
	return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* 잘못된 출력 형식은 0점 처리됩니다.
*/


//operator<< 로직을 구현을 했지만 채점기준에 있어서 같은 로직으로 구현해놓기 실제로 사용은 안함
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	//노드 자체가 존재 하지 않으면 바로 리턴
	if (__x == nullptr) {
		return;
	}

	std::cout << '<';
	//왼쪽 자식이 존재할 경우 왼쪽 출력
	if (__x->__left_ != nullptr) {
		std::cout << __x->__left_;
	}
	//현재 key 출력
	std::cout << ' ';
	std::cout << __x->__key_;
	std::cout << ' ';
	//오른쪽 자식이 존재할 경우 오른쪽 출력
	if (__x->__right_ != nullptr) {
		std::cout << __x->__right_;
	}
	std::cout << '>';
	return;
}
//아래에서 insertBST를 위해 getNodeBST()구현
template<class T>
Node<T>* getNodeBST() { //반환하는게 새로 만드는 노드의 함수니까 * 포인터 사용
	return new Node<T>(); //지역변수로 사라지지 않게 하기 위해 new로 할당 해주기 
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) { // __root를 레퍼런스로 받아야 바깥루트에 대해서도 변함
	// write your own code here
	_NodePtr p = __root;
	_NodePtr q = nullptr;

	//stack을 함수로 구현하려다가 단순 배열로 구현해서 top의 위치만 받아준다고 해도 될것 같아서 이렇게 구현
	// 입력되는 수의 크기가 1000을 넘지 않아서 다음과 같이 함
	_NodePtr* stack = new _NodePtr[1000];
	int top = 0;
	while (p != nullptr) {
		//넣으려는 키가 존재할 경우
		if (key == p->__key_) {
			delete[] stack;
			return std::pair<_NodePtr, bool>(p, false);
		}
		q = p;
		stack[top] = q;
		top++;
		if (key < p->__key_) {
			p = p->__left_;
		}
		else {
			p = p->__right_;
		}
	}

	_NodePtr newNode = getNodeBST<_Tp>();
	newNode->__key_ = key;
	newNode->__right_ = nullptr;
	newNode->__left_ = nullptr;

	//트리 자체가 존재하지 않을 경우
	if (__root == nullptr) {
		__root = newNode;
	}
	// 키가 넣으려는 것보다 작은 경우
	else if (key < q->__key_) {
		q->__left_ = newNode;
	}
	// 키가 넣으려는 것보다 큰 경우
	else {
		q->__right_ = newNode;
	}

	//현재 상황에서 이 과정이 필요하진 않음
	while (top != 0) {
		--top;
		q = stack[top];

		__height(q);
	}
	delete[] stack;

	return std::pair<_NodePtr, bool>(newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr;
	_NodePtr* stack = new _NodePtr[1000];
	int top = 0;
	//삭제할 노드 찾기는 insert와 동일하게 진행
	while (p != nullptr && key != p->__key_) {
		q = p;
		stack[top] = q;
		top++;
		if (key < p->__key_) {
			p = p->__left_;
		}
		else {
			p = p->__right_;
		}
	}

	if (p == nullptr) {
		delete[] stack;
		return nullptr;
	}

	//삭제할 노드의 차수가 2인 경우
	if (p->__left_ != nullptr && p->__right_ != nullptr) {
		_NodePtr tempNode = p;
		stack[top] = p;
		top++;

		//pdf 조건에 있는 height(T), size(T), minNode(T), MaxNode(T)를 이용한 것
		//오른쪽 서브트리의 높이가 더 크거나 같을때
		if ((__height(p->__left_) < __height(p->__right_)) or (__height(p->__left_) == __height(p->__right_) && __size(p->__left_) < __size(p->__right_))) {
			//부모찾기 구현 해놓으면서 p,q 갱신하기			
			_NodePtr parent = p;
			_NodePtr cur = p->__right_;

			cur = __tree_min(cur);

			if (cur != p->__right_) {
				parent = p->__right_;
				while (parent->__left_ != cur) {
					parent = parent->__left_;
				}
			}
			p = cur;
			q = parent;
		}
		//왼쪽 서브트리의 높이가 더 클때
		else {
			//부모찾기 구현 해놓으면서 p,q 갱신하기
			_NodePtr parent = p;
			_NodePtr cur = p->__left_;

			cur = __tree_max(cur);

			if (cur != p->__left_) {
				parent = p->__left_;
				while (parent->__right_ != cur) {
					parent = parent->__right_;
				}
			}
			p = cur;
			q = parent;
		}

		tempNode->__key_ = p->__key_;

	}

	//p의 차수가 0 혹은 1
	//여기는 수도코드 부분 그대로 조건문 쓰기
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

	//현재 상황에서 이 과정이 필요하진 않음
	while (top != 0) {
		--top;
		q = stack[top];

		__height(q); // 따로 사용은 안하지만 이후 avl코드에서 사용할 것 같아서 그냥 값 호출로만 그대로 둠 + 수도코드와 스켈레톤 코드 유지
	}
	delete[] stack;

	return p;


}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	//노드의 왼쪽 오른쪽을 후위순회 방식으로 삭제하는걸 선택
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
* 새로 구현하실 경우, 출력 형식에 주의하세요.
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