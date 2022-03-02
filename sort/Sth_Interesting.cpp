#ifndef __Interest
#define __Interest

#include <iostream>
#include <bitset>

// #1 函数，高阶函数，无限套娃

// 普通函数
// 普通的类模板
template <int val>
struct Base_Test {
	constexpr static int value = val & (-val);
};

// 高阶函数
// 接受一个模板作为参数，接受的这个模板必须是只接受一个 int 类型的类模板
template <template <int> class T, int val>
struct T_Test {
	constexpr static int value = T<val>::value << 2;
};

/*
	套娃
	U: 接受一个(以模板作为参数的模板)作为它的类型
	T: 接受一个 int 类型的模板作为它的类型
	val: 接受一个 int 类型

	例子：
	auto p = Test<T_Test, Base_Test, 34>::value;
	auto q = T_Test<Base_Test, 34>::value;
	cout << p << "  " << q << endl;
	输出：7 8


	在例子中：
	U: T_Test
	T: Base_Test
	val: int

	所以 U<T, val> ===> T_Test<Base_Test, val>

	那么就有
	---> value = T_Test<Base_Test, val>::value - 1
	---> value = (Base_Test<val>::value << 2) - 1
	---> value = {[val & (-val)] << 2} - 1
	---> value = {[34 & (-34)] << 2} - 1 = (2 << 2) - 1 = 8 - 1 = 7

	那 T 是接受一个 int 参数的模板，我们应该给 T 传入一个 val 才行啊。
	为什么 U<T, val> 就可以了而不是 U<T<val>, val> 呢？

	因为参数接受的是模板，而 T 才是模板，T<val> 则已经是某个具体的类型
	就譬如 Base_Test<3> 就已经是一个具体的类型而不是一个模板类了，它的类型为 Base_Test<3>
	而只有 Base_Test 本身才是一个模板类


	从这个例子我们可以看出模板是不断嵌套的。不难推出编译器做语法分析的时候，对模板做了一个递归的分析
	只有这样我们才能够在模板中嵌套模板，并解释这个现象，并且"恬不知耻"地套娃下去
*/
template <
		template <template <int> class, int> class U, 
		template <int> class T,
		int val
>
struct Test {
	constexpr static int value = U<T, val>::value - 1;
};

/* 	
	#2 define 宏展开和模板
	参考连接：https://zhuanlan.zhihu.com/p/371769440
	以下代码是仿照 v8 写的用于理解的精简版
	参考链接：https://zhuanlan.zhihu.com/p/371769440	

	当我们想只允许实例化特化模板，并且想要在实例化非法模板的时候，让编译器输出提示信息
	就可以像下面这样定义和利用宏。当不用编译器输出提示信息时，static_assert 则完全不必
	直接对 foo 只做声明，不做定义，而利用宏去定义合法模板即可。

	当我尝试使用 foo<long long>() 时，编译期便会产生一个静态断言，给出提示信息
	"Not supported type\n" 但是当我使用 foo<void>() 的时候就能正常运行

	// 当我不需要编译器提供提示信息时
	template <typename T>
	void foo();
*/

template <typename T>
void foo() {
	static_assert(sizeof(T) != sizeof(T), "Not supported type\n");
}

#define INFO_HELPER(T)	\
template <>				\
void foo<T>() { std::cout << "Amazing!\n"; };

#define INFO(T) \
	T(void)		\
	T(int)		\
	T(bool)		\
	T(float)	\
	T(short)	\
	T(unsigned short)	\
	T(unsigned int)


INFO(INFO_HELPER)

/*
	上面的代码会被编译期预编译展开成(为了美观，经过省略和格式化)
	虽然这样做不太符合 modern c++ 的规范，但确实是一种节省代码量，和提高代码可读性的方法
	g++ -E {$fileBasename}
	
	template <typename T>
	void foo() {
 		static_assert(sizeof(T) != sizeof(T), "Not supported type\n");
	}

	template <> void foo<void>() { printf("%s", "Amazing!\n"); }; 
	template <> void foo<int>() { printf("%s", "Amazing!\n"); };
	template <> void foo<bool>() { printf("%s", "Amazing!\n"); };
	template <> void foo<float>() { printf("%s", "Amazing!\n"); };
	template <> void foo<short>() { printf("%s", "Amazing!\n"); };
	template <> void foo<unsigned short>() { printf("%s", "Amazing!\n"); };
	template <> void foo<unsigned int>() { printf("%s", "Amazing!\n"); };
*/


/*  
	#3 SFINAE 妙用----C++20 Concepts 出来之后就不用这么麻烦了

	只有 T 类型拥有 Hello 函数，且 Hello 函数能接受 const char* 类型时，下面的 Say 模板才会被启用
	当然里面的 const char * 也可以是 template U 然后对这个 U 做出某种限制
	变成只允许拥有 Hello 函数，且 Hello 函数接受 U 类型时，Say 模板才会被启用

	1. 	Pure C++ 2020: 张轶 Concept Driven Design 
	   	https://www.bilibili.com/video/BV1B54y1s7dp
	2.	Pure C++ 2020： 杨超 现代C++的工程实践--跨语言脚本引擎ScriptEngine
		https://www.bilibili.com/video/BV1B54y1s7dp
*/
template <typename T, typename = void>
struct has_mfn_hello : std::false_type { };

// declval<T>() 的作用只是让他变成 T&&
// cppreference：返回一个类型的右值引用，不管是否有没有默认构造函数或该类型不可以创建对象(可以用于抽象基类)
template <typename T>
struct has_mfn_hello<T,
		std::void_t<decltype(std::declval<T>().Hello(std::declval<const char*>()))>
> : std::true_type { };

template <typename T,
		  typename = std::enable_if_t<has_mfn_hello<T>::value>
>
void Say(T t) { t.Hello("World"); }

class rabbit {
public:
	void Hello(const char* p) { std::cout << p << std::endl; }
};

class cat {
public:
	void Hello(int p) { std::cout << p << std::endl; }
};

class dog { };

#endif

int main(int argc, char* const argv[]) {
	using namespace std;
	// // #1 Test
	// auto p = Test<T_Test, Base_Test, 34>::value;
	// auto q = T_Test<Base_Test, 34>::value;
	// cout << p << "  " << q << endl;

	// #3 只有 Say(rabbit()) 通过了编译，输出字符串"World"
	// In function 'int main(int, char* const*)':
	// error: no matching function for call to 'Say(cat)'
	// error: no matching function for call to 'Say(dog)'
	// Say(rabbit());
	// Say(cat());
	// Say(dog());
	return 0;
}