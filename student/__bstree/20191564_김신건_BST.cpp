/*
* File Processing Report 2025
* BST Code
* KMU 소프트웨어학과 20191564 김신건
* 작성일: 2025년 10월 30일
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
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// write your own code here
		if(np == nullptr) {
			return os;
		}

		os << "<" << np -> __left_ << " " << np -> __key_ << " " << np -> __right_ << ">";
		
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) return 0;

	unsigned __leftSubTreeHeight = __height(__x->__left_);
	unsigned __rightSubTreeHeight = __height(__x->__right_);

	return std::max(__leftSubTreeHeight, __rightSubTreeHeight) + 1;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) return 0;

	unsigned __leftSubTreeSize = __size(__x->__left_);
	unsigned __rightSubTreeSize = __size(__x->__right_);

	return 1 + __leftSubTreeSize + __rightSubTreeSize;
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	_NodePtr __currentNode = __x;

	while(__currentNode && __currentNode -> __left_ != nullptr) {
		// 왼쪽 자식 노드가 존재하면, 계속해서 왼쪽 자식 노드로 이동한다.
		__currentNode = __currentNode -> __left_;
	}

	return __currentNode;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	_NodePtr __currentNode = __x;

	while(__currentNode && __currentNode -> __right_ != nullptr) {
		__currentNode = __currentNode -> __right_;
	}
	return __currentNode;
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __q = nullptr;
	_NodePtr __p = __root;
	
	while (__p != nullptr) {
		if(key == __p -> __key_) {
			/* 
			* 이미 존재하는 key인 경우
			* - 삽입 실패로 early return 시킨다.
			*/ 
			return std::pair<_NodePtr, bool>(__p, false);
		}

		__q = __p;

		__p = (key < __p -> __key_) ? __p -> __left_ : __p -> __right_;
	}
	
	_NodePtr __newNode = new Node<_Tp>(key);

	if(__root == nullptr) {
		__root = __newNode;
		return std::pair<_NodePtr, bool>(__root, true);
	}
	

	if(key < __q -> __key_) {
		__q -> __left_ = __newNode;
	} else {
		__q -> __right_ = __newNode;
	}

	return std::pair<_NodePtr, bool>(__newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __q = nullptr;
	_NodePtr __p = __root; 

	std::stack <_NodePtr> stk; // 삭제 위치를 찾기 위한 스택

	while(__p != nullptr && key != __p -> __key_) {
		__q = __p;
		stk.push(__q);

		__p = (key < __p -> __key_) ? __p -> __left_ : __p -> __right_;
	}

	if(__p == nullptr) {
		/*
		* 키 값을 찾지 못한 경우
		* - 삭제 실패로 nullptr을 반환하며 early return 시킨다.
		*/
		return nullptr;
	}

	// 차수가 2인 경우 
	if(__p -> __left_ != nullptr && __p -> __right_ != nullptr) {
		_NodePtr __tempNode = __p;
		stk.push(__p);

		unsigned __leftHeight = __height(__p -> __left_);
		unsigned __rightHeight = __height(__p -> __right_);

		unsigned __leftSize = __size(__p -> __left_);
		unsigned __rightSize = __size(__p -> __right_);

		bool __rightSelectionCondition = (
			__leftHeight < __rightHeight ||
			(__leftHeight == __rightHeight && __leftSize < __rightSize)
		);

		if(__rightSelectionCondition) {
			__p = __p -> __right_;
			while(__p -> __left_ != nullptr) {
				stk.push(__p);
				__p = __p -> __left_;
			}
		} else {
			__p = __p -> __left_;
			while(__p -> __right_ != nullptr) {
				stk.push(__p);
				__p = __p -> __right_;
			}
		}

		__tempNode -> __key_ = __p -> __key_;
		__q = (stk.empty() ? nullptr : stk.top());

	}
	
	// 이제 차수가 0 or 1인 노드를 삭제하면 된다.
	_NodePtr __child = (__p -> __left_ != nullptr) ? __p -> __left_ : __p -> __right_;

	if(__q == nullptr) {
		// 루트 노드가 삭제되는 경우
		__root = __child;
	} else {
		if(__q -> __left_ == __p) {
			__q -> __left_ = __child;
		} else {
			__q -> __right_ = __child;
		}
	}
	return __p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
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
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {}
	
	/*
	* 아래는 반드시 구현해야하는 부분입니다.
	* 위의 추천드리는 설계 방식을 이용하여 구현하는 것을 추천합니다.
	* 추전드리는 설계 방식을 이용하지 않고 새로 구현하셔도 무방합니다.
	*/
	
	public: // Destructor
		~BST() {
			__clear(__root_);
		}

	public: // Capacity
		size_type height() const {
			return __height(__root_);
		}

		size_type size() const {
			return __size(__root_);
		}

	public: // Modifier
		std::pair<const_pointer, bool> insert(const key_type& key) {
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			pointer __r = __eraseBST(__root_, key);

			// Client still needs to destruct/deallocate it
			// Or memory leak will occur
			delete __r;
			return __r; 
		}

		void clear() {
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
					// std::cout << "i " << key << ": The key already exists" << std::endl;
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == nullptr) {
					// std::cout << "d " << key << ": The key does not exist" << std::endl;
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
