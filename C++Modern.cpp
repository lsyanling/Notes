#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <future>
#include <type_traits>
#include <tuple>

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
const auto n = j;

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
// 函数返回类型后置，C++14之后不必这样写
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
T&& declval();

template <class T1, class T2>
decltype(declval<T1>() + declval<T2>()) sum4(T1 t1, T2 t2)
{
    return t1 + t2;
}



// C++11
// 右值引用

// 左值指一个指向特定内存的具有名称的值（具名对象），它有一个相对稳定的内存地址，并且有一段较长的生命周期
// 右值则是不指向稳定内存地址的匿名值（不具名对象），它的生命周期很短，通常是暂时性的
// 基于这一特征，可以用取址运算符&来判断左值和右值，能取到内存地址的值为左值，否则为右值

// const左值引用可以引用右值
const int &x = 11;
// 这使得复制构造和复制赋值可以传入右值
class X
{
public:
    X() {}
    X(const X &) {}
    X &operator=(const X &) { return *this; }
};
X make_x() { return X();}
int main() {
    X x1;
    X x2(x1);
    X x3(make_x());
    x3 = make_x();
}
// 但是const引用使得无法在函数内部修改对象（强制类型转换除外）

// 右值引用
// 右值引用可以延长临时对象生命周期，达到减少复制的目的

// 移动语义，包括移动构造函数和移动赋值运算符
// 编译器在 [没有复制函数，没有移动函数，没有析构函数] 的情况下会生成一份移动构造函数
// 虽然条件很严苛，但实际上编译器生成的移动构造函数和复制构造函数并没有区别
// 为避免只移动了一部分资源，应确保存在移动语义的函数不会抛出异常
// 如果无法保证移动构造函数不会抛出异常，可以使用noexcept说明符限制该函数，这样，当函数抛出异常的时候，程序不会再继续执行而是调用std::terminate中止执行以免造成其他不良影响


// C++17
// 值类别：泛左值glvalue和右值rvalue，左值lvalue、将亡值xvalue和纯右值prvalue

// 泛左值是指一个通过评估能够确定对象、位域或函数的标识的表达式。简单来说，它确定了对象或者函数的标识（具名对象）
// 纯右值是指一个通过评估能够用于初始化对象和位域，或者能够计算运算符操作数的值的表达式
// 将亡值属于泛左值的一种，它表示资源可以被重用的对象和位域，通常这是因为它们接近其生命周期的末尾，另外也可能是经过右值引用的转换产生的
// 左值是指非将亡值的泛左值，而右值则包含了纯右值和将亡值
// 值类别都是表达式的属性，所以左值和右值实际上指的是表达式
// 这里的左值（lvalue）就是C++98的左值，而这里的纯右值（prvalue）则对应C++98的右值，现在只需要弄清楚将亡值（xvalue）是如何产生的就可以了


// 产生将亡值的途径有两种
// 第一种是使用类型转换将泛左值转换为该类型的右值引用
struct X {};
int main() {
    X a;
    static_cast<X&&>(a);
}

// C++17
// 产生将亡值的第二种途径称为临时量实质化，指纯右值转换到临时对象的过程
struct X { int x; };
int main() {
    int b = X().x;
}
// X()产生的是一个纯右值，访问其成员变量a需要一个泛左值，所以这里会发生一次临时量实质化，将X()转换为将亡值，再访问其成员变量a
// 在C++17之前临时变量是纯右值，只有转换为右值引用的类型才是将亡值


// C++11
// 将左值转换为右值
int i = 0;
int&& k = static_cast<int&&>(i);
// 由于转换的并不是右值，因此它没有改变生命周期和内存地址


// 转换的意义是让左值使用移动语义
class BigMemoryPool{};
BigMemoryPool my_pool1;
BigMemoryPool my_pool2 = my_pool1;
BigMemoryPool my_pool3 = static_cast<BigMemoryPool&&>(my_pool1);
// 为了用移动构造函数构造my_pool3，使用static_cast<BigMemoryPool&&>(my_pool1) 将my_pool1强制转换为右值（将亡值）
// 由于调用了移动构造函数，my_pool1失去了自己的内存数据，后面的代码也不能对my_pool1进行操作了


// 单纯地将一个左值数据转换到另外一个左值似乎并没有什么意义，正确的使用场景是在一个右值被转换为左值后需要再次转换为右值
// 典型的例子是将一个右值作为实参传递到函数中
// 无论一个函数的实参是左值还是右值，其形参都是一个左值，即使这个形参看上去是一个右值引用
// 例如函数move_pool()中的pool，尽管它是一个右值引用，但作为形参仍然是一个左值
void move_pool(BigMemoryPool&& pool) {
    BigMemoryPool my_pool(pool);
}
// 在下面的代码中，move_pool函数的实参是make_pool函数返回的临时对象，也是一个右值
// move_pool的形参是一个右值引用，但是在使用形参pool构造my_pool的时候还是会调用复制构造函数而非移动构造函数
BigMemoryPool get_pool(const BigMemoryPool& pool) { return pool; }
BigMemoryPool make_pool() {
    BigMemoryPool pool;
    return get_pool(pool);
}
int main() { 
    move_pool(make_pool()); 
}

// 为了让my_pool调用移动构造函数进行构造，需要将形参pool强制转换为右值
void move_pool(BigMemoryPool &&pool) {
    std::cout << "call move_pool" << std::endl;
    BigMemoryPool my_pool(static_cast<BigMemoryPool&&>(pool));
}
// 在这个场景下强制转换为右值没有任何问题，因为move_pool函数的实参是make_pool返回的临时对象，当函数调用结束后临时对象就会被销毁，所以转移其内存数据不会存在任何问题

// C++11
// std::move()
// 函数模板std::move用于将左值转换为右值，其内部也是用static_cast做类型转换
// 由于std::move是使用模板函数，因此会根据传参类型自动推导返回类型
// 而从移动语义上来说，使用std::move函数的描述也比自己使用static_cast类型转换更加准确


// C++11
// 万能引用和引用折叠
// const左值引用既可以引用左值又可以引用右值，但是由于其常量性，使用范围受到限制
// C++11中确实存在着一个被称为 万能 的引用，它看似是一个右值引用，但其实有着很大区别
void foo(int&& i) {}    // i为右值引用

template <class T>
void bar(T&& t) {}      // t为万能引用

int get_val() { return 5; }
int&& x = get_val();    // x为右值引用
auto&& y = get_val();   // y为万能引用
// 在上面的代码中，函数foo的形参i和变量x是右值引用，而函数模板的形参t和变量y则是万能引用
// 可见，所谓的万能引用是因为发生了类型推导

// 右值引用只能绑定一个右值，但是万能引用既可以绑定左值也可以绑定右值，甚至可以绑定const和volatile的值
int i = 42;
const int j = 11;
bar(i);
bar(j);
bar(get_val());

auto&& x = i;
auto&& y = j;
auto&& z = get_val();
// 在T&&和auto&&的初始化过程中都会发生类型的推导，如果已经有一个确定的类型，比如int&&，则是右值引用
// 在这个推导过程中，初始化的源对象如果是一个左值，则目标对象会推导出左值引用
// 反之，如果源对象是一个右值，则会推导出右值引用，不过无论如何都会是一个引用类型。

// 万能引用能够起作用是因为C++11中有一套引用叠加推导的规则，即引用折叠，这套规则规定了在不同的引用类型互相作用的情况下应该如何推导出最终类型
// 引用折叠
// 类模板型    T实际类型   最终类型
// T&          R           R&

// T&          R&          R&

// T&          R&&         R&

// T&&         R           R&&

// T&&         R&          R&

// T&&         R&&         R&&
// 上述规则里，只要有左值参与，推导类型就是左值引用
// 万能引用对左值推导为左值引用，对右值推导为右值引用

// 转发函数
// normal_forwarding()可以完成对字符串的转发，但是需要复制
template <class T>
void show_type(T t) {
    std::cout << typeid(t).name() << std::endl;
}

template <class T>
void normal_forwarding(T t) { show_type(t); }

int main() {
    std::string s = "hello world";
    normal_forwarding(s);
}

// 如果将上述normal_forwarding()的参数改为T&，就无法转发右值
std::string get_string() { return "hi world"; }
int main() {
    normal_forwarding(get_string()); // 编译失败
}

// 同样，如果将normal_forwarding()的参数改为const T&，就无法修改转发的内容

// C++11
// 完美转发
// perfect_forwarding()通过万能引用实现，如果实参是左值，则形参被推导为左值引用，如果实参是右值，则形参被推导为右值引用
// 无论传递的是左值还是右值都可以被转发，而且不会发生多余的临时复制
template <class T>
void perfect_forwarding(T&& t) {
    // t作为形参是一个左值，为了让转发将左右值的属性也带到目标函数中，这里需要进行类型转换
    // 当实参是一个左值时，T被推导为std::string&，于是static_cast<T&&>被推导为static_cast<std::string&>，传递到show_type函数时保持了左值引用的属性
    // 当实参是一个右值时，T被推导为std::string，于是static_cast<T&&>被推导为static_cast<std::string&&>，传递到show_type函数时保持了右值引用的属性
    show_type(static_cast<T&&>(t));
}
int main() {
    std::string s = "hello world";
    perfect_forwarding(s);
    perfect_forwarding(get_string());
}

// C++11
// std::forward
// 和std::move一样，C++11提供了一个std::forward函数模板，其内部也使用static_cast进行类型转换
// 使用std::forward转发语义会表达得更加清晰，注意模板实参是T而不是T&&
template<class T>
void perfect_forwarding(T&& t) {
    show_type(std::forward<T>(t));
}


// C++20
// 右值引用和throw的移动操作
// 在C++20之前，由于return的x作为形参是一个左值，返回应调用复制构造函数，除非调用std::move进行转换
// 但由于x是一个右值引用，存在优化的空间
X f(X &&x) {
    return x;
}
int main() {
    X r = f(X{});
}
// 因此，对于一个非易失或一个右值引用的非易失自动存储对象，在下面两种情况下可以使用移动代替复制
// return或者co_return语句中的返回对象是函数或者lambda表达式中的对象或形参
// throw语句中抛出的对象是函数或try代码块中的对象
void f() {
    X x;
    throw x;
}
int main() {
    try {
        f();
    }
    catch (...) {}
}
// 函数f不再有返回值，它通过throw抛出x，main函数用try-catch捕获f抛出的x，这个捕获调用的就是移动构造函数


// 复制消除
// 在以下情况，对象将直接构造到它们本来要复制/移动到的存储中
// 即使复制/移动构造函数和析构函数拥有可观察的副作用
// 复制构造和移动构造函数不必存在或可访问

// C++11
// 在 return 语句中，当操作数是一个与函数返回类型相同的类类型的纯右值（忽略 cv 限定）时
// 返回类型的析构函数必须在 return 语句位置可访问且未被删除，即使没有对象要被销毁
X f() {
    return X();
}
f();        // 只调用一次 X 的默认构造函数

// C++17
// 在对象的初始化中，当初始化器表达式是一个与变量类型相同的类类型的纯右值（忽略 cv 限定）时
X x = X(X(f()));    // 只调用一次 X 的默认构造函数以初始化 x
// 该规则只能在已知要初始化的对象不是可能重叠的子对象时应用    TODO 比较复杂，以后再看cppreference

// 描述C++17语义的另一种方式是 [传递未实质化的值]，返回并使用纯右值时不实质化临时量


// lambda表达式
// C++11的定义：[captures] (params) specifiers exception -> ret { body }
auto foo = [x](int y) -> int { return x * y; };

// 能被捕获的变量必须是一个自动存储期变量，但局部静态变量和全局变量可以直接在表达式体里使用

// 捕获值是将函数作用域的x和y的值复制到lambda表达式对象的内部
// 但无法改变捕获变量的值，因为捕获变量默认为常量，或者说lambda是一个常量函数（类似于常量成员函数）
// 捕获引用在变量前加&，但不是取地址，可以改变引用变量的值

// specifiers 说明符 mutable
// mutable 可以移除捕获变量的常量性
// 如果存在说明符，形参列表不能省略

// 捕获值的变量在lambda表达式定义时已经固定了，无论函数在lambda表达式定义后如何修改外部变量的值，lambda表达式捕获的值都不会变化

// [this] 捕获this指针，捕获this指针可以让我们使用this类型的成员变量和函数
// [=]    捕获lambda表达式定义作用域的全部变量的值，在C++11也捕获this，但在C++20不允许用[=]隐式捕获this，应写作[=, this]
// [&]    捕获lambda表达式定义作用域的全部变量的引用，包括this

// 捕获this指针后可以直接使用私有成员变量和方法

// 无状态的lambda表达式可以隐式转换为函数指针
void f(void (*)()) {}
void g() {
    f([] {});
}
// 因此也可以对无状态的lambda表达式解引用以匹配函数引用
void f(void (&)()) {}
void g() {
    f(*[] {});
}

// C++14
// 广义捕获
// C++11只允许简单捕获，即捕获lambda表达式定义上下文的变量，无法捕获表达式结果，无法自定义捕获变量名
// C++14允许初始化捕获
int main()
{
    int x = 5;
    // 赋值表达式通过等号跨越了两个作用域，等号=左边的变量 x 存在于lambda表达式的作用域，等号=右边的 x 存在于main函数的作用域，两个 x 不同
    auto foo = [x = x + 1] { return x; };
    // 如果在foo2的表达式体中出现x，则会出现编译错误
    auto foo2 = [r = x + 1] { return r; };

    // 可以在初始化捕获中使用移动操作
    std::string x = "hello c++ ";
    auto foo = [x = std::move(x)] { return x + "world"; };
}

// 在异步调用时复制this对象，防止lambda表达式被调用时因原始this对象被析构造成未定义的行为
class Work
{
private:
    int value;
public:
    Work() : value(42) {}
    std::future<int> spawn() {
        // 错误
        return std::async(
            [=]() -> int { return value; }
        );
        // tmp复制了this对象，在C++17中不需要tmp，见下文
        return std::async(
            [=, tmp = *this]() -> int { return tmp.value; }
        );
    }
};
std::future<int> foo() {
    Work tmp;
    return tmp.spawn();
}
int main()
{
    std::future<int> f = foo();
    f.wait();
    std::cout << "f.get() = " << f.get() << std::endl;
}

// C++14
// 泛型lambda
int main()
{
    // 泛型lambda在形参列表使用auto占位符，不需要template关键字
    auto foo = [](auto a) { return a; };
    int three = foo(3);
    const char* hello = foo("hello");
}

// C++17
// 常量lambda，见constexpr

// C++17
// *this捕获
// 在捕获中直接添加[*this]，可以生成一个*this对象的副本并存储在lambda表达式，可以像this捕获一样直接访问这个副本
// 与[this]的区别是这是一个对象的副本
class Work
{
private:
    int value;
public:
    Work() : value(42) {}
    std::future<int> spawn() {
        // 不需要初始化tmp，可以直接访问value
        return std::async(
            [=, *this]() -> int { return value; }
        );
    }
};

// C++20
// [=, this]捕获
// 为了区分[=, this]和[=, *this]，标准要求应当用[=, this]代替[=]
[=, this] {};
[=, *this] {};
// 不允许同时使用两种语法捕获this
[this, *this]{};    // 编译错误

// C++20
// 模板泛型lambda
// C++14的泛型lambda使用auto占位符，但这带来两个问题
// 普通的函数模板可以通过形参模式匹配一个实参为vector的容器对象，但泛型lambda表达式的auto不具备这种表达能力
// 同样，auto语法也使得难以获取vector所存储对象的类型

// 模板泛型lambda的语法为   []<typename T>(T t) {}
// 因此上述例子可以写成
int main()
{
    auto f = []<typename T>(std::vector<T> vector) {};      // 匹配vector
    auto f = []<typename T>(T const &x) {
        T copy = x;
        // 获取容器存储对象的类型
        using Iterator = typename T::iterator;
    };
}

// C++20
// 可构造和可赋值的无状态lambda表达式
// 在C++20之前，无状态的lambda表达式可以隐式转换为函数指针，但是其默认构造函数和复制赋值运算符函数是被删除的
// 下面的代码在C++20才能运行
auto greater = [](auto x, auto y) { return x > y; };
std::map<std::string, int, decltype(greater)> mymap;    // 允许获取greater的类型用于构造
std::map<std::string, int, decltype(greater)> mymap1, mymap2;
mymap1 = mymap2;    // 允许赋值


// C++11
// 非静态数据成员默认初始化
// 不能使用()初始化成员变量，这将被认为是一个方法，从而存在歧义
struct A{
    static int x;
    int a(x);
};
struct B{
    typedef int x;
    int a(x);
};
// 另一种情况是下面这样的构造
std::vector<int> x(3);   // 3个元素的vector
std::vector<int> x{3};   // 1个元素3的vector
// 因此必须使用适当的符号
std::vector<int> x = std::vector<int>(3); // 而非 vector<int> x(3);
std::vector<int> x{3};               // 1个元素3的vector

// 另外，也不能使用auto初始化非静态成员变量
struct A{
    auto i = 0;     // 编译错误
};
// 原因是如果支持，就可能出现用auto成员变量初始化static成员变量的情况

// C++20
// 位域默认初始化
struct S {
    int y : 8 = 11;
    int z : 4 {7};
};

// C++11
// 列表初始化
// std::initializer_list是一个支持begin、end以及size成员函数的类模板
// 实现一个以std::initializer_list为参数的构造函数，即可使用列表初始化
struct A {
	A(std::initializer_list<std::string> str) {
		for (auto& i : str) {
			std::cout << i;
		}
	}
};
// 如果有一个类同时拥有满足列表初始化的构造函数，且其中一个是以std::initializer_list为参数，那么编译器将优先以std::initializer_ list为参数构造函数
std::vector<int> x1(5, 5);
std::vector<int> x2{5, 5};

// C++20
// 指定初始化
struct Point {
    int x;
    int y;
};
Point p{ .x = 4, .y = 2 };

// 指定初始化的对象必须是一个聚合类型，下面的struct不能使用指定初始化
struct Point3D {
    Point3D() {}
    int x;
    int y;
    int z;
};
// 指定的数据成员必须是非静态成员，且每个非静态成员最多只能初始化一次
Point p{.y = 4, .y = 2};    // 错误

// 非静态成员的初始化必须按照声明的顺序进行
// 这一点和C语言中指定初始化的要求不同，在C语言中，乱序的指定初始化是合法的
Point p{.y = 4, .x = 2};    // C++编译失败，C可以编译

// 针对联合体中的数据成员只能初始化一次，不能同时指定
union u {
    int a;
    const char *b;
};
u f = {.a = 1};              // 编译成功
u g = {.b = "asdf"};         // 编译成功
u h = {.a = 1, .b = "asdf"}; // 编译失败，同时指定初始化联合体中的多个数据成员

// 不能嵌套指定初始化数据成员，这一点也与C语言不同，C语言允许嵌套指定初始化
struct Line {
    Point a;
    Point b;
};
Line l{.a.y = 5};   // 编译失败，.a.y = 5访问了嵌套成员
Line l{.a{.y = 5}}; // 但是可以这么做

// 不能指定初始化数组，而这在C语言也是允许的，在C++中禁止的原因是与lambda表达式冲突
int arr[3] = {[1] = 5}; // 编译失败



// 类数据成员  来自cppreference

// 非静态数据成员
// 不允许使用 extern 和 register 存储类说明符

// C++11
// 不允许使用 thread_local 存储类说明符，但静态数据成员可以使用
// 当存在至少一个用户定义的构造函数时，非静态数据成员不能拥有与类名相同的名字
// 占位符类型说明符 auto、decltype(auto)(C++14)、待推导的类模板名(C++17)、受约束的占位符(C++20) 不能用于声明非静态数据成员，尽管允许将其用于类定义内初始化的静态数据成员

// TODO: 布局，POD，聚合类型，平凡类型等




// C++11
// 默认和删除函数
// 类的特殊函数：构造函数，析构函数，两个复制函数和两个移动函数

// 声明任何构造函数后，将不会添加默认构造函数
// 一旦用自定义构造函数代替默认构造函数，类就将转变为非平凡类型

// 过去，通过将复制函数设为私有且不给出实现的方式禁用复制，但这并不完美
// 用同样的方法禁用类的重载函数的某个版本时，会出现问题
class Base {
    void foo(long &);
public:
    void foo(int) {}
};
int main() {
    Base b;
    long l = 5;
    b.foo(8);
    b.foo(l);   // 编译错误，匹配了私有函数
}
// 在派生类中使用using引入基类foo函数时，using说明符无法将基类的私有成员函数引入派生类，编译失败
class Derived : public Base {
public:
    using Base::foo;    // 错误
    void foo(const char *) {}
};

// 显式默认 =default 可以添加到类外部，这使得可以不修改头文件而改变函数的行为，但显式删除 =delete 必须在类内部定义

// 将构造函数替换为显式默认的，类就恢复成平凡类型
class NonTrivial {
    int i;
public:
    NonTrivial(int n) : i(n), j(n) {}
    NonTrivial() {}
    int j;
};
class Trivial {
    int i;
public:
    Trivial(int n) : i(n), j(n) {}
    Trivial() = default;
    int j;
};

// 使用显式删除 =delete 禁用复制
class NonCopyable {
public:
    NonCopyable() = default;    // 由于显式删除了复制构造函数，编译器将不再自动添加默认构造函数
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};
// 解决禁止重载函数的继承问题
class Base {
public:
    void foo(long &) = delete;  // 需要改为public，否则仍会编译错误
    void foo(int) {}
};
class Derived : public Base {
public:
    using Base::foo;
    void foo(const char *) {}
};

// 显式删除对普通函数也有效
void foo() = delete;
static void bar() = delete;
// 可以用于 new 运算符和析构函数
struct type {
    void *operator new(std::size_t) = delete;   // 阻止类在堆上动态创建对象
    ~type() = delete;                           // 阻止类通过自动变量、静态变量或者全局变量的方式创建对象，可以用new创建对象，但不能delete
};

// 不要同时使用 explicit 和 =delete
struct type {
    type(long long) {}
    explicit type(long) = delete;
};
void foo(type) {}
int main() {
    foo(type(58));  // 显式调用type(long)，但被删除，编译失败
    foo(58);        // 隐式调用type(long long)
}

// C++11
// 非受限联合类型 union
// union 的成员可以是除了引用类型外的所有类型
// 如果 union 中存在非平凡类型，那么这个 union 的特殊成员函数将被隐式删除，必须自己提供 union 的构造函数和析构函数
union U {
    U() {}  // 存在非平凡类型std::string和std::vector<int>的成员，必须提供构造函数
    ~U() {} // 必须提供析构函数
    int x1;
    float x2;
    std::string x3;
    std::vector<int> x4;
};
int main() {
    U u;
    u.x3 = "hello world";
    std::cout << u.x3;
}
// 可以通过编译，但实际上是有问题的，因为非平凡类型 x3 并没有被构造，在赋值时必然会出错
// 使用 placement new 构造，手动析构
int main() {
    U u;
    new (&u.x3) std::string("hello world");
    std::cout << u.x3 << std::endl;
    u.x3.~basic_string();

    new (&u.x4) std::vector<int>;
    u.x4.push_back(58);
    std::cout << u.x4[0] << std::endl;
    u.x4.~vector();
}

// union 的静态成员不属于 union 的任何对象，和类一样，不能在 union 的内部初始化

// TODO: 使用std::variant来代替 union


// C++11
// 委托构造函数

// 面对多个构造函数中的冗余代码，如果将这些重复代码放在一个成员函数中供构造函数调用
// 则本该是初始化成员变成了赋值，多一次赋值将影响程序效率，如果其赋值被禁用，甚至不能正常工作
// 如果使用默认参数，容易引发二义性问题，有时也避免不了二次初始化

// 委托构造函数将控制权交给代理构造函数(或称目标构造函数)
// 一个构造函数可以同时是委托构造函数和代理构造函数
// 复制构造函数也可以是委托构造函数
// 如果一个构造函数为委托构造函数，那么其初始化列表里就不能对数据成员和基类进行初始化，因为一旦一个构造函数执行完毕，就认为这个对象已经被构造了，不能再对其初始化
// 如果在代理构造函数执行完成后，委托构造函数主体抛出了异常，则自动调用该类型的析构函数

// 委托模板构造函数，即代理构造函数是一个函数模板
class X {
    template <class T>
    X(T first, T last) : l_(first, last) {}
    std::list<int> l_;
public:
    X(std::vector<short> &);
    X(std::deque<int> &);
};
X::X(std::vector<short> &v) : X(v.begin(), v.end()) {}  // 委托模板构造函数
X::X(std::deque<int> &v) : X(v.begin(), v.end()) {}

// 捕获委托构造函数和初始化列表的异常
class X {
public:
    X()
    try : X(0) {}
    catch (int e) {
        std::cout << "catch: " << e << std::endl;
        throw 3;
    }
    X(int a)
    try : X(a, 0.) {}
    catch (int e) {
        std::cout << "catch: " << e << std::endl;
        throw 2;
    }
    X(double b) : X(0, b) {}
    X(int a, double b) : a_(a), b_(b) { throw 1; }
private:
    int a_;
    double b_;
};
int main() {
    try {
        X x;
    }
    catch (int e) {
        std::cout << "catch: " << e << std::endl;   // 输出 1 2 3
    }
}

// 通常，将参数较少的构造函数委托给参数较多的构造函数，这样做的自由度更高
// 但是，如果构造函数的参数必须在函数体中使用，可以从参数较多的构造函数委托参数较少的构造函数

// C++11
// 继承构造函数
// 使用using将基类的构造函数引入派生类
class Base {
public:
    Base() : x_(0), y_(0.){};
    Base(int x, double y) : x_(x), y_(y) {}
    Base(int x) : x_(x), y_(0.) {}
    Base(double y) : x_(0), y_(y) {}
private:
    int x_;
    double y_;
};
class Derived : public Base {
public:
    using Base::Base;
};
// 继承构造函数是隐式生成的，只有在程序中使用了这些构造函数，编译器才会为派生类生成继承构造函数的代码
// 派生类不会继承默认构造函数和复制构造函数，因为执行派生类的默认构造函数之前会先执行基类的构造函数
// 同样地，在执行复制构造函数之前也会先执行基类的复制构造函数
// 继承构造函数不会影响派生类默认构造函数的隐式声明，对于继承了基类构造函数的派生类，依然会自动生成派生类的默认构造函数
// 在派生类中声明签名相同的构造函数会禁止继承相应的构造函数

// 派生类同时继承多个签名相同的构造函数会导致编译失败
class Base1 {
public:
    Base1(int) { std::cout << "Base1(int x)" << std::endl; };
};
class Base2 {
public:
    Base2(int) { std::cout << "Base2(int x)" << std::endl; };
};
class Derived : public Base1, Base2 {
public:
    using Base1::Base1;
    using Base2::Base2;     // 编译失败
};

// 继承构造函数的基类构造函数不能为私有的
class Base {
    Base(int) {}
public:
    Base(double) {}
};
class Derived : public Base {
public:
    using Base::Base;   // 当执行Derived(5)的时候会产生错误
};


// C++11
// 强枚举类型
// 枚举标识符不会隐式转换为整型
// 能指定强枚举类型的底层类型，底层类型默认为int类型
enum class E : unsigned int {
    e1 = 1,
    e2 = 2,
    e3 = 0xfffffff0
};
// C++11也允许指定枚举类型的底层类型
enum E : unsigned int {
    e1 = 1,
    e2 = 2,
    e3 = 0xfffffff0
};
// 强枚举类型是POD类型
// 强枚举类型不允许匿名，必须给定一个类型名，普通枚举类型允许匿名
enum {
    e1 = 1,
    e2 = 2,
    e3 = 0xfffffff0
};

// C++17
// 列表初始化有底层类型枚举对象
enum class Color {  // 强枚举的底层类型是int
    Red,
    Green,
    Blue
};
int main() {
    Color c{5};         // 编译成功
    Color c1 = 5;       // 编译失败
    Color c2 = {5};     // 编译失败，注意这里不属于列表初始化
    Color c3(5);        // 编译失败
    Color c4 = Color{5};// 先初始化临时对象，再复制构造初始化
}
// C++17的std::byte就是用枚举定义的

// C++20
// using打开强枚举类型
using enum Color;
using Color::Red;
Color c = Red;


// C++17
// 聚合类型
// 聚合类型的定义：
// 没有用户提供的构造函数，没有私有的、保护的非静态数据成员，没有虚函数
// 如果是派生类，还要求必须是公有继承、非虚继承
// 注意，派生类是否为聚合类型与基类是否是聚合类型没有关系，只要满足上述条件，派生类就是聚合类型

// C++20
// 聚合类型的定义中，对构造函数的要求修改为没有用户声明或提供的构造函数，相比C++17，新增了 =default 和 =delete
// 事实上，声明了 =delete 的构造函数说明该类不应该被默认构造，但是聚合类型可以避开 =delete 而构造对象
// 此外，将 =default 的构造函数设为私有的也说明该类不应该被默认构造，聚合类型也可以避开 private 的限制
// 因此，C++20在聚合类型的定义上新增了禁止用户声明的构造函数

// 标准库 <type_traits> 的 is_aggregate 可以判断目标类型是否为聚合类型
class myString : public std::string {};
int main() {
    std::is_aggregate_v<std::string>;   // 0
    std::is_aggregate_v<myString>;      // 1
}

// 聚合类型的初始化
// 之前，要初始化 myString 对象，需要为 myString 类提供一个构造函数，并在初始化列表中调用基类构造函数
// C++17开始，只需要按继承顺序列表初始化基类，最后按顺序初始化数据成员
struct Count {
    int count;
};
struct myStringWithIndex : public std::string, public Count {
    float size;
    int index;
};
int main() {
    myStringWithIndex s{"Hello World!", 11, 2.5, 0};
}

// 可以理解为聚合类型中，基类只是派生类中排列在前面的成员，构造时按顺序构造这些成员
class Base {
protected:
	Base(){}
};
class Derived : public Base {
public:
	// Derived(){};
};
int main() {
    // 无法通过编译，按顺序构造Base时发现构造函数是protect的，不可访问
    // 提供一个 Derived 的默认构造函数，Derived 不再是聚合类型，将调用这个默认构造函数，并在其中调用 Base 的构造函数
    // 在C++17中，如果提供的构造函数是 Derived() = default，仍不能通过编译
    // 因为在C++17中，聚合类型允许用户声明 =deafult 的构造函数，C++20不再允许
    // 即使在C++17中，为构造函数添加explicit说明符，或者在类外而不是类定义中为构造函数添加 =default 说明符，都会将类转变为非聚合类型
    Derived d{};
}

// 上述标准的更改会导致旧的代码存在问题，下面是一个禁止复制构造的方法
struct X
{
    std::string s;
    std::vector<int> v;
    X() = default;
    X(const X &) = delete;
    X(X &&) = default;
};
// 在C++17中，X是聚合类型，但更新到C++20以后，原来聚合类型初始化的代码将引发错误
// 可以通过加入或者继承一个不可复制构造的类型来实现类型的不可复制
struct X
{
    std::string s;
    std::vector<int> v;
    [[no_unique_address]] NonCopyable nc;
};
// 或者
struct X : NonCopyable
{
    std::string s;
    std::vector<int> v;
};
// [[no_unique_address]] 是C++20提供的特性，指定了该属性的类，其数据成员的地址有可能和该类的其他非静态数据成员的地址相同

// C++20
// 圆括号列表初始化聚合类型对象
struct X
{
    int i;
    float f;
};
X x(11, 7.0f);  // C++17不允许，无法匹配到构造函数
// 圆括号列表初始化支持缩窄转换

// C++11
// override 和 final 说明符
// 编译器将指出下列4个函数重写失败
class Base
{
public:
    virtual void some_func() {}
    virtual void foo(int x) {}
    virtual void bar() const {}
    void baz() {}
};

class Derived : public Base
{
public:
    virtual void sone_func() override {}
    virtual void foo(int &x) override {}
    virtual void bar() override {}
    virtual void baz() override {}
};

// final 告诉编译器该虚函数不能被派生类重写
struct Base {
    virtual void foo(int x) {}
};
struct Derived : public Base {
    void foo(int x) final{};
};
struct Derived2 : public Derived {
    void foo(int x){};
};
// final 可以修饰最底层基类的虚函数，但 override 不可以
// override和final可以同时出现，通常是因为中间派生类继承基类，且希望后续的派生类不能修改本类虚函数的行为

// final说明符不仅能声明虚函数，还可以声明类
struct Base final {
  virtual void foo(int x) {}
};

// C++11
// range base for
// std::for_each(begin, end, func)

// 基于范围的for循环由一个范围声明和一个范围表达式组成
// 其中，范围声明是一个变量的声明，其类型是范围表达式中元素的类型或者元素类型的引用
// 而范围表达式可以是数组或对象，对象必须满足以下条件
// 对象类型定义了begin和end成员函数  或  定义了以对象类型为参数的begin和end普通函数

// C++17
// begin和end函数不必返回相同类型
// C++11要求range base for的begin和end函数的返回类型必须相同
{
  auto &&__range = range_expression;
  for (auto __begin = begin_expr, __end = end_expr; __begin != __end; ++__begin)
  {
        range_declaration = *__begin;
        loop_statement
  }
}
// 但实际上只需要确保 __begin != __end 能返回一个有效的布尔值即可，故C++17取消了限制
{
  auto &&__range = range_expression;
  auto __begin = begin_expr;
  auto __end = end_expr;
  for (; __begin != __end; ++__begin)
  {
        range_declaration = *__begin;
        loop_statement
  }
}
// 注意，在上述伪代码中，范围表达式被一个右值引用所引用
// 如果范围表达式是一个纯右值，则右值引用将会扩展其生命周期
// 但如果范围表达式是一个泛左值，由于右值引用无法扩展泛左值的生命周期，可能访问无效的对象
class T {
  std::vector<int> data_;
public:
  std::vector<int>& items() { return data_; }
  // …
};
T foo() {
    T t;
    return t;
}
for (auto& x : foo().items()) {}    // 未定义行为
// 因此需要将数据复制出来
T thing = foo();
for (auto &x : thing.items()) {}

// C++20
// range base for的初始化语句
for (T thing = foo(); auto &x : thing.items()) {}

// 实现支持range base for的类
// begin和end函数需要返回一组类似迭代器的对象，并且这组对象必须支持operator*、operator!=和前缀operator++运算符
struct IntIter{
    IntIter(int* p) : p_(p) {}
    bool operator!=(const IntIter &other) {
        return (p_ != other.p_);
    }
    const IntIter& operator++() {
        p_++;
        return *this;
    }
    int operator*() const {
        return *p_;
    }
private:
    int* p_;
};
template <unsigned int fix_size>
struct FixIntVector
{
    FixIntVector(std::initializer_list<int> init_list) {
        int* cur = data_;
        for (auto e : init_list)
        {
            *cur = e;
            cur++;
        }
    }
    IntIter begin(){
        return IntIter(data_);
    }
    IntIter end(){
        return IntIter(data_ + fix_size);
    }
private:
    int data_[fix_size]{0};
};
int main()
{
    FixIntVector<10> fix_int_vector{1, 3, 5, 7, 9};
    for (auto e : fix_int_vector)
    {
        std::cout << e << std::endl;
    }
}

// C++17
// 支持初始化语句的if和switch
// if初始化语句中声明的变量拥有和整个if结构一样长的生命周期，包括else和else if部分
// 但else if部分的初始化语句中声明的变量的生命周期只存在于else if部分和后续的部分，无法在之前的if部分使用

// 可以利用该特性对整个if结构加锁
int main()
{
    std::mutex mx;
    bool shared_flag = true;
    if (std::lock_guard<std::mutex> lock(mx); shared_flag){
        shared_flag = false;
    }
    // 不必在初始化语句中初始化判断条件的变量，如下判断条件为fgets函数的返回值
    std::string str;
    if (char buf[10]{0}; std::fgets(buf, 10, stdin)){
        str += buf;
    }
}

using namespace std::chrono_literals;
std::condition_variable cv;
std::mutex cv_m;
int main()
{
    switch (std::unique_lock<std::mutex> lk(cv_m); cv.wait_for(lk, 100ms))
    {
    case std::cv_status::timeout:
        break;
    case std::cv_status::no_timeout:
        break;
    }
}

// 动态断言assert，标头文件cassert，通常在release版本中忽略
// 断言可以用于对函数参数的判断，但不应该用于对用户输入的判断
// 一个断言处理一个判别式，便于定位问题

// C++11
// 静态断言static_assert，可以在命名空间、类或代码块内使用
// static_assert需要传入两个实参：常量表达式和诊断消息字符串
#include <type_traits>
class A{};
class B : public A{};
class C{};
template <class T>
class E{
    static_assert(std::is_base_of<A, T>::value, "T is not base of A");
};
int main(int argc, char *argv[]){
    static_assert(argc > 0, "argc > 0"); // 使用错误，argc>0不是常量表达式
    E<C> x;                              // 使用正确，但由于A不是C的基类，所以触发断言
    static_assert(sizeof(int) >= 4, "sizeof(int) >= 4"); // 使用正确，表达式返回真，不会触发失败断言
    E<B> y; // 使用正确，A是B的基类，不会触发失败断言
}

// C++17
// 单参数static_assert，诊断消息就是常量表达式自身
static_assert(sizeof(int) >= 4);



// 在C++11，可以通过元组让函数返回多个值
std::tuple<int, int> return_multiple_values(){
    return std::make_tuple(11, 7);
}
int main() {
    int x = 0, y = 0;
    std::tie(x, y) = return_multiple_values();
}
// 但是需要写出函数返回类型，且需要提前声明变量x和y
// 在C++14，可以使用auto的新特性解决函数返回类型的问题

// C++17
// 结构化绑定
// 结构化绑定是指将一个或者多个名称绑定到初始化对象中的一个或者多个子对象（或者元素）上
// 相当于给初始化对象的子对象（或者元素）起了别名，注意，别名不同于引用
int main() {
    auto [x, y] = return_multiple_values();
}
// auto [x, y]是一个典型的结构化绑定声明，其中，auto是类型占位符，[x, y]是绑定标识符列表
// x和y是用于绑定的名称，绑定的目标是函数返回结果副本的子对象或者元素

// 结构化绑定的目标不必是一个函数的返回结果，实际上等号的右边可以是任意一个合理的表达式
struct BindTest {
    int a = 42;
    std::string b = "hello structured binding";
};
int main() {
    BindTest bt;
    auto [x, y] = bt;
    // 结构化绑定用于 range base for
    std::vector<BindTest> btv{{11, "hello"}, {7, "c++"}, {42, "world"}};
    for (const auto& [x, y] : btv) {
        std::cout << "x=" << x << " y=" << y << std::endl;
    }
}

// 在结构化绑定中，编译器会根据限定符生成一个等号右边对象的匿名副本，而绑定的对象正是这个副本而非原对象本身
// 另外，这里的别名是单纯的别名而非引用，别名的类型和绑定目标对象的子对象类型相同
BindTest bt;
const auto [x, y] = bt;
// 编译器大概会将上述代码转换成
BindTest bt;
const auto _anonymous = bt;
aliasname x = _anonymous.a
aliasname y = _anonymous.b
// 其中，用了一个不存在的关键字 aliasname 来表示 x 和 y 是别名而非引用
// 此外，auto的限定符将直接应用到匿名对象上

// 如果要绑定原来的对象
auto& [x, y] = bt;
// 编译器将产生类似下面的代码
BindTest bt;
auto& _anonymous = bt;
aliasname x = _anonymous.a
aliasname y = _anonymous.b
// 然而，如果采用 const auto& 将 bt.a 绑定到 x 上，不能通过 x 修改 bt.a
// 因为 x 绑定的对象是一个常量引用，即使 bt 本身不存在常量问题

// 使用结构化绑定无法忽略对象的子对象或元素
auto t = std::make_tuple(42, "hello world");
auto [x] = t;   // 错误

// 可以使用 std::tie 和 std::ignore 来绑定
auto t = std::make_tuple(42, "hello world");
int x = 0, y = 0;
std::tie(x, std::ignore) = t;
std::tie(y, std::ignore) = t;

// 然而，这对结构化绑定无效，因为结构化绑定的别名无法在同一个作用域中重复使用
auto t = std::make_tuple(42, "hello world");
auto [x, ignore] = t;
auto [ignore, y] = t;   // 错误，ignore重复声明


// 结构化绑定可以作用于原生数组、结构体和类对象、元组和类元组对象三种类型
// 绑定到数组，要求别名数量与数组元素个数一致
int a[3]{1, 3, 5};
auto [x, y, z] = a;
// 注意，绑定时要小心数组的退化

// 绑定到结构体和类对象
// 非静态数据成员个数必须和标识符列表中的别名个数相同
// 在C++20之前，这些数据成员必须是public
// 这些数据成员必须在同一个类或基类中
// 绑定的类和结构体中不能存在匿名联合
struct BindBase3{
    int a = 42;
};
struct BindTest3 : public BindBase3{
    double b = 11.7;
};
BindTest3 bt3;
auto [x3, y3] = bt3;    // 错误，数据成员不在同一个类中

// 绑定到元组和类元组对象
// 对于一个元组或类元组类型T，需要满足以下条件：
// std::tuple_size<T>::value是一个符合语法的表达式，且该表达式获得的整数值与标识符列表中的别名个数相同
// std::tuple_element<i, T>::type是一个符合语法的表达式，其中i是小于std::tuple_size<T>::value的整数，表达式代表类型T中第i个元素的类型
// 类型T必须存在合法的成员函数模板get<i>()或函数模板get<i>(t)，其中i是小于std::tuple_size<T>::value的整数，t是类型T的实例，get<i>()和get<i>(t)返回的是实例t中第i个元素的值

// 根据上述要求，std::pair和std::array也能称为结构化绑定的目标
std::map<int, std::string> id2str{{1, "hello"}, {3, "Structured"}, {5, "bindings"}};
for (const auto &elem : id2str){
    std::cout << "id=" << elem.first
                << ", str=" << elem.second << std::endl;
}
// 上面的elem可以简化如下
for (const auto &[id, str] : id2str) {
    std::cout << "id=" << id 
                << ", str=" << str << std::endl;
}

// 实现类元组类型
struct BindBase3{
    int a = 42;
};
struct BindTest3 : public BindBase3{
    double b = 11.7;
};
namespace std{
    template <>
    struct tuple_size<BindTest3>{
        static constexpr size_t value = 2;
    };

    template <>
    struct tuple_element<0, BindTest3>{
        using type = int;
    };

    template <>
    struct tuple_element<1, BindTest3>{
        using type = double;
    };
}
// 函数模板get<i>(t)
template <std::size_t Idx>
auto& get(BindTest3 &bt) = delete;

// 模板特化
template <>
auto& get<0>(BindTest3 &bt) { return bt.a; }
template <>
auto& get<1>(BindTest3 &bt) { return bt.b; }

int main() {
    BindTest3 bt3;
    auto& [x3, y3] = bt3;
    x3 = 78;
    std::cout << bt3.a << std::endl;
}
// 上述代码为 BindTest3 实现了3种特性以满足类元组的限制条件，从而可以进行结构化绑定

// 成员函数的 get<i>()
struct BindTest3 : public BindBase3 {
    double b = 11.7;
    template <std::size_t Idx>
    auto& get() = delete;
};
template <>
auto& BindTest3::get<0>() { return a; }
template <>
auto& BindTest3::get<1>() { return b; }

// C++20
// 结构化绑定不要求数据成员是public
struct A{
    friend void foo();
private:
    int i;
};

void foo()
{
    A a{};
    auto x = a.i;   // 编译成功
    auto [y] = a;   // 编译失败
}
// 在友元函数 foo() 中，结构化绑定和非结构化绑定的表现不一致
// 同样的问题还出现在访问自身成员时，因此C++20取消了对数据成员访问权限的限制



// C++11
// noexcept说明符
// noexcept可以接受一个返回bool的常量表达式，当表达式为false时表示函数有可能会抛出异常
template <class T>
T copy(const T &o) noexcept(std::is_fundamental<T>::value){}
// 基本类型不会抛出异常，复杂类型可能抛出异常

// noexcept运算符
// noexcept运算符接受表达式参数，并返回true或false
// 该常量表达式不会被执行，返回的结果取决于编译器是否在表达式中找到潜在的异常
int foo() noexcept{
    return 42;
}
int foo1(){
    return 42;
}
int foo2() throw(){
    return 42;
}
int main(){
    std::cout << std::boolalpha;
    std::cout << "noexcept(foo())  = " << noexcept(foo()) << std::endl;
    std::cout << "noexcept(foo1()) = " << noexcept(foo1()) << std::endl;
    std::cout << "noexcept(foo2()) = " << noexcept(foo2()) << std::endl;
}
// 结果如下
noexcept(foo())  = true
noexcept(foo1()) = false
noexcept(foo2()) = true
// 因此可以准确判断函数是否声明不会抛出异常
template <class T>
T copy(const T &o) noexcept(noexcept(T(o))) {}
// 第二个noexcept是运算符，判断T(o)是否有可能抛出异常
// 第一个noexcept是说明符，接受第二个运算符的返回值，以此决定T类型的copy函数是否声明为不抛出异常

// 解决移动构造的异常问题
template<class T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b))))
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}
// 上面的代码检查T的移动构造和移动赋值是否都不会抛出异常，并移动a和b

template <class T>
void swap(T &a, T &b) noexcept(noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b))))
{
    // 断言T的移动构造和移动赋值都不会抛出异常，如果会抛出异常则编译失败
    static_assert(noexcept(T(std::move(a)))
                      && noexcept(a.operator=(std::move(b))));
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}
// 下面是最终版本的移动函数，在移动构造和移动赋值可能抛出异常时调用复制构造和复制赋值
template <typename T>
void swap_impl(T &a, T &b, std::integral_constant<bool, true>) noexcept
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}
template <typename T>
void swap_impl(T &a, T &b, std::integral_constant<bool, false>){
    T tmp(a);
    a = b;
    b = tmp;
}
template <typename T>
void swap(T &a, T &b) noexcept(
    noexcept(
        swap_impl(a, b, std::integral_constant<bool, noexcept(T(std::move(a))) 
        && noexcept(a.operator=(std::move(b)))>())
    )
){
    swap_impl(a, b, std::integral_constant<bool, noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b)))> ());
}
// 实际上，noexcept运算符中的常量表达式可以用下面的模板函数代替
std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_ assignable<T>::value;

// 默认带有noexcept声明的函数
// 编译器生成的默认构造函数，默认复制构造，默认复制赋值，默认移动构造，默认移动赋值
// 前提是对应的函数在基类和成员中也具有noexcept声明
// 析构函数和delete运算符具有noexcept声明，即使是自定义实现的，除非该类型或其基类和成员明确使用了noexcept(false)声明析构函数和delete运算符

// C++17
// 异常规范作为类型系统的一部分
void(*fp)() noexcept = nullptr;
void foo() {}
// 在C++17之前，std::is_same <decltype(fp), decltype(&foo)>::value的结果是true
// 这种宽松的规则使得一个会抛出异常的函数通过一个保证不抛出异常的函数指针进行调用，结果该函数确实抛出了异常
// C++17将异常规范引入类型系统，noexcept声明的函数指针无法接受没有noexcept声明的函数，反之则允许
// 虚函数的重写也遵守这个规则，基类声明了noexcept派生类的虚函数也必须声明noexcept，否则虚函数的行为表现得不一致
// 当基类没有声明noexcept时，派生类也可以声明noexcept

// C++11
// 类型别名
// using identifier = typeName;
// 别名模板
// template<template-parameter-list>
// using identifier = typeName;
template<class T>
using int_map = std::map<int, T>;
int_map<std::string> int2string;
int2string[11] = "7";

// 尽管上述代码用typedef也可以做到
template<class T>
struct int_map {
    typedef std::map<int, T> type;
};
int_map<std::string>::type int2string;
int2string[11] = "7";

// 如果遇上待决的类型，还需要在变量声明前加上typename关键字
template<class T>
struct int_map{
    typedef std::map<int, T> type;
};
template<class T>
struct X{
    typename int_map<T>::type int2other; // 必须带有typename关键字
};
// 上述代码中，类模板 X 没有确定模板形参 T 的类型，因此int_map<T>::type是一个未决类型，既有可能是一个类型，也有可能是一个静态成员变量


// C++11
// 空指针常量 nullptr
// nullptr是一个std::nullptr_t类型的纯右值
// nullptr仍然可以和0比较，0和NULL仍然可以用于初始化指针变量
assert(nullptr == 0);   // true
// 然而，nullptr不能隐式转换为整型，只能隐式转换为指针类型
int n1 = nullptr;   // 错误

// std::nullptr_t通过decltype(nullptr)定义，且该类型的长度与void*相同
static_assert(sizeof(std::nullptr_t) == sizeof(void *));
// 可以用该类型创建自己的nullptr
std::nullptr_t myNull;  // myNull是左值，因此可以对myNull取地址 &myNull

// 使用nullptr可以为函数模板或者类设计一些空指针类型的特化版本，在C++11之前是无法实现的，因为NULL的推导类型是int



// C++20
// 三向比较(spaceship)运算符 <=>
// <=>的结果只能与0和自身类型比较
bool b = 7 <=> 11 < 100; // 错误
// 因此，<=>的返回类型不是普通类型，而是以下三种类型之一
// std::strong_ordering
// std::weak_ordering
// std::partial_ordering
// 这三个类只实现了参数类型为自身类型和nullptr的比较运算符函数

// std::strong_ordering
// std::strong_ordering 有三种比较结果，分别是std::strong_ ordering::less，std::strong_ordering::equal和std::strong_ ordering::greater
// std::strong_ordering 的比较结果强调可替换性，即如果 lhs <=> rhs 结果为std::strong_ordering::equal，则lhs和rhs在任何情况下都可以互换
// int采用std::strong_ordering作为三向比较返回类型
// 对于有复杂结构的类型，std::strong_ordering要求其数据成员和基类的三向比较结果都为std::strong_ordering

// std::weak_ordering
// 与std::strong_ordering一样，有三种比较结果，注意相等时的返回类型是std::weak_ordering::equivalent，与std::strong_ordering不同
// std::weak_ordering表达的是不可替换性，即如果 lhs <=> rhs 结果为std::weak_ordering::equivalent，则lhs和rhs不可以替换
// 这种情况在基本类型中没有，可能出现在自定义的类中
// 例如，一个大小写不敏感的字符串类，在比较两个字符串时认为它们等价，但未必相等
// 当std::weak_ordering和std::strong_ ordering同时出现在基类和数据成员的类型中时，该类型的三向比较结果是std::weak_ordering
// 这种情况下，如果显式声明默认三向比较运算符函数为
std::strong_ordering operator<=>(const D&) const = default;
// 编译器将报错

// std::partial_ordering
// std::partial_ordering 有四种比较结果，分别是std::partial_ ordering::less，std::partial_ordering::equivalent，std::partial_ ordering::greater，std::partial_ordering::unordered
// 其中，第四种结果表示进行比较的两个操作数没有关系
// 浮点数float和double采用std::partial_ordering作为三向比较返回类型
// 这是因为浮点数中存在一个NaN，该值与其它浮点数值无关
std::print("{0}", ((0.0 / 0.0 <=> 1.0) == std::partial_ordering::unordered));   // 输出true
// 当std::weak_ordering和std::　partial_ordering同时出现在基类和数据成员的类型中时，该类型的三向比较结果是std::partial_ordering

// 模板函数std::common_comparison_category
// 用于在一个类型合集中判断出最终三向比较的结果类型
// 当类型合集中存在不支持三向比较的类型时，该函数返回void

// 对基本类型的支持
// 对两个算术类型的操作数进行一般算术转换，然后进行比较
7 <=> 11.1  // 结果为std::partial_ordering

// 对于无作用域枚举类型和整型操作数，枚举类型会转换为整型再进行比较，无作用域枚举类型无法与浮点类型比较
enum color {
    red
};
auto r = red <=> 11;   // 编译成功
auto r = red <=> 11.1; // 编译失败

// 对两个相同枚举类型的操作数比较结果，如果枚举类型不同，则无法编译

// 如果一个操作数为bool类型，另一个操作数必须也是bool类型，否则无法编译，比较结果为std::strong_ordering

// 不支持数组进行比较
int arr1[5];
int arr2[5];
auto r = arr1 <=> arr2; // 编译失败
// 如果一个操作数是指针，且另一个操作数是同类型的指针或可以转换为同类型的指针，如数组到指针、派生类指针到基类指针的转换，则最终比较结果为std::strong_ordering
char *ptr = arr2;
auto r = ptr <=> arr1;  // 编译成功

// C++20
// 自动生成的比较运算符函数
// 如果一个自定义类型声明了三向比较运算符，则编译器自动生成< <= > >=这四种运算符函数
// 只需再声明==运算符函数，编译器会自动生成!=运算符函数



// C++11
// 线程局部存储
// thread_local说明符
// thread_local用来声明线程生命周期的对象，可以与static或extern结合，指明链接性，static不影响对象的生命周期

// 可以对线程局部存储变量取地址，并且在线程生命周期内自由使用变量，但这很危险
// 取得的地址是在运行时被计算出来的，因此无法和constexpr结合

// 在同一个线程中，一个线程局部存储对象只会被初始化一次，即使在某个函数内被多次调用
// 对应地也只会被销毁一次，通常发生在线程退出的时刻
// 显然，一个线程的两个不同函数中的线程局部存储对象是无关的，即使它们具有相同的对象名



// C++17
// inline static 成员变量，编译器在该类的定义首次出现时定义和初始化内联静态成员变量


// C++11
// constexpr常量表达式
// const定义的常量可能是一个运行时常量，例如函数调用，即使该函数返回的是一个常量

// constexpr要求该值必须在编译期计算
int x1 = 42;
constexpr int x2 = x1;  // 编译失败，x2无法用x1初始化
char buffer[x2] = {0};
// 常量值必须用常量初始化，而 x1 并不是一个常量

// constexpr函数
// 要求该函数的返回值在编译期就计算出来
// 该函数必须返回值，即返回类型不能是void
// 函数体必须只有一条语句，return expr，其中expr也是一个常量表达式
// 如果函数有形参，则将形参替换到expr中后，expr仍然必须是一个常量表达式
// 函数在使用之前必须定义

// constexpr函数支持递归，可以使用递归来完成循环的操作
// constexpr函数调用时，如果实参不是一个常量，将退化为普通函数

// constexpr构造函数
// constexpr说明符不能用于声明自定义类型
class X{
    int x1;
public:
    X() : x1(5) {}
    int get() const{
        return x1;
    }
};
constexpr X x;              // 编译失败，X不是字面类型
char buffer[x.get()] = {0}; // 编译失败，x.get()无法在编译阶段计算

// 解决方法是用constexpr声明自定义类型的构造函数
// constexpr构造函数的初始化列表中必须是常量表达式，且函数体必须为空
class X{
    int x1;
public:
    constexpr X() : x1(5) {}
    constexpr X(int i) : x1(i) {}
    constexpr int get() const{
        return x1;
    }
};
constexpr X x;
char buffer[x.get()] = {0};
// 上面的代码只是给构造函数和get函数添加了constexpr说明符就可以通过编译
// 因为它们本身都符合常量表达式构造函数和常量表达式函数的要求，这样的类被称为字面量类类型（literal class type）
// 在C++11中，constexpr会自动给函数带上const属性，因此get()函数的const不必写出，这一点在C++14中不同
// constexpr构造函数与constexpr函数一样，具有退化特性

// 注意，constexpr声明自定义类型变量，必须确保该类型的析构函数是平凡的
// 平凡析构函数必须满足下面的条件
// 自定义类型中不能有用户自定义的析构函数，析构函数不能是虚函数，且基类和成员的析构函数必须都是平凡的

// constexpr对浮点的支持
// 在constexpr之前，常使用enum hack实现在编译期计算常量表达式的值，但仅限于整型
// constexpr支持声明浮点类型的常量表达式值，且保证其精度至少和运行时的精度相同


// C++14
// constexpr函数的增强
// 除static变量和thread_local变量外，constexpr函数体允许声明变量，但必须初始化
// 函数体允许if语句，switch语句和全部的循环语句，但不允许goto语句
// 函数可以修改生命周期和常量表达式相同的对象，例如，通过++x修改了形参x的值
// 函数的返回类型可以是void
// constexpr声明的成员函数不再具有const属性
// 上述增强同样适用于constexpr构造函数



// C++17
// constexpr lambda表达式
// lambda表达式在条件允许的情况下都会隐式声明为constexpr

// C++17
// constexpr的内联属性
// constexpr声明静态成员变量时，该变量自动内联

// C++17
// if constexpr
// if constexpr的条件必须是编译期常量表达式，条件结果一旦确定，编译器将只编译符合条件的代码块

// 该特性只有在使用模板时才有实际意义
if constexpr (i > 0)    // 编译失败，不是常量表达式
{
    std::cout << "i > 0" << std::endl;
}
// 将只编译选择的代码块
template <class T>
bool is_same_value(T a, T b){
    if constexpr(std::is_same<T, double>::value) 
        if (std::abs(a - b) < 0.0001)
            return true;
        else
            return false;
    else
        return a == b;
}
// 然而，需要小心函数有多个不同的返回类型导致的编译失败
template <class T>
auto minus(T a, T b){
    if constexpr (std::is_same<T, double>::value)
        if (std::abs(a - b) < 0.0001)
            return 0.;
        else
            return a - b;
    // 当T为double时，函数有不同的返回类型，编译失败
    return static_cast<int>(a - b);
}

// if constexpr不支持短路规则，应使用嵌套的if constexpr


// C++20
// constexpr虚函数
// 在C++20之前，虚函数不允许声明为constexpr
// constexpr的虚函数在继承重写上并没有特殊要求，constexpr的虚函数可以覆盖重写普通虚函数，普通虚函数也可以覆盖重写constexpr的虚函数

// C++20
// constexpr函数中出现try-catch
constexpr int f(int x){
    try { return x + 1; }
    catch (...) { return 0; }
}
// 然而，throw语句仍然被禁止，因此该try块是不能抛出异常的，也意味着catch永远不会执行
// 实际上，当函数被评估为常量表达式时，try-catch没有任何作用

// C++20
// 允许在constexpr中进行平凡的默认初始化
struct X{
    bool val;
};
constexpr void f(){
    X x;    // 编译错误，未初始化
}
f();
// C++20允许在constexpr中进行平凡的默认初始化，进一步减少了constexpr函数相比普通函数的特殊性

// C++20
// 允许在constexpr中更改联合类型的有效成员
union Foo{
    int i;
    float f;
};
constexpr int use(){
    Foo foo{};
    foo.i = 3;
    foo.f = 1.2f;   // C++20之前编译失败
    return 1;
}

// C++20
// 允许dynamic_cast和typeid出现在常量表达式中
// 允许在constexpr函数使用未经评估的内联汇编

// C++20
// consteval
// 立即函数像是更严格的constexpr，禁止退化到普通函数
consteval int sqr(int n){
    return n * n;
}
constexpr int r = sqr(100); // 编译成功，常量表达式上下文环境
int x = 100;
int r2 = sqr(x);    // 编译失败，x是变量
// 如果一个立即函数在另一个立即函数中被调用，则函数定义时的上下文环境不必是一个常量表达式
consteval int sqrsqr(int n){
    return sqr(sqr(n));     // 能否编译成功取决于调用sqrsqr时的上下文环境是否是常量表达式
}

// lambda表达式也可以使用consteval说明符

// C++20
// constinit
// constinit说明符用于具有静态存储持续时间的变量声明上，它要求变量具有常量初始化程序
constinit int x = 11; // 编译成功，全局变量具有静态存储持续
int main()
{
    constinit static int y = 42; // 编译成功，静态变量具有静态存储持续
    constinit int z = 7;         // 编译失败，局部变量是动态分配的
}
const char* f() { return "hello"; }
constexpr const char *g() { return "cpp"; }
constinit const char *str1 = f(); // 编译错误，f()不是一个常量初始化程序
constinit const char *str2 = g(); // 编译成功

// constinit还能用于非初始化声明，以告知编译器thread_local变量已被初始化
extern thread_local constinit int x;
int f() { return x; }

// 注意，constinit初始化的对象本身并不要求是常量

// C++20
// std::is_constant_evaluated()，该函数在<type_traits>头文件中定义
// 该函数用于判断当前表达式是否是一个常量求值环境，如果是一个明显常量求值的表达式则返回true，否则返回false
constexpr double power(double b, int x){
    if (std::is_constant_evaluated() && x >= 0){
        double r = 1.0, p = b;
        unsigned u = (unsigned)x;
        while (u != 0){
            if (u & 1)
                r *= p;
            u /= 2;
            p *= p;
        }
        return r;
    }
    else
        return std::pow(b, (double)x);
}
int main(){
    constexpr double kilo = power(10.0, 3); // 常量求值
    int n = 3;
    double mucho = power(10.0, n); // 非常量求值
    return 0;
}

// 明显常量求值分为以下几类
// 常量表达式，包括数组长度，case表达式，非类型模板实参，static_assert中的表达式等
// if constexpr语句中的条件
// constexpr变量的初始化程序
// 立即函数调用
// 约束概念表达式
// 可在常量表达式中使用或者具有常量初始化的变量初始化程序



// C++17
// 确定的表达式求值顺序

// 函数表达式一定在函数参数之前求值
foo(a, b, c);   // foo一定在a、b、c之前求值，但参数a、b、c的求值顺序仍然没有确定

// 对于后缀表达式和移位运算符，总是从左往右求值
E1[E2];
E1.E2;
E1.*E2;
E1->*E2;
E1 << E2;
E1 >> E2;

// 赋值表达式从右往左求值

// new表达式的内存分配优先于T构造函数中参数E的求值
new T(E)


// C++11
// 十六进制浮点表示
// 通过std::hexfloat和std::defaultfloat修改浮点输入和输出的默认格式化
0x1.7800p+2
// 十六进制仅用于有效数，底数是2，指数仍然是十进制

// C++17
// 十六进制浮点字面量
// 尽管C++11支持了输入输出时的十六进制浮点表示，直到C++17才能用十六进制浮点字面量来表示一个浮点数
double float_array[] { 0x1.7800p+2 };
// IEEE-754标准最小的单精度值即0x1.0p−126

// C++14
// 二进制整数字面量
0b11001101

// C++14
// 单引号整数分隔符
123'456

// C++11
// 原生字符串字面量
prefix R"delimiter(raw_characters)delimiter"
// prefix 和 delimiter 可选
R"HTML(raw_characters)HTML"
// prefix 用于定义不同类型的原生字符串字面量，wchar_t char16_t char32_t的 prefix 分别是 L u U
UR"cpp(raw_characters)cpp"  // char32_t

// C++20
// char8_t 的 prefix 是 u8
u8R"cpp(raw_characters)cpp"

// 原生字符串字面量也可以连接普通字符串字面量

// C++11
// 自定义字面量
returnType operator ""identifier(params)
// 上述运算符重载定义了一个字面量后缀
lengthWithCm operator ""cm(unsigned long long)
auto length = 1cm + 2cm;
// 标准建议字面量运算符函数的标识符以下划线开始，如_cm
// 自定义字面量支持整数、浮点数、字符和字符串四种类型，它们接受的参数类型如下

// 整数字面量运算符函数的形参有3种类型 unsigned long long、const char* 和无参
// 整数字面量会先被转换为 unsigned long long 或 const char* 再作为参数传入

// 浮点数字面量运算符函数的形参有3种类型 long double、const char* 和无参

// 字符串字面量运算符函数只有一种形参类型列表(const char* str, size_t len)

// 字符字面量运算符函数只有一种形参类型char

// 字面量运算符函数的标识符可以是保留关键字