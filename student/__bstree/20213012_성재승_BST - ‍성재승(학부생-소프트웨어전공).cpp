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
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) { // __inorder 방식으로 출력
		__inorder(np);
		return os;
		// write your own code here
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) { // 재귀적으로 height를 탐색
	if(__x == nullptr) return 0; // (base case) 현재 노드가 null ptr이면 높이는 0
	return std::max(__height(__x->__left_), __height(__x->__right_))+1; // 각 서버트리의 높이를 각각 구하고 최대값에 +1을 함
	// write your own code here
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) { // 재귀적으로 size를 탐색
	if(__x == nullptr) return 0; // (base case) 현재 노드가 null ptr이면 size는 0
	return __size(__x->__left_) + __size(__x->__right_) + 1; // 각 서브트리의 사이즈에 +1을 함 (+1은 현재 노드에 대한 것)
	// write your own code here
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) { // 트리를 탐험하며 최소값을 찾음(가장 왼쪽의 값)
	if(__x->__left_ == nullptr) return __x; // 더이상 왼쪽에 서브트리가 없다면 현재 값 반환
	return __tree_min(__x->__left_); // 왼쪽 탐색
	// write your own code here
}
/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) { // 트리를 탐험하며 최소값을 찾음(가장 오른쪽의 값)
	if(__x->__right_ == nullptr) return __x; // 더이상 오른쪽에 서브트리가 없다면 현재 값 반환
	return __tree_max(__x->__right_); // 오른쪽 탐색
	// write your own code here
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) { // in order로 출력
	if(__x == nullptr) return; // 루트 노드가 null이면 아무것도 출력 안함
	std::cout << '<';
	__inorder(__x->__left_);
	std::cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
	std::cout << '>';
	// write your own code here
}

template <class _NodePtr, class _Tp>
_NodePtr searchBST(_NodePtr __x, const _Tp& key) { // 원하는 key에 해당하는 node를 찾음(재귀)
	if (__x == nullptr) return nullptr; // 트리에 원하는 key가 없다면 null 반환
	if (__x->__key_ == key) return __x; // 트리에 원하는 키가 있다면 해당 노드 반환
	return (key < __x->__key_) ? searchBST(__x->__left_, key):searchBST(__x->__right_, key); // 현재 노드의 값과 key를 비교해서
	// 현재값이 key보다 더 크면 왼쪽으로
	// 현재 값이 key보다 더 작으면 오른쪽으로
}

template <class _NodePtr, class _Tp>
_NodePtr searchParent(_NodePtr __x, const _Tp& key) { // 해당하는 키의 parent를 찾기
	_NodePtr q = nullptr; // 부모 노드
	_NodePtr p = __x; // 현재 노드
	while(p != nullptr) {
		if(key == p->__key_) return q; // 현재 노드의 key가 찾고자 하는 key와 같으면 부모 노드 반환
		q = p; // 부모 업데이트
		if(key < p->__key_) p = p->__left_; // 현재 노드의 key가 찾고자 하는 key보다 크면 왼쪽으로 탐색
		else p = p->__right_; // 현재 노드의 key가 찾고자 하는 key보다 작으면 오른쪽으로 탐색
	}
	return nullptr; // 현재 노드가 null이면 (트리에 원하는 키가 없으면) null 반환
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) { // 삽입 연산(재귀)
	if (__root == nullptr) { // 현재 노드가 null이면
		__root = new Node<_Tp>(key); // 삽입하려는 key값으로 노드를 만듬
		return std::pair<_NodePtr, bool>(__root, true); // 삽입 성공을 반환
	}
	else if (__root->__key_ == key) return std::pair<_NodePtr, bool>(nullptr, false); // 만약 이미 키가 존재한다면 false 삽입 실패를 반환
	else if (__root->__key_ > key) { // 만약 현재 key가 찾고자하는 key보다 크다면
		if(__root->__left_ == nullptr) { // 왼쪽을 탐색해야 하는데 왼쪽이 비어있으면
			__root->__left_ = new Node<_Tp>(key); // 새로운 노드 삽입
			return std::pair<_NodePtr, bool>(__root->__left_, true); // 성공 반환
		}
		return __insertBST(__root->__left_, key); // 비어있지 않으면 왼쪽 탐색
	}
	else if (__root->__key_ < key) { // 만약 현재 key가 찾고자하는 key보다 작다
		if (__root->__right_ == nullptr) { // 오른쪽을 탐색해야 하는데 오른쪽이 비어있으면
			__root->__right_ = new Node<_Tp>(key); // 새로운 노드 삽입
			return std::pair<_NodePtr, bool>(__root->__right_, true); // 성공 반환
		}
		return __insertBST(__root->__right_, key); // 비어있지 않다면 오른쪽 탐색
	}
	// write your own code here
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) { // 삭제 연산
	_NodePtr p = searchBST(__root, key); // 삭제하려는 노드 찾기
	_NodePtr q = searchParent(__root, key); // 삭제 대상의 부모 찾기
	if (p == nullptr) return nullptr;
	else if (q == nullptr) { // 삭제하려는게 루트노드일 때
		if (p->__left_ == nullptr && p->__right_ == nullptr) { // 좌우에 아무 것도 없다면(degree=0)
			__root = nullptr; // 루트를 삭제
			return p;
		}
		else if (p->__left_ == nullptr) { // 오른쪽 서브트리만 존재하면(degree=1)
			__root = p->__right_; // 루트를 현재 노드의 오른쪽 노드로 설정
			return p;
		}
		else if (p->__right_ == nullptr) { // 왼쪽 서브트리만 존재하면(degree=1)
			__root = p->__left_; // 루트를 현재 노드의 왼쪽 노드로 설정
			return p;
		}
		else { // degree가 2일 때
			_NodePtr r; // 임시 저장 공간
			int flag; // 왼쪽을 탐색할 것인지 오른쪽을 탐색할 것인지 flag
			unsigned h_left = __height(p->__left_); // 왼쪽 서브트리의 높이
			unsigned h_right = __height(p->__right_); // 오른쪽 서브트리의 높이
			if (h_left > h_right) { r = __tree_max(p->__left_); flag = 0; } // 왼쪽이 더 높으면 왼쪽 서브트리의 최대값을 찾고 flag는 0
			else if (h_left < h_right) { r = __tree_min(p->__right_); flag = 1; } // 오른쪽이 더 높으면 오른쪽 서브트리의 최소값을 찾고 flag는 1
			else { // 양쪽의 높이가 같으면
				unsigned s_left = __size(p->__left_); // 왼쪽 서브 트리의 노드의 개수
				unsigned s_right = __size(p->__right_); // 오른쪽 서브 트리의 노드의 개수
				if (s_left >= s_right) { r = __tree_max(p->__left_); flag = 0; } // 왼쪽 서브트리의 노드의 개수가 오른쪽보다 크거나 같다면 왼쪽 서브트리의 최대값을 찾고 flag는 0
				else if (s_left < s_right) { r = __tree_min(p->__right_); flag = 1; } // 오른쪽 서브트리의 노드 개수가 왼쪽보다 크다면 오른쪽 서브트리의 최소값을 찾고 flag는 1
			}
			p->__key_ = r->__key_; // 임시로 저장한 노드의 key를 삭제하려는 노드에 덮어 씌움(이거 자체가 일종의 삭제 연산임)
			if (flag) return __eraseBST(p->__right_, r->__key_); // 만약 flag가 1이면 앞선 if문을 통해 right가 선택된 것이므로 현재 노드의 오른쪽에 있는 r->key값을 다시 삭제(재귀)
			else return __eraseBST(p->__left_, r->__key_); // 만약 flag가 0이면 앞선 if문을 통해 left가 선택된 것이므로 현재 노드의 왼쪽에 있는 r->key값을 다시 삭제(재귀)
		}
	}
	else if (p->__left_ == nullptr&&p->__right_ == nullptr) { // p에 자식 노드가 없을 때
		if (q->__left_ == p) { // 부모 노드의 왼쪽에 현재 노드가 있다면
			q->__left_ = nullptr; // 부모의 왼쪽 노드를 삭제
			return p;
		}
		else { // 부모 노드의 오른쪽에 현재 노드가 있다면
			q->__right_ = nullptr; // 부모의 오른쪽 노드를 삭제
			return p;
		}
	}
	else if (p->__left_ == nullptr) { // p에 오른쪽 서브트리만 있을 때
		if (q->__left_ == p) {  // 부모 노드의 왼쪽에 현재 노드가 있다면
			q->__left_ = p->__right_; // 부모의 왼쪽 포인터가 현재 노드의 오른쪽 노드를 가리키도록
			return p;
		}
		else { // 부모 노드의 오른쪽에 현재 노드가 있다면
			q->__right_ = p->__right_; // 부모의 오른쪽 포인터가 현재 노드의 오른쪽 노드를 가리키도록
			return p;
		}
	}
	else if (p->__right_ == nullptr) { // p에 왼쪽 서브트리만 있을 때
		if(q->__left_ == p) { // 부모 노드의 왼쪽에 현재 노드가 있다면
			q->__left_ = p->__left_;  // 부모의 왼쪽 포인터가 현재 노드의 왼쪽 노드를 가리키도록
			return p;
		}
		else { // 부모 노드의 오른쪽에 현재 노드가 있다면
			q->__right_ = p->__left_; // 부모의 오른쪽 포인터가 현재 노드의 왼쪽 노드를 가리키도록
			return p;
		}
	}
	else { // p에 왼쪽 오른쪽 서브트리가 있을 때 (degree가 2일 때)
		_NodePtr r;
		int flag; //0이면 왼쪽 1이면 오른쪽
		unsigned h_left = __height(p->__left_); // 왼쪽 서브트리의 높이
		unsigned h_right = __height(p->__right_); // 오른쪽 서브트리의 높이
		if (h_left > h_right) { r = __tree_max(p->__left_); flag = 0;}  // 왼쪽이 더 높으면 왼쪽 서브트리의 최대값을 찾고 flag는 0
		else if (h_left < h_right) { r = __tree_min(p->__right_); flag = 1;} // 오른쪽이 더 높으면 오른쪽 서브트리의 최소값을 찾고 flag는 1
		else {
			unsigned s_left = __size(p->__left_); // 왼쪽 서브 트리의 노드의 개수
			unsigned s_right = __size(p->__right_); // 오른쪽 서브 트리의 노드의 개수
			if(s_left >= s_right) {r = __tree_max(p->__left_); flag = 0;} // 왼쪽 서브트리의 노드의 개수가 오른쪽보다 크거나 같다면 왼쪽 서브트리의 최대값을 찾고 flag는 0
			else if (s_left < s_right) {r = __tree_min(p->__right_); flag = 1;} // 오른쪽 서브트리의 노드 개수가 왼쪽보다 크다면 오른쪽 서브트리의 최소값을 찾고 flag는 1
		}
		p->__key_ = r->__key_; // 임시로 저장한 노드의 key를 삭제하려는 노드에 덮어 씌움(이거 자체가 일종의 삭제 연산임)
		if(flag) return __eraseBST(p->__right_, r->__key_); // 만약 flag가 1이면 앞선 if문을 통해 right가 선택된 것이므로 현재 노드의 오른쪽에 있는 r->key값을 다시 삭제(재귀)
		else return __eraseBST(p->__left_, r->__key_); // 만약 flag가 0이면 앞선 if문을 통해 left가 선택된 것이므로 현재 노드의 왼쪽에 있는 r->key값을 다시 삭제(재귀)
	}

	// write your own code here
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) { // 재귀적으로 트리를 돌며 노드를 삭제(post order)
	if (__x == nullptr)return;
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	// write your own code here
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
			if(__r == nullptr) return nullptr;
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
