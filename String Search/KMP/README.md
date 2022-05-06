KMP 알고리즘
===
코테 끝나고 제대로 정리.

Knuth-Morris-Pratt Algorithm은 문자열 알고리즘에서 가장 유명한 알고리즘으로, 단순히 어떤 문자열을 검색한다고 생각 했을 때와 같이, 하나 하나 문자들이 일치하는지 확인한다.    
다만, 원본 문자열의 어떤 부분에서 비교 문자열의 비교가 끝났을 떄, 원본 문자열의 바로 다음 문자에서 다시 세는 것이 아니라, 확실히 일치하지 않을 것으로 기대되는 부분을 건너 뛰고 시작합니다.    
말을 너무 못 했습니다.    
**문자를 하나 하나 비교하며 얻는 정보들을 이용하여 무조건 아닐 부분은 전부 건너 뛰고 다음 탐색을 시작하는 것입니다.**

비교할 문자열 P가 있을 떄, P에서 P를 찾는다고 가정합니다. P는 가만히 있고, P의 1번째 원소 부터 마지막 원소까지 P와 비교를 하여 언제 얼마나 겹치는지를 알아냅니다.    
0번쨰 원소 부터 탐색을 실시한다면 아예 겹치기 때문에 비교하나 마나겠지요?    
비교를 해 나가면서 각 위치에서 P의 접두사이면서, P의 접미사인 최대 길이를 파악하여 저장하는 배열 **부분 일치 테이블**을 만듭니다.


```c++
int pi[1000001];
string S, P;  // 원본 문자열이 S, 비교할 문자열이 P
// 부분 일치 테이블 생성

int begin = 1, matched = 0;
while (begin + matched < P.size()) {
    if (P[begin + matched] == P[matched]) {
        ++matched;
        pi[begin + matched - 1] = matched;
    } else {
        if (matched == 0) {
            begin++;
        } else {
            begin += matched - pi[matched - 1];
            matched = pi[matched - 1];
        }
    }
}

begin = 0, matched = 0;
while (begin + P.size() <= S.size()) {
    if (matched < P.size() && S[begin + matched] == P[matched]) {
        ++matched;
        if (matched == P.size()) {
            // 문자열 P를 찾은 상황! 이때 종료합니다.
            return 0;
        }
    } else {
        if (matched == 0) {
            begin++;
        } else {
            begin += matched - pi[matched - 1];
            matched = pi[matched - 1];
        }
    }
}
```




참고
===
알고리즘 문제해결 전략 <구종만 저>
