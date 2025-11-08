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
};


template <class _NodePtr> // 노드의 키를 구하는 함수
unsigned __height(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) {
		return 0;
	}
	int leftHeight = __height(__x->__left_); //recursive
	int rightHeight = __height(__x->__right_);
	// 노드의 서브트리 중 높이가 더 큰쪽이 높이이므로 자기 자신과 더 높은 서브트리의 높이를 더해준다
	return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) { // 노드의 수를 구하는 함수
	// write your own code here
	if (__x == nullptr) {
		return 0;
	}
	int leftSize = __size(__x->__left_);
	int rightSize = __size(__x->__right_);

	return 1 + leftSize + rightSize; // 자기 자신과 left + right하면 전체 노드 수가 나온다
}
// 함수명 수정했습니다! (minNode , maxNode)
template <class _NodePtr>
_NodePtr __minNode(_NodePtr __x) {
	if (__x == nullptr) { // 노드가 비어있으면 Null
		return nullptr;
	}
	if (__x->__left_ == nullptr) {
		return __x; // p의 자식 노드가 없다면, 그대로 p 노드를 리턴
	}
	while (__x->__left_ != nullptr) {
		__x = __x->__left_; // 계속해서 왼쪽으로 내려가서 최소 노드를 찾는다
	}
	return __x;
}

template <class _NodePtr>
_NodePtr __maxNode(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) {
		return nullptr; // 노드가 비어있으면 Null
	}
	if (__x->__right_ == nullptr) {
		return __x; // p의 자식 노드가 없다면, 그대로 p 노드를 리턴
	}
	while (__x->__right_ != nullptr) {
		__x = __x->__right_; // 계속해서 오른쪽으로 내려가서 최대 노드를 찾는다
	}
	return __x;
}


template <class _NodePtr>
void __inorder(_NodePtr __x) { // left -> root -> right : inOrder(중위순회)
	if (__x != nullptr) {
		std::cout << "<";
		__inorder(__x->__left_);
		std::cout << " " << __x->__key_ << " "; // key 값 앞 뒤로 공백
		__inorder(__x->__right_);
		std::cout << ">";
	}
}


template <class _NodePtr, class _Tp>
_NodePtr getNodeBST() { // 새 노드 생성
	return new Node<_Tp>();
}

template <class _NodePtr, class _Tp>
_NodePtr searchBST(_NodePtr __root, const _Tp& key) {
	_NodePtr p = __root;
	if (p == nullptr) {
		return nullptr; // 초기에 루트 노드가 비어있거나, 혹은 검색 실패한다면 false를 리턴한다
	}
	if (key == p->__key_) { // 검색 성공 , p에서 탐색할려는 key값 발견
		return p;
	}
	if (key < p->__key_) { // 왼쪽으로 탐색 (recursive)
		return searchBST(p->__left_, key);
	}
	else { // 오른쪽으로 탐색 (recursive)
		return searchBST(p->__right_, key);
	}
}

template <class _NodePtr, class _Tp>
_NodePtr searchParent(_NodePtr __root, const _Tp& key) {
	_NodePtr q = nullptr; // q가 p의 부모 노드
	_NodePtr p = __root;
	while (p != nullptr) {
		if (key == p->__key_) {
			return q; // 검색 성공 (nullptr case -> 루트 노드의 key값이 searchKey와 동일한 경우)
		}
		q = p;
		if (key < p->__key_) {
			p = p->__left_; // 왼쪽 탐색
		}
		else {
			p = p->__right_; // 오른쪽 탐색
		}
	}
	return nullptr; // 검색 실패
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr q = nullptr; // q가 p의 부모 노드
	_NodePtr p = __root;
	while (p != nullptr) { // p가 null 값이 될때까지, 계속해서 탐색
		if (key == p->__key_) {
			return std::make_pair(p, false);
		}
		q = p;
		if (key < p->__key_) { // p의 key보다 작으면 왼쪽으로 내려감
			p = p->__left_;
		}
		else { // p의 key보다 크면 오른쪽으로 내려감
			p = p->__right_;
		}
	}
	// 새로운 key값을 가진 새 노드 생성 (use getNodeBST)
	_NodePtr newNode = getNodeBST<_NodePtr, _Tp>();
	newNode->__key_ = key;

	if (__root == nullptr) { // root가 비어있다면 새노드를 루트 노드로 만들기
		__root = newNode;
		return std::make_pair(__root, true);
	}
	else if (key < q->__key_) { // key가 q의 key보다 작다면, q의 left에 새 노드를 담는다
		q->__left_ = newNode;
	}
	else { // key가 q의 key보다 크다면, q의 right에 새노드를 담는다
		q->__right_ = newNode;
	}
	return std::make_pair(newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = searchBST(__root, key);
	_NodePtr q = searchParent(__root, key);
	_NodePtr r = nullptr;
	char flag;
	if (p == nullptr) {
		return nullptr; // 삭제할 노드가 없는 경우
	}
	// 루트 노드를 삭제해야하는 경우
	else if (q == nullptr && p != nullptr) {
		if (p->__left_ == nullptr && p->__right_ == nullptr) { // 루트가 리프 노드
			_NodePtr s = p; // nullptr인 경우 delete 실패와 겹칠 수 있으므로 p를 k에 임시 저장
			delete p;
			__root = nullptr;
			return s;
		}
		else if (p->__left_ == nullptr || p->__right_ == nullptr) { // 루트 노드의 차수가 1인 경우
			_NodePtr s = ((p->__left_ != nullptr) ? p->__left_ : p->__right_); //왼쪽, 오른쪽 자식 중 존재하는 자식 노드를 루트 노드로 만들기 
			delete p;
			__root = s;
			return s;
		}
		else { // 루트 노드의 차수가 2인 경우 (balance를 맞추기 위해 높이가 더 높은 노드 쪽에서 가져온다)
			if (__height(p->__left_) > __height(p->__right_)) {
				r = __maxNode(p->__left_);
				flag = 'L';
			}
			else if (__height(p->__left_) < __height(p->__right_)) {
				r = __minNode(p->__right_);
				flag = 'R';
			}
			else { // 높이가 같은 경우, 노드 수가 많은 쪽에서 노드를 가져온다
				if (__size(p->__left_) >= __size(p->__right_)) {
					r = __maxNode(p->__left_);
					flag = 'L';
				}
				else {
					r = __minNode(p->__right_);
					flag = 'R';
				}
			}
			p->__key_ = r->__key_;
			if (flag == 'L') {
				__eraseBST(p->__left_, r->__key_);
			}
			else {
				__eraseBST(p->__right_, r->__key_);
			}
			return p;
		}
	}

	else {
		if (p->__left_ == nullptr && p->__right_ == nullptr) { // 삭제할 노드가 리프 노드인 경우에는 해당 노드만 삭제해준다
			if (q->__left_ == p) {
				q->__left_ = nullptr;
			}
			else {
				q->__right_ = nullptr;
			}
			delete p;
		}  
		// 삭제할 노드의 차수가 1인 경우 (포인터를 옮겨주면 된다)
		else if ((p->__left_ == nullptr && p->__right_ != nullptr) || (p->__left_ != nullptr && p->__right_ == nullptr)) {
			if (p->__left_ != nullptr) {
				if (q->__left_ == p) {
					q->__left_ = p->__left_;
				}
				else {
					q->__right_ = p->__left_;
				}
			}
			else {
				if (q->__left_ == p) {
					q->__left_ = p->__right_;
				}
				else {
					q->__right_ = p->__right_;
				}
			}
			delete p;
		}
		// 삭제할 노드의 차수가 2인 경우 (balance를 맞추기 위해 높이가 더 높은 노드 쪽에서 가져온다)
		else if (p->__left_ != nullptr && p->__right_ != nullptr) {
			if (__height(p->__left_) > __height(p->__right_)) {
				r = __maxNode(p->__left_);
				flag = 'L';
			}
			else if (__height(p->__left_) < __height(p->__right_)) {
				r = __minNode(p->__right_);
				flag = 'R';
			}
			else { // 높이가 같은 경우, 노드 수가 많은 쪽에서 노드를 가져온다
				if (__size(p->__left_) >= __size(p->__right_)) {
					r = __maxNode(p->__left_);
					flag = 'L';
				}
				else {
					r = __minNode(p->__right_);
					flag = 'R';
				}
			}

			p->__key_ = r->__key_;
			if (flag == 'L') {
				__eraseBST(p->__left_, r->__key_);
			}
			else {
				__eraseBST(p->__right_, r->__key_);
			}
		} 
		return p;
	}
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) { // 후위순회로 삭제 (왼쪽 끝부터 -> 오른쪽 끝 삭제 후 루트 노드 삭제)
	if (__x == nullptr) {
		return;
	}
	__clear(__x->__left_);
	__clear(__x->__right_);

	delete __x;
	__x = nullptr; // Dangling pointer 방지하기 위해
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
		return __eraseBST(__root_, key);
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
		tree.inorder();
	}

	// 메모리 해제
	tree.clear();

	return 0;
}