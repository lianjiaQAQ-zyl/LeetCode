/*
 * @lc app=leetcode.cn id=856 lang=cpp
 *
 * [856] 括号的分数
 */

// @lc code=start
#include <string>
#include <vector>
#include <stack>
using namespace std;
class Solution {
public:
    int score(int left, int right, vector<int>& position){
        //返回 1
        if(right - left == 1){
            return 1;
        }
        // (A)
        if(position[left] == right){
            return 2 * score(left + 1, right - 1, position);
        }
        // AB
        int mid = position[left];
        return score(left, mid, position) + score(mid + 1, right, position);
    }
    int scoreOfParentheses(string s) {
        //计算每一个左括号匹配的右括号的位置
        vector<int> position(s.size(), -1);
        stack<int> temp;
        for(int i = 0;i < s.size();++i){
            if(s[i] == '('){
                temp.push(i);
            }
            else{
                int left = temp.top();
                temp.pop();
                int right = i;
                position[left] = i;
            }
        } 
        return score(0, s.size() - 1, position);
    }
};
// @lc code=end

