/*
 * 20223578_나비예브_압두마리크_BST.cpp
 * 리포트: Report 1 - 이진 탐색 트리 (Binary Search Tree)
 */

// 입출력(I/O)과 메모리 할당을 위해 필요한 라이브러리입니다.
#include <iostream> 
#include <fstream>  // 파일 입출력을 위해 필요합니다.
#include <string>   
#include <new>      // 'new' 와 'delete' (메모리 할당)를 위해 필요합니다.

template <class _Tp>
class Node {
public: // Member types
    typedef _Tp                __key_type;
    typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
    __key_type          __key_;
    __node_pointer_type __left_;
    __node_pointer_type __right_;
    
public: // Constructor
    Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
    Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}
};

template <class _Tp>
class BST {
public: // Member types
    typedef _Tp                key_type;
    typedef Node<key_type>     node_type;
    typedef node_type* pointer;

private:
    pointer __root_;

    /**
     * @brief 트리에 새 노드를 넣는 재귀 함수입니다.
     * @param node 지금 노드 포인터.
     * @param key 넣고 싶은 키.
     * @param inserted 성공하면 true가 됩니다.
     * @return 트리의 새 루트를 리턴합니다.
     */
    pointer __insertBST(pointer node, key_type key, bool& inserted) {
        // 기본 경우: 노드를 넣을 위치를 찾았습니다.
        if (node == nullptr) {
            inserted = true;
            return new node_type(key);
        }

        // 재귀 경우: 올바른 위치를 찾습니다.
        if (key < node->__key_) {
            node->__left_ = __insertBST(node->__left_, key, inserted);
        } else if (key > node->__key_) {
            node->__right_ = __insertBST(node->__right_, key, inserted);
        } else {
            // 만약 키가 같습니다 (key == node->__key_).
            // 'inserted'는 'false' 입니다. (바뀌지 않습니다)
            // 이것은 "already exists" 경우입니다.
        }
        return node;
    }

    /**
     * @brief 오른쪽 서브트리에서 가장 작은 노드를 찾습니다. (in-order successor)
     */
    pointer __findMin(pointer node) {
        while (node && node->__left_ != nullptr) {
            node = node->__left_;
        }
        return node;
    }

    /**
     * @brief 트리에서 노드를 지우는 재귀 함수입니다.
     * Report1.pdf 예시를 보면, 자식이 2개일 때
     * "in-order successor" (오른쪽에서 가장 작은 노드)를 사용합니다.
     * @param node 지금 노드.
     * @param key 지우고 싶은 키.
     * @param deleted 성공하면 true가 됩니다.
     * @return 트리의 새 루트를 리턴합니다.
     */
    pointer __eraseBST(pointer node, key_type key, bool& deleted) {
        // 기본 경우: 키를 못 찾았습니다.
        if (node == nullptr) {
            return nullptr;
        }

        // 재귀 경우: 지울 노드를 찾습니다.
        if (key < node->__key_) {
            node->__left_ = __eraseBST(node->__left_, key, deleted);
        } else if (key > node->__key_) {
            node->__right_ = __eraseBST(node->__right_, key, deleted);
        } 
        // 노드를 찾았습니다 (key == node->__key_)
        else {
            deleted = true; // 키를 찾았습니다. 지우기 시작합니다.

            // 1번 경우: 오른쪽 자식이 없습니다. (자식 0개 또는 왼쪽 자식 1개)
            if (node->__right_ == nullptr) {
                pointer temp = node->__left_;
                delete node; // 노드 지우기
                return temp;
            }
            // 2번 경우: 왼쪽 자식이 없습니다. (오른쪽 자식 1개)
            else if (node->__left_ == nullptr) {
                pointer temp = node->__right_;
                delete node; // 노드 지우기
                return temp;
            }

            // 3번 경우: 자식이 2개 있습니다.
            // Report1.pdf (7페이지, 'd 25' 예시)처럼 "in-order successor"를 찾습니다.
            pointer succ = __findMin(node->__right_);

            // successor의 키를 이 노드에 복사합니다.
            node->__key_ = succ->__key_;

            // 오른쪽 서브트리에서 successor 노드를 지웁니다.
            // (이것도 재귀입니다. 'deleted'가 다시 true가 되지만 괜찮습니다.)
            node->__right_ = __eraseBST(node->__right_, succ->__key_, deleted);
        }
        return node;
    }

    /**
     * @brief 'p' 명령어를 위해 Pre-order (루트-왼쪽-오른쪽) 순서로 출력합니다.
     */
    void __printPreOrder(pointer node, std::ofstream& outFile) {
        if (node == nullptr) {
            return;
        }
        outFile << node->__key_ << " "; // 루트
        __printPreOrder(node->__left_, outFile);  // 왼쪽
        __printPreOrder(node->__right_, outFile); // 오른쪽
    }

    /**
     * @brief 'q' 명령어를 위해 In-order (왼쪽-루트-오른쪽) 특별한 모양으로 출력합니다.
     */
    void __printPrettyInOrder(pointer node, std::ofstream& outFile) {
        if (node == nullptr) {
            return; // 'null'은 출력하지 않습니다.
        }

        outFile << "<";
        
        // 왼쪽 부분 (있으면, 재귀하고 스페이스)
        if (node->__left_ != nullptr) {
            __printPrettyInOrder(node->__left_, outFile);
            outFile << " ";
        }

        // 루트
        outFile << node->__key_;

        // 오른쪽 부분 (있으면, 스페이스하고 재귀)
        if (node->__right_ != nullptr) {
            outFile << " ";
            __printPrettyInOrder(node->__right_, outFile);
        }

        outFile << ">";
    }

    /**
     * @brief 모든 노드를 메모리에서 지웁니다 (Post-order: 왼쪽-오른쪽-루트).
     */
    void __clear(pointer node) {
        if (node == nullptr) {
            return;
        }
        __clear(node->__left_);
        __clear(node->__right_);
        delete node; // 노드 지우기
    }

public:
    // 생성자
    BST() : __root_(nullptr) {}

    // 소멸자 (메모리 청소)
    ~BST() {
        clear();
    }

    /**
     * @brief public 'insert' 함수입니다.
     * @return 'true' - 넣기 성공, 'false' - 이미 있습니다.
     */
    bool insert(key_type key) {
        bool inserted = false;
        __root_ = __insertBST(__root_, key, inserted);
        return inserted;
    }

    /**
     * @brief public 'erase' 함수입니다.
     * @return 'true' - 지우기 성공, 'false' - 키가 없습니다.
     */
    bool erase(key_type key) {
        bool deleted = false;
        __root_ = __eraseBST(__root_, key, deleted);
        return deleted;
    }

    /**
     * @brief 메모리를 모두 청소하는 public 함수입니다.
     */
    void clear() {
        __clear(__root_);
        __root_ = nullptr;
    }

    /**
     * @brief 'p' 명령어를 위한 public 함수입니다.
     */
    void printPreOrder(std::ofstream& outFile) {
        if (__root_ == nullptr) {
            outFile << "\n"; // 트리가 비어있으면, 새 줄을 출력합니다.
            return;
        }
        __printPreOrder(__root_, outFile);
        outFile << "\n"; // 'p' 명령어 끝에 새 줄을 출력합니다.
    }

    /**
     * @brief 'q' 명령어를 위한 public 함수입니다.
     */
    void printPrettyInOrder(std::ofstream& outFile) {
        if (__root_ == nullptr) {
            outFile << "\n"; // 트리가 비어있으면, 새 줄을 출력합니다.
            return;
        }
        __printPrettyInOrder(__root_, outFile);
        outFile << "\n"; // 마지막 결과 끝에 새 줄을 출력합니다.
    }
};
int main(int argc, char* argv[]) {
    //    프로그램 이름 (argv[0]), 입력 파일 (argv[1]), 출력 파일 (argv[2])
    if (argc != 3) {
        // 에러 (콘솔에 출력합니다. 왜냐하면 출력 파일 이름을 모릅니다.)
        std::cerr << "사용법: " << argv[0] << " <input_file> <output_file>\n";
        return 1; // 에러 코드로 종료합니다.
    }

    // 2. 파일 열기
    std::ifstream inFile(argv[1]);
    if (!inFile.is_open()) {
        std::cerr << "에러: 입력 파일 " << argv[1] << "을 열 수 없습니다.\n";
        return 1;
    }

    std::ofstream outFile(argv[2]);
    if (!outFile.is_open()) {
        std::cerr << "에러: 출력 파일 " << argv[2] << "을 열 수 없습니다.\n";
        inFile.close();
        return 1;
    }

    // 3. BST 객체 만들기
    BST<int> tree;

    // 4. 명령어 읽고 실행하기 (Report1.pdf, 4페이지)
    char command;
    int key;

    while (inFile >> command) {
        
        switch (command) {
            // 'i' (insert) 명령어
            case 'i':
                inFile >> key;
                if (tree.insert(key)) {
                    outFile << key << " is inserted.\n";
                } else {
                    outFile << key << " already exists.\n";
                }
                break;

            // 'd' (delete) 명령어
            case 'd':
                inFile >> key;
                if (tree.erase(key)) {
                    outFile << key << " is deleted.\n";
                } else {
                    outFile << key << " is not available.\n";
                }
                break;

            // 'p' (print) 명령어
            case 'p':
                tree.printPreOrder(outFile);
                break;

            // 'q' (quit) 명령어
            case 'q':
                tree.printPrettyInOrder(outFile);
                // 메모리 청소는 tree 소멸자(~BST())가 자동으로 합니다.
                // 'break'를 사용해서 루프를 나갑니다.
                goto end_loop; // 'while' 루프를 멈춥니다.
                break;

            default:
                break;
        }
    }

end_loop:

    // 5. 파일 닫고 프로그램 끝내기
    inFile.close();
    outFile.close();

    // 'tree' 객체가 여기서 사라집니다 (out of scope).
    // 이때 소멸자 (~BST())가 불리고, __clear() 함수가 모든 메모리를 지웁니다.
    
    return 0;

}