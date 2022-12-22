#include<iostream>

// 调用约定
// 调用约定写在函数返回类型的后面，函数名的前面
int __stdcall add(int a, int b);

// 对于函数指针，调用约定写在首层指针*的前面
int(__cdecl*p)(int, int);	// p的类型是int(__cdecl*)(int, int)

// 调用约定是函数类型的一部分，函数的声明和定义处调用约定要相同，不能只在声明处有调用约定，而定义处没有或与声明不同
int __stdcall add(int a, int b);
int add(int a, int b) {return a + b;}	// error

// __stdcall
// Windows和Win32API的标准调用约定
// 参数从右至左入栈，被调函数清理栈空间，返回值在EAX
// 函数名前加一个下划线，函数名后加@和函数参数的字节数
// 如int __stdcall add(int a, int b)的修饰名是_add@8

// __cdecl
// C/C++默认调用约定，可变参数成员函数只能使用__cdecl
// 参数从右至左入栈，主调函数清理栈空间，返回值在EAX
// 函数名前加一个下划线修饰
// 如int __cdecl add(int a, int b)的修饰名是_add

// __fastcall
// 通过寄存器传送参数，用ECX和EDX传送从左至右前两个小于等于4字节的参数，剩下的参数从右至左入栈，通过寄存器传送的两个参数是从左至右的，即第1个参数进ECX，第2个参数进EDX
// 被调函数清理栈空间，返回值在EAX
// 函数名前加@，函数名后加@符号和函数参数的字节数
// 如double __fastcall add(double a, double b)的修饰名是@add@16

// __thiscall
// C++类成员函数默认调用约定，仅用于C++成员函数，故__thiscall没有显式的声明形式
// 参数从右至左入栈，如果参数个数确定，this指针通过ECX传送，如果参数个数不确定，this指针最后一个入栈
// 如果参数个数确定，被调函数清理栈空间，如果参数个数不确定，主调函数清理栈空间

// C++11
inline namespace{}  // 内联命名空间
// 内联命名空间能够把空间内的函数和类型导出到父命名空间中
// 意义是可以帮助库的作者无缝升级库代码，将最新版本的接口以内联的方式导出到父命名空间
// 存在同名函数时，只能有一个内联命名空间，否则会造成二义性问题

// C++17
// 嵌套命名空间简化语法
namespace A::B::C{}

// C++20
// 内联命名空间简化语法
namespace A::B::inline C{}
namespace A::inline B::C{}

// auto
// 推导规则是从左往右的，以最左边的表达式推断auto的类型
int n = 5;
auto *pn = &n, m = 10;  // 这里的auto被推导为int

// auto不支持缩窄转换

// 使用条件表达式初始化auto变量时，总是使用表达能力更强的类型
auto i = true ? 5 : 8.0;    // i的类型为double

// auto不能声明非静态成员变量，如
struct sometype{
    auto i = 5;	// 错误
};

// C++11
// const静态成员变量可以用auto初始化
struct sometype{
    static const auto i = 5;	
};

// C++17
// inline静态成员变量可以用auto初始化
struct sometype{
    static inline auto i = 5;	
};
struct sometype {
    static inline auto i = myStruct();
};

// C++14
// lambda表达式用auto声明形参
auto f = [=](auto i, auto j) {};

// C++20
// 函数用auto声明形参
void func(auto str) {}

auto+new	// 避免这样写
auto i = new auto(5);
auto* j = new auto(5);

// auto的推导规则

// 如果使用auto时未使用引用和指针，则忽略cv限定符，否则推导cv限定符
const int i = 5;
auto j = i;		// auto是int
auto* k = &i;	// auto是const int，k是const int*
auto& m = i;	// auto是const int，m是const int&

// auto本身支持添加cv限定符
const auto u = j;

// 使用auto声明变量初始化时，如果目标对象是引用，则引用属性会被忽略
int i = 5;
int& j = i;
auto k = j;		// auto是int而非int&

// 使用auto和万能引用声明变量时，如果目标对象是左值，将推导为引用类型
int i = 5;
auto&& m = i;	// auto是int&，m是int&，引用折叠
auto&& j = i;	// auto是int

// 使用auto声明变量，如果目标对象是数组或函数，则auto会被推导为对应的指针类型
int i[5];
auto m = i;		// auto是int*

int sum(int a1, int a2) {return a1 + a2;}
auto j = sum;	// auto是int(__cdecl*)(int, int)

// 使用auto {}列表初始化时，列表中必须为单元素，auto类型被推导为单元素的类型
auto x1{3};     // x1为int
auto x2{4, 5};  // error

// 使用auto = {}列表初始化时，列表中可以包含单个或者多个元素，
// auto类型被推导为std::initializer_list<T>，元素的类型必须相同
auto x3 = {1, 2, 3};     // x3为std::initializer_list<int>
auto x4 = {1, 2.0};      // error

// 基类指针解引用初始化auto变量时，派生类对象被切割成基类，并调用复制构造函数，此时调用的虚方法是基类的
// 基类指针解引用初始化auto&引用变量时，由于auto变量被推导为引用，调用派生类的虚方法
Base* a = new Derived();
auto b = *a;    // auto是Base，b是Base
b.func();       // 调用Base::func()
auto& c = *a;   // auto是Base，c是Base&
c.func();       // 调用Derived::func()

// C++14
// 返回类型为auto的推导
auto sum(int a1, int a2) {return a1 + a2;}
// 当控制流存在多重返回值时，要保证返回值类型相同
auto sum(long a1, long a2) {
    if(a1 > 0)
        return a1 + a2;     // 返回long
    else
        return 0;          // 返回int，编译失败
}

// C++14
// lambda表达式auto
auto l = [](auto a1, auto a2){return a1 + a2;};
auto retval = l(5, 5.0);    // a1是int，a2是double，retval是double
// lambda表达式后置返回类型auto
auto l = [](int& i)->auto& {return i;};
auto x1 = 5;
auto& x2 = l(x1);
assert(&x1 == &x2);     // x1和x2的内存地址相同

// C++17
// 非类型模板形参占位符
template<auto N>
void f(){std::cout << N;}
int main(){
    f<5>();     // N是int
    f<5.0>();   // 编译失败，模板参数不能为double
    f<'c'>();   // N是char
}

// typeid运算符
// typeid可以作用于表达式和类型，获取的类型信息包含在std::type_info对象里，调用name()方法获取其类型名称
int x1 = 0;
double x2 = 5.5;
std::cout << typeid(x1).name() << typeid(x1 + x2).name() << typeid(int).name() << std::endl;
// typeid返回的是一个左值，且其生命周期被扩展到与程序生命周期相同
// std::type_info删除了复制构造函数，只能获取其引用或指针来保存该信息
auto t1 = typeid(int);  // 编译失败
auto& t2 = typeid(int); // t2是const std::type_info&
auto t3 = &typeid(int); // t2是const std::type_info*
// typeid的返回值总是忽略类型的cv限定符，即typeid(const T) == typeid(T)



// decltype
