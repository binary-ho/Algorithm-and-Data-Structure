두 수 XOR
====

[[백준 130505] 두 수 XOR](https://www.acmicpc.net/problem/13505)

두 수 XOR은 최대 1,000,000,000의 숫자들 10만개가 주어지고 그 중 XOR이 가장 큰 두 쌍의 XOR연산값을 2초 안에 구하는 문제이다.    
당연히 수가 10만개가 되기 때문에 직접 XOR 연산을 해준다면 (10만^2/1억) 초 정도의 시간이 소요되기 때문에 시간초과가 된다.    
해당 연산에 트라이를 적용하면 더 빠른 연산이 가능해진다.        

기본적으로 이진수 연산을 위해 모든 수를 이진수 문자열로 만들어 준 후, 그 수들로 트라이를 만들어 준다. 이후 모든 수들로 한번 더 탐색을 진행해준다. 
이진수 XOR연산은 두 수가 같으면 0, 다르면 1을 결과로 갖는다. 넣어줬던 수들을 또 트라이에서 탐색하되, **지금 탐색중인 문자 (0 or 1)에 대해 다른 수가 트라이 안에 있다면, 
그 쪽으로 진행하고, 없다면 원래의 수 방향으로 진행한다.** 그러면 자연스럽게 지금 탐색중인 문자열과 구성이 **다른** 문자열의 존재 여부와 그 위치를 쉽게 알 수 있게 된다.

그래서 나는 각 탐색에서 위와 같이 진행해주는 동시에 숫자를 들고 다녀서, 매번 비트를 1칸 왼쪽으로 밀어주고, 다른 수가 있으면 1을 더해 주었다. 
이렇게 하면 모든 탐색을 마쳤을 때, xOR연산의 결과를 가지고 있게 된다. 자리수를 맞춰주지 않는 방법도 있겠지만, 나는 맞춰준 이후 계산하면 더 편할 것 같아서, 
트라이에 숫자들을 넣어 줄 때 가장 큰 숫자에 대해 이진수 변환을 하고, 그 자릿수에 다른 숫자들도 모두 맞춰주었다.

구현 코드는 폴더 내에 solution.cpp에 첨부합니다.

```c++
#include<iostream>
#include<algorithm>
#include<vector>
#include <map>

using namespace std;

int limit;
long long ans;

// 이진수를 만들어 주는 코드.
// 모든 수를 이진수 표현이 가장 긴 숫자의 자릿수인 limit와 같은 자릿수로 만들어줌니다.
string makeBinary(long long num) {
    string str = "";
    while(num != 0) {
        str += to_string(num & 1);
        num >>= 1;
    }
    if(limit != -1) while(str.size() < limit) str += "0";
    reverse(str.begin(), str.end());
    return str;
}

// 트라이 노드 코드.
// 0과 1만 있기 때문에 2칸으로 구성 되어있습니다.
// 10억이라는 큰 숫자도 위와 같은 구성이면 공간복잡도 제한을 넘지 않을 것이라고 판단했습니다. 
struct TrieNode
{
    TrieNode *children[2];

    TrieNode() {
        children[0] = children[1] = {};
    }
    void insertString(const string &str, int idx) {
        if(str[idx]) {
            int next = str[idx] - '0';
            if (children[next] == nullptr) {
                children[next] = new TrieNode();
            }
            children[next]->insertString(str, idx + 1);
        }
    }
    
    // 탐색과정!
    // 재귀적으로 탐색을 진행하면서 위에서 언급한 대로 항상 비트를 밀어주고, 
    // 나와 다른 숫자로의 진행이 가능하면 1을 더해주며 그 쪽으로 탐색을 진행하는 방식입니다.
    // 마지막에 완성된 숫자를 최대값과 비교하여 초기화해줍니다.
    void find(const string &str, int idx, long long num) {
        if(!str[idx]) {
            ans = ans < num ? num : ans;
            return;
        }
        num <<= 1;
        if(children[1 - (str[idx] - '0')]) {
            num += 1;
            children[1 - (str[idx] - '0')]->find(str, idx + 1, num);
        }
        else children[(str[idx] - '0')]->find(str, idx + 1, num);
    }
};



int main() {
    int N;
    cin >> N;
    vector<long long> vec(N, 0);
    vector<string> str_vec;
    for(int i = 0; i < N; i++) {
        cin >> vec[i];
    }
    sort(vec.begin(), vec.end(), greater<long long>()); // 이진수 자릿수가 가장 긴 숫자를 찾기 위한 정렬
    vec.erase(unique(vec.begin(), vec.end()), vec.end()); // 정렬을 해준 김에 중복을 제거 해주었습니다. 큰 영향은 없습니다.
    limit = -1;
    string str;
    str = makeBinary(vec[0]);
    str_vec.emplace_back(str);
    limit = str.size();   // 가장 긴 이진수 표현의 자릿수를 찾아줍니다.
    TrieNode root;
    root.insertString(str, 0);
    for(int i = 1; i < N; i++) {
        str = makeBinary(vec[i]);
        str_vec.emplace_back(str);
        root.insertString(str, 0);
    }
    ans =0;
    for(string str_now : str_vec) {
        root.find(str_now, 0, 0);
    }
    cout << ans;
    return 0;
}
```
