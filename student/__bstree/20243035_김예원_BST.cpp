// 101 line return이 없어서 심각한 버그 발생

//#define _CRT_SECURE_NO_WARNINGS //freopen으로 .txt파일 참조(현재 주석처리)
// 
// 파일명: 20243035_김예원_BST.cpp

#include <iostream>
#include<algorithm> //max함수를 쓰기 위해서 가져온 라이브러리
using namespace std;

template<class _Tp> class Node;
template<class _Tp> class BST; 

template <class _Tp>
class Node {
    public:
        typedef _Tp __key_type; 
        typedef Node<__key_type>* __node_pointer_type; 

    public:
        __key_type __key_; //현재 노드에 저장된 키값
        __node_pointer_type __left_;// 왼쪽 자식 노드 포인터
        __node_pointer_type __right_; // 오른쪽 자식 노드 포인터

    public:
        Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {} //기본 생성자로 왼/오 자식포인터는 nullptr로 초기화
        Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {} //생성자로 현재 key값으로 초기화

        friend ostream& operator<<(ostream& os, const __node_pointer_type& np) {//출력연산자 operator<<
            if (!np) return os;

            os << '<';
            bool isleft = (np->__left_ != nullptr) ? true : false; //왼쪽자식과
            bool isright = (np->__right_ != nullptr) ? true : false; //오른쪽자식이 nullptr인지 아닌지 확인하는 bool 변수
            
            if (isleft) os << np->__left_;

            os << ' '; //키 앞
            os << np->__key_;
            os << ' '; //키 뒤에 공백(0x20)

            if (isright)  os << np->__right_;
            os << '>';
            return os;
        }

};

template<class T>
Node <T>* getNodeBST(const T& key) { //새로운 노드를 생성하는 함수
    return new Node<T>(key);
}
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    if (__x == nullptr) //단말노드를 만나면 return 0
        return 0;
    else
        return 1 + max(__height(__x-> __left_), __height(__x-> __right_)); //왼/오 자식을 호출하여 단말노드까지 가서 0을 return하면 왼쪽과 오른쪽 각각의 level+1 값이 최종적으로 return된다 왼/오 트리의 높이를 비교해서 큰 값이 트리의 높이이다 
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) { //트리의 크기를 구한다
    if (__x == nullptr) return 0;
    return 1 + __size(__x->__left_) + __size(__x->__right_);
}
//
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) { //BST는 왼쪽 트리 단말노드에 최소값이 있다
    while (__x->__left_ != nullptr) {
        __x = __x->__left_;
    }
    return __x;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) { //BST는 오른쪽 트리 단말노드에 최대값이 있다
    while (__x->__right_ != nullptr) {
        __x = __x->__right_;
    }
    return __x;
}

template <class _NodePtr> 
void __inorder(_NodePtr __x) { //중위순회
    if(__x->__left_ == nullptr) return;
    else {
        __inorder(__x->__left_); //왼쪽 자식 호출
        cout << __x->__key_ << ' ';
        __inorder(__x->__right_);//오른쪽 자식 호출 
    }
}

template <class _NodePtr, class _Tp>
pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    
    if (__root == nullptr) {
        __root = getNodeBST<_Tp>(key); //루트가 비어있으면 새 노드를 만들어 삽입
        return { __root, true }; //현재 루트포인터와 key값의 유무를 return한다
    }
    else if (key < __root->__key_) {    // key가 루트보다 작으면 왼쪽 서브트리로 재귀
        return __insertBST(__root->__left_, key); 
    }
    else if (key > __root->__key_) { // key가 루트보다 크면 오른쪽 서브트리로 재귀
        return __insertBST(__root->__right_, key);
    }
    else {   // 같으면 새로운 노드를 삽입하지 않고 이미 있음을 반환한다
        return { __root, false}; 
    }
} //반환할 때는 pair<_NodePtr, bool>형태를 지켜서 {}이렇게 반환해야한다.

//BST에서 삭제할때는 핵심 특성(왼쪽 서브트리 max < 루트값 < 오른쪽 서브트리의 min 을 유지하기 위해 다음과 같은 로직을 수행한다
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
    if (__root == nullptr) return nullptr; //루트가 비어있으면 null포인터를 반환
    if (key < __root->__key_) return __eraseBST(__root->__left_, key); //루트의 키값이 삭제하려는 값(key)보다 작으면 왼쪽 트리를 호출한다(BST의 특성때문)
    else if (key > __root->__key_) return __eraseBST(__root->__right_, key); //루트의 키값이 삭제하려는 값(key)보다 크면 오른쪽 트리를 호출한다(BST의 특성때문)
    else { //만약 루트의 키값과 삭제하려는 값(key)값이 같으면 
        _NodePtr _p = nullptr;
        if (__root->__left_ == nullptr && __root->__right_ == nullptr) { //단말노드일때는 
            _p = __root;
            __root = nullptr;
            return _p;
        }
        else if (__root->__left_ == nullptr || __root->__right_ == nullptr) { //자식이 하나 일때는 그 자식노드를 부모로 올리기
            _NodePtr _ch = (__root->__left_ != nullptr) ? __root->__left_ : __root->__right_; //자식 하나 선택
            _p = __root; //루트 노드를 잠시 복사해두고
            __root = _ch; //자식을 부모로 올린다음
            return _p; //복사해둔 루트노드를 return
        }
        else { //왼/오 자식이 모두 있다면, 
            _NodePtr _ch2 = __tree_min(__root->__right_); //오른쪽 서브트리의 최소값을 찾아서
            __root->__key_ = _ch2->__key_; //그 서브트리의 키값을 복사
            return __eraseBST(__root->__right_, _ch2->__key_); //
        }
    }
    
}

//트리의 모든 노드를 후위순회로 삭제  (메모리 해제)
template <class _NodePtr>
void __clear(_NodePtr& __x) { //후위 순회로 할당 해제
    if (__x == nullptr) return;
    else {
        __clear(__x->__left_); //오른쪽 자식 호출
        __clear(__x->__right_); //왼쪽 자식 호출
        delete __x;
    }
    __x = nullptr;
}

template <class _Tp>
class BST { //BST구조를 정의하는 클래스
    public:
        typedef _Tp key_type; //키의 자료형 별칭
        typedef size_t size_type; //크기 반환용 정수형
        typedef Node<key_type>* pointer;//노드 포인터 타입
        typedef const Node<key_type>* const_pointer;//상수 노드 포인터

    private:
        pointer __root_;//루트 노드 포인터

    public:
        BST() : __root_(nullptr) {} //생성자: 루트를 null로 초기화

    public:
        size_type height() const {
            return __height(__root_); //트리의 높이 함수 return
        }
        size_type size() const {
            return __size(__root_); //트리 크기(노드 개수)구하는 함수 return
        }
    public:
        void inorder() const {
                __inorder(__root_);
                cout << endl;
        }
    public:
        pair<const_pointer, bool> insert(const key_type& key) {
           pair<const_pointer, bool> ib = __insertBST(__root_, key);
            return { ib.first, ib.second };
        }
        const_pointer erase(const key_type& key) {
            pointer __r = __eraseBST(__root_, key);
            if (__r == nullptr) return nullptr;
            delete __r;

            return __r;
        }
        void clear() {
            __clear(__root_);
        }
        friend ostream& operator<<(ostream& os, const BST& tree) {
            os << tree.__root_; 
            return os;
    }

};

int main(){
    //freopen("BST-input.txt", "rt", stdin);
    BST<int> tree; 
    char command;
    int key;
    while (cin >> command >> key) {
        switch ((int)command) {
            case (int)'i':
                if (tree.insert(key).second == false) {
                    cerr << "i " << key << ": The key already exists" << endl;
                    continue;
                }
                break;
            case (int)'d':
                if (tree.erase(key) == nullptr) {
                    cerr << "d " << key << ": The key does not exist" << endl;
                    continue;
                }
                break;
            default:
                cerr << "Invalid command: " << command << endl;
                return (1);
                break;
        }
        cout << tree << endl;
    }
    tree.clear(); 
    return 0;
}