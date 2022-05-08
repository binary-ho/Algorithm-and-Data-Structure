트라이
===
문자열의 구성 charactor로 만든 트리. 전처리 과정이 매우 헤비한 대신, 탐색이 매우 빠릅니다.    
문자열에서 이어지는 알파벳의 시퀀스에 따라 새로운 노드를 만들어 다음 문자를 넣어주고 또 그 노드에 다음 철자의 노드를 만들어 주는 식으로 진행합니다.		     
포인트는 문자열 끝의 구분인데, 노드에서 bool이나 다른 여러 장치를 이용해서 현재 노드가 어떤 문자열의 끝 문자임을 알리는 기능을 만들어야합니다. 이를 통해 어떤 문자열이 실제로 존재하는지 파악 할 수 있습니다.    
트라이는 정말 정말 빠른 시간 내에 탐색을 진행 할 수 있습니다. 실제로 거의 탐색을 안 하는거나 마찬가지입니다. **다수의 짚더미 문자열들 중에 내가 가지고 있는 바늘 문자열이 존재하는지 파악하기에 아주 좋습니다.** 	    
대신에 알파벳 갯수만큼의 노드 포인터를 계속해서 갖을 수도 있기 때문에 공간 복잡도에 유의해야한다.    
**약 100만 글자에 200MB정도**니 주의해야한다.
<br/>

## 1. 정석적인 트라이
```c++
#define C_SIZE 26
using namespace std;
int toNum(char c) { return c - 'a'; }
struct TrieNode
{
	int keyNum;
	TrieNode* children[C_SIZE];

	TrieNode() {
		keyNum = -1;	// keyNum인 이유는 각 문자열이 몇번째 문자열인지 저장해주려는 의도. 나만의 활용법.
		memset(children, 0, sizeof(children));
	}
	~TrieNode() {
		for (int i = 0; i < C_SIZE; i++) {
			if (children[i]) delete children[i];
		}
	}

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

	TrieNode* find(const char* key) {
		//printf("%c", *key);
		if (*key == 0) return this;
		int next = toNum(*key);
		if (children[next] == nullptr) return nullptr;
		return children[next]->find(key + 1);
	}
} root;

```
1. keyNum가 문자열의 끝인지 구분을 위한 변수. (bool로 해도 되고 int일 경우 몇 번째 문자열인지 확인하기가 편함.)
2. 끊어지면 nullptr 반환 (해당 문자열이 존재하지 않음.)
<br/>

## 2. 무슨 심보인지는 몰라도 pool을 미리 만드는 버젼을 혼자 만들어 봤다.
그냥 pool 만들어 노드를 빼 써 오는데 한창 맛이 들렸을 떄인것 같다. 코드 첨부한다.


```C++

#define C_SIZE 10
#define MAXN 100001
int toNum(char c) { return c - '0'; }

int trieCnt, keyCnt;
struct TrieNode
{
    int keyNum;
    TrieNode* children[C_SIZE];
    TrieNode() {
        keyNum = -1;
        memset(children, 0, sizeof(children));
    }

    void clear() {
        keyNum = -1;
        memset(children, 0, sizeof(children));
    }
} triePool[MAXN];

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
