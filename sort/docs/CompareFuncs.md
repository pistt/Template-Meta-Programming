# Compare Functions

**如果需要使用自定义比较函数，请在 ./headers/CompareFucns.hpp 中定义你的比较函数**

**请注意自定义的比较函数必须接受且仅接受两个 int 的模板类型，即 template <int, int> struct...**

**并且你定义的元函数，必须有一个名字叫 value 的 constexpr static bool 类型的变量**

## 内置了两个比较函数

在使用本项目定义的各个元函数中，使用这两个函数的表现类似于使用 std::less\<int\> 和 std::greater\<int\> 的表现。即在各个排序中使用 lessthan 会使得序列按照升序排列，而 great 则会使得序列按照降序排列。

```c++
template <int A, int B>
struct lessthan {
	constexpr static bool value = (A < B);
};

template <int A, int B>
struct great {
	constexpr static bool value = (A > B);
};
```

