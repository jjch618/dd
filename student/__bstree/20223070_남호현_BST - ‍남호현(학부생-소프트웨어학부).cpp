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
	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
        //nullptr이면 출력X
        if(np==nullptr) return os;
        //Node를 "<{left subtree} {key} {right subtree}>"형태로 출력
        //재귀적으로 좌우 서브트리 형태 출력
        os << "<" << np->__left_ << " " << np->__key_ << " " << np->__right_ << ">";
        return os;
	}
};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

//트리의 높이 구하는 함수
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    if(__x==nullptr) return 0;
    //삼항 연산자를 이용해 좌우 서브트리의 높이를 재귀적으로 구함
    return (__height(__x->__left_)>__height(__x->__right_)) ? __height(__x->__left_)+1 : __height(__x->__right_)+1;
}
//트리의 노드 개수 구하는 함수
template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    if(__x==nullptr) return 0;
    //재귀적으로 트리의 노드 개수를 구함
    return __size(__x->__left_)+__size(__x->__right_)+1;
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
//트리의 최솟값 구하는 함수
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    //트리의 루트에서 왼쪽으로만 이동하면 트리의 최솟값
    while(__x->__left_!=nullptr)
        __x=__x->__left_;
    
    return __x;
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
//트리의 최댓값 구하는 함수
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
    //트리의 루트에서 오른쪽으로만 이동하면 트리의 최댓값
    while(__x->__right_!=nullptr)
        __x=__x->__right_;

    return __x;
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
//트리를 중위순회로 출력하는 함수
template <class _NodePtr>
void __inorder(_NodePtr __x) {
    //Node클래스의 <<연산자 오버로딩 이용해서 출력
    std::cout<<__x;
}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
//트리에 새로운 노드 삽입하는 함수
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    //새로운 노드 생성
    _NodePtr newnode=new Node<_Tp>;
    newnode->__key_=key;
    
    //트리의 루트가 없어서 newnode를 루트로
    if(__root==nullptr){ 
        __root=newnode;
        return std::pair<_NodePtr, bool>(__root, true);
    }
    
    //ptr: newnode삽입할 위치  q: ptr의 부모 노드
    _NodePtr ptr=__root;
    _NodePtr q=nullptr;
    
    //키 값 비교로 삽입 위치 찾기
    while(ptr!=nullptr){
        q=ptr;
        if(key<ptr->__key_) ptr=ptr->__left_;
        else if(key>ptr->__key_) ptr=ptr->__right_;
        else {
            //이미 있는 키값이면 동적할당 한거 지우기
            delete newnode;
            return std::pair<_NodePtr, bool>(ptr, false);
        }
    }
    
    //부모노드와 키값 비교로 newnode 삽입 위치 찾기
    if(q->__key_<key) 
        q->__right_=newnode;
    else 
        q->__left_=newnode;
    return std::pair<_NodePtr, bool>(newnode, true);
}

/*
* Root node가 삭제되는 경우를 고려하여 매개 변수 "__root" 를 참조 타입으로 받도록 설계하였습니다.
* "__root = 대체될 노드"로 BST class의 멤버 변수 __root_ 값을 변경할 수 있습니다.
* 현재 로직에서 존재하지 않는 key를 erase하려고 할 때, nullptr을 반환해야 정상 작동하도록 설계되어 있습니다.
*/
//트리에 주어진 키값과 같은 노드를 삭제하는 함수
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    //ptr:지울 노드, q:p의 부모 노드
    _NodePtr ptr=__root;
    _NodePtr q=nullptr;
    
    //ptr, q찾기
    while(ptr!=nullptr && ptr->__key_!=key){
        q=ptr;
        if(ptr->__key_<key) ptr=ptr->__right_;
        else ptr=ptr->__left_;
    }
    
    //지울 노드가 없으면
    if(ptr==nullptr){
        return nullptr;
    }
    
    //지울 노드가 루트노드(자식 0개, 1개, 2개)
    else if(q==nullptr){
        //자식 0개-루트 비우기
        if(ptr->__left_==nullptr && ptr->__right_==nullptr){
            __root=nullptr;
            return ptr;
        }
        //자식 1개-루트에 자식노드 값 넣기
        else if(ptr->__left_!=nullptr && ptr->__right_==nullptr){
            __root=ptr->__left_;
            return ptr;
        }
        else if(ptr->__left_==nullptr && ptr->__right_!=nullptr){
            __root=ptr->__right_;
            return ptr;
        }
        //자식 2개-좌우 서브트리의 높이, 사이즈 비교해서 큰쪽에서 빼기 
        else{
            //왼쪽 서브트리 높이가 높음
            //왼쪽 서브트리에서 최댓값 뽑기
            if(__height(ptr->__left_)>__height(ptr->__right_)){
                _NodePtr lmax=__tree_max(ptr->__left_);
                __eraseBST(__root, lmax->__key_);
                __root->__key_=lmax->__key_;
                return lmax;
            }
            //오른쪽 서브트리 높이가 높음
            //오른쪽 서브트리에서 최솟값 뽑기
            else if(__height(ptr->__left_)<__height(ptr->__right_)){
                _NodePtr rmin=__tree_min(ptr->__right_);
                __eraseBST(__root, rmin->__key_);
                __root->__key_=rmin->__key_;
                return rmin;
            }
            //양쪽서브트리 높이 같으면
            else{
                //왼쪽이 노드 수 많거나 같음
                //왼쪽에서 최댓값 뽑기
                if(__size(ptr->__left_)>=__size(ptr->__right_)){
                    _NodePtr lmax=__tree_max(ptr->__left_);
                    __eraseBST(__root, lmax->__key_);
                    __root->__key_=lmax->__key_;
                    return lmax;
                }
                //오른쪽이 노드 수 많음
                //오른쪽에서 최댓값 뽑기
                else{
                    _NodePtr rmin=__tree_min(ptr->__right_);
                    __eraseBST(__root, rmin->__key_);
                    __root->__key_=rmin->__key_;
                    return rmin;
                }
            }
        }
    }
    
    //ptr이 루트노드 아님
    else{
        //ptr 자식 없음-부모노드가 ptr가르키는것만 nullptr로
        if(ptr->__left_==nullptr && ptr->__right_==nullptr){
            if(q->__left_==ptr) q->__left_=nullptr;
            else q->__right_=nullptr;
            return ptr;
        }
        //ptr 자식 1개-부모랑 자식이랑 연결
        else if(ptr->__left_!=nullptr && ptr->__right_==nullptr){
            if(q->__left_==ptr) q->__left_=ptr->__left_;
            else q->__right_=ptr->__left_;
            return ptr;
        }
        else if(ptr->__left_==nullptr && ptr->__right_!=nullptr){
            if(q->__left_==ptr) q->__left_=ptr->__right_;
            else q->__right_=ptr->__right_;
            return ptr;
        }
        //ptr 자식 2개
        else{
            //왼쪽 서브트리 높이가 높음
            //왼쪽에서 최댓값 뽑기
            if(__height(ptr->__left_)>__height(ptr->__right_)){
                _NodePtr lmax=__tree_max(ptr->__left_);
                __eraseBST(__root, lmax->__key_);
                ptr->__key_=lmax->__key_;
                return lmax;
            }
            //오른쪽 서브트리 높이가 높음
            //오른쪽에서 최솟값 뽑기
            else if(__height(ptr->__left_)<__height(ptr->__right_)){
                _NodePtr rmin=__tree_min(ptr->__right_);
                __eraseBST(__root, rmin->__key_);
                ptr->__key_=rmin->__key_;
                return rmin;
            }
            //양쪽 높이가 같으면 노드 수 바교
            else{
                //왼쪽 서브트리가 노드 같거나 많음
                //왼쪽에서 최댓값 뽑기
                if(__size(ptr->__left_)>=__size(ptr->__right_)){
                    _NodePtr lmax=__tree_max(ptr->__left_);
                    __eraseBST(__root, lmax->__key_);
                    ptr->__key_=lmax->__key_;
                    return lmax;
                }
                //오른쪽 서브트리가 노드 많음
                //오른쪽에서 최솟값 뽑기
                else{
                    _NodePtr rmin=__tree_min(ptr->__right_);
                    __eraseBST(__root, rmin->__key_);
                    ptr->__key_=rmin->__key_;
                    return rmin;
                }
            }
        }
    }
    
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
//종료 전에 트리에 남아있는 노드들의 메모리 할당 해제하는 함수
template <class _NodePtr>
void __clear(_NodePtr& __x) {
    if(__x==nullptr) return;
    //재귀로 좌우 서브트리 메모리 할당 해제
    if(__x->__left_!=nullptr) __clear(__x->__left_);
    if(__x->__right_!=nullptr) __clear(__x->__right_);
    //루트 메모리 할당 해제
    delete __x;
    //dangling pointer 방지
    __x=nullptr;
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

			// Client still needs to destruct/deallocate it
			// Or memory leak will occur
			//리턴된 값 메모리 할당 해제
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