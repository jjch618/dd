#include <iostream>
#include <stack>

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

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
        if(np == nullptr)
            return os;
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

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    // left와 right로 가면서 최대 height를 제귀적으로 구현.
    // 강의자료대로 height를 level의 max와 동일하게 정의. (간선 수 x)
    // 만약 __x가 nullptr이라면 0.
    if(__x == nullptr)
        return 0;
    unsigned left_height = __height(__x -> __left_);
    unsigned right_height = __height(__x -> __right_);

    // left node까지의 height와 right_height를 비교. 가장 큰 값을선택.
    unsigned child_height = left_height > right_height ? left_height : right_height;
    
    // 현재 레벨 + child heights
    return 1 + child_height;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    // height와 마찬가지로 제귀적으로 left와 right node의 사이즈를 계산.
    if(__x == nullptr)
        return 0;
    unsigned left_size = __size(__x -> __left_);
    unsigned right_size = __size(__x -> __right_);

    // 현재 node(1) + left와 right의 사이즈 합.
    return left_size + right_size + 1;
}


template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    _NodePtr __p = __x;

    // 계속 왼쪽으로 가면 min임.
    // left가 null이 아닐때까지 왼쪽으로 이동.
    while(__p -> __left_ != nullptr)
    {
        __p = __p->__left_;
    }
    return __p;
}


template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	_NodePtr __p = __x;

    // 계속 오른쪽으로 가면 max임.
    // right가 null이 아닐때까지 오른쪽으로 이동.
    while(__p -> __right_ != nullptr)
    {
        __p = __p->__right_;
    }
    return __p;
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
    // 만약 __x가 nullptr이라면,
    if(__x != nullptr)
        std::cout << __x;
}


// key값을 갖는 노드를 만들어 리턴한다.
template <class _Tp>
Node<_Tp>* __getNodeBST(const _Tp& key) {
    Node<_Tp>* newNodePtr = new Node<_Tp>(key);

    return newNodePtr;
}

// 해당 key값의 parent를 리턴한다.
template <class _NodePtr, class _Tp>
_NodePtr __getParent(_NodePtr &__root, const _Tp& key) {
    _NodePtr __p = __root;
    _NodePtr __q = nullptr;

    while(__p)
    {
        if(__p->__key_ == key)
            return __q;
        // 부모로 저장.
        __q = __p;
        if(__p->__key_ < key)
        {
            __p = __p->__right_;
        }
        else{
            __p = __p->__left_;
        }
    }

    return nullptr;
}

// __root를 참조타입으로 받도록 변경함.
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr &__root, const _Tp& key) {
    _NodePtr __q = nullptr;
    _NodePtr __p = __root;
    while(__p)
    {
        // 만약 이미 key가 있다면 false 리턴.
        if(key == __p -> __key_)
            return std::pair<_NodePtr, bool>(__p, false);
        // __p를 변경전 __q를 저장. (while 문에서 나갔을 때 nullptr이 아닌 이전 node(parent)를 저장하기 위함.)
        __q = __p;
        // 만약 key가 더 작다면 왼쪽으로. 크다면 오른쪽으로.
        if(key < __p -> __key_)
        {
            __p = __p -> __left_;
        }
        else
        {
            __p = __p -> __right_;
        }
    }
    // 새로운 key를 가지는 Node를 만듦.
    _NodePtr newNodePtr = __getNodeBST(key);

    // 만약 root가 비어있었으면 __root는 newNode. newNode를 __q의 왼쪽에 삽입할지 오른쪽에 삽입할지 결정.
    if(__root == nullptr)
    {
        __root = newNodePtr;
    }
    else
    {
        if(key < __q->__key_)
        {
            __q -> __left_ = newNodePtr;
        }
        else
        {
            __q -> __right_ = newNodePtr;
        }
    }
    
    return std::pair<_NodePtr, bool>(newNodePtr, true);
}

// 삭제 구현.
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr __p = __root;
    _NodePtr __q = nullptr;

    // while문이 종료되었을 때는 찾으려는 key가 있을경우, p는 key가 있는 노드의 포인터를,
    // q는 p의 부모노드를 가르키게 된다.
    // 찾으려는 key가 없다면 __p에는 nullptr이 들어가게 된다.
    while(__p != nullptr && key != __p -> __key_)
    {
        __q = __p;

        if(key < __p -> __key_)
            __p = __p->__left_;
        else
            __p = __p->__right_;
    }

    // 만약 찾았는데 없다면 nullptr 반환.
    if(__p == nullptr)
        return nullptr;

    // 만약 __p의 차수가 2라면.
    if(__p -> __left_ != nullptr && __p -> __right_ != nullptr)
    {
        _NodePtr tempNode = __p;
        
        // 만약 right의 height가 더 크거나, 둘의 크기는 같은데 right의 사이즈가 더 크다면 right에 있는 최소값(가장 left)를 찾는다.
        if(__height(__p->__left_) < __height(__p->__right_) || ( __height(__p->__left_) == __height(__p->__right_) && __size(__p -> __left_) < __size(__p -> __right_) ))
        {            
            __p = __tree_min(__p->__right_);
        }
        // 반대로 left에서 가장 큰 값(가장 오른쪽)을 찾는다.
        else
        {
            __p = __tree_max(__p->__left_);
        }

        // __q를 __p의 parent로 설정한다.
        __q = __getParent(__root, __p->__key_);

        // 교체하려는 노드의 key값을 바꾼다.
        tempNode->__key_ = __p->__key_;

        
    }
    // 이제 차수가 2인 것도 교체할 값을 찾았기에 차수 1 또는 0으로 계산할 수 있다.
    // left쪽에 하나 있을때.
    if(__p->__left_ != nullptr)
    {   
        // 만약 p가 rootnode라면, root를 left로 교체한다.
        if(__q == nullptr)
            __root = __root->__left_;
        // 만약 p가 q의 왼쪽에 있다면 q의 left, 즉 p를 p의 left로 교체한다.
        else if(__q->__left_ == __p)
            __q->__left_ = __p->__left_;
        else
            __q->__right_ = __p->__left_;
    }
    // right쪽에 하나 있을때
    else if(__p->__right_ != nullptr)
    {
        // 만약 p가 rootnode라면, root를 right로 교체한다.
        if(__q == nullptr)
            __root = __root->__right_;
        // 만약 p가 q의 왼쪽에 있다면 q의 left, 즉 p를 p의 right로 교체한다.
        else if(__q->__left_ == __p)
            __q->__left_ = __p->__right_;
        else
            __q->__right_ = __p->__right_;
    }
    // 차수가 0일 때
    else
    {
        // p가 루트 노드라면 루트를 null로 바꾼다.
        if(__q == nullptr)
            __root = nullptr;
        // p가 있는 곳을 nullptr로 바꾼다.
        else if(__q->__left_ == __p)
            __q->__left_ = nullptr;
        else
        {
            __q->__right_ = nullptr;
        }
    }

    // 추후에 된다면 height를 멤버변수로 만들고 초기화 구현.
    return __p;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
    //만약 nullptr이면 그냥 return.
    if(__x == nullptr) return;

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
	
	public: // Capacity
		size_type height() const {
			return __height(__root_);
		}

		size_type size() const {
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
			return __insertBST(__root_, key);
		}

		const_pointer erase(const key_type& key) {
			pointer __r = __eraseBST(__root_, key);
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
		tree.inorder();
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

	return (0);
}