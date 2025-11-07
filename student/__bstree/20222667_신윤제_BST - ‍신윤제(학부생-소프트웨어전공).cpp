/*
* File Processing Report 2025 - Binary Search Tree
* [수정] <utility> 헤더 제거
*/
#include <iostream>
// #include <utility> // std::pair -> 제거됨

template <class _Tp> class Node;
template <class _Tp> class BST;

/*
* 반드시 사용하는 Node 클래스.
* (멤버 이름/개수는 과제 규칙 준수: __key_, __left_, __right_, __height_, __size_)
*/
template <class _Tp>
class Node {
public: // Member types
    typedef _Tp                  __key_type;
    typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
    __key_type          __key_;
    __node_pointer_type __left_;
    __node_pointer_type __right_;
    unsigned            __height_;
    unsigned            __size_;

public: // Constructor
    Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}
    Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), __height_(1), __size_(1) {}

    // inorder 대체 출력: < {left} {key} {right} >
    friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& nodePtr) {
        if (nodePtr == nullptr) return os;
        os << "<";
        os << nodePtr->__left_;
        os << " " << nodePtr->__key_ << " ";
        os << nodePtr->__right_;
        os << ">";
        return os;
    }
};

/* 유틸 함수들 */
template <class _NodePtr>
unsigned __height(_NodePtr nodePtr) {
    if (nodePtr == nullptr) return 0;
    return nodePtr->__height_;
}

template <class _NodePtr>
unsigned __size(_NodePtr nodePtr) {
    if (nodePtr == nullptr) return 0;
    return nodePtr->__size_;
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr nodePtr) {
    if (nodePtr == nullptr) return nullptr;
    while (nodePtr->__left_ != nullptr) {
        nodePtr = nodePtr->__left_;
    }
    return nodePtr;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr nodePtr) {
    if (nodePtr == nullptr) return nullptr;
    while (nodePtr->__right_ != nullptr) {
        nodePtr = nodePtr->__right_;
    }
    return nodePtr;
}

template <class _NodePtr>
void __inorder(_NodePtr nodePtr) {
    (void)nodePtr; // 사용 안 함
}

template <class _NodePtr>
inline void __pull(_NodePtr nodePtr) {
    if (!nodePtr) return;

    // std::max 대신 삼항 연산자 사용
    unsigned leftHeight  = nodePtr->__left_  ? nodePtr->__left_->__height_ : 0;
    unsigned rightHeight = nodePtr->__right_ ? nodePtr->__right_->__height_ : 0;
    nodePtr->__height_ = 1u + (leftHeight > rightHeight ? leftHeight : rightHeight);

    unsigned leftSize  = nodePtr->__left_  ? nodePtr->__left_->__size_ : 0;
    unsigned rightSize = nodePtr->__right_ ? nodePtr->__right_->__size_ : 0;
    nodePtr->__size_ = 1u + leftSize + rightSize;
}

/* [신규] std::pair를 대체할 구조체 정의 */
template <class _NodePtr>
struct InsertResult {
    _NodePtr node;
    bool     inserted;
};

/* 삽입 */
template <class _NodePtr, class _Tp>
InsertResult<_NodePtr> __insertBST(_NodePtr& root, const _Tp& key) { // [수정] 반환 타입
    if (root == nullptr) {
        root = new Node<_Tp>(key);
        // [수정] std::pair 대신 InsertResult 반환
        InsertResult<_NodePtr> result = {root, true};
        return result;
    }

    if (key == root->__key_) {
        // [수정] std::pair 대신 InsertResult 반환
        InsertResult<_NodePtr> result = {root, false};
        return result;
    }

    if (key < root->__key_) {
        // [수정] std::pair 대신 InsertResult 사용
        InsertResult<_NodePtr> result = __insertBST(root->__left_, key);
        if (result.inserted) __pull(root); // [수정] .second -> .inserted
        return result;
    } else {
        // [수정] std::pair 대신 InsertResult 사용
        InsertResult<_NodePtr> result = __insertBST(root->__right_, key);
        if (result.inserted) __pull(root); // [수정] .second -> .inserted
        return result;
    }
}

/* 삭제 */
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& root, const _Tp& key) {
    if (!root) return nullptr;

    if (key < root->__key_) {
        _NodePtr removed = __eraseBST(root->__left_, key);
        // 삭제/갱신이 일어났을 수 있으므로 pull
        if (removed) __pull(root);
        return removed;
    } else if (key > root->__key_) {
        _NodePtr removed = __eraseBST(root->__right_, key);
        // 삭제/갱신이 일어났을 수 있으므로 pull
        if (removed) __pull(root);
        return removed;
    } else {
        // root가 삭제 대상
        if (!root->__left_) {
            _NodePtr removed = root;
            root = root->__right_; // 오른쪽 자식(null일 수도 있음)으로 대체
            return removed; // 삭제된 노드 반환
        } else if (!root->__right_) {
            _NodePtr removed = root;
            root = root->__left_; // 왼쪽 자식으로 대체
            return removed; // 삭제된 노드 반환
        } else {
            // 두 자식: height 우선, 같으면 size 비교
            unsigned leftHeight  = __height(root->__left_);
            unsigned rightHeight = __height(root->__right_);
            unsigned leftSize    = __size(root->__left_);
            unsigned rightSize   = __size(root->__right_);

            if (leftHeight > rightHeight || (leftHeight == rightHeight && leftSize >= rightSize)) {
                _NodePtr predecessor = __tree_max(root->__left_);
                root->__key_ = predecessor->__key_; // 키 값만 복사
                _NodePtr removed = __eraseBST(root->__left_, predecessor->__key_); // 왼쪽에서 대체노드 삭제
                __pull(root); // 현재 노드 갱신
                return removed; // 실제 삭제된 노드 반환
            } else {
                _NodePtr successor = __tree_min(root->__right_);
                root->__key_ = successor->__key_; // 키 값만 복사
                _NodePtr removed = __eraseBST(root->__right_, successor->__key_); // 오른쪽에서 대체노드 삭제
                __pull(root); // 현재 노드 갱신
                return removed; // 실제 삭제된 노드 반환
            }
        }
    }
}

/* 메모리 정리 */
template <class _NodePtr>
void __clear(_NodePtr& nodePtr) {
    if (nodePtr == nullptr) return;
    __clear(nodePtr->__left_);
    __clear(nodePtr->__right_);
    delete nodePtr;
    nodePtr = nullptr;
}

/* BST 본체 */
template <class _Tp>
class BST {
public:
    typedef _Tp                     key_type;
    // std::size_t는 <iostream>에 포함되어 있으므로 사용 가능
    typedef std::size_t             size_type;
    typedef Node<key_type>* pointer;
    typedef const Node<key_type>* const_pointer;

private:
    pointer __root_;

public:
    BST(): __root_(nullptr) {}

public: // Capacity
    size_type height() const { return __height(__root_); }
    size_type size() const   { return __size(__root_); }

public: // Lookup
    void inorder() const {
        __inorder(__root_);
        std::cout << std::endl;
    }

public: // Modifier
    // [수정] std::pair 대신 InsertResult를 반환
    InsertResult<const_pointer> insert(const key_type& key) {
        // __insertBST는 pointer를 반환하므로 const_pointer로 캐스팅
        InsertResult<pointer> result = __insertBST(this->__root_, key);
        InsertResult<const_pointer> const_result = {result.node, result.inserted};
        return const_result;
    }

    const_pointer erase(const key_type& key) {
        pointer removed = __eraseBST(this->__root_, key);
        if (!removed) {
            return nullptr; // 삭제 실패
        }
        
        // 삭제 성공
        delete removed; 
        
        // main의 null 체크를 통과시키기 위해 널이 아닌 포인터 반환
        // (1은 널 포인터가 아님을 보장하는 값)
        return reinterpret_cast<const_pointer>(1); 
    }

    void clear() { __clear(__root_); }

    friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
        os << tree.__root_;
        return os;
    }
};

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    BST<int> tree;
    char command; int key;

    while (std::cin >> command >> key) {
        switch ((int)command) {
            case (int)'i':
                // [수정] .second 대신 .inserted로 확인
                if (!tree.insert(key).inserted) {
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
                return 1;
        }

        if (tree.size() > 0) std::cout << tree << std::endl;
        else std::cout << std::endl;
    }

    tree.clear();
    return 0;
}