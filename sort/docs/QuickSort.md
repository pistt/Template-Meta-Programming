# QuickSort

## QuickSort

使用 quick_sort\<CompareFuncs, sequence\> 来对序列进行排序

使用 quick_sort\<CompareFuncs, sequence\>::type 来获取排序后的类型

其中 CompareFuncs 为自定义的比较规则

```c++
e.g.
template <int A, int B>
struct CMP {
    constexpr static bool value = A < B;
};

quick_sort<CMP, int_seq<22,94,54,3,76,15,6,98,23>>::type
---> int_seq<3,6,122,23,54,76,94,98>
```

