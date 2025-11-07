// 대부분 skeleton 코드와 pseudo 코드 그대로 작성했습니다.

#include <iostream>
// #include <stack>
// height(T), size(T)를 사용하기 위해 pseudo코드의 stack 라이브러리와 height, size 멤버변수는 사용하지 않았습니다.
using namespace std;

// 1. Node: key, left, right
template <class T>
class Node {
    public:
        T key;
        Node<T>* left;
        Node<T>* right;
        Node(): key(T()), left(nullptr), right(nullptr) {}
        Node(const T& k): key(k), left(nullptr), right(nullptr) {}
};

// 2. getNodeBST()
template <class T>
Node<T>* getNodeBST(const T& key) {
    return new Node<T>(key);
}

// 2. insertBST(T, key)
template <class T>
bool insertBST(Node<T>*& t, const T& newKey) {
    Node<T>* p = t;
    Node<T>* q = nullptr;

    // step 1: find position to insert newKey
    while (p != nullptr) {
        if (newKey == p->key) return false; // already exist
        q = p;
        if (newKey < p->key) p = p->left;
        else p = p->right;
    }

    // step 2: create newNode with newKey
    Node<T>* newNode = getNodeBST(newKey);

    // step 3: insert newNode as a child of q
    if (t == nullptr) t = newNode;
    else if (newKey < q->key) q->left = newNode;
    else q->right = newNode;

    return true;
}

// 3. height(T)
template <class T>
size_t height(Node<T>* p) {
    if(p == nullptr) return 0;
    return 1 + max(height(p -> left), height(p -> right));
}

// 3. size(T)
template <class T>
size_t size(Node<T>* p) {
    if(p == nullptr) return 0;
    return 1 + size(p -> left) + size(p -> right);
}

// 3. minNode(T)
template <class T>
Node<T>* minNode(Node<T>* p) {
    if(p == nullptr) return nullptr;
    while(p -> left) p = p -> left;
    return p;
}

// 3. maxNode(T)
template <class T>
Node<T>* maxNode(Node<T>* p) {
    if (p == nullptr) return nullptr;
    while (p -> right) p = p -> right;
    return p;
}

// 3. eraseBST(T, key)
template <class T>
bool eraseBST(Node<T>*& t, const T& eraseKey) {
    Node<T>* p = t;
    Node<T>* q = nullptr;

    // searchBST
    while(p != nullptr && eraseKey != p->key) {
        q = p; // searchParent
        if (eraseKey < p->key) p = p->left;
        else p = p->right;
    }

    // eraseKey was not found
    if (p == nullptr) return false;

    // case of degree 2 is reduced to case of degree 0 or case of degree 1
    if (p->left != nullptr && p->right != nullptr) {
        Node<T>* pTemp = p;
        Node<T>* qTemp = pTemp;

        size_t plh = height(p->left);
        size_t prh = height(p->right);
        size_t pls = size(p->left);
        size_t prs = size(p->right);
        if (plh < prh || (plh == prh && pls < prs)) {
            p = p->right;
            while (p->left != nullptr) {
                qTemp = p;
                p = p->left;
            }
        }
        else {
            p = p->left;
            while (p->right != nullptr) {
                qTemp = p;
                p = p->right;
            }
        }

        pTemp->key = p->key;
        q = qTemp;
    }

    // now degree of p is 0 or 1, delete p from T
    if (p->left != nullptr) { // case of degree 1 - left child
        if (q == nullptr) t = t->left; // case of root
        else if (q->left == p) q->left = p->left;
        else q->right = p->left;
    }
    else if (p->right != nullptr) { // case of degree 1 - right child
        if (q == nullptr)  t = t->right; // case of root
        else if (q->left == p) q->left = p->right;
        else q->right = p->right;
    }
    else { // case of degree 0
        if (q == nullptr) t = nullptr; // case of root
        else if (q->left == p) q->left = nullptr;
        else q->right = nullptr;
    }

    delete p;

    return true;
}

// 4. inorder(T)
template <class T>
void inorder(Node<T>* p) {
    if(p == nullptr) return;

    cout << '<';
    inorder(p->left);
    cout << ' ' << p->key << ' ';
    inorder(p->right);
    cout << '>';
}

// 5. clear(T)
template <class T>
void clear(Node<T>*& p) {
    if(p == nullptr) return;
    clear(p -> left);
    clear(p -> right);
    delete p;
    p = nullptr;
}

template <class T>
class BST {
    private:
        Node<T>* root;

    public:
        BST(): root(nullptr) {}

        bool insert(const T& key) {
            return insertBST(root, key);
        }

        bool erase(const T& key) {
            return eraseBST(root, key);
        }

        void inorderBST() {
            inorder(root);
        }
        void clearBST() {
            clear(root);
        }
    };

// only changed return type of some functions from skeleton code
int main() {
	BST<int>	tree;
	char		command;
	int			key;

	while (cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (tree.insert(key) == false) {
					cerr << "i " << key << ": The key already exists" << endl;
					continue;
				}
				break;
			case (int)'d':
				if (tree.erase(key) == false) {
					cerr << "d " << key << ": The key does not exist" << endl;
					continue;
				}
				break;
			default:
				cerr << "Invalid command: " << command << endl;
				return (1);
				break;
		}
		tree.inorderBST();
        cout << endl;
	}

	tree.clearBST();

	return (0);
}