# Natural Merge Sort

네추럴 마지 소트는 머지 소트의 분할과 합병 과정을 최대한 줄인 버전이다! <br>
기존 머지 소트에서는 빠른 분할을 위해 배열의 가운데 지점을 기준으로 계속해서 배열을 거의 비슷한 크기의 두 배열로 계속해서 분할하였다. <br>
이번에는 분할 과정에서 바로 분할하는 것이 아니라 **배열의 기존 상태를 고려하면서 분할한다.** <br>
뭘 어떻게 고려하냐면, **애초에 정렬된 상태라면 굳이 나누지 않겠다는 것이다!** <br>
{1, 2, 3, 4}와 같은 배열을 보자. 이 배열은 이미 정렬 되어 있다. 기존 merge sort에서는 당연히 2분의 1로 분할해 버리는데, **자연 머지 소트는 배열이 이미 정렬되어 있다면 굳이 나누지 말자는 아이디어이다.** <br>

이런 구현은 물론 **정렬이 잘 되어 있는지 확인하는 과정에서 N의 수행시간이 더 소모 되는 것이 자명하다.** <br>
하지만 확실하게 분할 횟수와 합병 횟수를 줄여준다! Big O 표기법으로 따지면, 당연히 시간 복잡도는 기존 머지 소트와 같겠지만, 어느 정도 정렬 되어있다면, 평균적으로 더 빠를 것이다. 구체적인 코드와 함께 부연 설명하겠다.

```cpp
void mergeSort(int *num, int size) {
    int left[size], right[size], leftSize = 0, rightSize = 0, now;
    int cur = num[0], p = 0;
    for (int i = 0; i < size; i++) {
        if (num[i] < cur) p ^= 1;
        if (p == 0) {
            left[leftSize++] = cur = num[i];
            continue;
        }
        right[rightSize++] = cur = num[i];
    }

    if (rightSize == 0) return;
    mergeSort(left, leftSize);
    mergeSort(right, rightSize);

    int leftIdx = 0, rightIdx = 0, arrayIdx = 0;
    while (leftIdx < leftSize && rightIdx < rightSize) {
        if (left[leftIdx] < right[rightIdx]) num[arrayIdx++] = left[leftIdx++];
        else num[arrayIdx++] = right[rightIdx++];
    }
    while (leftIdx < leftSize) num[arrayIdx++] = left[leftIdx++];
    while (leftIdx < leftSize) num[arrayIdx++] = left[leftIdx++];

    for (int i = 0; i < size; i++) {
        cout << num[i] << " ";
    }
    cout << '\n';
}
```
(원래는 링크드 리스트와 같은 자료구조로 구현하는 것이 더 빠르고 공간도 적게 차지한다.)<br>
잘 보면 그냥 머지 소트처럼 반으로 분할 하는 것이 아니라, 두 배열을 p라는 변수를 이용해 오가면서 채워나간다. <br> 
오름차순 정렬이 깨지는 경우엔 다른 배열에 원소를 채우기 위해 p값을 toggle 하고, 유지되는 동안은 같은 배열에 원소를 채우는 행위를 계속해서 반복한다. <Br>
이렇게 완성된 코드는 배열의 원소를 한번 훑는 시간 복잡도와 두 배열을 만들어야 하는 만큼 메모리가 소모되지만, 평균적인 재귀적 분할 과정 자체의 횟수를 크게 줄일 수 있다. 

## Reference
- [github - kuoe0](https://gist.github.com/kuoe0)
