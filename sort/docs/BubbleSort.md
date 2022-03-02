# BubbleSort

## Helper Functions

### CAI 比较并插入到列表中

CAI\<CompareFuncs, val, sequence\> 通过比较 val 和序列首元素的大小，判断 val 应该插入到序列首部还是序列第二的位置。CompareFuncs 是自定义的比较规则，val 应该是一个 int 类型数值。

使用 CAI\<CompareFuncs, val, sequence\>::type  来获取插入后的序列类型

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

CAI<CMP, 2, int_seq<1,3,5>>::type 类型为
---> int_seq<1,2,3,5>
```





### oneloop 把序列中满足比较规则(最大，最小)的数置于序列首部

使用 oneloop\<CompareFuncs, sequence\> 来把满足比较规则(最大，最小)的数放在序列首部

使用 oneloop\<CompareFuncs, sequence\>::type 来获取操作完毕后的序列类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

oneloop<CMP, int_seq<78,23,80,45>>::type 类型为
---> int_seq<23,78,45,80>
```



## BubbleSort

使用 bubble_sort\<CompareFuncs, sequence\> 来对序列进行排序

使用 bubble_sort\<CompareFuncs, sequence\>::type 来获取排序后的类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

bubble_sort<CMP, int_seq<98,34,51,67,24,80,23>>::type
---> int_seq<23,24,34,51,67,80,98>
```

