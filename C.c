// C23
// bool 关键字转正，代替 _Bool
// true false 成为关键字
// 弃用头文件<stdbool.h>

// C23
// static_assert 成为关键字
// thread_local 成为关键字
// nullptr 空指针常量及其类型 nullptr_t

// C23
// auto 类型推导

// C23
// typeof，类似C++的decltype
// typeof_unqual，去掉cvr限定符

// C23
// constexpr 存储说明符
// 与C++不同，constexpr不能用于函数

// C23
// 废除旧的 K&R C 的函数定义
// 函数参数列表为空表示不接受参数，与C++行为一致
// 允许函数参数省略名称，与C++行为一致

// C23 C++14
// 二进制整数字面量
// 单引号整数分隔符

// C23 C++11
// 数据对齐
// alignof 运算符可以用于获取类型的对齐字节长度
// alignas 说明符可以用来改变类型的默认对齐字节长度

// C23 C++20
// 可选宏 __VA_OPT__
// 可以在可变参数为空的情况下使用
#define LOG(msg, …) printf("[" __FILE__ ":%d] " msg, __LINE__ __VA_OPT__(, ) __VA_ARGS__) // 逗号是可选的

// C23 C++23
// #elifdef #elifndef
// #ifdef 和 #ifndef 分别是 #if defined() 和 #if !defined() 的简写，而C23和C++23之前 #elif defined() 和 #elif !defined() 却并没有与之对应的简写指令

// C23 C++23
// #warning
#ifndef FOO
#warning "FOO defined, performance might be limited"
#endif

// C23 C++26
// #embed 嵌入资源
// 一般用法
const unsigned char some_data[] =
#embed "data.dat"
;