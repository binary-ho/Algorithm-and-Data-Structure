// 구현중 현재 insertion까지 구현
#ifndef JINHO_BPLUSTREE_H
#define JINHO_BPLUSTREE_H


/*
 * class Node: B+ Tree에 쓰일 노드
 * 엔트리 <key, childPtr>를 내포하고 있다.
 * size는 엔트리를 몇 개나 가지고 있는지 알려준다.
 * isLeaf를 통해 리프 노드인지, 비단말 노드인지 확인 가능
 * */
class Node {
    friend class BPlusTree;
private:
    int *key, size;
    Node **childPtr;
    bool isLeaf;

public:
    Node();
    Node(int K);
};

/* class BPlusTree: B+Tree 클래스
 * root의 포인터를 가지고 있고, 탐색, 삽입, 제거 커멘드를 제공.
 * 내부적으로는 추가적인 부모 노드 탐색, 삽입, 삭제, 루트 노드 찾기를 제공.
 * 교재에 따르면 트리의 모든 노드는 order d에 대해  m개의 엔트리를 가져야 한다. d <= m <= 2d
 * 상수 MAX는 가질 수 있는 엔트리의 상한으로, 2d + 1의 값이다.
 * */
class BPlusTree {
    friend class Node;  // for MAX
private:
    Node *root;
    void insertInternal(int, Node *, Node *);
    void removeInternal(int, Node *, Node *);
    Node *findParent(Node *, Node *);
    Node *getRoot() { return root; }

public:
    BPlusTree(int order = 1) : MAX(2*order + 1), root(nullptr) {}
    void serch(int);
    void insert(int);
    void remove(int);
    const int MAX;

} tree;


/*
 * 노드의 생성자 MAX개의 key값을 갖는 배열과 MAX+1 ptr를 갖는 배열을 만든다.
 * */
Node::Node() {
    key = new int[tree.MAX];
    childPtr = new Node *[tree.MAX + 1];
}

// 새롭게 하나만 딱 만들어지는 상황에 쓸 예정
Node::Node(int K) {
    Node();
    key[0] = K;
    isLeaf = true;
    size = 1;
}

/*
 * K 값을 insert
 * */
void BPlusTree::insert(int K) {
    if (root == nullptr) {
        root = new Node(K);
        return;
    }
    Node *ptr = root;
    Node *parent;
    bool findNext = false;
    while (!ptr->isLeaf) {
        findNext = false;
        parent = ptr;
        for (int i = 0; i < ptr->size; i++) {
            if (K >= ptr->key[i]) continue;
            // if K < ptr->buffer[i]인 경우
            ptr = ptr->childPtr[i];
            findNext = true;
            break;
        }
        if (!findNext) ptr = ptr->childPtr[ptr->size];
    }

    // 현재 가르키는 노드가 꽉찼는지 아닌지 확인한다.
    if (ptr->size < MAX) {
        int idx = 0;

        // K값이 들어갈 위치 찾기기
        while (K > ptr->key[idx] && idx < ptr->size) idx++;

        // idx에 K가 들어가기 위해 key들을 한 칸씩 옮긴다.
        for (int i = ptr->size; i > idx; i--) ptr->key[i] = ptr->key[i - 1];

        // 삽입
        ptr->key[idx] = K;
        ptr->size++;
        ptr->childPtr[ptr->size] = ptr->childPtr[ptr->size - 1];
        ptr->childPtr[ptr->size - 1] = nullptr;
        return;
    }

    // 꽉 찬 경우
    // buffer key들 받아와서 K 삽입한 이후 분할할 것이다.
    // buffer에 삽입
    int buffer[MAX + 1];
    for (int i = 0; i < MAX; i++) buffer[i] = ptr->key[i];
    int idx = 0;
    while (K > buffer[idx] && idx < MAX) idx++;
    for (int i = MAX + 1; i > idx; i--) buffer[i] = buffer[i - 1];
    buffer[idx] = K;


    /*
     * 조금 복잡할 수 있다. 천천히 설명
     * 새로 노드를 만든 다음, 반을 나눈다.
     * 기존 노드의 끝 포인터는 새 노드를 가리키고
     * 새 노드의 끝은 기존 노드가 원래 가리키던 다음 노드를 가리키게 만들어준다.
     * */
    Node *newNode = new Node;
    newNode->isLeaf = true;
    ptr->size = (MAX + 1)/2;
    newNode->size = MAX + 1 - ptr->size;
    ptr->childPtr[ptr->size] = newNode; // 새로 만든 노드를 가리킨다
    newNode->childPtr[newNode->size] = ptr->childPtr[MAX];
    ptr->childPtr[MAX] = nullptr;

    // 내용물을 두 노드에 나누어 넣어준다.
    for (int i = 0; i < ptr->size; i++) ptr->key[i] = buffer[i];
    for (int i = 0, j = ptr->size; i < newNode->size; i++, j++) newNode->key[i] = buffer[j];

    // 분할한 다음 엔트리 하나가 위로 올라가게 된다..
    if (ptr == root) {
        Node *newRoot = new Node(newNode->key[0]);
        newRoot->childPtr[0] = ptr;
        newRoot->childPtr[1] = newNode;
        newRoot->isLeaf = false;
        root = newRoot;
    } else {
        insertInternal(newNode->key[0], parent, newNode);
    }
}

// 처음 insert는 단말 노드에 집중한 것이고, 내부 insert는 비단말 노드들의 insert
void BPlusTree::insertInternal(int K, Node *ptr, Node *childNode) {
    // 위에서 꽉차있지 않은 노드에 K값 엔트리를 넣는 과정과 유사하다.
    if (ptr->size < MAX) {
        int idx = 0;
        while (K > ptr->key[idx] && idx < ptr->size) idx++;
        for (int i = ptr->size; i > idx; i--) ptr->key[i] = ptr->key[i - 1];
        ptr->key[idx] = K;
        ptr->size++;
        ptr->childPtr[idx + 1] = childNode;
        return;
    }

    // 꽉찬 경우
    // 이번에는 key값들 뿐만 아니라 ptr도 복사해야한다.
    int keyBuffer[MAX + 1];
    Node *childPtrBuffer[MAX + 2];
    for (int i = 0; i < MAX; i++) keyBuffer[i] = ptr->key[i];
    for (int i = 0; i < MAX + 1; i++) childPtrBuffer[i] = ptr->childPtr[i];

    int idx = 0;
    while (K > keyBuffer[idx] && idx < MAX) idx++;
    for (int i = MAX + 1; i > idx; i--) keyBuffer[i] = keyBuffer[i - 1];
    keyBuffer[idx] = K;

    for (int i = MAX + 2; i > idx + 1; i--) childPtrBuffer[i] = childPtrBuffer[i - 1];
    childPtrBuffer[idx + 1] = childNode;

    Node *newInternalNode = new Node;
    newInternalNode->isLeaf = false;
    ptr->size = (MAX + 1) / 2;
    newInternalNode->size = MAX - ptr->size;
    for (int i = 0, j = ptr->size + 1; i < newInternalNode->size; i++, j++) newInternalNode->key[i] = keyBuffer[j];
    for (int i = 0, j = ptr->size + 1; i < newInternalNode->size + 1; i++, j++) newInternalNode->childPtr[i] = childPtrBuffer[j];

    if (ptr == root) {
        Node *newRoot = new Node(ptr->key[ptr->size]);
        newRoot->childPtr[0] = ptr;
        newRoot->childPtr[1] = newInternalNode;
        newRoot->isLeaf = false;
        root = newRoot;
    } else {
        insertInternal(ptr->key[ptr->size], findParent(root, ptr), newInternalNode);
    }
}

// 루트 부터 거슬러 내려와서 child node의 부모 노드를 찾는다.
Node *BPlusTree::findParent(Node *ptr, Node *childNode) {
    // 시작 부터 리프거나 리프의 부모인 경우 pass
    if (ptr->isLeaf || ptr->childPtr[0]->isLeaf) return nullptr;

    Node *parent;
    for (int i = 0; i < ptr->size + 1; i++) {
        if (ptr->childPtr[i] == childNode) return ptr;
        parent = findParent(ptr->childPtr[i], childNode);
        if (parent != nullptr) return ptr;
    }
    return parent;
}
#endif //JINHO_BPLUSTREE_H
