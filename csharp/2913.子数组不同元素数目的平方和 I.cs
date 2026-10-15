/*
 * @lc app=leetcode.cn id=2913 lang=csharp
 *
 * [2913] 子数组不同元素数目的平方和 I
 */

// @lc code=start
public class Solution {
    public int SumCounts(IList<int> nums) {
        // 遍历所有子数组
        int ans = 0;
        // 固定左端点
        for(int i = 0;i < nums.Count; ++i){
            var temp = new HashSet<int>();
            for(int j = i;j < nums.Count; ++j){
                temp.Add(nums[j]);
                ans += temp.Count * temp.Count;
            }
        }
        return ans;
    }
}
// @lc code=end

