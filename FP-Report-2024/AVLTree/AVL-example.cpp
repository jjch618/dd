/*
* File Processing Report 2024 - Adelson-Velsky and Landis Tree skeleton code in C/C++
*
* 본 Skeleton code는 구현에 도움을 주기 위해 작성된 코드입니다.
* ❗️본 Skeleton code는 과제의 요구 조건을 완전히 충족시키지 않을 수 있습니다.❗️
* ❗️최종 점수 산정은 과제 PDF에 명시된 요구 사항을 기준으로 이루어집니다.❗️
*
*/

#include <iostream>
#include <utility>
#include <algorithm>
#include <cassert>

template <class _Tp> class Node;
template <class _Tp> class AVL;

typedef enum {
	NO = 0,
	LL = 1,
	LR = 2,
	RL = 3,
	RR = 4,
} ROTATION;

/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* height, size를 제외한 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
template <class _Tp>
class Node {
	private: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
	
	public: // Constructor
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (np == nullptr) {
			return os;
		}

		os << "<";
		os << np->__left_;
		os << " " << np->__key_ << " ";
		os << np->__right_;
		os << ">";
		
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	return (__x == nullptr) ? 0 : 1 + std::max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	return (__x == nullptr) ? 0 : 1 + __size(__x->__left_) + __size(__x->__right_);
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr& __tree_min(_NodePtr& __x) {
	assert(__x != nullptr);
	return (__x->__left_ == nullptr) ? __x : __tree_min(__x->__left_);
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr& __tree_max(_NodePtr& __x) {
	assert(__x != nullptr);
	return (__x->__right_ == nullptr) ? __x : __tree_max(__x->__right_);
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) {
		return;
	}
	
	std::cout << "<";
	__inorder(__x->__left_);
	std::cout << " " << __x->__key__ << " ";
	__inorder(__x->__right_);
	std::cout << ">";
}

template <class _NodePtr>
void __tree_left_rotate(_NodePtr& __root) {
	_NodePtr __x = __root;
	_NodePtr __y = __x->__right_;
	__x->__right_ = __y->__left_;
	__root = __y;
	__y->__left_ = __x;
}

template <class _NodePtr>
void __tree_right_rotate(_NodePtr& __root) {
	_NodePtr __x = __root;
	_NodePtr __y = __x->__left_;
	__x->__left_ = __y->__right_;
	__root = __y;
	__y->__right_ = __x;
}

template <class _NodePtr>
std::pair<_NodePtr&, ROTATION> __check_balance(_NodePtr& __root, _NodePtr __p) {
	if (__root == nullptr) {
		return (std::pair<_NodePtr&, ROTATION>(__root, NO));
	}

	if (__root != __p && __p != nullptr) {
		std::pair<_NodePtr&, ROTATION> __ret =	(__root->__key_ < __p->__key_) ?
												__check_balance(__root->__right_, __p) :
												__check_balance(__root->__left_, __p);

		if (__ret.second != NO) {
			return (__ret);
		}
	}

	int __left_height = __height(__root->__left_);
	int __right_height = __height(__root->__right_);

	if (__left_height - __right_height >= 2) {
		return (std::pair<_NodePtr&, ROTATION>(__root, (__height(__root->__left_->__right_) < __height(__root->__left_->__left_)) ? LL : LR));
	}
	if (__right_height - __left_height >= 2) {
		return (std::pair<_NodePtr&, ROTATION>(__root, (__height(__root->__right_->__left_) < __height(__root->__right_->__right_)) ? RR : RL));
	}
	return (std::pair<_NodePtr&, ROTATION>(__root, NO));
}

template <class _NodePtr>
void __tree_balance(_NodePtr& __root, _NodePtr __p) {
	std::pair<_NodePtr&, ROTATION> __result = __check_balance(__root, __p);

	switch (__result.second) {
		case NO:
			break;
		case LL:
			__tree_right_rotate(__result.first);
			break;
		case LR:
			__tree_left_rotate(__result.first->__left_);
			__tree_right_rotate(__result.first);
			break;
		case RL:
			__tree_right_rotate(__result.first->__right_);
			__tree_left_rotate(__result.first);
			break;
		case RR:
			__tree_left_rotate(__result.first);
			break;
	}
}

template <class _NodePtr, class _Tp>
_NodePtr& __find_equal(_NodePtr& __root, const _Tp& key) {
	if (__root == nullptr || __root->__key_ == key) {
		return __root;
	}
	if (key < __root->__key_) {
		if (__root->__left_ == nullptr) {
			return __root->__left_; // 참조를 이용한 값 반환
		}
		return __find_equal(__root->__left_, key);
	}
	if (__root->__right_ == nullptr) {
		return __root->__right_; // 참조를 이용한 값 반환
	}
	return __find_equal(__root->__right_, key);
}

template <class _NodePtr, class _Tp>
_NodePtr __find_parent(_NodePtr& __root, const _Tp& key) {
	if (__root == nullptr || __root->__key_ == key) {
		return nullptr;
	}
	
	if (key < __root->__key_) {
		if (__root->__left_ == nullptr || __root->__left_->__key_ == key) {
			return __root;
		}
		return __find_parent(__root->__left_, key);
	}
	if (__root->__right_ == nullptr || __root->__right_->__key_ == key) {
		return __root;
	}
	return __find_parent(__root->__right_, key);
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr __root, const _Tp& key) {
	_NodePtr& __child = __find_equal(__root, key);
	bool __inserted = false;

	if (__child == nullptr) {
		__child = new Node<_Tp>(key);
		__inserted = true;
	}

	return std::pair<_NodePtr, bool>(__child, __inserted);
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertAVL(_NodePtr& __root, const _Tp& key) {
	_NodePtr& __child = __find_equal(__root, key);
	_NodePtr __parent = __find_parent(__root, key);
	bool __inserted = false;

	if (__child == nullptr) {
		__child = new Node<_Tp>(key);
		__inserted = true;
		__tree_balance(__root, __parent);
	}

	return std::pair<_NodePtr, bool>(__child, __inserted);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr>
_NodePtr __eraseBST(_NodePtr& __root) {
	unsigned __left_height = __height(__root->__left_);
	unsigned __right_height = __height(__root->__right_);
	unsigned __left_size = __size(__root->__left_);
	unsigned __right_size = __size(__root->__right_);

	_NodePtr __z = __root;
	_NodePtr& __y = (__z->__left_ == nullptr || __z->__right_ == nullptr) ? __z
					: (__left_height < __right_height || (__left_height == __right_height && __left_size < __right_size)) ? __tree_min(__z->__right_)
					: __tree_max(__z->__left_);
	_NodePtr __x = __y;
	_NodePtr __w = (__left_height < __right_height || (__left_height == __right_height && __left_size < __right_size)) ? __y->__right_ : __y->__left_;
	_NodePtr __v = __find_parent(__root, __y->__key_);

	if (__y == __root) {
		__root = __w;
	} else {
		__y = __w;
		__x->__left_ = __z->__left_;
		__x->__right_ = __z->__right_;
		__root = __x;
	}

	return __z;
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseAVL(_NodePtr& __root, const _Tp& key) {
	_NodePtr& __p = __find_equal(__root, key);

	if (__p == nullptr) {
		return nullptr;
	}

	unsigned __left_height = __height(__p->__left_);
	unsigned __right_height = __height(__p->__right_);
	unsigned __left_size = __size(__p->__left_);
	unsigned __right_size = __size(__p->__right_);

	_NodePtr __z = __p;
	_NodePtr& __y = (__z->__left_ == nullptr || __z->__right_ == nullptr) ? __z
					: (__left_height < __right_height || (__left_height == __right_height && __left_size < __right_size)) ? __tree_min(__z->__right_)
					: __tree_max(__z->__left_);
	_NodePtr __x = __y;
	_NodePtr __w = (__left_height < __right_height || (__left_height == __right_height && __left_size < __right_size)) ? __y->__right_ : __y->__left_;
	_NodePtr __v = __find_parent(__root, __x->__key_);

	if (__y == __p) {
		__p = __w;
	} else {
		__y = __w;
		__x->__left_ = __z->__left_;
		__x->__right_ = __z->__right_;
		__p = __x;
	}

	while (__v != nullptr) {
		__tree_balance(__root, __v);
		__v = __find_parent(__root, __v->__key_);
	}

	return __z;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) {
		return;
	}
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}

// 아래는 반드시 사용해야하는 AVL-Tree 클래스입니다.
template <class _Tp>
class AVL {
	public: // Member types
		typedef _Tp						key_type;
		typedef std::size_t				size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		AVL(): __root_(nullptr) {}
	
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
			// use __insertAVL or write your own code here
			return __insertAVL(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			// use __eraseAVL or write your own code here
			const_pointer __r = __eraseAVL(__root_, key);

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
	friend std::ostream& operator<<(std::ostream& os, const AVL& tree) {
		os << tree.__root_;
		return os;
	}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* ❗️새로 구현하실 경우, 출력 형식에 주의하세요.❗️
*/
int main(int argc, char **argv) {
	AVL<int>	tree;
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
		}
		std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}