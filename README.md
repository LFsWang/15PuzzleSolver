## 15 Puzzle Solver

* 本程式求的是 "最佳解" ，理論上最多 85 步就能復原，但是一般電腦求到 50 來步就大概快不行了。
* 如果提示更換盤面，建議就換掉，不然可能跑個老半天到你電腦掛掉也沒有結果。
* 如果你精神狀況夠好，可以試試看加速，會給你一個任意解答 ( 有出現過要 800 步的方法，慎用 )

## Build

```bash
g++ -std=c++17 -O3 -Wall -Wextra -static -static-libgcc -static-libstdc++ 15PuzSol.cpp -o solver
```


![image](https://i.imgur.com/QDQPUcs.png)
