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

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// write your own code here
		if (!np) return os; //null이면 아무것도 출력하지 않음
		os << "<"; // 괄호 열기
		os << np->__left_; // 왼쪽 서브트리 출력
		os << " " << np->__key_ << " "; // 키값 출력 (공백 포함)
		os << np->__right_; // 오른쪽 서브트리 출력
		os << ">"; // 괄호 닫기
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
	if (__x== nullptr) return 0; // null은 높이 0
	unsigned left_h = __height(__x->__left_); // 왼쪽 서브트리 높이 계산
	unsigned right_h = __height(__x->__right_); // 오른쪽 서브트리 높이 계산

	// 더 큰 쪽에 1 더해서 노드 높이 결정
	if (left_h > right_h)
		return left_h + 1;
	else
		return right_h + 1;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return 0; // null은 크기 0
	return 1 + __size(__x -> __left_) + __size(__x ->__right_); // (현재 노드) 1 + 왼쪽크기 + 오른쪽크기
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return nullptr; // null은 nullptr 반환
	while (__x->__left_) __x = __x -> __left_; // 왼쪽 연결노드가 없을 떄까지 내려감
	return __x; // 가장 왼쪽 노드 반환
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr) return nullptr; // null은 nullptr 반환
	while (__x -> __right_) __x = __x -> __right_; // 오른쪽 연결노드가 없을 떄까지 내려감
	return __x; // 가장 오른쪽 노드 반환
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// write your own code here
	if (__x == nullptr ) return; // null이면 종료
	__inorder(__x -> __left_); // 왼쪽 서브트리 순회
	std::cout << __x -> __key_ << " "; // 현재노드 key 출력
	__inorder(__x -> __right_); // 오른쪽 서브트리 순회
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	if (__root == nullptr) { // null이면 새 노드 생성
		__root = new Node<_Tp>(key);
		return std::pair<_NodePtr, bool>(__root, true);
	} 
	if (key < __root->__key_) { // key가 현재노드 key보다 작으면 왼쪽 서브트리에 삽입
		auto ret = __insertBST(__root->__left_, key);
		return ret;
	}
	else if (key > __root->__key_) { // 크면 오른쪽 서브트리에
		auto ret = __insertBST(__root->__right_, key);
		return ret;
	}
	else { // 같은 값 존재
		return std::pair<_NodePtr, bool>(__root, false);
	}
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	if (__root == nullptr) 
		return nullptr; // null이면 nullptr

	if (key < __root->__key_) { // key가 현대노드 key보다 작으면 왼쪽 서브트리에서 삭제
		return __eraseBST(__root->__left_, key);
	}
	else if (key > __root->__key_) { // 크면 오른쪽 서브트리에서 삭제
		return __eraseBST(__root->__right_, key);
	}
	else { // key = 현재노드 key
		if (__root->__left_ == nullptr) { // 왼쪽 노드 없으면 오른쪽으로 대체
			_NodePtr tmp = __root;
			__root = __root->__right_;
			return tmp;
		}
		else if (__root->__right_ == nullptr) { // 오른쪽 노드 없으면 왼쪽으로 대체
			_NodePtr tmp = __root;
			__root = __root->__left_;
			return tmp;
		}
		else { // 둘 다 노드 있으면 오른쪽 서브트리 최소 노드로
			_NodePtr minNode = __tree_min(__root->__right_);
			__root->__key_ = minNode->__key_;
			return __eraseBST(__root->__right_, minNode->__key_);
		}
	}
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// write your own code here
	if (__x == nullptr) return; // null이면 종료
	__clear(__x->__left_); // 왼쪽 서브트리 메모리 삭제
	__clear(__x->__right_); // 오른쪽 서브트리 메모리 삭제
	delete __x; // 현재노드 삭제
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
		return __height(__root_); // 트리 높이 반환
	}

	size_type size() const {
		// use __size or write your own code here
		return __size(__root_); // 전체 노드 개수 반환
	}

public: // Lookup
	void inorder() const {
		// use __inorder or write your own code here
		__inorder(__root_); // 중위 순회
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		// use __insertBST or write your own code here
		return __insertBST(__root_, key); //키 값을 가진 노드 삽입
	}

	const_pointer erase(const key_type& key) {
		// use __eraseBST or write your own code here
		pointer __r = __eraseBST(__root_, key); // 키 값에 해당하는 노드 삭제

		// Client still needs to destruct/deallocate it
		// Or memory leak will occur
		delete __r;

		return __r;
	}

	void clear() {
		// use __clear or write your own code here
		__clear(__root_); // 전체 메모리 삭제
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