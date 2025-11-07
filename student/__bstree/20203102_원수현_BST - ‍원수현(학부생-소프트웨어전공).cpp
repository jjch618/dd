/* 본 코드는 BST-skeleton.cpp 파일을 참고하여 작성되었습니다. */

#include <iostream>
using namespace std;

// BST에 사용되는 class
template <class _Tp> class Node;
template <class _Tp> class BST;

// BST를 구성하는 node 구조 동작에 사용되는 function
template <class _NodePtr> unsigned __height(_NodePtr __x);
template <class _NodePtr> unsigned __size(_NodePtr __x);
template <class _NodePtr> _NodePtr __tree_min(_NodePtr __x);
template <class _NodePtr> _NodePtr __tree_max(_NodePtr __x);

// BST 검색(inorder) function
template <class _NodePtr> void __inorder(_NodePtr __x);

// 신규 node 인스턴스 생성 function
template <class _Tp> Node<_Tp>* __getNodeBST(const _Tp& key);

// key 값을 받아서 해당 node와 parent node 주소를 반환하는 function
template <class _NodePtr, class _Tp> _NodePtr __searchBST(_NodePtr __x, const _Tp& key);
template <class _NodePtr, class _Tp> _NodePtr __searchParent(_NodePtr __x, const _Tp& key);

// BST에 node를 삽입하고 삭제하는 function
template <class _NodePtr, class _Tp> pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key);
template <class _NodePtr, class _Tp> _NodePtr __eraseBST(_NodePtr& __root, const _Tp& key);

// 최종적으로 BST의 메모리 할당 해제용 function
template <class _NodePtr> void __clear(_NodePtr& __x);

// BST에 각각 들어가는 node class. height, size를 추가했다.
template <class _Tp>
class Node {
    public: // Member types - class에서 사용할 type 치환
        typedef _Tp                 __key_type;             // node에 입력된 key값의 type
        typedef Node<__key_type>*   __node_pointer_type;    // key type을 기반으로 만들어진 node 인스턴스 포인터

    public: // Member variables - 변수 정의, skeleton code에서 height와 size를 추가함
        __key_type              __key_;                     // node에 입력된 key값
        __node_pointer_type     __left_;                    // 현재 node의 왼쪽 자식 주소 (포인터)
        __node_pointer_type     __right_;                   // 현재 node의 오른쪽 자식 주소 (포인터)

    public: // Constructor - node class 생성자. 변수를 초기화하고, key value가 입력된 경우와 아닌 경우를 나누어 둔다.
        Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
        Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr) {}

    friend ostream& operator<<(ostream& os, const __node_pointer_type& np) {
        // << operator를 사용하면 inorder 함수가 작동한다. 출력은 inorder 함수에서만 한다.
        __inorder(np);
        return os;
    }
};

// BST class. root node의 주소를 기억한다.
template <class _Tp>
class BST {
    public: // Member types - class에서 사용할 type 치환
        typedef _Tp                     key_type;           // tree를 구성하는 node type
        typedef size_t                  size_type;          // height(), size() 반환에 사용되는 size
        typedef Node<key_type>*         pointer;            // 특정 node를 가리키는 인스턴스 포인터
        typedef const Node<key_type>*   const_pointer;      // const로 선언되는 포인터

    private: // Member variables
        pointer __root_;    // tree의 root node 정의
    
    public: // Constructor - BST 생성자. 최초에 생성될 때에는 root node가 존재하지 않는다.
        BST(): __root_(nullptr) {}

    // 아래에 포함되는 function은 모두 외부에 구현한 function을 통해 동작한다.
    public: // Capacity
        size_type height() const {  // 트리의 높이를 반환한다. 인스턴스에서 호출되진 않았다.
            return __height(__root_);
        }

        size_type size() const {    // 트리에 포함된 node 개수를 반환한다. 인스턴스에서 호출되진 않았다.
            return __size(__root_);
        }

    public: // Lookup
        void inorder() const {      // inorder 방식으로 탐색하며 출력한다. 인스턴스에서 호출되진 않았다.
            __inorder(__root_);
            cout << endl;
        }

    public: // Modifier
        pair<const_pointer, bool> insert(const key_type& key) { // 트리에 key를 가지는 node를 삽입한다.
            // 실제로 key가 insert 되었는지 여부를 return한다.
            return __insertBST(__root_, key);
        }

        const_pointer erase(const key_type& key) {  // 트리에서 key 값을 가지는 node를 삭제한다.
            pointer __r = __eraseBST(__root_, key);
            // 해당하는 key 값을 가진 node가 return되므로, delete를 통해 메모리 누수를 방지한다.
            delete __r;
            return __r;
        }

        void clear() {  // 트리에 있는 모든 node에 할당된 메모리를 해제한다.
            __clear(__root_);
        }
    
    friend ostream& operator<<(ostream& os, const BST& tree) {
        os << tree.__root_;
        return os;
    }
};

// height(T) 함수. node를 기준으로 한 tree에서 height를 반환한다. recursion으로 구현하였다.
template <class _NodePtr>
unsigned int __height(_NodePtr __x) {
    if(__x == nullptr) {    // 입력된 node가 존재하지 않는다. 0을 반환한다.
        return 0;
    }

    // left, right node의 height를 각각 확인한다. 
    size_t left = __height(__x->__left_);
    size_t right = __height(__x->__right_);
    
    // node가 존재하므로 height에 +1을 하여, 더 높은 height를 가진 자식을 구분하여 반환한다.
    if(left > right) {
        return left+1;
    } else {
        return right+1;
    }
}

// size(T) 함수. node 기준으로 한 tree에서 node의 개수를 반환한다. recursion으로 구현하였다.
template <class _NodePtr>
unsigned int __size(_NodePtr __x) {
    if(__x == nullptr) {    // 입력된 node가 존재하지 않는다. 0을 반환한다.
        return 0;
    }
    
    // left, right node가 존재하는지 확인하지 않고 더한다. 자식 노드가 존재하지 않는다면 0이 반환될 것이다.
    return 1+__size(__x->__left_)+__size(__x->__right_);
}

// minNode(T) 함수. node 기준으로 하위의 tree에서 가장 작은 key 값을 가지는 node를 반환한다.
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    _NodePtr p = __x;

    // tree의 가장 왼쪽의 node가 가장 작은 값을 가지고 있다. 왼쪽 자식이 없을 때까지 반복한다.
    while(p->__left_ != nullptr) {
        p = p->__left_;
    }
    
    return p;
}

// maxNode(T) 함수. node 기준으로 하위의 tree에서 가장 큰 key 값을 가지는 node를 반환한다.
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
    _NodePtr p = __x;

    // tree의 가장 오른쪽의 node가 가장 큰 값을 가지고 있다. 오른쪽 자식이 없을 때까지 반복한다.
    while(p->__right_ != nullptr) {
        p = p->__right_;
    }

    return p;
}

// getNodeBST(key) 함수. key 값을 가지는 새로운 node를 작성하여 반환한다.
template <class _Tp>
Node<_Tp>* __getNodeBST(const _Tp& key) {
    return new Node<_Tp>(key);
}

// searchBST(T, key)와 searchParent(T, key)의 역할을 하는 함수. key 값을 가지는 node와 부모 node의 주소를 반환한다.
template <class _NodePtr, class _Tp>
_NodePtr __searchBST(_NodePtr __x, const _Tp& key) {
    _NodePtr p = __x;

    // key 값과 동일한 node를 탐색한다. p가 변경될 경우에는 현재의 p가 새로운 q가 된다.
    while(p != nullptr) {
        if(key == p->__key_) {
            break;              // key 값을 가지는 node를 발견한 위치에서 멈춘다.
        }

        if(key < p->__key_) {      // key 값이 현재 node 값보다 작으면 왼쪽으로 이동한다.
            p = p->__left_;
        } else {                // key 값이 현재 node 값보다 크면 오른쪽으로 이동한다.
            p = p->__right_;
        }
    }

    return p;  // 해당 node가 발견되지 않은 경우에는 p에 nullptr이 입력되어 반환한다.
}

// searchBST(T, key)와 searchParent(T, key)의 역할을 하는 함수. key 값을 가지는 node와 부모 node의 주소를 반환한다.
template <class _NodePtr, class _Tp>
_NodePtr __searchParent(_NodePtr __x, const _Tp& key) {
    _NodePtr p = __x;
    _NodePtr q = nullptr;

    // key 값과 동일한 node를 탐색한다. p가 변경될 경우에는 현재의 p가 새로운 q가 된다.
    while(p != nullptr) {
        if(key == p->__key_) {
            break;              // key 값을 가지는 node를 발견한 위치에서 멈춘다.
        }

        if(key < p->__key_) {      // key 값이 현재 node 값보다 작으면 왼쪽으로 이동한다.
            q = p;
            p = p->__left_;
        } else {                // key 값이 현재 node 값보다 크면 오른쪽으로 이동한다.
            q = p;
            p = p->__right_;
        }
    }
    return q;  // 해당 node가 발견되지 않은 경우에는 p에 nullptr이 입력되어 반환한다.
}

// insertBST(T, key) 함수. node 삽입을 구현한다. key 값이 이미 tree에 있는지 또한 확인한다. 이 여부를 반환하기 위해 return을 pair로 작성한다.
template <class _NodePtr, class _Tp>
pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {    // root가 최초에 null일 때, __root_의 값을 바꾸기 위해 참조 타입으로 작성하였다.
    _NodePtr p = __searchBST(__root, key);      // 현재 node
    _NodePtr q = __searchParent(__root, key);   // 부모 node

    // tree에 key 값을 가진 node가 존재할 때의 처리. root인 상황을 방지한다.
    if(p != nullptr && key == p->__key_) {
        return pair<_NodePtr, bool>(p, false);  // node 주소와 함께 false를 반환한다.
    }

    // node를 삽입할 위치를 찾는 과정에서, 이미 node가 존재하는 경우를 처리했다.
    // 따라서 새로운 node를 삽입하는 경우의 if-else 문으로 작성한다.
    _NodePtr newNode = __getNodeBST(key);   // key 값을 전달하여 새로운 node를 만들어 주소를 반환받는다.
    if(newNode->__key_ != key) {    // key 값이 제대로 전달되지 않은 경우를 방지하기 위한 if문
        newNode->__key_ = key;      // left/right 서브트리의 처리는 node를 만들 때 미리 처리되었다.
    }

    if(__root == nullptr) {         // root node가 없을 때에는 최초에 입력된 node가 root가 된다.
        __root = newNode;
    } else if(key < q->__key_) {    // key 값이 부모(q)의 값보다 작은 경우
        q->__left_ = newNode;       // 부모의 왼쪽 자식으로 node를 삽입한다.
    } else {                        // key 값이 부모(q)의 값보다 큰 경우
        q->__right_ = newNode;      // 부모의 오른쪽 자식으로 node를 삽입한다.
    }

    return pair<_NodePtr, bool>(newNode, true);     // tree에 key 값을 가진 node가 정상적으로 삽입되었으므로 new node의 주소와 함께 true를 반환한다.
}

// eraseBST(T, key) 함수. node 삭제를 구현한다. key 값을 가지는 node가 tree에 없는 경우도 처리한다. tree에 없으면 nullptr이 반환된다.
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = __searchBST(__root, key);      // 현재 node
    _NodePtr q = __searchParent(__root, key);   // 부모 node

    // 삭제될 주소 연결을 먼저 처리하고, 값을 변경한다.
    // tree에 key 값과 동일한 node가 존재하지 않으면 현재 node가 nullptr이 된다.
    // 처음 들어온 root node가 nullptr이어도 동일하게 동작한다.
    if(p == nullptr) {
        return nullptr;     // 대상을 찾지 못한 경우이므로 nullptr을 반환한다.
    }

    // degree = 2. 두 서브트리 중에서 height가 높고 size가 큰 방향에서 node의 key 값을 가져온다.
    if(p->__left_ != nullptr && p->__right_ != nullptr) {
        _NodePtr r = p;    

        // 양쪽 서브트리의 높이를 비교하여 저장한다.
        int heightDiff = (int)__height(p->__left_) - (int)__height(p->__right_);

        // 새로 바뀔 key 값의 node 주소를 찾는다. 이후에 p를 반환할 예정이므로, 주소는 p에 저장한다.
        // degree가 2일 때는 서브트리에서도 재귀적으로 함수를 호출해야 한다.
        if(heightDiff > 0) {                // left 서브트리의 height가 높을 때
            p = __tree_max(p->__left_);     // left 서브트리에서 가장 큰 값을 가지는 node를 저장한다.
        } else if (heightDiff < 0) {        // right 서브트리의 height가 높을 때
            p = __tree_min(p->__right_);    // right 서브트리에서 가장 작은 값을 가지는 node를 저장한다.
        } else {                            // 양쪽 서브트리의 높이가 동일할 때는 node 개수를 비교한다. 동일하면 left를 우선한다.
            if(__size(p->__left_) < __size(p->__right_)) {
                p = __tree_min(p->__right_);            // 가져온 node를 right 서브트리에서 삭제한다.
            } else {
                p = __tree_max(p->__left_);             // 가져온 node를 left 서브트리에서 삭제한다.
            }
        }
        q = __searchParent(__root, p->__key_);          // 찾은 p의 부모 노드를 찾는다.
        r->__key_ = p->__key_;  // 삭제될 key 값을 가진 node가 r이므로, 대체할 node인 p의 key 값으로 변경한다.
    }

    // 위 if문에서 p와 q 값이 변경되었다면, 변경된 채로 동작한다.
    // degree = 2의 if문을 거친 경우엔 서브트리의 가장 크거나/작은 key 값을 가지는 node로 p가 변경되었기 때문에, degree는 0 또는 1이다.
    // 삭제할 node의 주소에 연관된 서브트리 연결을 바꿔준다.
    if(p->__left_ != nullptr) {         // 삭제할 node의 left 서브트리가 존재하는 경우 (degree = 1)
        if(q == nullptr) {                  // root node의 삭제인 경우
            __root = __root->__left_;       // left 서브트리를 연결해준다.
        } else if(q->__left_ == p) {        // 부모 노드의 left 서브트리였을 경우
            q->__left_ = p->__left_;        // 삭제할 node의 left 서브트리를 부모 노드의 left로 연결한다.
        } else {                            // 부모 노드의 right 서브트리였을 경우
            q->__right_ = p->__left_;       // 삭제할 node의 left 서브트리를 부모 노드의 right로 연결한다.
        }
    } else if(p->__right_ != nullptr) { // 삭제할 node의 right 서브트리가 존재하는 경우 (degree = 1)
        if(q == nullptr) {                  // left와 동일한 절차로 연결을 다시 구성한다.
            __root = __root->__right_;
        } else if(q->__left_ == p) {
            q->__left_ = p->__right_;
        } else {
            q->__right_ = p->__right_;
        }
    } else {                            // 삭제할 node의 서브트리가 존재하지 않는 경우 (degree = 0)
        if(q == nullptr) {                  // 위와 동일한 절차이나, 새로운 연결 대신 nullptr로 연결된 노드를 모두 끊어준다.
            __root = nullptr;
        } else if(q->__left_ == p) {
            q->__left_ = nullptr;
        } else {
            q->__right_ = nullptr;
        }
    }

    // 돌려준 주소를 호출했던 function에서 삭제되므로 값을 반환하기만 한다.
    return p;
}

// inorder(T) 함수. 출력을 위한 순회 알고리즘이 구현되어 있다. cout을 통한 recursion 구조이다.
template <class _NodePtr>
void __inorder(_NodePtr __x) {
    if(__x != nullptr) {
        cout << '<';                        // node가 존재한다면 <을 우선적으로 출력
        if(__x->__left_ != nullptr) {       // left 서브트리 우선 출력 (recursion)
            cout << __x->__left_;
        }
        cout << ' ' << __x->__key_ << ' ';  // 값의 좌우에 공백을 두고 key 값 출력
        if(__x->__right_ != nullptr) {      // right 서브트리 우선 출력 (recursion)
            cout << __x->__right_;
        }
        cout << '>';                        // node의 끝을 >으로 마무리
    }
}

// clear(T) 함수. 메모리 누수를 방지하기 위해 동적으로 할당된 객체들을 프로그램 종료 시 해제하는 역할을 한다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
    // 함수에 입력된 node가 null이면 움직이지 않는다.
    if(__x != nullptr) {
        __clear(__x->__left_);  // recursion으로 모든 left 서브트리에 적용된다.
        __clear(__x->__right_); // recursion으로 모든 right 서브트리에 적용된다.
        delete __x;             // 위 동작을 마친 경우에 해당하는 node를 삭제한다.
    }
}

// 동작되는 메인 함수. 입력되는 argc, argv의 경우엔 input file의 입력으로 대체하여 생략했다.
int main(){
    BST<int>    tree;
    char        command;
    int         key;

    // 공백을 기준으로 하여, 순서대로 command와 key를 각각 입력한다.
    while(cin >> command >> key) {
        try {
            switch((int)command) {
            case(int)'i':   // insert case. 삽입 실패 시에 false를 반환한다.
                if(tree.insert(key).second == false) {
                    cerr << "i " << key << ": The key already exists" << endl;
                    continue;
                }
                break;
            case(int)'d':   // delete case. 삭제 실패 시에 nullptr을 반환한다.
                if(tree.erase(key) == nullptr) {
                    cerr << "d " << key << ": The key does not exist" << endl;
                    continue;
                }
                break;
            default:        // 잘못된 입력에 대한 반환.
                cerr << "Invalid command: " << command << endl << flush;
                break;
            }
            cout << tree << endl;
        } catch(const exception& e) {
            cout << "Exception: " << e.what() << endl;
        }
    }

    tree.clear();
    
    return(0);
}