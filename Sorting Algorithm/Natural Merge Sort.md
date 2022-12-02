# Natural Merge Sort

네추럴 마지 소트는 머지 소트의 분할과 합병 과정을 최대한 줄인 버전이다! <br>
기존 머지 소트에서는 빠른 분할을 위해 배열의 중간점을 기준으로 계속해서 배열을 분할하였다. <br>
**이번에는 분할 과정에서 배열의 기존 나열 상태를 고려하면서 분할한다.** <br>
**애초에 정렬된 상태라면 굳이 나누지 않겠다는 것이다!** <br>
{1, 2, 3, 4}와 같은 배열이 있을 떄, 기존 merge sort에서는 당연히 분할한다. **하지만 자연 머지 소트는 이미 정렬된 상태와 같은 배열을 굳이 나누지 말자는 아이디어이다.
** <br>

이런 구현은 물론 **정렬이 잘 되어 있는지** 확인하는 과정에서 N의 수행시간이 더 소요될 것이 자명하다. <br>
하지만 확실하게 분할 과정을 줄여줄 수 있게 된다! 구체적인 코드와 함께 부연 설명하겠다.

```cpp
void mergeSort(int *num, int size) {
    vector<int> sub[2];
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

## Reference
- [github - kuoe0](https://gist.github.com/kuoe0)
