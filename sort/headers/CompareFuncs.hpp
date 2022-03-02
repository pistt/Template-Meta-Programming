#ifndef __CompareFuncs
#define __CompareFuncs

// 在这里定义你自己的比较函数 CMP Functions
// 注意必须是 template<int, int> 形式
template <int A, int B>
struct lessthan {
	constexpr static bool value = (A < B);
};

template <int A, int B>
struct great {
	constexpr static bool value = (A > B);
};

#endif