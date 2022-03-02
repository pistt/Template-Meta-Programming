#ifndef __QuickSort
#define __QuickSort
#include "BaseTools.hpp"
/*
	紧张刺激的快速排序终于来了！
	我们实现快速排序的思路其实很简单。每次选取第一个元素作为 pivot
	根据 CMP 规则，比 pivot 小的元素放在左边，反之放在右边
	如此构建出 left, pivot, right 序列，并对 left 和 right 序列
	递归进行上述过程，即完成了快速排序

	实现方法：
	借助一个过滤器 filter 筛选出满足条件的元素即可
	可以查看 basetools 中 filter 的实现和讲解
*/
template <template <int, int> class CMP, typename seq>
struct quick_sort;

template <template <int, int> class CMP>
struct quick_sort<CMP, int_seq<>> {
	using type = int_seq<>;
};

template <template <int, int> class CMP, int head, int... tails>
struct quick_sort<CMP, int_seq<head, tails...>> {

	// 过滤器，后续嵌入 filter 中，筛出快速排序 pivot 元素左右两边的元素
	// 注意偏序关系，val 和 head 的顺序不能反转，否则语义是错误的，尽管结果可能正确
	template <int val>
	using left = CMP<val, head>;


	template <int val>
	struct right {
		constexpr static bool value = !(CMP<val, head>::value);
	};


	// 借助过滤器滤出left, pivot, right 并且递归进行上述过程
	// filter_t 这行：过滤出满足条件的元素并放在左边，并且递归进行过滤 quick_sort 过程
	// push 这行：过滤出满足条件的元素，放在右边并把 pivot 放入头部，并且递归进行过滤 quick_sort 过程
	using type = concat_t<
				filter_t<left, typename quick_sort<CMP, int_seq<tails...>>::type>,
				push_front_t<head, 
							filter_t<right, typename quick_sort<CMP, int_seq<tails...>>::type>
				>
	>;
};

#endif