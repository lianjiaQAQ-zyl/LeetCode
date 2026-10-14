/*
 * @lc app=leetcode.cn id=301 lang=cpp
 *
 * [301] 删除无效的括号
 */

// @lc code=start
#include <string>
#include <vector>
#include <set>
using namespace std;
class Solution {
public:
    bool isValid(const string& s){
        int left = 0;
        for(auto& c : s){
            if(c == '(') left++;
            else if(c == ')'){
                left--;
                if(left < 0){
                    return false;
                }
            }
        }
        return left == 0;
    }
    void dfs(int index, string& cur, const string& s, int left, int right, set<string>& ans){
        // 递归返回条件
        if(index == s.size()){
            if(left ==0 && right == 0){
                if(isValid(cur)) {
                    ans.insert(cur);
                }
            }
            return;
        }
        //删除括号
        if(s[index] == '('){
            // 删除
            if(left > 0){
                dfs(index + 1, cur, s, left - 1, right, ans);
            }
            // 保留
            cur.push_back(s[index]);
            dfs(index + 1, cur, s, left, right, ans);
            //cur.pop_back();
        }
        else if(s[index] == ')'){
            if(right > 0){
                dfs(index + 1, cur, s, left, right - 1, ans);
            }
            cur.push_back(s[index]);
            dfs(index + 1, cur, s, left, right, ans);
            //cur.pop_back();
        }
        else{
            cur.push_back(s[index]);
            dfs(index + 1, cur, s, left, right, ans);
            //cur.pop_back();
        }
    }
    vector<string> removeInvalidParentheses(string s) {
        int balance = 0, left = 0, right = 0;
        for(auto& c : s){
            if(c == '('){
                balance++;
            }
            else if(c == ')'){
                if(balance == 0){
                    right++;
                }
                else{
                    balance--;
                }
            }
        }
        left = balance;
        //需要删除 left 个 '(' 和 right 个 ')'
        int index = 0;
        string cur = "";
        set<string> ans;
        dfs(0, cur, s, left, right, ans);
        return vector<string>(ans.begin(), ans.end());
    }
};
// @lc code=end

