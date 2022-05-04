#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;


int solution(vector<vector<int>> board) {
    int dist[26][26], arrowArr[26][26];
    int dy[4] = {0, 0, 1, -1};
    int dx[4] = {1, -1, 0, 0};
    int vs[5] = {1, 0, 3, 2, 4};
    int yNow, xNow, newY, newX, arrowNow, N = board.size(), costNow, newArrow, hate;
    for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) dist[i][j] = arrowArr[i][j]  = -1;

    queue<pair<int, int>> que;
    que.push({0, 0});
    dist[0][0] = 0;
    arrowArr[0][0] = 4;
    while(!que.empty()) {
        tie(yNow, xNow) = que.front();
        que.pop();
        //cout << yNow << xNow << distNow << arrowNow << '\n';
        arrowNow = arrowArr[yNow][xNow] >> 1;
        hate = vs[arrowArr[yNow][xNow]];
        for(int i = 0; i < 4; i++) {
            if(i == hate) continue;
            newY = yNow + dy[i]; newX = xNow + dx[i];
            //cout << newY << newX << i << '\n';
            if(newY < 0 || newY >= N || newX < 0 || newX >= N || board[newY][newX] == 1) continue;
            if(arrowNow == 2 || arrowNow == (i >> 1)) costNow = 100;
            else costNow = 600;
            if(dist[newY][newX] != -1 && dist[newY][newX] < dist[yNow][xNow] + costNow) continue;
            if(dist[newY][newX] == dist[yNow][xNow] + costNow && (i >> 1) != (arrowArr[newY][newX] >> 1)) newArrow = 4;
            else newArrow = i;
            //cout << newY << " " << newX << ": " << dist[newY][newX] << " -> ";
            dist[newY][newX] = dist[yNow][xNow] + costNow;
            arrowArr[newY][newX] = newArrow;
            //cout << dist[newY][newX] << '\n';
            que.push({newY, newX});
        }
    }
    //cout << dist[N-1][N-1];
    return dist[N-1][N-1];
}
