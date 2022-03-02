#ifndef __Sequence
#define __Sequence

#include <type_traits>
#include <cstddef>

namespace helper {
	template <typename first>
	constexpr first help_tail(first val) { return val; }

	template <typename first, typename... other>
	constexpr first help_tail(first fir, other... vals) { return help_tail(vals...); }

	template <typename first, typename... other>
	constexpr first help_head(first fir, other... vals) { return fir; }
}


template <typename T, T... tails>
struct sequence {
	typedef T type;
	typedef sequence<T, tails...> cur_type;
	static int counts;


	// 我不明白为什么 为什么 enable_if_t<sizeof...(tails) != 0> 这样子会不可以
	// 无论放在函数参数，模板参数，函数返回值都不行，怎么做都不行。没有调用过它都不行，SFINAE 仿佛不起效
	// 报错  error: no type named 'type' in 'struct std::enable_if<false, bool>'
	// 如果可以，就能够不用特化 sequence<T> 了
	// template <typename U = T, std::enable_if_t<sizeof...(tails) != 0>* = nullptr>
	constexpr static T get_head() noexcept { return helper::help_head(tails...); }

// #if __cplusplus > 201703L
// 	constexpr static T get_tail() noexcept { return (tails, ...); }
// #else
	constexpr static T get_tail() noexcept { return helper::help_tail(tails...); }
// #endif // c++17

	constexpr static size_t is_empty() noexcept { return false; }
	constexpr static size_t length() noexcept { return sizeof...(tails); }

	template <typename U, U... others>
	constexpr bool operator==(sequence<U, others...> two) {
		return std::is_same<cur_type, decltype(two)>::value;
	}
};

template <typename T>
struct sequence<T> {
	typedef T type;
	typedef sequence<T> cur_type;

	static int counts;

	constexpr static size_t is_empty() noexcept { return true; }
	constexpr static size_t length() noexcept { return 0; }

	template <typename U, U... others>
	constexpr bool operator==(sequence<U, others...> two) {
		return std::is_same<cur_type, decltype(two)>::value;
	}
};

// 为方便计数和测试时留的，注意模板中变量的初始化方法
template <typename T, T... tails>
int sequence<T, tails...>::counts = 0;

template <typename T>
int sequence<T>::counts = 0;


// template <typename T>
// struct sequence<T> {
// 	typedef T type;
// 	typedef sequence<T> cur_type;

// 	// operator void() const noexcept { }
// 	// operator std::nullptr_t() const noexcept { return nullptr; }

// 	constexpr static size_t length() noexcept { return 0; }
// 	constexpr static size_t is_empty() noexcept { return true; }

// 	template <typename U>
// 	constexpr bool operator==(sequence<U> two) { return std::is_same<T, U>::value; }
// 	// constexpr static sequence<T> get_head() noexcept { return sequence<T>(); }
// 	// constexpr static sequence<T> get_tail() noexcept { return sequence<T>(); }
// };

template <typename T>
using empty_type = typename sequence<T>::type;

template <int... tails>
using int_seq =  sequence<int, tails...>;

#endif