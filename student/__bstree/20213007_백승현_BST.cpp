#include <iostream>
using namespace std;

struct Node {
	int key; // 노드의 키값
	Node* left; // 왼쪽 자식 포인터
	Node* right; // 오른쪽 자식 포인터

	// 생성자
	Node() {
		key = 0;
		left = nullptr;
		right = nullptr;
	}
};

class BST {
public:
	Node *root;
	
	// 생성자
	BST() {
		this->root = nullptr;
	}

	// 트리의 높이를 계산
	int height(Node* T) {
		// base case
		if(T == nullptr)
			return 0;

		int hleft = this->height(T->left);
		int hright = this->height(T->right);

		// 더 높은 값에 1을 더해서 반환
		if(hleft > hright)
			return hleft + 1;
		else
			return hright + 1;
	}

	// 트리의 사이즈를 계산
	int size(Node *T) {
		// base case
		if(T == nullptr)
			return 0;
		
		return size(T->left) + size(T->right) + 1;
	}

	// 제일 작은 키값을 가진 노드를 반환
	Node* minNode(Node *T) {
		while(T != nullptr && T->left != nullptr)
			T = T->left;

		return T;
	}


	// 제일 큰 키값을 가진 노드를 반환
	Node *maxNode(Node *T) {
		while(T != nullptr && T->right != nullptr)
			T = T->right;

		return T;
	}

	// 새로운 노드를 생성 및 반환
	Node* getNodeBST() {
		return new Node();
	}

	// 해당 키값의 노드가 있는지 찾는 함수
	bool find(Node* T, int key) {
		if(T == nullptr)
			return false;
		if(T->key == key)
			return true;

		if(key < T->key)
			return this->find(T->left, key);
		else
			return this->find(T->right, key);
	}

	// 삽입 연산자
	bool insertBST(BST* T, int key) {
		Node *parent = nullptr; // 부모노드 
		Node *node = T->root; // 현재 노드로 루트부터 시작

		// 삽입할 위치 찾기
		while(node != nullptr) {
			if(key == node->key) // 이미 키값이 존재하는 경우
				return false;
			
			parent = node; // 내려가기 전 부모 노드 변경
			if(key < node->key) 
				node = node->left;
			else
				node = node->right;
		}

		// key값을 갖는 노드 생성
		Node *newNode = this->getNodeBST();
		newNode->key = key;
		newNode->left = nullptr;
		newNode->right = nullptr;

		// 찾은 위치에 삽입
		if(T->root == nullptr) // 처음 삽입하는 경우에는 루트에 저장
			T->root = newNode;
		else if(key < parent->key)
			parent->left = newNode;
		else 
			parent->right = newNode;

		return true;
	}

	// 삭제 연산자
	Node* eraseBST(Node *T, int key) {
		Node* parent = nullptr;
		Node* node = T; // 마찬가지롵 탐색할 노드로 루트부터 탐색

		// 삭제할 노드의 위치를 탐색
		while(node != nullptr && key != node->key) {
			parent = node; // 내려가기전 부모노드 변경

			if(key < node->key)
				node = node->left;
			else
				node = node->right;
		}

		// 삭제할 노드가 없는 경우
		if(node == nullptr)
			return nullptr;

		// 단말 노드인 경우
		if(node->left == nullptr && node->right == nullptr) {
			// 삭제 노드가 부모 노드인 경우
			if(parent == nullptr) 
				T = nullptr;
			else if(parent->left == node)
				parent->left = nullptr;
			else
				parent->right = nullptr;
			
			delete node;
		}
		// 차수가 1인 경우
		else if(node->left == nullptr || node->right == nullptr) {
			// 삭제할 노드의 왼쪽 자식이 있는 경우
			if(node->left !=  nullptr) {
				if(parent == nullptr) T = node->left;
				else if(parent->left == node) 
					parent->left = node->left;
				else 
					parent->right = node->left;
			}
			// 삭제할 노드의 오른쪽 자식이 있는 경우
			else {
				if(parent == nullptr) T = node->right;
				else if(parent->left == node)
					parent->left = node->right;
				else
					parent->right = node->right;
			}

			delete node;
		}
		// 차수가 2인 경우
		else if(node->left != nullptr && node->right != nullptr) {
			Node* tmp = nullptr;
			bool left = true;

			// 왼쪽이 더 긴 경우
			if(this->height(node->left) > this->height(node->right)) {
				tmp = this->maxNode(node->left);
				left = true;
			}
			// 오른쪽이 더 긴 경우
			else if(this->height(node->left) < this->height(node->right)) {
				tmp = this->minNode(node->right);
				left = false;
			}
			// 길이가 같은 경우 노드 갯수로 판별
			else {
				if(this->size(node->left) >= this->size(node->right)) {
					tmp = this->maxNode(node->left);
					left = true;
				}
				else {
					tmp = this->minNode(node->right);
					left = false;
				}	
			}

			node->key = tmp->key; // 키값 교체
			if(left)
				node->left = this->eraseBST(node->left, tmp->key);
			else
				node->right = this->eraseBST(node->right, tmp->key);
		}

		return T;
	}

	// 중위순회 연산 - 출력용
	void inOrder(Node* T) {
		if(T != nullptr) {
			cout << '<';
			if(T->left != nullptr)
				this->inOrder(T->left);
			cout << ' ' << T->key << ' ';
			if(T->right != nullptr)
				this->inOrder(T->right);
			cout << '>'; 
		}

	}

	// clear 함수
	void clear(Node* T) {
		if(T == nullptr)
			return ;
		this->clear(T->left);
		this->clear(T->right);
		delete T;
	}
};

int main() {
	BST bst;
	char com;
	int key;

	while(cin >> com >> key) {
		if(com == 'i') {
			// 이미 키값이 존재하는 경우
			if(bst.insertBST(&bst, key) == false) 
				cout << "i " << key << ": The key already exists\n";
			else {
				bst.inOrder(bst.root);
				cout << '\n';
			}
		}
		// 지우는 경우
		else {
			// 삭제할 키값이 없는 경우
			if(bst.find(bst.root, key) == false) {
				cout << "d " << key << ": The key does not exist\n";
			}
			else {
				bst.root = bst.eraseBST(bst.root, key);
				bst.inOrder(bst.root);
				cout << '\n';
			}
		}
	}

	bst.clear(bst.root);
	return 0;
}