#include <iostream>
#include <stack>
#include <utility> 

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node {
	public: // Member types
		typedef _Tp					__key_type; //template를 통해 입력되는 데이터 타입에 따라 키타입 설정됨
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
		int height;
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(0) {} //key값 입력 안했을 경우 : __key_(__key_type())는 멤버 초기화 리스트로 _Tp가 int면 __key_type()은 int() -> 0 즉 T타입의 기본 생성자로 초기화한다.
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), height(0) {}

	// friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// inorder함수로 출력 대체
	// }
};


template <typename T>
T __max(const T& a, const T& b) {
	return (a < b) ? b : a; 
}

template <class _NodePtr, class _Tp>
_NodePtr getNodeBST(){
	return new Node<_Tp>();
}
template <class _NodePtr> // 템플릿 매개변수, 입력받는 클래스에 따라 타입이 변한다.
unsigned __height(_NodePtr& __x) { 
	if(__x == nullptr) return 0;
	return 1+__max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr& __x) { // 멤버변수에 size 추가하지않고, 필요할 때  서브트리의 노드 개수 구하는 방법으로 선택
	if (__x == nullptr) return 0;
	return 1+__size(__x->__left_)+__size(__x->__right_);
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr& __x) {
	if(__x->__left_==nullptr) return __x;
	return __tree_min(__x->__left_);
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr& __x) {
	if(__x->__right_==nullptr) return __x;
	return __tree_max(__x->__right_);
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) return;
	std::cout << '<';              
    if (__x->__left_) __inorder(__x->__left_);
    std::cout << ' ' << __x->__key_ << ' ';   
    if (__x->__right_) __inorder(__x->__right_);
    std::cout << '>'; 
}


template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __p = __root;
	_NodePtr __q = nullptr;
	std::stack<_NodePtr> stack; // 트리 경로에 따라 높이를 재설정 해주기 위해 stack 사용
	while(__p != nullptr){
		if(key == __p->__key_) { // 이미 key값이 트리에 존재하는 경우
			return std::pair<_NodePtr, bool>(__p, false);
		}
		__q = __p;
		stack.push(__q);
		if(key < __p->__key_) __p = __p->__left_;
		else __p = __p->__right_;
	}
	_NodePtr newNode = getNodeBST<_NodePtr, _Tp>();
	newNode->__key_ = key;
	if (__root == nullptr) { // 트리에 노드가 없는 경우
        __root = newNode;
    } else {
        if (key < __q->__key_) __q->__left_  = newNode;
        else __q->__right_ = newNode;
    }
	stack.push(newNode);
	while(!stack.empty()){
		_NodePtr node = stack.top();
		stack.pop();
		node->height = 1+__max(__height(node->__left_),__height(node->__right_));
	}
	return std::pair<_NodePtr, bool>(newNode, true);
}

template <class _NodePtr, class _Tp>
// 삭제할 값을 찾지 못해 nullptr을 반환하는 것과, 값 제거는 성공했지만 nullptr 상태인 __root를 반환하는 것을 구분하기 위해 std::pair<_NodePtr,bool> 반환 형식으로 변경함. 삭제 성공시 true, 실패 시 false 반환함.
std::pair<_NodePtr, bool> __eraseBST(_NodePtr& __root, const _Tp& key) {
	if(__root == nullptr) return std::pair<_NodePtr, bool>(__root, false); // 트리에 노드 존재 X
	_NodePtr __p = __root;
	_NodePtr __q = nullptr;
	std::stack<_NodePtr> stack;
	while(__p!=nullptr && key != __p->__key_){ // 찾는 key를 가진 노드를 찾아 p에 객체 주소 저장.
		__q = __p;
		stack.push(__q);
		if(key<__p->__key_) __p = __p->__left_;
		else __p = __p->__right_;
	}
	if(__p == nullptr) return std::pair<_NodePtr, bool>(__root, false); // 찾는 데이터가 존재하지 않음
	
	if(__p->__left_ != nullptr && __p->__right_ != nullptr){ // 찾은 데이터 자식이 두명 존재
		_NodePtr tempNode = __p; // 같은 노드를 가리키고 있으며, __p값이 변해도 tempNode의 값은 그대로 처음 __p를 가리키고 있음.
		stack.push(tempNode); 
		if(__p->__left_->height<__p->__right_->height || ((__p->__left_->height == __p->__right_->height) && (__size(__p->__left_)<__size(__p->__right_)))){ //삭제할 노드의 왼쪽 서브트리의 높이보다 오른쪽 서브트리의 높이가 크거나, 높이는 같지만 오른쪽 서브트리 노드의 개수가 더 많은 경우
			__q = tempNode;                 // 부모를 미리 tempNode로 설정
			__p = tempNode->__right_;
			while (__p->__left_ != nullptr) {
				__q = __p;
				stack.push(__p);
				__p = __p->__left_; // 제거할 __p를 루트로 하는 트리중 가작 작은 노드 찾음
			}
			tempNode->__key_ = __p->__key_;
			if (__q->__left_ == __p) __q->__left_ = __p->__right_; // 만약 최소 노드가 오른쪽 자식을 가지고 있을 경우 제거될 최소 노드 자리에 오른쪽 자식 노드 옮겨줌.
			else __q->__right_ = __p->__right_; // 최소 노드가 부모의 오른쪽 자식인 경우 실행. (루트 바로 아래 자식인 경우 해당 될 수 있음)
		} else {
			__q = tempNode;                
			__p = tempNode->__left_;
			while (__p->__right_ != nullptr) {
				__q = __p;
				stack.push(__p);
				__p = __p->__right_;
			}
			tempNode->__key_ = __p->__key_;
			if (__q->__right_ == __p) __q->__right_ = __p->__left_;
			else __q->__left_ = __p->__left_;

		}
	} else{ // 자식이 1개거나 0개인 경우
		if(__p->__left_ != nullptr){ // 왼쪽 자식만 존재
			if(__q==nullptr) __root = __root->__left_;
			else if(__q->__left_ == __p) __q->__left_ = __p->__left_;
			else __q->__right_ = __p->__left_;
		} else if(__p->__right_ != nullptr){ // 오른쪽 자식만 존재
			if(__q==nullptr) __root = __root->__right_;
			else if(__q->__left_ == __p) __q->__left_ = __p->__right_;
			else __q->__right_ = __p->__right_;
		} else{ // 자식이 없음.
			if(__q==nullptr) __root = nullptr;
			else if(__q->__left_ == __p) __q->__left_ = nullptr;
			else __q->__right_ = nullptr;
		}
	}
	
	while(!stack.empty()){
		_NodePtr node = stack.top();
		stack.pop();
		if (node == nullptr || node == __p) continue; // 삭제된 노드 접근 방지
		node->height = 1 + __max(__height(node->__left_), __height(node->__right_));
	}

	delete __p; // 삭제한 노드의 메모리 할당 해제

	return std::pair<_NodePtr, bool>(__root, true); // 삭제 성공
}


template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if(__x == nullptr) return;
	else{
		__clear(__x->__left_);
		__clear(__x->__right_);
		delete __x;
	}
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
		~BST() { clear(); }

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
			if (!__root_) return;
			// use __inorder or write your own code here
			__inorder(__root_);
			std::cout << std::endl;
		}
		
	public: // Modifier
		std::pair<const_pointer, bool> insert(const key_type& key) {
			// use __insertBST or write your own code here
			return __insertBST(__root_, key);
		}

		std::pair<const_pointer, bool> erase(const key_type& key) {
			// use __eraseBST or write your own code here
			return __eraseBST(__root_, key);
		}

		void clear() {
			// use __clear or write your own code here
			__clear(__root_);
			__root_ = nullptr;
		}
	
	// friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		// inorder함수로 출력 대체
	// }
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
				if (tree.erase(key).second == false) {
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
	}
	tree.clear();
	return (0);
}
