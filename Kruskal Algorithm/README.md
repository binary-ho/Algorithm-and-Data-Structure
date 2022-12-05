# Kruskal Algorithm
크루스칼 알고리즘은 Disjoint Set을 이용한 Union find 알고리즘을 통해 그리디하게 minimum spanning tree를 구성할 수 있는 알고리즘이다. <br>

[Union Find 설명 레포 바로가기](https://github.com/binary-ho/Algorithm-and-Data-Structure/tree/main/Union%20Find)

<br>

## 1. 신장 트리와 최소 신장 트리
신장트리라고도 불리우는 스패닝 트리란, 어떤 그래프를 구성하는 노드와 간선들이 있을 때, <br> **모든 노드를 포함하고, n-1개의 간선을 이용해 만드는, 사이클이 없는 트리다.** <br>
기존 그래프에서 간선들을 지워내서, 모든 노드가 연결되어 있으면서, n-1개의 간선만 사용되었고, 사이클이 없는 버전을 만들어 냈다면, 그것이 신장트리이다. <br>

**최소 스패닝 트리는, 스패닝 트리 중 최소 cost 간선만을 갖는 트리이다.** 여러 스패닝 트리 중에, 간선들의 비용 합이 가장 적은 버전이 최소 스패닝 트리이다! <br>

이는 그리디한 알고리즘들로 구성할 수 있는데, 대표적으로 Kruskal, Prim, Sollin알고리즘 등이 있다.

## 2. Kruskal Algorithm
기본적인 흐름은 다음과 같다. <br>

1. 간선들을 전부 모은다. 
2. 가중치가 낮은 간선들을 우선적으로 트리에 추가시킬 후보로 삼는다. 
3. 만약 새로 추가할 후보 간선을 파인드 연산을 통해 사이클을 형성하는지 확인한다.
4. 사이클을 만든다면 해당 간선을 채택하지 않고 버린다.
5. 만들지 않는다면 해당 간선을 유니온 연산으로 트리에 추가한다.

### 사이클 형성은 어떻게 확인할까?
경로 압축된 유니온 파인드는 기본적으로 모든 노드가 트리의 루트를 부모로 가리키고 있다. <br>
어떤 트리를 상상해보자. 그래프의 노드와 간선들로 어느 정도 완성 되어 있는 상태의 사이클이 없는 스패닝 트리인 것이다. <br>

새로 추가할 간선이 있다. 당연히 하나의 간선에는 두 노드가 연결되어 있다. <br>
만약 새로 추가될 간선에 이어져 있는 두 노드가, 같은 부모를 갖는다면?? <br> -> **이미 같은 트리 안에 있는 것이다!** <br>

한 스패닝 트리에 있는 두 노드를 잇는 간선이 스패닝 트리에 추가된다면, 사이클이 형성되는 것은 자명하다! <br>
어떤 스패닝 트리에 대해서, 파인드 연산을 통해 파악이 가능한데, **간선에 이어진 두 노드가 파인드의 결과로 같은 부모를 가리키고 있다면, 이것은 이미 같은 트리 안에 있는 것이라고 확인할 수 있다!** <br>

### 코드
위에서 설명한 내용을 모두 반영한 코드는 아래와 같다.
```cpp
// 그래프 크기는 R*C
int R, C, r1, c1, r2, c2;
vector<pair<pair<int, int>, pair<int, int>>> vec;

struct disjoint_set {
    vector<int> parent, rank;
    disjoint_set(int N) : parent(N), rank(N, 1) {
        for (int i = 0; i < N; i++) parent[i] = i;
    }

    int find(int u) {
      return u == parent[u] ? u : parent[u] = find(parent[u]);
    }
    
    // union
    void merge(int u, int v) {
        u = parent[u]; v = parent[v];
        if (u == v) return;

        if (rank[u] > rank[v]) swap(u, v);
        
        parent[u] = v;
        if (rank[u] == rank[v]) rank[v]++;
    }
};


void kruskal() {
    // 노드 갯수 크기의 disjoint_set 만들기
    disjoint_set sets(R * C);
    int ans = 0;
    for (int i = 0; i < vec.size(); i++) {
        tie(r1, c1, r2, c2) = vec[i];

        // 두 노드가 이미 같은 트리에 있다면 pass!
        if (sets.find(r1 * M + c1) == sets.find(r2 * M + c2)) continue;

        // 아니라면 합쳐준다!
        sets.merge(r1 * M + c1, r2 * M + c2);
    }
}
```

위에서 설명한 것과 같다. 
1. 모든 노드를 가질 수 있는 disjoint_set을 만든다.
2. vec이라는 이름의 vector가 모든 간선을 가지고 있다. <br> 앞에서 부터 비용이 적은 순으로 있다고 가정하자.
3. 모든 간선들을 꺼내서 파인드 연산을 해본다
4. 간선에 연결된 노드들이 같은 트리 안에 있다면 버린다.
5. 아닌 경우 합쳐준다
6. 모든 연산이 끝나면 최소 스패닝 트리가 완성 된다!!


## Reference
- 알고리즘 문제 해결 전략 \<구종만 저>
