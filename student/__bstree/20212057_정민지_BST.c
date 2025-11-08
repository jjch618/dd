#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>


// 상수 정의 
#define LEFT 0 
#define RIGHT 1

// 매크로 정의 (두 값 중 더 큰 값 반환)
#define max(a,b) ((a)>(b)?(a):(b))

// 노드 구조체 정의 
typedef struct NODE {
	int key;			// 노드가 저장하는 키 값
	struct NODE* left;	// 왼쪽 자식 노드 포인터
	struct NODE* right;	// 오른쪽 자식 노드 포인터 
}Node;


// 트리 높이 계산 
int height(Node* T) {
	if (T == NULL) {
		return 0; // 공백 트리의 높이는 0 
	}
	else {
		return 1 + max(height(T->left), height(T->right)); // 재귀문으로 자식 노드가 있다면 방문함과 동시에 높이는 + 1 증가 
	}
}

// 트리의 전체 노드 수 계산 
int size(Node* T) {
	if (T == NULL) { return 0; }
	return 1 + size(T->left) + size(T->right); // 재귀문으로 자식 노드가 있다면 방문함과 동시에 전체 노드 수 + 1
}

// 가장 작은 키 갖는 노드 찾기 
Node* minNode(Node* T) {
	Node* current = T;

	// 더이상 왼쪽 자식이 존재하지 않을 때까지 이동 
	while (current->left != NULL) {
		current = current->left;
	}

	return current;
}


// 가장 큰 키 갖는 노드 찾기 
Node* maxNode(Node* T) {
	Node* current = T;

	// 더이상 오른쪽 자식이 존재하지 않을 때까지 이동 
	while (current->right != NULL) {
		current = current->right;
	}

	return current;
}

// 중위순회 
void inorder(Node* T) {
	if (T == NULL) {
		return;
	}
	printf("<");
	inorder(T->left);		// 왼쪽 서브트리 출력
	printf(" %d ", T->key);	// 현재 노드의 키 값 출력
	inorder(T->right);		// 오른쪽 서브트리 출력 
	printf(">");
}

// 트리의 모든 노드의 메모리 해제 
void clear(Node** T) {
	if (*T == NULL) {
		return;
	}

	clear(&((*T)->left));
	clear(&((*T))->right);


	free(*T);	// 현재 노드의 메모리 해제
	*T = NULL;	// 댕글링 포인터 방지 

}

// 새로운 노드 메모리 할당
Node* getNodeBST() {
	Node* newNode = (Node*)calloc(1, sizeof(Node));			// 새 노드 메모리 할당 
	assert(newNode != NULL);

	return newNode;
}

// 특정 키 노드의 부모 노드 탐색
Node* searchParent(Node* T, int key) {
	Node* q = NULL;					// 부모 노드는 널 포인트로
	Node* p = T;					// 특정 키 값 갖는 노드 탐색하는 노드 포인터 

	while (p != NULL) {
		if (key == p->key) {		// 찾으면 부모 노드 포인터 반환
			return q;
		}
		q = p;						// 찾지 못하면 부모 노드 포인터를 현재 탐색 포인터로 갱신
		if (key < p->key) {			// 찾는 키 값보다 작으면 왼쪽으로 탐색 노드 이동
			p = p->left;
		}
		else {
			p = p->right;			// 찾는 키 값보다 크면 오른쪽으로 탐색 노드 이동 
		}
	}
	return NULL;					// 못찾은 경우 널 반환 
}

// 특정 키 갖는 노드 탐색 
Node* searchBST(Node* T, int key) {
	Node* p = T;
	if (p == NULL) {						// 공백 트리이거나 해당 키 갖는 노드가 없는 경우 널 포인터 반환 
		return NULL;
	}
	if (key == p->key) {					// 해당 키 찾으면 현재 탐색 포인터 반환 
		return p;
	}
	if (key < p->key) {						// 찾는 키 값보다 작으면 왼쪽으로 탐색 노드 이동
		return searchBST(T->left, key);
	}
	else {
		return searchBST(T->right, key);	// 찾는 키 값보다 크면 오른쪽으로 탐색 노드 이동 
	}
}

// BST 노드 삽입 함수 
bool insertBST(Node** T, int key) {
	Node* q = NULL;
	Node* p = *T;

	// 새 노드의 삽입 위치 탐색 
	while (p != NULL) {
		if (key == p->key) {								// 이미 해당 키 갖는 노드가 존재하면 경고 출력 후 true 반환 
			printf("i %d: The key already exists", key);
			return false;
		}
		q = p;												// 노드 p가 이동하기 전 현재 p의 포인터를 q로 업데이트
		if (key < p->key) {									// 찾는 키 값보다 작으면 왼쪽으로 노드 p 이동
			p = p->left;
		}
		else {
			p = p->right;									// 찾는 키 값보다 크면 오른쪽으로 노드 p 이동
		}
	}

	// while문을 빠져나왔다는 건, 새 노드를 삽입할 위치를 찾았다는 것 (= p는 NULL인 상태)
	Node* newNode = getNodeBST();
	// 새 노드의 키값, 왼쪽/오른쪽 자식 노드 NULL로 초기화 
	newNode->key = key;
	newNode->right = NULL;
	newNode->left = NULL;

	// 공백 트리인 경우 새 노드를 루트 노드로 
	if (*T == NULL) {
		*T = newNode;
	}
	else if (key < q->key) {	// 새 노드의 부모 키 값보다 작다면 부모 노드의 왼쪽으로 새 노드 할당 
		q->left = newNode;
	}
	else {
		q->right = newNode;		// 새 노드의 부모 키 값보다 크다면 부모 노드의 오른쪽으로 새 노드 할당 
	}

	return true;
}

// BST 노드 삭제 함수
Node* eraseBST(Node** T, int key) {
	// 공백 트리인 경우 삭제할 노드가 존재하지 않기에 경고 출력 후 널 반환 
	if (*T == NULL) {
		printf("d %d: The key does not exist", key);
		return NULL;
	}


	Node* p = searchBST(*T, key);		// 삭제할 키 값 갖는 노드 위치 가져옴
	Node* q = searchParent(*T, key);	// 삭제 노드의 부모 노드 위치 가져옴 

	// 삭제 노드가 존재하지 않다면 삭제할 수 없기에 경고 출력 후 널 반환 
	if (p == NULL) {
		printf("d %d: The key does not exist", key);
		return q;
	}

	// 자식이 없는 단말 노드인 경우 
	if (p->left == NULL && p->right == NULL) {
		if (q != NULL) {				// 삭제 노드가 루트 노드가 아닌 경우 
			if (q->left == p) {			// 삭제 노드가 부모 노드의 왼쪽 노드라면 부모 노드의 왼쪽 자식 노드 포인터를 널로 
				q->left = NULL;
			}
			else {
				q->right = NULL;		// 삭제 노드가 부모 노드의 오른쪽 노드라면 부모 노드의 오른쪽 자식 노드 포인터를 널로
			}
		}
		else {							// 삭제 노드가 루트 노드인 경우 트리의 루트를 널로 
			*T = NULL;
		}

		free(p); p = NULL;				// 최종 삭제 노드는 메모리 해제후 댕글링 포인터 방지 위해 널로 설정 
	}
	else if (p->left == NULL || p->right == NULL) {	// 자식이 하나만 있는 경우 
		Node* r = NULL;					// 삭제 노드의 자식 노드 
		if (p->left != NULL) {			// 삭제 노드가 왼쪽 자식 노드만 있는 경우 자식 노드를 삭제노드의 왼쪽 노드 포인터로 할당 
			r = p->left;
		}
		else {							// 삭제 노드가 오른쪽 자식 노드만 있는 경우 자식 노드를 삭제노드의 오른쪽 노드 포인터로 할당 
			r = p->right;
		}
		if (q != NULL) {				// 공백 트리가 아닌 경우 
			if (q->left == p) {			// 삭제 노드가 부모 노드의 왼쪽 노드였던 경우 부모 노드의 왼쪽을 삭제 노드의 자식 노드로 할당
				q->left = r;
			}
			else {						// 삭제 노드가 부모 노드의 오른쪽 노드였던 경우 부모 노드의 오른쪽을 삭제 노드의 자식 노드로 할당
				q->right = r;
			}
		}
		else {							// 공백 트리였던 경우 삭제 노드의 자식 노드를 루트로 
			*T = r;
		}
		free(p); p = NULL;				// 최종 삭제 노드는 메모리 해제후 댕글링 포인터 방지 위해 널로 설정 
	}
	else {
		Node* r = NULL;						// 삭제 노드의 후계 노드 
		int left_hight = height(p->left);	// 삭제 노드의 왼쪽 서브트리 높이 
		int right_hight = height(p->right);	// 삭제 노드의 오른쪽 서브트리 높이
		int left_count = size(p->left);		// 삭제 노드의 왼쪽 서브트리 총 노드 수 
		int right_count = size(p->right);	// 삭제 노드의 오른쪽 서브트리 총 노드 수 



		int flag;

		if (left_hight > right_hight) {		// 삭제 노드의 왼쪽 서브트리의 높이가 오른쪽 서브트리의 높이보다 크다면 왼쪽 서브트리에서 후계자 노드 찾기 
			r = maxNode(p->left);
			flag = LEFT;					// 플래그 설정 
		}
		else if (left_hight < right_hight) {// 삭제 노드의 오른쪽 서브트리의 높이가 왼쪽 서브트리의 높이보다 크다면 오른쪽 서브트리에서 후계자 노드 찾기
			r = minNode(p->right);
			flag = RIGHT;					// 플래그 설정 
		}
		else {								// 왼쪽 서브트리와 오른쪽 서브트리의 높이가 같은 경우 
			if (left_count >= right_count) {// 삭제 노드의 왼쪽 서브트리의 전체 노드 수가 오른쪽 서브트리의 전체 노드 수보다 크다면 왼쪽 서브트리에서 후계자 노드 찾기
				r = maxNode(p->left);
				flag = LEFT;				// 플래그 설정
			}
			else {							// 삭제 노드의 오른쪽 서브트리의 전체 노드 수가 왼쪽 서브트리의 전체 노드 수보다 크다면 오른쪽 서브트리에서 후계자 노드 찾기
				r = minNode(p->right);
				flag = RIGHT;				// 플래그 설정
			}
		}

		p->key = r->key;					// 삭제 노드의 키 값을 후계자 노드의 키 값으로 업데이트 
		if (flag == LEFT) {					// 플래그가 왼쪽이라면 후계자 노드가 삭제 노드의 왼쪽 서브트리에 존재하는 경우이므로 왼쪽 서브트리에서 해당 후계자 노드의 키 값을 갖는 노드 삭제
			eraseBST(&(p->left), r->key);
		}
		else {
			eraseBST(&(p->right), r->key);	// 플래그가 오른쪽이라면 후계자 노드가 삭제 노드의 오른쪽 서브트리에 존재하는 경우이므로 오른쪽 서브트리에서 해당 후계자 노드의 키 값을 갖는 노드 삭제
		}
	}

	return *T;								// 최종 루트 노드 반환 

}

int main() {
	Node* root = NULL;			// BST 노드의 루트는 널로 초기화
	char cmd[1000][10];			// 명령어 저장 배열
	char input_string[5000];	// 한 줄 마다의 입력 버퍼
	char choice;				// 명령 타입 (i 또는 d)
	int key;					// 키 값 



	int i = 0;
	while (1) {	// 입력 받기 
		if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
			break;
		}
		if (input_string[0] == '\n') { // 빈 줄이면 종료하고 명령어 저장 배열에 입력 저장 
			break;
		}
		strcpy(cmd[i++], input_string);
	}

	for (int j = 0; j < i; j++) {
		sscanf(cmd[j], " %c%d", &choice, &key);	// 한 줄 명령어씩 수행 
		if (choice == 'i') {	// 삽입 명령어라면 노드 삽입 후 트리 출력 
			if (insertBST(&root, key) == true && root != NULL) {
				inorder(root);

			}

		}
		else if (choice == 'd') {	// 삭제 명령어라면 노드 삭제 후 트리 출력 
			if (eraseBST(&root, key) != NULL) {
				inorder(root);

			}

		}
		printf("\n");

	}

	clear(&root);	// 메모리 해제 


	return 0;

}