#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
typedef pair<int, int> axis;

int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, -1, 1};

int N, cnt = 1;

int map[10][10];
int block_arr[10][10];    // 사각형 구역이 몇번째 구역인지 체크하기 위한 배열
bool check_square[9][10]; // 사각형 구역이 스도쿠 규칙을 만족했는지 체크하기 위한 배열

vector<axis> zero;
int zero_size;

axis pair_arr[10][10];
int pair_size;
bool check_pair[10][10];
bool check_domino[10][10];

bool all_stop = false;

// 각 column과 row가 스도쿠 규칙을 만족하는지 확인하기 위한 함수들
bool check_col(int y, int x, int num)
{
    for (int i = 1; i <= 9; i++)
    {
        if (map[i][x] == num)
        {
            return false;
        }
    }
    return true;
}
bool check_row(int y, int x, int num)
{
    for (int i = 1; i <= 9; i++)
    {
        if (map[y][i] == num)
        {
            return false;
        }
    }
    return true;
}

void Go(int now)
{

    if (all_stop == true)
    {
        return;
    }
    if (now == zero_size && all_stop == false)
    {
        all_stop = true;
        cout << "Puzzle " << cnt << '\n';
        cnt++;
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                cout << map[i][j];
            }
            cout << '\n';
        }
        return;
    }

    int zy = zero[now].first;
    int zx = zero[now].second;
    int block = block_arr[zy][zx];

    int py = pair_arr[zy][zx].first;
    int px = pair_arr[zy][zx].second;
    int p_block = block_arr[py][px];

    if (map[zy][zx] != 0)
    {
        Go(now + 1);
        return;
    }
    for (int i = 1; i <= 9; i++)
    {
        if (all_stop == true)
        {
            break;
        }
        if (check_col(zy, zx, i) == false)
        {
            continue;
        }
        if (check_row(zy, zx, i) == false)
        {
            continue;
        }
        if (check_square[block][i] == true)
        {
            continue;
        }
        map[zy][zx] = i;
        check_square[block][i] = true;
        for (int j = 1; j <= 9; j++)
        {
            if (j == i)
            {
                continue;
            }
            if (check_domino[i][j] == true)
            {
                continue;
            }
            if (check_col(py, px, j) == false)
            {
                continue;
            }
            if (check_row(py, px, j) == false)
            {
                continue;
            }
            if (check_square[p_block][j] == true)
            {
                continue;
            }
            map[py][px] = j;
            check_square[p_block][j] = true;
            check_domino[i][j] = true;
            check_domino[j][i] = true;
            Go(now + 1);
            check_domino[i][j] = false;
            check_domino[j][i] = false;
            check_square[p_block][j] = false;
            map[py][px] = 0;
        }
        check_square[block][i] = false;
        map[zy][zx] = 0;
    }
}

// 가능한 도미노 쌍을 만들어내는 함수
void pair_maker(int pair_now, int zero_now)
{
    if (all_stop == true)
    {
        return;
    }
    if (pair_now == pair_size)
    {
        Go(0);
        return;
    }
    else if (zero_now == zero_size)
    {
        return;
    }

    int zy = zero[zero_now].first;
    int zx = zero[zero_now].second;
    if (check_pair[zy][zx] == true)
    {
        pair_maker(pair_now, zero_now + 1);
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        int new_zy = zy + dy[i];
        int new_zx = zx + dx[i];
        if (1 <= new_zy && new_zy <= 9 && 1 <= new_zx && new_zx <= 9 && check_pair[new_zy][new_zx] == false)
        {
            if (all_stop == true)
            {
                return;
            }

            check_pair[zy][zx] = true;
            check_pair[new_zy][new_zx] = true;
            pair_arr[zy][zx] = make_pair(new_zy, new_zx);
            pair_arr[new_zy][new_zx] = make_pair(zy, zx);

            pair_maker(pair_now + 1, zero_now + 1);

            pair_arr[zy][zx] = make_pair(0, 0);
            pair_arr[new_zy][new_zx] = make_pair(0, 0);
            check_pair[new_zy][new_zx] = false;
            check_pair[zy][zx] = false;
        }
    }
}

// 풀이 전 준비과정
void init()
{
    memset(map, 0, sizeof(map));
    memset(check_pair, false, sizeof(check_pair));
    memset(check_square, false, sizeof(check_square));
    memset(check_domino, false, sizeof(check_domino));
    while (!zero.empty())
    {
        zero.pop_back();
    }
    all_stop = false;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (1)
    {

        init();
        cin >> N;
        if (N == 0)
        {
            return 0;
        }

        int num_input1, num_input2;
        string s_input1, s_input2;
        int y, y2, x, x2;

        for (int i = 0; i < N; i++)
        {
            cin >> num_input1 >> s_input1 >> num_input2 >> s_input2;

            y = s_input1[0] - 'A' + 1;
            x = s_input1[1] - '0';
            map[y][x] = num_input1;
            check_pair[y][x] = true;

            y2 = s_input2[0] - 'A' + 1;
            x2 = s_input2[1] - '0';
            map[y2][x2] = num_input2;
            check_pair[y2][x2] = true;

            check_domino[num_input1][num_input2] = true;
            check_domino[num_input2][num_input1] = true;
        }

        for (int i = 1; i <= 9; i++)
        {
            cin >> s_input1;
            y = s_input1[0] - 'A' + 1;
            x = s_input1[1] - '0';
            map[y][x] = i;
            check_pair[y][x] = true;
        }

        //
        int block_numbering;
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                if (map[i][j] == 0)
                {
                    zero.push_back(make_pair(i, j));
                }
                block_numbering = ((i - 1) / 3) * 3 + ((j - 1) / 3);
                block_arr[i][j] = block_numbering;
                check_square[block_numbering][map[i][j]] = true;
            }
        }
        zero_size = zero.size();
        pair_size = zero_size / 2;
        pair_maker(0, 0);
    }
}
