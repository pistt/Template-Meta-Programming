# MergeSort

## Helper Functions

### merge 合并两个有序序列

**从语义上，merge 显然只应该对两个有序且排序方式相同的序列将进行合并**

使用 merge\<CompareFuncs, sequence1, sequence2\> 来合并两个有序序列

使用 merge\<CompareFuncs, sequence1, sequence2\>::type 来获取合并后的序列类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

merge<CMP, int_seq<1,3,5>, int_seq<2,4,6,8>>::type 类型为
---> int_seq<1,2,3,4,5,6,8>
```



### help_split 分割单个序列为两部分

使用 help_split\<val, sequence\> 来分割单个序列为两部分。前面 val 个元素分割成一个序列，剩余的元素作为另一个序列。其中 val 应该是一个大于等于 0 的 int 值

使用  help_split\<val, sequence\>::left 来获取分割后前 val 个元素组成的序列类型

使用 help_split\<val, sequence\>::right 来获取分割后剩余元素组成的序列类型

e.g. help_split\<2, int_seq<1,2,3,4,5,6>\>::left 类型为 int_seq\<1,2\>

​		help_split\<2, int_seq<1,2,3,4,5,6>\>::right 类型为 int_seq\<3,4,5,6\>



### split 二分单个序列为两部分

使用 split\<sequence\> 来二分单个序列

使用 split\<sequence\>::left 来获取二分该序列之后左半部分组成的序列类型

使用 split\<sequence\>::right 来获取二分该序列之后又半部分组成的序列类型

e.g. split\<int_seq\<90,50,70,80,20,10,30\>\>::left 类型为 int_seq\<90, 50, 70\>

​		split\<int_seq\<90,50,70,80,20,10,30\>\>::right 类型为 int_seq\<80, 20, 10, 30\>



## MergeSort

使用 merge_sort\<CompareFuncs, sequence\> 来对序列进行排序

使用 merge_sort\<CompareFuncs, sequence\>::type 来获取排序后的类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

merge_sort<CMP, int_seq<70,80,10,20,40,30,60,50,90>>::type 类型为
---> int_seq<10,20,30,40,50,60,70,80,90>
```

