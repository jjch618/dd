#include <iostream>
using namespace std;

template <class T>
class Node {
public:
    using key_type = T;
    using node_ptr = Node<T>*;
    using size_type = std::size_t;

    key_type key_;        // 노드 키값 data
    node_ptr left_;       // 왼쪽 자식
    node_ptr right_;      // 오른쪽 자식
    size_type height_;    // 높이 
    size_type count_;     // 노드 size

    Node() {
        key_ = key_type();
        left_ = nullptr;
        right_ = nullptr;
        height_ = 1;
        count_ = 1;
    }

    Node(const key_type& k) {
        key_ = k;
        left_ = nullptr;
        right_ = nullptr;
        height_ = 1;
        count_ = 1;
    }

    friend ostream& operator<<(ostream& os, const node_ptr& np) {
        if (np == nullptr) return os;
        os << np->left_;
        os << np->key_ << " ";
        os << np->right_;
        return os;
    }
};

template <class NodePtr>
unsigned __height(NodePtr n) {
    if (n == nullptr)
        return 0;
    else
        return n->height_;
}

template <class NodePtr>
unsigned __size(NodePtr n) {
    if (n == nullptr)
        return 0;
    else
        return n->count_;
}

template <class NodePtr>
NodePtr __tree_min(NodePtr n) {
    while (n != nullptr && n->left_ != nullptr)
        n = n->left_;
    return n;
}

template <class NodePtr>
void refresh(NodePtr n) {
    if (n == nullptr) return;

    unsigned lh = __height(n->left_);
    unsigned rh = __height(n->right_);

    if (lh > rh)
        n->height_ = 1 + lh;
    else
        n->height_ = 1 + rh;

    n->count_ = 1 + __size(n->left_) + __size(n->right_);
}

// inorder()
template <class NodePtr>
void __inorder(NodePtr root) {
    if (root == nullptr) return;
    cout << "<";
    __inorder(root->left_);
    cout << " " << root->key_ << " ";
    __inorder(root->right_);
    cout << ">";
}

// node maker
template <class T>
Node<T>* getNodeBST(const T& key) {
    Node<T>* newNode = new Node<T>(key);
    return newNode;
}

// insert()
template <class NodePtr, class T>
NodePtr __insertBST(NodePtr& root, const T& key, bool& ok) {
    if (root == nullptr) {
        ok = true;
        root = getNodeBST<T>(key);
        return root;
    }

    if (key < root->key_)
        __insertBST(root->left_, key, ok);
    else if (key > root->key_)
        __insertBST(root->right_, key, ok);
    else {
        ok = false; 
        return root;
    }

    refresh(root);
    return root;
}

// erase ()
template <class NodePtr, class T>
NodePtr __eraseBST(NodePtr& root, const T& key, bool& removed) {
    if (root == nullptr) {
        removed = false;
        return nullptr;
    }

    if (key < root->key_)
        __eraseBST(root->left_, key, removed);
    else if (key > root->key_)
        __eraseBST(root->right_, key, removed);
    else {
        removed = true;
        if (root->left_ == nullptr && root->right_ == nullptr) {
            delete root;
            root = nullptr;
            return nullptr;
        }
        else if (root->left_ == nullptr || root->right_ == nullptr) {
            Node<T>* child;
            if (root->left_ != nullptr)
                child = root->left_;
            else
                child = root->right_;
            delete root;
            root = child;
            return root;
        }
        else {
            Node<T>* succ = __tree_min(root->right_);
            root->key_ = succ->key_;
            __eraseBST(root->right_, succ->key_, removed);
        }
    }
    refresh(root);
    return root;
}

// tree clear
template <class NodePtr>
void __clear(NodePtr& root) {
    if (root == nullptr) return;
    __clear(root->left_);
    __clear(root->right_);
    delete root;
    root = nullptr;
}

// Bst class
template <class T>
class BST {
public:
    using node_ptr = Node<T>*;
    using const_ptr = const Node<T>*;
    using size_type = std::size_t;

private:
    node_ptr __root_;

public:
    BST() {
        __root_ = nullptr;
    }

    size_type height() const { return __height(__root_); }
    size_type size() const { return __size(__root_); }

    void inorder() const {
        __inorder(__root_);
        cout << endl;
    }

    pair<const_ptr, bool> insert(const T& key) {
        bool ok = false;
        node_ptr result = __insertBST(__root_, key, ok);
        return make_pair(result, ok);
    }

    bool erase(const T& key) {
        bool ok = false;
        __eraseBST(__root_, key, ok);
        return ok;
    }

    void clear() {
        __clear(__root_);
    }

    friend ostream& operator<<(ostream& os, const BST& tree) {
        __inorder(tree.__root_);
        return os;
    }
};

// main()
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    BST<int> tree;
    char cmd;
    int value;

    while (cin >> cmd >> value) {
        if (cmd == 'i') {
            if (!tree.insert(value).second) {
                cerr << "i " << value << ": The key already exists" << endl;
                continue;
            }
        } else if (cmd == 'd') {
            if (!tree.erase(value)) {
                cerr << "d " << value << ": The key does not exist" << endl;
                continue;
            }
        } else {
            cerr << "Invalid command: " << cmd << endl;
            return 1;
        }

        cout << tree << endl;
    }

    tree.clear();
    return 0;
}
