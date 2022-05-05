Union Find
====

코테 끝나고 정확히 정리하자


## 2. map으로 구현해본 버전
```c++
unordered_map<long long, long long> map;
int insert(int num) {
    auto itr = map.find(num);
    if(map[num] == 0) {
        map[num] = num + 1;
        return num;
    } else {
        map[num] = insert(itr->second);
        return map[num];
    }
}

```
    
재귀적으로 구현하였다. 문제는 [프로그래머스 호텔방 배정] 어디선가 많이 본 문제...    
문제는 예약하고 싶은 방의 번호가 이미 차 있다면 배치 가능한 방 중 가장 작은 숫자의 방에 배정하는 문제.       
내가 넣고 싶은 값이 없으면 그냥 넣고, 이미 있으면 value를 재귀적으로 조사한다. 여기서의 value는 배치된 끝 방이 되겠다.    
배치해주고, 재귀적으로 호출된 함수를 종료하면서 해당 union들의 value 값을 모두 마지막으로 배치해준 방 번호로 해주는 것이 포인트.    
그럼 같은 유니온들은 모두 해당 유니온의 가장 끝 번호를 알게 되는 것이라 아주 짧은 시간 안에 모든 탐색이 가능해진다.    
이런 식으로 활용 하는 것은 처음인데 좋은 것 같다. 잊지 말자.
