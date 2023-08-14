# [DB] 코드로 이해하는 B+Tree 연산 과정
트리 기반 인덱싱에 자주 쓰이는 자료 구조인 B+Tree에 대해 공부하면서,

구체적으로 코드는 어떻게 이루어져 있는지에 대한 궁금증이 있었다.

호기심으로 살펴본 코드를 직접 한줄 한줄 보는 과정에서 많은 괴로움이 있었지만,

B+Tree에 대해 더 제대로 이해할 수 있었던 것 같다.

직접 작성한 코드는 아니지만, 한줄 한줄 읽기 쉽게 변수명이나 코드를 정말 많이 수정했다.

그리고 최대한 모든 요소에 대한 주석을 직접 달았다.

이론을 먼저 살펴보고, 코드를 보며 제대로 이해해보자.

## 1\. 동적 인덱스 구조 B+Tree 

B+Tree는 트리 구조의 일종으로 동적 인덱스 구조를 가지고 있다 

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/010841ce-c91e-4a94-bace-b1f3449b24fb)

B+Tree는 DB에서 인덱싱을 위해 탁월한 자료구조로, 단말 노드에 실제 데이터들을 저장한다. (리프 노드)

그리고 그림에서 보다 싶이 값들이 오름차순으로 정렬 되어 있으며,

삽입-삭제가 이루어질 때 동적으로 트리의 균형을 맞춘다. 즉, 어느 단말에서도 높이가 같다.

결국 오름차순 정렬 저장에, 어디서든 같은 Depth..

떠오르는게 있는가?

바로 M분 탐색이 가능하다는 것이다.

단말 노드 하나에 M개의 데이터를 저장하고, 총 N개의 데이터가 있을 때,

빅 오 표기법으로 O(log\_m(N))라는 무시무시한 시간 복잡도를 가지는 것이다.

물론 Heap의 구현을 아는 사람이라면 삽입과 삭제가 지끈거릴 정도로 복잡할 것이라는 것쯤은 예상 가능할 것이다.

B+Tree의 노드가 어떻게 이루어졌는지

삽입 연산과 삭제 연산이 어떻게 이루어지는지 알아보고, 코드도 살펴보자.

## 2\. B+Tree 노드

B+Tree를 공부하며 가장 헷갈리기 쉬운건 단말 노드와 비단말 노드이다.

앞으로 최하단 리프 노드의 노드를 단말 노드, 그 위의 노드들을 내부 노드 혹은 비단말 노드라고 부르겠다.

둘은 정확히 구분되어 있고, 다른 특징을 가진다는 점에 유의하지 않으면 이해하는데 어려움을 느낄 수 있다.

둘은 존재 목적 부터가 다르다는걸 알면 좀 더 이해하기 쉬운데

단말 노드는 실제 데이터들을 저장하기 위해 존재하며

비단말 노드는 원하는 값이 저장된 단말 노드까지의 빠른 탐색을 위해 보조하는 역할이라고 생각하면 편하다.

### 2.1 비단말 노드

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/926ca9f3-74d2-474e-8eba-27a368ffe5a0)

위는 비단말 노드의 그림이다. 큰 네모칸이 하나의 비단말 노드이다. 헷갈리지 말자.

각 비단말 노드는 m개의 key 값과 m + 1개의 포인터를 가지고 있다.

그림을 보면 좌우 끝에 포인터가 있고, 그 사이엔 key, pointer, key, pointer ... key와 같은 형태로 정렬 되어 있다.

각 포인터는 일정한 순서로 정렬 되어 있는 단말노드를 찾아 가기 위해 자식 노드를 가리키고 있다.

i번째 key값 k(i)에 대해 포인터 p(i-1), p(i)는 각각 k(i)값 미만 값만을 가진 노드, k(i)값 이상 값만을 가진 노드를 가리킨다.  
  

첫 번째 그림을 다시 보자.

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/3f639643-1541-4226-80df-2fa4abde0fbb)

루트 노드 (비단말 노드다)의 첫 key를 보면 값이 13이다.

13의 왼쪽 포인터가 가르키는 노드에는 2, 3, 5, 7이 들어있는데, 언급한 대로 13 미만의 값들만이 존재한다.

그리고 루트 노드 13과 17 사이의 포인터는 13이상 17 미만의 값을 담고 있다.

나머지 포인터들도 살펴보며 같은지 확인해보자.

24와 30 사이의 포인터는 24, 27, 29로 24 이상 30 미만의 값을 담고 있다.

기본적으로 비단말 노드는 이렇게 구성 되어 있다.

한 노드가 가지는 key의 갯수 m은 차수 'd'가 결정한다.

루트 노드를 제외한 모든 노드는 d <= m <= 2d개의 key를 가질 수 있다.

왜 d면 d지 2d까지인가? 라고 생각할 수 있는데, 나중에 insert와 delete를 보면 이 2d가 얼마나 절묘하고 예술적인 값 설정인지 알 수 있게 될 것이다.

그리고 범위만 딱 보면 놓치기 쉬운데 d개 부터 2d개 일 수 있군!

이라는 생각 보다는 최소 d개는 갖춰야 한다는 포인트도 짚고 갔으면 좋겠다.

값의 갯수가 d개보다 아래일 때를 지나면 모든 노드는 d개의 값을 최소로 갖는다.

### 2.2 단말 노드

바로 위의 그림을 다시 보자.

단말 노드는 실제 저장할 데이터가 저장되는 노드이다.

테이터는 값일 수도 있고, 어떤 곳을 가리키는 포인터일 수도 있다.

위의 그림을 보면 양쪽 끝에 다른 단말 노드를 가리키는 부분이 있는데,

탐색의 편의를 위해 옆에 이웃한 형제 노드를 가리키고 있다.

여기까지만 보면 그렇게 안 다른데? 라고 할 수 있지만,

다양한 연산에 대해 다르게 처리를 해줘야 한다는 점만 알고 넘어가자.

## 3\. 노드와 트리 코드

### 3.1 노드 클래스

```
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

1.  트리 함수에서 노드에 접근할 일은 정말 많다. 이에 friend class 선언을 해주었다.
2.  key와 다음 노드를 가리키는 pointer는 포인터의 형태로 되어있다. 여러 값들을 가지게 하기 위해서이다.
3.  size는 말 그대로 size. key의 갯수가 되겠다. 당연히 포인터는 하나가 더 많다.
4.  isLeaf는 단말 노드인지 확인을 위한 변수이다.
5.  생성자가 2개인데, 변수 하나만 넣어줬을 떄의 생성자를 만들어 봤다.

### 3.2 B+Tree class

```
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

1.  B+Tree의 노드들은 각 노드별로 가질 수 있는 엔트리 갯수에 제한을 갖는다.  
    이에 Tree 자체에서 갯수 제한 상수 MAX를 가지게 하였고, Node에서 MAX에 접근하기 위해 friend class 선언 해주었다.
2.  차수 order d에 대해서 하나의 노드는 m개의 엔트리를 가질 수 있다 즉, d <= m <= 2d  
    이에 생성 시점에서 order를 입력으로 받아 상수 MAX를 초기화 해주었다.  
    MAX는 2d+1로 노드가 가질 수 있는 엔트리의 상한이다.
3.  public으로 공개한 메서드들은 단말 노드에 대한 연산이 대부분이다.  
    그리고 private로 선언안 것들은 보통 비단말노드간의 연산을 위해 존재한다.
4.  기본적으로 탐색, 삽입, 삭제의 메서드를 제공하고,  
    내부적으로는 단말 부터 타고 올라가는 연쇄적인 삽입, 삭제를 위해 부모 노드를 찾는 메서드가 있다.
5.  마지막 줄을 보면 tree라는 이름으로 하나 만들었다.

### 3.3 Node class 생성자!

```
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

Node의 생성자이다.

1.  MAX개의 key값 배열과 MAX + 1개의 child node를 가리키는 ptr를 갖는 ptr 배열을 만든다.  
    (처음엔 childPtr로 만들었다가 의미상 수정하였다.)
2.  K를 넣어주는 경우 K값을 가진 단말 노드를 만든다. 편의상 추가했다.

## 4\. 탐색 연산

B+Tree를 사용하는 이유이다.    

주어진 데이터가 속한 단말 노드를 겨우 O(log\_m(N)) 만에 찾아내는데 그 과정은 간단하다.

각 노드의 값들이 오름차순으로 정렬 되어 있으므로 검색하려는 값 Q가 있을 때,

i번째 key값 k(i) < Q < k(i+1)인 i를 찾아내어, 두 key값 사이에 있는 포인터를 타고 다음 노드로 이동한다.

만약 가장 작은 key보다 Q가 작으면 가장 왼쪽의,

가장 큰 key값 보다 Q가 크면 가장 오른쪽으로 가면 되겠다.

이런 식으로 탐색하다가 리프 노드를 찾으면 데이터를 반환하면 되는데,

이동할 포인터를 찾거나 단말 노드에서 값을 때에도 오름차순 정렬이므로 이분 탐색이 가능하다.

m이 일반적으로 그리 크지 않으므로, 매우 빠르다.

이런 탐색 연산은 삽입 삭제시 값을 찾을 때도 사용된다.

search 코드는 따로 구현하지 않았다. 단순히 적절한 위치를 찾아 계속 나아가면 된다.

## 5\. 삽입 연산

삽입 연산은 재귀적으로 리프로 부터 타고 올라오면서 이루어진다.

무슨 말이나면 탐색 연산으로 우선 삽입할 값이 들어갈 정확한 위치를 파악한 다음

단말 노드에 삽입된 다음 필요하다면, 해당 값을 찾기 위한 비단말 노드들의 갱신이 이루어질 것이다.

### 5.1 단말 노드

K값을 넣는다고 생각해보자.

탐색 연산을 통해 K값이 들어갈 적절한 위치를 루트로 부터 비단말 노드를 타고 내려가며 탐색한다.

적당한 위치를 찾았다면, 노드의 key값 갯수를 확인한다!

최대값인 2d 보다 작은 갯수라면 안심하고 삽입한 다음 끝내면 된다.

문제는 이미 2d개의 key를 가지고 있을 때이다.

이 때는 노드를 분할하거나 재분배 해야 한다.

재분배는 단말 노드를 2개로 나누는 과정이다.

기존 2d개의 key가 있고 하나가 더해져야 하니 2d + 1개일 것이다.

이때 두 노드에 대해 d개 d + 1개로 나눈다.

이렇게 하나의 단말 노드를 2개의 단말 노드로 나눌 수 있었다.

여기서 끝이 아니다.

단말 노드가 하나 늘어난 만큼, 포인터가 하나 더 필요해졌다!

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/d39c99e8-c63b-4037-9d3a-4cd3076912e7)

예를 들어 2d가 4인 단말 노드에 2, 3, 5, 7이 들어 있었다가 8이 삽입 되었다고 가정하자.

그러면 노드는 2, 3,한개  5, 7, 8 한개로 나뉠 것이다.

기존의 노드를 가리키는 포인터 오른쪽의 key값은  8 초과의 어떤 수 K일 것이다.

K를 13이라고 생각해보자.

애초에 이 단말 노드에 값이 들어왔다는건, 새로운 key값이 13 미만이라는 것이다.

이 포인터는 5, 7, 8이 들어있는 노드를 그대로 가리키면 된다.

새로운 포인터는 2 이상 5 미만의 수를 가리키면 된다.

왜냐하면 오른쪽 노드의 가장 작은 값이 5이므로, 왼쪽 노드엔 항상 5보다 작은 값이 있어야 한다.

그러므로 비단말 노드에 key값 5을 새로 만들어서 올려주고,

key 값 5의 왼쪽엔 2, 3이 들어있는 노드를 가리키는 새로운 포인터가 추가되면 되겠다!

간단하게 생각하면 오른쪽 노드의 가장 왼쪽 값이 올라가면 되는 것이다.

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/7c4394ac-5753-4bd7-a527-6ac3e6ae1ff2)

이해가 가는가? 구현할 때는 2d+1 상태에서 가장 가운데 위치한 값을 위로 올려주면 되는 것이다.

절묘하다.

물론 값이 추가될 때 비단말 노드를 분할할 필요가 없다면 새로 값을 비단말 노드 쪽으로 올려줄 필요는 없겠다!

다른 방법으론 재분배가 있다.

재분배는 형제 노드들을 확인하여 여유가 있다면 노드를 하나씩 나눠 주는 것인데,

들어만 봐도 구현이 무척 어렵다. 노드를 가리키는 비단말 노드의 key값에도 변화가 생기기 때문이다.

그래서 보통은 분할로 구현이 되고, 재분배의 경우 확장된 B+Tree에서 쓰인다고 한다.

### 5.2 단말 노드 insert 코드 구현

```
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

단말 노드 insert를 구현한 메서드이다.

1.  루트인 경우 새로 만들어준 다음 삽입하고 연산을 끝낸다.
2.  루트가 아닌 경우 K값이 들어갈만한 위치를 루트로 부터 노드들을 타고 내려가며 탐색한다.
3.  단말 노드를 찾은 다음 노드가 꽉차있는지 확인한다.
4.  꽉 차있지 않은 경우 올바른 위치를 찾아 값을 삽입한다.
5.  꽉 차있는 경우 분할을 위해 buffer를 만든다.  
    buffer에 값들을 모두 옮기고 올바른 위치에 K값을 넣는다.
6.  새로운 노드를 만든 다음 버퍼를 이용해 **내용물을 기존 노드와 새로운 노드에 나눠준다.**
7.  기존 노드의 마지막 ptr은 새로운 노드를 가리킨다. (단말 끼리는 이어져 있으므로!)
8.  이제 key값 하나를 위로 올려야 하는데,  
    현재 가리키는 노드가 루트인 경우 편하게 하나를 만들어 새로운 루트로 지정해 위로 올린다.
9.  아닌 경우 비단말 노드의 insert를 위한 insertInternal을 호출한다.

### 5.3 비단말 노드

단말노드에서 key를 하나 위의 비단말 노드로 올리는 과정에서

만약 비단말 노드도 꽉차서 2d + 1개의 key가 생긴다면 당연히 분할이 필요해진다.

위의 과정과 대부분 비슷하나 한가지 다른 점이 있다.

두 노드로 나눌 때 d개, d개로만 나누면 된다.

음.. 왜?

실제 데이터가 저장되는 곳은 단말 노드이고 비단말 노드는 단지 단말 노드를 찾아가기 위한 이정표에 불과하다.

따라서 실제하는 모든 값이 비단말 노드에 있을 필요는 없다.

그럼 2d+1에서 2d개만 쓴다면 나머지 하나는 어디로 가는가?

이 하나의 노드는 위로 한칸 올라간다.

노드가 2개로 나뉘어 지면서 당연히 부모 비단말 노드에도 하나의 pointer와 key가 더 필요해지지 않겠는가?

이를 위한 key로 남은 하나의 값을 올려주면 되겠다..

너무 절묘하고 아름답다.. 하나 딱 올리고 반 나누니

노드의 최소 key 갯수인 d개가 되어 버린다.

당연히 이번에도 정렬된 2d+1개의 key 중 중간 값이 올라가 주면 된다.

만약 나뉘어 지는 비단말 노드가 루트 노드인 경우, 트리 자체의 높이가 한 단계 높아지게 되는 것이다! 

4.4 비단말 노드 insert 구현

```
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

위로 거슬러 올라가며 insert!

  
전체적인 로직은 기본 단말 노드 insert와 비슷하다.  

한가지 다른 점은 노드를 분할-재분배 할떄인데,

이제는 자식 노드들을 가리키는 pointer들도 복사해서 나누어 주어야 한다는 점이다.

그리고 올라가게 되는 값은 중간 크기의 값이다 -> cursor->key\[cursor->size\]

## 6\. B+Tree 삭제 연산

삽입 연산과 비슷하게 처음엔 지울 key를 찾는 탐색 연산으로 시작한다.

단말 노드에서 적절한 key를 찾아 지운 이후, 그냥 끝나면 행복하겠지만,

최소 key 갯수인 d개 보다 key의 갯수가 작아진다면,

삽입과 반대로 합병이 일어나거나, 재분배가 일어난다.

### 6.1 삭제 재분배

삭제에서의 재분배는 양 옆 형제 노드에서 key를 하나 뜯어(?) 오는 것이다.

양 옆 형제 노드 둘 중 하나라도 d개 보다 많은 key를 가지고 있다면 가져올 수 있다!

중요한 점은 부모 key값을 바꿔줘야 한다는 것이다.

그 이유를 살펴보자.

왼쪽에서 하나 가져오는 경우엔 두 노드를 가리키는 포인터 사이에 있는 key값이 문제가 된다.

왜냐하면 위 key값을 기준으로 왼쪽엔 key값 보다 작은 값들이 있는 노드가,

오른쪽엔 key값 이상의 값들이 있는 노드가 있기 때문에, 하나를 빌려오는 순간 이 규칙이 깨진다.

따라서 두 노드를 가리키는 포인터 사이의 key값을 새로 빌려온 값으로 대체하면 모든 것이 깔끔하게 해결된다.

오른쪽에서 가져오는 경우에는 오른쪽에 남게된 값들 중 가장 작은 값을 올리면 해결된다.

그런데 만약 양 쪽 다 d개의 key만을 가지고 있다면 어떻게 해야 할까?

노드를 합병하면 된다!

### 6.2 삭제 합병

삭제 합병은 한쪽 노드에 다른쪽 노드의 key들을 전부 몰아주는 식으로 진행된다.

세개의 노드가 각각 d개, d개, d개의 key들을 가지고 있는 상태에서 하나가 빠지므로

단말 노드의 경우

d개, 2d - 1 개 혹은 2d - 1개, d개로 구성된 2개의 노드로 합쳐진다.

만약 현재 노드에서 왼쪽 형제 노드로 모든 key를 몰아 줬다고 생각해보자.

원래 현재 노드를 가리키는 부모의 포인터 왼쪽의 key값은 현재 노드에서 가장 작은 값 이하의 값이였다.

그래서 이제 그 어떤 노드도 가리키기 어려워 졌다. 이 key값과 포인터를 삭제한다.

그러면 원래 왼쪽 노드를 가리키던 pointer와 그 오른쪽의 key값은 가만히 있어도 조건을 만족할 수 있고,

현재 노드를 가리키던 포인터 오른쪽의 key값은 항상 왼쪽 노드의 key들 보다 큰 값을 갖고 있으니 그대로 가만히 있으면 된다.

오른쪽 노드로 몰아지는 경우에도 동일하다.

비단말 노드의 경우엔 조금 특이한 점이 있다.

d개, d개, d개 3개의 노드가 합쳐질 때

d개, 2d개 혹은 2d개 d개로 합쳐진다. 왜일까? 

예를 들어 왼쪽의 노드로 key를 전부 몰아준다고 생각해보자.

이때 재미있는 일이 일어나는데, 

몰아주기 전에 두 노드를 가리키는 포인터 사이의 key값을 왼쪽 노드 끝에 붙여준다!!

그럼 무슨 일이 일어날까?

왼쪽 자식 노드의 가장 오른쪽 포인터를 생각해보자.

이 포인터는 왼쪽 자식 노드의 가장 큰 key 값 이상의 key 값들이 모인 노드를 가리키고 있다. 

이 포인터 오른쪽에 부모 key값이 오는게 이상한가?

어떤 규칙도 깨트리지 않는다.

반대로 현재 노드의 가장 왼쪽 포인터를 생각해보자.

이 포인터는 오른쪽 노드의 가장 작은 값 미만의 값들이 모여있는 노드를 가리킨다.

이 포인터 왼쪽에 부모 노드의 key가 오는게 이상한가?

어떤 규칙도 깨트리지 않는다.

오히려 두 노드를 이어주는 중간 다리 역활을 완벽하게 해준다.

그러니까 왼쪽 자식 노드의 가장 오른쪽 포인터와 현재 노드의 가장 왼쪽 포인터를 버리지 말고

그 사이에 부모 key값을 끼워 넣으면 모든 것이 해결되는 것이다!!

너무 아름답다!

부모 key값의 왼쪽 포인터와 부모 key값만 걷어 내면 완벽하게 합병 과정이 완료된다. 코드를 살펴보자.

### 6.3 부모 노드 찾기 연산

삭제를 위한 형제 노드를 찾기 위해 부모 노드를 찾아내는 코드이다.

```
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

### 6.4 B+Tree 단말 노드 Remove

이번엔 노드를 지우는 과정이다.

1.  K값을 가진 말단 노드를 찾아 지운다.
2.  루트면 모든 연산을 끝낸다 -> 행복
3.  아닌 경우 부모 노드의 포인터 또한 적절하게 처리한다.
4.  크기가 충분히 작지 않다면! 하한을 넘는다면 그대로 끝!
5.  아니라면, 부모 노드를 찾아 형제 노드를 확보한다.
6.  왼쪽 형제 노드 먼저 확인을 해서,  
    왼쪽 혹은 오른쪽 형제 노드의 키 값의 수가 d보다 많다면,  
    그냥 key값을 하나 뺏어오고 연산을 끝낸다.
7.  아닌 경우 왼쪽 혹은 오른쪽 형제 노드에 key를 전부 몰아준다. -> 자세한 과정은 코드에 더 있다.
8.  그러면서 재귀적으로 다시 루트로 타고 올라가며 비단말 노드 remove 연산을 진행해준다.

복잡해 보이지만 실제 코드를 보며 자세한 동작을 보면 더 복잡하다!

함께 확인해보자.

```
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

### 6.5 B+Tree 비단말 노드 Remove

단말 노드인 경우와 대동소이하다!

1.  루트인 경우 지우고 끝난다.
2.  루트가 아닌 경우 키 값을 지우고, 포인터도 없애 준다.
3.  사이즈를 줄이는데, 이 때 사이즈가 d이상이라면
4.  아닌 경우 부모 노드를 통해 양 옆 형제 노드 찾아낸다.
5.  왼쪽 노드 부터 확인해서 원소 수가 d보다 크다면 하나 뺏어 온다.
6.  부족한 경우 한 쪽에 몰아주기

이렇게 적으면 쉬워 보이지만 구체적인 구현은 어렵다.

앞서 이론 부분에서 설명한 것들이 코드에 녹아있으니 꼭 확인하자.

```
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

### Reference

-   Database Management Systems <Raghu Ramkrishnan 저>
-   [B+ Tree - Programiz](https://www.programiz.com/dsa/deletion-from-a-b-plus-tree)
-   [B+ Tree - Wikipedia](https://en.wikipedia.org/wiki/B-tree)
