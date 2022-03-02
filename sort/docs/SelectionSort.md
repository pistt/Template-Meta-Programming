# SelectionSort

## Helper Functions

### myselect 选择序列中满足条件的元素

使用 myselect\<CompareFucns, sequence\> 来选择满足条件的元素，并返回两个序列类型。

使用 myselect\<CompareFucns, sequence\>::type 来获取仅由被选中元素构成的序列

使用 myselect\<CompareFucns, sequence\>::rest_type 来获取除被选中元素以外都的元素构成的序列

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

myselect<CMP, int_seq<1,52,84,10,25,60>>
::type 为 ---> int_seq<1>
::rest_type 为 ---> int_seq<52,84,10,25,60>
```



## SelectionSort

使用 selection_sort\<CompareFuncs, sequence\> 来对序列进行排序

使用 selection_sort\<CompareFuncs, sequence\>::type 来获取排序后的类型

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

