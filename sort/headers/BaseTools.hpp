#ifndef __BaseTools
#define __BaseTools

#include <iostream>
#include <type_traits>
#include <cstddef>
#include "Sequence.hpp"
#include "CompareFuncs.hpp"

// 求长度
template <typename T>
struct length { };

template <int... values>
struct length<int_seq<values...>> : std::integral_constant<size_t, sizeof...(values)> { };

// 求是否为空列表
template <typename T>
struct empty : std::false_type { };

template <>
struct empty<int_seq<>> : std::true_type { };



/*
	* 注意和正确具体实现的区别!!!!

	template <int V, template <int...> class T>
	struct push_front;

	template <int V>
	struct push_front<V, int_seq> {
		using type = int_seq<V>;
	};

*/

// 前面插入
template <int val, typename T>
struct push_front;

template <int val, typename T>
using push_front_t = typename push_front<val, T>::type;

template <int val, int... values>
struct push_front<val, int_seq<values...>> {
	using type = int_seq<val, values...>;
};

// 后面插入
template <int val, typename T>
struct push_back;


template <int val, int... values>
struct push_back<val, int_seq<values...>> {
	using type = int_seq<values..., val>;
};

// 删除头部
template <typename T>
struct erasehead;

template <typename T>
using erasehead_t = typename erasehead<T>::type;

template <>
struct erasehead<int_seq<>> {
	using type = int_seq<>;
};

template <int head, int... tails>
struct erasehead<int_seq<head, tails...>> {
	using type = int_seq<tails...>;
};

// 获取头部
template <typename seq>
struct gethead;

template <>
struct gethead<int_seq<>>;

template <int head, int... val>
struct gethead<int_seq<head, val...>> {
	constexpr static int value = head;
};

template <typename seq>
constexpr static int gethead_v = gethead<seq>::value;

// 交换头部
template <int val, typename seq>
struct swaphead;

template <int val>
struct swaphead<val, int_seq<>> {
	using type = int_seq<val>;
};

template <int val, int head, int... tails>
struct swaphead<val, int_seq<head, tails...>> {
	using type = int_seq<val, tails...>;
};


// 连接两个列表
template <typename T, typename U>
struct concat { 
	using type = std::conditional_t<!empty<T>::value, T, U>;
};

template <typename T, typename U>
using concat_t = typename concat<T, U>::type;
// 这个特化其实没必要
// template <>
// struct concat<int_seq<>, int_seq<>> {
// 	using type = int_seq<>;
// };

template <int... fir, int... sec>
struct concat<int_seq<fir...>, int_seq<sec...>> {
	using type = int_seq<fir..., sec...>;
};


// 翻转
template <typename T>
struct reverse;

template <typename T>
using reverse_t = typename reverse<T>::type;

template <>
struct reverse<int_seq<>> {
	using type = int_seq<>;
};

template <int head, int... vals>
struct reverse<int_seq<head, vals...>> {
	using type = typename push_back<head, typename reverse<int_seq<vals...>>::type>::type;
};

// 变换操作
template <template <int> class change, typename seq>
struct transform;

template <template <int> class change, typename seq>
using transform_t = typename transform<change, seq>::type;

template <template <int> class change>
struct transform<change, int_seq<>> {
	using type = int_seq<>;
};

// 对内部元素做 transfrom 变换，显然变换模板应该接受的是 int 参数
// 第一个参数是 template template parameter，详情可以参见 侯捷的视频 和 李伟《c++模板元编程实战》
// 模板的第一个参数，接受的参数是一个(可以放一个 int 参数的模板)
template <template <int> class change, int head, int... tails>
struct transform<change, int_seq<head, tails...>> {
/*
	1.change<head>::value 取出变换模板对 head 进行变换之后的值 val
	2.push_front 对取出的值插入到后面的 int_seq<.....> 中，变成 int_seq<val, ......>
	3.transform<change, int_seq<tails...>> 这里做了递归展开，通过不断地取出包中的第一个元素

	把剩余元素放入下一个包中，从而不断地递归展开，到最后变成int_seq<> 展开完毕
	如：transform<sub, int_seq<1,2,3>>
	（1）push_front<0, typename transform<sub, int_seq<2,3>>::type>::type
	(2) 然后(1)中的 transform 展开成 push_front<1, typename transform<sub, int_seq<3>>::type>::type
	(3) 同理变成 push_front<2, transform<sub, int_seq<>>::type>::type
	(4) 由于做了模板特化，此时到了递归出口递归结束
	(5) 然后(3)收缩成 push_front<2, int_seq<>> 	---> int_seq<2>
	(6) 然后(2)收缩成 push_front<1, int_seq<2>>	---> int_seq<1,2>
	(7) 然后(1)收缩成 push_front<0, int_seq<1,2>>	---> int_seq<0,1,2>  
	(8) 最后我们就得到了经过变换后的一个新的类型，它为 int_seq<0,1,2>
	
	4. using type 中我们对 type 的定义至关重要，因为我们自始至终做的都是类型的推导，而从未涉及任何一个实实在在具体存在的变量
	5. 如果学过离散数学的递归定义，或者对递归比较熟悉，应该不难明白我们的 type 为何要这样递归定义
*/
	using type = typename push_front<change<head>::value, typename transform<change, int_seq<tails...>>::type>::type;
};

// 具体的某种 transform 函数..........
// 注意必须是只接受一个 int 参数的模板
// sub 函数
template <int val>
struct sub {
	constexpr static int value = val - 1;
};

// add 函数
template <int val>
struct add {
	constexpr static int value = val + 1;
};


// 过滤器
// 实现 if (f x) then [x] else []
// 明白上面的 transform 就很容易明白这里了
template <template <int> class F, typename seq>
struct filter;

template <template <int> class F, typename seq>
using filter_t = typename filter<F, seq>::type;

template <template <int> class F>
struct filter<F, int_seq<>> {
	using type = int_seq<>;
};

// 使用了一个 conditional_t 做条件类型的变换
template <template <int> class F, int head, int... vals>
struct filter<F, int_seq<head, vals...>> {
	using type = std::conditional_t<
				F<head>::value,
				typename push_front<head, typename filter<F, int_seq<vals...>>::type>::type,
				typename filter<F, int_seq<vals...>>::type
	>;

};

// 过滤器，过滤奇偶，当然可以自己实现一个过滤别的的过滤器
template <int val>
struct isodd {
	constexpr static bool value = (val % 2 == 1);
};

// print 打印数值
// c++14以前模板的展开，c++17 后可以用 folder expression 去做
template <int head, int... vals>
void print(int_seq<head, vals...> seq) {
	std::cout << head << " ";
	print(int_seq<vals...>());
}

void print(int_seq<>) { std::cout << std::endl; }

#endif