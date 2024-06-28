using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


// 扩展方法
// 扩展方法是非泛型静态类中的静态方法，它可以像实例方法一样调用
// 扩展方法的第一个参数是this关键字修饰的参数，表示扩展的类型
public static class MyExtension{
    // 为string类型添加一个扩展方法，可以通过string.show()调用
    public static void Show(this string str){
        Console.WriteLine(str);
    }
}

// 异步多线程
void func(string s){}
Action<string> action = func;
action.Invoke("同步");
action("同步")
action.BeginInvoke("异步", null, null); // 后面两个参数类型是 AsyncCallback, object，分别表示回调和状态

// 当前线程ID
Thread.CurrentThread.NamagedThreadId.ToString();

// Thread都是前台线程，Task是基于ThreadPool的，都是后台线程

// AsyncCallback的定义
public delegate void AsyncCallback(IAsyncResult ar);    // 是一个委托

IAsyncResult asyncResult = null;
AsyncCallback asyncCallback = new AsyncCallback((ar) =>
{
    Console.WriteLine(object.ReferenceEquals(asyncResult, ar)); // true，说明是同一个引用
    Console.WriteLine(ar.AsyncState); // "这是异步状态AsyncState"
})
// BeginInvoke的返回类型也是IAsyncResult
asyncResult = action.BeginInvoke("异步带回调", asyncCallback, "这是异步状态AsyncState");
// BeginInvoke除了接受action委托的参数，还接受两个参数，一个是执行完action后要执行的回调，一个是在回调中可见的AsyncState
// 回调类型是AsyncCallback，接受一个IAsyncResult类型的参数，这个参数由BeginInvoke提供，实际上同时是BeginInvoke的返回值
// BeginInvoke异步调用action并立即返回asyncResult，当action异步执行完毕后，在asyncResult中设置AsyncState并将asyncResult传入回调
// 从而，回调中可以通过形参ar即asyncResult来访问ar.AsyncState，AsyncState实际上是执行回调时需要的数据

asyncResult.IsCompleted // 是否完成

asyncResult.AsyncWaitHandle.WaitOne()       // 信号量
asyncResult.AsyncWaitHandle.WaitOne(1000)   // 最多等1000ms

asyncResult.EndInvoke(asyncResult)          // 等待，需要传入一个IAsyncResult，可以返回值
Func<int> func = new(() => {
    return 10;
})
IAsyncResult asyncResult = func.BeginInvoke(ar => {
    // func.EndInvoke(asyncResult)  // 可以在回调中调用EndInvoke，以取得异步调用的结果
    // 每一次异步调用只能调用一次EndInvoke
    Console.WriteLine(ar.AsyncState);
}, "这是AsyncState");
Console.WriteLine($"{ func.EndInvoke(asyncResult) }");  // 10，返回了值

// Thread
// 构造方法
public Thread(ThreadStart start [, int maxStackSize]);
public Thread(ParameterizedThreadStart start [, int maxStackSize]);

// ThreadStart
public delegate void ThreadStart(); // 无参数无返回值的委托，实际上和无参的Action一样，但是不能隐式类型转换

void func() { }
ThreadStart threadStart = func;
Thread thread = new(func);
thread.Start(); // 启动线程

thread.Abort(); // 抛异常，销毁线程
Thread.ResetAbort(); // 静态方法，在catch块中取消异常

thread.Join();  // 当前线程等待thread完成
thread.Join(1000);  // 最多等1000ms

thread.ThreadState == ThreadState.Running  // 枚举
thread.IsBackground = true; // 默认是前台线程，改为后台线程
// 后台线程随进程退出，前台线程会阻止进程退出
// 除了Thread构造的线程是前台线程，下面的都是后台线程

// 带返回值的异步调用，手动实现BeginInvoke和EndInvoke
// 注意，返回类型是Func<T>，异步调用立即获得返回值，即包装了所需值的Func，需要获取值的时候调用该Func
// 实际上就是C++的promise和future
Func<T> ThreadWithReturn<T>(Func<T> func)
{
    T t = default(T);
    Thread thread = new(() =>
    {
        T = func.Invoke();  // 相当于promise在future中设置值
    });
    thread.Start();

    // 相当于返回future
    return () =>
    {
        thread.Join();
        return t;
    }
}
Func<int> func = ThreadWithReturn<int>(() =>
{
    return 10;
});
int result = func.Invoke(); // 调用这个返回的函数相当于调用future::get()


// ThreadPool
bool ThreadPool.QueueUserWorkItem(WaitCallback callback, object state); // 将方法排入队列以便执行
// WaitCallback的定义
public delegate void WaitCallback(object state);    // 接受一个参数，无返回值的委托
// BeginInvoke和EndInvoke使用的就是线程池线程

// 信号量
ManualResetEvent manualResetEvent = new(false);
manualResetEvent.WaitOne(); // ManualResetEvent默认是false，false的时候WaitOne()等待，true的时候通过
manualResetEvent.Set();     // 设置为true
manualResetEvent.Reset();   // 设置为false

// Task
// Task基于ThreadPool，以下都是Task的启动方式
Task Task.Run(Action action);   // 以异步方式执行任务，静态方法，注意返回的是一个Task
new Task(action).Start();
TaskFactory taskFactory = Task.Factory; // 工厂也是静态变量
taskFactory.StartNew(action);   // StartNew有一个重载，第二个参数是AsyncState

// 等待所有任务完成
List<Task> taskList = new();
taskList.Add(Task.Run(action));
taskList.Add(Task.Run(action));
taskList.Add(Task.Run(action));
Task.WaitAll(taskList.ToArray());
// 等待任一任务完成
Task.WaitAny(taskList.ToArray());

// WhenAll，WhenAny，当任务完成时
Task<Task> Task.WhenAll(params Task[] tasks);   // 返回值的类型是Task，可以继续调用Task的方法
// 任务完成后，继续异步调用
Task.WhenAll(taskList.ToArray()).ContinueWith(task => {
    Console.WriteLine("完成了");
});
// ContinueWith()
Task Task.ContinueWith(Action<Task> continuationAction);

taskFactory.ContinueWhenAll(taskList.ToArray(), tasks => {  // 注意第二个参数类型是Action<Task[]>，表示将第一个参数传进去
    Console.WriteLine("全部完成了");
});
taskFactory.ContinueWhenAny(taskList.ToArray(), task => {  // 注意第二个参数类型是Action<Task>，将完成的那个任务传进去
    Console.WriteLine("一个完成了");
});

Task.Delay(1000);   // 延迟1000ms
Task.Delay(1000).ContinueWith(() => {   // 1000ms后执行Action，不阻塞当前线程，显然Delay也返回Task
    Console.WriteLine("延迟了1000ms");
})

// Parallel
// 在Task的基础上封装
void Parallel.Invoke(params Action[] actions);
Parallel.Invoke(() => {}, () => {}, () => {});  // 特点是当前线程也会参与执行

ParallelLoopResult Parallel.For(int fromInclusive, int toExclusivem, Action<int> body);
Parallel.For(0, 5, i => Console.WriteLine($"{i}")); // 从0到5执行5次

ParallelLoopResult Parallel.ForEach<TSource>(IEnumerable<TSource> source, Action<TSource> body);
Parallel.ForEach(new string[] {"0", "1" , "2" }, i => Console.WriteLine("字符串" + i)); // 传入的i是string

ParallelOptions parallelOptions = new();
parallelOptions.MaxDegreeOfParallelism = 2; // 最大并发是2
Parallel.For(0, 5, parallelOptions, i => Console.WriteLine($"{i}"));

// 取消线程执行 [不推荐]
ParallelLoopResult Parallel.For(int fromInclusive, int toExclusivem, Action<int, ParallelLoopState> body);
Parallel.For(0, 5, parallelOptions, (i, parallelLoopState) => {
    if (i == 2){
        parallelLoopState.Break();  // 结束当前线程，如果当前是主线程就结束Parallel
        return;
    }
    if (i == 3){
        parallelLoopState.Stop();   // 结束Parallel
        return;
    }
    Console.WriteLine($"{i}");
});

// 异步异常捕获，注意子线程throw的时候父线程是否脱离了catch块，可以使用WaitAll等待子线程执行
// 实际上，线程里不应该往外抛出异常，异常应该在线程内部处理

// 线程取消
// 线程应该自己决定是否继续执行，通过信号量，公共变量
// CancellationTokenSource
public class CancellationTokenSource : IDisposable{ // 线程取消信号量
    // ...
    public bool IsCancellationRequested{ get; };    // 检查信号量
    public CancellationToken Token { get; }
    public void Cancel();
}
Task TaskFactory.StartNew(Action action, CancellationTokenSource cancellationToken); // 第二个参数来自CancellationTokenSource.Token
// 如果传入cancellationToken，当Token对应的cancellationTokenSource被Cancel了，尚未启动的Task不会启动，且会抛出一个异常

// 锁
private static readonly object xxxLock = new object();  // 类的不同实例是同一个锁
lock (xxxLock) {
    // 锁上的区域
}   // 出来自动解锁
// 等价于
Monitor.Enter(xxxLock);
// 锁上的区域
Monitor.Exit(xxxLock);
// lock和Monitor都只能锁引用变量，因此需要注意多个锁指向同一个字符串字面量
string lock1 = "lock";
string lock2 = "lock";
lock(lock1){}
lock(lock2){}   // 实际上是同一个锁

lock(this){}    // 类的不同实例是不同的锁

// async await
// await作用于一个已经启动的Task
async void NoReturn(){
    TaskFactory taskFactory = new();
    Task task = taskFactory.StartNew(() => {
        // 子线程干活
    });
    await task; // 父线程返回
    // 其余代码
}
// 父线程在遇到await时立即返回，当子线程完成Task中的工作后，另起一个线程继续执行await后面的代码
// 实际上，每个await后面的所有内容，都被包装到一个委托中，交由Task执行，线程来自ThreadPool
// 上面的代码和下面等价
async void NoReturn(){
    TaskFactory taskFactory = new();
    Task task = taskFactory.StartNew(() => {
        // 子线程干活
    });
    task.ContinueWith((t => {
        // 其余代码
    }));
}

// 下面的代码返回类型是Task，但是没有return语句
async Task NoReturnTask(){
    TaskFactory taskFactory = new();
    Task task = taskFactory.StartNew(() => {
        // 子线程干活
    });
    await task; // 父线程返回
    // 其余代码
}
// 由于返回类型仍然是Task，可以继续通过await等待
await NoReturn();       // 错误，返回类型是void，不能等待
await NoReturnTask();   // 可以等待
NoReturnTask().Wait();  // 父线程等待子线程完成

// async方法可以返回值，将返回类型包装在Task<T>中
async Task<int> NoReturnTask(){
    int result = 10;
    TaskFactory taskFactory = new();
    Task task = taskFactory.StartNew(() => {
        // 子线程干活
    });
    await task; // 父线程返回
    // 其余代码
    return result;  // 自动包装成Task<int>
}
Task<int> t = NoReturnTask();
int result = t.Result; // 等待返回值
t.Wait();  // 跟上面等价

// 可以不使用async和await
Task NoReturnTask(){
    TaskFactory taskFactory = new();
    Task task = taskFactory.StartNew(() => {
        // 子线程干活
    });
    task.ContinueWith((t => {
        // 其余代码
    }));
    return task;
}

// 编译器看到await时，将await前面的代码切成一块，将await所等待的Task异步执行的代码切成一块，将await后面的代码切成一块，并使用状态机执行上面三块代码

// ConfigureAwait
ConfiguredTaskAwaitable Task.ConfigureAwait(bool continueOnCapturedContext);    // await后面的内容由什么线程执行
// 如果传入true，则使用主线程执行，UI线程默认传入true，传入false则从线程池中获取线程执行

// ValueTask
// 返回一个值或者包装值的Task
// 有时候可以不进行异步调用，直接返回值，有时候可能需要异步返回Task的场景
async ValueTask<object> DownloadData(string website){
    if(cache.Contains(website))
        return cache.Get(website);  // 返回一个值
    var response = await httpClient.GetAsync(website);
    cache.Set(website, response);
    return response;    // 返回一个Task
}




// 反射
// 加载程序集 获取类型 创建对象 类型转换 使用对象
Assembly assembly = Assembly.Load("xxx");           // 程序集名称，不带扩展名，从当前目录加载
Assembly assembly = Assembly.LoadFile("xxx.dll");   // 意义不大，加载的dll可能还依赖其它项，可以从其它目录加载
Assembly assembly = Assembly.LoadFrom("xxx.dll");

Type type = assembly.GetType("xxx.MySqlHelper");
object oHelper = Activator.CreateInstance(type);
object iDBHelper = (IDBHelper)oDBHelper;    // 编译器不知道oDBHelper是IDBHelper类型，需要强制类型转换
iDBHelper.Query();

// 使用configuration加载配置到字符串，实现不修改代码，通过修改配置文件使得程序构造的对象不同
// 配置文件App.config中

// <configuration>
//     <appSettings>
//         <add key="IDBHelperConfig" value="MySqlHelper"/>
//     </appSettings>
// </configuration>

// 类中
string IDBHelperConfig = ConfigurationManager.AppSettings["IDBHelperConfig"];
// 程序的扩展
// 当新引入其它的程序集时，只要该程序集实现了IDBHelper接口，不需要编译就可以支持
// 因为反射不是通过引用加载dll，而是通过Assembly.Load()从路径下加载程序集

// 通过反射可以调用私有构造函数，破坏单例
// 调用带参数的构造函数
Type type = assembly.GetType("xxx.MySqlHelper");
object oHelper = Activator.CreateInstance(type, new object[]{123, "123"});  // 第一个参数是int，第二个参数是string

// 泛型类的反射
Assembly assembly = Assembly.LoadFrom("xxx.dll");
Type type = assembly.GetType("xxx.GenericClass`3");     // `3表示接受3个泛型参数，不加`3会找不到类型 class GenericClass<T, X, W>
object oGeneric = Activator.CreateInstance(type);       // 错误，不传入泛型参数会抛出异常
Type newType = type.MakeGenericType(new Type[]{typeof(int),typeof(string),typeof(bool)});  // 创建一个泛型的Type
object oGeneric = Activator.CreateInstance(newType);

// 不类型转换而调用方法：获取方法
Type type = assembly.GetType("xxx.MySqlHelper");  
object oHelper = Activator.CreateInstance(type);
MethodInfo method = type.GetMethod("Show");     // 通过方法名获取方法
method.Invoke(oHelper, null);       // 第一个参数是对象，第二个参数是object[]，即方法的参数列表
                                    // 如果是静态方法，第一个参数可以是null
// 获取私有方法
MethodInfo privateMethod = type.GetMethod("Show", BingdingFlags.Instance|BindingFlags.NonPublic);
// 获取重载方法，需要传入参数列表
MethodInfo OverloadMethod = type.GetMethod("Show", new Type[]{typeof(int)});

// 获取泛型类的泛型方法
Type type = assembly.GetType("xxx.GenericClass`1");     // 假定type是一个泛型类class GenericClass<T>
Type newType = type.MakeGenericType(new Type[]{typeof(int)});   // newType是一个泛型类class GenericClass<int>
object oGeneric = Activator.CreateInstance(newType);
MethodInfo method = newType.GetMethod("Show");     // 假定Show(T t, X x, W w)是泛型类方法，但方法不需要写占位符`2
MethodInfo genericMethod = method.MakeGenericMethod(new Type[]{typeof(int), typeof(string)});   // 注意，类本身的泛型参数已经确定，所以不需要传入
genericMethod.Invoke(oGeneric, new object[]{111, 123, "123"});  // 第一个参数是泛型类的泛型参数T t，后面两个是方法的泛型参数X x, W w

// 将硬编码转为字符串的意义：MVC+URL —— 类名称+方法名称


// 属性和字段的反射
Type type = typeof(MySqlHelper);
type.Name;  // MySqlHelper
object oHelper = Activator.CreateInstance(type);    // 创建对象
PropertyInfo property = type.GetProperty("Name");   // 获取Name属性
foreach(PropertyInfo property in type.GetProperties()){  // 获取所有属性
    Console.WriteLine(property.Name);
    Console.WriteLine(property.GetValue(oHelper));   // 获取对象的属性值
    property.SetValue(oHelper, "123");  // 设置对象的属性值
}

// 意义
// 例如，将数据库对象转换为视图对象，可以通过反射将数据库对象的属性值赋给视图对象的属性值，它们的属性名称相同，因此不需要显式知道它们的属性名即可操作

// 获取Type的方式
Type type = typeof(MySqlHelper);    // 通过typeof和类名获取Type
Type type = oHelper.GetType();      // 通过对象获取Type
Type type = assembly.GetType("xxx.MySqlHelper");    // 通过程序集和类的全限定名获取Type



// 特性 attribute
// 使用特性，可以有效地将元数据或声明性信息与代码（程序集、类型、方法、属性等）相关联
// 将特性与程序实体相关联后，可以在运行时使用反射这项技术查询特性

// 特性向程序添加元数据
// 为一个元素添加特性，编译器会在中间语言IL中向该元素添加元数据

// 特性是一个继承了Attribute的类，可以通过反射获取特性的信息
// 特性类一般以Attribute结尾，使用的时候可以省略Attribute，如果不是Attribute结尾，使用的时候必须写全名
class MyAttribute : Attribute{
    public string Name { get; set; }
    public MyAttribute(string name){
        Name = name;
    }
    public MyAttribute(){
    }
}

[My]
[My()]  // 等价于调用无参构造的[My]，但一个特性只能出现一次，除非为特性类添加特性[AttributeUsage(AllowMultiple = true)]
[My(Name = "123")]  // 在声明特性时，不仅可以向构造函数传参，还可以直接指定属性和字段的值
class MyClass{
    // 可以为函数、参数、返回值添加特性
    [return: My]
    public string MyMethod([My]string name){
        return name;
    }
    // 特性[AttributeUsage] 的其它参数 
    // AttributeTargets.All  // 可以应用到任何元素
    // AttributeTargets.Class  // 可以应用到类
    // AttributeTargets.Constructor  // 可以应用到构造函数
    // Inherited = true  // 特性可以被继承，默认true
}

// 为类添加特性后，在类中主动调用特性，特性才会生效
// 通过反射获取特性
Type type = typeof(MyClass);
object[] attributes = type.GetCustomAttributes(true);  // 通过GetCustomAttributes()扩展方法获取所有特性，第一个参数表示是否获取继承的特性
// 特性的构造函数被编译器注入到了类的元数据中，这一步可以理解为调用类中特性的构造函数，实际上是通过反射获取特性的构造函数
foreach(object attribute in attributes){
    if(attribute is MyAttribute){
        MyAttribute myAttribute = (MyAttribute)attribute;
        Console.WriteLine(myAttribute.Name);    // 获取特性的属性值，特性就像扩展了一个类
    }
}

// 获取特性时判断特性是否存在
if(type.IsDefined(typeof(MyAttribute), true)){
    MyAttribute myAttribute = (MyAttribute)type.GetCustomAttribute(typeof(MyAttribute), true);
    Console.WriteLine(myAttribute.Name);
}

// 获取属性的特性
PropertyInfo property = type.GetProperty("Name");
object[] attributes = property.GetCustomAttributes(true);
foreach(object attribute in attributes){
    if(attribute is MyAttribute){
        MyAttribute myAttribute = (MyAttribute)attribute;
        Console.WriteLine(myAttribute.Name);
    }
}

// 获取方法的特性
MethodInfo method = type.GetMethod("MyMethod");
object[] attributes = method.GetCustomAttributes(true);
foreach(object attribute in attributes){
    if(attribute is MyAttribute){
        MyAttribute myAttribute = (MyAttribute)attribute;
        Console.WriteLine(myAttribute.Name);
    }
}

// 获取参数的特性
ParameterInfo parameter = method.GetParameters()[0];
object[] attributes = parameter.GetCustomAttributes(true);
foreach(object attribute in attributes){
    if(attribute is MyAttribute){
        MyAttribute myAttribute = (MyAttribute)attribute;
        Console.WriteLine(myAttribute.Name);
    }
}

// 获取返回值的特性
parameterInfo parameter = method.ReturnParameter;
object[] attributes = parameter.GetCustomAttributes(true);
foreach(object attribute in attributes){
    if(attribute is MyAttribute){
        MyAttribute myAttribute = (MyAttribute)attribute;
        Console.WriteLine(myAttribute.Name);
    }
}


// 特性的意义
// 没有破坏类的封装性，为类添加功能，可以通过特性来实现AOP Aspect Oriented Programming面向切面编程，例如日志、事务、权限等

// 数据库值、属性名称、描述的映射
public class RemarkAttribute : Attribute{
    public string Remark { get; set; }
    public RemarkAttribute(string remark){
        Remark = remark;
    }
}

public enum UserStatus{
    [Remark("正常")]
    Normal = 0,
    [Remark("禁用")]
    Disable = 1,
    // [Remark("删除")]
    Delete = 2
}

public static class RemarkExtension{
    public static string GetRemark(this Enum value){
        // 取枚举值的类型，这里是UserStatus
        Type type = value.GetType();
        // 取枚举值的字段
        FieldInfo field = type.GetField(value.ToString());
        // 判断字段是否有特性
        if(field.IsDefined(typeof(RemarkAttribute), true)){
            // 取字段对应的Remark特性，这里可以理解为调用Remark特性的构造函数
            RemarkAttribute remarkAttribute = (RemarkAttribute)field.GetCustomAttribute(typeof(RemarkAttribute), true);
            // 返回Remark特性类的Remark属性
            return remarkAttribute.Remark;
        }
        // 如果没有特性，返回枚举值的字符串，即 Delete
        return value.ToString();
    }
}
UserStatus.Normal.GetRemark();   // "正常"
UserStatus userStatus = UserStatus.Normal;
Console.WriteLine(userStatus.GetRemark());  // 实现了枚举值（即属性名称）到描述的映射

// 可以通过特性来实现ORM Object Relation Mapping对象关系映射，将数据库表映射为类，将字段映射为属性

// 特性的继承
// 特性可以继承，继承的特性可以继承基类的特性，也可以重写基类的特性
public class BaseAttribute : Attribute{
    public virtual void Show(){
        Console.WriteLine("Base");
    }
}
// 通过继承特性，利用特性的时候field.IsDefined(typeof(BaseAttribute)检查基类特性，只需要写一份代码，然后遍历每个派生的特性