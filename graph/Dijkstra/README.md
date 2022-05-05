다익스트라 알고리즘
===
블로그에도 설명 해둔 알고리즘 <https://dwaejinho.tistory.com/19>

잘 알려진 3가지 최단 경로 알고리즘 중 가장 빠른 탐색도를 가짐.    
priority_queue를 통해 구현 했을 때 O(ElogV)의 속도를 자랑한다.    
최악의 경우 모든 간선을 전부 탐색하기 때문에 (값을 갱신해야 해서) O(E)만큼 걸리고,    
E만큼의 크기를 가지는 heap정렬 때문에 O(logE)만큼 걸리기 때문에 O(ElogE)라고 할 수 있다.    
이 때 V^2는 보통 E 보다 작으므로 이를 반영하여 **O(ElogV)로 나타낼 수도 있다.**

출발 지점을 src라고 했을 때,

```c++
for(int i = 0; i < N; i++) {
    dist[i] = 987654321;
}
priority_queue<pair<int, int>> que;
dist[src] = 0;
que.push({-dist[src], src});
while (!que.empty()) {
    int dist_now = -que.top().first; 
    int node_now = que.top().second;
    que.pop();
    if(dist[node_now] < dist_now) continue;
    for (int i = 0; i < vec[node_now].size(); i++) {
        int next_node = vec[node_now][i].first;
        int cost_now = vec[node_now][i].second;
        if (dist[next_node] <= dist_now + cost_now) continue; //  이 부분 실수하기 쉬움 조심 메모리 초과 나온다.
        dist[next_node] = dist_now + cost_now;
        que.push({-dist[next_node], next_node});
    }
}
```

자주 쓰는 알고리즘이기 때문에 요약하겠다.    
출발 지점은 src이고, src로 부터 도착 지점까지의 최단 거리를 나타낸 배열이 dist이다.
1. src를 제외한 모든 dist 배열의 값을 아주 큰 값으로 설정한다. dist[src] = 0;으로 설정한다. 더 짧은 거리를 찾으면 갱신 시켜줄 것이다.
2. 탐색을 시작한다 c++의 priority_queue는 오름차순 구현이기 때문에 거리의 누적합을 넣을 때 -를 붙여준다.
3. que에서 원소를 꺼낸 다음 dist를 확인하는데 내가 이미 해당 도착지점으로의 더 짧은 dist를 알고 있다면 그대로 continue;
4. 아니라면 탐색을 시작한다. 이어진 노드들을 확인 하는데 이동 cost와 누적합의 합이 다음 dist가 된다.
5. 이번에도 이 값과 원래 알고 있는 값을 비교하여, 원래 알고 있는 값 보다 이번 값이 더 작을 경우에만 값을 갱신해주고 queue에 push해준다.

끝!
