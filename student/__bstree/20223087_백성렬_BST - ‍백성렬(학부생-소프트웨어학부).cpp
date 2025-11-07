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
	
	// inorder함수를 사용할 것이기 때문에 주석처리
	//friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
	//	// write your own code here
	//}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _Tp>
Node<_Tp>* getBSTNode() { // 새로운 노드를 생성한 후  그 포인터를 반환하는 getBSTNode함수
	return new Node<_Tp>(); 
}

template <class _NodePtr>
unsigned __height(_NodePtr T) { // 매개변수 트리 T의 높이를 반환하는 함수
	if (T == nullptr) return (unsigned)0; // basecase: 트리가 비어있을경우 높이 0을 리턴
	unsigned leftHeight = __height(T->__left_); // 왼쪽 자식 트리의 높이
	unsigned rightHeight = __height(T->__right_); // 오른쪽 자식 트리의 높이
	return (unsigned)1 + (leftHeight > rightHeight ? leftHeight : rightHeight); // 왼쪽 자식의 높이와 오른쪽 자식의 높이중 큰 값에 1을 더한 높이를 리턴
}

template <class _NodePtr>
unsigned __size(_NodePtr T) { // 매개변수 트리 T의 사이즈 즉, 포함한 노드의 개수를 반환하는 함수
	if (T == nullptr) return (unsigned)0; // basecase: 트리가 비어있을경우 사이즈 0을 리턴
	return (unsigned)1 + __size(T->__left_) + __size(T->__right_); // 왼쪽 자식의 사이즈와 오른쪽 자식의 사이즈, 1(자기 자신이 가진 노드)를 모두 더한 값 리턴
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr T) { // 매개변수 트리 T의 노드중 키값이 최소인 노드의 포인터를 반환하는 함수
	_NodePtr p = T;
	while (p->__left_ != nullptr) { // 노드의 왼쪽 자식은 항상 자신의 키값보다 작기 때문에 왼쪽 자식이 없을때까지 접근
		p = p->__left_;
	}
	return p;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) { // 매개변수 트리 T의 노드중 키값이 최대인 노드의 포인터를 반환하는 함수
	_NodePtr p = __x;
	while (p->__right_ != nullptr) { // 노드의 오른쪽 자식은 항상 자신의 키값보다 작기 때문에 오른쪽 자식이 없을때까지 접근
		p = p->__right_;
	}
	return p;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* 잘못된 출력 형식은 0점 처리됩니다.
*/
template <class _NodePtr>
void __inorder(_NodePtr T) { // pdf에 명시된 형식에 따라 중위순회로 구현
	_NodePtr p = T;
	if (p == nullptr) return;
	std::cout << '<';
	__inorder(p->__left_); // 자신의 키값을 출력하기 전에 왼쪽 자식 먼저 출력 
	std::cout << ' ' << p->__key_ << ' ';
	__inorder(p->__right_); // 자신의 키 값을 출력 후 오른쪽 자식 출력
	std::cout << '>';
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& T, const _Tp& key) { // __root가 nullptr일 경우 __root 포인터를 newNode의 포인터로 변경하기 위해 포인터 참조로 변경하였습니다. 
	_NodePtr p = T;
	_NodePtr q = nullptr; //q는 p의 부모 노드

	while (p != nullptr) { // p가 새로운 키값이 삽입되어야 하는 위치로 갈 때 까지 반복
		if (key == p->__key_) return std::pair<_NodePtr, bool>(p, false); // 삽입하는 키값이 이미 존재하는경우 std::pair<_NodePtr, bool>(__p, false)리턴

		q = p;

		if (key < p->__key_) p = p->__left_; // 삽입하는 키 값이 현재 노드의 키값보다 작을경우 왼쪽 자식으로 접근
		else p = p->__right_; // 삽입하는 키 값이 현재 노드의 키값보다 클경우 오른쪽 자식으로 접근
	}

	// 새로운 노드 생성
	_NodePtr newNode = getBSTNode<_Tp>(); 
	newNode->__key_ = key; // 새로운 노드의 키 값 세팅

	// 새로운 노드를 q(q는 키 값이 삽입되어야 하는 위치인 p의 부모노드)의 자식노드로 삽입
	if (T == nullptr) T = newNode; // 트리가 비어있을경우 삽입되는 키 값을 가진 노드 하나만 가진다
	else if (key < q->__key_) q->__left_ = newNode; // 키 값이 q의 키 값 보다 작을경우(p가 q의 왼쪽 자식일 경우) 새로운 노드를 왼쪽 자식노드에 삽입
	else q->__right_ = newNode; // 키 값이 q의 키 값 보다 클경우(p가 q의 오른쪽 자식일 경우) 새로운 노드를 오른쪽 자식노드에 삽입

	return std::pair<_NodePtr, bool>(newNode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
bool __eraseBST(_NodePtr& T, const _Tp& key) { // erase(const key_type& key)를 통해 이 함수를 호출할 때 메모리를 두 번 지우는것을 막기 위해 bool값을 리턴하도록 변경
	_NodePtr p = T;
	_NodePtr q = nullptr; // q는 p의 부모 노드

	// 삭제할 키 값을 가진 노드 탐색
	while (p != nullptr && key != p->__key_) { // p가 null값이거나(삭제할 키 값을 가진 노드가 존재하지 않는 경우) 삭제하려는 key값을 가진 노드를 찾을 때까지 반복
		q = p;

		if (key < p->__key_) p = p->__left_; // 삭제하는 키 값이 현재 노드의 키값보다 작을경우 왼쪽 자식으로 접근
		else p = p->__right_; // 삭제하는 키 값이 현재 노드의 키값보다 클경우 오른쪽 자식으로 접근
	}

	if (p == nullptr) return false; // p가 가르키는 노드가 null이라면 삭제할 키 값을 가진 노드가 존재하지 않는다는 것이므로 false 리턴
	
	if (p->__left_ != nullptr && p->__right_ != nullptr) { // 삭제할 노드가 왼쪽 자식, 오른 자식을 모두 가진 경우

		// 왼쪽, 오른쪽 자식중 높이가 큰 자식 선택, 높이가 같다면 트리의 크기가(노드의 개수가)큰 자식 선택
		if (__height(p->__left_) < __height(p->__right_) || (__height(p->__left_) == __height(p->__right_) && __size(p->__left_) < __size(p->__right_))) { // 오른쪽 자식이 선택된 경우
			p->__key_ = __tree_min(p->__right_)->__key_; // 삭제할 키 값을 오른쪽 자식의 최소값으로 바꾸고 
			__eraseBST(p->__right_, p->__key_); // 오른쪽 자식에서 최소값을 삭제
		}
		else { // 왼쪽 자식이 선택된 경우
			p->__key_ = __tree_max(p->__left_)->__key_; // 삭제할 키 값을 왼쪽 자식의 최대값으로 바꾸고
			__eraseBST(p->__left_, p->__key_); // 왼쪽 자식에서 최대값을 삭제
		}

		// 자식 노드에서 최대, 최소값을 가진 노드를 삭제할 때 메모리를 delete하므로 따로 메모리 delete하지 않는다
		return true; // 삭제에 성공하였으므로 true리턴
	}

	// 삭제할 노드의 자식 노드가 1개 or 2개인 경우
	if (p->__left_ != nullptr) { // 왼쪽 자식이 존재하는 경우
		if (q == nullptr) T = T->__left_; // q가 nullprt즉, p가 루트 노드일경우 루트노드의 왼쪽 자식 노드를 루트노드로 변경
		else if (q->__left_ == p) q->__left_ = p->__left_; // p(삭제할 노드)가 그 보모노드인 q의 왼쪽 자식노드인 경우 삭제할 노드의 왼쪽 자식을 부모노드의 왼쪽 자식이 가리키도록 변경
		else q->__right_ = p->__left_; // p(삭제할 노드)가 그 보모노드인 q의 오른쪽 자식노드인 경우 삭제할 노드의 왼쪽 자식을 부모노드의 오른쪽 자식이 가리키도록 변경
	}
	else if (p->__right_ != nullptr) { // 오른쪽 자식이 존재하는 경우
		if (q == nullptr) T = T->__right_; // q가 nullprt즉, p가 루트 노드일경우 루트노드의 오른쪽 자식 노드를 루트노드로 변경
		else if (q->__left_ == p) q->__left_ = p->__right_; // p(삭제할 노드)가 그 보모노드인 q의 왼쪽 자식노드인 경우 삭제할 노드의 오른쪽 자식을 부모노드의 왼쪽 자식이 가리키도록 변경
		else q->__right_ = p->__right_; // p(삭제할 노드)가 그 보모노드인 q의 오른쪽 자식노드인 경우 삭제할 노드의 오른쪽 자식을 부모노드의 오른쪽 자식이 가리키도록 변경
	}
	else { // 자식이 없는 경우
		if (q == nullptr) T = nullptr; // q가 nullprt즉, p가 루트 노드일경우 루트노드를 nullptr로 변경
		else if (q->__left_ == p) q->__left_ = nullptr; // p(삭제할 노드)가 부모노드의 왼쪽 자식인 경우 부모노드의 왼쪽 자식을 null 포인터로 변경
		else q->__right_ = nullptr; // p(삭제할 노드)가 부모노드의 오른쪽 자식인 경우 부모노드의 오른쪽 자식을 null 포인터로 변경
	}

	delete p; // 이제 p(삭제할 노드)를 가리키는 노드가 없기 때문에 메모리 누수 방지를 위해 delete

	return true;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) { // 트리의 모든 노드를 delete하는 함수를 재귀적으로 구현
	if (__x == nullptr) return; // basecase: delete할 노드가 비어있는경우 함수 나가기
	__clear(__x->__left_); // 메모리를 delete하기 전에 왼쪽 자식의 메모리 먼저 delete
	__clear(__x->__right_); // 메모리를 delete하기 전에 오른쪽 자식의 메모리 먼저 delete
	delete __x; // 자식 노드의 메모리를 모두 delete한 후에 자신의 메모리 delete
	__x = nullptr; // __x는 여전히 delete된 메모리의 주소를 가리키고 있기 때문에 nullptr로 변경
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
		// __eraseBST의 반환값을 bool값으로 변경하였기 때문에 따로 메모리를 delete할 필요 없음
		bool __r = __eraseBST(__root_, key);

		return __r;
	}

	void clear() {
		__clear(__root_);
	}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/

	// __inorder함수를 사용할 것이기 때문에 주석처리
	//friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
	//	os << tree.__root_;
	//	return os;
	//}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* 새로 구현하실 경우, 출력 형식에 주의하세요.
*/
int main() {
	BST<int>	tree;
	char		command; /*godyescircle. tearang. cheer_up*/
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

		// inorder함수를 사용할 것이기 때문에 주석처리
		//std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}