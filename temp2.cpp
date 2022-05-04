#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

// 오왼, 위아래, 자유
int solution(vector<vector<int>> board) {
    int dist[26][26];
    int arrowArr[26][26];
    int dy[4] = {0, 0, 1, -1};
    int dx[4] = {1, -1, 0, 0};
    int dy2[4] = {1, -1, 0, 0};
    int dx2[4] = {0, 0, 1, -1};

    //int vs[5] = {1, 0, 3, 2, 4};
    int yNow, xNow, newY, newX, arrowNow, N = board.size(), costNow, newArrow, hate;
    for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) dist[i][j] = -1;

    queue<pair<int, int>> que;
    que.push({0, 0});
    dist[0][0] = 0;
    arrowArr[0][0] = 2;
    while(!que.empty()) {
        tie(yNow, xNow) = que.front();
        que.pop();
        //cout << yNow << xNow << distNow << arrowNow << '\n';
        arrowNow = arrowArr[yNow][xNow];
        //hate = vs[arrowArr[yNow][xNow]];

        // 오왼에서 왔다
        if(arrowNow == 0) {
            for(int i = 0; i < 4; i++) {
                //if(i == hate) continue;
                newY = yNow + dy[i]; newX = xNow + dx[i];
                //cout << newY << " " << newX << " " << i << '\n';
                if(newY < 0 || newY >= N || newX < 0 || newX >= N || board[newY][newX] == 1) continue;
                if(i<2) costNow = 100;
                else costNow = 600;
                if(dist[newY][newX] != -1) {
                    if(dist[newY][newX] == dist[yNow][xNow] + costNow) {
                        //if(arrowArr[newY][newX] == 2) continue;
                        if (i < 2 && arrowArr[newY][newX] == 1) {arrowArr[newY][newX] = 2; que.push({newY, newX});que.push({yNow, xNow});}
                        else if (i >= 2 && arrowArr[newY][newX] == 0) {arrowArr[newY][newX] = 2; que.push({newY, newX});que.push({yNow, xNow});}
                        continue;
                    } else if(dist[newY][newX] < dist[yNow][xNow] + costNow) {
                        continue;
                    }
                    que.push({yNow, xNow});
                }
                if(i < 2) newArrow = 0;
                else newArrow = 1;
                //cout << newY << " " << newX << ": " << dist[newY][newX] << " -> ";
                dist[newY][newX] = dist[yNow][xNow] + costNow;
                arrowArr[newY][newX] = newArrow;
                //cout << dist[newY][newX] << '\n';
                que.push({newY, newX});
            }
        } else if(arrowNow == 1) {
            for(int i = 0; i < 4; i++) {
                //if(i == hate) continue;
                newY = yNow + dy2[i]; newX = xNow + dx2[i];
                //cout << newY << " " << newX << " " << i << '\n';
                if(newY < 0 || newY >= N || newX < 0 || newX >= N || board[newY][newX] == 1) continue;
                if(i<2) costNow = 100;
                else costNow = 600;
                if(dist[newY][newX] > -1) {
                    if(dist[newY][newX] == dist[yNow][xNow] + costNow) {
                        //if(arrowArr[newY][newX] == 2) continue;
                        if (i < 2 && arrowArr[newY][newX] == 0) {arrowArr[newY][newX] = 2;que.push({newY, newX});que.push({yNow, xNow});}
                        else if (i >= 2 && arrowArr[newY][newX] == 1) {arrowArr[newY][newX] = 2;que.push({newY, newX});que.push({yNow, xNow});}
                        continue;
                    } else if(dist[newY][newX] < dist[yNow][xNow] + costNow) {
                        continue;
                    }
                    que.push({yNow, xNow});
                }
                if(i < 2) newArrow = 1;
                else newArrow = 0;
                //cout << newY << " " << newX << ": " << dist[newY][newX] << " -> ";
                dist[newY][newX] = dist[yNow][xNow] + costNow;
                arrowArr[newY][newX] = newArrow;
                //cout << dist[newY][newX] << '\n';
                que.push({newY, newX});
            }
        } else {
            for(int i = 0; i < 4; i++) {
                //if(i == hate) continue;
                newY = yNow + dy[i]; newX = xNow + dx[i];
                //cout << newY << " " << newX << " " << i << '\n';
                if(newY < 0 || newY >= N || newX < 0 || newX >= N || board[newY][newX] == 1) continue;
                if(dist[newY][newX] > -1) {
                    if(dist[newY][newX] == dist[yNow][xNow] + 100) {
                        //if(arrowArr[newY][newX] == 2) continue;
                        if (i < 2 && arrowArr[newY][newX] == 1) {arrowArr[newY][newX] = 2;que.push({newY, newX});que.push({yNow, xNow});}
                        else if (i >= 2 && arrowArr[newY][newX] == 0) {arrowArr[newY][newX] = 2;que.push({newY, newX});que.push({yNow, xNow});}
                        continue;
                    } else if(dist[newY][newX] < dist[yNow][xNow] + 100) {
                        continue;
                    }
                    que.push({yNow, xNow});
                }
                if(i < 2) newArrow = 0;
                else newArrow = 1;
                //cout << newY << " " << newX << ": " << dist[newY][newX] << " -> ";
                dist[newY][newX] = dist[yNow][xNow] + 100;
                arrowArr[newY][newX] = newArrow;
                //cout << dist[newY][newX] << '\n';
                que.push({newY, newX});
            }
        }

    }
    //cout << dist[N-1][N-1];
    return dist[N-1][N-1];
}
