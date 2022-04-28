#include <string>
#include <vector>
#include <map>

using namespace std;

vector<int> solution(vector<string> gems) {
    map<string, int> gems_map;
    vector<int> answer(2, 1);

    for(const string& str: gems) {
        gems_map[str];
    }
    int gems_kinds = gems_map.size();
    gems_map.clear();
    int left = 0, right = 0, ans_range = 100001, temp;
    gems_map[gems[0]]++;
    while(left < gems.size()) {
        if(gems_map.size() == gems_kinds) {
            if(ans_range > right - left) {
                ans_range = right - left;
                answer[0] = left + 1;
                answer[1] = right + 1;
            }
        }

        if(right < gems.size() - 1 && gems_map.size() < gems_kinds) {
            right++;
            gems_map[gems[right]]++;
        } else {
            if(gems_map[gems[left]] == 1) {
                gems_map.erase(gems[left]);
            } else {
                gems_map[gems[left]]--;
            }
            left++;
        }
    }
    return answer;
}
