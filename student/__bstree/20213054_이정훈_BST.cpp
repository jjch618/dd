#include <iostream>
#include <stack>
#include <utility>

template <class _Tp> class Node;
template <class _Tp> class BST;

template<class _TP>
class Node {
public: // Member types
	typedef _TP					__key_type;
	typedef Node<__key_type>*	__node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// 빈 트리가 아닐 때 중위순회 출력
		if (np != nullptr) {
			__inorder(np);
		}

		return os;
	}
};

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	unsigned maxHeight = 0; // 가장 높은 높이 저장
	std::stack<std::pair<_NodePtr, unsigned>> s; // 노드와 높이를 저장하는 stack

	s.push({ __x, 0 }); // root node를 stack에 넣음

	// 방문할 노드가 없을 때까지 반복
	while (!s.empty()) {
		std::pair<_NodePtr, unsigned> top = s.top(); // stack top 저장
		s.pop(); // 

		if (top.first == nullptr) continue; // 현재 노드가 nullptr이면 continue

		// 현재 노드가 유효할 때
		_NodePtr left = top.first->__left_, right = top.first->__right_; // 현재 노드의 왼쪽, 오른쪽 자식을 저장
		maxHeight = (top.second > maxHeight ? top.second : maxHeight); // 현재 노드의 높이가 maxHeight보다 크면 현재 노드의 높이로 갱신

		// child의 height는 parent의 height + 1
		s.push({ left, top.second + 1 });
		s.push({ right, top.second + 1 });
	}

	return maxHeight;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	unsigned count = 0; // 노드의 개수를 count
	std::stack<_NodePtr> s; // 노드를 저장하는 stack
	s.push(__x); // 시작 node를 넣음

	// 모든 노드를 한 번씩 방문해서 노드의 개수 세기
	while (!s.empty()) { // 남은 노드가 없을 때까지 반복
		_NodePtr top = s.top();
		s.pop(); // 각 노드를 한 번만 방문하기 위해 pop

		if (top == nullptr) continue;
		count++; // 노드가 nullptr가 아닐 때 count + 1
		
		/*
			현재 노드의 자식노드 넣기
			자식 노드가 nullptr일 경우에는 위에 있는 if문에서 걸러짐
		*/
		s.push(top->__left_);
		s.push(top->__right_);
	}

	return count;
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	// node: 탐색할 때 사용하는 노드, retNode: return할 노드
	_NodePtr node = __x, retNode = nullptr;

	// bst에서 최솟값은 가장 왼쪽 노드이기 때문에 왼쪽으로만 이동
	while (node != nullptr) {
		retNode = node; // retNode에 현재 탐색하고 있는 노드 저장
		node = node->__left_; // 현재 노드의 왼쪽 자식으로 이동
	}

	return retNode;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
		// node: 탐색할 때 사용하는 노드, retNode: return할 노드
	_NodePtr node = __x, retNode = nullptr;

	// bst에서 최댓값은 가장 오른쪽 노드이기 때문에 오른쪽으로만 이동
	while (node != nullptr) {
		retNode = node; // retNode에 현재 탐색하고 있는 노드 저장
		node = node->__right_; // 현재 노드의 오른쪽 자식으로 이동
	}

	return retNode;
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) return; // 현재 노드가 nullptr일 때 예외처리

	// 중위 순회는 왼쪽, 부모, 오른쪽 순으로 확인함
	std::cout << "<";
	__inorder(__x->__left_); // 왼쪽 자식 노드로 이동
	std::cout << " " << __x->__key_ << " "; // 현재 노드의 키 값 출력
	__inorder(__x->__right_); // 오른쪽 자식 노드로 이동
	std::cout << ">";
}

// 새로운 Node 생성 함수
template <class _Tp>
Node<_Tp>* getNodeBST() { 
	return new Node<_Tp>;
}

/*
	1. root부터 시작
	2. 키값이 작으면 왼쪽으로 크면 오른쪽으로 더이상 이동할 수 없을 때까지 이동
	3. 끝까지 이동했을 때 그 노드의 키값보다 작으면 왼쪽에 삽입 크면 오른쪽에 삽입
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	// cur: 탐색할 때 사용하는 노드, before: cur이 이동하기 전 탐색한 노드 저장
	_NodePtr cur = __root, before = nullptr;

	// 각 노드의 키값과 삽입하려는 키값을 비교하면서 삽입할 키가 작으면 왼쪽으로 크면 오른쪽으로 더이상 이동할 수 없을 때까지 이동
	while (cur != nullptr) {
		if (key == cur->__key_ ) return std::pair<_NodePtr, bool>(cur, false); // key값이 같을 때 예외처리

		before = cur; // cur이 다음으로 움직이기 전에 저장

		// 삽입될 키값이 노드의 키값보다 작으면 왼쪽으로 이동
		if (key < cur->__key_) { 
			cur = cur->__left_;
		}

		// 삽입될 키값이 노드의 키값보다 작이면 오른쪽으로 이동
		else {
			cur = cur->__right_;
		}
	}

	// 새로운 노드 생성 및 초기화
	_NodePtr newNode = getNodeBST<_Tp>();
	newNode->__key_ = key;

	// 최대한 이동했을 때의 node가 before에 저장되어 있음
	if (before == nullptr) { // 트리에 아무 노드가 없을 때 root에 새로운 노드 삽입
		__root = newNode;
	}

	 else if (key < before->__key_) { // 삽입될 키값이 작으면 왼쪽에 삽입
		before->__left_ = newNode;
	}

	else { // 삽입될 키값이 크면 오른쪽에 삽입
		before->__right_ = newNode;
	}

	return std::pair<_NodePtr, bool>(newNode, true);
}

/*
	1. root부터 시작
	2. 삭제하려는 key와 같은 key를 가진 노드를 찾을 때까지 이동
	3. 같은 key를 가진 노드가 없으면 return nullptr, 있으면 계속
	4. key가 같은 노드의 degree가 2일 때, 우선도가 높은 subtree에서 bst를 유지할 수 있는 key 찾아 삭제하려는 node에 대입
	6. 대체한 key를 가진 원래 노드를 삭제해야하고 해당 노드는 degree가 항상 0 or 1이기 때문에 degree 0 or 1인 노드를 삭제하는 문제로 변경
	7. degree가 1일 때, 삭제하려는 노드의 부모와의 edge를 삭제하려는 노드의 left(right) 자식 노드로 바꿈
	8. degreer가 0일 때, 해당 노드만 삭제하면 되기 때문에 부모와의 연결을 끊음
	9. 메모리 해제는 외부에서
*/
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	 // cur: 탐색할 때 사용하는 노드, parent: cur의 부모를 저장하는 변수
	_NodePtr cur = __root, parent = nullptr;

	/*
		삭제할 노드의 key와 같은 노드를 찾을 때까지 반복
		1. 동일한 키값을 찾았을 때 cur = 삭제하려는 노드, parent = cur의 부모 노드
		2. 동일한 키값을 못 찾았을 때 cur = nullptr
			동일한 키값을 찾을 때까지 이동하기 때문에 찾지 못했을 때는 cur = nullptr 될 때까지 탐색
	*/
	while (cur != nullptr && cur->__key_ != key) {
		parent = cur; // cur이 이동하기 전에 parent에 cur값 저장

		/*
			현재 노드의 키값과 삭제할 키값을 비교해 작으면 왼쪽, 크면 오른쪽
			키값이 같을 때, cur에 동일한 키값을 가진 노드를 저장해야 하기 때문에 다음 노드로 이동하지 않음
		*/
		cur = (key < cur->__key_ ? cur->__left_ : cur->__right_);
	}

	if (cur == nullptr) return nullptr; // 삭제할 노드가 존재하지 않거나 root가 nullptr일 때 예외처리
	
	// 삭제하려는 키값과 같은 노드의 왼쪽 자식과 오른쪽 자식 변수
	_NodePtr left = cur->__left_, right = cur->__right_;

	unsigned leftHeight = __height(left), rightHeight = __height(right); // left, right의 높이
	unsigned leftSize = __size(left), rightSize = __size(right); // left, right의 size

	/* 
		degree가 2일 때 삭제하려는 노드의 자리를 대체할 노드는 
		항상 left subtree의 가장 오른쪽 노드 or right subtree의 가장 왼쪽 노드이기 때문에 항상 degreer가 0 or 1
		삭제하려는 노드의 키값을 대체하는 노드의 키값으로 덮어쓰고, 대체하려는 노드는 cur의 자리로 이동했기 때문에 해당 노드의 원래 노드를 삭제해야함
		즉, degree가 2인 노드를 삭제하는 문제를 degree가 0 or 1인 노드를 삭제하는 문제로 변경
	*/
	if (left != nullptr && right != nullptr) { // degree가 2일 때
		_NodePtr tmpNode = cur; // 나중에 삭제하려는 node에 접근하기 위해 cur이 이동하기 전에 저장

		// cur의 subtree 중 height가 더 크거나 height가 같을 땐 size가 큰 subtree를 사용하는 것이 삭제후 트리의 높이를 줄일 수 있어 우선도가 높음
		// right subtree가 우선도가 더 높을 때
		if (leftHeight < rightHeight || (leftHeight == rightHeight && leftSize < rightSize)) {
			/* 
				right subtree에서 가장 작은 키값을 가진 노드
				cur의 right subtree에는 항상 cur의 키값보다 큰 키값만 있음
				삭제 후 bst를 유지하려면 cur의 자리에 들어갈 키값이 right subtree 중 가장 작은 값
			*/
			cur = __tree_min(right); // cur == (right 노드 ~ right의 가장 왼쪽 노드)
			parent = tmpNode; // 될 수 있는 cur의 부모 노드 중 최상단 노드
			
			// cur이 삭제하려는 노드의 오른쪽 자식 노드가 아닐 때 cur의 부모를 찾음
			if (cur != right) {
				parent = right; // parent가 왼쪽으로 이동(right subtree로 이동)

				// 왼쪽 자식이 cur일 때까지 이동
				while (parent->__left_ != nullptr && parent->__left_->__key_ != cur->__key_) {
					parent = parent->__left_;
				}
			}
		}

		// left subtree가 우선도가 더 높거나 같을 때
		else {
			/* 
				left subtree에서 가장 큰 키값을 가진 노드
				cur의 left subtree에는 항상 cur의 키값보다 작은 키값만 있음
				삭제 후 bst를 유지하려면 cur의 자리에 들어갈 키값이 left subtree 중 가장 큰 값
			*/
			cur = __tree_max(left); // cur == (left 노드 ~ left 노드의 가장 오른쪽 노드)
			parent = tmpNode; // 될 수 있는 cur의 부모 노드 중 최상단 노드

			// cur이 삭제하려는 노드의 왼쪽 자식 노드가 아닐 때 cur의 부모를 찾음
			if (cur != left) {
				parent = left; // parent가 왼쪽으로 이동(left subtree로 이동)

				// 오른쪽 자식이 cur일 때까지 이동
				while (parent->__right_ != nullptr && parent->__right_->__key_ != cur->__key_) {
					parent = parent->__right_;
				}
			}
		}

		tmpNode->__key_ = cur->__key_; // 삭제하려는 노드에 left subtree 키의 최댓값 or right subtree 키의 최솟값 덮어쓰기(삭제하려는 key 값 삭제)

		// cur의 키값을 삭제하려는 노드로 옮겼기 때문에 cur의 키값을 삭제해야 함
		// degree가 0 or 1인 노드를 삭제하는 문제로 변경
		left = cur->__left_;
		right = cur->__right_;
	}

	// degree가 0 or 1일 때
	if (left != nullptr) { // 왼쪽 자식만 있을 때
		// 삭제하려는 노드가 root일 때 왼쪽 자식을 root로 바꿈
		if (parent == nullptr) __root = __root->__left_;

		// 삭제하려는 노드가 부모의 왼쪽 자식일 때 부모의 왼쪽 자식을 삭제하려는 노드의 왼쪽 자식으로 바꿈
		else if (parent->__left_ == cur) parent->__left_ = left;

		// 삭제하려는 노드가 부모의 오른쪽 자식일 때 부모의 오른쪽 자식을 삭제하려는 노드의 왼쪽 자식으로 바꿈
		else parent->__right_ = left;
	}

	else if (right != nullptr) { // 오른쪽 자식만 있을 때
		// 삭제하려는 노드가 root일 때 오른쪽 자식을 root로 바꿈
		if (parent == nullptr) __root = __root->__right_;

		// 삭제하려는 노드가 부모의 왼쪽 자식일 때 부모의 왼쪽 자식을 삭제하려는 노드의 오른쪽 자식으로 바꿈
		else if (parent->__left_ == cur) parent->__left_ = right;

		// 삭제하려는 노드가 부모의 오른쪽 자식일 때 부모의 오른쪽 자식을 삭제하려는 노드의 오른쪽 자식으로 바꿈
		else parent->__right_ = right;
	}

	else { // degree가 0일 때
		// 삭제하려는 노드가 root일 때 root의 자식 노드가 없으므로 root는 nullptr
		if (parent == nullptr) __root = nullptr;

		// 삭제하려는 노드가 부모의 왼쪽 자식 노드일 때 부모의 left를 끊어버려 삭제
		else if (parent->__left_ == cur) parent->__left_ = nullptr;

		// 삭제하려는 노드가 부모의 오른쪽 자식 노드일 때 부모의 right를 끊어버려 삭제
		else parent->__right_ = nullptr;
	}

	return cur;
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
	std::stack<_NodePtr> s;
	s.push(__x);

	// root node부터 시작해 모든 노드를 돌면서 메모리 해제
	while (!s.empty()) {
		_NodePtr cur = s.top(); // 부모 노드 저장
		s.pop(); // 각 노드를 한 번씩만 방문하기 위해 pop

		if (cur == nullptr) continue; // 노드가 nullptr일 때 예외 처리

		// cur의 왼쪽나 오른쪽 노드가 nullptr일 때 위 if문에서 걸러짐
		s.push(cur->__left_);
		s.push(cur->__right_);

		delete cur; // 메모리 해제
	}

	__x = nullptr; // root를 nullptr 변경
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
	BST() : __root_(nullptr) {}

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
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertBST(__root_, key);
	}

	bool erase(const key_type& key) {
		pointer __r = __eraseBST(__root_, key);

		if (__r == nullptr) return false; // 삭제한 노드가 없을 때

		delete __r; // 삭제 성공했으면 메모리 해제

		return true;
	}

	void clear() {
		__clear(__root_);
	}

	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;

		return os;
	}
};

int main(void) {
	BST<int> bst;

	char cmd;
	int key;

	while (std::cin >> cmd >> key) {
		std::pair<const Node<int>*, bool> insertedNode = { nullptr, false }; // insert한 결과 노드 저장

		switch (cmd) {
		case 'i':
			insertedNode = bst.insert(key);

			// 삽입 실패했을 때
			if (insertedNode.second == false) {
				std::cout << "i " << insertedNode.first->__key_ << ": The key already exists\n";
			}

			// 삽입 성공했을 때
			else std::cout << bst << "\n";
			break;
		case 'd':
			bool wasDeleted = bst.erase(key); // 삭제 성공했으면 true, 실패했으면 false

			if (wasDeleted == false) { // 삭제 실패했을 때
				std::cout << "d " << key << ": The key does not exist\n";
			}
			else std::cout << bst << "\n"; // 삭제 성공했을 때

			break;
		}
	}

	bst.clear(); // tree의 모든 노드 메모리 해제

	return 0;
}