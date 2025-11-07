#include <iostream>
#include <algorithm>

//노드 만들기!
struct Node {
    int key;
    Node* right;
    Node* left;
    int height;
    int size;
    //getNodeBST에서 초기화를 해주는 것이 훨씬 좋음
};

Node* getNodeBST(int key){ 
    Node* node = new Node;
    node-> key = key;
    node-> right = nullptr;
    node->left = nullptr;
    node-> height = 1;
    node-> size = 1;
    return node;
}
int height(Node* T){ 
    if(T == nullptr){
        return 0;
    }
    else{
        return 1 + std::max(height(T->left), height(T->right)); 
    } //노드 T가 존재한다면 루트로 하는 트리의 높이는 현재 노드 자신(1)+왼쪽과 오른쪽 서브트리 중 더 높은 쪽의 높이를 더하기
}
int size(Node* T){ 
    if(T == nullptr){
        return 0;
    }
    else{
        return 1 + size(T->left) + size(T->right);
    }//노드 T가 존재한다면 트리의 전체 크기는 현재 노드 자신 + 왼쪽 서브트리의 크기 + 오른쪽 서브트리의 크기라는 뜻
}
Node* minNode(Node* T){
    while(T->left != nullptr){
        T = T->left;
    } //왼쪽에 아무것도 없는게 아니라면 계속 왼쪽으로 가겠다
    return T;
}
Node* maxNode(Node* T){
    while (T->right != nullptr) {
        T = T->right;
    } //오른쪽에 아무것도 없는게 아니라면 계쏙 오른쪽으로 가겠다
    return T;
}
void clear(Node * T){
    if(T == nullptr) return;
    clear(T->left);
    clear(T->right);
    delete T;
    T = nullptr; //메모리 누수 방지를 위해 clear만들어주기
}

void inorder(Node* T){ 
    if(T == NULL){
        return;
    } 

    std::cout << "<";

    inorder(T->left);
    std::cout << " " << T->key << " ";

    inorder(T->right);
    std::cout << ">";
}

int insertBST(Node*& T, int key){
    int result = 0;
    if(T == NULL) { //nullptr로 하면 오류, NULL로 해야 정답
        T = getNodeBST(key);
        return 1;
    } //트리가 비어있으면 새 노드를 트리의 루트로 만든다
    if(T->key == key){
        return 0; //삽입하려는 키와 원래 있던 값이 같다면 넣을 필요 X
    }//
    else if(T->key < key) {
        result = insertBST(T->right, key); //만약 삽입하려는 키가 있던 값보다 크다면 오른쪽으로 가기
    }
    else if(T->key > key) {
        result = insertBST(T->left, key); //만약 삽입하려는 키가 있던 값보다 작으면 왼쪽으로 가기
    }
    if(T != NULL){
        T->size = 1 + size(T->left) + size(T->right);
        T->height = 1 + std::max(height(T->left), height(T->right)); //NULL이 아니라면 사이즈와 높이를 각각 업데이트해주기
    }
    return result;
}
int eraseBST(Node*& T, int key){
    int result = 0;
    if(T == NULL) { //nullptr로 하면 오류, NULL로 해야 정답
        return 0 ; //아무것도 없는 트리라면 return 0
    }

    if(key < T->key) {
        result = eraseBST(T->left, key); //삭제하려는 값이 현재 노드 값보다 작다면 왼쪽으로 가기
    }
    else if(key > T->key){
        result = eraseBST(T->right, key); //삭제하려는 값이 현재 노드 값보다 크다면 오른쪽으로 가기
    }
    else{
        Node* q;
        if (T->left == nullptr && T->right == nullptr){ //자식이 0개인 경우
            delete T; //그냥 그 노드를 삭제하면 된다
            T = nullptr;
        }
        else if(T->left == nullptr){//자식이 1개인 경우(왼쪽이 없는 경우, 오른쪽에만 자식이 있는 경우)
            q = T->right; //임시 포인터에 현재 노드의 오른쪽 자식을 저장
            delete T; //현재 노드를 삭제
            T = q; //q를 연결하기
        }
        else if(T->right == nullptr){ //자식이 1개인 경우(오른쪽이 없는 경우, 왼쪽에만 자식이 있는 경우)
            q = T->left; //임시 포인터에 현재 노드의 왼쪽 자식장
            delete T; //현재 노드를 제거
            T = q; //q 연결해주기
        }
        else{ //자식이 2개인 경우
            if( height(T->left) < height(T->right) || (height(T->left) == height(T->right) && size(T->left) < size(T->right)))
            {//왼쪽 서브트리의 높이보다 오른쪽 서브트리의 높이가 크거나 왼쪽 서브트리와 오른쪽 서브트리의 높이가 같고 오른쪽 서브트리의 크기가 큰 경우
                Node* p = minNode(T->right); //그럴 때에는 오른쪽 서브트리에서 가장 최소의 값을 가진 것을 찾기
                T->key = p->key; //p의 값을 T로 복사
                result = eraseBST(T->right, p->key); //중복되니까 삭제하기
            }
            else
            {
                Node* p = maxNode(T->left); //왼쪽 서브트리에서 가장 최대인 값을 찾기
                T->key = p->key; //p의 값을 T로 복사
                result = eraseBST(T->left,p->key); //중복되니까 삭제하기
            }
            if(T != NULL){ //nullptr로 하면 오류, NULL로 해야 정답 //값이 NULL이 아니라면 크기와 높이를 업데이트해주기
                T->size = 1 + size(T->left) + size(T->right);
                T->height = 1 + std::max(height(T->left), height(T->right));
            }
            return 1;
        }
        if(T != NULL){ //nullptr로 하면 오류, NULL로 해야 정답 //값이 NULL이 아니라면 크기와 높이를 업데이트해주기
            T->size = 1 + size(T->left) + size(T->right);
            T->height = 1 + std::max(height(T->left), height(T->right));
        }
        return 1;
    }
    
    if(T != NULL){ //nullptr로 하면 오류, NULL로 해야 정답 //값이 NULL이 아니라면 크기와 높이를 업데이트해주기
        T->size = 1 + size(T->left) + size(T->right);
        T->height = 1 + std::max(height(T->left), height(T->right));
    }
    return result;
}
int main() {
    char command;
    int key;
    Node* T = nullptr;

    while (std::cin >> command >> key) { //skeleton 함수 참고 부분
        switch ((int)command) {
            
         case (int)'i':
            if (insertBST(T, key) == 0) {
               std::cerr << "i " << key << ": The key already exists" << std::endl;
            }
            else{
                inorder(T);
                std::cout << std::endl;
            }
            break;
                
         case (int)'d':
            if (eraseBST(T, key) == 0) {
               std::cerr << "d " << key << ": The key does not exist" << std::endl;
               
            }
            else{
                inorder(T);
                std::cout << std::endl;
            }
            break;
        default:
            std::cerr << "Invalid command: " << command << std::endl;
      }
   }
   clear(T); //메모리 누수 방지를 위해 clear해주기
   T = nullptr;

   return 0;
}