/*
* File Processing Report 2025 - Binary Search Tree skeleton code in C/C++
*/

#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;

// BST node class
template <class _Tp>
class Node {
public: // Member types
	typedef _Tp					__key_type;
	typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;

	unsigned			__height_;
	unsigned			__size_;

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(0), __size_(0) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), __height_(0), __size_(0) {}
};

// return the height of the subtree rooted at __x
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) {
		return 0;
	}
	if (__x->__height_ != 0) {
		return __x->__height_;
	}
	return std::max(__height(__x->__left_), __height(__x->__right_)) + 1;
}

// return the size of the subtree rooted at __x
template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) {
		return 0;
	}
	if (__x->__size_ != 0) {
		return __x->__size_;
	}
	return __size(__x->__left_) + __size(__x->__right_) + 1;
}

// return the node with the minimum key in the subtree rooted at __x
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if (__x == nullptr) {
		return nullptr;
	}
	while (__x->__left_ != nullptr) {
		__x = __x->__left_;
	}
	return __x;
}

// return the node with the maximum key in the subtree rooted at __x
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (__x == nullptr) {
		return nullptr;
	}
	while (__x->__right_ != nullptr) {
		__x = __x->__right_;
	}
	return __x;
}

// output the inorder traversal of the subtree rooted at __x
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) {
		return;
	}
	std::cout << "<";
	__inorder(__x->__left_);
	std::cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
	std::cout << ">";
}


// return the inserted node and whether the insertion was successful
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr;
	std::stack<_NodePtr> stack;

	// step 1: find the insertion point
	while (p != nullptr) {
		if (p->__key_ == key) { // key already exists
			return std::pair<_NodePtr, bool>(p, false);
		}

		q = p;
		stack.push(q); // push the parent node

		if (p->__key_ > key) {
			p = p->__left_;
		}
		else {
			p = p->__right_;
		}
	}

	// step 2: create a new node and insert it
	_NodePtr newNode = new Node<_Tp>(key);
	if (__root == nullptr) {
		__root = newNode;
	}
	else if (q->__key_ > key) {
		q->__left_ = newNode;
	}
	else {
		q->__right_ = newNode;
	}

	// step 3: update height and size of parent nodes
	while (!stack.empty()) {
		q = stack.top();
		stack.pop();
		
		q->__height_ = 1 + std::max(__height(q->__left_), __height(q->__right_));
		q->__size_ = 1 + __size(q->__left_) + __size(q->__right_);
	}

	return std::pair<_NodePtr, bool>(newNode, true);
}

// return the replaced node to delete, or nullptr if the key does not exist
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr;
	std::stack<_NodePtr> stack;

	// step 1: find the node to be deleted
	while (p != nullptr && p->__key_ != key) {
		q = p;
		stack.push(q); // push the parent node

		if (p->__key_ > key) {
			p = p->__left_;
		}
		else {
			p = p->__right_;
		}
	}

	// step 1-1: if the node does not exist, return nullptr
	if (p == nullptr) {
		return nullptr;
	}

	// step 2: replace the node (two children case)
	if (p->__left_ != nullptr && p->__right_ != nullptr) {
		_NodePtr tempNode = p;
		stack.push(p);

		// choose the replacement node based on subtree height and size
		if (p->__left_->__height_ < p->__right_->__height_			// right subtree is taller
			|| (p->__left_->__height_ == p->__right_->__height_		// OR (heights are equal
				&& p->__left_->__size_ < p->__right_->__size_)) {	// AND right subtree is larger)
			// find the minimum node in the right subtree
			p = p->__right_;
			while (p->__left_ != nullptr) {
				stack.push(p);
				p = p->__left_;
			}
		}
		else {
			// find the maximum node in the left subtree
			p = p->__left_;
			while (p->__right_ != nullptr) {
				stack.push(p);
				p = p->__right_;
			}
		}

		// copy the key of the replacement node to the target node
		tempNode->__key_ = p->__key_; 

		// update q to be the parent of p
		if (!stack.empty())
			q = stack.top(); 
		else
			q = nullptr; // p was the root node
	}

	// step 3: unlink the node (zero or one child case)
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

	// step 4: update height and size of parent nodes
	while (!stack.empty()) {
		q = stack.top();
		stack.pop();

		q->__height_ = 1 + std::max(__height(q->__left_), __height(q->__right_));
		q->__size_ = 1 + __size(q->__left_) + __size(q->__right_);
	}

	return p;
}

// clear the subtree rooted at __x
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

// Binary Search Tree class
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

	bool erase(const key_type& key) {
		if (pointer __r = __eraseBST(__root_, key)) { // key found and node to delete returned
			delete __r;
			return true;
		}

		return false; // key does not exist
	}

	void clear() {
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

	tree.clear();

	return (0);
}