/*
 * @lc app=leetcode.cn id=2913 lang=cpp
 *
 * [2913] 子数组不同元素数目的平方和 I
 */

// @lc code=start
#include <vector>
#include <set>
#include <iostream>
using namespace std;
class Solution {
public:
    int sumCounts(vector<int>& nums) {
        // 1. 找到所有子数组并且去重
        vector<set<int> > sub_nums;
        int length = 1; // 子数组长度
        while(length <= nums.size()){
            int start = 0;
            while(start + length <= nums.size()){
                set<int> temp;
                for(int i = start;i < nums.size() && i < start + length; ++i){
                    temp.insert(nums[i]);
                }
                sub_nums.push_back(temp);
                start++;
            }
            length++;
        }
        // 2. 求和
        int ans = 0;
        for(int i = 0;i < sub_nums.size();++i){
            int temp = sub_nums[i].size();
            ans += temp * temp;
        }
        return ans;
    }
};
// @lc code=end

