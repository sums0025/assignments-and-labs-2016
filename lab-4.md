# 混合索引逻辑地址到物理地址映射

* 条件：自定义混合索引 `inode` 结构
  * 必须包括一次，二次，和三次间接块
  * 逻辑块 `n` 对应物理块 `n`
* 输入：文件逻辑地址
* 输出
  1. 输出 `inode` 详细信息（间接块不展开）
  2. 物理地址（物理块号，块内偏移）
