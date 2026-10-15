/*
 * @lc app=leetcode.cn id=2913 lang=typescript
 *
 * [2913] 子数组不同元素数目的平方和 I
 */

// @lc code=start
function sumCounts(nums: number[]): number {
    let n : number = nums.length;
    let ans : number = 0;
    for(let i : number = 0; i < n; ++i){
        const temp = new Set<number>();
        for(let j : number = i; j < n; ++j){
            temp.add(nums[j]);
            ans += temp.size * temp.size;
        }
    }
    return ans;

};
// @lc code=end

