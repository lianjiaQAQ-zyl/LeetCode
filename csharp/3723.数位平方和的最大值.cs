/*
 * @lc app=leetcode.cn id=3723 lang=csharp
 *
 * [3723] 数位平方和的最大值
 */

// @lc code=start
public class Solution {
    public string MaxSumOfSquares(int num, int sum) {
        //1. sum <= 9 * num，否则无法表示
        if(num * 9 < sum){
            return "";
        }
        //2. sum / 9 位 9 + sum % 9，后面全是 0
        string begin = new string('9', sum / 9);
        string mid = sum % 9 > 0 ? new string((char)('0' + sum % 9), 1) : "";
        
        string end = new string('0', num - begin.Length - mid.Length);
        return begin + mid + end;
    }
}
// @lc code=end

