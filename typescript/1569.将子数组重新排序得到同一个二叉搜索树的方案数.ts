/*
 * @lc app=leetcode.cn id=1569 lang=typescript
 *
 * [1569] 将子数组重新排序得到同一个二叉搜索树的方案数
 */

// @lc code=start
function numOfWays(nums: number[]): number {
    const MOD = 1_000_000_007;        // number：组合数表里的加法用它
    const MOD_BIG = 1_000_000_007n;   // bigint：只有乘法需要
    const n = nums.length;

    // Array.from 的工厂函数每一行都会重新执行，保证行与行是独立数组。
    // 不能写成 new Array(n).fill(new Array(n).fill(0))，那样所有行共享同一个引用。
    const C: number[][] = Array.from(
        { length: n },
        () => new Array<number>(n).fill(0)
    );

    for (let i = 0; i < n; i++) {
        C[i][0] = 1;
        C[i][i] = 1;
        for (let j = 1; j < i; j++) {
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
        }
    }

    // 只有"乘法"需要 bigint：两个因子都接近 1e9，
    // 乘积约 1e18 > 2^53 ≈ 9e15，number 会丢精度。
    // 转成 bigint 乘完取模，再转回 number（结果 < MOD，number 装得下）。
    const mulMod = (a: number, b: number): number =>
        Number(BigInt(a) * BigInt(b) % MOD_BIG);

    const dfs = (arr: number[]): number => {
        if (arr.length <= 1) return 1;

        // 根是 arr[0]，剩下的按大小分成左右两组，保持原相对顺序
        const left: number[] = [];
        const right: number[] = [];
        for (let i = 1; i < arr.length; i++) {
            if (arr[i] < arr[0]) left.push(arr[i]);
            else if (arr[i] > arr[0]) right.push(arr[i]);
        }

        // 根这一层的交错数：从 arr.length - 1 个空位里挑 left.length 个
        let ways = C[arr.length - 1][left.length];

        // 左右子树内部的方案数，每乘一次立刻取模
        ways = mulMod(ways, dfs(left));
        ways = mulMod(ways, dfs(right));

        return ways;
    };

    // dfs 把原数组本身也算了一种，题目要的是"重排"，所以减 1。
    // + MOD 防止结果为负。
    return (dfs(nums) - 1 + MOD) % MOD;
}
// @lc code=end
