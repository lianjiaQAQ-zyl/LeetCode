/*
 * @lc app=leetcode.cn id=921 lang=cpp
 *
 * [921] 使括号有效的最少添加
 */

// @lc code=start
#include <string>
using namespace std;
class Solution {
public:
    int minAddToMakeValid(string s) {
        int ans = 0;
        int left = 0;
        for(auto& c : s){
            if(c == '('){
                left++;
            }
            else if(c == ')'){
                if(left > 0){
                    left--;
                }
                else{
                    //没有左括号和这个括号匹配
                    ans++;
                }
            }
        }
        ans += left;
        return ans;
    }
};
// @lc code=end

