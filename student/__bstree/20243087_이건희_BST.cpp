#include <iostream>
#include <stack>
#include <utility>

using namespace std;

struct Node
{
    explicit Node(const int key)
        : _key(key), _left(NULL), _right(NULL), _height(0)
    {}

    pair<Node*, Node*> getChildren() { return make_pair(_left, _right); }

    // 노드 높이 구하는 함수는 재귀적으로 구성하자.
    int height()
    {
        const int left_height  = _left  ? _left->height()  : 0;
        const int right_height = _right ? _right->height() : 0;
        _height = (left_height > right_height ? left_height : right_height) + 1;

        return _height;
    }
    // 노드 밑으로 서브트리의 전체 볼륨을 구한다.
    int size()
    {
        const int left_size  = _left  ? _left->size()  : 0;
        const int right_size = _right ? _right->size() : 0;
        return left_size + right_size + 1;
    }

    int   _key;
    Node* _left;
    Node* _right;
    int   _height;
};

class BST
{
public:
    BST()
        : _root(NULL)
    {}

    ~BST()
    {
        clear();
    }
    // insert 함수는 insertBST함수를 편리하게 사용하기 위해 제작하였습니다. 교안에 나타난 insertBST(T)함수 구현과는 별도입니다.
    bool insert(const int& key)
    {
        pair<Node*, bool> result = insertBST(_root, key);
        // 오류 받으면 에러 메세지 출력
        if (!result.second)
            cout << "i " << key << ": The key already exists" << '\n';

        return result.second;
    }

    // 삽입실패하면 false반환하자.
    bool erase(const int& key)
    {
        Node* to_delete = eraseBST(_root, key);
        // 삭제과정에서 받은 오류 메세지 출력
        if (to_delete == NULL)
        {
            cout << "d " << key << ": The key does not exist" << '\n';
            return false;
        }

        delete to_delete;
        return true;
    }

    // 삽입성공하면 트루 반환하자.
    // 두번째 인자로 성공 여부를 반환하고, insertBST(T) 프로토타입을 따름.
    pair<Node*, bool> insertBST(Node*& root, const int& key)
    {
        if (root == NULL)
        {
            root = getNodeBST(key);
            return make_pair(root, true);
        }

        if (root->_key == key)
            return make_pair(root, false);

        return key < root->_key ? insertBST(root->_left, key)
                                : insertBST(root->_right, key);
    }

    Node* eraseBST(Node*& node,  const int& key)
    {
        if (node == NULL)
            return NULL;

        if (key < node->_key) return eraseBST(node->_left, key);
        if (key > node->_key) return eraseBST(node->_right, key);

        Node* to_delete = node;

        // 자식 없음.
        if (node->_left == NULL && node->_right == NULL)
        {
            node = NULL;
            return to_delete;
        }

        // 1자식 오른쪽
        if (node->_left == NULL)
        {
            node = node->_right;
            return to_delete;
        }

        //1자식 왼쪽
        if (node->_right == NULL)
        {
            node = node->_left;
            return to_delete;
        }

        //2자식
        // 왼쪽 서브트리가 높이가 같거나 크면 그 쪽으로 먼저 노드를 승계함.
        int left_height = node->_left->height();
        int right_height = node->_right->height();

        if (left_height >= right_height)
        {
            // 왼쪽 서브트리에서 최댓값 (predecessor)
            Node* predecessor = maxNode(node->_left);
            node->_key = predecessor->_key;
            return eraseBST(node->_left, predecessor->_key);
        }
        else
        {
            // 오른쪽 서브트리에서 최솟값 (successor)
            Node* successor = minNode(node->_right);
            node->_key = successor->_key;
            return eraseBST(node->_right, successor->_key);
        }
    }

    void print()
    {
        if (_root == NULL) { cout << '\n'; }
        else
        {
            inorder(_root);
            cout << '\n';
        }
    }

    // 한줄함수
    void clear() { _clear(_root); }
    int getHeight() { return _root->height(); }
    Node* getNodeBST(const int key) { return new Node(key); }
private:
    Node* maxNode(Node* node)
    {
        if (node == NULL)
            return NULL;

        while (node->_right != NULL)
            node = node->_right;

        return node;
    }

    Node* minNode(Node* node)
    {
        if (node == NULL)
            return NULL;

        while (node->_left != NULL)
            node = node->_left;

        return node;
    }

    void _clear(Node*& root)
    {
        if (root == NULL) return;

        _clear(root->_left);
        _clear(root->_right);
        delete root;
        root = NULL;
    }

    void inorder(Node* node)
    {
        if (node == NULL)
            return;

        cout << '<';
        inorder(node->_left);
        cout << ' ' << node->_key << ' ';
        inorder(node->_right);
        cout << '>';
    }

private:
    Node* _root;
};

int main()
{
    char mod;
    int val;
    BST tree;

    // 입력개수제한이 없음
    while (cin >> mod)
    {
        switch (mod)
        {
        case 'i':
            cin >> val;
            if (tree.insert(val))
                tree.print();
            break;
        case 'd':
            cin >> val;
            if (tree.erase(val))
                tree.print();
            break;
        default:
            break;
        }
    }

    tree.clear();

    return 0;
}
