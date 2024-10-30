import java.io.Serializable;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.security.auth.callback.Callback;

public class Java {

    {
        // 序列化
        transient String java = "Java"; // 阻止序列化
        // transient 只能修饰变量，不能修饰类和方法
        // transient 修饰的变量，在反序列化后变量值将会被置成类型的默认值,例如，如果是修饰 int 类型，那么反序列后结果就是 0
        // static 变量不会被序列化，因为序列化是针对对象的，而 static 变量是属于类的

        // 一般不会使用 JDK 自带的序列化方式
        // 1 JDK序列化效率较低，生成的字节码较大
        // 2 不支持跨语言调用
        // 3 安全性问题，可以通过序列化来构造恶意对象，导致安全漏洞

        // 比较常用的序列化协议有 Hessian、Kryo、Protobuf、ProtoStuff，这些都是基于二进制的序列化协议
        // 像 JSON 和 XML 这种属于文本类序列化方式，虽然可读性比较好，但是性能较差，一般不会选择


        // JDK 自带的序列化
        // 只需实现 java.io.Serializable 接口
        @AllArgsConstructor
        @NoArgsConstructor
        @Getter
        @Builder
        @ToString
        public class RpcRequest implements Serializable {
            private static final long serialVersionUID = 1905122041950251207L;
            private String requestId;
            private String interfaceName;
            private String methodName;
            private Object[] parameters;
            private Class<?>[] paramTypes;
            private RpcMessageTypeEnum rpcMessageTypeEnum;
        }

        // 序列化号 serialVersionUID 用于版本控制
        // 反序列化时，会检查 serialVersionUID 和当前类的 serialVersionUID 是否一致。如果 serialVersionUID 不一致则会抛出 InvalidClassException 异常
        // 每个序列化类都应该手动指定其 serialVersionUID，如果不手动指定，那么编译器会动态生成默认的 serialVersionUID

        // serialVersionUID 是 static 变量，不会被序列化，但是它比较特殊，序列化时会被写入到二进制的序列化流中，反序列化时也会解析它并以此来验证序列化对象的版本一致性
        // 因此，serialVersionUID 只是用来被 JVM 识别，实际并没有被序列化

        // Kryo
        // Kryo 是一个高性能的 Java 序列化/反序列化框架，它的速度比 JDK 自带的序列化方式快很多
        // Kryo 是一个第三方序列化框架，它不需要实现 Serializable 接口，也不需要提供 serialVersionUID
        // Kyro 采用变长存储方式，序列化后的字节数比 JDK 自带的序列化方式要小很多
        // Kyro 线程不安全
        // https://javaguide.cn/java/basis/serialization.html#kryo

        // Protobuf
        // Protobuf 是 Google 开发的一种数据序列化协议，它是一种轻便高效的结构化数据存储格式
        // Protobuf 生成的序列化类是不可读的二进制文件，因此不适合用于存储数据，但是它的序列化和反序列化速度非常快，且安全

        // Hessian
        // Hessian 是一个轻量级的，自定义描述的二进制 RPC 协议
    }

    {
        // 反射
        // 反射是一个类库，可以在运行时读取和使用类的metadata
        // 在.NET中，反射在System.Reflection命名空间中

        // Class 对象
        // Class 对象是描述类结构的元数据的对象，类似于 C# 中的 Type 类型
        Type type = assembly.GetType("xxx.MySqlHelper");    // C# 的 type就是Class对象

        // 获取Class对象的4种方式
        Class<?> clazz = String.class;  // 通过类名.class
        Class<?> clazz = new String().getClass();  // 通过对象的getClass()方法
        Class<?> clazz = Class.forName("java.lang.String");  // 通过Class.forName和类的全限定名
        Class<?> clazz = ClassLoader.getSystemClassLoader().loadClass("java.lang.String");  // 通过类加载器

        // 通过类的Class对象创建类对象
        TargetObject targetObject = (TargetObject)targetClass.newInstance();

        // 获取类的所有方法
        Method[] methods = targetClass.getDeclaredMethods();
        for (Method method : methods) {
            System.out.println(method.getName());
        }

        // 与 C# 不同，传入参数时是可变参数，而不是object数组
        Method publicMethod = targetClass.getDeclaredMethod("publicMethod", String.class, Integer.class);  // 获取指定方法
        publicMethod.invoke(targetObject, "Java", 123);  // 调用方法

        // 获取类的所有属性
        Field[] fields = targetClass.getDeclaredFields();
        for (Field field : fields) {
            System.out.println(field.getName());

            // 获取属性的值
            field.setAccessible(true);  // 设置属性可访问，private方法也是这样
            Object value = field.get(targetObject);
            System.out.println(value);
        }
    }

    {
        // 代理模式
        // 使用代理对象来代替对真实对象(real object)的访问，这样就可以在不修改原目标对象的前提下，提供额外的功能操作，扩展目标对象的功能
        // 代理模式的主要作用是扩展目标对象的功能，比如说在目标对象的某个方法执行前后你可以增加一些自定义的操作
        // 代理模式是一种结构型设计模式，它通过创建一个代理对象来控制对其他对象的访问

        // 静态代理
        // 对目标对象的每个方法的增强都是手动完成的，接口一旦新增加方法，目标对象和代理对象都要进行修改，且需要对每个目标类都单独写一个代理类
        // 静态代理在编译时就将接口、实现类、代理类这些都变成了实际的 class 文件

        // 步骤
        // 定义一个接口，创建一个类实现这个接口
        public interface SmsService {
            String send(String message);
        }
        public class SmsServiceImpl implements SmsService {
            public String send(String message) {
                System.out.println("send message:" + message);
                return message;
            }
        }
        // 创建一个代理类，同样实现这个接口
        public class SmsProxy implements SmsService {

            private final SmsService smsService;
            public SmsProxy(SmsService smsService) {
                this.smsService = smsService;
            }
            @Override
            public String send(String message) {
                // 调用方法之前，添加代码
                System.out.println("before method send()");
                smsService.send(message);
                // 调用方法之后，添加代码
                System.out.println("after method send()");
                return null;
            }
        }
        // 代理类中保存一个目标类对象，创建目标类对象，然后传入该目标类对象初始化代理类对象
        // 在代理类的对应方法调用目标类中的对应方法，这样就通过代理类屏蔽了对目标对象的访问，并且可以在目标方法的执行前后添加代码
        public class Main {
            public static void main(String[] args) {
                SmsService smsService = new SmsServiceImpl();
                SmsProxy smsProxy = new SmsProxy(smsService);
                smsProxy.send("java");
            }
        }

        // 动态代理
        // 从 JVM 角度来说，动态代理是在运行时动态生成类字节码，并加载到 JVM 中的
        // Spring AOP、RPC 框架的实现都依赖了动态代理
        // 动态代理的实现方式有很多种，比如 JDK 动态代理、CGLIB 动态代理

        // JDK 动态代理
        // InvocationHandler 接口和 Proxy 类
        // Proxy.newProxyInstance(ClassLoader loader, Class<?>[] interfaces, InvocationHandler h)
        // 这个方法用来生成一个代理对象
        // interfaces 参数是目标对象的接口，因为 JDK 动态代理只能代理接口
        // handler 参数是一个 InvocationHandler 对象，有一个 invoke() ，当代理对象调用方法时，会调用该方法
        public interface InvocationHandler {
            public Object invoke(Object proxy, Method method, Object[] args)
                throws Throwable;
        }

        // 目标方法接口
        public interface SmsService {
            String send(String message);
        }

        // 目标类的目标方法
        public class SmsServiceImpl implements SmsService {
            public String send(String message) {
                System.out.println("send message:" + message);
                return message;
            }
        }

        // 代理类，实现了 InvocationHandler 接口
        public class DebugInvocationHandler implements InvocationHandler {
            // 代理类持有一个目标对象
            private final Object target;
            public DebugInvocationHandler(Object target) {
                this.target = target;
            }

            @Override
            public Object invoke(Object proxy, Method method, Object[] args) throws InvocationTargetException, IllegalAccessException {
                System.out.println("before method " + method.getName());
                Object result = method.invoke(target, args);
                System.out.println("after method " + method.getName());
                return result;
            }
        }

        // 代理工厂类，对于任何目标类，通过 DebugInvocationHandler 类生成一个代理对象
        public class JdkProxyFactory {
            public static Object getProxy(Object target) {
                return Proxy.newProxyInstance(
                        target.getClass().getClassLoader(), // 目标类的类加载器
                        target.getClass().getInterfaces(),  // 代理需要实现的接口，可指定多个
                        new DebugInvocationHandler(target)  // 代理对象对应的自定义 InvocationHandler
                );
            }
        }

        SmsService smsService = (SmsService) JdkProxyFactory.getProxy(new SmsServiceImpl());
        smsService.send("java");

        // 在动态代理中，代理类不实现目标接口，而是实现 InvocationHandler 接口，具体要代理的方法接口由 Proxy.newProxyInstance() 方法传入

        // CGLIB 动态代理
        // JDK 动态代理的问题是只能代理实现了接口的类，而不能代理类本身的方法
        // CGLIB 通过继承实现动态代理，原理是对指定的目标类生成一个子类，并覆盖其中的方法

        // CGLIB 动态代理通过 MethodInterceptor 接口和 Enhancer 类实现
        // Enhancer 类用于生成代理对象
        public interface MethodInterceptor extends Callback{
            // 拦截目标类中的方法
            public Object intercept(Object obj, java.lang.reflect.Method method, Object[] args, MethodProxy proxy) 
                throws Throwable;
        }

        public class AliSmsService {
            public String send(String message) {
                System.out.println("send message:" + message);
                return message;
            }
        }

        public class DebugMethodInterceptor implements MethodInterceptor {
            /**
             * @param o           被代理的对象（需要增强的对象）
             * @param method      被拦截的方法（需要增强的方法）
             * @param args        方法入参
             * @param methodProxy 用于调用原始方法
             */
            @Override
            public Object intercept(Object o, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
                System.out.println("before method " + method.getName());
                Object object = methodProxy.invokeSuper(o, args);
                System.out.println("after method " + method.getName());
                return object;
            }
        }

        public class CglibProxyFactory {
            public static Object getProxy(Class<?> clazz) {
                // 创建动态代理增强类
                Enhancer enhancer = new Enhancer();
                // 设置类加载器
                enhancer.setClassLoader(clazz.getClassLoader());
                // 设置被代理类
                enhancer.setSuperclass(clazz);
                // 设置方法拦截器
                enhancer.setCallback(new DebugMethodInterceptor());
                // 创建代理类
                return enhancer.create();
            }
        }

        AliSmsService aliSmsService = (AliSmsService) CglibProxyFactory.getProxy(AliSmsService.class);
        aliSmsService.send("java");

        // CGLIB 动态代理通过继承的方式实现，因此不能代理 final 类和 final 方法

        // JDK 动态代理和 CGLIB 动态代理的区别
        // JDK 动态代理是基于接口的，CGLIB 动态代理是基于类的

        // Unsafe
        // Unsafe 依赖 Native 方法

        // Unsafe是一个单例类

        // Service Provider Interface (SPI)
        // API 由实现方提供，SPI 由调用方提供

        // SPI 的实现方式
        // 1. 通过配置文件
        // 2. 通过注解
        // 3. 通过 Java 的 ServiceLoader 类
    }
}