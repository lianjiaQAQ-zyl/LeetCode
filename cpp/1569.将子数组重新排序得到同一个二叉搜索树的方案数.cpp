/*
 * @lc app=leetcode.cn id=1569 lang=cpp
 *
 * [1569] 将子数组重新排序得到同一个二叉搜索树的方案数
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
public:
    // ---- 取模要点 0：模数 ----
    // 1e9 + 7 是质数，int 装得下（1000000007 < INT_MAX ≈ 2.147e9）。
    // 写成 1'000'000'007 用的是 C++14 的数字分隔符，只是好读，数值不变。
    static constexpr int MOD = 1'000'000'007;

    // ---- 取模要点 1：返回值用 long long ----
    // 取模之后每个结果都 < MOD（约 1e9），int 确实装得下。
    // 但下一步要做"两个 < MOD 的数相乘"，结果可达 1e18，
    // int（上限约 2.1e9）会溢出。所以返回值统一用 long long。
    long long dfs(vector<int>& nums, const vector<vector<int>> & C){
        if(nums.size() <= 1) return 1;
        int n = nums.size();
        vector<int> left, right;
        for(int i = 1;i < n; ++i){
            if(nums[i] < nums[0]){
                left.push_back(nums[i]);
            }
            else if(nums[i] > nums[0]){
                right.push_back(nums[i]);
            }
        }
        // 根节点确认以后，只要左右子树内部相对顺序不变，就会得到相同的 BST
        // 相当于在 n - 1 个空位中选取 left.size() 个空位存放 left 中的数据
        // left 内部顺序不变，只有一种填充方式
        // 剩下的空位用于填充 right 数据

        // 表里的组合数在建表时就已经取过模了
        long long ways = C[n - 1][left.size()];

        // ---- 取模要点 2：每乘一次就取一次模 ----
        // 取模对乘法"可分配"：
        //     (a * b) % MOD == ((a % MOD) * (b % MOD)) % MOD
        // 所以任何一步先取模，都不会改变最终答案。
        //
        // 反过来，为什么不能攒到最后再取模：
        //   n = 1000 时真实答案可达 10^298 量级，
        //   long long 上限才约 1.8e19，早就溢出成垃圾值了，
        //   溢出之后再取模没有任何意义。
        //
        // 为什么这里必须用 long long：
        //   ways 和 dfs(...) 都 < 1e9，相乘约 1e18，
        //   在 int 里算会溢出，long long 刚好装得下。
        ways = ways * dfs(left, C) % MOD;
        ways = ways * dfs(right, C) % MOD;

        return ways;
    }
    int numOfWays(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> C(n, vector<int>(n, 0));
        for(int i = 0; i < n; ++i){
            // C(i, j) = C(i - 1, j - 1) + C(i - 1, j)
            C[i][0] = 1;
            C[i][i] = 1;
            for(int j = 1; j < i; ++j){
                // ---- 取模要点 3：这里的加法用 int 是安全的 ----
                // 两个加数都已经 < MOD，
                // 和 < 2 * 1e9 + 14 < INT_MAX，不会溢出；
                // % MOD 再把结果压回 [0, MOD)。
                // 注意对比：加法安全不代表乘法安全（见上面要点 1、2）。
                C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % MOD;
            }
        }
        // 不要原本序列，只要重排方案：减 1。
        // ---- 取模要点 4：减法可能减出负数 ----
        // 如果 dfs 的结果取模后恰好是 0，直接 -1 会得到 -1。
        // 先 + MOD 再 % MOD，把结果拉回 [0, MOD)。
        return (int)((dfs(nums, C) - 1 + MOD) % MOD);
    }
};
// @lc code=end
