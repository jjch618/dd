#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(A,B) ((A)>(B)?(A):(B))
#define MIN(A,B) ((A)<(B)?(A):(B))

typedef struct Node Node;
struct Node {
	Node *left;
	Node *right;
	long key;
	int size;
	int height;
};

typedef struct Stack {
	size_t bufsz;
	unsigned top;
	Node **buf;
} Stack;
typedef enum StackWhence {
	BOTTOM,
	TOP,
} StackWhence;

void stack_init(Stack *s) {
	s->bufsz = 1024;
	s->top = 0;
	s->buf = malloc(sizeof(Node *) * s->bufsz);
}
int stack_push(Stack *s, void *data) {
	Node **r;
	if (s->top == s->bufsz) {
		s->bufsz *= 2;
		if ((r = realloc(s->buf, sizeof(Node *) * s->bufsz)) == NULL)
			return -1;
		s->buf = r;
	}
	s->buf[s->top++] = data;
	return 0;
}
// index가 범위 벗어나면 -1, 아니면 0리턴
int stack_get(Stack *s, int index, StackWhence whence, Node **data) {
	if (whence == TOP)
		index += s->top - 1;
	if (index < 0 || index >= s->top)
		return -1;
	*data = s->buf[index];
	return 0;
}
int stack_pop(Stack *s, Node **data) {
	if (s->top <= 0)
		return -1;
	if (data)
		*data = s->buf[--s->top];
	else
		s->top--;
	return 0;
}
int stack_is_empty(Stack *s) { return s->top == 0; }
void stack_clear(Stack *s) { s->top = 0; }
void stack_del(Stack *s) { free(s->buf); }
void stack_show(Stack *s) {
	for (int i = 0; i < s->top; i++)
		printf(" %ld", s->buf[i]->key);
	putchar('\n');
}


ssize_t read_cmd(char **lineptr, size_t *n, char *op, long *key);

void searchBST(Node *T, long key, Stack *s);
// 삽입
Node *getNodeBST(long key);
int insertBST(Node **T, long key);

// 삭제
#define height(T) ((T) ? (T)->height : 0)
#define size(T) ((T) ? (T)->size : 0)
Node *minNode(Node *T, Stack *s);
Node *maxNode(Node *T, Stack *s);
Node *eraseBST(Node **T, long key);

// 순회
void inorder(Node *T);

void clear(Node **T);


int main(int argc, char *argv[])
{
	char *cmdbuf = NULL;
	size_t cmdbufsize = 0;
	char op;
	long key;
	Node *root = NULL;

	while (read_cmd(&cmdbuf, &cmdbufsize, &op, &key) != -1) {
		if (op == 'i')
			if (insertBST(&root, key) == -1)
				printf("i %ld: The key already exists\n", key);
		else if (op == 'd')
			eraseBST(&root, key);
		inorder(root);
	}

	clear(&root);

	if (cmdbuf)
		free(cmdbuf);

	return EXIT_SUCCESS;
}

// getline과 동일한 반환값
// lineptr, n: getline에 전달. lineptr은 사용 후 해제해야 함.
// op, val 값 반환 포인터
ssize_t read_cmd(char **lineptr, size_t *n, char *op, long *key)
{
	char *p;
	ssize_t nread;

	nread = getline(lineptr, n, stdin);
	p = *lineptr;
	*op = *p++;
	*key = strtol(p, NULL, 10);

	return nread;
}

// BST 검색
// key값 일치하는 노드 발견되면 해당 노드 push, 없으면 NULL push
void searchBST(Node *T, long key, Stack *s)
{
	Node *p = T;

	while (p) {
		if (key == p->key)
			break;
		stack_push(s, p);
		if (key < p->key)
			p = p->left;
		else
			p = p->right;
	}
	stack_push(s, p);
}


// 새 노드 생성
Node *getNodeBST(long key)
{
	Node *p = malloc(sizeof(Node));
	p->left = p->right = NULL;
	p->key = key;
	p->height = 1;
	p->size = 1;
	return p;
}


// 성공: 0 실패: -1
int insertBST(Node **T, long key)
{
	Node *q = NULL, *p = *T;
	Node *newNode;
	Stack path;

	stack_init(&path);

	searchBST(*T, key, &path);
	if (stack_get(&path, 0, TOP, &p) < 0) p = NULL;
	if (stack_get(&path, -1, TOP, &q) < 0) q = NULL;

	// 키가 이미 존재
	if (p) {
		stack_del(&path);
		return -1;
	}

	newNode = getNodeBST(key);

	if (!*T)
		*T = newNode;
	else {
		if (key < q->key)
			q->left = newNode;
		else 
			q->right = newNode;

		// height, size 재계산
		stack_pop(&path, NULL); // p는 NULL이므로 하나 버림
		while (!stack_is_empty(&path)) {
			stack_pop(&path, &p);
			p->height = MAX(height(p->left), height(p->right)) + 1;
			p->size = size(p->left) + size(p->right) + 1;
		}
	}

	stack_del(&path);
	return 0;
}

Node *minNode(Node *T, Stack *s)
{
	Node *p;

	for (p = T; p; p = p->left)
		stack_push(s, p);

	stack_get(s, 0, TOP, &p);
	return p;
}

Node *maxNode(Node *T, Stack *s)
{
	Node *p;

	for (p = T; p; p = p->right)
		stack_push(s, p);

	stack_get(s, 0, TOP, &p);
	return p;
}


// 삭제한 노드의 부모를 리턴
Node *eraseBST(Node **T, long key)
{
	Node *p, *q = NULL, *tmp;
	Stack path;

	stack_init(&path);

	searchBST(*T, key, &path);
	if (stack_get(&path, 0, TOP, &p) < 0) p = NULL;
	if (stack_get(&path, -1, TOP, &q) < 0) q = NULL;

	if (!p) {
		stack_del(&path);
		return NULL;
	}

	// 차수2인 경우 차수 0이나 1인 경우로 환원
	if (p->left && p->right) {
		tmp = p;
		if (height(p->left) > height(p->right)) {
			p = maxNode(p->left, &path);
		} else if (height(p->left) < height(p->right)) {
			p = minNode(p->right, &path);
		} else {
			if (size(p->left) >= size(p->right))
				p = maxNode(p->left, &path);
			else
				p = minNode(p->right, &path);
		}
		tmp->key = p->key;
		stack_get(&path, -1, TOP, &q);
	}

	// 차수 0
	if (!p->left && !p->right) {
		if (q) {
			if (q->left == p)
				q->left = NULL;
			else
				q->right = NULL;
		} else
			*T = NULL;
		free(p);
		goto ret;
	}

	// 차수 1
	if (p->left) {
		if (q) {
			if (q->left == p)
				q->left = p->left;
			else
				q->right = p->left;
		} else
			*T = p->left;
	} else {
		if (q) {
			if (q->left == p)
				q->left = p->right;
			else
				q->right = p->right;
		} else
			*T = p->right;
	}
	free(p);

ret:
	// height, size 재계산
	stack_pop(&path, NULL); // stack의 top은 방금 삭제한 노드이므로 스킵
	while (!stack_is_empty(&path)) {
		stack_pop(&path, &p);
		p->height = MAX(height(p->left), height(p->right)) + 1;
		p->size = size(p->left) + size(p->right) + 1;
	}
	stack_del(&path);

	return q;
}

// 순회
void __inorder(Node *T)
{
	if (!T)
		return;
	printf("<");
	if (T->left)
		__inorder(T->left);
	printf(" %ld ", T->key);
	if (T->right)
		__inorder(T->right);
	printf(">");
}

void inorder(Node *T)
{
	__inorder(T);
	putchar('\n');
}

void clear(Node **T)
{
	Node *p = *T;

	if (!p)
		return;

	clear(&p->left);
	clear(&p->right);
	
	free(*T);
	*T = NULL;
}
