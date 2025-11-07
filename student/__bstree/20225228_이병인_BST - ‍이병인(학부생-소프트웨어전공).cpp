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

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (np == nullptr) {
			// 빈 서브트리는 아무 것도 출력하지 않음 (중요!)
			return os;
		}
		os << "<";
		if (np->__left_)   os << np->__left_;
		os << " " << np->__key_ << " ";
		if (np->__right_)  os << np->__right_;
		os << ">";
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr node) {
	// write your own code here
	//노드의 높이를 재귀로 계산한다.
	if (node == nullptr) return 0;
	unsigned left_h = __height(node->__left_);
	unsigned right_h = __height(node->__right_);
	if (left_h > right_h)
		return left_h + 1;
	else
		return right_h + 1;
}

template <class _NodePtr>
unsigned __size(_NodePtr node) {
	// write your own code here
	//트리내의 전체 노드 개수를 계산한다
	if (node == nullptr) return 0;
	return 1 + __size(node->__left_) + __size(node->__right_);
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr node) {
	// write your own code here
	//트리에서 가장 작은 키값을 가진 노드를 찾는다.왼쪽 자식이 없을때까지 이동한다
	if (node == nullptr) return nullptr;
	while (node->__left_ != nullptr)
		node = node->__left_;
	return node;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr node) {
	// write your own code here
	//트리에서 가장 큰 키값을 가진 노드를 찾는다.오른쪽 자식이 없을때 까지 이동한다.
	if (node == nullptr) return nullptr;
	while (node->__right_ != nullptr)
		node = node->__right_;
	return node;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr node) {
	std::cout << "<";
	if (node != nullptr) {
		if (node->__left_ != nullptr)
			__inorder(node->__left_);
		std::cout << " " << node->__key_ << " ";
		if (node->__right_ != nullptr)
			__inorder(node->__right_);
	}
	std::cout << ">";
}

template <class _Tp>
Node<_Tp>* getNodeBST() {
	Node<_Tp>* p = new Node<_Tp>();
	return p;
}
/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr q = nullptr; //부모노드
	_NodePtr p = __root;  //현재 탐색중인 노드

	while (p != nullptr) { //삽입할 위치  탐색
		if (key == p->__key_) { //중복 키 존재할경우
			return std::make_pair(p, false);
		}
		q = p;
		if (key < p->__key_)
			p = p->__left_;
		else
			p = p->__right_;
	}

	_NodePtr newNode = getNodeBST<_Tp>(); //새 노드 생성
	newNode->__key_ = key;
	newNode->__left_ = nullptr;
	newNode->__right_ = nullptr;

	if (__root == nullptr) { //트리가 비어있는 경우
		__root = newNode; //새노드가 루트가 된다.
	}else if (key < q->__key_) {
		q->__left_ = newNode; //부모의 왼쪽에 삽입한다.
	}else {
	q->__right_ = newNode; //부모의 오른쪽에 삽입한다.
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
	// write your own code here

	_NodePtr p = __root;
	_NodePtr q = nullptr;

	while (p != nullptr && p->__key_ != key) { //삭제할 노드 탐색
		q = p;
		if (key < p->__key_) p = p->__left_;
		else                 p = p->__right_;
	}

	if (p == nullptr) return nullptr; //트리에 존재하지 않는다면 null포인터 반환

	if (p->__left_ == nullptr && p->__right_ == nullptr) { //자식이 없는 노드의 경우
		if (q == nullptr) { //루트가 유일한 노드인 경우
			__root = nullptr;
		}
		else if (q->__left_ == p) { //부모의 왼쪽 자식 제거
			q->__left_ = nullptr;
		}
		else { //부모의 오른쪽 자식 제거
			q->__right_ = nullptr;
		}
		return p;
	}

	//자식이 하나만 있는경우
	if (p->__left_ == nullptr || p->__right_ == nullptr) {
		_NodePtr child = (p->__left_ != nullptr) ? p->__left_ : p->__right_;
		if (q == nullptr) {
			__root = child; //루트를 바꾼다
		}else if (q->__left_ == p) {
			q->__left_ = child;
		}else {
			q->__right_ = child;
		}
		return p;
	}

	//자식이 두개인 경우
	_NodePtr r = nullptr;
	int from_left = 0;
	// 왼쪽, 오른쪽 서브트리의 높이를 비교하여 대체 노드 선택한다.
	unsigned left_height = __height(p->__left_);
	unsigned right_height = __height(p->__right_);
	if (left_height > right_height) {// 왼쪽이 더 높을 경우 왼쪽 서브트리의 최대 노드를 선택
		r = __tree_max(p->__left_);
		from_left = 1;
	}else if (left_height < right_height) {// 오른쪽이 더 높을 경우 오른쪽 서브트리의 최소 노드를 선택
		r = __tree_min(p->__right_);
		from_left = 0;
	}
	else {
		// 높이가 같을 경우 size를 비교하여 더 큰 쪽에서 선택한다
		if (__size(p->__left_) >= __size(p->__right_)) {
			r = __tree_max(p->__left_);
			from_left = 1;
		}else {
			r = __tree_min(p->__right_);
			from_left = 0;
		}
	}

	p->__key_ = r->__key_; //대체 노드의 키로 현재노드를 갱신한다.

	if (from_left) return __eraseBST(p->__left_, r->__key_);  // 대체노드 삭제
	else           return __eraseBST(p->__right_, r->__key_);  


}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& node) {
	// write your own code here
	if (node == nullptr) return;
	__clear(node->__left_);
	__clear(node->__right_);
	delete node;
	node = nullptr;
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
		if (tree.__root_ == nullptr) {
			// os << "<>";  // <-- 이거 금지
			return os;      // 빈 줄을 기대하므로 아무 것도 출력하지 않음
		}
		os << tree.__root_;   // Node 쪽 operator<<가 재귀적으로 호출됨
		return os;
	}
};

/*
* 아래는 추천드리는 main 함수의 예시입니다.
* 반드시 아래의 main 함수를 사용해야할 필요는 없습니다.
* ❗️새로 구현하실 경우, 출력 형식에 주의하세요.❗️
*/
int main(int /*argc*/, char** /*argv*/) {
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