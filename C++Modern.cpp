#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <future>
#include <type_traits>

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
