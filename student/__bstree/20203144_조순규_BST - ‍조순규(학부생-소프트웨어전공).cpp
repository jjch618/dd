#include <iostream>
#include <algorithm>
using namespace std;

struct Node
{
    int key;
    Node* left;
    Node* right;
    int size, height;
};
 // 높이 구하기
int height(Node* T) { return T ? T->height : 0; }

 // 크기 구하기
int size(Node* T) { return T ? T->size : 0; }

Node* getNodeBST(int key) // 노드 추가
{
    Node* newNode = new Node; // 노드 추가
    newNode->key = key; // key값 반영
    newNode->left = nullptr; // 왼쪽 null
    newNode->right = nullptr; // 오른쪽 null
    newNode->height = 1; // 높이 1
    newNode->size = 1; // 크기 1
    return newNode;
}

void clear(Node* T)
{
    if(!T) return; // 노드가 없어질때까지 찾다가 null이면 멈춤
    clear(T->left); // 왼쪽 노드 찾으러 가기
    clear(T->right); // 오른족 노드 찾으러 가기
    delete T; // 노드 삭제
    T = nullptr;
}



void inorder(Node* T) //출력
{
    if(!T) return;
    cout << "<";
    inorder(T->left);
    cout << " " << T->key << " ";
    inorder(T->right);
    cout << ">";
}


 // right에서 가장 작은거 구하기 즉 오른쪽중에 가장 왼쪽애 있는 애 찾기
Node* minNode(Node* T) { while(T->left) T = T->left; return T; }

// left에서 가장 큰거 구하기 즉 왼쪽중에 가장 오른쪽 애 찾기
Node* maxNode(Node* T) { while(T->right) T = T->right; return T; }


void update(Node* T) // 높이, 크기 갱신
{
    if(!T) return;
    T->height = 1 + max(height(T->left), height(T->right)); // 왼쪽 노드의 높이 vs 오른쪽 노드의 높이 중 큰 값에 +1 한 높이를 저장
    T->size = 1 + size(T->left) + size(T->right); // 왼쪽 노드의 크기 + 오른쪽 노드의 크기 + 1 크기 저장
}

int insertBST(Node*& T, int key) // 노드 삽입
{
    if (!T)
    {
        T = getNodeBST(key); // key 를 넣어야 할 빈 노드를 찾으면 삽입
        return 1;
    }

    int result = 0;
    if(key < T->key) 
        result = insertBST(T->left, key); // 키 값이 현재 노드보다 작으면 왼쪽으로
    else if(key > T->key) 
        result = insertBST(T->right, key); // 키 값이 현재 노드보다 크면 오른쪽
    else 
        return 0;

    update(T); // 높이, 크기 갱신
    return result;

}


int eraseBST(Node*& T, int key) // 노드 삭제
{
    if(!T) return 0;
    
    int result = 0;
    if(T->key < key)
        result = eraseBST(T->right, key); // 현재 노드의 값이 삭제할 값보다 작으면 오른쪽
    else if(T->key > key)
        result = eraseBST(T->left, key); // 현재 노드의 값이 삭제할 값보다 크면 왼쪽
    else //찾음
    {
        Node* temp;
        if(T->left == nullptr && T->right == nullptr) // 양옆이 없을때
        {
            delete T;
            T = nullptr;
        }
        else if(T->left == nullptr) // 오른쪽만 있을때 오른쪽을 올림
        {
            temp = T->right;
            delete T;
            T = temp;
        }
        else if(T->right == nullptr) // 왼쪽만 있을때 왼쪽을 올림
        {
            temp = T->left;
            delete T;
            T = temp;
        }
        else //둘다 있으면 오른쪽에서 가장 작은 애를 찾아서 올림
        {
            // 왼쪽이 더 높은지, 더 큰지, 오른쪽이 더 높은지, 더 큰지 찾아서 minNode or maxNode 사용
            if( height(T->left) < height(T->right) || 
            (height(T->left) == height(T->right) && 
            size(T->left) < size(T->right)))
            {
                Node* mN = minNode(T->right);
                T->key = mN->key;
                result = eraseBST(T->right, mN->key);
            }
            else
            {
                Node* mN = maxNode(T->left);
                T->key = mN->key;
                result = eraseBST(T->left,mN->key);
            }
            update(T);
            return 1;
        }
        update(T);
        return 1;
    }
    update(T);
    return result;
}
int main()
{
    Node* T = nullptr;
    char command;
    int key;

    while (cin >> command >> key)
    {
        switch((int)command)
        {
            case (int) 'i':
                if(insertBST(T, key) == 0)
                    cerr << "i " << key << ": The key already exists" << endl;
                else
                {
                    inorder(T); // 출력
                    cout << endl;
                }
                break;
            case (int) 'd':
                if(eraseBST(T, key) == 0)
                    cerr << "d " << key << ": The key does not exist" << endl;
                else
                {
                    inorder(T); // 출력
                    cout << endl;
                }
                break;
            default:
                cerr << "Invalid command: " << command << endl;
        }
    }
    clear(T);
    T = nullptr;
    return 0;
}