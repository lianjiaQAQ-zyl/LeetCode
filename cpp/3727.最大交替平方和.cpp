/*
 * @lc app=leetcode.cn id=3727 lang=cpp
 *
 * [3727] 最大交替平方和
 */

// @lc code=start
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:
    long long maxAlternatingSum(vector<int>& nums) {
        //1. 按绝对值进行排序
        for(int i = 0;i < nums.size(); ++i){
            if(nums[i] < 0){
                nums[i] = -nums[i];
            }
        }
        sort(nums.begin(), nums.end());
        //2. 双指针，从首位取值
        int small = 0, big = nums.size() - 1;
        long long ans = 0;
        while(small < big){
            ans += nums[big] * nums[big] - nums[small] * nums[small];
            small++;
            big--;
        }
        if(small == big){
            ans += nums[small] * nums[small];
        }
        return ans;
    }
};
// @lc code=end

