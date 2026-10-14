/*
 * @lc app=leetcode.cn id=1021 lang=csharp
 *
 * [1021] 删除最外层的括号
 */

// @lc code=start
public class Solution {
    public string RemoveOuterParentheses(string s) {
        string cur = "", ans = "";
        int left = 0;
        foreach(char c in s){
            if(c == '('){
                left++;
                cur += c;
            }
            else if(c == ')'){
                left--;
                cur += c;
                if(left == 0){
                    ans += cur.Substring(1, cur.Length - 2);
                    cur = "";
                }
            }
        }
        return ans;
    }
}
// @lc code=end

