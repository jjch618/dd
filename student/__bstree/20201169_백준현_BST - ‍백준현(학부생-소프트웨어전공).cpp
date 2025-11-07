/*
* File Processing Report 2024 - Binary Search Tree skeleton code in C/C++
*
* 본 Skeleton code는 구현에 도움을 주기 위해 작성된 코드입니다.
* ❗️본 Skeleton code는 과제의 요구 조건을 완전히 충족시키지 않을 수 있습니다.❗️
* ❗️최종 점수 산정은 과제 PDF에 명시된 요구 사항을 기준으로 이루어집니다.❗️
*
* 미리 말씀드리면, skeleton code가 다소 어려울 수 있습니다.
* C++ 스러운 코딩 스타일과 코드 설계 방식에 관한 고찰이 담겨있으니, 충분한 시간을 들여 분석해보시기 바랍니다.
* 또, 재사용성을 고려하여 설계된 코드인 만큼 처음에는 이해하기 어려울 수 있습니다.
* 이후 AVL-Tree, B-Tree 과제에서도 그대로 사용되니, 이번 BST에서 잘 이해하고 넘어가시길 바랍니다.
*
*/

#include <iostream> 
#include <stack>


template <class _Tp> class Node; ///전방 선언
template <class _Tp> class BST;


template <class _Tp> /// _Tp는 타입형
class Node {
	public: // Member types
		typedef _Tp					__key_type;
		typedef Node<__key_type>*	__node_pointer_type;
	
	public: // Member variables
		__key_type			__key_;
		__node_pointer_type	__left_;
		__node_pointer_type	__right_;
		unsigned height; // Node의 높이 새로 선언
		unsigned size; // Node의 크기 새로 선언
	
	public: // Constructor
		Node(): __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(1), size(1) {}
		Node(const __key_type& key): __key_(key), __left_(nullptr), __right_(nullptr), height(1), size(1) {}

};




// getNodeBST 함수, 새로 노드 생성해서 포인터로 반환하며 __insertBST 함수에서 쓰임
template <class _Tp>
Node<_Tp>* getNodeBST(_Tp key){
	return new Node<_Tp>(key);
}

//노드의 높이를 반환하며 __eraseBST 함수에서 쓰임
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) return 0;// 빈트리일 경우
	return __x->height;
}

// 특정 노드 __x의 사이즈를 반환 마찬가지로 __eraseBST에서 쓰임
template <class _NodePtr>
unsigned __size(_NodePtr __x) {

	if (__x == nullptr) // 빈트리일 경우
        return 0; 

	
	return __x->size;
} 


// 가장 좌측에 있는 node를 출력한다.
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	
	if (__x == nullptr) 
        return nullptr;

	_NodePtr p = __x;
	// p->__left가 null이 될때까지 반복해서 탐색한다.
	while(p->__left_!=nullptr){
		p=p->__left_;
	}
	return p;
}


// 가장 우측에 있는 node를 출력한다.
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	if (__x == nullptr) 
        return nullptr;

	_NodePtr p = __x;
	// p->__right_가 null이 될때까지 우측을 탐색한다.
	while(p->__right_!=nullptr){
		p=p->__right_;
	}
	return p;

}



template <class _NodePtr>
void __inorder(_NodePtr __x) {


	if(__x== nullptr)
	{
		return;
	}

	// __x의 좌하단 노드, __x, __x의 우하단 노드 순으로 탐색을 진행한다.
	// 재귀를 이용해 탐색을 하면 전체 노드를 좌하단 -> 중간 -> 우하단 순으로 탐색하게 된다.
	std::cout<<"<";
	__inorder(__x->__left_);
	std::cout<<" "<<__x->__key_<<" ";
	__inorder(__x->__right_);
	std::cout<<">"; 

}


/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp> /// 처음 호출될 때 루트를 __root로 넘겨받는다.
std::pair<_NodePtr, bool> __insertBST(_NodePtr __root, const _Tp& key) { 
	
	// p는 삽입하려는 노드, q는 p의 부모노드, Pair는 반환 값으로 사용한다.
	_NodePtr p = __root;
	_NodePtr q = nullptr;
	std::pair<_NodePtr, bool> Pair;

	std::stack<_NodePtr> insertStack; // 삽입 경로에 있는 노드의 높이와 크기를 정정할 때 쓰인다.
	while(p!=nullptr){
		if(key==p->__key_){
			Pair.first = p;                   
			Pair.second = false; // 이미 있는 값을 삽입하려고 하였으니 false를 반환한다.
			return Pair; // 같은 값을 갖는 노드의 포인터, value 반환
		}
		q=p;
		insertStack.push(q); // 삽입 경로를 기록하기 위해 stack에 push


		// 현재 p의 노드보다 __key_가 더 작으면 p는 좌하단 노드로 이동, 아니면 우하단 노드로 이동
		if(key < p->__key_)
		{
			p=p->__left_;
		}
		else
		{
			p=p->__right_;
		}
	}
	//key를 key값으로 갖는 새로운 노드 생성하고 그 node를 p가 가르키게 한다.
	p = getNodeBST(key);

	// __root가 nullptr라는 건 새로 삽입하려는 node가 root 노드가 된다는 것.
	if(__root == nullptr)
	{
		__root=p;
		Pair.first = p;                  
		Pair.second = true;

	}else if(key<q->__key_){ //q(p의 부모노드)의 key값보다 key가 더 작으면 p는 q의 좌하단에 위치하게 된다.
		q->__left_=p;
		Pair.first = q->__left_;                  
		Pair.second = true;
	}else{ //q(p의 부모노드)의 key값보다 key가 더 크면 p는 q의 우하단에 위치하게 된다.
		q->__right_=p;
		Pair.first = q->__right_;                  
		Pair.second = true;
	}
	
	while(!insertStack.empty()){
		q=insertStack.top();
		insertStack.pop();
		int childHeight; // q노드의 높이를 갱신하는데 이 자식 노드의 높이를 이용한다.
		
		if(q->__left_!=nullptr && q->__right_!=nullptr) //q의 자식 노드가 두 개 다 있는 경우
		{
			// 좌하단 노드가 크면 childHeight는 좌하단 노드
			if(q->__left_->height>=q->__right_->height)
				childHeight=q->__left_->height;
			else
				childHeight=q->__right_->height;

			// q의 size값은 좌하단 노드와 우하단 노드와 자기자신의 단일 size값(1)을 모두 더한 것.
			q->size=1+q->__left_->size+q->__right_->size;

		}else if(q->__left_==nullptr && q->__right_!=nullptr){//q의 자식 노드가 좌측에만 있는 경우
			{
				childHeight=q->__right_->height; // childHeight는 우하단노드의 높이
				q->size=1+q->__right_->size; // q의 size는 우하단 노드의 size+1
			}
		}else if(q->__right_==nullptr && q->__left_!=nullptr){//q의 자식 노드가 우측에만 있는 경우
			{
				childHeight=q->__left_->height; // childHeight는 좌하단노드의 높이
				q->size=1+q->__left_->size; // q의 size는 좌하단 노드의 size+1
			}
		}else{
			childHeight=0; //자식노드가 없다면 childHeight=0
			q->size=1; // 이때 q의 size는 단일 노드 크기 1
		}

		q->height=1+childHeight; // q의 높이는 childHeight값에 자신의 단일 높이(1)을 더한 것
		
	}
	return Pair;

}



template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	// p는 삭제하려는 노드, q는 p의 부모노드
	_NodePtr p = __root;
	_NodePtr q = nullptr;

	// 삭제 경로에 있는 노드의 높이와 크기를 정정할 때 쓰인다.
	std::stack<_NodePtr> eraseStack;

	// 삭제할 노드를 찾았거나, 끝까지 탐색했는데 못찾는 경우 루프를 나온다.
	while(p!=nullptr && key!=p->__key_){
		q=p;
		eraseStack.push(q);// 삭제 경로를 기록하기 위해 stack에 push

		// key가 현재 p의 값보다 작으면 p는 좌하단 노드로 이동, 아니면 우하단 노드로 이동.
		if(key<p->__key_)
		{
			p=p->__left_; 
		}
		else
		{
			p=p->__right_;
		}
	}

	// 삭제할 노드를 찾지 못하면 return nullptr;
	if(p==nullptr)
		return nullptr;

	//fixedP는 삭제할 노드의 위치
	_NodePtr fixedP = p;
	if(p->__left_ != nullptr && p->__right_ !=nullptr){ //삭제하려는 p노드의 두 자식 노드가 있는 경우

		int flag = 0; //flag값에 따라 좌하단 subtree의 max값을 삭제할 지, 우하단 subtree의 min값을 삭제할 지 결정
		_NodePtr tmp; //삭제하려는 노드
		eraseStack.push(p); //p가 삭제 경로에 있으므로 p를 stack에 삽입

		if(__height(p->__left_) > __height(p->__right_) || ( (__height(p->__left_) == __height(p->__right_)) && (__size(p->__left_)>=__size(p->__right_)))){
			{
				// 좌하단 subtree의 높이가 더 크거나 두 자식노드의 heigt가 같지만 좌하단 subtree의 노드 수가 더 많으면 실행
				tmp = __tree_max(p->__left_); // 좌하단 subtree의 최댓값
				flag=-1;
				p=p->__left_;
				eraseStack.push(p); 
				
				// 좌하단 subtree의 최댓값을 찾는 경로를 stack에 저장한다.
				while (p->__right_ != nullptr){
					p = p->__right_;
					eraseStack.push(p);
				}
			}
		} else {
			{
				// 우하단 subtree의 최댓값으로 동일 실행
				tmp = __tree_min(p->__right_);
				flag=1;
				p=p->__right_;
				eraseStack.push(p);

				while (p->__left_ != nullptr){
					p = p->__left_;
					eraseStack.push(p);
				}
			}
		}

		// 원래 삭제하려는 p노드에 key값을 tmp의 key값으로 바꾼다.
		fixedP->__key_= tmp->__key_;
		if (flag == -1) { //fixedP의 좌하단 노드의 최댓값 삭제
			fixedP = __eraseBST(fixedP->__left_, tmp->__key_);
			
		} else if (flag ==1){ //fixedP의 우하단 노드의 최댓값 삭제
			fixedP = __eraseBST(fixedP->__right_, tmp->__key_);
			
		}

	}
	else if(p->__left_ != nullptr && p->__right_ ==nullptr){ //삭제하려는 p노드가 좌하단 자식 노드만 있는 경우
		if(q==nullptr) // p의 부모노드가 nullptr이다 즉 p가 root node이다.
			__root=p->__left_; //새로운 root 노드는 p의 좌하단 노드.
		else if(q->__left_ == p) //p가 q의 좌하단 노드인 경우
			q->__left_=p->__left_; //새로운 q의 좌하단 노드는 p의 좌하단 노드이다.(p를 삭제하기 때문에)
		else //p가 q의 우하단 노드인 경우
			q->__right_ = p->__left_; //새로운 q의 p의 좌하단 노드는 p의 우하단 노드이다.(p를 삭제하기 때문에)
	}
	else if(p->__left_ == nullptr && p->__right_ !=nullptr){ //삭제하려는 p노드가 우하단 자식 노드만 있는 경우
		
		if(q==nullptr) // p의 부모노드가 nullptr이다 즉 p가 root node이다.
		{
			__root=p->__right_;//새로운 root 노드는 p의 우하단 노드.
		}
		// 아래 경우 들은 위케이스와 동일 다만 q의 자식 노드에는 p의 우하단 노드가 새로 설정된다.
		else if(q->__left_ == p){ 
			q->__left_=p->__right_;
		}
		else{
			q->__right_ = p->__right_; 
		}
			
	}
	else if(p->__left_ == nullptr && p->__right_ ==nullptr){
		if(q==nullptr) // p의 부모노드가 nullptr이다 즉 p가 root node이다.
			__root=nullptr; //p가 root node이고 p의 자식 노드도 없으니 __root는 nullptr가 된다.
		else if(q->__left_ == p) //p노드(q의 좌하단 노드)의 자식노드가 없고, p노드를 삭제하니
			q->__left_=nullptr; // q의 좌하단 노드는 nullptr가 된다.
		else//p노드(q의 우하단 노드)의 자식노드가 없고, p노드를 삭제하니
			q->__right_ = nullptr;// q의 우하단 노드는 nullptr가 된다.
			
	}


	//__insertBST의 while문과 동일
	while(!eraseStack.empty()){
		q=eraseStack.top();
		eraseStack.pop();
		
		
		int childHeight=0; 

		
		if(q!=nullptr){
		if(q->__left_!=nullptr && q->__right_!=nullptr)
		{
			if(q->__left_->height>=q->__right_->height)
				childHeight=q->__left_->height;
			else
				childHeight=q->__right_->height;


			q->size=1+q->__left_->size+q->__right_->size;
		}
		else if(q->__left_==nullptr && q->__right_!=nullptr){
			childHeight=q->__right_->height;
			q->size=1+q->__right_->size;
		}
		else if(q->__right_==nullptr && q->__left_!=nullptr){
			childHeight=q->__left_->height;
			q->size=1+q->__left_->size;
		}
		else{
			childHeight=0;
			q->size=1;
		}
		q->height=1+childHeight;
		}


	}
	
	return fixedP;
}

// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// __x가 nullptr이라면 바로 반환
	if(__x == nullptr)
		return;
	
	// 재귀를 이용해 __x의 좌하단 노드부터 처리하고 우하단 노드를 제거한 다음, __x를 제거한다.
	__clear(__x->__left_); 
	__clear(__x->__right_);

	delete __x; // 해당 __x 노드 메모리 해제
	__x = nullptr; //댕글링 방지
}


template <class _Tp>
class BST {
	public: // Member types
		typedef _Tp						key_type;
		typedef std::size_t				size_type;
		typedef Node<key_type>*			pointer;
		typedef const Node<key_type>*	const_pointer;
	
	private: // Member variables
		pointer	__root_;
	
	public: // Constructor
		BST(): __root_(nullptr) {}
	

	public: // Capacity
		size_type height() const {

			//__height 함수 이용
			return __height(__root_);
		}

		size_type size() const {
			//__size 함수 이용
			return __size(__root_);
		}
	
	public: // Lookup
		void inorder() const {
			//__inorder 함수 이용
			__inorder(__root_);
			std::cout << std::endl;
		}
		
	public: // Modifier
		std::pair<const_pointer, bool> insert(const key_type& key) {
			// __insertBST 함수 이용
			
			std::pair<pointer, bool> Pair;
			Pair = __insertBST(__root_, key);
			if(__root_==nullptr) //만약 BST의 __root가 nullptr라면 
				__root_=Pair.first; //__root를 새로 설정한다.
			return Pair;
		}

		const_pointer erase(const key_type& key) {
			// __eraseBST 함수 이용
			pointer __r = __eraseBST(__root_, key);

			//삭제한 노드 메모리 해제
			delete __r;

			return __r;  
		}

		void clear() {
			// __clear 함수 이용
			__clear(__root_);
		}
	
	/*
	* 아래는 inorder traversal을 대체할 수 있는 operator<< 입니다.
	* 반드시 아래의 함수를 사용해야할 필요는 없습니다.
	*/
	friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
		os << tree.__root_;
		return os;
	}
};


int main() {//int argc, char **argv 필요 없음
	BST<int>	tree;
	char		command;
	int			key;

while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
			// i라면 tree에 새로운 노드를 추가한 다음 inorder 순회 결과를 출력하고
			// 만약 key값이 이미 있다면 std::cerr을 출력한다.
				if (tree.insert(key).second == false) {
					std::cout << "i " << key << ": The key already exists" << std::endl; 
					continue;
				}
				else{
					tree.inorder();
				}
				break;
			case (int)'d':
			// d라면 tree에 해당 key를 갖는 노드를 삭제한 다음 inorder 순회 결과를 출력하고
			// 만약 key값이 없다면 std::cerr을 출력한다.
				if (tree.erase(key) == nullptr) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl; 
					continue;
				}
				else{
					tree.inorder();
				}
				break;
			default:
			// 그밖의 명령어라면 유효하지 않으므로 std::cerr을 출력한다.
				std::cerr << "Invalid command: " << command << std::endl; 
				return (1);
				break;
		}

}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();

return (0);
}