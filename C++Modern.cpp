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
auto&& j = 5;	// auto是int

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

// C++11
// decltype可以获取表达式的类型
int x1 = 0;
double x2 = 1.0;
decltype(x1) x3 = 1;    // x3是int
decltype(x1 + x2) x4 = x1 + x2;     // x1 + x2是double

// auto不能在非静态成员变量中使用，但decltype可以
struct S1{
    int x1;
    double x2;
    decltype(x1) x3;
    decltype(x1 + x3) x4;
};

// 可以在函数形参列表和返回值中使用
int x1 = 0;
decltype(x1) sum(decltype(x1) a1, decltype(a1) a2) {return a1 + a2;}
auto x2 = sum(5, 10);

// C++11
// 在C++11中，auto不能直接用于推导函数返回类型，需要使用后置返回类型
auto sum(int a1, int a2)->int {return a1 + a2;}

// 但是，当使用函数模板时
template<typename T>
T sum(T a1, T a2) {return a1 + a2;}
auto x1 = sum(5, 5.0);  // 编译失败，无法确定T的类型

// 因此只能编写一个更宽泛的模板
template<typename R, typename T1, typename T2>
R sum(T1 a1, T2 a2) {return a1 + a2;}
auto x2 = sum<double>(5, 5.0);

// 如果想让编译器完成返回值类型推导
template<typename T1, typename T2>
auto sum(T1 a1, T2 a2)->decltype(a1 + a2) {return a1 + a2;}
auto x3 = sum(5, 5.0);

// C++14
// C++14已经支持函数模板的返回类型推导
template<typename T1, typename T2>
auto sum(T1 a1, T2 a2) {return a1 + a2;}
auto x4 = sum(5, 5.0);

// 但是在返回引用时
template<typename T>
auto return_ref(T& t) {return t;}
int x1;
static_assert(
    std::is_reference_v<
        decltype(return_ref(x1))>);     // 编译错误，返回值不是引用
// 因为auto被推导为值类型

// 这时用到decltype
template<typename T>
auto return_ref(T& t)->decltype(t) {return t;}
int x1;
static_assert(
    std::is_reference_v<
        decltype(return_ref(x1))>);     // 编译成功

// decltype(e)的推导规则
// e的类型为T
// 如果e是一个不加括号的标识符表达式（除了结构化绑定外）或者不加括号的类成员访问，则推导的类型是e的类型T，如果不存在这样的类型或者e是一组重载函数，则推导失败
// 如果e是一个函数调用或仿函数调用，则推导的类型是其返回值类型
// 如果e是一个类型为T的左值，则推导的类型是T&
// 如果e是一个类型为T的将亡值，则推导的类型是T&&
// 除上述之外的情况，推导为T

const int&& foo();
int i;
struct A{double x;};
const A* a = new A();

decltype(foo());    // 推导为const int&&
decltype(i);        // int
decltype(a->x);     // double
decltype((a->x));   // 带括号，是左值，且a是const指针，推导为const double&

const int&& foo();
int i;
int *j;
int n[10];

decltype(static_cast<short>(i));    // decltype(static_cast<short>(i))推导类型为short
decltype(j);        // decltype(j)推导类型为int*
decltype(n);        // decltype(n)推导类型为int[10]
decltype(foo);      // 不是函数调用，而是函数指针，推导类型为const int&& (void)

struct A {
  int operator() () { return 0; }
};

A a;
decltype(a());      // a是仿函数，decltype(a())推导为返回值类型int

int i;
int *j;
int n[10];
decltype(i = 0);                    // 左值，decltype(i = 0)推导类型为int&
decltype(0, i);                     // 逗号表达式，左值，decltype(0, i)推导类型为int&
decltype(i, 0);                     // 逗号表达式，字面量，decltype(i, 0)推导类型为int
decltype(n[5]);                     // 左值，decltype(n[5])推导类型为int&
decltype(*j);                       // 左值，decltype(*j)推导类型为int&
decltype(static_cast<int&&>(i));    // 将亡值，decltype(static_cast<int&&>(i))推导类型为int&&
decltype(i++);                      // 纯右值，decltype(i++)推导类型为int
decltype(++i);                      // 左值，decltype(++i)推导类型为int&
decltype("hello world");            // 字符串字面量，const，左值，decltype("hello world")推导类型为const char(&)[12]

// cv限定符的推导
// 通常情况下，decltype(e)所推导的类型会同步e的cv限定符
const int i = 0;
decltype(i);        // decltype(i)推导类型为const int
// 但当e是不加括号的成员变量时，父对象表达式的cv限定符会被忽略，不能同步到推导结果
struct A {
    double x;
};
const A* a = new A();
decltype(a->x);    // decltype(a->x)推导类型为double, const属性被忽略
// 如果加上括号，则能够同步父对象的cv限定符
decltype((a->x));    // decltype((a->x))推导类型为const double

// C++14
// decltype(auto)
// 让编译器使用decltype的推导规则来推导auto
// decltype(auto)不能结合指针、引用和cv限定符
int i;
int&& f();
auto x1a = i;                   // x1a推导类型为int
decltype(auto) x1d = i;         // x1d推导类型为int
auto x2a = (i);                 // x2a推导类型为int
decltype(auto) x2d = (i);       // x2d推导类型为int&
auto x3a = f();                 // x3a推导类型为int
decltype(auto) x3d = f();       // x3d推导类型为int&&
auto x4a = { 1, 2 };            // x4a推导类型为std::initializer_list<int>
decltype(auto) x4d = { 1, 2 };  // 编译失败, {1, 2}不是表达式
auto *x5a = &i;                 // x5a推导类型为int*
decltype(auto)*x5d = &i;        // 编译失败，decltype(auto)必须单独声明，不能结合指针、引用、cv限定符

// C++17
// decltype(auto)作为非类型模板形参占位符，推导规则与上述相同
template <decltype(auto) N>
void f()
{
    std::cout << N << std::endl;
}

static const int x = 11;
static int y = 7;

int main()
{
    f<x>();   // N为const int类型
    f<(x)>(); // N为const int&类型
    f<y>();   // 编译错误，y不是一个常量，无法实例化函数模板
    f<(y)>(); // N为int&类型，(y)被推断为引用类型，恰好静态对象的内存地址是固定的
}

// C++11
// 函数返回类型后置
auto foo() -> int { return 42; }

// 当函数返回函数指针时，传统函数声明无法将函数指针类型作为返回类型，需要创建别名
int bar_impl(int x) { return x; }
typedef int (*bar)(int);    // bar是一个函数指针
bar foo1() { return bar_impl; }

// 函数返回类型后置，auto作为返回类型占位符，在->后声明返回的函数指针类型int (*)(int)即可
auto foo2() -> int (*)(int) { return bar_impl; }

// C++11
// 返回类型后置与auto、decltype结合，推导函数模板的返回类型
template <class T1, class T2>
auto sum1(T1 t1, T2 t2) -> decltype(t1 + t2)
{
    return t1 + t2;
}

// 只使用decltype，不使用auto占位符
// 一种容易理解的写法，但是这种写法可能存在问题
// 虽然编译器在推导表达式类型的时候并没有真正计算表达式，但是会检查表达式是否正确，所以仍然要求T1和T2的默认构造函数必须存在
template <class T1, class T2>
decltype(T1() + T2()) sum2(T1 t1, T2 t2)
{
    return t1 + t2;
}

// 函数模板sum3使用指针类型转换和解引用求和的方法推导返回值
// *static_cast<T1*>(nullptr) + *static_cast<T2*>(nullptr)将nullptr转换为T1和T2的指针类型，然后解引用求和，最后利用decltype推导出求和后的对象类型
// 由于编译器不会真的计算求值，因此这里求和操作不会有问题
template <class T1, class T2>
decltype(*static_cast<T1*>(nullptr) + *static_cast<T2*>(nullptr)) sum3(T1 t1, T2 t2)
{
    return t1 + t2;
}

// 标准库提供了一个std::declval函数模板声明，它将类型T转换成引用类型，这样在使用decltype推导表达式类型时不必经过构造函数检查
// 这是declval的一个简化声明，declval<T1>() + declval<T2>()表达式分别通过declval将T1和T2转换为引用类型并且求和，最后通过decltype推导返回类型
template <class T>
T &&declval();

template <class T1, class T2>
decltype(declval<T1>() + declval<T2>()) sum4(T1 t1, T2 t2)
{
    return t1 + t2;
}

