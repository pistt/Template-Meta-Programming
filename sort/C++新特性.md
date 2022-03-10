[TOC]

## C++ New Features

### Pure C++

#### Pure C++ 2020

选了两个讲得比较好的视频，做了一些笔记

[Video1 现代C++的工程实践--跨语言脚本引擎ScriptEngine](https://www.bilibili.com/video/BV1vh411275E)

[Video2 Concepts Driven design](https://www.bilibili.com/video/BV1B54y1s7dp)



### Pre—knowledge

- C++ primer plus P288 页的详细解释

<img src=".\images\Templates.jpg" alt="模板的实例化与具体化" style="zoom:50%;" align="left" />

- 模板的特化与偏特化。函数模板只能全特化，不能偏特化。函数模板的全特化，就是显式具体化了一个新的函数实体，模板推断时不再从给定的模板出发，而直接使用具体化的模板。

> 从给定的模板出发，编译器自动根据传入的参数类型推断，生成函数定义和实体的过程，叫做隐式实例化，一般的模板推断都是隐式实例化。

```c++
// 1.类模板
template<class T, class U>
class Cal {};

//全特化
template <>
class<int, char> Cal { };

//偏特化
template<class T>
class<int, T> Cal { };


// 2.函数模板
// 函数模板不能偏特化，只有类模板可以
template<class T, class U>
void print(){ }

//显式实例化(全特化)
template <>
void print<int, char>() { }

//下面代码是错误的，因为尝试偏特化模板
template <class T>
void print<int, T>() { }
```



### Apply "Using" instead of  "typedef"

- 优先使用 using 声明，而非 typedef。在模板中有不少的好处

```c++
// 1.函数指针
typedef void(*FP)(int, const std::string&);
using FP = void(*)(int, const std::string&);

// 2.别名模板
//假设我定义了一个自己的分配器，想在 std::list 中使用
//使用 typedef 的麻烦
template<typename T>
struct MyAllocatorList {
    typedef std::list<T, MyAllocator<T>> type;
};

MyAllocatorList<int>::type mylist;

//使用 using 声明，简单快捷
template<typename T>
using MyAllocatorList = std::list<T, MyAllocator<T>>;

MyAllocatorList<T> mylist;
```



### Constexpr

1. 他表示的是这样的值：不仅是 const，而且编译阶段已知（包括编译和链接）
2. Constexpr 函数：若传入编译期已知值，结果也将是编译期计算出；若传入运行期值，则结果也在运行期计算出。在 C++11 中，只允许单行语句，且不允许返回值为 void ，因为 void 不是字面值；在 C++14中，允许多行语句包括控制和循环。
3. 构造一个在内存中的类

```c++
class Point {
public:
    constexpr Point(double xval, double yval) noexcept
    : x = xval, y = yval
    { }
    
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    
private:
    double x,y;
};

//下面这些都能在编译期完成
// 1
constexpr Point p1(233, 452.3);
constexpr Point p2(238223.133,9872423);

// 2
constexpr Point midPoint(const Point& p1, const Point& p2) {
    return { p1.xValue() / 2 + p2.xValue() / 2,
           p1.yValue() / 2 + p2.yValue() / 2 };
}

constexpr Point mid = midPoint(p1, p2);
```



### Template Meta Programming

> 关于模板元的诞生，应用，讨论可以参考下述文章：知乎还有很多黑魔法 
>
> 腾讯云：https://cloud.tencent.com/developer/article/1347878
>
> 知乎：https://www.zhihu.com/question/21656266?sort=created

- 从 C++11 之后，引入了模板元编程。我们可以把许多运行期的事情，提早到编译期完成。详细运用可以参考 type_traits 库
- 最核心的用法在于 **SFINAE(Substitution Failure Is Not An Error)**，翻译是模板推断失败不是一个错误。因此我们可以特化或者偏特化某些模板，让某些模板在一定条件下推断成功可以用，某些模板在一定条件下推断失败不能用。从而达到自动选用匹配模板的效果。

```c++
//利用模板的递归构建，实现编译期求和
//C++ 11
template<int T>
struct fac {
    static const int value = T * fac<T - 1>::value;
};

template <>
struct fac<0> {
    static const int value = 1;
};

auto fac5 = fac<5>::value;

//利用 constexpr 替代原有的复杂编程
constexpr int fac(int n) {
	return n == 1 ? 1 : n * fac(n - 1);
}

auto fac5 = fac(5);
```

#### Meta Functions

0. 在这里，我们对元函数做一个宽松的定义。对形如 $y=f(x)$ 形式的函数，我们均称之为元函数。

```c++
template<class T>
struct Fun_ { using type = T; };

// 对如下形式，我们也认为他是一个元函数，因为它具有输入 T，同时明确定义了映射规则
// 输出(Fun<T>)，因此我们将他视为一个元函数
template<class T>
    using Fun = typename Fun_<T>::type;
        
Fun_<int> h = 3;
```



1. 各式各样的元函数

```c++
// #1
template<class T>
struct Fun {};

// #2
constexpr int fun() { return 10; }

// #3
template<int a>
   constexpr int fun = a + 1;
```

#### Static member in class template

```c++
// 错误示范，ISO C++ 禁止在类(包括模板类)中初始化 non-static-const 变量
template <class T>
struct Test { 
    static int member1 = 0; // 错误
	const static int member2 = 0; // 正确
};


// 初始化模板类中变量的正确方法
template <class T>
struct Test {
    static T member3;
    static int member4;
};

template <class T>
T Test<T>::member3 = 0;

template <class T>
int Test<T>::member4 = 1;
```



#### Template template parameters

```c++
// template <template <typename U> class T1, typename T2> 等价
template <template <typename> class T1, typename T2>
struct Fun_ {
    using type = typename T1<T2>::type;
};

// 该 typename 的作用为告诉编译器 type 是一个类型，而不是某个类成员
// 即告诉编译器不是 std::cout 这种具体的成员，而是一个 int，double 或者别的类型
template <template <typename> class T1, typename T2>
using Fun = typename Fun_<T1, T2>::type;

// 相当于 int h = 3
Fun<std::remove_reference, int&> h = 3;
```

简单来说，该函数实现的功能为 $Fun(T_1, t_2)=T_1(t_2)$



```c++
template<bool Add_Or_Remove_Ref> struct Fun_;

// 模板的特化
template <>
struct Fun_<true> {
    template <typename T>
    using type = std::add_lvalue_reference<T>;
};

template <>
struct Fun_<false> {
    template <typename T>
    using type = std::remove_reference<T>;
};
```

按书上，以下实测会报错 too many template-parameter-lists

> 书上的代码和注释：
>
> // 两层顺序不能搞错，内层 template 定义了元函数的模板参数
> // 外层 template 则表示 Fun 的返回值是一个接收一个模板参数的模板
> template <typename T>
> template <bool Add_Or_Remove>
> using Fun = typename Fun_<Add_Or_Remove>::template type<T>;
>
> // using 声明，方便简写
> template <typename T>
> using Res_ = Fun<false>;
>
> Res_<int&>::type h = 3;

按我自己的实践，应该这样写：

```c++
template <bool Add_Or_Remove>
struct Fun {
    template <typename T>
    using type = typename Fun_<Add_Or_Remove>::template type<T>;
};

template <typename T>
using Res_help = typename Fun<false>::template type<T>;

template <typename T>
using Res_ = Res_help<T>::type;

Res_help<int&>::type h = 3;
```

同样地需要注意改完后不能这样写`template <class T> using Res_ = Fun<false>;`会报错

<img src=".\images\error_template.png" alt="模板的实例化与具体化" style="zoom:50%;" align="left" />



#### Middle-Summary about templates

```c++
template <int... vals> class IntContainer;
template <bool... vals> class BoolContainer;
template <typename... types> class TypeContainer;
template <template <typename> class... T> class TemContainer1;
template <template <typename...> class... T> class TemContainer2;
```

上述代码段相当于定义了5个容器：

① 可以放入 int 类型

② 可以放入 bool 类型

③ 可以存放很多种 type 类型

④ 可以存放很多个模板，其中每个模板接受一个参数类型

⑤ 可以存放很多个模板，其中每个模板可以接受很多个参数类型



#### Coding with TMP —— sequences, branches and loops

1. 顺序执行，过于简单，略
2. 分支结构

实际上在上面一节，我们已经知道分支结构是怎么执行的了。这节我们详细地说一说

① 使用 std::conditional 和 std::conditional_t 实现分支

```c++
// 其定义如下
namespace std {
    template <bool B, typename T, typename F>
    struct conditional {
        using type = T;
    };
    
    // 偏特化
    template <typename T, typename F>
    struct conditional<false, T, F> {
        using type = F;
    };
    
    template <bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;
}
```

```c++
// 行为逻辑类似于 x = B ? T : F
std::conditional<true, int, float>::type x = 3;
std::conditional<false, int ,float>::type y = 1.0f;
```

② 使用（部分）特化来实现分支

这一部分更类似前一节所讨论的内容。相较于前面的复杂例子， 我们举一个简单的例子

```c++
template <typename T>
struct Fun_ {
    constexpr static size_t value = 0;
};

template <>
struct Fun_<int> {
    constexpr static size_t value = 1;
};

template <>
struct Fun_<float> {
    constexpr static size_t value = 2;
};
```

需要注意的是，在非完全特化的模板类中，引入完全特化的分支代码是非法的，如下：

```c++
template <class T>
struct Wrapper {
    template <class U>
    struct Fun_ {
        constexpr static size_t value = 0;
    };
    
    template <>
    struct Fun_<int> {
        constexpr static size_t value = 1;
    };
};
```

原因是 Wrapper 是一个**未完全特化的类模板，但在其内部包含了一个完全特化的 Fun_\<int\>，这是 c++ 标准所不允许的。同样地，在一个普通类中(即不是模板类的类)，偏特化和全特化也是不允许的。这时候我们可以引入一个伪参数解决这个问题，如下**

```c++
template <class T>
struct Wrapper {
    template <class U, class dummy = void*>
    struct Fun_ {
        constexpr static size_t value = 0;
    };
    
    template <class dummy>
    struct Fun_<int, dummy> {
        constexpr static size_t value = 1;
    };
};
```

③ 编译期分支与多种返回类型

```c++
// 运行期这样的代码是错误的，因为返回类型实际上是确定的
auto fun(bool check) {
    if (check) return static_cast<int>(0);
    else return static_cast<double>(0.0);
}

// 元编程可以这样写
template <bool check, std::enable_if_t<check>* = void*>
auto fun() { return (int)0; }

template <bool check, std::enable_if_t<!check>* = void*>
auto fun() { return (double)0.0; }

// c++17 引入了 if constexpr 后可以这样写
template <bool check>
auto fun() {
    if constexpr (check) return (int)0;
    else return (double)0.0;
}
```

3. 循环执行

​	① 借助递归

```c++
// 声明了一个接受一个“包”的 Accumulate<包> 
// 并且定义了所有 Accumulate<包> 默认均为 0
template <size_t... Inputs>
constexpr size_t Accumulate = 0;

template <size_t First, size_t... Tails>
constexpr size_t Accumulate<First, Tails...> = First + Accumulate<Tails...>
```

​	② 借助 folder expression 技术，借助逗号表达式，见 [Variadic Templates](#Variadic Templates)



##### Optimise----Be careful for the booming of instanciation 

① 分离循环

这是个计算 $\sum^{A+ID}_{i=1}i$ 的一个函数模板。**理想情况下，我们可以利用 imp\<N\> 所计算出的一系列实例，去计算 imp\<N以下\>。但是在下面的未优化版本中，却做不到。因为一个属于 Wrap_\<10\>::imp 另一个则属于Wrap_\<3\>::imp**

```c++
template <size_t A>
struct Wrap_ {
    template <size_t ID, typename Dummy = void>
    struct imp { constexpr static size_t value = ID + imp<ID - 1>::value; };
    
    template <typename dummy>
    struct imp<0, dummy> { constexpr static size_t value = 0; };
    
    template <size_t ID>
    constexpr static size_t value = imp<A + ID>::value;
};

auto p = Wrap_<10>::value<2>;
auto q = Wrap_<3>::value<2>;
```

**分离循环**，优化之后应该是这样的

```c++
template <size_t ID>
struct imp { constexpr static size_t value = ID + imp<ID - 1>::value; };

template <>
struct imp<0> { constexpr static size_t value = 0; };

template <size_t A>
struct Wrap_ {
    template <size_t ID>
    constexpr static size_t value = imp<A + ID>::value;
};
```

② 分支选择和短路逻辑





③ [CRTP](#Curiously Recurring Template Pattern)

#### Usage of Enable_if() in Template Meta Programming

- 参考这个：https://en.cppreference.com/w/cpp/types/enable_if

1. enable_if 的定义

```c++
template<bool T, class _Ty = void>
struct enable_if { };

//利用模板偏特化的特性，定义出值为 true 时的定义
//如此，当值为 false 时将不会有 type 属性
template<class T>
struct enable_if<true, T> { typedef T type; };
```

2. 不同版本 C++ 的写法

```c++
// C++11 中使用以下语句可简化后续书写
// C++14 自带 enable_if_t 等等，更为方便
template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;

// C++20 中使用 concepts 则更为方便
// 如果 T 是整型
template<typename T>
void display(std::integral auto num) { }
//等价于以下形式
template<typename T>
void display(typename std::enable_if_t<std::is_integral_v<T>>* = nullptr) { }
```

3. 作返回值类型

```C++
//此时将会根据传入的参数类型，自动使用某一个函数模板
//对于传入非 int 与 double 型的参数，将会报错
//与普通函数不同的是，看似函数只有返回值不一样，但是并不会被认为是重定义
//这是模板函数的一个好用的特性
// Example 1
template<typename T>
enable_if_t<is_same<T, int>::value, T> fac() {
    return 1;
}

template<typename T>
enable_if_t<is_same<T, double>::value, T> fac() {
    return 3.14;
}

auto INT = fac<int>(), DOUBLE = fac<double>();

//等价形式，不同之处在于函数形式fac<int>()，与fac(int)
template<typename T>
enable_if_t<is_same<T, int>::value, T> fac(T joke) { return 1; }

template<typename T>
enable_if_t<is_same<T, double>::value, T> fac(T joke) { return 3.14; }

auto INT = fac(1), DOUBLE = fac(3.14);
```

4. 作函数参数

```c++
template<class T>
void fac(T, enable_if_t<is_same<T, int>::value>* = nullptr) {
    cout << "1\n";
}

fac(1);//成功
fac(3.14)//编译报错
    
//等价形式同 3 中所述，稍加修改可用 fac<int>() 替换，下面不再赘述
template<class T, class U>//Variable templates: Since C++17
inline constexpr bool is_same_v = is_same<T, U>::value;
```

5. 作模板参数

```c++
// #5.1 enabled via a non-type template parameter
template<class T,
		enable_if_t<
    is_same_v<T, int>,
	bool> = true
>
void print() {
	cout << "sure\n";        
}

// #5.2 enabled via a type template parameter
template<class T,
	typename = std::enable_if_t<std::is_array<T>::value> >
void destroy(T* t) // note: function signature is unmodified
{
    for(std::size_t i = 0; i < std::extent<T>::value; ++i) {
        destroy((*t)[i]);
    }
}
/*
//error: has the same signature with #5.2
template<class T,
	typename = std::enable_if_t<std::is_void<T>::value> >
void destroy(T* t){} 
*/
```



### Variadic Templates

- 可变模板参数对参数进行了高度泛化，**它能表示 0 到任意个数、任意类型的参数。**使用时和模板元编程一样常常结合 type_traits 库使用

```c++
// Example
// parameter pack
template<typename... T>
class Car<T...> { };

template<typename... T>
class BMW : public Car<T...> { };

BMW<int, char> car;
```

- 可变模板参数在 c++11 之后加入，但是并不能很好的使用，需要结合一些方法进行包的展开。

0. 查看包内参数的数目，可以使用 sizeof 函数。

```c++
// 下面这个函数使用 sizeof...(args) 打印包内参数的数目
template<typename... T>
void print(T... args) {
    cout << sizeof...(args) << endl;
}
```

1. 递归方式展开包

- 下面的函数打印每个传入的参数，**① **我们设置了一个终止递归的 print 函数，它只接受一个参数。**② **设计了一个用于递归的 print 函数，它接受两个参数。**③** 递归 print 的作用是取出参数包的第一个参数，而后把剩下的参数传入 print 递归调用自己。**④** 这样当递归到只剩下最后一个参数时，将会调用我们设置的只接受一个参数的 print 函数，从而实现相应的功能。
- **注意只接受一个参数的 print 必须写在前面，否则编译会出错**

> 具体可以参考 c++ 模板函数的优先次序。当然，你也可以设置一个接受 0 个参数的 print 函数和相应的用于递归的 print 函数来实现。

```c++
// 用于终止的 print 函数 #1
template <typename T>
void print(T arg) {
    cout << arg << endl;
}

// 用于终止的 print 函数 #2
// void print() { }

// 用于递归的 print 函数
template <typename First, typename... Rest>
void print(First first, Rest... args) {
    cout << first << endl;
    print(args...);
}

可以想想，为什么我们要用 #1 而不用 #2？
```

2. 逗号表达式展开。利用逗号表达式的性质，我们可以省去递归这种书写方式。

- 下面方法的原理是这样的
- 首先 c++11 之后，可以利用大括号初始化，大括号的背后是 std::initializer_list 类型，而 std::initializer_list 接受一个模板参数 T 。它的内部有指向传入参数首个元素，和末尾元素的指针。也就是说，我们需要将变参转换为统一的某个类型 T，因为 std::initializer_list 不能接受多个类型。
- 因此我们用逗号表达式完成这个工作，逗号前进行打印，逗号后为一个常量右值，则逗号表达式的结果为这个常量右值，所以我们接下来就可以借助大括号来完成包的展开了。展开后是这样的`{(print(arg0), false), (print(arg1), false), (print(arg2), false)........} `

```c++
template<typename T>
void print(T arg) { cout << arg << endl; }

// 利用 初始化列表 和 逗号表达式 进行包展开
template<typename... T>
void expand(T... args) {
    auto q = {(print(arg), false)...};
    // 注意 q 是 std::initializer_list 类型哦
    // 实际上也可以这样写
    // bool q[] = {(print(arg), 0)...};
}
```

3. 继承方式展开包

- **经典实现可以参考 STL 内 tuple 的实现，就用到了继承**

```c++
// 前置声明
template<typename... T>
class Base;

// 终止的情况, 空类
template <>
class Base<> { };

// 非空的情况
template<typename First, typename... Rest>
class Base<First, Rest...> : public Base<Rest...> {
    /* ...something... */
};
```



2. 可变模板参数与完美转发

```c++
template<typename... T>
void foo(T... args) { }

//我需要下面这个函数转发变参给foo()
template<typename... T>
void myforward(T&&... args) {
    foo(std::forward<T>(args)...);
}
```









### Folder expression(Since c++17)

- **运用该特性可以简化 Variadic Templates 的很多书写**

- 折叠表达式的展开形式和运算顺序

> 一元左折叠（binary left fold）
> ( ... op pack )
> 一元左折叠 (... op E) 展开之后变为 ((E1 op E2) op ...) op En
>
> 
>
> 一元右折叠（binary right fold）
> ( pack op ... )
> 一元右折叠 (E op ...) 展开之后变为 E1 op (... op (En-1 op En))
>
> 
>
> 二元左折叠（binary left fold） 
> ( init op ... op pack )
> 二元左折叠 (I op ... op E) 展开之后变为 (((I op E1) op E2) op ...) op En
>
> 
>
> 二元右折叠（binary right fold） 
> ( pack op ... op init )
> 二元右折叠 (E op ... op I) 展开之后变为 E1 op (... op (EN−1 op (EN op I)))

- 参考以下示例

```c++
// 1 一元左折叠
template<typename... T>
int minus(T... args) {
    return args - ...;
}
auto q = minus(10,5,3,1);
//q = 7, (10 - (5 - ( 3 - 1 ))) = 7

// 2 一元右折叠
template<typename... T>
int minus(T... args) {
    return ... - args;
}
auto q = minus(10, 5, 3, 1);
//q = 1, (((10 - 5) - 3) - 1) = 1

// 3 二元左折叠
template<class... T>
int minus(T... args) {
    return (100 - ... - args);
}
auto q = minus(10, 5, 3, 1);
//q = 81, ((((100 - 10) - 5) - 3) - 1) = 81


// 4 二元右折叠
template<class... T>
int minus(T... args) {
    return (args - ... - 100);
}
auto q = minus(10, 5, 3, 1);
//q = 107, (10 - (5 - (3 - (1 - 100)))) = 107
```



### Curiously Recurring Template Pattern

> 参考链接：https://zhuanlan.zhihu.com/p/137879448

何为CRTP？

- 继承自模板类
- 派生类将自身作为参数传给模板类

CRTP的常见用法

1. 实现静态多态

```c++
template <class T> 
struct Base {
    void interface() {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }

    static void static_func() {
        // ...
        T::static_sub_func();
        // ...
    }
};

struct Derived : Base<Derived> {
    void implementation()；

    static void static_sub_func();
};
```

以`Base<Derived>::interface()` 为例，在`Derived : Base<Derived>`中，`Base<Derived>`是先于`Derived`而存在的，所以当`Base<Derived>::interface()`被申明时，编译器并不知道`Derived`的存在的，但由于此时` Base<Derived>::interface()` 并不会被实例化。只有当`Base<Derived>::interface()`被调用时，才会被实例化，而此时编译器也已经知道了 `Derived::implementation()`的声明了

2. 实现各个子类实例创建和析构独立的计数

```c++
template <typename T>
struct counter {
    static int objects_created;
    static int objects_alive;

    counter() {
        ++objects_created;
        ++objects_alive;
    }
    
    counter(const counter&) {
        ++objects_created;
        ++objects_alive;
    }
protected:
    // objects should never be removed through pointers of this type
    ~counter() {
        --objects_alive;
    }
};
template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );

class X : counter<X> { };

class Y : counter<Y> { };
```

每次X或者Y实例化时，`counter<X>`或者` counter<Y>`就会被调用，对应的就会增加对创建对象的计数。同样，每次X或者Y悉构后，对应的减少`objects_alive`。这里最重要的是实现了对不同子类单独的计数。

3. 多态链（Polymorphic chaining）

    还是通过一个具体的例子来对此进行说明。

```cpp
class Printer
{
public:
    Printer(ostream& pstream) : m_stream(pstream) {}
 
    template <typename T>
    Printer& print(T&& t) { m_stream << t; return *this; }
 
    template <typename T>
    Printer& println(T&& t) { m_stream << t << endl; return *this; }
private:
    ostream& m_stream;
};

class CoutPrinter : public Printer
{
public:
    CoutPrinter() : Printer(cout) {}

    CoutPrinter& SetConsoleColor(Color c)
    {
        // ...
        return *this;
    }
};
```

​	上面Printer定义打印的方法，`CoutPrinter`是`Printer`的子类，并且添加了一个设置打印颜色的方法。接下来我们看看下面这行代码：

```cpp
CoutPrinter().print("Hello ").SetConsoleColor(Color.red).println("Printer!");
```

​	前半段`CoutPrinter().print("Hello ")`调用的是`Printer`实例，后面接着`SetConsoleColor(Color.red)`实际上又需要调用`CoutPrinter`实例，这样编译器就会报错。

而CRTP就可以很好的解决这个问题，代码如下：

```c++
// Base class
template <typename ConcretePrinter>
class Printer
{
public:
    Printer(ostream& pstream) : m_stream(pstream) {}
 
    template <typename T>
    ConcretePrinter& print(T&& t)
    {
        m_stream << t;
        return static_cast<ConcretePrinter&>(*this);
    }
 
    template <typename T>
    ConcretePrinter& println(T&& t)
    {
        m_stream << t << endl;
        return static_cast<ConcretePrinter&>(*this);
    }
private:
    ostream& m_stream;
};
 
// Derived class
class CoutPrinter : public Printer<CoutPrinter>
{
public:
    CoutPrinter() : Printer(cout) {}
 
    CoutPrinter& SetConsoleColor(Color c)
    {
        // ...
        return *this;
    }
};
 
// usage
CoutPrinter().print("Hello ").SetConsoleColor(Color.red).println("Printer!");
```



4. 多态的复制构造体

当我们用到多态时，经常会需要通过基类的指针来复制子对象。通常我们可以通过在基类里面构造一个`clone()`虚函数，然后在每个子类里面定义这个`clone()`函数。使用CRTP可以让我们避免反复地在子类中去定义`clone()`函数。

```c++
// Base class has a pure virtual function for cloning
class AbstractShape {
public:
    virtual ~AbstractShape () = default;
    virtual std::unique_ptr<AbstractShape> clone() const = 0;
};

// This CRTP class implements clone() for Derived
template <typename Derived>
class Shape : public AbstractShape {
public:
    std::unique_ptr<AbstractShape> clone() const override {
        return std::make_unique<Derived>(static_cast<Derived const&>(*this));
    }

protected:
   // We make clear Shape class needs to be inherited
   Shape() = default;
   Shape(const Shape&) = default;
};

// Every derived class inherits from CRTP class instead of abstract class
class Square : public Shape<Square>{};
class Circle : public Shape<Circle>{};
```





### About C++20------Main features

#### Concept

#### Range

#### Coroutine

#### Module



