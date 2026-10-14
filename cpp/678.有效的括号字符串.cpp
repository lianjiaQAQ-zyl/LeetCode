/*
 * @lc app=leetcode.cn id=678 lang=cpp
 *
 * [678] 有效的括号字符串
 */

// @lc code=start
#include <string>
#include <vector>
using namespace std;
class Solution {
    bool dfs(int n, int index, int left, string& s, vector<vector<int> >& state){
        // 递归终止条件
        if(left < 0){
            return false;
        }
        if(index == n){
            if(left == 0){
                return true;
            }
            else{
                return false;
            }
        }
        //查表
        if(state[index][left] != -1){
            return state[index][left] == 1;
        }
        // 计算
        bool res;
        if(s[index] == '('){
            res = dfs(n, index + 1, left + 1, s, state);
        }
        else if(s[index] == ')'){
            res = dfs(n, index + 1, left - 1, s, state);
        }
        else{
            res = dfs(n, index + 1, left + 1, s, state) || dfs(n, index + 1, left - 1, s, state) || dfs(n, index + 1, left, s, state);
        }
        // 存表
        state[index][left] = (int)res;
        return res;
    }
public:
    bool checkValidString(string s) {
        int n = s.size();
        int left = 0;
        int index = 0;
        // state[i][j] 表示下标为 i 时 left 为 j 时是否可能 
        // -1 未访问 0 不可能 1 可能
        vector<vector<int> > state(n, vector<int>(n + 1, -1));
        return dfs(n, index, left, s, state);
    }
};
// @lc code=end

