#define MAXNODE 100001
using namespace std;

struct Node {
    int data;
    Node *parent, *child[2];
} node_pool[200001];
int pool_cnt;

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
Node* Heap::get_node(int data) {
    Node *node = &node_pool[pool_cnt++];
    node->data = data;
    node->parent = node->child[0] = node->child[1] = nullptr;
    return node;
}

Node* Heap::find_node(int idx) {
    if(idx > size) return nullptr;
    if(idx == 1) return root;
    return find_node(idx >> 1)->child[idx & 1];
}

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
