#include "BPlusTree.h"

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

/*
 * K 값을 insert
 * */
void BPlusTree::insert(int K) {
    if (root == nullptr) {
        root = new Node(K);
        return;
    }
    Node *cursor = root;
    Node *parent;
    bool findNext;
    while (!cursor->isLeaf) {
        findNext = false;
        parent = cursor;
        for (int ptrIdx = 0; ptrIdx < cursor->size; ptrIdx++) {
            if (K >= cursor->key[ptrIdx]) continue;
            // if K < cursor->buffer[ptrIdx]인 경우
            cursor = cursor->ptr[ptrIdx];
            findNext = true;
            break;
        }
        if (!findNext) cursor = cursor->ptr[cursor->size];
    }

    /* 현재 가르키는 노드가 꽉찼는지 아닌지 확인한다. */
    if (cursor->size < MAX) {
        int idx = 0;

        /* K값이 들어갈 위치 찾기기 */
        while (K > cursor->key[idx] && idx < cursor->size) idx++;

        /* idx에 K가 들어가기 위해 key들을 한 칸씩 옮긴다. */
        for (int i = cursor->size; i > idx; i--) cursor->key[i] = cursor->key[i - 1];

        /* 삽입 */
        cursor->key[idx] = K;
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;
        return;
    }

    /* 꽉 찬 경우
    * buffer key들 받아와서 K 삽입한 이후 분할할 것이다.
    * buffer에 삽입 */
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

    /* 내용물을 두 노드에 나누어 넣어준다. */
    for (int i = 0; i < cursor->size; i++) cursor->key[i] = buffer[i];
    for (int i = 0, j = cursor->size; i < newNode->size; i++, j++) newNode->key[i] = buffer[j];

    /* 분할한 다음 엔트리 하나가 위로 올라가게 된다.. */
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

/* 처음 insert는 단말 노드에 집중한 것이고, 내부 insert는 비단말 노드들의 insert */
void BPlusTree::insertInternal(int K, Node *cursor, Node *childNode) {
    /* 위에서 꽉차있지 않은 노드에 K값 엔트리를 넣는 과정과 유사하다. */
    if (cursor->size < MAX) {
        int idx = 0;
        while (K > cursor->key[idx] && idx < cursor->size) idx++;
        for (int i = cursor->size; i > idx; i--) cursor->key[i] = cursor->key[i - 1];
        cursor->key[idx] = K;
        cursor->size++;
        cursor->ptr[idx + 1] = childNode;
        return;
    }

    /* 꽉찬 경우
    * 이번에는 key값들 뿐만 아니라 ptr도 복사해야한다. */
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

/* 루트 부터 거슬러 내려와서 child node의 부모 노드를 찾는다. */
Node *BPlusTree::findParent(Node *cursor, Node *childNode) {
    /* 시작 부터 리프거나 리프의 부모인 경우 pass */
    if (cursor->isLeaf || cursor->ptr[0]->isLeaf) return nullptr;

    Node *parent;
    for (int i = 0; i < cursor->size + 1; i++) {
        if (cursor->ptr[i] == childNode) return cursor;
        parent = findParent(cursor->ptr[i], childNode);
        if (parent != nullptr) return cursor;
    }
    return parent;
}

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
    //  곳 찾았으니, 한 칸씩 키 땡겨주면서 자연스럽게 키 값을 지워버린다.
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
