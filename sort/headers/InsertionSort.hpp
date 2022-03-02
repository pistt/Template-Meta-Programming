#ifndef __InsertionSort
#define __InsertionSort

#include "BaseTools.hpp"
/*
	插入排序：插入函数
	根据比较规则，找到序列中第一个插入的位置
	如 insert 25 to int_seq<10, 20, 30, 40>
	如果比较规则为 val < head 就插入，则变为 int_seq<10, 20, 25, 30, 40>
	如果比较规则为 val > head 就插入，则变为 int_seq<25, 10, 20, 30, 40>
*/
template <template <int, int> class CMP, int val, typename seq>
struct insert;

/* 对于空序列的插入规则，也是递归的出口 */
template <template <int, int> class CMP, int val>
struct insert<CMP, val, int_seq<>> {
	using type = int_seq<val>;
};

/*
	CMP 负责比较待插入 val 和 序列 int_seq<head, tails...> 中首个元素 head 的大小
	显然约束是 CMP 中必须有名字为 value 的 constexpr bool 值

	若满足比较条(true)，则直接将 val 插入序列中，类型变为 int<val, head, tails...>
	否则，将 head 放入头部，并继续递归查找第一个满足 CMP 条件的位置进行插入
	最后序列变为 int_seq<head1, head2, head3, ... , (首次满足 CMP 条件)val, tails...>

	能看懂 filter 和 transform 这里应该就很好理解了
*/
template <template <int, int> class CMP, int val, int head, int... tails>
struct insert<CMP, val, int_seq<head, tails...>> {
	using type = std::conditional_t<
				CMP<val, head>::value,
				int_seq<val, head, tails...>,
				typename push_front<head, typename insert<CMP, val, int_seq<tails...>>::type>::type
	>;
};

/*
	插入排序
*/

template <template <int, int> class CMP, typename seq>
struct insertion_sort;

template <template <int, int> class CMP>
struct insertion_sort<CMP, int_seq<>>{
	using type = int_seq<>;
};

// 递归进行展开，每次选一个元素进行插入
template <template <int, int> class CMP, int head, int... tails>
struct insertion_sort<CMP, int_seq<head, tails...>>{
	using type = typename insert<CMP, head, typename insertion_sort<CMP, int_seq<tails...>>::type>::type;
};

#endif