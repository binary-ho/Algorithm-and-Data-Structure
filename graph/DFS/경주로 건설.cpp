#include <iostream>
#include <vector>

// 프로그래머스 경주로 건설 lv3

// 시작 점 부터 도착 점까지 경로를 그리는 문제
// 다만, 직선 경로이냐, 코너가 있냐에 따라서 cost가 다른 점이 포인트

// So, 완전 탐색을 두 번 돌린다.
// 경로가 꺾이냐, 지금은 최적이 아니여도, 결국엔 최적일 수가 있는 문제여서
// 두 번 돌리지 않으면 정답이 나오지 않아 이렇게 구현했다.

using namespace std;
int N, dist[25][25],dist2[25][25];
int dy[4] = {0, 0, 1, -1};
int dy2[4] = {1, -1, 0, 0};
int dx[4] = {1, -1, 0, 0};
int dx2[4] = {0, 0, 1, -1};
vector<vector<int>> boardCopy;
void dfs(int y, int x, int arrow, int costNow) {
    int newY, newX, cost;
    for(int i = 0; i < 4; i++) {
        newY = y + dy[i]; newX = x + dx[i];
        if(newY < 0 || newY >= N || newX < 0 || newX >= N || boardCopy[newY][newX] == 1) continue;
        if(arrow == 0) {
            cost = i < 2 ? 100 : 600;
        } else if(arrow == 1) {
            cost = i >= 2 ? 100 : 600;
        } else {
            cost = 100;
        }
        if(dist[newY][newX] != -1 && dist[newY][newX] < costNow + cost) continue;
        if(dist[newY][newX] == -1 || dist[newY][newX] > costNow + cost) dist[newY][newX] = costNow + cost;
        costNow += cost;
        dfs(newY, newX, i >> 1, costNow);
        costNow -= cost;
    }
}
void dfs2(int y, int x, int arrow) {
    int newY, newX, cost;
    for(int i = 0; i < 4; i++) {
        newY = y + dy2[i]; newX = x + dx2[i];
        if(newY < 0 || newY >= N || newX < 0 || newX >= N || boardCopy[newY][newX] == 1) continue;
        if(arrow == 0) {
            cost = i < 2 ? 100 : 600;
        } else if(arrow == 1) {
            cost = i >= 2 ? 100 : 600;
        } else {
            cost = 100;
        }
        if(dist2[newY][newX] != -1 && dist2[newY][newX] < dist2[y][x] + cost) continue;
        if(dist2[newY][newX] == -1 || dist2[newY][newX] > dist2[y][x] + cost) dist2[newY][newX] = dist2[y][x] + cost;
        dfs2(newY, newX, i >> 1);
    }
}

int solution(vector<vector<int>> board) {
    N = board.size();
    //boardCopy = board;

    for(int i = 0; i < N; i++) {
        boardCopy.emplace_back(board[i]);
    }
    for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) dist[i][j] = dist2[i][j] = -1;
    dist[0][0] = dist2[0][0] = 0;
    dfs(0, 0, 2, 0);
    dfs2(0, 0, 2);
    return min(dist[N-1][N-1], dist2[N-1][N-1]);
}
