# \[자료구조\] 트라이
​
​
트라이는 기본적으로 문자열의 구성 character를 노드로 만든 만든 트리입니다. 전처리 과정이 매우 헤비한 대신, 탐색이 매우 빠릅니다.  
문자열에서 이어지는 문자의 시퀀스에 따라 새로운 노드를 만들어 다음 문자를 넣어주고 또 그 노드에 다음 철자의 노드를 만들어 주는 식으로 진행합니다. 이렇게 해주면 접두사가 겹치는 문자열들에 대해 빠른 탐색이 가능하겠죠?  
포인트는 문자열 끝의 구분인데, 노드에서 bool이나 다른 여러 장치를 이용해서 현재 노드가 어떤 문자열의 끝 문자임을 알리는 기능을 만들어야합니다. 이를 통해 어떤 문자열이 실제로 존재하는지 파악 할 수 있습니다.  
트라이는 정말 정말 빠른 시간 내에 탐색을 진행 할 수 있습니다. 실제로 거의 탐색을 안 하는거나 마찬가지입니다. **다수의 짚더미 문자열들 중에 내가 가지고 있는 바늘 문자열이 존재하는지 파악하기에 아주 좋습니다.**  
대신에 알파벳 갯수만큼의 노드 포인터를 계속해서 갖을 수도 있기 때문에 공간 복잡도에 유의해야합니다.  
대강 파악하자면 **약 100만 글자에 200MB정도입니다.** 주의합니다.  
그리고 만약에 **철자 종류가 너무 많다면**, 배열로 child를 만들기에 용량이 너무 크므로, **map으로 구현해주면 됩니다.**  
ex) 대소문자, 숫자 '.'다 되는 경우 -> `map<char, TrieNode*> child`, string 단위로 끊고 싶다 -> `map<string, TrieNode*> child`  
  
​
## 1\. 정석적인 트라이
​
```
#define C_SIZE 26
using namespace std;
int toNum(char c) { return c - 'a'; }
struct TrieNode
{
    int keyNum;
    TrieNode* children[C_SIZE];
​
    TrieNode() {
        keyNum = -1;    // keyNum인 이유는 각 문자열이 몇번째 문자열인지 저장해주려는 의도. 나만의 활용법.
        memset(children, 0, sizeof(children));
    }
    ~TrieNode() {
        for (int i = 0; i < C_SIZE; i++) {
            if (children[i]) delete children[i];
        }
    }
​
    void insert(const char* key) {
        //printf("%c", *key);
        if (*key == 0) keyNum = keyCnt++;
        else {
            int next = toNum(*key);
            if (children[next] == nullptr) {
                children[next] = new TrieNode();
            }
            children[next]->insert(key + 1);
        }
    }
​
    TrieNode* find(const char* key) {
        //printf("%c", *key);
        if (*key == 0) return this;
        int next = toNum(*key);
        if (children[next] == nullptr) return nullptr;
        return children[next]->find(key + 1);
    }
} root;
```
​
1.  keyNum가 문자열의 끝인지 구분을 위한 변수. (bool로 해도 되고 int일 경우 몇 번째 문자열인지 확인하기가 편함.)
2.  끊어지면 nullptr 반환 (해당 문자열이 존재하지 않음.)
​
## 2\. pool을 미리 만드는 버젼을 만들어 봤습니다.
​
pool 만들어 노드를 빼 써 오는데 한창 맛이 들렸을 때 만들어 봤습니다. 코드 첨부합니다.
​
```
​
#define C_SIZE 10
#define MAXN 100001
int toNum(char c) { return c - '0'; }
​
int trieCnt, keyCnt;
struct TrieNode
{
    int keyNum;
    TrieNode* children[C_SIZE];
    TrieNode() {
        keyNum = -1;
        memset(children, 0, sizeof(children));
    }
​
    void clear() {
        keyNum = -1;
        memset(children, 0, sizeof(children));
    }
} triePool[MAXN];
​
bool insert(TrieNode &trieNode, const char* key) {
    if (trieNode.keyNum == 1) return false;
    if (*key == 0) {
        if (trieNode.keyNum == -1) trieNode.keyNum = 1;
        else return false;
    }
    else {
        trieNode.keyNum = 0;
        int next = toNum(*key);
        if (trieNode.children[next] == nullptr) {
            trieNode.children[next] = &triePool[trieCnt++];
        }
        if(!insert(*trieNode.children[next], key + 1)) return false;
    }
    return true;
}
```
​
## 3\. XOR연산을 트라이로 할 수 있다.
​
전부 이진수 문자열로 환산하고 가장 긴 문자열에 모든 숫자의 이진수 표현의 길이를 맞춰줍니다. (앞에서 부터 연산할 때)  
전부 트라이에 insert 해준 다음 계산 하고 싶은 수를 넣습니다 (물론 길이 수를 맞춰줘야 좋을 듯)  
넣어 준 수의 반대, 즉 0이면 1, 1이면 0 노드가 있을 경우 한 자리 수를 키워준 다음 (num <<= 1); 1을 더해주고, 없을 경우엔 자리수만 키워줍니다.  
그렇게 끝까지 나아가면 수가 완성됩니다.
​
관련 문제과 코드를 첨부하겠습니다.
​
[https://dwaejinho.tistory.com/30](https://dwaejinho.tistory.com/30)
​
 [\[백준 130505\] 두 수 XOR - 트라이 문제
​
\[백준 130505\] 두 수 XOR \[백준 130505\] 두 수 XOR 두 수 XOR은 최대 1,000,000,000의 숫자들 10만개가 주어지고 그 중 XOR이 가장 큰 두 쌍의 XOR연산값을 2초 안에 구하는 문제이다. 당연히 수가 10만개가 되기..
​
dwaejinho.tistory.com](https://dwaejinho.tistory.com/30)
#
태그입력
