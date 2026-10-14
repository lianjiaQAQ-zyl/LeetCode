/*
 * @lc app=leetcode.cn id=3723 lang=cpp
 *
 * [3723] 数位平方和的最大值
 */

// @lc code=start
#include <string>
using namespace std;
class Solution {
public:
    // a ^ 2 + b ^ 2 <= (a + b) ^ 2
    // 因此将 sum 拆分时保证每一位尽可能大即可，即 sum - 9
    // 这样得到的分数一定是大于拆成多位的
    string maxSumOfSquares(int num, int sum) {
        string ans = "";
        while(num > 0 ){
            if(sum > 9){
                ans.push_back('0' + 9);
                sum -= 9;
            }
            else if(sum > 0){
                ans.push_back('0' + sum);
                sum -= sum;
            }
            else{
                ans.push_back('0');
            }
            num--;
        }
        if(sum != 0) return "";
        return ans;
    }
};
// @lc code=end

