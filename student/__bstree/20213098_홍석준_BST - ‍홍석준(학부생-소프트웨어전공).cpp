/*
* File Processing Report 2024 - Binary Search Tree skeleton code in C/C++
*
* 본 Skeleton code는 구현에 도움을 주기 위해 작성된 코드입니다.
* ❗️본 Skeleton code는 과제의 요구 조건을 완전히 충족시키지 않을 수 있습니다.❗️
* ❗️최종 점수 산정은 과제 PDF에 명시된 요구 사항을 기준으로 이루어집니다.❗️
*
* 미리 말씀드리면, skeleton code가 다소 어려울 수 있습니다.
* C++ 스러운 코딩 스타일과 코드 설계 방식에 관한 고찰이 담겨있으니, 충분한 시간을 들여 분석해보시길 바랍니다.
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
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
		unsigned			__height_;
		unsigned			__size_;
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (np == nullptr) { // 빈 노드인 경우
			return os;
		} 
		// < left key right >
		os << '<';
		if (np->__left_) os << np->__left_;
		os << ' ' << np->__key_ << ' ';
		if (np->__right_) os << np->__right_;
		os << '>';
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) { // 왼쪽 오른쪽 중 더 큰 높이 + 1
	if (__x == nullptr) return 0;
	return __x->__height_;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) { // 왼 + 오 + 1
	if (__x == nullptr) return 0;
	return __x->__size_;
}

template <class _NodePtr>
void __update_node(_NodePtr __x) {
	if (__x == nullptr) return;
	unsigned lh = __height(__x->__left_);
	unsigned rh = __height(__x->__right_);
	__x->__height_ = 1 + (lh > rh ? lh : rh);
	__x->__size_ = 1 + __size(__x->__left_) + __size(__x->__right_);
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) { // 왼쪽 끝 노드
	if (__x == nullptr) return nullptr;
	while (__x->__left_ != nullptr) __x = __x->__left_;
	return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) { // 오른쪽 끝 노드
	if (__x == nullptr) return nullptr;
	while (__x->__right_ != nullptr) __x = __x->__right_;
	return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) { // 왼 -> 루트 -> 오
	if (__x == nullptr) return;
	__inorder(__x->__left_);
	std::cout << __x->__key_ << ' ';
	__inorder(__x->__right_);
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr __root, const _Tp& key) {
	std::stack<_NodePtr> st;
	_NodePtr p = __root; 
	_NodePtr q = nullptr; 

	while (p != nullptr) { // 
		if (key == p->__key_) return std::make_pair(__root, false); // 중복 확인
		q = p; 
		st.push(q); // 삽입 경로 저장
		if (key < p->__key_) p = p->__left_; // 작으면 왼쪽
		else p = p->__right_; // 크면 오른쪽
	}


	_NodePtr nn = new Node<_Tp>(key);
	if (q == nullptr) { // 트리가 비어있던 경우
		__root = nn; // 루트 노드로 삽입
	} else if (key < q->__key_) { // 작으면 왼쪽
		q->__left_ = nn;
	} else { // 크면 오른쪽
		q->__right_ = nn;
	}

	while (!st.empty()) { // 삽입 경로 따라가며 노드 정보 갱신
		_NodePtr node = st.top(); st.pop(); // 스택에서 꺼내기
		__update_node(node); // 높이, 크기 갱신
	}

	if (__root != nullptr && st.empty()) __update_node(__root); // 루트 노드 정보 갱신

	return std::make_pair(__root, true); // 삽입한 위치와 중복여부 반환
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	std::stack<_NodePtr> st;
	_NodePtr p = __root;
	_NodePtr q = nullptr;

	while (p != nullptr && key != p->__key_) { // 삭제할 노드 탐색
		q = p;
		st.push(q);
		if (key < p->__key_) p = p->__left_; // 작으면 왼쪽
		else p = p->__right_; // 크면 오른쪽
	}

	if (p == nullptr) return nullptr; // 삭제할 노드가 없는 경우 nullptr 반환

	if (p->__left_ != nullptr && p->__right_ != nullptr) { // 자식 노드가 둘 다 있는 경우
		_NodePtr temp = p;
		st.push(p);
		unsigned lh = __height(p->__left_);
		unsigned rh = __height(p->__right_);
		if ( (lh < rh) || (lh == rh && __size(p->__left_) < __size(p->__right_)) ) { // 높이 비교 후 사이즈 비교
			p = p->__right_; // 오른쪽 서브트리가 더 크면
			while (p->__left_ != nullptr) {
				st.push(p);
				p = p->__left_;
			}
		} else { // 왼쪽 서브트리가 더 크면
			p = p->__left_;
			while (p->__right_ != nullptr) {
				st.push(p);
				p = p->__right_;
			}
		}
		temp->__key_ = p->__key_;
		q = st.empty() ? nullptr : st.top();
	}

	if (p->__left_ != nullptr) { // 왼쪽 자식 노드
		if (q == nullptr) { 
			__root = p->__left_; 
		} else if (q->__left_ == p) { 
			q->__left_ = p->__left_;
		} else {
			q->__right_ = p->__left_;
		}
	} else if (p->__right_ != nullptr) { // 오른쪽 자식 노드
		if (q == nullptr) { 
			__root = p->__right_; 
		} else if (q->__left_ == p) { 
			q->__left_ = p->__right_;
		} else {
			q->__right_ = p->__right_;
		}
	} else { // 자식 노드가 없는 경우
		if (q == nullptr) {
			__root = nullptr;
		} else if (q->__left_ == p) {
			q->__left_ = nullptr;
		} else {
			q->__right_ = nullptr;
		}
	}

	while (!st.empty()) { // 삭제 경로 따라가며 노드 정보 갱신
		_NodePtr node = st.top(); st.pop();
		__update_node(node);
	}

	return p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) { // 왼 -> 오 -> 루트 순으로 삭제
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
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {} 
		~BST() { __clear(__root_); } // 메모리 누수 방지 위해 소멸자에서 clear 호출
	
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
			std::pair<pointer, bool> res = __insertBST<pointer, key_type>(__root_, key);
			__root_ = res.first;
			return std::make_pair(static_cast<const_pointer>(res.first), res.second);
		}

		const_pointer erase(const key_type& key) {
			pointer __r = __eraseBST<pointer, key_type>(__root_, key);
			if (__r == nullptr) return nullptr;
			delete __r;
			return __r; 
		}

		void clear() {
			__clear(__root_);
		}
	
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
		std::cout << tree << std::endl;
	}

	//tree.clear(); //BST 소멸자에서 자동 호출되므로 생략 가능

	return (0);
}