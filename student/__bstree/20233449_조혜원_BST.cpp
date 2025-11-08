#include <iostream>

class Node {
	int key;
	Node* left;
	Node* right;

	int height;
	int size;

public:
	Node* getNodeBST();
	int insertBST(Node*& T, int newKey);	// 이미 값이 존재할 경우를 위해 int 반환값 사용

	void updateHeight(Node*& T);					// 멤버 변수와 이름 충돌해서 height(T)에서 변경
	void updateSize(Node*& T);						// 멤버 변수와 이름 충돌해서 size(T)에서 변경
	Node* minNode(Node* T, Node* stack[], int& i);	// stack에 부모 노드를 저장하므로 매개변수 추가
	Node* maxNode(Node* T, Node* stack[], int& i);	// stack에 부모 노드를 저장하므로 매개변수 추가
	int eraseBST(Node*& T, int deleteKey);	// 삭제할 키가 없을 경우를 위해 int 반환값 사용

	void inorder(Node* T);

	void clear(Node*& T);
};

Node* Node::getNodeBST() {
	Node* ptr = new Node;
	ptr->right = NULL;
	ptr->left = NULL;
	ptr->height = 0;
	ptr->size = 1;
	return ptr;
}

void Node::updateHeight(Node*& T) {
	if (T == NULL) return;

	// height 초기화
	int l_height = (T->left == NULL) ? -1 : T->left->height;
	int r_height = (T->right == NULL) ? -1 : T->right->height;
	T->height = 1 + ((l_height > r_height) ? l_height : r_height);
}
void Node::updateSize(Node*& T) {
	if (T == NULL) return;

	// size 초기화
	int l_size = (T->left == NULL) ? 0 : T->left->size;
	int r_size = (T->right == NULL) ? 0 : T->right->size;
	T->size = 1 + l_size + r_size;
}

Node* Node::minNode(Node* T, Node* stack[], int& i) {
	if (T == NULL) return NULL;

	Node* p = T;
	while (p->left != NULL) {
		stack[i++] = p; //지나간 노드를 stack에 저장
		p = p->left;
	}
	return p; //최솟값 노드 반환
}

Node* Node::maxNode(Node* T, Node* stack[], int& i) {
	if (T == NULL) return NULL;

	Node* p = T;
	while (p->right != NULL) {
		stack[i++] = p; //지나간 노드를 stack에 저장
		p = p->right;
	}
	return p; // 최댓값 노드 반환
}

int  Node::insertBST(Node*& T, int newKey) {
	Node* p = T; // 현재노드
	Node* q = NULL; // 부모노드
	Node* stack[1000];
	int i = 0;
	
	// 새로 삽입할 위치 찾기 & 방문한 노드를 스택에 저장
	while (p != NULL) {
		if (newKey == p->key) //이미 트리에 존재하므로 종료
			return -1;

		q = p;
		stack[i++] = q;

		if (newKey < p->key)
			p = p->left; //현재 노드의 왼쪽 트리를 가리키게 됨
		else p = p->right; // 현재 노드의 오른쪽 트리를 가리키게 됨
	}

	// newKey를 갖는 새로운 노드 생성
	Node* newNode = getNodeBST();
	newNode->key = newKey;

	// 노드를 BST에 삽입
	if (T == NULL)
		T = newNode;
	else if (newKey < q->key)
		q->left = newNode;
	else q->right = newNode;

	// 부모 노드의 높이 & 크기 초기화
	while (i > 0) {
		q = stack[--i];

		updateSize(q);
		updateHeight(q);
	}
	return 0;
}

int Node::eraseBST(Node*& T, int deleteKey) {
	Node* p = T; // 현재노드
	Node* q = NULL; // 부모노드
	Node* stack[1000];
	int i = 0;

	// 삭제할 노드의 위치 찾기 & 방문한 노드를 스택에 저장
	while (p != NULL && deleteKey != p->key) {
		q = p;
		stack[i++] = q;

		if(deleteKey < p->key)
			p = p->left; //현재 노드의 왼쪽 트리를 가리키게 됨
		else p = p->right; // 현재 노드의 오른쪽 트리를 가리키게 됨
	}

	if (p == NULL) // 삭제할 노드가 존재하지 않음
		return -1;

	if (p->left != NULL && p->right != NULL) { // 삭제할 노드의 차수가 2
		Node* tempNode = p;
		stack[i++] = p;

		// 현재 p노드 대신 삭제할 노드 찾기
		if (p->left->height < p->right->height ||
			(p->left->height == p->right->height && p->left->size < p->right->size)){
			// 오른쪽 subTree에서 최솟값 찾기
			p = minNode(p->right, stack, i);
		}
		else {
			// 왼쪽 subTree에서 최댓값 찾기
			p = maxNode(p->left, stack, i);
		}

		tempNode->key = p->key; 
		q = stack[--i]; // 대신 삭제할 노드의 부모를 가리킴
	}

	// 삭제할 노드의 차수가 1
	if (p->left != NULL) {		// eraseNode 왼쪽 subtree만 존재
		if (q == NULL)			// root tree
			T = T->left;
		else if (q->left == p)	// eraseNode가 부모 노드의 왼쪽에 존재
			q->left = p->left;
		else					// eraseNode가 부모 노드의 오른쪽에 존재
			q->right = p->left;
	}
	else if (p->right != NULL) {// eraseNode의 오른쪽 subtree만 존재
		if (q == NULL)			// root tree
			T = T->right;
		else if (q->left == p)	// eraseNode가 부모 노드의 왼쪽에 존재
			q->left = p->right;
		else					// eraseNode가 부모 노드의 오른쪽에 존재
			q->right = p->right;
	}
	// 삭제할 노드의 차수가 0
	else { 
		if (q == NULL)			// root tree
			T = NULL;
		else if (q->left == p)	// 왼쪽 단말 노드 삭제
			q->left = NULL;
		else					// 오른쪽 단말 노드 삭제
			q->right = NULL;
	}

	// 삭제할 노드 메모리 해제
	delete p;

	// 부모 노드의 높이 & 크기 초기화
	while (i > 0) {
		q = stack[--i];
		updateSize(q); 
		updateHeight(q);
	}
	return 0;
}

void Node::inorder(Node* T) {
	if (T != NULL) {
		std::cout << "<";
		inorder(T->left);
		std::cout << " "<<T->key<<" ";
		inorder(T->right);
		std::cout << ">";
	}
}

void Node::clear(Node*& T) {
	if (T == NULL) // 더 이상 삭제할 노드 없음
		return;

	// 자식 노드 먼저 삭제
	clear(T->left);
	clear(T->right);

	// 현재 노드 삭제
	delete T;
}
int main() {
	Node* T = NULL; // 루트 노드를 가리킬 포인터
	Node bst;

	char command;
	int num;


	while (std::cin >> command >> num) {
		int existF = 0, noExistF=0;

		if (command == 'i')
			existF=bst.insertBST(T, num);
		else if (command == 'd')
			noExistF = bst.eraseBST(T, num);

		if (existF == -1)
			std::cout << "i " << num << ": The key already exists\n";
		else if (noExistF == -1)
			std::cout << "d " << num << ": The key does not exist\n";
		else {
			bst.inorder(T);
			std::cout << std::endl;
		}
	}
	// 트리 메모리 해제
	bst.clear(T);

	return 0;
}