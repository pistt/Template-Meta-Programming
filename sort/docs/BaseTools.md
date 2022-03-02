## 基本组件——Basetools

### length 求序列长度

使用 length\<sequence\>::value 获得序列的长度，返回 int 类型

e.g. length\<int_seq\<1,2,3,4,5\>\>::value 的值为 5 



### empty 判断序列是否为空

使用 empty\<sequence\>::value 判断序列是否为空，返回 bool 类型



### push_front 往序列头部放入一个值

使用 push_front\<int, sequence\> 往序列头部插入一个 int 值

使用 push_front\<int, sequence\>::type 获得放入 int 值后序列的类型

e.g. push_front\<10, int_seq\<1,2,3,4\>\>::type 类型为 int_seq\<10,1,2,3,4\>



### push_back 往序列尾部放入一个值

和 push_front 用法相同，只是往序列尾部放入 int 值



### erasehead 删除头部元素

使用 erasehead\<sequence\> 删除序列头部元素

使用 erasehead\<sequence\> ::type 获取删除头部元素后的序列类型



### gethead 获取头部元素的值

使用 gethead\<sequence\>::value 获取序列头部元素的值



### swaphead 交换给定的 val 和序列头部元素的值

使用 swap\<int, sequence\> 交换给定的 int 值和序列头部元素的值

使用 swap\<int, sequence\>::type 获取交换后的序列类型



### concat 连接两个序列

使用 concat\<sequence1, sequence2\> 连接两个序列

使用 concat\<sequence1, sequence2\>::type 获取连接序列后的序列类型

e.g. concat\<int_seq\<1,2\>, int_seq\<5,9\>\>::type 类型为 int_seq\<1,2,5,9\>



### reverse 反转序列

使用 reverse\<sequence\> 来反转整个序列

使用 reverse\<sequence\>::type 来获取反转后的序列类型

e.g. reverse\<int_seq<1,2,3\>\>::type 类型为 int_seq\<3,2,1\>



### transform 对序列进行某种变换，需要自定义变换函数

transform 是高阶函数，用法见例子。你需要定义一个返回 **constexpr static int 类型并且名称为 value **的基本函数，该基本函数必须接受且只接受一个 int 参数。

```c++
// 基本函数
template <int val>
struct add {
	constexpr static int value = val + 2;
};

transform<add, int_seq<1,2,3,4>>::type 的类型为 int_seq<3,4,5,6>
```



### filter 对序列进行过滤，需要自定义过滤函数

transform 也是高阶函数，用法见例子。你需要定义一个返回 **constexpr static bool 类型并且名称为 value **的基本函数，该基本函数必须接受且只接受一个 int 参数。

```c++
// 基本函数
template <int val>
struct isodd {
	constexpr static bool value = (val % 2 == 1);
};

filter<isodd, int_seq<1,2,3,4,5,6>>::type 的类型为 int_seq<1,3,5>
```



### print 打印序列内容

print\(int_seq<90,80,70,10,20,30>\(\)\) 输出结果将为：

> 90 80 70 10 20 30 和一个换行符