/*
* File Processing Report 2025 - Binary Search Tree skeleton code in C/C++
*/

#include <iostream>
#include <stack>
#include <utility>
#include <algorithm>
#include <cstddef>

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node {
public:
	typedef _Tp					__key_type;                 // 노드에 저장되는 키의 자료형
	typedef Node<__key_type>* __node_pointer_type;          // 노드 포인터 타입

public:
	__key_type			__key_;     // 현재 노드에 저장된 키 값
	__node_pointer_type	__left_;    // 왼쪽 자식
	__node_pointer_type	__right_;   // 오른쪽 자식
	int                 height;     // 이 노드를 루트로 하는 서브트리의 높이
	std::size_t         size;       // 이 노드를 루트로 하는 서브트리의 노드 개수

public:
	// 기본 생성자: 키를 디폴트 값으로, 자식 포인터는 null, 통계(height/size)는 0/1로 초기화
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(0), size(1) {}
	// 키를 받는 생성자: 동일하게 초기화
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(0), size(1) {}

	// 중위 순회 결과를 ostream으로 내보내는 헬퍼 (템플릿 포인터 타입 허용)
	template <class _NodePtr>
	friend void __inorder_stream_helper(std::ostream& os, _NodePtr np) {
		if (np == nullptr) return;
		__inorder_stream_helper(os, np->__left_);   // 왼쪽 서브트리
		os << np->__key_ << " ";                    // 현재 노드
		__inorder_stream_helper(os, np->__right_);  // 오른쪽 서브트리
	}

	// Node* 에 대한 출력 연산자: 서브트리를 중위 순회해 키들을 출력
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		__inorder_stream_helper(os, np);
		return os;
	}
};

template <class _NodePtr>
int __height(_NodePtr __x) {
	// 공백 트리의 높이는 -1로 정의, 리프의 높이는 0
	return __x ? __x->height : -1;
}

template <class _NodePtr>
std::size_t __size(_NodePtr __x) {
	// 공백 트리의 크기는 0
	return __x ? __x->size : 0;
}

template <class _NodePtr>
void __update_stats(_NodePtr __x) {
	// 현재 노드의 height/size를 자식 정보를 기반으로 갱신
	if (__x) {
		__x->height = 1 + std::max(__height(__x->__left_), __height(__x->__right_));
		__x->size = 1 + __size(__x->__left_) + __size(__x->__right_);
	}
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// 서브트리에서 최소 키(가장 왼쪽) 노드 탐색
	while (__x && __x->__left_) {
		__x = __x->__left_;
	}
	return __x;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	// 서브트리에서 최대 키(가장 오른쪽) 노드 탐색
	while (__x && __x->__right_) {
		__x = __x->__right_;
	}
	return __x;
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
	// 중위 순회 출력 (오름차순)
	if (__x == nullptr) return;
	__inorder(__x->__left_);
	std::cout << __x->__key_ << " ";
	__inorder(__x->__right_);
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// 표준 BST 삽입: {삽입/발견된 노드 포인터, 새로 삽입됐는지 여부}
	if (__root == nullptr) {
		__root = new Node<_Tp>(key);          // 새 리프 노드 생성
		return std::make_pair(__root, true);
	}

	std::pair<_NodePtr, bool> result;
	if (key < __root->__key_) {
		result = __insertBST(__root->__left_, key);   // 왼쪽으로 재귀
	}
	else if (key > __root->__key_) {
		result = __insertBST(__root->__right_, key);  // 오른쪽으로 재귀
	}
	else {
		// 중복 키: 삽입 없이 false 반환
		return std::make_pair(__root, false);
	}

	// 실제로 하위에서 삽입이 일어났다면 통계 갱신
	if (result.second) {
		__update_stats(__root);
	}

	return result;
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// 표준 BST 삭제: 제거된 노드(분리된 포인터)를 반환, 없으면 nullptr
	// (반환된 포인터는 바깥에서 delete 해야 함)
	if (__root == nullptr) {
		return nullptr;  // 키 없음
	}

	_NodePtr node_to_delete = nullptr;

	if (key < __root->__key_) {
		node_to_delete = __eraseBST(__root->__left_, key);    // 왼쪽 탐색
	}
	else if (key > __root->__key_) {
		node_to_delete = __eraseBST(__root->__right_, key);   // 오른쪽 탐색
	}
	else {
		// 삭제할 노드 발견
		if (__root->__left_ == nullptr) {
			// 오른쪽 자식으로 대체
			node_to_delete = __root;
			__root = __root->__right_;
		}
		else if (__root->__right_ == nullptr) {
			// 왼쪽 자식으로 대체
			node_to_delete = __root;
			__root = __root->__left_;
		}
		else {
			// 두 자식이 있는 경우: successor(오른쪽 최소) 또는 predecessor(왼쪽 최대) 중 선택
			// 여기서는 서브트리의 높이/크기를 비교해 선택(약간의 균형 유지 목적)
			if (__height(__root->__left_) < __height(__root->__right_) ||
				(__height(__root->__left_) == __height(__root->__right_) && __size(__root->__left_) < __size(__root->__right_)))
			{
				// 성공자(successor) 방식: 오른쪽 서브트리 최소 노드의 키를 올리고, 그 키를 아래에서 삭제
				_NodePtr successor = __tree_min(__root->__right_);
				__root->__key_ = successor->__key_;
				node_to_delete = __eraseBST(__root->__right_, successor->__key_);
			}
			else {
				// 전임자(predecessor) 방식: 왼쪽 서브트리 최대 노드의 키를 올리고, 그 키를 아래에서 삭제
				_NodePtr predecessor = __tree_max(__root->__left_);
				__root->__key_ = predecessor->__key_;
				node_to_delete = __eraseBST(__root->__left_, predecessor->__key_);
			}
		}
	}

	// 구조 변경 이후 현재 노드의 통계 갱신
	if (__root) {
		__update_stats(__root);
	}

	return node_to_delete;
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// 후위 순회로 전 노드 해제 (메모리 누수 방지)
	if (__x == nullptr) return;
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}

template <class _Tp>
class BST {
public:
	typedef _Tp						key_type;       // 키 타입 별칭
	typedef std::size_t				size_type;      // 크기 타입 별칭
	typedef Node<key_type>* pointer;
	typedef const Node<key_type>* const_pointer;

private:
	pointer	__root_;   // BST의 루트 포인터

public:
	BST() : __root_(nullptr) {}

	~BST() {
		clear();       // 소멸 시 전체 트리 해제
	}

public:
	size_type height() const {
		// 트리 전체 높이(공백 트리일 경우 -1 반환됨)
		return __height(__root_);
	}

	size_type size() const {
		// 트리 전체 노드 수
		return __size(__root_);
	}

public:
	void inorder() const {
		// 중위 순회 출력 + 개행
		__inorder(__root_);
		std::cout << std::endl;
	}

public:
	std::pair<const_pointer, bool> insert(const key_type& key) {
		// 키 삽입. {해당 키를 가진 노드 포인터, 새로 삽입 여부} 반환
		return __insertBST(__root_, key);
	}

	const_pointer erase(const key_type& key) {
		// 키 삭제. __eraseBST가 분리한 노드 포인터를 돌려줌
		// 여기서 즉시 delete 하고 그 포인터를 다시 반환(주의: 이미 해제된 포인터)
		pointer __r = __eraseBST(__root_, key);
		delete __r;
		return __r;
	}

	void clear() {
		// 전체 트리 삭제
		__clear(__root_);
	}

	// 트리 출력 연산자: 루트 노드부터 중위 순회로 키 나열
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;
		return os;
	}
};

int main(int argc, char** argv) {
	BST<int>	tree;     // int 키를 저장하는 BST
	char		command;  // 'i' = 삽입, 'd' = 삭제
	int			key;      // 처리할 키 값

	// 표준 입력에서 "i 10", "d 5" 같은 쌍을 EOF까지 읽어 처리
	while (std::cin >> command >> key) {
		switch ((int)command) {
		case (int)'i':
			// 삽입 시 중복이면 경고 출력
			if (tree.insert(key).second == false) {
				std::cerr << "i " << key << ": The key already exists" << std::endl;
				continue;
			}
			break;
		case (int)'d':
			// 삭제 실패(키 없음) 시 경고 출력
			if (tree.erase(key) == nullptr) {
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			break;
		default:
			// 알 수 없는 명령이면 에러로 종료
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
			break;
		}
		// 연산 성공 시, 트리를 중위 순회 형태로 한 줄 출력
		std::cout << tree << std::endl;
	}

	tree.clear(); // 명시적 정리(소멸자에서도 자동 수행되지만 안전차원)

	return (0);
}
