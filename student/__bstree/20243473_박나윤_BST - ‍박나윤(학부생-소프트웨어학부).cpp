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
using namespace std;

// class template : 클래스를 다시 작성할 필요 없이 여러 자료형으로 사용 가능
// 전방 선언
template <class _Tp> class Node;
template <class _Tp> class BST;

/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* height, size를 제외한 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
template <class _Tp>
class Node {
	public: // Member types
		typedef _Tp					__key_type; // 노드가 저장하는 값 타입 (int 등)
        typedef Node<__key_type>*	__node_pointer_type; // 노드의 포인터 타입
	
	public: // Member variables
		__key_type			__key_; // 노드가 저장하는 실제 데이터
		__node_pointer_type	__left_; // 왼쪽 자식 포인터
		__node_pointer_type	__right_; // 오른쪽 자식 포인터
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {} // 기본 생성자
        Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {} // 값 생성자 : 키를 주어진 값으로 초기화

	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
    // 출력 연산자
	friend ostream& operator<<(ostream& os, const __node_pointer_type& np) {

        // 노드 포인터를(root)를 받아 트리 전체를 출력하도록 동작한다
		// < {left} {key} {right} > 형태로 재귀 출력한다
		// 아무것도 없으면 출력하지 않는다
		if (np == nullptr) return os; 

		os << "<";
		if (np->__left_ != nullptr) { os << np->__left_ << " "; }
		else { os << " "; } 
		os << np->__key_ << " ";
		if (np->__right_ != nullptr) { os << np->__right_; }
		os << ">";
		
		return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr> // Node Pointer
unsigned __height(_NodePtr __x) {
    // __x를 루트로 한 서브트리의 높이를 구한다
    // 좌측 서브트리의 높이 합과 우측 서브트리의 높이 합 중 큰 값에 1을 더한다
	// 재귀적 방식을 활용한다
	if (__x == nullptr) return 0;

	return 1 + max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	// __x를 루트로 하는 서브트리의 노드 개수를 구한다
	// __height() 함수와 형태 비슷
	// 재귀적 방식을 활용한다
	if (__x == nullptr) return 0;

	return 1 + (__size(__x->__left_) + __size(__x->__right_));
}

// 추가 : 새로운 노드를 생성한다 (동적 할당)
// __insertBST()에서 새 노드 생성 시 사용한다
template <class _Tp>
Node<_Tp>* getBSTNode(const _Tp& key) {
	return new Node<_Tp> (key);
}


/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// 서브트리에서 제일 작은 키(종단 왼쪽)값을 찾는다
	// 왼쪽 서브트리로 값이 존재하지 않을 때까지 이동한다
	if (__x == nullptr) return nullptr;

	while (__x->__left_ != nullptr) {
		__x = __x->__left_;
	}

	return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	// 서브트리에서 제일 큰 키(종단 오른쪽) 값을 찾는다
	// 오른쪽 서브트리로 값이 존재하지 않을 때까지 이동한다
	if (__x == nullptr) return nullptr;

	while(__x->__right_ != nullptr) {
		__x = __x->__right_;
	}

	return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// 1. 루트 기준으로 왼쪽 서브트리 재귀 방문
	// 2. 루트 방문
	// 3. 루트 기준으로 오른쪽 서브트리 재귀 방문
	if (__x == nullptr) return;

	__inorder(__x->__left_);
	cout << " " << __x->__key_ << " ";
	__inorder(__x->__right_);
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    // 같은 키가 이미 존재하면 false
    // return시 포인터 타입 const 변환에 주의
	
	_NodePtr p = __root;
	_NodePtr q = nullptr; // __root의 부모 노드
	// stack<_NodePtr> stack; // AVL 구현용 stack (BST에서는 사용 X)

	while (p != nullptr) {
		// 중복되는 key가 존재 : false 반환
		if (key == p->__key_) { return pair<_NodePtr, bool>(p, false); }
		q = p; // root가 부모 노드가 된다
		// stack.push(p);

		// 삽입하려는 key가 p의 key보다 작을 때는 왼쪽 자식 노드로 내려간다
		if (key < p->__key_) { p = p->__left_; }
		// 삽입하려는 key가 p의 key보다 크거나 같으면 오른쪽 자식 노드로 내려간다
		else { p = p->__right_; }
	}

	_NodePtr newNode = getBSTNode(key); // 새로 만들어질 노드

	// insert NewNode as a child of q
	if (q == nullptr) { __root = newNode; }
	else if (key < q->__key_) { q->__left_ = newNode; }
	else { q->__right_ = newNode; }

	return pair<_NodePtr, bool> (newNode, true);
}


/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    // 반환 : 삭제된 노드 포인터, 실패하면 nullptr
    // __eraseBST는 삭제할 노드를 분리해서 포인터로 반환하고, 호출 측에서 delete필요

	// base case
	if (__root == nullptr) { return nullptr; }

	// recursive
	// 1. 삭제하려는 key가 __root보다 작으면 왼쪽 서브트리에서 재귀 호출
	// 2. __root보다 크면 오른쪽 서브트리에서 재귀 호출
	if (key < __root->__key_) { return __eraseBST(__root->__left_, key); }
	else if (key > __root->__key_) { return __eraseBST(__root->__right_, key); }

	// 3. key가 현재 확인하려는 노드의 값과 일치하는 경우 (key == __root_->__key_)
	else {
		_NodePtr tmpNode = __root; // 삭제할 노드 임시 저장용

		// (3.1.1) 삭제할 노드의 차수가 0 또는 1  (case 1. 왼쪽 서브트리가 없음)
		// 왼쪽 서브트리로 이동 후 부모 노드를 삭제한다
		if (__root->__left_ == nullptr) { 
			__root = __root->__right_;
			return tmpNode;
		}
		// (3.1.2) 삭제할 노드의 차수가 0 또는 1 (case 2. 오른쪽 서브트리가 없음)
		// 오른쪽 서브트리로 이동 후 부모 노드를 삭제한다
		else if (__root->__right_ == nullptr) {
			__root = __root->__left_;
			return tmpNode;
		}
		// (3.2) 삭제할 노드의 차수가 2 (양쪽 서브트리가 모두 존재)
		// 왼쪽 서브트리에서 최대 노드를 찾아 key 값을 복사한 후 최대 노드를 삭제한다
		// psudo code 참고..
		else {
			// 높이와 크기를 미리 계산하여 저장한다
			unsigned leftHeight = __height(__root->__left_);
			unsigned rightHeight = __height(__root->__right_);
			unsigned leftSize = __size(__root->__left_);
			unsigned rightSize = __size(__root->__right_);

			_NodePtr subNode; // 대체 노드를 가리킬 포인터

			// 조건문이 길어져 flag를 사용하였다.
			// 오른쪽 서브트리가 더 크거나, 높이가 같으면서 크기가 큰 경우 = true
			// 그 외의 경우는 all false
			bool flag = ((leftHeight < rightHeight) || 
			(leftHeight == rightHeight && leftSize < rightSize));

			// true : 오른쪽 서브트리에서 최솟값을 찾는다
			// false : 왼쪽 서브트리에서 최댓값을 찾는다
			if (flag) {
				subNode = __tree_min(__root->__right_);
			}
			else { subNode = __tree_max(__root->__left_); }

			// 대체 노드의 key 값을 현재의 key로 복사
			// 복사한 노드를 삭제한다
			__root->__key_ = subNode->__key_;
			if (flag) { return __eraseBST(__root->__right_, subNode->__key_); }
			else { return __eraseBST(__root->__left_, subNode->__key_); }
		}
	}
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// 모든 노드들을 재귀적으로 삭제하고 최종적으로 __x를 nullptr로 설정한다
	// 자식 노드들을 먼저 삭제 후 마지막으로 현재 노드를 삭제한다
	if (__x == nullptr) return;

	// recursive step
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
		typedef size_t				    size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_; // 트리 루트
	
	public: // Constructor
		BST(): __root_(nullptr) {} // 트리 루트를 nullptr로 초기화
	
	public: // Capacity
		size_type height() const {
			// __height 전역 함수 호출하여 루트의 높이 반환
			return __height(__root_);
		}

		size_type size() const {
			// use __size or write your own code here
            // __size 전역 함수 호출
			return __size(__root_);
		}
	
	public: // Lookup
		void inorder() const {
			// use __inorder or write your own code here
            // __inorder 호출 후 줄바꿈
			__inorder(__root_);
			cout << endl;
		}
		
	public: // Modifier
		pair<const_pointer, bool> insert(const key_type& key) {
			// use __insertBST or write your own code here
            // __insertBST 결과 그대로 반환
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			// use __eraseBST or write your own code here
            // 전역 __eraseBST 호출 후 delete __r; 하고 __r 반환

			pointer __r = __eraseBST(__root_, key);

			// Client still needs to destruct/deallocate it
			// Or memory leak will occur
			delete __r;

			return __r; 
		}

		void clear() {
			// use __clear or write your own code here
            // 메모리 해제
			__clear(__root_);
		}
	
	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend ostream& operator<<(ostream& os, const BST& tree) {
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
	BST<int>	tree; // 빈 트리 생성
	char		command; 
	int			key;

	while (cin >> command >> key) { // 명령(command) + 정수(key) 읽음
		switch ((int)command) { // 입력된 command에 따라 아래 case 진행
			case (int)'i': // 삽입
				if (tree.insert(key).second == false) {
					cerr << "i " << key << ": The key already exists" << endl;
					continue;
				}
				break;
			case (int)'d': // 삭제 
				if (tree.erase(key) == nullptr) {
					cerr << "d " << key << ": The key does not exist" << endl;
					continue;
				}
				break;
			default:
				cerr << "Invalid command: " << command << endl;
				return (1);
				break;
		}
		cout << tree << endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}