# 模板元排序

[TOC]



## 详细参考文档请见 docs 文件夹中内容

- 具体的实现方法请参考源代码及注释

## 个人项目说明

Linux 下使用 make 查看 SortTest.cpp 中测试例子的测试效果

### 项目实现的编译期的五种基本排序方法(以整型为例)

- 冒泡排序

- 插入排序
- 选择排序

- 快速排序

- 归并排序

### 实现思路——**基于类型的变换**

1. 首先，实现了一个可变长的序列 seq\<type, val1, val2, val3...\> ，并对 int 类型进行了特化，名称为 int_seq 。例如 int_seq\<1,2,3,4,5,6\> 即是一个整型序列类型，其中的参数也是类型的一部分。
2. 而后，基于这个序列类型，实现了诸如求长度，判空，翻转，插入等基本操作方法。
3. 最后具体实现每个排序，这些排序操作都在**基于类型的变换上**进行。
4. int_seq\<90,70,80,10,50,20\> 排序后类型变为 int_seq\<10,20,50,70,80,90\>



### 基本架构图

<img src=".\images\architecture.png" alt="测试效果1" style="zoom:50%;" align="left" />



### 关于模板元一些有趣的补充 Sth_Interesting

一些关于模板元的实用视频：

[Pure C++ 2020: 跨语言脚本引擎ScriptEngine](https://www.bilibili.com/video/BV1vh411275E)

[Pure C++ 2020: Concept Driven Design](https://www.bilibili.com/video/BV1B54y1s7dp)

在 Sth_Interesting.cpp 中有关于本项目之外的模板元的另外一些有趣的补充



### CompareFuncs

自定义的用于比较的比较元函数，类似于使用 std::sort 时需要给定的比较规则



### 测试效果

使用 `auto p = sort_functions<CompareFuncs, sequence>::type();   `即可实例化一个某序列类型的变量

1. 测试效果1

<img src=".\images\Test1.png" alt="测试效果1" style="zoom:50%;" align="left" />

2. 测试效果2

<img src=".\images\Test2.png" alt="测试效果2" style="zoom:50%;" align="left" />



## 使用和阅读前提

1. 本文假设你达到了一些基本的要求
    - C++ 基本语法
    - 模板的相关概念及其基本用法
    - 了解模板元的概念
    - 了解 C++11，14 的常用特性
    - 如果对于 C++17 和 C++20 有所了解，那么将会更有帮助

2. 对 SFINAE (Substitution Failure Is Not An Error) 应该有所了解，并能够明白运用 SFINAE 做编译期的推导和替换。
3. 对 C++ 头文件 \<type_traits\> 要有一定的了解，了解其基础构件 integral_constant 



## 可供模板元入门和进阶参考的博客和书目

- 经典巨著《C++ Primer》，
- 著名书籍 《C++ Templates》
- 高博译《Effective Modern C++》
- 李伟《C++ 模板元编程实战》
- Youtube 上侯捷老师的视频 [C++11 新特性](https://www.youtube.com/watch?v=TJIb9TGfDIw&list=PL-X74YXt4LVYo_bk-jHMV5T3LHRYRbZoH&index=2)
- Github 项目 [CPPTemplateTutorial(施工中)](https://github.com/wuye9036/CppTemplateTutorial)
- 知乎博主张雅宸：[C++11-17 模板核心概念](https://www.zhihu.com/column/c_1306966457508118528)
- 知乎博主郭不昂：[单手敲代码](https://www.zhihu.com/column/c_1384125795212886016)
- 其它博客等等，可自行用搜索引擎搜索
