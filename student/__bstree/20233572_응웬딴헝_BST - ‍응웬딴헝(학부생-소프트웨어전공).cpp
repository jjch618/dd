// 참고 내용 (Reference): geeksforgeeks's Binary Search Tree in C++ (https://www.geeksforgeeks.org/cpp/cpp-binary-search-tree/)
#include <iostream>
using namespace std;

// Structure representing a node in the binary search tree (이진 탐색 트리의 노드를 나타내는 구조체)
struct Node {
    int key;
    Node* left;
    Node* right;
};

// Create a new node (새로운 노드를 생성합니다)
Node* getNodeBST(int key) {
    Node* newNode = new Node();
    newNode->key = key;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Returns the height of the tree (트리의 높이를 반환합니다)
int height(Node* T) {
    if(T == nullptr) {
        return 0;
    }
    int hl = height(T->left);
    int hr = height(T->right);
    int higher = hl > hr ? hl : hr;
    return higher + 1;
}

// Returns the number of nodes in the tree (트리의 노드 수를 반환합니다)
int size(Node* T) {
    if (T == nullptr) {
        return 0;
    }
    int sl = size(T->left);
    int sr = size(T->right);
    return sl + sr + 1;
}

// Returns the smallest node (가장 작은 노드를 반환합니다)
Node* minNode(Node* T) {
    if (T == nullptr) {
        return nullptr;
    }
    Node* p = T;
    while (p->left != nullptr) {
        p = p->left;
    }
    return p;
}

// Returns the biggest node (가장 큰 노드를 반환합니다)
Node* maxNode(Node* T) {
    if (T == nullptr) {
        return nullptr;
    }
    Node* p = T;
    while (p->right != nullptr) {
        p = p->right;
    }
    return p;
}

// Inserts a key into the BST, returns true if inserted successfully, false if key already exists
// BST에 키를 삽입하고, 성공적으로 삽입되면 true를 반환하고 키가 이미 존재하면 false를 반환합니다
bool insertBST(Node*& T, int key) {
    if (T == nullptr) {
        T = getNodeBST(key);
        return true;
    }

    // Go to left or right subtree depending on key value (키 값에 따라 왼쪽 또는 오른쪽 서브 트리로 이동합니다)
    if (key < T->key) {
        return insertBST(T->left, key);
    } else if (key > T->key) {
        return insertBST(T->right, key);
    }

    // Key already exists (키가 이미 존재합니다)
    return false;
}

// Inorder traversal
void inorder(Node* T) {
    if (T == nullptr) {
        return;
    }

    cout << "< ";
    if (T->left != nullptr) {
        inorder(T->left);
        cout << " ";
    }

    cout << T->key;

    if (T->right != nullptr) {
        cout << " ";
        inorder(T->right);
    }

    cout << " >";
}

bool eraseBST(Node*& T, int key) {
    if (T == nullptr) {
        return false;
    }

    // Search for the key (키를 검색합니다)
    if (key < T->key) {
        return eraseBST(T->left, key);
    } else if (key > T->key) {
        return eraseBST(T->right, key);
    } else {
        // When the node has no child (노드에 자식이 없는 경우):
        if (T->left == nullptr && T->right == nullptr) {
            delete T;
            T = nullptr;
            return true;
        // When the node has 1 child (노드에 자식이 1개 있는 경우):
        } else if (T->left == nullptr) {
            Node* temp = T;
            T = T->right;
            delete temp;
            return true;
        } else if (T->right == nullptr) {
            Node* temp = T->left;
            T = T->left;
            delete temp;
            return true;
        // When the node has 2 children (노드에 자식이 2개 있는 경우):
        } else {
            Node* temp = nullptr;
            int flag = 0;

            int leftH = height(T->left);
            int rightH = height(T->right);
            int leftS = size(T->left);
            int rightS = size(T->right);

            if (leftH > rightH) {
                temp = maxNode(T->left);
                flag = 1;
            } else if (leftH < rightH) {
                temp = minNode(T->right);
                flag = 2;
            } else {
                if (leftS >= rightS) {
                    temp = maxNode(T->left);
                    flag = 1;
                } else {
                    temp = minNode(T->right);
                    flag = 2;
                }
            }

            // Replace current key with chosen replacement key (현재 키를 선택한 대체 키로 교체)
            T->key = temp->key;

            if(flag == 1) {
                return eraseBST(T->left, temp->key);
            } else {
                return eraseBST(T->right, temp->key);
            }
        }
    }
}

// Clears all nodes in the tree (트리의 모든 노드를 지웁니다)
void clear(Node*& T) {
    if (T == nullptr) {
        return;
    }
    clear(T->left);
    clear(T->right);
    delete T;
    T = nullptr;
}

// Prints the BST using inorder traversal (inorder traversal를 사용하여 BST를 인쇄합니다)
void print(Node* T) {
    if (T == nullptr) {
        cout << "\n";
    } else {
        inorder(T);
        cout << "\n";
    }
}

int main() {
    Node* T = nullptr;
    char command;
    int key;

    while (cin >> command >> key) {
        if(command == 'i') {
            // Insert operation (삽입 작업)
            if(!insertBST(T, key)) {
                cout << "i " << key << ": The key already exists\n";
            } else {
                print(T);
            }
        } else if (command == 'd') {
            // Delete operation (삭제 작업)
            if(!eraseBST(T, key)) {
                cout << "d " << key << ": The key does not exist\n";
            } else {
                print(T);
            }
        }
    }
    
    clear(T);
    return 0;
}



