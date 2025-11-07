#include <iostream>
using namespace std;

class Node {
public:
	int key;
	Node* left;
	Node* right;

	//getNodeBST()를 대체하는 생성자
public:
	Node() : key(0), left(nullptr), right(nullptr) {}
};

//서브트리의 높이 반환
int height_detail(Node* T) {
	if (T == nullptr) return 0;
	int lh = height_detail(T->left);
	int rh = height_detail(T->right);

	return 1 + (lh > rh ? lh : rh);
}

//서브트리 크기(노드 수) 반환
int size_detail(Node* T) {
	if (!T) return 0;
	return 1 + size_detail(T->left) + size_detail(T->right);
}

//서브트리에서 가장 작은 키를 가진 노드 반환
Node* minNode(Node* T) {
	if (!T) return nullptr;
	Node* cur = T;
	while (cur->left) {
		cur = cur->left;
	}
	return cur;
}

//서브트리에서 가장 큰 키를 가진 노드 반환
Node* maxNode(Node* T) {
	if (!T) return nullptr;
	Node* cur = T;
	while (cur->right) {
		cur = cur->right;
	}
	return cur;
}

// < {left-subtree} {key} {right-subtree} > 형태로 중위순회 출력
void inorder_detail(Node* T) {
	if (T) {
		cout << '<';

		if (T->left) {
			inorder_detail(T->left);
		}

		cout << ' ' << T->key;

		if (T->right) {
			cout << ' ';
			inorder_detail(T->right);
		}
		else {
			cout << ' ';
		}

		cout << '>';
	}
}

//새 키를 삽입
//생성된 노드와 생성 여부를 반환
pair<Node*, bool> insert_detail(Node*& T, int newKey) {
	Node* q = nullptr;
	Node* p = T;

	//삽입 위치 탐색
	while (p != nullptr) {
		//이미 존재하는 키라면 해당 노드와 false 반환
		if (newKey == p->key) {
			return pair<Node*, bool>(p, false);
		}
		q = p;

		if (newKey < p->key) p = p->left;
		else p = p->right;
	}

	Node* newNode = new Node(); //새 노드 생성(getNodeBST 대체)
	newNode->key = newKey; //새 노드에 키 설정

	if (T == nullptr) {
		T = newNode; //루트가 없었다면 새 노드가 루트
	}
	else if (newKey < q->key) {
		q->left = newNode; //부모의 왼쪽 자식으로 연결
	}
	else {
		q->right = newNode;//부모의 오른쪽 자식으로 연결
	}

	return pair<Node*, bool>(newNode, true);
}

//키에 해당하는 노드를 삭제하고 삭제된 노드 반환
Node* delete_detail(Node*& T, int deleteKey) {
	Node* q = nullptr;
	Node* p = T;

	//deleteKey 탐색
	while (p != nullptr && deleteKey != p->key) {
		q = p;
		if (deleteKey < p->key) p = p->left;
		else p = p->right;
	}

	//삭제할 노드를 찾지 못할 경우 nullptr 반환
	if (p == nullptr) {
		return nullptr;
	}

	//차수가 2인 경우
	if (p->left != nullptr && p->right != nullptr) {
		int lh = height_detail(p->left);
		int rh = height_detail(p->right);

		//높이가 더 높거나 노드 개수가 더 많은 쪽의 서브트리에서 대체할 노드를 찾음
		const char* flag;
		if (lh > rh) {
			flag = "left";
		}
		else if (lh == rh) {
			if (size_detail(p->left) >= size_detail(p->right)) {
				flag = "left";
			}
			else {
				flag = "right";
			}
		}
		else {
			flag = "right";
		}

		//대체 노드 선택 및 키 복사
		if (flag[0] == 'r') {
			Node* r = minNode(p->right);
			p->key = r->key;
			return delete_detail(p->right, r->key);
		}
		else {
			Node* r = maxNode(p->left);
			p->key = r->key;
			return delete_detail(p->left, r->key);
		}
	}

	//차수가 1인 경우
	if (p->left != nullptr) { //왼쪽 자식만 존재
		//루트 삭제 시 루트를 왼쪽 자식으로 교체
		if (q == nullptr) {
			T = T->left;
		}
		//부모 노드에 왼쪽 자식 연결
		else if (q->left == p) {
			q->left = p->left;
		}
		else {
			q->right = p->left;
		}
	}
	else if (p->right != nullptr) { //오른쪽 자식만 존재
		//루트 삭제 시 루트를 오른쪽 자식으로 교체
		if (q == nullptr) {
			T = T->right;
		}
		//부모 노드에 오른쪽 자식 연결
		else if (q->left == p) {
			q->left = p->right;
		}
		else {
			q->right = p->right;
		}
	}

	//차수가 0인 경우
	else {
		//트리에 삭제할 노드 하나뿐인 경우 빈트리가 됨
		if (q == nullptr) {
			T = nullptr;
		}
		//부모의 노드에서 p에 해당하는 자식 포인터를 nullptr로 끊음
		else if (q->left == p) {
			q->left = nullptr;
		}
		else {
			q->right = nullptr;
		}
	}
	return p; //삭제 노드 반환
}

//트리 전체 메모리 해제
void clear_detail(Node*& T) {
	if (T == nullptr) return;
	clear_detail(T->left);
	clear_detail(T->right);
	delete T;
	T = nullptr;

}

class BST {
private:
	Node* root;

public:
	BST() : root(nullptr) {}

public: //capacity
	int height() const {
		return height_detail(root);
	}
	int size() const {
		return size_detail(root);
	}

public: //lookup
	void inorder() const {
		inorder_detail(root);
		cout << endl;
	}

public: //modifier
	pair<Node*, bool> insert(int key) {
		return insert_detail(root, key);
	}

	//dangling pointer로 인해 bool 타입을 리턴하도록 수정
	bool erase(int key) {
		Node* removed = delete_detail(root, key);
		if (removed == nullptr) return false; //삭제할 노드가 존재하지 않는 경우 false 리턴
		delete removed; //삭제된 노드 메모리 해제
		return true;
	}

	void clear() {
		clear_detail(root);
	}
};

int main() {

	BST tree;
	char cmd;
	int key;

	while (cin >> cmd >> key) {
		switch (cmd) {
		case 'i':
			if (tree.insert(key).second == false) {
				cerr << "i " << key << ": The key already exists" << endl;
				continue;
			}
			break;

		case 'd':
			if (tree.erase(key) == false) {
				cerr << "d " << key << ": The key does not exist" << endl;
				continue;
			}
			break;

		default:
			cerr << "Invalid command: " << cmd << endl;
			tree.clear();
			return 1;
		}
		tree.inorder();
	}

	tree.clear();
	return 0;
}