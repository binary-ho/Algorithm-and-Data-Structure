# Heap
가장 좋아하는 자료구조! 그런데 아직도 Heap에 대해 글을 쓴 적 없다는 사실을 깨달아버렸다. 서운했을 Heap을 위해 부랴부랴 써 본다. <br>
Heap은 나같은 사람들에게는 Priority Queue로 먼저 접해봤을 것이다. 이 우선순위 큐는 Heap의 한 종류인데, 큐는 큐인데 들어온 순서가 아닌, 우선순위 대로 정렬 되어 있는 큐이다. <br>

그럼 Heap은 뭘까? Heap은 정해둔 우선순위상 가장 높은 data가 root에 있는 **완전 이진 트리이다.** 우선순위 큐와 조금 다른 점은 내부적으로 우선순위대로 차곡차곡 정렬 되어 있지는 않다. <br>
**대신에 부모 노드는 항상 두 자식 노드보다 크다는 규칙만을 따른다.** 이러한 규칙만을 따르기 때문에, 완전 이진트리의 규칙을 잘 지켜낼 수 있고, 덕분에 균형 잡힌 트리로 빠른 평균 탐색 시간을 보장 해주는 것이다.


## 1. 완전 이진 트리와 Heap
```
완전 이진 트리(完全二進-, 영어: complete binary tree) 
트리 에서, 마지막 레벨을 제외하고 모든 레벨이 완전히 채워져 있으며, 
마지막 레벨의 모든 노드는 가능한 한 가장 왼쪽에 있다. 
- 위키백과 
```
내부적으로 자식을 최대 둘만 갖는 이진트리로 구성되어 있으며, 완전 이진트리의 성질을 지키기 위해서, 새로운 노드가 들어오는 경우, 말단 레벨의 가장 왼쪽 부터 채워나가고, 지울 때도 똑같다. <br>
이런 구현은 그리 어렵지 않은데, 쉬운 이유 또한 완전 이진트리이기 때문이다 <br>
완전 이진 트리는 노드의 갯수만 알 수 있다면, 부모노드와 자식 노드의 계산이 아주 쉽다! <br>
왼쪽 부터 차곡차곡 채우면 되기 때문에 노드의 갯수가 곧, 새로 들어가야 할 위치를 가리키고, 그 숫자를 2로 나누면 부모 노드의 위치가 바로 특정된다. <Br>
말로만 하면 복잡하다. 
```
1
2, 3
4, 5, 6
```
그림으로 나중에 대체하겠다 읽는 이에게 정말 미안하다.. <br> 
루트는 1이고, 그 자식들은 2와 3이다. 2의 자식은 4와 5이고, 3의 자식은 6, 7이다. <br>

잘 보면, **부모의 인덱스에 곱하기 2를 하면 왼쪽 자식의 인덱스, 거기에 1을 더하면 오른쪽 자식의 인덱스가 된다.** <Br>
반대로 자식의 인덱스를 2로 나누어 보아라. 대부분 언어에서의 integer는 2로 나눌 경우 floor연산을 하기 때문에, 4와 5는 2로 나눌 경우 2가 나오게 된다. **이는 부모 노드의 인덱스이다!!** <br>
이러한 성질은 배열로된 Heap을 구현할 때 아주 큰 도움이 된다.

완전 이진트리로 구현함으로서 모든 과정이 쉬워진다. <br> 

하지만 나는 포인터로 가리키는 형식으로 구현했다. 이렇게 한다고 완전 이진 트리의 장점을 버린건 아니다. 완전 이진 트리의 가장 큰 장점은 사실 **그래프가 편향되지 않는 다는 점이고, 덕분에 로그 시간의 빠른 평균 탐색 시간을 보장한다!**

## 2. Heap의 연산 구현
Heap을 구현하는 데에 있어 기본적으로 가장 중요한 철칙은 <br>
**부모 노드가 가진 값은 항상 두 자식 노드의 값보다 커야 한다는 점이다.** <br>
더도 말고 덜도 말고 이 한가지만 기억하고 있으면 힙 연산들에 대한 이해가 참 빠르다. <br>
전체 코드는 폴더 내 다른 파일에 있다. [여기](https://github.com/binary-ho/Algorithm-and-Data-Structure/blob/main/Heap/heap.cpp) 

## 2.1 node와 get node method
들어가기 전에, 내가 구현한 방식은 링크드 리스트 형식의 tree이고, node pool을 이용한 방식이다. 미리 만들어둔 노드들을 가져오는 방식을 선호한다. 
```cpp
struct Node {
    int data;
    Node *parent, *child[2];
} node_pool[200001];
```
위에서 언급한 것과 같이, 각 노드들은 data와 부모, 자식 들을 가리키는 포인터가 존재한다. <Br>
만약에 모든 노드를 배열에 넣어 구현한다면, 굳이 이렇게 모든 포인터를 가질 필요는 없다. 자신의 인덱스만 알면 전부 계산으로 구할 수 있기 때문!
```cpp
int pool_cnt;

Node* Heap::get_node(int data) {
    Node *node = &node_pool[pool_cnt++];
    node->data = data;
    node->parent = node->child[0] = node->child[1] = nullptr;
    return node;
}
``` 
pool_cnt를 통해 노드풀에서 노드를 가져온다. <br>
그냥 새로 노드를 만들어 가져오는 메서드다.

## 2.2 Heap
```cpp
struct Heap {
    Node *root;
    int size;
    
    void init();
    Node* get_node(int data);
    Node* find_node(int idx);
    void push(int data, int id);
    void pop();
    int top() { return root->data; }
};

void Heap::init() {
    root = nullptr;
    size = 0;
}

Node* Heap::find_node(int idx) {
    if(idx > size) return nullptr;
    if(idx == 1) return root;
    return find_node(idx >> 1)->child[idx & 1];
}
```
Heap 코드이다. root 노드의 포인터와 Heap의 size를 가지고 있다. 여러가지 메서드를 제공한다. 
1. init은 Heap을 시행하는 메서드
2. top은 우선순위가 가장 높은 값을 보여주는 메서드
3. find_node는 재귀적으로 특정 노드를 찾아내는 메서드


## 2.3 push
```cpp
void Heap::push(int data, int id) {
    if (size >= MAXNODE) return;

    Node *node = get_node(data);
    size++;

    if (size == 1) {
        root = node;
        return;
    }

    /* 새로운 노드가 들어갈 위치에 배치해준다.
     * 부모 노드와 이어준다. */
    Node *parent = find_node(size >> 1);
    parent->child[size & 1] = node;
    node->parent = parent;

    /* 여기서 parent는 node_now의 parent임을 절대 잊지 말자. */
    while (node != root && node->parent->data < node->data) {
        // data값 교환
        int temp = node->parent->data;
        node->parent->data = node->data;
        node->data = temp;

        node = node->parent;
    }
}
```
대망의 push이다. 
1. 새로운 노드는 완전 이진 트리의 유지를 위해 최하단 레벨, 가장 왼편에 둔다. find_node와 현재 크기를 이용하면 찾기 쉽다.
2. 새로운 노드를 배치해주고, 부모와 이어준다.
3. **이후 계속해서 자신보다 작은 숫자를 가진 부모 노드와 숫자를 바꿔가면서 타고 올라간다.**

그런데, 내가 부모 노드 보다 값이 크다고 막 바꾸어 가면서 타고 올라가도 되나? 혹시 나(현재 노드) 외에 나머지 한 쪽 자식의 값이 나보다 클 수도 있지 않나? <br> 
논리적으로 그럴 순 없다. 이미 부모노드는 다른 나머지 한 노드 보다 큰 값을 가지고 있기 때문에, 현재 노드의 값이 부모 노드 보다 크다면, 자연스럽게 다른 자식 노드보다 값이 크다! <br> 그러므로 안심하고 타고 올라가면 된다! 

## 2.4 pop
대망의 pop! 처음 본다면 조금 코드가 헷갈릴 수 있다. 컨셉을 먼저 이해하고 보면 쉬울 것이다.
```cpp
void Heap::pop() {
    if(size <= 1) {
        size = 0;
        return;
    }

    Node *node = find_node(size);
    root->data = node->data;
    node->parent->child[size & 1] = nullptr;
    size--;
    node = root;

    while(node->child[0] != nullptr) {
        Node *child;
        if(node->child[1] == nullptr) child = node->child[0];
        else child = node->child[0]->data < node->child[1]->data ? node->child[1] : node->child[0];

        if(child->data < node->data) break;
        int temp = child->data;
        child->data = node->data;
        node->data = temp;

        node = child;
    }
    return;
}
```

push와는 조금 다른 흐름
1. 가장 마지막 노드를 찾는다. -> 최하단 가장 오른쪽 노드
2. 가장 마지막 노드의 값을 루트에 넣고! 노드를 지운다! <br> **(참고 1)** 배열을 통한 구현의 경우 단순히 size를 하나 줄여만 주어도 참조할 idx가 없는 것이나 마찬가지이므로 size-- 처리로 끝 <br> **(참고 2)** 나는 커멘드-쿼리 분리를 좋아하기 때문에 pop 메서드의 반환값이 없지만, 기존 que들과 같이 pop이 top값을 반환하는 형태로 만들려면 이 때 미리 root의 값을 따로 저장해두어야 한다!!
3. 이후 반대로 타고 내려가면서, 현재 노드 보다 큰 값을 가진 자식이 있다면, 그 자리를 바꿔준다 <br> 여기서 push와 달리 주의해야 할 점이 있는데, **반드시 두 자식 노드들 중에서 값이 큰 노드와 비교해야 한다는 점이다.** <Br> 단순해 보이지만 이 부분을 놓치고 구현해서, 구현을 실패하는 경우가 종종 있었다!


## 3. 정리
막상 설명하고 보니 쉬운듯 하지만, 처음 배우고 구현할 때는 너무 힘들었던 것 같다. <Br>
heap은 내가 가장 좋아하는 자료구조로 다양한 커스터마이징을 통해 아주 빠른 속도로 우선순위 정렬이 가능하다! <br>
다양한 우선순위를 빠르게 뽑아내야 하는 상황이나, 다익스트라에서도 활용하기 좋다. <br>
push와 pop은 같이 봤다 싶이 이진탐색을 통해 타고 내려오고 올라오므로, log_2(N) 즉, **O(logN)의 시간복잡도를 가진다.** 다만 하나 하나 넣어주어야 하므로 또 N의 시간이 소요된다. <Br>
따라서, **N개의 모든 원소의 정렬에 대해 O(NlogN)의 시간복잡도를 갖는다!** <br>
전체 정렬 결과를 확보하기 보다는, 원소들이 실시간으로 추가- 삭제 되는 과정에서 가장 우선순위가 높은 값을 빠르게 알아내야 할 때 아주 유용하다. 

## Reference
- course of Samsung dx S/W algorithm
