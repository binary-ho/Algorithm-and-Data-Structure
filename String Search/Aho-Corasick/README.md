Aho-Corasick Algorithm
====
아호-코라식 문자열 검색 알고리즘은 **트라이를 이용한 다중 문자열의 검색 알고리즘입니다.**    
마치 트라이를 이용해 KMP 알고리즘을 수행하는 것과 같은 알고리즘으로, 짚더미 문자열이 길 때 여러 바늘 문자열들의 존재 여부를 따져야 하는 경우에 효과적인 알고리즘입니다.

그 존재와 필요성은 아래 문제를 풀다가 알게 되었습니다.
[[백준 9250번] 문자열 집합 판별](https://www.acmicpc.net/problem/9250)

긴 짚더미 문자열 다수가 주어지고, 주어진 바늘 문자열들 중에 짚더미 문자열의 부분 문자열이 되는 것이 있는지를 판단하는 문제입니다.
여기서 짚더미 문자열의 길이가 10000이고, 1000개나 되기 때문에 정말 정말 다양한 방법으로 탐색 속도를 줄이고 줄이고 줄여도 2초의 탐색 내에 발견할 수 없었습니다.

바늘 문자열이 매우 적을 때는 KMP 알고리즘이면 족하지만, 모든 바늘 문자열에 대해 KMP 알고리즘을 수행하기에는 부담이 있습니다. **아호-코라식 알고리즘은 이런 바늘 문자열이 여러개일 때 다중 문자열 검색 문제에서 효과적인 알고리즘입니다.** 


아호-코라식 알고리즘은 KMP 알고리즘의 실패 했을 시 다시 탐색을 수행해야하는 위치를 알려주는 방식에 트라이를 응용한 방식입니다. 

KMP 알고리즘은 바늘 문자열의 각 접두사마다 접두사의 <U>접두사도 되고, 접미사도 되는 최대 문자열의 길이</U>를 미리 전처리 과정에서 계산하여 배열에 저장해줍니다. 이 실패 함수(failure funtion)을 **바늘 문자열의 접두사들로 이루어진 트라이에도 만들어줍니다.** 접두사들로 이루어진 트라이에서, 각 노드는 **자신의 접미사 중 트라이 상에 노드로 존재하는 것이 있다면, 그 중 가장 긴 것을 가르키게 됩니다.** 이렇게 하면 탐색에 실패해도 다시 그 지점 부터 트라이 탐색을 진행해 나가도 되니까 정말 편리하겠죠? 아주 긴 짚더미 문자열을 트라이에 던져줘도 단 한번의 트리 순회로 바늘 문자열이 속해있는지에 대한 여부를 파악 할 수 있겠습니다.

**기존의 트라이 코드에 실패 했을시 탐색해야할 노드를 가르킬 실패 노드와, 해당 노드까지 방문 하였을 때 까지 만나게 되는 바늘 문자열들을 기억해주는 배열이 필요합니다.** 이 때문에 아주 무거운 트라이가 되겠습니다. 시간 복잡도는 등장 문자열 배열 제작에 의해 바늘 문자열 등장 횟수 P, 짚더미 문자열 길이 N, 바늘 문자열 길이합 M을 모두 더한 O(N + M + P)이 되겠습니다. 사실상 짚더미의 길이 합이나 바늘의 길이 합 중 긴 것의 복잡도를 가지는 수준으로 아주 빠릅니다.

[기존 트라이 코드](https://github.com/binary-ho/Algorithm-and-Data-Structure/blob/main/String%20Search/Trie/README.md)에 실패 함수를 계산하는 부분과, 출력할 문자열의 목롤을 만들어 주는 부분이 추가됩니다.


```C++
struct TrieNode
{
    int keyNum; // 문자열 번호 기억을 위해 int로 선언해 주었습니다. 
    TrieNode* fail;
    TrieNode* children[C_SIZE];
    vector<int> output; // 출력 문자열 저장

...이하 생략
} root;
```
위와 같이 조금의 추가를 해줍니다.       
실패 함수의 계산은 루트 부터 탐색하는 bfs로 구현됩니다.
<br/>

```C++
void trieBfs(TrieNode *root) {
    root->fail = root;

    queue<TrieNode*> que;
    que.push(root);
    while(!que.empty()) {
        TrieNode *node_now = que.front();
        que.pop();
        for(int ch = 0; ch < C_SIZE; ch++) {
            TrieNode *next_node = node_now->children[ch];
            if(!next_node) continue;    // 다음 철자 없으면 pass
            if(node_now == root) next_node->fail = root;
            else {
                // 부모의 실패 연결을 추적하며 실패 연결을 뒤져가며,
                // 접미사 노드중 현재 노드의 문자열에 다음 철자를 붙인 문자열의
                // 접미사중 가장 긴 것을 가지고 있는 노드를 찾습니다.
                TrieNode *ptr = node_now->fail;
                while(ptr != root && ptr->children[ch] == nullptr) ptr = ptr->fail;

                // 탐색하는 동안 발견 했을 경우 그 노드의 포인터를 가져옵니다.
                if(ptr->children[ch]) ptr = ptr->children[ch];
                next_node->fail = ptr;
                // 발견 못 했을 경우엔 자연스럽게 root node를 가르키게 되겠습니다.
            }
            
            next_node->output = next_node->fail->output;    // 복사
            if(next_node->keyNum != -1) { 
                next_node->output.emplace_back(next_node->keyNum);    //추가
            }
            que.push(next_node);
        }
    }
}
```

탐색은 아래와 같다.
<br/>

```c++
vector<pair<int, int>> ahoCorasick(const string &str, TrieNode *root) {
    vector<pair<int, int>> vec;
    TrieNode *state = root;
    for(int i = 0; i < str.size(); i++) {
        int ch = toNum(str[i]);
        
        // 메칭에 실패하면 다음 철자를 맨 끝으로 가진 접미사들을 찾아 헤메인다.
        while(state != root && state->children[ch] == nullptr) state = state->fail;
        
        // 발견
        if(state->children[ch]) state = state->children[ch];
        
        for(int j = 0; j < state->output.size(); j++) {
            vec.emplace_back(i, state->output[j]);
            // 마지막 철자와 문자열의 번호 저장
        }
    }
    return vec;
}
```

보기 좋게 전체 코드도 첨부하겠다.
<br/>

```C++
#define C_SIZE 26
using namespace std;

int toNum(char c) { return c - 'a'; }
char str_arr[10001];
string str;


struct TrieNode
{
    bool terminal = false;  // 터미널로 구현한 버젼
    TrieNode* fail;
    TrieNode* children[C_SIZE];

    TrieNode() {
        terminal = false;
        memset(children, 0, sizeof(children));
    }
    ~TrieNode() {
        for (int i = 0; i < C_SIZE; i++) if(children[i]) delete children[i];
    }

    void insert(const char* key) {
        if (*key == 0) terminal = true;
        else {
            int next = toNum(*key);
            if (children[next] == nullptr) {
                children[next] = new TrieNode();
            }
            children[next]->insert(key + 1);
        }
    }
};


void trieBfs(TrieNode *root) {
    root->fail = root;

    queue<TrieNode*> que;
    que.push(root);
    while(!que.empty()) {
        TrieNode *node_now = que.front();
        que.pop();
        for(int ch = 0; ch < C_SIZE; ch++) {
            TrieNode *next_node = node_now->children[ch];
            if(next_node == nullptr) continue;
            if(node_now == root) next_node->fail = root;
            else {
                TrieNode *ptr = node_now->fail;
                while(ptr != root && ptr->children[ch] == nullptr) ptr = ptr->fail;
                if(ptr->children[ch]) ptr = ptr->children[ch];
                next_node->fail = ptr;
            }
            //if(next_node->fail->terminal) next_node->terminal = true;
            que.push(next_node);
        }
    }
}


vector<pair<int, int>> ahoCorasick(const string &str, TrieNode *root) {
    vector<pair<int, int>> vec;
    TrieNode *state = root;
    for(int i = 0; i < str.size(); i++) {
        int ch = toNum(str[i]);
        
        // 메칭에 실패하면 다음 철자를 맨 끝으로 가진 접미사들을 찾아 헤메인다.
        while(state != root && state->children[ch] == nullptr) state = state->fail;
        
        // 발견
        if(state->children[ch]) state = state->children[ch];
        
        for(int j = 0; j < state->output.size(); j++) {
            vec.emplace_back(i, state->output[j]);
            // 마지막 철자와 문자열의 번호 저장
        }
    }
    return vec;
}

```


<br/><br/>
## 부분 문자열 찾기 예외 상황
[[백준 9250번] 문자열 집합 판별](https://www.acmicpc.net/problem/9250) 문제에서 발견한 예외 상황이다.
짚더미 문자열의 부분 문자열이 바늘 배열에 존재하는가 찾는 문제에서, 어떤 노드의 실패 노드가 완성된 문자열라면, 해당 노드에서도 이를 알게 해줘야 한다.
어차피 어떤 노드의 실패 노드가 완성된 문자열이라면 그 문자열은 무조건 짚더미 문자열의 부분 문자열인 것이기 때문이다.

이는 que에 push하기 전에 손볼 수 있다.
keyNumber가 아니라, bool type의 변수 terminal로 문자열 끝을 구분 했을 떄,
``` c++
... 생략...

queue<TrieNode*> que;
    que.push(root);
    while(!que.empty()) {
        TrieNode *node_now = que.front();
        que.pop();
        for(int ch = 0; ch < C_SIZE; ch++) {
            TrieNode *next_node = node_now->children[ch];
            if(next_node == nullptr) continue;
            if(node_now == root) next_node->fail = root;
            else {
                TrieNode *ptr = node_now->fail;
                while(ptr != root && ptr->children[ch] == nullptr) ptr = ptr->fail;
                if(ptr->children[ch]) ptr = ptr->children[ch];
                next_node->fail = ptr;
            }
            if(next_node->fail->terminal) next_node->terminal = true;  ----->> 한 줄 추가
            que.push(next_node);
        }
    }
    
    
... 생략 ...
```


참고
--
알고리즘 문제 해결 전략 <구종만 저>
