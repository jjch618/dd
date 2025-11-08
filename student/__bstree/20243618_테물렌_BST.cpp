#include <cstdio>
#include <cstdlib>

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int size;
};

Node* getNodeBST(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return nullptr;
    n->key = key;
    n->left = n->right = nullptr;
    n->height = 1;
    n->size = 1;
    return n;
}

int height(Node* T) {
    return T ? T->height : 0;
}

int size(Node* T) {
    return T ? T->size : 0;
}

Node* minNode(Node* T) {
    if (!T) return nullptr;
    while (T->left) T = T->left;
    return T;
}

Node* maxNode(Node* T) {
    if (!T) return nullptr;
    while (T->right) T = T->right;
    return T;
}

static Node* insert_internal(Node* T, int key, bool &inserted) {
    if (!T) {
        inserted = true;
        return getNodeBST(key);
    }
    if (key < T->key) {
        T->left = insert_internal(T->left, key, inserted);
    } else if (key > T->key) {
        T->right = insert_internal(T->right, key, inserted);
    } else {
        inserted = false;
        return T;
    }
    int hl = height(T->left);
    int hr = height(T->right);
    T->height = 1 + (hl > hr ? hl : hr);
    T->size = 1 + size(T->left) + size(T->right);
    return T;
}

Node* insertBST(Node* T, int key) {
    bool inserted = false;
    Node* newT = insert_internal(T, key, inserted);
    if (!inserted)
        fprintf(stderr, "i %d: The key already exists\n", key);
    return newT;
}

static Node* erase_internal(Node* T, int key, bool &deleted) {
    if (!T) return nullptr;
    if (key < T->key) {
        T->left = erase_internal(T->left, key, deleted);
    } else if (key > T->key) {
        T->right = erase_internal(T->right, key, deleted);
    } else {
        deleted = true;
        if (!T->left && !T->right) {
            free(T);
            return nullptr;
        } else if (!T->left) {
            Node* r = T->right;
            free(T);
            return r;
        } else if (!T->right) {
            Node* l = T->left;
            free(T);
            return l;
        } else {
            Node* succ = minNode(T->right);
            T->key = succ->key;
            T->right = erase_internal(T->right, succ->key, deleted);
            deleted = true;
        }
    }
    int hl = height(T->left);
    int hr = height(T->right);
    T->height = 1 + (hl > hr ? hl : hr);
    T->size = 1 + size(T->left) + size(T->right);
    return T;
}

Node* eraseBST(Node* T, int key) {
    bool deleted = false;
    Node* newT = erase_internal(T, key, deleted);
    if (!deleted)
        fprintf(stderr, "d %d: The key does not exist\n", key);
    return newT;
}

void inorder(Node* T) {
    if (!T) return;
    putchar('<');
    if (T->left) inorder(T->left);
    printf(" %d ", T->key);
    if (T->right) inorder(T->right);
    putchar('>');
}

void clear(Node* T) {
    if (!T) return;
    clear(T->left);
    clear(T->right);
    free(T);
}

int main() {
    Node* root = nullptr;
    char op;
    int key;
    while (scanf(" %c %d", &op, &key) == 2) {
        if (op == 'i') {
            root = insertBST(root, key);
        } else if (op == 'd') {
            root = eraseBST(root, key);
        }
        if (!root) {
            putchar('\n');
        } else {
            inorder(root);
            putchar('\n');
        }
    }
    clear(root);
    return 0;
}
