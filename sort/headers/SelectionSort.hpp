#ifndef __SelectionSort
#define __SelectionSort

#include "BaseTools.hpp"
/*
	选择排序：选择函数
	select 函数选择一个 CMP::value 为真的 val 构成 int_seq<val>
	并且准备了两个 type：
	一个是选择完 val 之后剩下的 int_seq 序列，为 rest_type
	一个是选出来的 int_seq<val>
	准备两个 type 的目的是下面的选择排序中，选完一个元素，可以知道剩下的是什么
*/
template <template <int, int> class CMP, typename seq>
struct myselect;

template <template <int, int> class CMP, int... tails>
struct myselect<CMP, int_seq<tails...>> {
	using rest_type = int_seq<>;
	using type = int_seq<tails...>;
};


template <template <int, int> class CMP, int fir, int sec, int... tails>
struct myselect<CMP, int_seq<fir, sec, tails...>> {
	using rest_type = std::conditional_t<
				CMP<fir,sec>::value,
				typename push_front<sec, typename myselect<CMP, int_seq<fir, tails...>>::rest_type>::type,
				typename push_front<fir, typename myselect<CMP, int_seq<sec, tails...>>::rest_type>::type
	>;

	using type = std::conditional_t<
				CMP<fir, sec>::value,
				typename myselect<CMP, int_seq<fir, tails...>>::type,
				typename myselect<CMP, int_seq<sec, tails...>>::type

	>;
};

/*
	选择排序
	每次调用 select 后选择出一个符合比较规则 CMP 的元素(最小, 最大....)
	而后递归调用 selection_sort 从余下序列(即 rest_type )中继续 select
	每次 select 完毕后拼接两个序列，最后即完成了排序
*/

template <template <int, int> class CMP, typename seq>
struct selection_sort;

template <template <int, int> class CMP>
struct selection_sort<CMP, int_seq<>> {
	using type = int_seq<>;
};


template <template <int, int> class CMP, int head, int... tails>
struct selection_sort<CMP, int_seq<head, tails...>> {
	// help 只是方便书写得少一点而已
	using help = myselect<CMP, int_seq<head, tails...>>;
	using type = typename concat<
				typename help::type,
				typename selection_sort<CMP, typename help::rest_type>::type
	>::type;
};

#endif