#include <iostream>
using namespace std;

template <class _Tp> class Node;
template <class _Tp> class BST;


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
};

// 트리 높이
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) {return 0;} // 해당 트리가 존재하지 않으면 0 리턴, 존재하면 재귀호출로 서브 트리 접근

	unsigned l_height = __height(__x->__left_); // 왼쪽 트리로 이동
	unsigned r_height = __height(__x->__right_); // 오른쪽 트리로 이동

	return 1 + (l_height > r_height ? l_height : r_height); // 좌우 트리 중 큰 길이 + 1(함수 호출 기준 루트)
}

// 트리 노드 갯수
template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) {return 0;} // 해당 트리가 존재하지 않으면 0 리턴, 존재하면 재귀호출로 서브 트리 접근

	return 1 + __size(__x->__left_) + __size(__x->__right_); // 왼쪽 트리 노드 + 오른쪽 트리 노드 + 1(함수 호출 기준 루트)
}

// 트리에서 제일 작은 값 (minNode())
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if (__x == nullptr) {return nullptr;} // 빈 트리면 null값 리턴

	while (__x->__left_ != nullptr) { // while문을 통해 좌하단 끝 노드까지 접근 == 트리 노드의 최솟값
		__x = __x->__left_;
	}
	return __x;
}

// 트리에서 제일 큰 값 (maxNode())
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (__x == nullptr) {return nullptr;} // 빈 트리면 null값 리턴

	while (__x->__right_ != nullptr) { // while문을 통해 우하단 끝 노드까지 접근 == 트리 노드의 최댓값
		__x = __x->__right_;
	}
	return __x;
}

// 트리 출력시 노드 접근 법 (LVR 스타일)
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) {return;} // 빈 트리면 출력 X

	cout << "<"; // 각 트리 출력의 시작 지점

	if (__x->__left_ != nullptr) {
		__inorder(__x->__left_); // 왼쪽 자식 있으면 키 값 출력 하지 말고 다시 inorder 호출 (좌하단부터 출력)
		cout << " " << __x->__key_; // 부등호끼리는 붙이고 부등호, 키는 공백으로 구분하니 공백 출력 후 키 출력
	}else { // 왼쪽 서브트리 없으면 공백 후 키 값 출력
		cout << " " << __x->__key_;
	}

	if (__x->__right_ != nullptr) {
		cout << " "; // 위에서 키 값 출력 했으니 공백 출력
		__inorder(__x->__right_);
		cout << ">"; // 바로 앞에 키가 없는 경우
	}else { // 바로 앞에 키가 있는 경우
		cout << " >";
	}
}

// 노드 생성
template <class _Tp>
Node<_Tp>* getNodeBST(const _Tp& key) {
	return new Node<_Tp>(key);
}

// 삽입 알고리즘
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	if (__root == nullptr) { // 빈트리임 -> 루트에 바로 삽입
		__root = getNodeBST(key);
		return pair<_NodePtr, bool>(__root, true);
	}

	_NodePtr p = __root; // p : 탐색 주체
	_NodePtr q = nullptr; // q : p의 부모

	while (p != nullptr) { // 삽입될 위치까지 계속 이동
		if (key == p->__key_) { // 삽입될 키가 이미 존재
			return pair<_NodePtr, bool>(p, false);
		}

		// 현재 키 != 삽입 될 키
		q = p; // 부모 위치를 자식 위치로
		if (key < p->__key_) { // 키 값 크기에 따라 p 이동
			p = p->__left_;
		}else {
			p = p->__right_;
		}
	}
	// p : nullptr(삽입할 자리 대기 중), q : p의 부모

	// 삽입할 노드 생성
	_NodePtr newNode = getNodeBST(key);

	// 새 노드(p)랑 부모(q) 연결
	if (key < q->__key_) {
		q->__left_ = newNode;
	} else {
		q->__right_ = newNode;
	}

	return pair<_NodePtr, bool>(newNode, true);
}


// 삭제 알고리즘
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root; // p : 탐색 주체
	_NodePtr q = nullptr; // q : p의 부모

	// p, q 위치 설정
	while (p && (p->__key_ != key)) { // p가 널이면 반복, p가 널이 아닐 때 p의 key랑 삭제할 키 비교
		q = p;
		p = (key < p->__key_) ? p->__left_ : p->__right_;
	}

	if (!p) {return nullptr;} // 삭제할 p(key)가 트리에 없으면 널 포인터 리턴

	// 밑의 case에서 p는 삭제할 키, q는 삭제할 키(p)의 부모
	// case0 : 자식 없음
	if (p->__left_ == nullptr && p->__right_ == nullptr) {
		if (q == nullptr) { // p가 루트인 경우
			__root = nullptr;
		}else if (q->__left_ == p) { // 삭제할 키가 q의 왼쪽
			q->__left_ = nullptr;
		}else { // 삭제할 키가 q의 오른쪽
			q->__right_ = nullptr;
		}
		return p; // 삭제한 노드 반환
	}

	// case1-1 : 자식 1개(왼쪽만)
	if (p->__left_ != nullptr && p->__right_ == nullptr) { // 왼쪽 자식만 있으면
		if (q == nullptr) { // 삭제할 키가 루트임
			__root = p->__left_;
		}else if (q->__left_ == p) { // 삭제할 키가 부모의 왼쪽 : 부모의 왼쪽이 삭제할 키의 왼쪽 가르킴
			q->__left_ = p->__left_;
		}else { // 삭제할 키가 부모의 오른쪽 : 부모의 오른쪽이 삭제할 키의 왼쪽 가르킴
			q->__right_ = p->__left_;
		}
		// 삭제 노드 트리에서 분리 후 메모리 정리
		p->__left_ = nullptr;
		p->__right_ = nullptr;

		return p; // 삭제한 노드 반환
	}

	// case1-2 : 자식 1개(오른쪽만)
	if (p->__left_ == nullptr && p->__right_ != nullptr) { // 오른쪽 자식만 있으면
		if (q == nullptr) { // 삭제할 키가 루트임
			__root = p->__right_;
		}else if (q->__left_ == p) { // 삭제할 키가 부모의 왼쪽 : 부모의 왼쪽이 삭제할 키의 오른쪽 가르킴
			q->__left_ = p->__right_;
		}else { // 삭제할 키가 부모의 오른쪽 : 부모의 오른쪽이 삭제할 키의 오른쪽 가르킴
			q->__right_ = p->__right_;
		}
		// 삭제 노드 트리에서 분리 후 메모리 정리
		p->__left_ = nullptr;
		p->__right_ = nullptr;

		return p; // 삭제한 노드 반환
	}

	// case2 : 자식 2개
	{
		unsigned hLeft = __height(p->__left_);
		unsigned hRight = __height(p->__right_);

		bool flag; // 서브 트리 선택용 flag (true면 왼쪽 서브 트리 maxNode를 루트, false면 오른쪽 서브 트리 minNode를 루트)

		if (hLeft > hRight) { // 왼쪽 높이가 더 크면 ture
			flag = true;
		}else if (hLeft < hRight) { // 오른쪽 높이가 더 크면 false
			flag = false;
		}else { // 왼쪽 높이 == 오른쪽 높이면 노드 수 비교
			unsigned sLeft = __size(p->__left_);
			unsigned sRight = __size(p->__right_);
			flag = (sLeft >= sRight); // 노드 수가 같거나 왼쪽 노드 수가 더 많으면 true
		}

		// e : 루트로 이동할 노드
		if (flag) { // 왼쪽 서브 트리의 maxNode를 루트로
			_NodePtr e = __tree_max(p->__left_);
			p->__key_ = e->__key_; // 삭제할 키 값을 왼쪽 서브트리의 maxNode 키 값으로 변경

			return __eraseBST(p->__left_, e->__key_); // e의 키 값 옮기면서 p 삭제 했으니 e도 삭제해야함
		}else { // 오른쪽 서브 트리의 minNode를 루트로
			_NodePtr e = __tree_min(p->__right_);
			p->__key_ = e->__key_; // 삭제할 키 값을 오른쪽 서브트리의 minNode 키 값으로 변경

			return __eraseBST((p->__right_), e->__key_);
		}

	}
}

//메모리 누수 제거
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) {return;} // 포인터가 가리키는게 없으면 clear 필요 X

	__clear(__x->__left_);
	__clear(__x->__right_);

	delete __x;
	__x = nullptr; // dangling 해소
}


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
		tree.inorder(); // operator<< 구현대신 inorder() 함수로 출력 구현
	}

	tree.clear();

	return (0);
}