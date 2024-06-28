import java.io.Serializable;
import java.lang.reflect.Method;

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
    }
}