

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


};



template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    // 트리가 비어있을경우 높이는 0
	if (__x == nullptr) {
        return 0;
	}
	// 재귀적으로 왼쪽과 오른쪽 서브트리의 높이 비교
	unsigned left_h = __height(__x->__left_);
	unsigned right_h = __height(__x->__right_);
	// 더 큰값  + 1 반환
	if (left_h > right_h) {
        return 1 + left_h;
	}
	else {
        return 1 + right_h;
	}
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    if (__x == nullptr) {
        return 0;
    }
    // 자신(1) + 왼쪽 서브트리의 크기 + 오른쪽 서브트리의 크기
    return 1 + __size(__x->__left_) + __size(__x->__right_);

}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x, std::stack<_NodePtr>& stack) {
	if (__x ==nullptr) {
        return nullptr;
	}
	// BST에서 가장 작은값은 항상 맨 왼쪽 노드이므로 맨 왼쪽 노드 탐색
	while (__x->__left_ != nullptr) {
            stack.push(__x);
            __x = __x->__left_;
	}
	return __x;
}


template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x, std::stack<_NodePtr>& stack) {
        if (__x == nullptr) {
            return nullptr;
        }
        // 마찬가지로 BST에서 가장 큰값은 항상 맨 오른쪽 노드이므로 맨 오른쪽 노드 탐색
        while (__x->__right_ != nullptr) {
            stack.push(__x);
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
    if (__x == nullptr) {
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
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = __root;
    _NodePtr q = nullptr;

    // q를 삽입하고자 하는 key의 부모가 될 노드까지 이동
    while (p != nullptr) {
            // 이미 같은 키 값이 존재하는 경우
            if (key == p->__key_) {
                return std::make_pair(p, false);
            }

            q = p;

            if (key < p->__key_) {
                p = p->__left_;
            }
            else {
                p = p->__right_;
            }
    }

    _NodePtr newNode =  new Node<_Tp>(key);
    // 트리가 비어있는경우
    if  (__root == nullptr) {
        __root = newNode;
    }
    // key값이 q보다 작으면 왼쪽에, 크면 오른쪽에 삽입
    else if (key < q->__key_) {
        q->__left_ = newNode;
    }
    else {
        q->__right_ = newNode;
    }

    return std::make_pair(newNode, true);
}


template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = __root;
    _NodePtr q = nullptr;
    std::stack<_NodePtr> stack;

    // 삭제할 노드 찾기
    while (p != nullptr && key != p->__key_ ) {
        q = p;

        if (key < p->__key_) {
            p = p->__left_;
        }
        else {
            p = p->__right_;
        }
    }

    if (p == nullptr) {
        // 삭제할 노드가 없는경우 nullptr 반환
        return nullptr;
    }

    // degree가 2인 경우 1이나 0으로 축소
    if (p->__left_ != nullptr && p->__right_ != nullptr) {
        _NodePtr tempNode = p;
        stack.push(p);

        // __height()와 __size()를 호출해서 오른쪽, 왼쪽 서브트리의 높이,사이즈 계산
        unsigned left_h = __height(p->__left_);
        unsigned right_h = __height(p->__right_);
        unsigned left_s = __size(p->__left_);
        unsigned right_s = __size(p->__right_);

        // 서브트리의 높이,사이즈를 활용해 successor, predessor중 선택
        if (left_h < right_h  || (left_h  == right_h && left_s < right_s)) {
                p = __tree_min(p->__right_, stack);
        }
        else {
                p = __tree_max(p->__left_, stack);
            }
        tempNode->__key_ = p->__key_;
        // stack의 top = 부모노드
        q = stack.top();
        }

    _NodePtr toDelete = p;

    // degree가 1이고, p의 왼쪽에 자식이 있는 경우
    if (p->__left_ != nullptr) {
        if (q == nullptr) {
            __root = p->__left_;
        }
        else if (q->__left_ == p) {
            q->__left_ = p->__left_;
        }
        else {
            q->__right_ = p->__left_;
        }
    }
    // degree가 1이고, p의 오른쪽에 자식이 있는 경우
    else if  (p->__right_ != nullptr) {
        if (q == nullptr) {
            __root = p->__right_;
        }
        else if (q->__left_ == p) {
            q->__left_ = p->__right_;
        }
        else {
            q->__right_ = p->__right_;
        }
    }
    // degree가 0인경우, 즉 자식이 없는경우
    else {
        if (q == nullptr) {
            __root = nullptr;
        }
        else if (q->__left_ == p) {
            q->__left_ = nullptr;
        }
        else {
            q->__right_ = nullptr;
        }
    }

    // stack 비우기
    while (!stack.empty()) {
        stack.pop();
    }

    return toDelete;
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
    if (__x == nullptr) return;
    __clear(__x->__left_);
    __clear(__x->__right_);
    delete __x;
    __x = nullptr;
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
			delete __r;
			return __r;
		}

		void clear() {
			__clear(__root_);
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
		// inorder함수로 트리 전체 구조 출력
		tree.inorder();
	}

	tree.clear();

	return (0);
}
