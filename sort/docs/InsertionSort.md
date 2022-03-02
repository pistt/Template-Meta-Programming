# InsertionSort

## Helper Functions

### insert  找到序列中第一个满足条件的插入位置进行插入

使用 insert\<CompareFucns, val, sequence\> 找到序列中第一个满足条件的插入位置进行插入

使用 insert<CompareFucns, val, sequence\>::type 来获取插入元素后的序列类型

其中 CompareFuncs 为自定义的比较规则，val 应该为 int 类型数值

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

insert<CMP, 8, int_seq<1,2,3,4,5,10>>::type 类型为
---> int_seq<1,2,3,4,5,8,10>
    
insert<CMP, 8, int_seq<5,7,3,1,9,10,20>::type 类型为
---> int_seq<5,7,3,1,8,9,10,20>
```



## InsertionSort

使用 insertion_sort\<CompareFuncs, sequence\> 来对序列进行排序

使用 insertion_sort\<CompareFuncs, sequence\>::type 来获取排序后的类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

selection_sort<CMP, int_seq<1,0,1,1,2,5,7,9,3,5>>::type
---> int_seq<0,1,1,1,2,3,5,5,7,9>
```

