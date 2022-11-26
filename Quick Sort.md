# Quick Sort
Merge Sort와 함께 분할 정복 패러다임을 기반으로 만들어진 정렬이다. <br>
머지 소트는 수열을 가운데에서 쪼개 나가면서 정렬하고, 재귀적으로 합쳐 나가는 방향으로 정렬된다. <br>
이와 반대로 **퀵 정렬은 병합 과정이 필요 없도록 한쪽의 배열에 포함된 수가 다른 쪽 배열의 수보다 항상 작도록 배열을 분할한다.** <br>
이를 위한 Partition 단계는 배열에 있는 수 중 임의의 기준 숫자 pivot을 정해서 기준 보다 작거나 같은 숫자를 왼쪽, 더 큰 숫자를 오른쪽으로 보내는 단계다. <br>
병합 정렬의 분할 방식은 아주 단순하고 효율적이다.
상수시간 O(1) 만에 분할이 가능하다. 
대신 이들을 하나의 배열로 합치기 위한 병합 과정이 시간을 많이 잡아 먹는다. -> O(N) <br> <br>

퀵 정렬은 각 부분 수열의 맨 처음에 있는 수를 기준으로 삼고, 이들보다 작은 수를 왼쪽으로 큰 수를 오른 쪽으로 가도록 분할한다.  <br>
이 분할은 O(N)의 시간이 걸리는 무거운 작업이다. 
그리고 분할 과정에서 한 쪽으로 수열이 몰리는 상황이 벌어진다면, 비효율적인 분할을 할 수도 있다. <br>
대신에, 각 부분 배열이 이미 정렬된 상태가 되므로, 별도의 병합 작업이 필요 없다. <br>


시간이 많이 걸리는 작업을 분할 단계에서 하느냐, 병합 단계에서 하느냐의 차이가 있다.

## 2. 구현
기본적인 구현 형태이다. 굳이 분류하자면, 투 포인터와 분할 정복을 이용한 정렬이다. <br>
1. pivot을 처음에는 배열 가장 앞 숫자로 잡고, 
2. 그 다음 원소를 i, 그리고 배열 가장 끝 원소를 j로 둔다.
3. i++, j--를 통해 움직이며, **i의 경우 pivot 보다 큰 값을, j의 경우 pivot 보다 작은 값을 찾을 때까지 움직인다.** or 서로를 지나치거나 겹칠 때까지..
4. 발견했다면 둘의 원소 위치를 바꿔주고 i, j는 또 갈길을 간다
5. i, j가 엇갈리거나 만나면 종료하고, **종료 지점에 pivot을 둔다.**
6. 이렇게 하면 pivot을 사이로 **왼편은 전부 pivot 보다 작은 값, 오른편은 전부 pivot 보다 큰 값만 놓이게 된다!**
7. 이후 pivot을 기준으로 두 부분으로 나누어서 재귀적으로 분할 정복을 시행한다...

```cpp
void quickSort(int *arr, int start, int end){
	if(start >= end) return;
	
	int pivot = start;
	int i = pivot+1;
	int j = end;
	
	while(i<=j){
		while(i<=end && arr[i]<=arr[pivot]) ++i;
		while(j>start && arr[j]>=arr[pivot])  --j;

		if(i>=j) break;

    swap(i, j);
	}
  
  swap(pivot, j);
	
	quickSort(arr, start, j-1);
	quickSort(arr, j+1, end);	
}
```
## 3. 시간 복잡도
Qucik Sort의 시간 복잡도는 Merge Sort의 시간 복잡도와 비교하며 이해하면 더 좋다. <br>
Merge Sort의 시간 복잡도는 병합 과정에 지배된다. 한 번의 합병에 대해 N의 연산량이 발생한다. 
그리고 N개의 원소가 있는 배열을 원소가 1개가 될 때까지 이등분 하므로, log_2(N), 그러니까 O(logN)이 걸린다. <br>
따라서 Merge Sort의 시간 복잡도는 O(NlogN)이다. <br> 

Qucik Sort의 연산의 대부분은 Partition 과정에서 발생한다. 파티션은 주어진 수열의 길이에 비례하는 시간이 걸리므로, O(N)만큼 걸린다. <br>
**이제 관건은 분할 과정이 얼마나 일어나냐는 것이다.** 예를 들어서 운이 좋아 딱 절반의 크기로 계속 나뉘어 졌다고 생각해보자. <br>
그러면 결과는 Merge Sort와 같이 O(NlogN)으로 나올 것이다. 하지만 재수가 없어서 계속 한 개씩만 분할된다면 어떻게 될까? <br>
예를 들어 수열 `{1, 2, 3, 4, 5}`가 있을 떄, `{1, 2, 3,4}, {5}` 와 같은 분할만 계속해서 일어나는 것이다. 이후 `{1, 2, 3, 4}`는 `{1, 2, 3}, {4}`... <br>
이런 진행이라면 수열 전체 길이와 같은 O(N)회 분할이 일어날 수 있으므로 전체 시간 복잡도가 O(N^2)에 육박하게 될 수도 있다. <br>

<!-- ## 4. Qucik Sort 평균 시간 복잡도 증명
1. Let `T(n)`를 Quick Sort 함수가 N의 레코드를 갖는 리스트를 정렬할 때의 예상 시간이라고 하자.
2. in the call to QuickSort(list, 1, n), the pivot gets placed at position 'j'
- This leaves us with the problem of sorting two sublists of size j-1, and n-j. 이들의 예상 시간은 T(j-1), T(n-j)로 나타날 것이다.
- The remainder of the function clearly takes at most cn time for some constant 'c'
3. Since j may take on any of the values 1 to n with equal probability, we have

(for n \>= 2)
다 더해서 확률을 곱해주는 것이 포인트다. 수식에서 n으로 나누는 것을 단순히 n회니까 n으로 나눈다고 생각하지 말고, 확률을 곱해준다고 생각하는 것이 올바른 이해다. -->

## Reference

- 알고리즘 문제해결 전략 \<구종만 저>
- Fundamentals of Data Structures in C++ \<HOROWITZ, SAHNI, MEHTA 저>
- [퀵 정렬 원리부터 구현까지](https://swblossom.tistory.com/43)
