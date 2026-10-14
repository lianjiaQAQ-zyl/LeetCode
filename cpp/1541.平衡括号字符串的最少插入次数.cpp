/*
 * @lc app=leetcode.cn id=1541 lang=cpp
 *
 * [1541] 平衡括号字符串的最少插入次数
 */

// @lc code=start
#include <string>
#include <iostream>
using namespace std;
class Solution {
public:
    int minInsertions(string s) {
        int need = 0;
        int ans = 0;
        for(int i = 0;i < s.size(); ++i){
            if(s[i] == '('){
                //判断 need 的奇偶，need 为奇数说明前面只有一个 )，需要补充
                if(need % 2 == 1){
                    need--;
                    ans++;
                }
                need += 2;
            }
            else{
                //右括号
                if(need == 0){
                    //证明此时这个右括号前面无法匹配
                    ans++;
                    need += 2;
                }
                need--;
            }
            //cout << need << endl;
        }
        ans += need;
        return ans;
    }
};
// @lc code=end

