#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node
{
public: // Member types
    typedef _Tp					__key_type;
    typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
    __key_type			__key_;
    __node_pointer_type	__left_;
    __node_pointer_type	__right_;

public: // Constructor
    Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
    Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

    // 연산자 오버로딩 이해에 어려움이 있어 사용하지 않고, 직접 inorder를 호출하는 방식을 사용함
    // friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np)
    // {}
};

template <class _NodePtr>
unsigned __height(_NodePtr __x)
{
    // 각 서브트리에 대해 순환호출을 하고 그 반환값 중 최댓값을 구한 게 현재 내 노드를 제외한 높이니까 +1을 해서 반환

    unsigned h = 0;
    if (__x != nullptr)
    {
        // max를 사용할 수 없으니 각각 구해서 삼항 연산자로 두 값 중 최댓값을 반환하도록 함
        unsigned left = __height(__x->__left_);
        unsigned right = __height(__x->__right_);
        h = 1 + (left > right ? left : right);
    }

    return h;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x)
{
    // 각 서브트리에 대해 순환 호출을 하고 그 반환값을 더한 게 현재 내 노드를 제외한 나머지 노드의 개수이니까 +1을 해서 반환

    unsigned cnt = 0;
    if (__x != nullptr)
        cnt = 1 + __size(__x->__left_) + __size(__x->__right_);

    return cnt;
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x)
{
    // BST에서는 왼쪽 가장 아래에 위치한 값이 최솟값임

    if (__x == nullptr)
        return nullptr;                         // __x가 nullptr이면 빈 트리
    else if (__x->__left_ == nullptr)
        return __x;                             // 더이상 왼쪽 서브트리가 없으면 __x가 최솟값
    else
        return __tree_min(__x->__left_);        // 왼쪽 서브트리가 있으면 순환 호출

}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x)
{
    // BST에서는 오른쪽 가장 아래에 위치한 값이 최댓값임

    if (__x == nullptr)
        return nullptr;                         // __x가 nullptr이면 빈 트리
    else if (__x->__right_ == nullptr)
        return __x;                             // 더이상 오른쪽 서브트리가 없으면 __x가 최댓값
    else
        return __tree_max(__x->__right_);       // 오른쪽 서브트리가 있으면 순환 호출
}

template <class _NodePtr>
void __inorder(_NodePtr __x)
{
    // 중위 순회는  왼쪽 서브트리 방문 -> 루트 노드 방문 -> 오른쪽 서브트리 방문
    // nullptr이면 출력하지 않음
    if (__x == nullptr)
        return;

    // 출력 형식은 <{left subtree} {key} {right subtree}>이므로 순서에 맞게 작성
    std::cout << '<';
    __inorder(__x->__left_);
    std::cout << ' ' << (__x->__key_) << ' ';
    __inorder(__x->__right_);
    std::cout << '>';
}

template <class _Tp>
Node<_Tp>* getNodeBST(const _Tp& key)
{
    // 새 노드 생성 -> Node class의 생성자를 이용
    return (new Node<_Tp>(key));
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key)
{
    _NodePtr q = nullptr;
    _NodePtr p = __root;

    // p가 삽입할 위치, q가 그 부모 노드를 가르키도록 반복
    while (p != nullptr)
    {
        // 이미 같은 키 값이 존재하는 경우 실패(false) 반환
        if (key == p->__key_)
            return std::pair<_NodePtr, bool>(p, false);

        q = p;
        if (key < p->__key_)
            p = p->__left_;
        else
            p = p->__right_;
    }

    _NodePtr newNode = getNodeBST<_Tp>(key);       // getNodeBST()를 이용하여 key 값을 가지는 새 노드 생성

    if (__root == nullptr)
        __root = newNode;           // __root가 nullptr이면 그 자리에 newNode 삽입
    else if (key < q->__key_)
        q->__left_ = newNode;       // q의 키 값보다 작으면 왼쪽에 삽입
    else
        q->__right_ = newNode;      // q의 기 값보다 크면 오른쪽에 삽입

    // 삽입된 노드의 위치와 삽입 여부 반환
    return std::pair<_NodePtr, bool>(newNode, true);
}

// __eraseBST에서 스택을 사용하고 있지 않기 때문에 차수가 2인 경우 p의 부모를 찾는 데에 사용하기 위해 제작 (강의 슬라이드 참고)
template <class _NodePtr, class _Tp>
_NodePtr __searchParent(_NodePtr& __root, const _Tp& key)
{
    _NodePtr p = __root;
    _NodePtr q = nullptr;

    while (p != nullptr)
    {
        if (key == p->__key_)       // p의 key가 찾을 key와 같은 경우 그때의 q가 부모 노드
            return q;

        // 같지 않은 경우 q에는 p를, p에는 키 값이 큰지 작은지에 따라 p의 자식 노드를 저장해 한 단계 아래로 내려가도록 함
        q = p;
        if (key < p->__key_)
            p = p->__left_;
        else
            p = p->__right_;
    }

    return nullptr;
}

/*
* root에만 key가 있고, 그 키를 삭제하는 경우 root의 부모 노드인 nullptr을 반환하는 것과
* 키가 존재하지 않는 경우에 nullptr을 반환하는 것을 구분할 수 없기 때문에
* __insertBST처럼 성공 실패 여부를 반환하도록 기존의 반환형인 _NodePtr에서 pair<_NodePtr, bool>로 수정
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __eraseBST(_NodePtr& __root, const _Tp& key)
{
    _NodePtr p = __root;        // 삭제할 노드를 저장할 변수
    _NodePtr q = nullptr;       // 삭제한 노드의 부모 노드를 저장할 변수
    // pseudo code에서는 최대 최소를 구하는 과정과 Node에 높이 변수가 있어 추후 계산에 사용하려고 스택이 사용됐지만
    // min max 값을 찾는 함수를 사용해야 하고, 높이 변수 역시 멤버 변수로 사용하고 있찌 않기 때문에 pseudo code와 달리 스택이 필요하지 않다고 생각함

    // p는 삭제할 노드, q는 삭제할 노드의 부모를 가르키도록 반복
    while (p != nullptr && key != p->__key_)
    {
        q = p;
        if (key < p->__key_)
            p = p->__left_;
        else
            p = p->__right_;
    }

    // 키가 존재하지 않는 경우, nullptr와 실패(false) 반환
    if (p == nullptr)
        return std::pair<_NodePtr, bool>(nullptr, false);

    // 삭제할 노드의 차수가 2인 경우
    if (p->__left_ != nullptr && p->__right_ != nullptr)
    {
        _NodePtr tmp = p;
        unsigned lefth = __height(p->__left_);      // 트리의 왼쪽 서브트리의 높이
        unsigned righth = __height(p->__right_);    // 트리의 오른쪽 서브트리의 높이
        unsigned lefts = __size(p->__left_);        // 왼쪽 서브트리의 노드 개수
        unsigned rights = __size(p->__right_);      // 오른쪽 서브트리의 노드 개수

        if (lefth < righth || (lefth == righth && lefts < rights))      // 왼쪽 서브트리의 높이가 더 작거나, 높이가 같은데 왼쪽의 노드의 개수가 더 적다면
            p = __tree_min(p->__right_);                                // 오른쪽 서브트리에서 최솟값을 가지는 노드를 p에 저장
        else                                                            // 오른쪽 서브트리의 높이가 더 작거나, 높이가 같은데 오른쪽의 노드의 개수가 더 적다면
            p = __tree_max(p->__left_);                                 // 왼쪽 서브트리에서 최댓값을 가지는 노드를 p에 저장

        q = __searchParent(__root, p->__key_);      // p의 부모~를 q에 저장함으로써 p를 삭제할 노드로 바꾸게 됨 -> 차수가 0 또는 1인 경우로 바뀜
        tmp->__key_ = p->__key_;                    // 기존의 삭제할 노드의 키 값을 p의 값으로 대체
    }

    // 삭제할 노드의 차수가 1(왼쪽에 자식 존재)인 경우
    if (p->__left_ != nullptr)
    {
        if (q == nullptr)                   // 삭제할 노드가 root인 경우
            __root = p->__left_;            // 삭제할 노드의 왼쪽이 root가 됨
        else if (q->__left_ == p)           // 삭제할 노드가 부모 노드의 왼쪽인 경우
            q->__left_ = p->__left_;        // 삭제할 노드의 왼쪽이 부모 노드의 왼쪽이 됨
        else                                // 삭제할 노드가 부모 노드의 오른쪽인 경우
            q->__right_ = p->__left_;       // 삭제할 노드의 왼쪽이 부모 노드의 오른쪽이 됨

    }

    // 삭제할 노드의 차수가 1(오른쪽에 자식 존재)인 경우
    else if (p->__right_ != nullptr)
    {
        if (q == nullptr)                   // 삭제할 노드가 root인 경우
            __root = p->__right_;           // 삭제할 노드의 오른쪽이 root가 됨
        else if (q->__left_ == p)           // 삭제할 노드가 부모 노드의 왼쪽인 경우
            q->__left_ = p->__right_;       // 삭제할 노드의 오른쪽이 부모 노드의 왼쪽이 됨
        else                                // 삭제할 노드가 오른쪽인 경우
            q->__right_ = p->__right_;      // 삭제할 노드의 오른쪽이 부모 노드의 오른쪽이 됨
    }

    // 삭제할 노드의 차수가 0인 경우 (리프)
    else
    {
        if (q == nullptr)               // 삭제할 노드가 root인 경우
            __root = nullptr;           // root가 nullptr이 됨
        else if (q->__left_ == p)       // 삭제할 노드가 부모 노드의 왼쪽인 경우
            q->__left_ = nullptr;       // 부모 노드의 왼쪽이 nullptr이 됨
        else                            // 삭제할 노드가 부모 노드의 오른쪽인 경우
            q->__right_ = nullptr;      // 부모 노드의 오른쪽이 nullptr이 됨
    }

    // 삭제할 노드 p의 메모리를 해제하고 삭제한 노드의 부모 노드인 q와 성공(true) 반환
    delete p;
    return std::pair<_NodePtr, bool>(q, true);
}

template <class _NodePtr>
void __clear(_NodePtr& __x)
{
    // 전체 트리의 모든 메모리 해제

    if (__x == nullptr)         // nullptr이면 이미 존재하지 않음
        return;

    __clear(__x->__left_);      // 왼쪽 서브트리에 대해 재귀
    __clear(__x->__right_);     // 오른쪽 서브트리에 대해 재귀

    delete __x;     // 메모리 해제
}

template <class _Tp>
class BST
{
public: // Member types
    typedef _Tp						key_type;
    typedef std::size_t				size_type;
    typedef Node<key_type>* pointer;
    typedef const Node<key_type>* const_pointer;

private: // Member variables
    pointer	__root_;

public: // Constructor
    BST() : __root_(nullptr) {}

public: // Capacity
    size_type height() const
    {
        return __height(__root_);
    }

    size_type size() const
    {
        return __size(__root_);
    }

public: // Lookup
    void inorder() const
    {
        __inorder(__root_);
        std::cout << std::endl;
    }

public: // Modifier
    std::pair<const_pointer, bool> insert(const key_type& key)
    {
        return __insertBST(__root_, key);
    }

    std::pair<const_pointer, bool> erase(const key_type& key)
    {
        return __eraseBST(__root_, key);
    }

    void clear()
    {
        __clear(__root_);
    }

    // 연산자 오버로딩 사용 X
    // friend std::ostream& operator<<(std::ostream& os, const BST& tree)
    // {
    //     os << tree.__root_;
    //     return os;
    // }

    // erase에서 사용하는 함수인데 멤버 함수로 만드는 게 일관성(?) 있는 것 같아서 멤버 함수로 만듦
    pointer searchParent(const key_type& key)
    {
        return __searchParent(__root_, key);
    }
};

// main에 인자를 따로 받지 않기 때문에 argc, argv 지우고 void로 처리함 (컴파일 플래그에 걸림)
int main(void)
{
    BST<int>	tree;
    char		command;
    int			key;

    while (std::cin >> command >> key)
    {
        switch ((int)command)
        {
        case (int)'i':
            if (tree.insert(key).second == false)
            {
                std::cerr << "i " << key << ": The key already exists" << std::endl;
                continue;
            }
            break;
        case (int)'d':
            if (tree.erase(key).second == false)
            {
                std::cerr << "d " << key << ": The key does not exist" << std::endl;
                continue;
            }
            break;
        default:
            std::cerr << "Invalid command: " << command << std::endl;
            return (1);
            break;
        }
        // std::cout << tree << std::endl;
        // 연산자 오버로딩을 사용하지 않아 주석 처리 후 inorder 직접 호출
        tree.inorder();
    }

    // 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
    tree.clear();

    return (0);
}