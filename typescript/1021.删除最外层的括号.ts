/*
 * @lc app=leetcode.cn id=1021 lang=typescript
 *
 * [1021] 删除最外层的括号
 */

// @lc code=start
function removeOuterParentheses(s: string): string {
    let cur = "", ans = "";
    let left = 0;
    for(const c of s){
        if(c === '('){
            left++;
            cur += c;
        }
        else if(c === ')'){
            left--;
            cur += c;
            if(left === 0){
                ans += cur.slice(1, -1);
                cur = "";
            }
        }
    }
    return ans;
};
// @lc code=end

