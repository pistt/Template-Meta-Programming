#ifndef __MergeSort
#define __MergeSort
#include "BaseTools.hpp"

/* 	
	归并排序：merge 函数的实现
	显然我们知道，merge(arr1[], arr2[]) 中的两个数组，必须有序且使用同一种 CMP 排序方式
	简单来说，即不能一个数组升序，另一个数组降序，该 merge 基于上述条件实现。

	写了三个特化模板，因为若使用 conditional_t 写，代码可阅读性会非常差
	如果用 C++17 的 if constexpr 可以直接一键特化 int_seq 中参数个数为 (0,n)(n,0)(0,0) 三种情况

	// since C++17, 经过测试没有问题
	template <template <int ,int> class CMP, int... fir, int... sec>
	struct merge<
		CMP,
		int_seq<fir...>,
		int_seq<sec...>
	> {

		static decltype(auto) get_type() {
			if constexpr (sizeof...(fir) == 0)
				return int_seq<sec...>();
			if constexpr (sizeof...(sec) == 0)
				return int_seq<fir...>();
		}

		using type = decltype(get_type());
	
	};

*/

template <template <int ,int> class CMP, typename seq_one, typename seq_two>
struct merge;

template <template <int ,int> class CMP, typename seq_one, typename seq_two>
using merge_t = typename merge<CMP, seq_one, seq_two>::type;

/*
	三个特化模板，分别对应 (0, 0) (0, n) (n, 0) 的情况。
	(0, 0) 是必须的，因为如果少了它，当输入为 (0, 0) 时，就会产生一个 ambiguous 的报错
	因为从 (0, n) 和 (n, 0) 出发推导时，它们的特化程度是一样的，编译器不知道该选择哪个


	一个简单的例例子如下，当我们定义 test<int, double>() 的时候，此时由于两个模板的特化程度是相同的
	所以编译器将会报一个 ambiguous 错误，编译失败。后面还会有相关的讨论
	template <typename A, typename B>
	struct test { };

	template <typename A>
	struct test<A, double> { };

	template <typename B>
	struct test<int, B> { };
*/
template <template <int ,int> class CMP>
struct merge< CMP, int_seq<>, int_seq<> > {
	using type = int_seq<>;
};

template <template <int ,int> class CMP, int... val>
struct merge< CMP, int_seq<val...>, int_seq<> > {
	using type = int_seq<val...>;
};

template <template <int ,int> class CMP, int... val>
struct merge< CMP, int_seq<>, int_seq<val...> > {
	using type = int_seq<val...>;
};

/*
	合并两个有序的数组
	实现的就是普通归并排序的 merge 函数
	用 conditional_t 决定类型，CMP 比较大小，符合条件的元素放进新序列
	然后继续递归实现在新序列中放入剩余两个数组中的元素
*/
template <
	template <int ,int> class CMP, 
	int head_one,
	int... tails_one,
	int head_two,
	int... tails_two
>
struct merge< 
	CMP,
	int_seq<head_one, tails_one...>, 
	int_seq<head_two, tails_two...>
> {
	using type = std::conditional_t<
		CMP<head_one, head_two>::value,
		// head_one 满足 CMP 条件，headone 放入新序列头部，继续递归处理剩余序列
		push_front_t<head_one, 
					merge_t<CMP,
							int_seq<tails_one...>, 
							int_seq<head_two, tails_two...>  
					>
		>,
		// head_two 满足 CMP 条件，headtwo 放入新序列头部，继续递归处理剩余序列
		push_front_t<head_two, 
					merge_t<CMP, 
							int_seq<head_one, tails_one...>, 
							int_seq<tails_two...>  
					>
		>
	>;
};


/*
	分割序列，根据序列长度分割成左右两半子序列
	例如 1,2,3,4,5 就会被分割成 left:1,2 right:3,4,5

	实现思想：
	通过一个数 val 表示应该取得前 val 个元素，定义 val = 0 时不再获取元素，此时为递归出口
	每次我们都可以通过 int_seq<head, tails...> 的 head 来获取当前元素，并调用自身进行递归展开
	这样每一个序列都被展开为 <head, <head, 递归调用...>> 直到递归终止
	那么我们利用一个 push_front 即可用递归中获得的各个 head 构成一个新的序列
	这个序列就是原序列的前 val 个元素
*/
template <int val, typename seq>
struct help_split;

/*
	两个关于 help_split<0> 的特化，用作递归出口。
	展开了前 val 个元素后，显然在递归调用的过程中还会传入 int_seq<tails...> 意图展开
	但我们是在 0 处强行中断，所以此时剩下的这部分就是原序列的右端部分
	所以我们在递归出口处有定义 using right = int_seq<head, tails...>

	注意两个 help_split<0> 的特化不能合并后简写为下述形式,下述形式会报 ambiguous 错误，
	因为递归到 0 时，编译器无法判断它和最下方那个 
	template <int val, int head, int... tails> struct help_split{...} 之间哪个特化程度更高

	template <int...tails>
	struct help_split<0, int_seq<tails...>> {
		using left = int_seq<>;
		using right = int_seq<tails...>;
	};

	关于模板偏特化程度的判断
	可以参考经典名著 《C++ Primer》, 高博翻译的 《Effective modern C++》 和
	知乎博主郭不昂 https://zhuanlan.zhihu.com/p/390783543 4.4节中的讲解
	亦可自行用搜索引擎搜索
*/
template <>
struct help_split<0, int_seq<>> {
	using left = int_seq<>;
	using right = int_seq<>;
};

/*
	递归出口之一，当递归到 0 时，左边 val 个参数已取出为 head 并等待 push_front
	因此使用一个空序列类型 int_seq<>，不再调用自己而终止递归
	右边剩余参数即为 int_seq<head, tails...>
*/
template <int head, int... tails>
struct help_split<0, int_seq<head, tails...>> {
	using left = int_seq<>;
	using right = int_seq<head, tails...>;
};

/*
	递归调用自己，展开到 0 处停止，左边 val 个参数用 push_front 形成新的序列
	展开成 push<  head_val, push<  head_val-1, push<  ....  >  >  >
	即最后 push<  ........, push<  head_2,     push<  head_1, int_seq<>   >
	即最后递归收缩变为不断地 push 一个数进入新序列中
	而右边剩余的参数可以直接取用
*/
template <int val, int head, int... tails>
struct help_split<val, int_seq<head, tails...>> {
	using left = push_front_t<head, typename help_split<val - 1, int_seq<tails...>>::left>;
	using right = typename help_split<val - 1, int_seq<tails...>>::right;
};

/*
	split 函数只是设定了一个初始的 val 并把拆分左右序列的工作交给 help_split
	为什么要分开，因为 split 并不需要初始 val 参数，而且初始 val 不应该影响递归
	如果在 help_split 中定义了 midlen 那么就不可避免地递归定义 midlen
	而使得每层递归都依赖于上层的 midlen 了。
*/
template <typename seq>
struct split;

template <int... tails>
struct split<int_seq<tails...>> {
	constexpr static int midlen = length<int_seq<tails...>>::value / 2;
	using type = help_split<midlen, int_seq<tails...>>;
	using left = typename type::left;
	using right = typename type::right;
};


/*
	归并排序
	明白了 split 和 merger 之后归并排序就很简单了
	只需要递归拆分左右两个序列，并且在收缩过程中合并左右两个有序序列就可以了
*/
template <template <int ,int> class CMP, typename T>
struct merge_sort;

template <template <int ,int> class CMP, typename T>
using merge_sort_t = typename merge_sort<CMP, T>::type;


/*
	同样地，不能用 
	using type = conditional_t<
		(sizeof...(tails) <= 1), 
		int_seq<tails...>, 
		merge_t(.......) 这里代表的是最后那一个很长的特化 using type = merge_t<....>那里
	> 
	的形式写最后那个 merge_sort 从而省略下面两个特化，编译器会报错提示类型不完整
	理由当然是因为，这样写编译器无法判断你到底展开到哪一层，无法判断递归到哪结束
	error: definition of xxx is not complete until the closing brace
*/
template <template <int ,int> class CMP>
struct merge_sort<CMP, int_seq<>> {
	using type = int_seq<>;
};

template <template <int ,int> class CMP, int val>
struct merge_sort<CMP, int_seq<val>> {
	using type = int_seq<val>;
};

template <template <int ,int> class CMP, int... tails>
struct merge_sort<CMP, int_seq<tails...>> {
	using type = merge_t< 
				CMP,
				merge_sort_t<  CMP, typename split<int_seq<tails...>>::left   >,
				merge_sort_t<  CMP, typename split<int_seq<tails...>>::right  >
	>;
};

#endif