#ifndef JINHO_BPLUSTREE_H
#define JINHO_BPLUSTREE_H

/*
 * class Node: B+ Tree에 쓰일 노드
 * 엔트리 <key, ptr>를 내포하고 있다.
 * size는 엔트리를 몇 개나 가지고 있는지 알려준다.
 * isLeaf를 통해 리프 노드인지, 비단말 노드인지 확인 가능
 * */
class Node {
    friend class BPlusTree;
private:
    int *key, size;
    Node **ptr;
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

#endif //JINHO_BPLUSTREE_H
