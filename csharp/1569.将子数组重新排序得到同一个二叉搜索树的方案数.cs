/*
 * @lc app=leetcode.cn id=1569 lang=csharp
 *
 * [1569] 将子数组重新排序得到同一个二叉搜索树的方案数
 */

// @lc code=start
public class Solution {
    const int MOD = 1_000_000_007;
    public long dfs(int[] nums, int[][] C){
        if(nums.Length <= 1) return 1;
        int n = nums.Length;
        var left = new List<int>();
        var right = new List<int>();
        for(int i = 1;i < n;++i){
            if(nums[i] < nums[0]){
                left.Add(nums[i]);
            }
            else if(nums[i] > nums[0]){
                right.Add(nums[i]);
            }
        }
        long ways = C[n - 1][left.Count];
        ways = ways * dfs(left.ToArray(), C) % MOD;
        ways = ways * dfs(right.ToArray(), C) % MOD;
        return ways;
    }
    public int NumOfWays(int[] nums) {
        int n = nums.Length;
        int[][] C = new int[n][];
        for(int i = 0;i < n; ++i){
            C[i] = new int[n];
        }
        for(int i = 0;i < n;++i){
            C[i][0] = 1;
            C[i][i] = 1;
            for(int j = 1;j < i; ++j){
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
            }
        }
        return (int)((dfs(nums, C) - 1 + MOD) % MOD);
    }
}
// @lc code=end

