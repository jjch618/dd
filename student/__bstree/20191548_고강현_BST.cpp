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

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// write your own code here
		if(np == nullptr) return os;
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
	if(__x == nullptr) return 0;

	// 왼쪽 오른쪽 중 높은 높이를 반환하고 1을 더한다.
	return std::max(__height(__x->__left_), __height(__x->__right_)) + 1;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if(__x == nullptr) return 0;

	// 재귀로 오른쪽 왼쪽을 찾아들어가면서 노드 갯수를 센다.
	return __size(__x->__left_) + __size(__x->__right_) + 1;
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if(__x == nullptr) return nullptr;

	// 왼쪽 자식이 존재하면 계속 왼쪽 노드로 이동한다.
	return __x->__left_ == nullptr ? __x : __tree_min(__x->__left_);
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if(__x == nullptr) return nullptr;

	// 오른쪽 자식이 존재하면 계속 오른쪽 노드로 이동한다.
	return __x->__right_ == nullptr ? __x : __tree_max(__x->__right_);
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// write your own code here
	_NodePtr q = nullptr, p = __root;
	while(p != nullptr) {
		// 이미 같은 키가 존재하면 삽입 실패처리한다.
		if(key == p->__key_) return std::pair<_NodePtr, bool>(nullptr, false);

		q = p;
		if(key < p->__key_) p = p->__left_;
		else p = p->__right_;
	}

	_NodePtr newNode = new Node<_Tp>(key);

	if(__root == nullptr) __root = newNode;
	else if(key < q->__key_) q->__left_ = newNode;
	else q->__right_ = newNode;

	return std::pair<_NodePtr, bool>(newNode, true);
}

/* 
아래는 key에 따른 위치를 검색하는 함수입니다.
부모와 자식 노드를 반환합니다.
찾지 못할시 nullptr을 반환합니다.
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, _NodePtr> __searchBST(_NodePtr __root, const _Tp& key) {
	// write your own code here
	_NodePtr p = __root, q = nullptr;
	while(p != nullptr) {
		if(key == p->__key_) return std::pair<_NodePtr, _NodePtr>(q, p);
		q = p;
		if(key < p->__key_) p = p->__left_;
		else p = p->__right_;
	}
	return std::pair<_NodePtr, _NodePtr>(nullptr, nullptr);
}
/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) { // &가 빠져있어서 참조 타입으로 받아 함수 내에서 수정 가능하게 변경
	// write your own code here
	_NodePtr q, p, r;
	bool is_left = false;
	// 부모, 자식 노드를 가져오는 함수 실행
	std::pair<_NodePtr, _NodePtr> searchResult = __searchBST(__root, key);
	q = searchResult.first; // 부모 노드
	p = searchResult.second; // 삭제할 노드

	if(p == nullptr) return nullptr; // 삭제할 노드가 없으면 nullptr 반환
	
	if(p->__left_ == nullptr && p->__right_ == nullptr){ // 차수가 0인 경우
		if(q == nullptr) __root = nullptr; // 삭제할 노드가 루트일때 루트를 nullptr로 변경
		else if(q->__left_ == p) q->__left_ = nullptr; // 부모 노드의 왼쪽 자식을 nullptr로 변경
		else q->__right_ = nullptr; // 부모 노드의 오른쪽 자식을 nullptr로 변경
	}
	else if(p->__left_ == nullptr || p->__right_ == nullptr){ // 차수가 1인경우
		_NodePtr child = (p->__left_ != nullptr) ? p->__left_ : p->__right_; // 자식 노드를 가져옴
		if(q == nullptr) __root = child; // 삭제할 노드가 루트일때 루트를 자식 노드로 변경
		else if(q->__left_ == p) q->__left_ = child;
		else q->__right_ = child;
	}
	else{
		int l_height = __height(p->__left_); // 왼쪽 서브트리의 높이
		int r_height = __height(p->__right_); // 오른쪽 서브트리의 높이
		
		if(l_height > r_height) { // 왼쪽 서브트리의 높이가 더 큰경우
			r = __tree_max(p->__left_); // 왼쪽 서브트리의 최대 노드
			is_left = true; // 방향을 지정
		}
		else if(l_height < r_height) r = __tree_min(p->__right_); // 오른쪽 서브트리가 더 높은경우
		else { // 양쪽 서브트리의 높이가 같은경우
			int l_size = __size(p->__left_); // 왼쪽 서브트리의 크기
			int r_size = __size(p->__right_); // 오른쪽 서브트리의 크기
			
			if(l_size >= r_size) { // 왼쪽 서브트리의 크기가 더 큰경우
				r = __tree_max(p->__left_);
				is_left = true;
			} else { // 오른쪽 서브트리의 크기가 더 큰경우
				r = __tree_min(p->__right_);
				is_left = false;
			}
		}

		p->__key_ = r->__key_; // 삭제할 노드의 키를 최대 노드의 키로 변경
		// 재귀로 왼쪽 또는 오른쪽 삭제할 노드 갱신 후 삭제
		if(is_left) p = __eraseBST(p->__left_, r->__key_); // 왼쪽 서브트리에서 삭제
		else p = __eraseBST(p->__right_, r->__key_); // 오른쪽 서브트리에서 삭제
	}
	return p; // 삭제할 노드 반환
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// write your own code here
	if(__x == nullptr) return;
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

	public: // Capacity
		size_type height() const {
			// use __height or write your own code here
			return __height(__root_);
		}

		size_type size() const {
			// use __size or write your own code here
			return __size(__root_);
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

		std::pair<const_pointer, const_pointer> search(const key_type& key) {
			// 검색 결과를 반환하는 함수
			return __searchBST(__root_, key);
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