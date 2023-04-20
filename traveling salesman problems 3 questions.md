# TSP 3 questions
TSP와 관련된 3가지 질의에 대한 답변을 작성해 보았다.

## 1. principle of optimality가 성립함을 보이시오.

<br>

![image](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FdpXden%2FbtsbEmREu8P%2F1FRbx0K7XlLxCds6Y6ZqR0%2Fimg.jpg)

<br>

## 2. recusive한 property를 갖는 식을 세우시오.

<br>

![image](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FmpUqY%2FbtsbtenJ9xn%2Fj0VxtIjWhhgSkOmOSt9dM0%2Fimg.jpg)

<br>

![image](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FcOG243%2FbtsbvpJwZxB%2FwFq3Updq68BG5FHQC8zBek%2Fimg.jpg)

<br>


## 3. 수도 코드를 작성하시오.
```c++
n : vertext  갯수
W : weight
P : P[i][A]는 A에 속한 모든 도시를 정확히 한번만 거쳐서 vi에서 v1으로 가는 최단 경로에서, vi 다음에 오는 첫 도시의 인덱스

void travel(int n, const number W[][], index P[][], number &minlength) {
  index i, j, k;
  number D[1 .. n][subset of V - {v1}];

  for (i = 2; i <= n; i++) {
    /* vi에서 v1로 아무곳도 거치지 않고 바로 가는 경우 */
    D[i][공집합] = W[i][1];
  }
  
  for (k = 1; k <= n - 2; k++) {
    for (A는 V - v1 집합 안에 있으며 원소가 k개인 모든 부분집합 A) {
      for (i는 1이 아니며 A에 속하지 않는 vertex 번호) {
        /*최소가 되는 j값을 찾아 아래 P에 넣기*/
        D[i][A] = minimum(W[i][j] + D[j][A - {vj}]);
        P[i][A] = 최소가 되는 j값;
      }
    }
  }

  /*최소가 되는 j값을 찾아 아래 P에 넣기*/
  D[1][V - {v1}] = minimun(W[1][j] + D[j][V - {v1, vj}]);
  P[1][V - {v1}] = j;
  minlength = D[1][N - {v1}];
}
```

중간의 3중 for문의 흐름을 이해하자.
k개의 노드가 있는 부분 집합 A를 만들어 가면서,
안에 포함된 vertex 번호 i들을 훑어가며 D[i][A]를 구해 나간다.

## Reference
- Foundation Of Algorithms \<Richard Neapolitan>
