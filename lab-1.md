# 生产者消费者问题

共享缓冲区中放置一个数字，取值范围为[0, 10]，初值为0。生产者将此值加1，消费者将此值减1。

1. 场景1
  * 同一进程内启动一组生产者线程和一组消费者线程
  * 缓冲区为本进程的全局变量
2. 场景2
  * 启动一组生产者进程和一组消费者进程
  * 同一个数据文件为缓冲区

* 输入
  * `p`：生产者数量
  * `c`：消费者数量
* 输出

  打印当前共享缓冲区中的数值，或者生产者消费者的状态。如
```
Producer 1: 0 -> 1
Consumer 2: 1 -> 0
Consumer 3: waiting
...
Producer 0: 0 -> 1
Consumer 3: resumed
Consumer 3: 1 -> 0
...
Producer 1: 9 -> 10
Producer 2: waiting
Consumer 1: 10 -> 9
Producer 2: resumed
Producer 2: 9 -> 10
```
