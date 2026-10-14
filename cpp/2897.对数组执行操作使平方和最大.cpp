/*
 * @lc app=leetcode.cn id=2897 lang=cpp
 *
 * [2897] 对数组执行操作使平方和最大
 */

// @lc code=start
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
class Solution {
public:
    int maxSum(vector<int>& nums, int k) {
        /*
        1. 考虑每一位
        (0, 0) -> (0, 0)
        (0, 1) -> (0, 1)
        (1, 0) -> (0, 1)
        (1, 1) -> (1, 1)

        2. 不管怎么操作，每一位中 1 的总个数不会发生变化
        
        3. a + b = (a & b) + (a | b)，而对于平方和函数，（a + b）^ 2 > a^2 + b^2，这意味
        应当尽可能使某个数字很大，这比将 1 的个数平均分配在两个数字中得到的总和要大，比如：
        10 和 01 应当变为 11 和 00，此时平方和会变大
        
        4. 所以问题转变为想办法使得 1 的位数越靠前越好
        */
       // dight[i] 表示第 i 位上 1 的个数
       const int MOD = 1e9 + 7;
       vector<int> dight(32, 0);
       for(int i = 0; i < nums.size(); ++i){
            int temp = nums[i];
            vector<int> weight;
            while(temp > 0){
                weight.push_back(temp % 2);
                temp = temp / 2;
            }
            for(int j = 0;j < weight.size(); ++j){
                dight[j] += weight[j];
            }
       }
       int ans = 0;
       while(k -- > 0){
            long long int num = 0;
            for(int i = dight.size() - 1;i >= 0;--i){
                if(dight[i] > 0){
                    dight[i]--;
                    num += (1 << i);
                }
            }
            long long int temp = ((num % MOD) * (num % MOD)) % MOD;
            ans = (ans + temp) % MOD;
       }
       return ans;
    }
};
// @lc code=end

