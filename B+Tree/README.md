# B+Tree
# 이 글 읽지말고 이거 읽으세요 -> [코드로 이해하는 B+Tree 연산 과정](https://github.com/binary-ho/Algorithm-and-Data-Structure/blob/main/B%2BTree/%EC%BD%94%EB%93%9C%EB%A1%9C%20%EC%9D%B4%ED%95%B4%ED%95%98%EB%8A%94%20B%2BTree%20%EC%97%B0%EC%82%B0%EA%B3%BC%EC%A0%95.md)
B+Tree는 DBMS에서 트리 기반 인덱싱에 사용하는 자료구조이다.
트리 기반 인덱싱과 B+ Tree에 대한 기본 설명은 ~~[이곳을 참고]~~
<!--- (https://github.com/binary-ho/TIL-public/blob/main/3%ED%95%99%EB%85%84%202%ED%95%99%EA%B8%B0/DB/10.%20%ED%8A%B8%EB%A6%AC%20%EA%B5%AC%EC%A1%B0%20%EC%9D%B8%EB%8D%B1%EC%8B%B1.md) --->
<br> <br>

이 곳에서는 구체적인 코드와 설명을 해보려고 한다. <br>
여러 사이트를 참고하고, 학교에서 DB 수업을 진행하는 교재의 변수나 제한 사항들을 최대한 살려 보려고 노력했다. <br>
전체적으로 읽는 사람이 최대한 이해하기 쉽게, 그리고 읽기 좋게, 간단하게 리팩토링 했다. 그리고 각 코드 라인마다 헷갈릴 수 있는 부분은 최대한 설명하려고 노력했다. <br>
그래도 기본적인 트리 구조나 B+Tree의 흐름에 대한 이해는 있다고 생각하고 설명하였다. <br>
c++ 컨벤션을 지키려 노력했다.

## 1. Node class
```cpp
/*
 * class Node: B+ Tree에 쓰일 노드
 * 엔트리 <key, 자식 노드를 가리키는 pointer>를 내포하고 있다.
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
```
노드 클래스이다. 
1. 트리 함수에서 노드에 접근할 일은 정말 많다. 이에 friend class 선언을 해주었다.
2. key와 다음 노드를 가리키는 pointer는 포인터의 형태로 되어있다. 여러 값들을 가지게 하기 위해서이다.
3. size는 말 그대로 size. key의 갯수가 되겠다. 당연히 포인터는 하나가 더 많다.
4. isLeaf는 단말 노드인지 확인을 위한 변수이다.
5. 생성자가 2개인데, 변수 하나만 넣어줬을 떄의 생성자를 만들어 줘 봤다.

## 2. B+Tree class
```cpp
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
```
트리 클래스
1. B+Tree의 노드들은 각 노드별로 가질 수 있는 엔트리 갯수에 제한을 갖는다. 이에 Tree 자체에서 갯수 제한 상수 `MAX`를 가지게 하였고, `Node`에서 `MAX`에 접근하기 위해 friend class 선언 해주었다.
2. 교재의 정의에 따르면 order d에 대해서 하나의 노드는 m개의 엔트리를 가질 수 있다고 하였다. `d <= m <= 2d` <br> 이에 생성 시점에서 order를 입력으로 받아 상수 MAX를 초기화 해주었다. MAX는 `2d+1`로 노드가 가질 수 있는 엔트리의 상한이다.
3. public으로 공개한 메서드들은 단말 노드에 대한 연산이 대부분이다. 그리고 private로 선언안 것들은 보통 비단말노드간의 연산을 위해 존재한다.
4. 기본적으로 탐색, 삽입, 삭제의 메서드를 제공하고, 내부적으로는 단말 부터 타고 올라가는 연쇄적인 삽입, 삭제를 위해 부모 노드를 찾는 메서드가 있다.
5. 마지막 줄을 보면 tree라는 이름으로 하나 만들었다.

## 3. Node class 생성자
```cpp
/*
 * 노드의 생성자 MAX개의 key값을 갖는 배열과 MAX+1 ptr를 갖는 배열을 만든다.
 * */
Node::Node() {
    key = new int[tree.MAX];
    ptr = new Node *[tree.MAX + 1];
}

// 새롭게 하나만 딱 만들어지는 상황에 쓸 예정
Node::Node(int K) {
    Node();
    key[0] = K;
    isLeaf = true;
    size = 1;
}
```
Node의 생성자이다. <br>
1. MAX개의 key값 배열과 MAX + 1개의 child node를 가리키는 ptr를 갖는 ptr 배열을 만든다. (처음엔 childPtr로 만들었다가 의미상 수정)
2. K를 넣어주는 경우 K값을 가진 단말 노드를 만든다. 내가 편하려고 추가했다.

## 4. B+Tree Insert
```cpp
void BPlusTree::insert(int K) {
    if (root == nullptr) {
        root = new Node(K);
        return;
    }
    Node *cursor = root;
    Node *parent;
    bool findNext = false;
    while (!cursor->isLeaf) {
        findNext = false;
        parent = cursor;
        for (int i = 0; i < cursor->size; i++) {
            if (K >= cursor->key[i]) continue;
            // if K < cursor->buffer[i]인 경우
            cursor = cursor->ptr[i];
            findNext = true;
            break;
        }
        if (!findNext) cursor = cursor->ptr[cursor->size];
    }

    // 현재 가르키는 노드가 꽉찼는지 아닌지 확인한다.
    if (cursor->size < MAX) {
        int idx = 0;

        // K값이 들어갈 위치 찾기기
        while (K > cursor->key[idx] && idx < cursor->size) idx++;

        // idx에 K가 들어가기 위해 key들을 한 칸씩 옮긴다.
        for (int i = cursor->size; i > idx; i--) cursor->key[i] = cursor->key[i - 1];

        // 삽입
        cursor->key[idx] = K;
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;
        return;
    }

    // 꽉 찬 경우
    // buffer key들 받아와서 K 삽입한 이후 분할할 것이다.
    // buffer에 삽입
    int buffer[MAX + 1];
    for (int i = 0; i < MAX; i++) buffer[i] = cursor->key[i];
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
    cursor->size = (MAX + 1) / 2;
    newNode->size = MAX + 1 - cursor->size;
    cursor->ptr[cursor->size] = newNode; // 새로 만든 노드를 가리킨다
    newNode->ptr[newNode->size] = cursor->ptr[MAX];
    cursor->ptr[MAX] = nullptr;

    // 내용물을 두 노드에 나누어 넣어준다.
    for (int i = 0; i < cursor->size; i++) cursor->key[i] = buffer[i];
    for (int i = 0, j = cursor->size; i < newNode->size; i++, j++) newNode->key[i] = buffer[j];

    // 분할한 다음 엔트리 하나가 위로 올라가게 된다..
    if (cursor == root) {
        Node *newRoot = new Node(newNode->key[0]);
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newNode;
        newRoot->isLeaf = false;
        root = newRoot;
    } else {
        insertInternal(newNode->key[0], parent, newNode);
    }
}
```
insert를 구현한 메서드이다.
1. 루트인 경우 새로 만들어준 다음 삽입하고 연산을 끝낸다.
2. 루트가 아닌 경우 K값이 들어갈만한 위치를 루트로 부터 노드들을 타고 내려가며 탐색한다. 단말 노드에 도착할 때까지. 나중에 시간내서 search를 구현하는 것이 맞겠다.
3. 단말 노드를 찾은 다음 노드가 꽉차있는지 확인
4. 꽉 차있지 않은 경우 올바른 위치를 찾아 값을 삽입한다
5. 꽉 차있는 경우 분할을 위해 buffer를 만든다. buffer에 값들을 모두 옮기고 올바른 위치에 K값을 넣는다.
6. 새로운 노드를 만든 다음 버퍼를 이용해 **내용물을 기존 노드와 새로운 노드에 나눠준다.**
7. 기존 노드의 마지막 ptr은 새로운 노드를 가리킨다. (단말 끼리는 이어져 있으므로)
8. 이제 하나를 위로 올려야 하는데, 현재 가리키는 노드가 루트인 경우 편하게 하나를 만들어 새로운 루트로 지정해 위로 올린다.
9. 아닌 경우 비단말 노드의 insert를 위한 insertInternal을 호출한다.


## 5. B+Tree 비단말 노드 insert
```cpp
// 처음 insert는 단말 노드에 집중한 것이고, 내부 insert는 비단말 노드들의 insert
void BPlusTree::insertInternal(int K, Node *cursor, Node *childNode) {
    // 위에서 꽉차있지 않은 노드에 K값 엔트리를 넣는 과정과 유사하다.
    if (cursor->size < MAX) {
        int idx = 0;
        while (K > cursor->key[idx] && idx < cursor->size) idx++;
        for (int i = cursor->size; i > idx; i--) cursor->key[i] = cursor->key[i - 1];
        cursor->key[idx] = K;
        cursor->size++;
        cursor->ptr[idx + 1] = childNode;
        return;
    }

    // 꽉찬 경우
    // 이번에는 key값들 뿐만 아니라 ptr도 복사해야한다.
    int keyBuffer[MAX + 1];
    Node *childPtrBuffer[MAX + 2];
    for (int i = 0; i < MAX; i++) keyBuffer[i] = cursor->key[i];
    for (int i = 0; i < MAX + 1; i++) childPtrBuffer[i] = cursor->ptr[i];

    int idx = 0;
    while (K > keyBuffer[idx] && idx < MAX) idx++;
    for (int i = MAX + 1; i > idx; i--) keyBuffer[i] = keyBuffer[i - 1];
    keyBuffer[idx] = K;

    for (int i = MAX + 2; i > idx + 1; i--) childPtrBuffer[i] = childPtrBuffer[i - 1];
    childPtrBuffer[idx + 1] = childNode;

    Node *newInternalNode = new Node;
    newInternalNode->isLeaf = false;
    cursor->size = (MAX + 1) / 2;
    newInternalNode->size = MAX - cursor->size;
    for (int i = 0, j = cursor->size + 1; i < newInternalNode->size; i++, j++) newInternalNode->key[i] = keyBuffer[j];
    for (int i = 0, j = cursor->size + 1; i < newInternalNode->size + 1; i++, j++) newInternalNode->ptr[i] = childPtrBuffer[j];

    if (cursor == root) {
        Node *newRoot = new Node(cursor->key[cursor->size]);
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newInternalNode;
        newRoot->isLeaf = false;
        root = newRoot;
    } else {
        insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternalNode);
    }
}
```
위로 거슬러 올라가며 insert를 한다. <br>
전체적인 로직은 기본 insert와 비슷하다. <br>
다른 점은 노드를 분할-재분배 할떄인데, 이제는 자식 노드들을 가리키는 pointer들도 복사해서 나누어 주어야 한다는 점이다. 그리고 올라가게 되는 값은 중간 크기의 값이다 -> `cursor->key[cursor->size]`

## 6. 부모 노드 찾기
```cpp
// 루트 부터 거슬러 내려와서 child node의 부모 노드를 찾는다.
Node *BPlusTree::findParent(Node *cursor, Node *childNode) {
    // 시작 부터 리프거나 리프의 부모인 경우 pass
    if (cursor->isLeaf || cursor->ptr[0]->isLeaf) return nullptr;

    Node *parent;
    for (int i = 0; i < cursor->size + 1; i++) {
        if (cursor->ptr[i] == childNode) return cursor;
        parent = findParent(cursor->ptr[i], childNode);
        if (parent != nullptr) return cursor;
    }
    return parent;
}
```
부모 노드를 찾기 위한 메서드, 주석의 설명이 대부분이다. 아래 부분에서는 재귀적으로 부모 노드를 찾고 있다.

## 7. B+Tree Remove
이번엔 노드를 지우는 과정이다.
1. K값을 가진 말단 노드를 찾는다.
2. K값을 지운다.
3. 루트면 끝.
4. 아닌 경우 포인터도 조정.
5. 크기가 충분히 작지 않다면! 하한을 넘는다면 그대로 끝!
6. 아니라면.............
7. 부모 노드를 찾아 형제 노드를 확보
8. 왼쪽 형제 노드 먼저 확인을 해서 키 값의 수가 충분히 많다면 그냥 하나 뺏어오고 끝낸다
9. 아닌 경우 왼쪽 혹은 오른쪽 형제 노드에 몰아준다.
10. 그러면서 재귀적으로 다시 루트로 타고 올라가며 비단말 노드 remove 연산을 진행해준다.

복잡해 보이지만 실제 코드를 보며 자세한 동작을 보면 더 복잡하다. 확인해보자.
```cpp
void BPlusTree::remove(int K) {
    if (root == nullptr) return;
    Node *cursor = root;
    Node *parent;
    int leftSiblingIdx, rightSiblingIdx;

    /* 일단 루트로 부터 삭제 원소를 찾아 쭉 타고 내려간다. */
    bool findNext;
    while (!cursor->isLeaf) {
        findNext = false;
        int ptrSize = cursor->size;
        for (int ptrIdx = 0; ptrIdx < ptrSize; ptrIdx++) {
            parent = cursor;
            leftSiblingIdx = ptrIdx - 1;
            rightSiblingIdx = ptrIdx + 1;
            if (K < cursor->key[ptrIdx]) {
                cursor = cursor->ptr[ptrIdx];
                findNext = true;
                break;
            }
        }
        if (!findNext) {
            leftSiblingIdx = ptrSize - 1;
            rightSiblingIdx = ptrSize + 1;
            cursor = cursor->ptr[ptrSize];
        }
    }

    /* 지우려는 숫자가 말단노드에 존재하지 않다면 종료 */
    int position = -1;
    for (int i = 0; i < cursor->size; i++) if (cursor->key[i] == K) position = i;
    if (position == - 1) return;

    /* 발견했다면, 지우는 작업 시작. 한 칸씩 왼쪽으로 이동 */
    for (int i = position; i < cursor->size; i++) cursor->key[i] = cursor->key[i + 1];
    cursor->size--;

    /*
     * 이제 루트인지 확인하고,
     * 루트라면 트리 자체를 없애고 끝낸다. (메모리 반환)
     * 아닐 경우..
     * */
    if (cursor == root) {
        for (int i = 0; i < MAX + 1; i++) cursor->ptr[i] == nullptr;
        if (cursor->size == 0) {
            delete[] cursor->key;
            delete[] cursor->ptr;
            delete cursor;
            root = nullptr;
        }
        return;
    }

    /* 값 하나를 삭제했으니 한칸 땡긴다.*/
    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = nullptr;

    /* 크기가 충분히 작지 않으면 그대로 끝. */
    if (cursor->size >= (MAX + 1) / 2) return;

    /*
     * 작은 경우 양 쪽 sibling 노드를 들여다 본다.
     * 왼쪽 먼저 들여다 보는데, 왼쪽 노드의 크기가 최저 기준 보다 크다면 왼쪽에서 가져온다.
     * 아닐 경우 오른쪽에서 가져온다.
     * */
    Node *leftSibling = leftSiblingIdx >= 0 ? parent->ptr[leftSiblingIdx] : nullptr;
    if (leftSibling && leftSibling->size >= (MAX + 1) / 2 + 1) {
        /* 가져온 키를 맨 앞에 붙일 것이므로 자리를 한칸 만든다. */
        for (int i = cursor->size; i > 0; i--) cursor->key[i] = cursor->key[i - 1];
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;

        /* 왼쪽 노드 끝 key를 가져온다. */
        cursor->key[0] = leftSibling->key[leftSibling->size - 1];
        leftSibling->size--;
        leftSibling->ptr[leftSibling->size] = cursor;
        leftSibling->ptr[leftSibling->size + 1] = nullptr;
        parent->key[leftSiblingIdx] = cursor->key[0];
        return;
    }

    /* 이번엔 오른쪽 */
    Node *rightSibling = rightSiblingIdx <= parent->size ? parent->ptr[rightSiblingIdx] : nullptr;
    if (rightSibling && rightSibling->size >= (MAX + 1) / 2 + 1) {
        /* 오른 쪽에 붙이므로, 자리를 한칸 만들 필요가 없다. */
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;

        /* 오른쪽 노드 맨 앞 key를 가져온다. */
        cursor->key[cursor->size - 1] = rightSibling->key[0];
        rightSibling->size--;

        rightSibling->ptr[rightSibling->size] = rightSibling->ptr[rightSibling->size + 1];
        rightSibling->ptr[rightSibling->size + 1] = nullptr;
        for (int i = 0; i < rightSibling->size; i++) rightSibling->key[i] = rightSibling->key[i + 1];
        parent->key[rightSiblingIdx - 1] = rightSibling->key[0];
        return;
    }

    /*
     * 오른쪽 왼쪽이 다 최소 노드 갯수만 가지고 있는 경우..
     * 왼쪽이나 오른 쪽에 현재 노드의 원소들을 전부 옮겨준다!
     * */
    if (leftSibling) {
        /* 키값 모두 옮기고, 맨 끝 포인터 설정 다시 해주기 */
        for (int i = leftSibling->size, j = 0; j < cursor->size; i++, j++) leftSibling->key[i] = cursor->key[j];
        leftSibling->ptr[leftSibling->size] = nullptr;
        leftSibling->size += cursor->size;
        leftSibling->ptr[leftSibling->size] = cursor->ptr[cursor->size];    // 말단 노드가 가르키던 오른쪽 ptr 가져오기

        // 말단 노드 삭제에 맞춰 비단말 노드에서 기존 노드를 가리키던 엔트리 삭제 확인.
        removeInternal(parent->key[leftSiblingIdx], parent, cursor);

        // 메모리 반환
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete cursor;
        return;
    }

    if (rightSibling) {
        /* 이번엔 현재 노드에 몰아 넣어주기 */
        for (int i = cursor->size, j = 0; j < rightSibling->size; i++, j++) cursor->key[i] = rightSibling->key[j];
        cursor->ptr[cursor->size] = nullptr;
        cursor->size += rightSibling->size;
        cursor->ptr[cursor->size] = rightSibling->ptr[rightSibling->size];

        removeInternal(parent->key[rightSiblingIdx - 1], parent, rightSibling);
        delete[] rightSibling->key;
        delete[] rightSibling->ptr;
        delete rightSibling;
    }
}
```

## 8. B+Tree 비단말 노드 remove
1. 루트인 경우 지우고 끝
2. 아닌 경우 키 값 지우고, 포인터도 없앤다.
3. 사이즈를 줄이는데, 이 때 사이즈가 충분히 작지 않다면 그냥 끝낸다
4. 아닌 경우 부모 노드를 통해 양 옆 형제 노드 찾기
5. 왼쪽 노드 부터 확인해서 원소 수가 충분하다면 하나 뺏어 오기
6. 부족한 경우 한 쪽에 몰아주기

이렇게 적으면 쉽지만, 자세한 구동을 주석으로 확인해 보자.
```cpp
void BPlusTree::removeInternal(int K, Node *cursor, Node *child) {
    if (cursor == root && cursor->size == 1) {
        for (int i = 0; i < 2; i++)
            if (cursor->ptr[i - 1] == child) {
                delete[] child->key;
                delete[] child->ptr;
                delete child;

                root = cursor->ptr[i];

                delete[] cursor->key;
                delete[] cursor->ptr;
                delete cursor;
                return;
            }
    }


    int position;
    for (int i = 0; i < cursor->size; i++) if (cursor->key[i] == K) position = i;
    // 넣을 곳 찾았으니, 한 칸씩 키 땡겨주면서 자연스럽게 키 값을 지워버린다.
    for (int i = position; i < cursor->size; i++) cursor->key[i] = cursor->key[i + 1];

    /* 이번엔 포인터 */
    for (int i = 0; i < cursor->size + 1; i++) if (cursor->ptr[i] == child) position = i;
    for (int i = position; i < cursor->size + 1; i++) cursor->ptr[i] = cursor->ptr[i + 1];

    /* 사이즈 줄이고 사이즈가 적당하거나, 루트 였다면 이만 끝낸다. */
    cursor->size--;
    if (cursor == root || cursor->size >= (MAX + 1) / 2 - 1) return;

    /* 부모와 양 옆 노드 찾기 */
    Node *parent = findParent(root, cursor);
    int leftSiblingIdx, rightSiblingIdx;
    for (int i = 0; i < parent->size + 1; i++) {
        if (parent->ptr[i] != cursor) continue;
        leftSiblingIdx = i - 1;
        rightSiblingIdx = i + 1;
        position = i;
        break;
    }

    /* 왼쪽 자식이 있다면, 원소 하나 뺏어오기 */
    Node *leftSibling = leftSiblingIdx >= 0 ? parent->ptr[leftSiblingIdx] : nullptr;
    if (leftSibling && leftSibling->size >= (MAX + 1) / 2 + 1) {
        /* 가져온 키를 맨 앞에 붙일 것이므로 자리를 한칸 만든다. */
        for (int i = cursor->size; i > 0; i--) cursor->key[i] = cursor->key[i - 1];
        cursor->size++;

        /* 여기서 조금 헷갈릴 수 있다.
         * 현재 노드의 가장 왼쪽은, 부모 노드에서 왼쪽 자식을 가리키던 엔트리의 키값을 놓고,
         * 부모 노드에서 왼쪽 자식을 가리키던 엔트리의 키값이 있던 부분은
         * 왼쪽 자식의 가장 큰 값으로 바꾼다. */
        cursor->key[0] = parent->key[leftSiblingIdx];
        parent->key[leftSiblingIdx] = leftSibling->key[leftSibling->size - 1];

        /* 포인터도 옮겨 준다. 왼쪽 자식이 현재 노드를 가리키도록 잘 설정 */
        for (int i = cursor->size + 1; i > 0; i--) cursor->ptr[i] = cursor->ptr[i - 1];
        cursor->ptr[0] = leftSibling->ptr[leftSibling->size];
        cursor->size++;
        leftSibling->size--;

        return;
    }

    /* 오른쪽 반복 */
    Node *rightSibling = rightSiblingIdx <= parent->size ? parent->ptr[rightSiblingIdx] : nullptr;
    if (rightSibling && rightSibling->size >= (MAX + 1) / 2 + 1) {
        /* 부모 노드에서 현재 노드를 가리키던 엔트리의 키 값을 가져온다.
         * 그 자리엔 오른쪽 노드의 가장 작은 숫자를 채운다.
         * */
        cursor->key[cursor->size] = parent->key[position];
        parent->key[position] = rightSibling->key[0];

        /* 한 칸씩 땡겨 주면서 자연스럽게 가장 앞쪽 키를 지운다.
         * */
        for (int i = 0; i < rightSibling->size - 1; i++) rightSibling->key[i] = rightSibling->key[i + 1];
        /* 포인터 수정 */
        cursor->ptr[cursor->size + 1] = rightSibling->ptr[0];
        for (int i = 0; i < rightSibling->size; i++) rightSibling->ptr[i] = rightSibling->ptr[i + 1];

        cursor->size++;
        rightSibling->size--;
        return;
    }

    /*
    * 오른쪽 왼쪽 비단말 노드들이 다 원소를 노드 최소 갯수만 가지고 있는 경우.. */
    if (leftSibling) {
        /* 단말 노드 때와는 다르다. 또 키값 뺏어 오기*/
        leftSibling->key[leftSibling->size] = parent->key[leftSiblingIdx];

        for (int i = leftSibling->size + 1, j = 0; j < cursor->size; i++, j++) leftSibling->key[i] = cursor->key[j];
        for (int i = leftSibling->size + 1, j = 0; j < cursor->size + 1; i++, j++) {
            leftSibling->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = nullptr;
        }
        leftSibling->size += cursor->size + 1;
        cursor->size = 0;
        removeInternal(parent->key[leftSiblingIdx], parent, cursor);
        return;
    }

    if (rightSibling) {
        cursor->key[cursor->size] = parent->key[rightSiblingIdx - 1];

        for (int i = cursor->size + 1, j = 0; j < rightSibling->size; i++, j++) cursor->key[i] = rightSibling->key[j];
        for (int i = cursor->size + 1, j = 0; j < rightSibling->size; i++, j++) {
            cursor->ptr[i] = rightSibling->ptr[j];
            rightSibling->ptr[j] = nullptr;
        }
        cursor->size += rightSibling->size + 1;
        rightSibling->size = 0;
        removeInternal(parent->key[rightSiblingIdx - 1], parent, cursor);
    }
}
```

## Reference
- Database Management Systems \<Raghu Ramkrishnan 저>
- [B+ Tree - Programiz](https://www.programiz.com/dsa/deletion-from-a-b-plus-tree)
- [B+ Tree - Wikipedia](https://en.wikipedia.org/wiki/B-tree)
