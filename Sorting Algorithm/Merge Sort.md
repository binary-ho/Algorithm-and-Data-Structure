# Merge Sort
분할 정복을 배우면서 가장 처음으로 배우게 되는 Merge Sort. 처음 배우던 시절 뇌가 굉장히 아팠었고, 로그함수 복잡도 알고리즘을 거의 처음 만나면서 충격 받았던 기억이 난다. <br> 
나에겐 익숙한 주제지만, 이번에 자료구조 과제를 위해 다시 한번 정리하고 코드를 구현해 보겠다. <Br>

Merge Sort는 대표적인 분할-정복 문제이다. 분할-정복 문제는 **크고 복잡한 문제들을 작게 분할해서, 분할한 내용들을 각기 정복해 나가는 컨셉이다.** <br>
옛 어르신들이 자주 쓰던 말을 이용해 말장난 해보면, 100개의 나뭇가지를 한번에 꺾는 것은 어렵지만, 하나 하나 부러트리는건 쉬우니, 나누어 보자는 것이다. <br>

Merge Sort 이전에 배운 정렬들을 떠올려 보면, 보통은 Bubble Sort를 가장 처음 배울 것이고, 이후엔 간단하게 Insertion Sort를 배웠던 것 같은데.. 이들은 선형적으로 배열 안을 이리 저리 옮겨 다니며, O(N^2)의 시간 복잡도를 갖는다. <br>

이런 일차원적인 비교 방식을 벗어나, 거대한 수열을 잘게 쪼게어서, 부분 수열들을 정렬한 다음 합쳐 보자는 컨셉이다! <br>
작게 쪼게어서 정렬한 다음, 어차피 붙이면서 또 정렬해야 할텐데 대체 왜 빠르다는 거지? 했던 기억이 난다. 이 부분은 다시 합쳐지는 과정의 구체적인 코드를 보며 설명하는 것이 낫다. <br>
먼저 전체 흐름을 설명하겠다.
일단, 크기가 100,000,000인 큰 수열 하나를 생각해 보자. 분할-정복 이전의 알고리즘들을 이용하면 최악의 경우 1억 초라는 큰 시간이 소요된다. <br> 

## 1. 분할
이런 거대한 문제를 작게 분할해서 살펴보자. **이 거대한 수열을 계속해서 반으로 나눌 것이다.** 배열의 가운데를 기점으로 왼편, 오른편으로 나눈 부분만을 보겠다는 것이다. <br>
인덱스가 0 ~ 9인 배열이 있다면, 0 ~ 4, 5 ~ 9와 같이 두 부분으로 나누어서 보자는 것이다. <br>
**이런 '분할' 행위를 원소가 1개가 될 때까지 시행하겠다.** 0 ~ 4를 또 0 ~ 2, 3 ~ 4 와 같이, 0 ~ 2를 0 ~ 1, 2와 같이 나누겠다는 것이다. <br>
```cpp
void partition(int start, int end) {
	if (start == end) return;
	int mid = (start + end) / 2;
	partition(start, mid);
	partition(mid+1, end);
	merge(start, end);
}
```
인덱스의 범위를 start와 end로 넣어 주면, 위와 같이 재귀적으로 계속 나누어진다. <br>
일단 계속 반으로 잘라 주면서 전체 배열 크기 N에 대해 log_2(N) 만큼의 시간이 소요된다.  -> O(logN)

## 2. 재귀적인 호출
이런 partition() 메서드에서의 재귀적인 흐름이 잘 이해가 가지 않을 수도 있어서 짧게 설명하겠다. <br>
호출되는 partition들을 임의로 p1, p2, p3 ... 와 같이 부르겠다. <br>
가장 처음 p1이 호출되면, 그 안에 partition의 재귀적 호출이 2개 있는데 이를 p2, p3라고 하자. 먼저 p2가 호출될 것이다. 그리고 p3가 호출되나? 이렇게 생각하면 머리가 아파진다. <br>
p2가 호출된 다음, **p2의 모~~~~든 수행이 끝나고 나서야 비로소 p3가 호출된다.** <br> 
즉, p3가 호출되기 이전에 p2안에 있는 두 partition p4, p5과 merge가 먼저 호출된다는 것이다! <Br>
예를 들어, partition(0, 16)이 호출된 다음, partition(0, 8), partition(9, 16) 순서로 호출되는 것이 아니라, partition(0, 16), partition(0, 8), partition(0, 4), partition(0, 2), partition(0, 1), partition(0, 0)의 순서대로 호출된다는 것이다!! <br>
그렇다면 0 ~ 0에서는 0 == 0이므로 바로 return이 호출된다. <br> 
	
이후 0 ~ 0, 1 ~ 1 의 첫 merge인 merge(0, 1) 호출된다! 이후엔 0 ~ 1과 2 ~ 2의 merge(0, 2) 호출될 것이고, 이러한 흐름은 계속 이어져 마지막엔 <br>
**p2가 끝나고 제대로 정렬된 0 ~ 8가 확보되고 나서야 p3 partition(9, 16)이 호출되고, 이 모든 과정이 끝나 제대로 정렬된 9 ~ 16가 모두 확보되고 나서야, p1에서의 merge(0, 16)이 호출된다는 것이다!!** <br>


## 3. 정복 (합병!)
**merge sort에서는 이후 합쳐주는 과정이 중요하다.** 정렬을 하면서 합쳐줄 것인데, 여기에서 겨우 N회만에 정렬해줄 것이다! <br>
이 부분은 구체적인 코드를 보자.
```cpp
void merge(int start, int end) {
  // 가운데 index를 점지
  int mid = (start + end) / 2;
	
  // i가 맨 앞, j가 가운데, idx는 buffer의 인덱스
  int i = start, j = mid +1, idx = start;

	while (i <= mid && j <= end) {
		if (arr[i] <= arr[j]) buffer[idx++] = arr[i++];
		else buffer[idx++] = arr[j++];
	}

	while (i <= mid)  buffer[idx++] = arr[i++]; 
	while (j <= end)  buffer[idx++] = arr[j++];

	for (int k = start; k <= end; k++) {
		arr[h] = buffer[h];
	}
}
```
버퍼를 쓰는 버전이다. <br>
i는 합쳐질 첫 번째 배열의 가장 왼쪽에서 부터 시작하고 j는 가운데 부터 시작한다고 되어있는데! **이것은 사실 합쳐질 두 번째 배열의 가장 왼쪽 인덱스다** <br>
전체 코드를 모르니 일단 그런 느낌만 알고 있자. <br>
자, 이제 buffer에 정렬된 상태의 배열을 넣어 두고, 다시 받아오자. buffer에서 사용할 index는 변수 idx이다. <br>
**전체 컨셉은 이렇다.** 합쳐질 두 배열의 가장 앞 원소 부터 체크한다. 오름차순 정렬을 기준으로 두 원소 중 더 작은 숫자를 buffer에 넣는다! <br> 이후 원소 하나를 buffer로 보낸 배열에서 사용하는 인덱스는 +1이 되어 한 칸 이동한다. <br> 예를 들어, {1, 2, 3}, {4, 5, 6}의 두 배열이 있다. i와 j는 각각 1과 4를 가리키고 있다는 것이다. 이 때, 1이 4보다 작으므로, buffer[idx]에는 1이 들어가고, idx++와 i++을 해주는 것이다. <br>
이후엔 2와 4의 비교가 될 것이다.. 이런 행위를 계속 반복하면 buffer에는 정렬된 형태의 숫자들이 놓일 것이다! <br>
그래서 첫 번째 while문을 보면, 크기를 비교하고, 더 작은 쪽을 buffer에 넣은 다음. buffer와 원본 배열의 인덱스를 ++하여 한 칸 이동해준다. <br>
while문의 범위는 왜 `while (i <= mid && j <= end)`일까? 이러한 행위를 두 배열 중 한 쪽의 원소를 모두 넣었을 때까지 진행하라는 것이다. <br>
이렇게 끊어주지 않는다면, 모든 원소를 소진한 쪽에서는 딱히 크기를 비교할 원소가 없어서 out of bound 에러가 날 것이다. <Br>
대신해서 2번째, 3번째 while문이 있는 것이다. <br>
한 쪽 배열이 모든 원소를 소진했다는 것은, 반대쪽 배열에서 남은 원소들은 전부 이미 소진된 배열의 원소들 보다 값이 큰 원소들만 남게 되었다는 것이다! <br>
**그리고 그 남은 값들은 이미 오름차순 정렬되어 있기 때문에 그대로 넣어주기만 하면 된다.** <br>
왜 이미 오름차순 정렬이 되어 있을까? 정복(합병) 과정에서 다루어지는 모든 배열들은 이미 직전 단계에서 합병 과정을 거치고 왔기 때문에, 이미 오름차순 배열되어 있다. <Br> 
원소가 1개일 때까지 나눠진 이후, 2개로 합쳐지면서, 4개로 합쳐지면서.. 계속 정렬되어서 올라왔기 때문에, 2번째 3번째 while문과 같이, 그저 남은 부분들을 buffer의 뒤에 붙여주면 된다. 


## Reference
- me
