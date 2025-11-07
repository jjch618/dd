#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;

// class Node start
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
};// end Node



template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	// if node is leaf: height is 0
	if (__x == nullptr) return 0;

	// height of left
	std::size_t h1 = __height(__x->__left_);
	// height of right
	std::size_t h2 = __height(__x->__right_);
	// return bigger value
	return (h1 > h2) ? h1 + 1 : h2 + 1;
} // end __height

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	// if node is leaf: size is 1
	if (__x == nullptr) return 0;

	// size of left
	std::size_t s1 = __size(__x->__left_);
	// height of right
	std::size_t s2 = __size(__x->__right_);

	// result is left size + right size + 1
	return s1 + s2 + 1;
} // end__size()

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// current pointer is mean value
	if (__x->__left_ == nullptr) return __x;
	// next left child is mean value
	else return __tree_min(__x->__left_);
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	// current pointer is max value
	if (__x->__right_ == nullptr) return __x;
	// next left child is max value
	else return __tree_max(__x->__right_);
}

template <class _NodePtr>
void __inorder(_NodePtr& __x) {
	// empty node
	if (__x == nullptr) return;

	// inorder traversal
	// output:<{left subtree}‘ ’{key}‘ ’{right subtree}>
	std::cout << '<';
	__inorder(__x->__left_);
	std::cout << ' ' << __x->__key_ << ' ';
	__inorder(__x->__right_);
	std::cout << '>';

	return;
}

// util functions
template < class _Tp>
Node<_Tp>* getNodeBST() {
	return new Node<_Tp>();
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// if tree is empty
	if (__root == nullptr) {
		__root = getNodeBST<_Tp>();
		__root->__key_ = key;
		return std::pair<_NodePtr, bool>(__root, true);
	}

	// q is parent node
	_NodePtr q = nullptr;
	// p is child node
	_NodePtr p = __root;
	// isLeft is flag of new node's direction
	bool isLeft = false;

	// find key's location
	while (p != nullptr) {
		// case0: alreay key is in tree
		if (p->__key_ == key) return std::pair<_NodePtr, bool>(p, false);

		// key is smaller than p
		if (p->__key_ > key) {
			q = p;
			p = p->__left_;
			isLeft = true;
		}
		// key is bigger than p
		else {
			q = p;
			p = p->__right_;
			isLeft = false;
		}
	}

	// new Node(key) >>>> getNodeBST()로 바꿔야 함----------------------------------------------------------------------------------------------------------------------------------------
	// new node is left chlid
	if (isLeft) {
		q->__left_ = getNodeBST<_Tp>();
		q->__left_->__key_ = key;
		return std::pair<_NodePtr, bool>(q->__left_, true);
	}
	// new node is right child
	else {
		q->__right_ = getNodeBST<_Tp>();
		q->__right_->__key_ = key;
		return std::pair<_NodePtr, bool>(q->__right_, true);
	}
}

template <class _NodePtr, class _Tp>
bool __eraseBST(_NodePtr& __root, const _Tp& key) {
	// q is parent node
	_NodePtr q = nullptr;
	// p is child node
	_NodePtr p = __root;
	// isLeft is flag of new node's direction
	bool isLeft = false;

	// find target
	while (p != nullptr && p->__key_ != key) {
		// key is smaller than p
		if (key < p->__key_) {
			q = p;
			p = p->__left_;
			isLeft = true;
		}
		// key is bigger than p
		else {
			q = p;
			p = p->__right_;
			isLeft = false;
		}
	}

	// case0: p is not in tree
	if (p == nullptr) return false;

	// case1: p's degree is 0
	if (p->__left_ == nullptr && p->__right_ == nullptr) {

		// case1-1: p is root node
		if (q == nullptr) {
			delete p;
			p = nullptr;
			__root = nullptr;
			return true;
		}

		// caes1-2: p is leaf node
		delete p;
		p = nullptr;
		// p is left child node
		if (isLeft) q->__left_ = nullptr;
		// p is right child node
		else q->__right_ = nullptr;

		return true;
	}// end case1

	// case2: p is not leaf node and p's degree is 1
	if (p->__left_ == nullptr || p->__right_ == nullptr) {

		// case2-1: p is root node
		if (q == nullptr) {
			// p(__root) has a left child
			if (p->__left_ != nullptr) {
				__root = p->__left_;
				delete p;
				p = nullptr;
				q = __root;
				return true;
			}
			// p(__root) has a right child
			else {
				__root = p->__right_;
				delete p;
				p = nullptr;
				q = __root;
				return true;
			}
		}

		// case2-2: p is q's left chlid
		if (isLeft) {
			// p has a left child
			if (p->__left_ != nullptr) {
				q->__left_ = p->__left_;
				delete p;
				p = nullptr;
				return true;
			}
			// p has a right child
			else {
				q->__left_ = p->__right_;
				delete p;
				p = nullptr;
				return true;
			}
		}
		// case2-3: p is q's right child
		else {
			// p has a left child
			if (p->__left_ != nullptr) {
				q->__right_ = p->__left_;
				delete p;
				p = nullptr;
				return true;
			}
			// p has a right child
			else {
				q->__right_ = p->__right_;
				delete p;
				p = nullptr;
				return true;
			}
		}
	}

	// case3: p is not leaf node and p's degree is 2
	int leftHeight = __height(p->__left_);
	int rightHeight = __height(p->__right_);

	// case3-1: left subtree is taller than right subtree
	if (leftHeight > rightHeight) {
		_NodePtr maxNode = __tree_max(p->__left_);
		p->__key_ = maxNode->__key_;
		__eraseBST(p->__left_, maxNode->__key_);
		return true;
	}
	// case3-2: left subtree is smaller than right subtree
	else if (leftHeight < rightHeight) {
		_NodePtr minNode = __tree_min(p->__right_);
		p->__key_ = minNode->__key_;
		__eraseBST(p->__right_, minNode->__key_);
		return true;
	}
	// case3-3: left subtree and right subtree have same height
	else {
		int leftSize = __size(p->__left_);
		int rightSize = __size(p->__right_);
		if (leftSize >= rightSize) {
			_NodePtr maxNode = __tree_max(p->__left_);
			p->__key_ = maxNode->__key_;
			__eraseBST(p->__left_, maxNode->__key_);
			return true;
		}
		else {
			_NodePtr minNode = __tree_min(p->__right_);
			p->__key_ = minNode->__key_;
			__eraseBST(p->__right_, minNode->__key_);
			return true;
		}
	}
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// base case
	if (__x == nullptr) return;

	// postorder traversal
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
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


public: // Capacity
	size_type height() const {
		// use __height or write your own code here
		// code is on __height() in class node
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

	bool erase(const key_type& key) {
		// use __eraseBST or write your own code here
		return __eraseBST(__root_, key);
	}

	void clear() {
		// use __clear or write your own code here
		__clear(__root_);
	}

};

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
			if (tree.erase(key) == false) {
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