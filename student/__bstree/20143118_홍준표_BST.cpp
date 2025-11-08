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
	* inorder 함수를 대신해서 사용하였습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if(np == nullptr)
		{return os;}
		os << "<";
		os << np->__left_;// 왼쪽 서브트리 방문
		os <<" "<< np->__key_ <<" "; // 현재 노드 키값
		os << np->__right_; // 오른쪽 서브트리 방문
		os << ">" ;  
        return os;
	}
};

	/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if(__x == nullptr) { return 0; } // 노드가 null이면 0
	unsigned left__h = __height(__x->__left_); //왼쪽 서브트리 높이 
	unsigned right__h = __height(__x ->__right_); //오른쪽 서브트리 높이 
	return 1+ std::max(left__h,right__h); // 높이 1에다 왼쪽과 오른쪽 높이 중 큰 값을 더함

}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if(__x == nullptr) {return 0;} // 노드가 null이면 0
	unsigned left_s = __size(__x ->__left_); // 왼쪽 서브트리 사이즈
	unsigned right_s = __size(__x->__right_); // 오른쪽 서브트리 사이즈
	return 1 + left_s + right_s ; //전체 노드 개수
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	if(__x == nullptr) {return __x;}
	if(__x->__left_) {return __tree_min(__x->__left_);} // 왼쪽 서브트리가 있으면 왼쪽 서브트리에서 작은 값
    return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if(__x == nullptr) {return __x;}
	if(__x->__right_) {return __tree_max(__x->__right_);} // 오른쪽 서브트리가 있으면 오른쪽 서브트리에서 큰 값
    return __x;
}

template <class _NodePtr,class _Tp> 
_NodePtr getNodeBST(const _Tp& key) //getNodeBST 구현
{
    return new Node<_Tp>(key);
}
/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
* operator<<를 사용하였지만 구현은 했습니다.
*/
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	  if(__x==nullptr) {
        return;
    }
    std::cout << "<";
    __inorder(__x->__left_);
    std::cout << " " << __x->__key_ << " ";
    __inorder(__x->__right_);
    std::cout << ">";
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
//__root값을 변경하는 경우를 위해 참조 타입으로 받도록 설계하였습니다.

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr __p = __root; //p는 삽입할 위치 탐색용
	_NodePtr __q = nullptr; //q는 p의 부모노드
	while(__p != nullptr ) // new key의 삽입위치 p를 찾음
	{
		if(__p->__key_ == key) // 이미 존재하면 false를 리턴
		{return std::pair<_NodePtr, bool>(__p, false);}
		__q = __p ; // 존재하지 않으면 q를 p로
		if(key < __p->__key_) { __p = __p->__left_;} // new key값이 작으면 p의 왼쪽서브트리로 이동
		else{ __p = __p->__right_; } // 반대로 크면 p의 오른쪽 서브트리로 이동
	}
	_NodePtr __newNode = getNodeBST<_NodePtr>(key); //new key값을 갖는 새로운 노드 생성 getNodeBST함수 사용
	if(__root == nullptr)  //root가 null이면 root에 삽입
	{ __root = __newNode;
	  return std::pair<_NodePtr, bool>(__root, true); //삽입위치인 root와 true 리턴
	}
	else if(key <  __q -> __key_) //삽입할 키값이 q보다 작으면 q의 왼쪽에 삽입
	{
		__q->__left_ = __newNode;
		 return std::pair<_NodePtr, bool>(__q->__left_ ,true); 
	}
    else // 반대의 경우 오른쪽에 삽입
	{
		__q->__right_ = __newNode;
		return std::pair<_NodePtr, bool>(__q->__right_, true);
	}
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
/* _NodePtr로 반환할 경우 루트노드만 있는 노드에서 루트 노드를 삭제할 경우 루트가 nullptr값이 되어 main함수에서
   삭제가 잘 되었음에도 삭제가 안되었다고 인식해 do not exist라는 오류 메세지를 출력하는 경우가 생겨 bool타입으로 반환받도록 설계하였습니다.
*/
template <class _NodePtr, class _Tp>
bool __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr __p = __root;   // 삭제할 노드 탐색용 포인터
    _NodePtr __q = nullptr;  // 삭제할 노드의 부모 포인터
    _NodePtr __r = nullptr;  //삭제할 노드의 대체할 노드 r
	int i;  // 수업 슬라이드에서 왼쪽 오른쪽 방향 정해줄 플래그(FLAG) (i=1은 왼쪽 i=-1은 오륵쪽)
    // 삭제할 노드 탐색 
    while (__p != nullptr ) {
		if(key == __p->__key_){break;} //키값이 같으면 찾았으므로 바로 while문 종료
        __q = __p; // 다르면 q를 p로 내리고 키값을 비교하여 p도 왼쪽 또는 오른쪽으로 이동
        if (key < __p->__key_)
            __p = __p->__left_;
        else
            __p = __p->__right_;
    }
	if(__p==nullptr) {return false;} // 키값을 못찾은 경우 삭제할 값이 없으므로 false를 리턴

	
	if(__p->__left_ ==nullptr && __p->__right_==nullptr) // 리프노드인 경우
	{ 
	  if(__q == nullptr ) {__root = nullptr;} // 루트 노드가 삭제될 경우
	  else if(__q->__left_==__p) {__q->__left_=nullptr;} //왼쪽 자식이 삭제되는 경우
	  else { __q->__right_ = nullptr;} //오른쪽 자식인 경우
	  delete __p; //메모리 삭제
	  return true; //삭제가 되었으므로 true를 리턴
	}
	else if(__p->__left_ !=nullptr && __p->__right_==nullptr) // 왼쪽 자식만 있는 경우
	{ 
	  if (__q == nullptr) __root = __p->__left_; //루트 노드가 삭제될 경우
      else if(__q->__left_ == __p){ __q->__left_ = __p->__left_;} //왼쪽 자식이 삭제되는 경우
	  else{__q->__right_ = __p->__left_;} //오른쪽 자식인 경우
	  delete __p; //메모리 삭제
	  return true; //삭제가 되었으므로 true를 리턴
	}
	else if(__p->__left_ ==nullptr && __p->__right_!=nullptr) //오른쪽 자식만 있는 경우
	{  if (__q == nullptr) {__root = __p->__right_;} //루트 노드가 삭제될 경우
      else if(__q->__left_ == __p){ __q->__left_ = __p->__right_;} //왼쪽 자식이 삭제되는 경우
	  else{__q->__right_ = __p->__right_;} //오른쪽 자식인 경우
	  delete __p; //메모리 삭제
	  return true; //삭제가 되었으므로 true를 리턴
	}
	//왼쪽 오른쪽 전부 있는 경우(차수가 2인 경우)
    else
	{
		if(__height(__p->__left_) > __height(__p->__right_)) //노드의 높이 비교 왼쪽이 큰 경우
		{
		   __r = __tree_max(__p->__left_); //왼쪽 서브트리에서 가장 큰 노드로 대체
             i = 1; // 플래그는 왼쪽
		}
		else if(__height(__p->__left_) < __height(__p->__right_)) //오른쪽이 큰 경우
		{
		    __r = __tree_min(__p->__right_); //오른쪽 서브트리에서 가장 작은 노드로 대체
             i = -1; //플래그는 오른쪽
		}
		else //높이가 같은 경우 노드의 개수를 비교
		{
			if(__size(__p->__left_) >= __size(__p->__right_)) //왼쪽이 노드가 많은 경우
			{
			__r = __tree_max(__p->__left_);
             i = 1;
		
			}
			else
			{
            __r = __tree_min(__p->__right_); // 오른쪽이 많은 경우
             i = -1;
			}
		}
		__p->__key_ = __r->__key_; //삭제될 노드인 __p의 키값을 대체할 노드인 __r의 키값으로 변경
		if(i== 1) {__eraseBST(__p->__left_,__r->__key_);} //i=1이므로 왼쪽서브트리에서 __r의 키값을 갖는 노드 삭제
		else{ __eraseBST(__p->__right_,__r->__key_);} ////i=-1이므로 오른쪽서브트리에서 __r의 키값을 갖는 노드 삭제
		return true;
	}

 }


// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if(__x == nullptr) {return;}
    __clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
	return ;
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
        /*__r 포인터를 이용하여 반환받을 경우 root만 있을 때 root노드를 삭제하면 
		 * root노드 값이 nullptr값을 가져 아래 main함수에서 오류 메세지를 출력을 합니다
		 * 그래서 bool타입으로 리턴받도록 설계하였습니다.
		 */
		bool erase(const key_type& key) {
			// use __eraseBST or write your own code here
			return __eraseBST(__root_, key);
           }
            
			// Client still needs to destruct/deallocate it
			// Or memory leak will occur

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
				if (tree.erase(key) == false) {
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