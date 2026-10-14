/*
 * @lc app=leetcode.cn id=1021 lang=cpp
 *
 * [1021] 删除最外层的括号
 */

// @lc code=start
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    string removeOuterParentheses(string s) {
        // 1. 拆分原语
        string cur = "", ans = "";
        int left = 0;
        for(auto& c : s){
            if(c == '('){
                left++;
                cur.push_back(c);
            }
            else if(c == ')'){
                left--;
                cur.push_back(c);
                if(left == 0){
                    //此时证明 cur 是一个原语
                    ans += cur.substr(1, cur.size() - 2);
                    cur = "";
                }
            }
        }
        return ans;
    }
};
// @lc code=end

