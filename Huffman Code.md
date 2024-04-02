# 허프만 코드

허프만 코드는 데이터 압축을 위한 알고리즘이다.  

어떤 데이터를 압축할 때, 최대한 적은 Bit 갯수를 가전 결과물을 만들면 공간적으로 많은 절약이 될 것인데, 허프만 코드는 주어진 데이터를 최대한 짧은 길이의 이진 문자열로 만드는 것을 목표로 한다.

한 데이터엔 여러 문자들이 쓰일 것이다.

데이터 파일은 통상적으로 이진코드로 표현되는데, 각 문자들을 유일한 이진 문자열로 표현한 것을 **code word**라고 부른다.

데이터 encoding 결과의 총 길이는 어떻게 될까?

각 문자가 데이터에서 나타나는 갯수인 빈도 수와,

각 문자의 인코딩 결과인 코드 워드의 길이의 곱들을 모두 더한 값일 것이다.


![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/62cac87b-ba01-4b3f-9f5e-4a4180f7faa1)


그러니까 총 길이를 줄이려면, 

빈도수가 많은 문자일 수록 더 짧은 CodeWord를 부여 해주면 될 것이다. 

예를 들어 어떤 문자 a, b, c로만 이루어진 데이터가 있을 때

만약에 데이터가 ababcbbbc이고, 각 Code Word가 아래와 같을 때

- a : 00
- b : 01
- c : 11

인코딩 결과는 000100011101010111 이다.

허프만 코드는 빈도수를 반영하는데, b가 가장 많이 나오므로, b를 0으로둔다.

그리고, a와 b는 길이가 같으므로, 하나한테 1을 주면 될까?

아니다. 허프만 코드는 prefix code라는 전치코드의 형태로 이진코드를 작성하는데,

전치코드는 한 문자의 Code Word를 다른 문자의 Code Word의 prefix로 못 두게 한다.

따라서, a나 b 중 하나에 1을 줘버리면 다른 문자열들이 다 생성될 수 없으므로, a와 b에 10, 11을 부여해준다.

- a : 10
- b : 0
- c : 11

그 결과는 1001001100011 이다. 길이가 그리 길지 않은데도 5 ~ 6개나 절약되었다.

데이터가 큰 경우 이 차이는 매우 커질 것이다.

이런 허프만 코드는 priority queue를 통해 트리를 구성하면서 쉽게 만들 수 있다.

빈도수를 비교 대상으로 삼아, 빈도수가 더 작은 노드를 최상위에 유지한다.

처음엔 Queue에 문자와 빈도수를 넣어두며 시작한다.

그리고 매번 두 개의 노드를 꺼낸 다음, 새로운 노드 r을 만든다.

그리고 빈도수가 더 작은 노드를 왼쪽 자식 노드로, 더 큰 노드를 오른쪽 자식 노드로 이어준다.

왼쪽 자식으로의 간선에는 0을, 오른쪽 자식으로 가는 간선에는 1을 부여한다.

그리고 빈도수를 합쳐 현재 노드에 넣은 다음 다시 큐에 넣는다.

이 시행을 계속 반복하여 전체 트리를 만들면 된다.

그럼 리프 노드에 원래 문자들이 들어가는 꼴이 되는데,

리프 노드 부터 거꾸로 타고 올라오면서, Code Word를 읽어내면 된다.

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/8ce57115-099e-4753-8fea-6c68b593c93d)

코드는 아래와 같다.

i == n - 1일때, 두 노드를 합치는 수행이 n회 발생하고, 

이때 트리가 완성된다. (그래서 따로 확인이나 탈출 수행이 없다)

```c++
for (int i = 0; i <= n - 1; i++) {
	remove(PQ, p);
	remove(PQ, q);

	r = new nodetype;
	r -> left = p;
	r -> right = q;
	r -> freq = (p -> freq) + (q -> freq);
	insert(PQ, r);
}

remove(PQ, r);
return r;
```

앞서 언급한 대로 n회 트리를 만들고Top Node를 꺼내어 반환한다!

# 2. 귀납적 증명

코드 위의 사진을 보며 이해하면 좋다.

최적 코드에 해당하는 이진트리에서의 가지들임

### 1. 귀납 기초

0번째 단계는 최적 코드에 해당하는 이진트리의 가지들일 수 밖에 없다.

### 2. 귀납 가정

i번쨰 단계에서 만들어진 트리 집합을 최적 코드에 해당하는 이진트리의 가지들 T라고 가정하자.

### 3. 귀납 철차

허프만 알고리즘의 (i + 1) 번째 단계에서 만들어진 트리들의 뿌리들을 u, v라고 하자.

**만약 T에서 u, v가 형제 마디라면**,

 i + 1 단계에서 만들어진 트리집합이 T에서 가지들이므로 증명이 완료된다.

**u, v가 형제가 아닐 때**

u는 최소 v의 높이를 가졌고, 형제 노드 w가 존재한다고 가정하자.
그리고 i 단계 이후에 존재하는 트리들의 집합을 S라고 하자.

뿌리가 w인 T에서의 가지
→ S에 있는 트리 중 하나임 or 그러한 트리중 하나를 하위 트리로 포함함
둘 중 어느 경우던지 뿌리가 v인 트리는 
freq(w) ≥ freq(v)가 성립한다.

더욱이 T에서 depth(w) ≥ depth(v)가 성립한다.

T에서 v와 w를 뿌리로 하는 가지의 위치를바꾸어

새로운 이진트리 T’을 만들 수 있다.

bits(T’) = bits(T) + [depth(w) - depth(v)][freq(v) - freq(w)] 

       ≤ bits(T)

이는 T’에 해당하는 코드가 최적이라는 의미를 가진다.

명백하게 허프만 알고리즘의 (i + 1) 번째 단계에서 만들어진 트리 집합은 T’에 있는 가지이다.

그림으로 나타내면 아래와 같은 상황이다.

![image](https://github.com/binary-ho/Algorithm-and-Data-Structure/assets/71186266/272efa8c-706f-4131-a853-25f2b540dc27)

## Reference
- Foundation Of Algorithms <Richard Neapolitan>