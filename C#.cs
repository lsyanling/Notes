
// 反射
// 加载程序集 获取类型 创建对象 类型转换 使用对象
Assembly assembly = Assembly.Load("xxx");           // 程序集名称，不带扩展名
Assembly assembly = Assembly.LoadFile("xxx.dll");   // 意义不大，加载的dll可能还依赖其它项
Assembly assembly = Assembly.LoadFrom("xxx.dll");

Type type = assembly.GetType("xxx.MySqlHelper");
object oHelper = Activator.CreateInstance(type);
object iDBHelper = (IDBHelper)oDBHelper;
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

// 泛型的反射
Assembly assembly = Assembly.LoadFrom("xxx.dll");
Type type = assembly.GetType("xxx.GenericClass`3");     // `3表示接受3个泛型参数，不加`3会找不到类型
object oGeneric = Activator.CreateInstance(type);       // 不传入泛型参数会抛出异常
Type newType = type.MakeGenericType(new Type[]{typeof(int),typeof(string),typeof(bool)});  // 返回一个Type
object oGeneric = Activator.CreateInstance(newType);

// 不类型转换而调用方法：获取方法
Type type = assembly.GetType("xxx.MySqlHelper");  
object oHelper = Activator.CreateInstance(type);
MethodInfo method = type.GetMethod("Show");
method.Invoke(oHelper, null);       // 第一个参数是对象，第二个参数是object[]，即方法的参数列表
                                    // 如果是静态方法，第一个参数可以是null
// 获取私有方法
MethodInfo privateMethod = type.GetMethod("Show", BingdingFlags.Instance|BindingFlags.NonPublic);
// 获取重载方法
MethodInfo OverloadMethod = type.GetMethod("Show", new Type[]{typeof(int)});
// 获取泛型类的泛型方法
Type newType = type.MakeGenericType(new Type[]{typeof(int),typeof(string),typeof(bool)});
object oGeneric = Activator.CreateInstance(newType);
MethodInfo method = type.GetMethod("Show");     // Show(T t, X x)是泛型方法，但不需要写`1
MethodInfo genericMethod = method.MakeGenericMethod(new Type[]{typeof(int), typeof(string)});
genericMethod.Invoke(oGeneric, new object[]{123, "123"});

// 将硬编码转为字符串的意义：MVC+URL —— 类名称+方法名称






