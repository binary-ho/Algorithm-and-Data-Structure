Two Pointer
====
두 개의 가상의 커서로 범위를 표현하여, 조건에 따라 범위를 조절해가며 문제를 해결하는 문제입니다.    
예시로 보여주는 것이 가장 빠를 것으로 생각되어 투 포인터 다운 문제를 가져와 설명 드리겠습니다.

# [백준 2470] 두 용액
[**[백준 2470] 두 용액 문제 바로가기**](https://www.acmicpc.net/problem/2470) <br/> 

먼저 문제를 읽고 오시길 바랍니다. <br/>    
두 용액의 특성값의 합이 0과 가장 가까운 두 용액을 찾아내는 문제입니다.    
브루트 포스적인 접근으로는 당연히 NC2에 해당하는 시간 복잡도가 걸릴 수 밖에 없겠습니다. 이번 문제는 1초 안에 10만개의 용액을 검사해야하므로, 적당한 풀이가 아님을 알 수 있습니다.    
이럴 때 유용한 것이 투 포인터 입니다. **여러 값들 중에서 2개를 계속해서 검사해야하는데, 마치 이분 탐색과 같이 특정 조건에 따라 검사하는 탐색 없이 다른 값으로 바꿔줄 수 있는 경우 대부분을 투 포인터로 풀 수 있습니다.** 탐색 없이 옮긴다는 표현을 했는데, 이번 경우에는 그냥 인덱스만 옮기면 되서 따로 탐색 과정이 없습니다. <br/>

해당 문제는 용액값을 오름차순으로 정리해준 뒤에, 가장 작은 값과 가장 큰 값을 더해줍니다. 해당 값이 0보다 크다면, 0과 더 가까운 값이 있나 확인하기 위해 오른쪽 커서의 인덱스를 하나 줄입니다. 그렇게 해주면 어떤 일이 일어날까요?  숫자를 오름차순으로 정리해 주었기 때문에 자연스럽게 두 합은 더 작아집니다. 아직은 알 수 없지만, 양수인 경우 첫 번째 경우보다는 확실히 값이 0과 가까워 지게 됩니다. 두 수의 합이 0 이하일 때는 반대로, 값을 키워주기 위해 왼 쪽 커서 인덱스를 하나 늘려주면 음수의 경우 값이 0과 더 가까워지게 됩니다. <br/>

이런 탐색을 계속하면, 자연스럽게 0과 가장 가까운 수를 찾아낼 수 있습니다. 아래에 코드를 첨부합니다. 설명과 달리, 어느 쪽으로 커서를 이동 시킬 때 더 절댓값이 작아지는지를 기준으로 하였습니다. 이후 확인해보니 0과 비교할 때가 훨씬 빠른 성능을 보였습니다. 0과 비교하는 구현은 다음 문제에서 보여주겠습니다. (연산량이 적어져서 그렇다.) <br/>

```c++
cin >> N;
vector<int> vec(N, 0);
for(int i = 0; i < N; i++)  cin >> vec[i];
sort(vec.begin(), vec.end());   // 정렬해줍니다.
    
left = solution1 = 0;     // 왼쪽 커서로 가장 작은 값을
right = solution2 = N-1;  // 오른쪽 커서로 가장 큰 값을 가르킵니다.
int property = abs(vec[left] + vec[right]);
    
while(left < right) {
    if(property > abs(vec[left] + vec[right])) {  // 0과 더 가까운 값을 찾는 경우 갱신
        property = abs(vec[left] + vec[right]);
        solution1 = left;
        solution2 = right;
    }
    // 어느 쪽으로 가는 것이 더 절댓값을 줄이는 일인지 체크
    if(abs(vec[left + 1] + vec[right]) < abs(vec[left] + vec[right - 1])) {
        left++;
    } else {
        right--
    }
}
cout << vec[solution1] << " " << vec[solution2];
```


# [백준 2473] 세 용액
[**[백준 2473] 세 용액문제 바로가기**](https://www.acmicpc.net/problem/2473) <br/> 

이번 문제는 **3개의 기준이 있는 문제입니다.** 평범하게 또 투 포인터와 같은 풀이를 생각한다면, 시간 초과를 피할 수 없습니다.    
그런데 이 문제를 투 포인터로 N^2만에 푸는 알고리즘이 있습니다. 이를 [3SUM](https://en.wikipedia.org/wiki/3SUM) 문제라 합니다. (일반화 시킨 문제를 K-SUM이라고 부릅니다.) <br/>

똑같이 정렬을 해준 다음, 가장 작은 인덱스 부터 첫 커서로 삼습니다. 이후 나머지 오른쪽 원소들에 대해, 투 포인터를 실행하면, Big O 표기법상 N^2의 시간복잡도로 문제를 해결해 낼 수 있게 됩니다. <br/>

```c++
cin >> N;
    vector<long long> vec(N, 0);
    for(int i = 0; i < N; i++) cin >> vec[i];
    sort(vec.begin(), vec.end());

    vector<int> ans(3, 0);
    int second, third;
    long long sum = 3000000000;
    for(int first = 0; first < N - 2; first++) {
        second = first + 1; third = N - 1;
        while(second < third) {
            if(abs(sum) > abs(vec[first] + vec[second] + vec[third])) {
                sum = vec[first] + vec[second] + vec[third];
                ans = {first, second, third};
            }
            if(vec[first] + vec[second] + vec[third] == 0) break;
            else if(vec[first] + vec[second] + vec[third] < 0) {
                second++;
            } else {
                third--;
            }
        }
    }
    cout << vec[ans[0]] << " " << vec[ans[1]] << " " << vec[ans[2]] << '\n';
```
