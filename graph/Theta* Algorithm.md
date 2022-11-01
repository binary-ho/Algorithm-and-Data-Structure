## 1. Theta* Algorithm 
Theta\* 알고리즘의 도입 배경과, 더 나은 점, 그리고 롤에서 비슷한 알고리즘이 적용되었다고 추측하는 이유, A\*와 다른 점인 line-of-sight 개념, 그리고 Theta*의 수도코드를 분석해보자. 
<br> <br>

Theta* Algorithm은 A* 알고리즘을 개선시킨 수 많은 바리에이션 중 하나이다. <br>
A*를 게임에 적용할 때 경로에 관한 최적화인데, 다음과 같은 경우를 생각해보자. <br>

스타크래프트나 롤의 경우를 생각해보자. 우리는 어떤 방향이든 이동할 수 있지만, 컴퓨터의 연산을 위해 내부적으로는 **그리드 형태로 맵을 쪼갠 다음, 최단거리 연산을 할 수 밖에 없다.** <br>

디지털 세상의 한계일 수 있다.
좀 더 풀어서 말 하자면, 우리는 상하좌우 대각선 뿐만 아니라, 1도, 41도, 77도, 132도 등 다양한 각도로 움직일 수 있다. 하지만, 그래프 탐색을 위해, 맵을 자른다고 생각해 보면, **그 형태는 체스판과 같을 수 밖에 없다.** <br>

아래 그림을 보자.

![theta 1](https://user-images.githubusercontent.com/71186266/199237948-b29fd0cd-8033-4afb-9aef-0609df64007f.png)

(출처: [1]) <br>

검은색은 A star 알고리즘의 결과로 만들어진 경로이다. 왼쪽 아래가 시작 점이고, 오른쪽 위가 도착 지점이다. <br> 

왼쪽 하단의 첫 번째로 마주하는 장애물을 지나고 계속 직진한 다음 대각선으로 꺾는다. 실제 세상에서 저렇게 장애물들이 배치되어 있을 때, 우리는 저런 식으로 움직일까? <br>

**아니다.** 분명 첫 번째 장애물을 벗어나자 마자, 도착지점 쪽으로 바로 꺾는 것이 더 자연스럽다. 

![theta 2](https://user-images.githubusercontent.com/71186266/199239054-d3e814f4-8771-4bf4-98fe-74e1d5fbbc38.png)
(출처: [1]) <br>


위의 노란색 화살표 처럼 말이다. 기존의 움직임은 상당히 부자연스럽고, 또 조금은 돌아가는 꼴이 된다. <br>



바로 생각이 들었을 수도 있지만, 정말 작은 공간으로 자르면, 아날로그와 같은 움직임을 낼 수도 있다. 캐릭터 보다도 훨신 작은 칸들을 만들어서 연산한다면, 더욱 부드러운 움직임을 낼 수도 있다. <br> 

하지만 당연히 시간복잡도가 무시무시하게 증가한다. 네모 한 칸을 4등분만 해도 시간 복잡도와 공간 복잡도가 얼마나 증가할지 바로 생각이 들 것이다. <br>

이런 상황을 위해 제시된 것이 **Theta* 알고리즘이다.** <br> 
theta star는 게임에 적용되기 딱 좋은 알고리즘인데, A star의 휴리스틱에 **line-of-sight** 개념이 추가된다. <br>

전체적인 진행은 A Star 알고리즘과 유사하다. (A star 알고리즘 설명 링크) **현재 탐색중인 노드의 다음 탐색 노드와, 현재 노드의 부모가, line-of-sight를 가질 경우, 현재 노드 대신 부모 노드와의 거리와 휴리스틱을 적용한다.** <br>

적용할 경우 아래와 같은 움직임을 보인다.

![theta 3](https://user-images.githubusercontent.com/71186266/199240769-bf9a957c-c2ca-4899-ba6f-a5af9d1079fb.png)
(출처: [1]) <br>

움직임이 훨신 부드러워 졌다. <br>

사실 처음에 롤에 해당 알고리즘이 적용되었다고 해서 공부했으나, 사실은 밝힌 적이 없는 것으로 밝혀졌다. 이 또한 추측이였다. 많은 사람들은 어떤 A\*의 바리에이션 중 무언가가 적용된 것으로 추측될 뿐이다. 아마 이와 비슷한 알고리즘이 사용되었을 것으로 추측된다. 실제 전략 게임에 많이 쓰이는 점프 포인트 서치도 A\*의 바리에이션인데, 결국은 꺾을만한 지점, 점프 포인트들을 찾아서 이어준다. <br>

## 2. 롤에서의 패스 파인딩
롤에서도 유사한 알고리즘이 적용되었을 것으로 추측된다. 미니언과 AI 봇 챔피언은 움직임이 위에서 제시한 a star와 굉장히 유사하다. 꺾어야 할 지점에서 꺾지 않고 뚝딱거리며, 경로 변경시 잠시 머뭇거리기도 한다. 그래서 기본적으로는 A star가 적용되었을 것 같다. 다만 순수 A star 보다는 D star일 것으로 추측된다. 아래의 그림을 보자. 
![d star 11](https://user-images.githubusercontent.com/71186266/199243520-8d84c8ce-40df-4bc5-9e67-e0930c737e85.png)
![d star 22](https://user-images.githubusercontent.com/71186266/199243531-8cfae63c-cd74-44d6-adaa-83ba86cdef9a.png)

벽을 만드는 챔피언인 애니비아로, 따라오는 봇의 경로를 빠르게 막아 보았다. 아주 짧은 찰나에 방향을 트는데, A star에서 동적으로 바뀌는 경로에 빠르게 반응 할 수 있는 Dynamic A star를 사용했을 것 같다. 물론 다시 연산했을 수도 있지만, 거의 순식간에 방향을 바꾸었기 떄문에 개선된 버전을 사용했을 것으로 추측된다.

<br> <br>

챔피언의 이동은 어떨까? 생각보다 엄청나게 부드럽게 이동했다. 아래는 이동 경로를 남기는 캐릭터로 실험을 해 본 스크린샷이다.

![Screen14](https://user-images.githubusercontent.com/71186266/199244703-ccc9b768-1e35-4b81-aaab-c8206f508377.png)
![Screen05](https://user-images.githubusercontent.com/71186266/199244708-bd5e8e24-1d38-43ee-9c0a-0edce3f6a9a4.png)
![Screen06](https://user-images.githubusercontent.com/71186266/199244712-65ec8cc9-cffc-4c14-bff5-49457fecbc70.png)
![Screen07](https://user-images.githubusercontent.com/71186266/199244714-3e21bab9-95ec-4c84-9bb2-45bcd31957f3.png)

찍느라 힘들었다. 꺾는 지점이나, 둥근 벽을 탈 때 정말 부드럽게 이동하는 모습을 관찰 할 수 있다. 이런 이유로 세타*와 유사한 알고리즘을 도입했다고 추측해보고 있다.

## 3. line of sight
line of sight는 가시선으로 직역할 수 있다. 세타 스타에서 후처리, 평활화에 쓰인다. 

![loss](https://user-images.githubusercontent.com/71186266/199245920-b6c47c12-f306-4a2c-ac22-d20c925ce791.png)
(출처 [2]) <br>


위는 일종의 슬라이더가 적용된 LOSS(line of sight slider) 알고리즘의 그림이다. point 2의 부모 노드인 point 3에서 부터 내려오던 중 point 1이 보이게 되면, 해당 노드를 현재 노드로 바꾼다는 아이디어이다. <br>

어떤 los 알고리즘을 넣든간에 아이디어는 이렇게 **어떤 지점에서 목적지가 보이면, 다른 노드를 거치지 않고 곧장 가도록 설정한다.** <br>

Theta\*에서는 이를 탐색하며 바로 바로 적용하기 보다는, 이미 만들어진 경로에서, **부모 노드가 현재 탐색중인 다음 노드를 가리는 장애물 없이 볼 수 있다면, 다음 노드까지 현재 노드에서 출발하지 말고 부모 노드에서 출발하도록 한다.** <br>
즉, **후처리이다.** 이러한 수행이 연속해서 계속 이루어지면, 
![theta 3](https://user-images.githubusercontent.com/71186266/199240769-bf9a957c-c2ca-4899-ba6f-a5af9d1079fb.png)
(출처 [1]) <br>

검은색으로 되어 있는 경로들이 점점 노란색 경로로 바뀌게 되는 것이다. <br>

오른쪽 그림은 몇 개의 검은 점이 있고, 이를 노란색으로 바로 잇는다. 저 검은 점과 점 사이에는 원래 A star와 유사한 경로가 형성되어 있었을 것이다. 이를 후처리를 통해 지워나간 것이다.


## 4. Theta* 수도 코드
이제 어떻게 진행되는지 감이 왔을 테니, 수도 코드를 보자.

```c++
function theta*(start, goal)
    gScore(start) := 0
    parent(start) := start

    open := {}
    open.insert(start, gScore(start) + heuristic(start))

    closed := {}
    while open is not empty
        s := open.pop()
        if s = goal
            return reconstruct_path(s)
        closed.push(s)
        for each neighbor of s
            if neighbor not in closed
                if neighbor not in open
                    gScore(neighbor) := infinity
                    parent(neighbor) := Null
                update_vertex(s, neighbor)
    return Null
            
    
function update_vertex(s, neighbor)
    // This part of the algorithm is the main difference between A* and Theta*
    if line_of_sight(parent(s), neighbor)
        if gScore(parent(s)) + c(parent(s), neighbor) < gScore(neighbor)
            gScore(neighbor) := gScore(parent(s)) + c(parent(s), neighbor)
            parent(neighbor) := parent(s)
            if neighbor in open
                open.remove(neighbor)
            open.insert(neighbor, gScore(neighbor) + heuristic(neighbor))
    else
        if gScore(s) + c(s, neighbor) < gScore(neighbor)
            gScore(neighbor) := gScore(s) + c(s, neighbor)
            parent(neighbor) := s
            if neighbor in open
                open.remove(neighbor)
            open.insert(neighbor, gScore(neighbor) + heuristic(neighbor))
```
(출처 - 위키피디아 Theta*) <br>

전체적으로는 A star와 그 진행이 유사하다. 우선순위 큐 open과 방문 체크 배열 closed를 이용해 다익스트라를 진행한다. <br>

이제 함수 update_vertex의 첫 번째 if문을 보자. 여기가 다른 지점이다. <br>
**line_of_sight가 true일 경우, 다음 노드를 open에 넣을 때, 현재 노드 s와 neighbor의 가중치 + 휴리스틱 값을 넣어주는 것이 아니라! `부모 노드 까지의 코스트 + neighbor의 가중치 + 부모 to neighbor의 휴리스틱 + neighbor와 목적지의 휴리스틱`을 넣어준다!** <br>

line_of_sight가 false일 경우 노드를 넣기 전 A star와 같이 휴리스틱을 통한 목적지까지의 거리 추정값만을 더해준다. \

<br>

이런 세타 스타 알고리즘은 그리드 형태의 그래프에서의 패스 파인딩, 패스 플래닝을 상당히 자연스럽고 빠른 경로로 이동하게끔 만들어준다. <br> **단, 후처리가 들어가므로 자연히 동작 시간은 더 느려진다.**

![comparison](https://user-images.githubusercontent.com/71186266/199248565-803681b6-96e2-4f5d-acc0-5ef2d00c01f5.png)
(출처: [1])
## Reference
[1] Applying Theta* in Modern Game <Hoa Nguyen, Nguyen Truong, Chinho Park, Wonshoup So, Lee Ki-Dong> <br> 
[2] [Line-of-Sight Slider (LoSS) Smoothing <Nickson Joram>](https://medium.com/nerd-for-tech/line-of-sight-slider-loss-smoothing-5ee2c90af58d) <br> 
[3] [Theta* - Wikipedia](https://en.wikipedia.org/wiki/Theta*) <br> 
[4] [A* - Wikipedia](https://en.wikipedia.org/wiki/A*_search_algorithm) <br> 
[5] 게임 리그오브레전드 <br> 
