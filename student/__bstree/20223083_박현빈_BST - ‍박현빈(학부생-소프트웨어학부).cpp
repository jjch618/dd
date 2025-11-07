#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;

// Node 클래스 템플릿 선언
template <class _Tp> 
class Node {
	public: // Member types
		typedef _Tp					__key_type;             // Key type 별칭 
		typedef Node<__key_type>*	__node_pointer_type;    // Node 포인터 별칭
	
	public: // Member variables
		__key_type			__key_;                         // Key type key 생성
		__node_pointer_type	__left_;                        // Node type 포인터 생성1       
		__node_pointer_type	__right_;                       // Node type 포인터 생성2
	
	public: // Constructor -> 생성자 선언 영역
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}                // 기본 생성자 키를 타입의 기본값으로 초기화
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}    // 값을 받아 키를 초기화 하는 생성자


	// inorder traversal을 대체하는 operator<<, (inorder 함수를 대체)
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (np == nullptr){
			return os;
		}

		os << "<";
		os << np->__left_;
		os << " ";
		os << np->__key_;
		os << " ";
		os << np->__right_;
		os << ">";
		
		return os;
	}
};

// 트리의 높이를 반환하는 함수
template <class _NodePtr>   
unsigned __height(_NodePtr __x) {   
	if(__x == nullptr){
		// 노드가 없는 경우에는 해당 함수가 호출될 수 없으므로 -1을 반환해도 반환 타입에 문제가 되지 않는다. (해당 함수는 노드가 있고 해당 노드의 차수가 2인 경우에만 호출된다. )
		return -1;	// 초기값을 -1로 할지 1로 할지 고민된다. -> -1이 맞다 노드가 한개인 경우 높이가 0으로 되게 하려면 -1이 맞다
	}

	int left_h = __height(__x->__left_);
	int right_h = __height(__x->__right_);
	int max_h = (left_h > right_h ? left_h : right_h);

	return 1 + max_h;
}

// 트리의 크기를 반환하는 함수
template <class _NodePtr>           
unsigned __size(_NodePtr __x) {		

	if(__x == nullptr){
		return 0; 
	}

	// 현재 노드를 기준으로 서브트리를 재귀적으로 호출하여 트리의 크기를 반환
	return 1 + __size(__x->__left_) + __size(__x->__right_);
}

// 트리의 최소값을 반환하는 함수 (minNode와 동일)
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) { 
	_NodePtr p = __x;

	// 트리의 가장 왼쪽에 있는 노드 까지 접근 (BST 특징에 의해 최솟값을 갖는 키는 트리의 왼쪽 끝에 있다)
	while(p->__left_ != nullptr){
		p = p->__left_;
	}

	return p;
}

// 트리의 최대값을 반환하는 함수 (maxNode와 동일)
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) { 
	_NodePtr p = __x;

	// 트리의 가장 오른쪽에 있는 노드 까지 접근 (BST 특징에 의해 최댓값을 갖는 키는 트리의 오른쪽 끝에 있다)
	while(p->__right_ != nullptr){	
		p = p->__right_;
	}

	return p;
}

// 트리의 중위 순회하는 함수 (operator<<로 대체 구현)
template <class _NodePtr>
void __inorder(_NodePtr __x) {      
	// inorder traversal을 대체하는 operator<<, 해당 방식으로 구혀하였기에 inorder함수는 구현은 하였으나, 사용하지는 않습니다
    if (__x == nullptr) return;
    std::cout << "<";
    __inorder(__x->__left_);
    std::cout << " " << __x->__key_ << " ";
    __inorder(__x->__right_);
    std::cout << ">";
}

// 트리에서 key값을 갖는 노드의 포인터를 반환 함수 (교재의 searchBST와 동일)
template <class _NodePtr, class _Tp>
_NodePtr __searchBST(_NodePtr& __root, const _Tp& key) { 
	_NodePtr p = __root;	// 타겟 노드 포인터

		
	while( p != nullptr && p->__key_ != key ){ 	// 현재 노드가 nullptr이 아니고 현재 노드의 키값이 제거할 노드와 다른 경우 반복 
		if (key < p->__key_){
			p = p->__left_;
		}
		else{
			p = p->__right_;
		}
	}

	return p;
}

// 트리에서 key의 부모를 검색하는 함수 (교재의 searchParent와 동일)
template <class _NodePtr, class _Tp>
_NodePtr __searchParent(_NodePtr& __root, const _Tp& key) { 
	_NodePtr q = nullptr;	// 타겟 노드의 부모 노드 포인터
	_NodePtr p = __root;	// 타겟 노드 포인터

	while( p != nullptr && p->__key_ != key ){	// 현재 노드가 nullptr이 아니고 현재 노드의 키값이 제거할 노드와 다른 경우 반복 
		q = p; 
		if (key < p->__key_){
			p = p->__left_;
		}
		else{
			p = p->__right_;
		}
	}

	return q;
}

// 트리에 키값을 갖는 노드를 추가하는 함수 (InsertBST와 동일)
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {	
	_NodePtr p = __searchBST(__root, key);		// 삭제할 노드 포인터 
	_NodePtr q = __searchParent(__root, key);	// 삭제할 노드의 부모 노드 포인터 

	if (p != nullptr) return std::pair<_NodePtr, bool>(p, false);

	_NodePtr newNode = new Node<_Tp>(key); // getNodeBST 함수사용

    if (__root == nullptr){	// Tree가 비어 있는지를 __root 가 nullptr과 같은지를 통해서 확인 (BST 생성자 기본값이 nullptr임)
        __root = newNode;
    } 
    else if(newNode->__key_ < q->__key_){   // 삽입할 값이 부모 노드의 키값보다 작은 경우 
        q->__left_ = newNode;
    }
    else{                                   // 삽입할 값이 부모 노드의 키값보다 큰 경우
        q->__right_ = newNode;
    }

    return std::pair<_NodePtr, bool>(newNode, true); 
}

// 트리에서 키값을 갖는 노드를 삭제하는 함수 (deleteBST와 동일)
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {	
	_NodePtr p = __searchBST(__root, key);		// 삭제할 노드 포인터 
	_NodePtr q = __searchParent(__root, key);	// 삭제할 노드의 부모 노드 포인터 
	_NodePtr r = nullptr;						// 제거할 노드의 차수가 2인경우 새롭게 제거할 노드 포인터
	bool flag = true;							// 제거할 노드의 차수가 2인경우 삭제할 노드의 방향을 명시 true(왼쪽), false(오른쪽)

	if (p == nullptr){	// tree에 삭제할 노드가 없는 경우 -> nullptr 반환
		return nullptr;		
	}

	// 아래 부터는 삭제할 노드가 있는 경우에 대한 연산입니다

	if (q == nullptr){	// 삭제할 노드가 root node

		if(p -> __left_ == nullptr && p->__right_ == nullptr){		// root node의 차수가 0인 경우 
			__root = nullptr;	//  p가 반환되어 힙 영역에서 p가 가리키는 값이 해제 되었을때, __root가 잘못된 주소를 가리키는 댕글링 포인터를 방지하기 위함
		}
		else if (p->__left_ == nullptr || p->__right_ == nullptr){	// root node의 차수가 1인 경우 
			if(p->__left_ != nullptr){	// 자식이 왼쪽에 있는 경우 
				__root = p->__left_;				// 왼쪽 노드가 루트노드가 된다. 
			}
			else{						// 자식이 오른쪽에 있는 경우 
				__root = p->__right_;			// 오른쪽 노드가 루트 노드가 된다.
			}
		}
		else{														// root node의 차수가 2인 경우 
			if (__height(p->__left_) > __height(p->__right_)){			// 왼쪽 서브트리의 높이가 더 큰경우 
				r = __tree_max(p->__left_);
			}
			else if (__height(p->__left_) < __height(p->__right_)){		// 오른쪽 서브트리의 높이가 더 큰경우
				r = __tree_min(p->__right_);
				flag = false;
			}
			else{														// 양쪽 서브트리의 높이가 같은 경우
				if (__size(p->__left_) >= __size(p->__right_)){				// 왼쪽 서브트리에 노드가 더 많거나 같은 경우 
					r = __tree_max(p->__left_);
				}else{
					r = __tree_min(p->__right_);							// 오른쪽 서브트리의 노드가 더 만은 경우 
					flag = false;
				}
			}
			
			p->__key_ = r->__key_;
			if (flag){	// 삭제할 노드의 위치를 flag에 따라서 결정한다. 
				return __eraseBST(p->__left_, r->__key_);	// 새롭게 삭제할 노드인 r을 제거 
			}else{
				return __eraseBST(p->__right_, r->__key_);	// 새롭게 삭제할 노드인 r을제거
			}
		}
	}
	else{	// 삭제할 노드가 root node가 아닌 경우 
		if(p->__left_ == nullptr && p->__right_ == nullptr){		// node의 차수가 2인 경우 
			if(q->__left_ == p){						
				q->__left_ = nullptr;
			}else{
				q->__right_ = nullptr;
			}
		}
		else if(p->__left_ == nullptr || p->__right_ == nullptr){	// node의 차수가 1인 경우 
			if (p->__left_ != nullptr){			// 왼쪽 서브 트리가 있는 경우
				if(q->__left_ == p){				// 삭제할 노드가 부모의 왼쪽 서브트리인 경우 
					q->__left_ = p->__left_;
				}else{
					q->__right_ = p->__left_;
				}
			}else{								// 오른쪽 서브 트리가 있는 경우
				if(q->__left_ == p){				// 삭제할 노드가 부모의 오른쪽 서브트리인 경우 
					q->__left_ = p->__right_;
				}else{
					q->__right_ = p->__right_;
				}
			}
		}
		else{														// node의 차수가 0인 경우 
			if (__height(p->__left_) > __height(p->__right_)){			// 왼쪽 서브트리의 높이가 더 큰경우 
				r = __tree_max(p->__left_);
			}
			else if (__height(p->__left_) < __height(p->__right_)){		// 오른쪽 서브트리의 높이가 더 큰경우
				r = __tree_min(p->__right_);
				flag = false;
			}
			else{														// 양쪽 서브트리의 높이가 같은 경우
				if (__size(p->__left_) >= __size(p->__right_)){				// 왼쪽 서브트리에 노드가 더 많거나 같은 경우 
					r = __tree_max(p->__left_);
				}else{
					r = __tree_min(p->__right_);							// 오른쪽 서브트리의 노드가 더 만은 경우 
					flag = false;
				}
			}
			
			p->__key_ = r->__key_;
			if (flag){	// 삭제할 노드의 위치를 flag에 따라서 결정한다. 
				return __eraseBST(p->__left_, r->__key_);	// 새롭게 삭제할 노드인 r을 제거 
			}else{
				return __eraseBST(p->__right_, r->__key_);	// 새롭게 삭제할 노드인 r을제거
			}
		}
	}

	// 삭제할 노드인 p를 반환
	return p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr){
		return; 
	}

	__clear(__x->__left_);
	__clear(__x->__right_);

	delete __x;
	__x = nullptr;
}

// 아래는 반드시 사용해야하는 BST 클래스입니다.
template <class _Tp>
class BST {
	public: // 멤버 타입의 별칭을 지정
		typedef _Tp						key_type;
		typedef std::size_t				size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {}  // 기본 생성자, __root_를 nullptr로 초기화

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
			// use __eraseBST or write your own code here
			// 삭제를 하는 경우 삭제한 노드의 포인터를 반환
			pointer __r = __eraseBST(__root_, key);

			// Client still needs to destruct/deallocate it
			// Or memory leak will occur
			delete __r;		// 삭제한 노드를 메모리 상에서 제거

			return __r; 
		}

		void clear() {
			__clear(__root_);
		}
	
	// tree 객체가 호출 된고, 내부에서 node객체를 해당 연산으로 한번 더 호출
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;
		return os;
	}
};

int main(int argc, char **argv) {
	BST<int>	tree;           // BST 객체 생성
	char		command;        // command 문자 변수 선언
	int			key;            // key 정수 변수 선언 

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key).second == false) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == nullptr) {	// null인 경우 -> 삭제할 값이 존재 하지 않는다. 
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