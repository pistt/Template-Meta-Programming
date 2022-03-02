#ifndef __BubbleSort
#define __BubbleSort
#include "BaseTools.hpp"
/*
	冒泡排序：
	曾经我 oneloop 的实现：
		把满足 CMP 条件(最大，最小...)的数放到 int_seq 序列的尾部。
		这样有一个问题，做冒泡排序的时候，怎么每次做尾部长度减一的循环，不好实现。
		因此最后做了相对好实现的 oneloop 版本，每次把满足 CMP 条件的数放到 int_seq 的头部
		这样就非常好做头部长度减一的循环，来实现冒泡排序了

// template <template <int, int> class CMP, typename seq>
// struct oneloop;

// template <template <int, int> class CMP, int... tails>
// struct oneloop<CMP, int_seq<tails...>> {
// 	using type = int_seq<tails...>;
// };

// template <template <int, int> class CMP, int fir, int sec, int... tails>
// struct oneloop<CMP, int_seq<fir, sec, tails...>> {
// 	using type = std::conditional_t<
// 				CMP<fir, sec>::value,
// 				typename push_front<fir, typename oneloop<CMP, int_seq<sec, tails...>>::type>::type,
// 				typename push_front<sec, typename oneloop<CMP, int_seq<fir, tails...>>::type>::type
// 	>;
// };

*/

// Compare and Insert：oneloop 的辅助函数，根据 CMP 规则，比较待插入数值应该放在头部还是第二的位置
// 即根据 val 的值表现为 int_seq<val, head, tails...> 还是 int_seq<head, val, tails...> 形式
template <template <int, int> class CMP, int val, typename seq>
struct CAI;

template <template <int, int> class CMP, int val, typename seq>
using CAI_t = typename CAI<CMP, val, seq>::type;

template <template <int, int> class CMP, int val>
struct CAI<CMP, val, int_seq<>> {
	using type = int_seq<val>;
};

template <template <int, int> class CMP, int val, int head, int... tails>
struct CAI<CMP, val, int_seq<head, tails...>> {
	using type = std::conditional_t<
				CMP<val, head>::value,
				int_seq<val, head, tails...>,
				int_seq<head, val, tails...>
	>;
};

/* 
	oneloop：实现把 int_seq<...> 中满足 CMP 条件(最小，最大....)的数放在整个序列的头部
	不断展开判断 <val1 seq<val2, seq<val3........>>> 并后往前进行 CAI 操作形成新序列。
	即实现了从后往前，把满足 CMP 条件的元素不断进行交换。
	即冒泡排序外层循环，不断比较第(k + 1, k)，第(k, k - 1)，第(k - 1, k - 2)个元素大小并决定是否交换。
*/
template <template <int, int> class CMP, typename seq>
struct oneloop;


template <template <int, int> class CMP, typename seq>
using oneloop_t = typename oneloop<CMP, seq>::type;

template <template <int, int> class CMP>
struct oneloop<CMP, int_seq<>> {
	using type = int_seq<>;
};


template <template <int, int> class CMP, int fir, int... tails>
struct oneloop<CMP, int_seq<fir, tails...>> {
	using type = CAI_t<  CMP, fir, oneloop_t<  CMP, int_seq<tails...>  >  >;
};



/*
	冒泡排序：此处实现双重循环
	oneloop 每次形成一个把最大或最小元素放在头部的新的序列 seq
	那么我们就利用包展开的特性，每次取出 seq 的头部元素
	放入到 oneloop 对剩下元素构造出的新序列中，如此直到包展开完毕
	即 push<   val,  oneloop<tails...>  >
	便形成了双重循环实现了冒泡排序
*/
template <template <int, int> class CMP, typename seq>
struct bubble_sort;

template <template <int, int> class CMP>
struct bubble_sort<CMP, int_seq<>> {
	using type = int_seq<>;
};

template <template <int, int> class CMP, int head, int... tails>
struct bubble_sort<CMP, int_seq<head, tails...>> {
	using help = oneloop_t<  CMP, int_seq<head, tails...>  >;
	using type = typename push_front<
				gethead_v<help>,
				typename bubble_sort<  CMP, erasehead_t<help>  >::type
	>::type;
};

#endif