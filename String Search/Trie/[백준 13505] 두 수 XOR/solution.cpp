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
