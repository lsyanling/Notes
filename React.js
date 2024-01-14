// 2023.12
// index.js是入口
// react核心包
import React from 'react';
import ReactDOM from 'react-dom/client';

// 导入根组件
import App from './App'
// 把App根组件渲染到id为root的dom节点
const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
	<App />
);

// JSX列表渲染;
let songs = [
	{ id: 1, name: "111" },
	{ id: 2, name: "222" },
];

const list = (
	<ul>
		{songs.map((item) => (
			<li>{item.name}</li>
		))}
	</ul>
);
// map将songs的每一项映射为一个 <li> 元素
// 为每个 <li> 元素添加一个唯一的key属性，这是react框架使用的，例如在渲染优化上
list = (
	<ul>
		{songs.map((item) => (
			<li key={item.id}>{item.name}</li>
		))}
	</ul>
);

// 条件渲染
// && 和 ? : 两种方式

// 函数组件
// 组件名必须以大写字母开头, 函数必须有返回值, 返回null表示不渲染任何内容;
// 组件名即标签名;
function Hello() {
	return <div>2333</div>;
}
ReactDOM.render(<Hello></Hello>, document.getElementById("root"));

// useState
// 是一个react hook函数，用于向组件添加一个状态
// 返回一个数组，数组的第一个元素是状态的值，第二个元素是set函数，用于修改状态
// useState()的参数是状态的初始值
const [count, setCount] = useState(0);
setCount(count + 1);
// 如果状态是复杂对象，需要保证每次传入一个全新的对象，不能改变原对象的键值，即要改变引用地址
const [obj, setObj] = useState({ name: "pink", age: 17 });
// 可以先解包原状态对象，再更新某个属性
setObj({ ...obj, age: 18 });

// 删除、过滤项
// filter()方法返回一个新数组，不会改变原数组
// splice()方法会改变原数组
// 通过filter()方法过滤出不需要删除的项
commentList = [
	{ id: 1, name: "pink", comment: "pink is good"}, 
	{ id: 2, name: "blue", comment: "blue is good" }
]

function Comments() {
	const [comments, setComments] = useState(commentList);
	const handleDelete = (id) => {
		const newComments = comments.filter((item) => item.id !== id);
		setComments(newComments);
	}
	return (
		comments.map((item) => (
			<button onClick={() => handleDelete(item.id)}>删除</button>
		))
	)
}

// 样式控制
// 行内样式，内花括号表示一个对象，采用驼峰写法
<div style={{ color: "red", fontSize: '10px' }}></div>;

// 类名
// 首先将css文件import进来
import "./index.css";
.foo{
	color: red;
}
// react采用className属性，而不是class属性
<div className="foo"></div>;

// 动态类名
// 模板字符串
<div className={`pink ${isShow ? "show" : ""}`}></div>;

// 需求：点击tab项，做高亮处理
// 点击谁就把谁的id记录下来，然后和遍历时的每一项的id匹配，谁匹配到就设置负责高亮的className
tabList = [{ id: 1, text: "最新" }, { id: 2, text: "最热" }];
function Tabs(){
	const [tabs, setTabs] = useState(tabList);
	const [nowTabId, setNowTab] = useState(1);
	const handleTabChange = (id) => {
		setNowTab(id);
	};
	
	return (
		tabs.map((item) => (
			<div
				key={item.id}
				// 这里是一个模板字符串拼接，如果item.id等于nowTabId，就添加active类名，下面用classnames库优化
				className={`nav-item ${item.id == nowTabId && 'active'}`}
				onClick={() => handleTabChange(item.id)}>
					{item.text}
			</div>
		))
	)
}

// 排序，这里用lodash库
import _ from "lodash";
setTabs(_.orderBy(tabs, "id", "desc"));
setTabs(_.orderBy(tabs, "id", "asc"));

// classnames库
// 通过条件动态控制className
import classNames from "classnames";
// key表示要控制的类名，value表示条件，如果value为true，就添加key类名
<div className={classNames('nav-item', { active: item.id == nowTabId })} />

// 受控组件绑定
// 通过useState控制表单元素的值
function Input() {
	const [value, setValue] = useState("");
	return (
		<input
			type="text"
			value={value}
			onChange={(e) => setValue(e.target.value)}
		/>
	);
}

// React获取DOM
// 通过hook函数 useRef() 获取DOM元素，并绑定到DOM元素的ref属性上
// useRef()的参数是初始值，返回一个对象，当DOM可用时，对象的current属性指向DOM对象
function GetDom(){
	const domRef = useRef(null);
	const handleClick = () => {
		console.dir(domRef.current);
	}
	return (
		<div>
			<div ref={domRef}></div>
			<button onClick={handleClick}>获取DOM</button>
		</div>
	)
}

// uuid库
// 生成唯一id
import { v4 as uuidv4 } from "uuid";
uuidv4();	// 生成一个随机id

// dayjs库
// 日期格式化
import dayjs from "dayjs";
dayjs(new Date()).format("YYYY-MM-DD HH:mm:ss");

// 元素聚焦
// 通过useRef()获取DOM元素，然后调用DOM的focus()方法
function Focus() {
	const inputRef = useRef(null);
	const handleClick = () => {
		inputRef.current.focus();
	};
	return (
		<div>
			<input type="text" ref={inputRef} />
			<button onClick={handleClick}>聚焦</button>
		</div>
	);
}

// 类组件，react 18不再推荐
类组件名必须以大写字母开头, 组件必须继承自React.Component;
类组件必须提供render(); // render()方法必须有返回值
class Hello extends React.Component {
	render() {
		return <div>2333</div>;
	}
}

// 绑定事件
// 合成事件，即事件处理中的事件对象e;
class Hello extends React.Component {
	render() {
		return <button onClick={handleClick2}>123</button>;
	}
	handleClick1() {
		console.log("点击了");
	}
	handleClick2(e) {
		e.preventDefault(); // 阻止浏览器的默认行为 如超链接跳转
	}
}

// 带参数的事件
// 事件处理函数的调用打包成箭头函数作为事件处理参数
class Hello extends React.Component {
	render() {
		return <button onClick={(e) => handleClick(e, "123")}>123</button>;
	}
	handleClick(e, str) {
		console.log(str);
		e.preventDefault();
	}
}

// 数据传递
// 父传子，子组件接受props参数，父组件通过props向子组件传递数据
// props是一个只读对象，里面包含了父组件传递过来的数据
function Son(props){
	return <div>{props.name}</div>
}
function Father(){
	const name = "pink";
	return <Son name={name} />
}
// props的children属性，表示组件标签的子节点，当组件标签有子节点时，props就会有该属性
// 这里，Son组件有一个子节点，因此在props中有children属性
function Father(){
	return (
		<Son>
			<div>子节点</div>
		</Son>
	)
}

// 子传父，父组件提供回调函数，子组件要传递的数据作为回调函数的参数
function Son(props){
	return <button onClick={() => props.onReceive("pink")}>按钮</button>
}

// context跨层传递数据
// 从顶层向底层传递数据，即顶层只能是发送方，底层只能是接收方
// 使用hook函数createContext()创建一个上下文对象MsgContext，该对象中有一个组件Provider
// MsgContext.Provider组件作为父结点包裹子组件，通过value属性向子组件传递数据
const MsgContext = createContext(); // 创建上下文对象
// 在接收消息的组件中调用hook函数useContext接收数据参数
function App(){
	const msg = "message";
	return (
		<MsgContext.Provider value={msg}>
			<A />
		</MsgContext.Provider>
	)
}
function A() {
	return <B />
}
function B() {
	const msg = useContext(MsgContext);
	return <div>{msg}</div>
}

// hook函数useEffect
// 用于在React组件中创建由渲染本身引起的操作，例如发送Ajax请求，更改DOM等
// useEffect函数有两个参数，第一个是副作用函数，即要执行的操作，第二个是依赖项数组，是可选的，会影响副作用函数的执行
// 如果没有依赖项数组，每次组件渲染或更新完毕后都会执行，当依赖项数组为空时，表示只在组件渲染完毕后执行一次，
useEffect(() => {}, []);
const URL = "http://localhost:3000/api/list";
function App() {
	useEffect(() => {
		// 异步请求数据，应当在组件渲染完毕后执行，因此放在useEffect()中
		async function getData() {
			const res = await fetch(URL);
			const data = await res.json();
			console.log(data);
		}
		getData();
		console.log("组件渲染完毕");
	}, []);
	return <div>2333</div>;
}
// 当依赖项数组不为空时，表示只有依赖项发生变化时才会执行副作用函数
const [count, setCount] = useState(0);
useEffect(() => {}, [count]);	// 当count变化时执行副作用函数

// 清除副作用
// 当组件卸载时，清除副作用
useEffect(() => {
	// 在副作用函数中返回一个函数，在这里清除副作用
	// 清除副作用的常见时机是在组件卸载时自动执行
	const timer = setInterval(() => { 
		console.log("定时器执行了"); 
	}, 1000);
	return () => {
		// 清除定时器
		clearInterval(timer);
	};
}, []);

// 自定义hook函数
// 自定义hook函数必须以use开头
function useCount() {
	const [count, setCount] = useState(0);
	const add = () => {
		setCount(count + 1);
	};
	// 返回要使用的对象、函数等
	return [count, add];
}

// hook函数的使用规则
// 只能在组件或自定义hook函数中调用hook函数，不能在外部或普通函数中使用
// 只能在组件的最外层使用hook函数，不能在循环、条件判断、嵌套函数中使用
// 只能在React函数组件中使用hook函数，不能在普通的js函数中使用







// state
类组件就是有状态组件, 函数组件就是无状态组件;
class Hello extends React.Component {
	constructor() {
		super();
		this.state = { count: 0 };
	}
	render() {
		return <div onClick={handleClick}>2333</div>;
	}
	handleClick(e) {
		this.setState({
			count: this.state.count + 1,
		});
	}
}

// 绑定this的三个方法;
将事件处理方法的调用打包成箭头函数作为事件处理参数;
<div onClick={() => this.handleClick()}>{this.state.count}</div>;

在组件的构造方法中绑定this;
Function.prototype.bind();
constructor();
{
	super();
	this.handleClick = this.handleClick.bind(this);
}

将事件处理方法设为箭头函数;
handleClick = () => {};

// 受控组件
HTML中的表单元素有元素维护的可变状态;
React组件的可变状态保存在state中;
React将state与表单元素的value属性绑定, 由state控制表单元素的值, 称为受控组件;
<input type="text" value={this.state.txt} />;

受控组件需要在state中添加一个充当表单元素value的状态;
需要给表单元素绑定onChange事件, 用于更新state;
state = { txt: "" };
<input
	type="text"
	value={this.state.txt}
	onChange={(e) => this.setState({ txt: e.target.value })}
/>;

// 属性名表达式
用于多元素表单;
{
	<input
		type="text"
		name="txt"
		value={this.state.value}
		onChange={this.handleChange}
	/>;
}
let value = e.target.type === "checkbox" ? target.checked : target.value;
name的值是txt, 则取state的txt属性;
this.setState({
	[name]: value,
});

// 非受控组件
在组件的构造方法中保存一个ref对象;
constructor();
{
	super();
	this.任意的名称 = React.createRef();
}
将该ref对象关联到组件的ref属性;
{
	<input type="text" ref={this.任意的名称} />;
}
在事件方法中通过current属性获取该组件的值;
{
	this.任意的名称.current.value;
}

// 数组解包和打包
comments是一个对象数组;
comments;
[{ id: xxx, name: yyy, comment: zzz }, ...comments];

// 数据传递
向组件传递数据;
给组件标签添加属性;
<Hello name="thisHello" age={19}></Hello>;

组件接收数据;
函数组件通过props参数接收数据, 类组件通过this.props接收数据;

function Hello(props) {
	return (
		<div>
			接收到数据:
			{props.name}
			{props.age}
		</div>
	);
}

props是只读的;
构造方法中需要将props传递给super(), 否则在构造方法中无法获取props;
但即使在构造方法中没有这么做, 在其他方法中仍然能获取props;

父组件传子组件;
在父组件的state中添加要传递的数据, 然后在子组件的元素标签中添加属性;

子组件传父组件;
父组件提供回调函数, 子组件要传递的数据作为回调函数的参数;

context;
调用React.createContext(), 创建Provider和Consumer两个组件;
const { Provider, Consumer } = React.createContext();
使用Provider作为父结点, 即用;
<Provider></Provider>;
包裹子组件;

在 < Provider > 组件中设置value属性, 表示要传递的数据;
<Provider value={{ 1: "pink" }}></Provider>;

在 < Consumer > 组件中调用使用回调函数接收数据参数;
<Consumer>
	{(data) => {
		<div>接收到的数据是:{data}</div>;
	}}
</Consumer>;

// props
props.children;
表示组件标签的子结点, 当组件标签有子结点时, props就会有该属性;

props校验;
在创建组件时指定props的类型;
props校验功能在包prop - types中;
添加校验, 组件名.propTypes;
Hello.propTypes = {
	color: PropTypes.array,
};
PropTypes的类型有;
array,
	bool,
	func,
	number,
	object,
	string,
	node(可以被渲染的节点类型),
	element(React元素类型);

必填项约束, 在类型后加上isRequired属性;
Hello.propTypes = {
	color: PropTypes.array.isRequired,
};

结构约束, 通过PropTypes.shape({});
Hello.propTypes = {
	object: PropTypes.shape({
		color: PropTypes.string,
		fontSize: PropTypes.number,
	}),
};

实例约束, 通过PropTypes.instanceOf(ClassName);
Hello.propTypes = {
	color: PropTypes.instanceOf(Hello),
};

取值约束, 通过PropTypes.oneOf([]);
Hello.propTypes = {
	color: PropTypes.oneOf([1, 2, 3]),
};

取类型约束, 通过PropTypes.oneOfType([]);
Hello.propTypes = {
	color: PropTypes.oneOfType([
		PropTypes.instanceOf(Hello),
		PropTypes.string,
		PropTypes.number,
	]),
};

// props默认值
props的默认值通过组件的defaultProps属性实现;
Hello.defaultProps = {
	color: "red",
};
当组件标签不传入props时, 组件使用默认的props;
即使组件标签传入了props;
组件仍会将defaultProps中未被传入props的属性设置到props中;
Hello.defaultProps = {
	color: "red",
	colour: "blue",
};
<Hello color="green"></Hello>;
上面的元素中, colour仍被设置为默认值;

// 组件的生命周期
只有类组件有生命周期;
在render(), 执行之后和component(), 执行之前, React更新DOM和refs;

挂载阶段;
constructor();
在创建组件时执行, 初始化state和绑定this;
render();
每次组件渲染时执行, 在render(), 里面不能调用setState();
componentDidMount();
在render(), 执行完之后执行, 发送网络请求和进行DOM操作;

更新阶段;
更新的三种情况, 接收到props更新, 调用setState(), 调用forceUpdate();
render();
componentDidUpdate();
组件完成DOM渲染后执行, 发送网络请求和进行DOM操作;
componentDidUpdate(),
	方法有一个参数prevProps,
	该参数是在render(),
	执行前的props,
	即上一次的props;
componentDidUpdate(prevProps);
在componentDidUpdate(), 中调用setState(), 或者发送网络请求, 应该给出条件;
componentDidUpdate = (prevProps) => {
	if (prevProps.color !== this.props.color) {
		this.setState({});
		// 发送网络请求
	}
};

卸载阶段;
当组件从页面中消失时, 调用componentWillUnmount();
执行清理工作, 如清理定时器clearInterval();

// 组件复用
组件的复用即复用组件的state和操作state的方法;

// render props模式
如何获取复用组件的state;
在创建组件标签时, 添加一个值为函数的prop, 向该函数传入state参数;
如何复用该state来渲染任意的UI;
将上述函数的返回值作为要渲染的元素;
<Mouse
	render={(mouse) => {
		<p>
			鼠标位置:
			{mouse.x},{mouse.y}
		</p>;
	}}
></Mouse>;
在组件内部, 通过调用上述函数来渲染元素;
class Mouse extends React.Component {
	render() {
		return this.props.render(this.state);
	}
}

// children写法
将上述render写法改为children写法, 使用props.children代替props.render属性;
<Mouse>
	{
		(render = (mouse) => {
			<p>
				鼠标位置:
				{mouse.x},{mouse.y}
			</p>;
		})
	}
</Mouse>;
render是Mouse组件元素的子节点, 同时是一个接收state参数, 返回要渲染的元素的函数;
从而在组件内部, 以如下方式调用render();
class Mouse extends React.Component {
	render() {
		return this.children(this.state);
	}
}

// 关于render props模式的代码优化
给props添加一个children校验;
Mouse.propTypes = {
	children: PropTypes.func.isRequired,
};
在组件卸载时解除监听事件绑定;

// 高阶组件
装饰器;
高阶组件是一个函数, 接收要包装的组件作为参数, 返回增强后的组件;
在高阶组件内部创建一个类组件,
	在这个类组件中提供可复用的状态逻辑代码,
	通过props将复用的状态和逻辑传递给被包装的组件;

高阶组件约定以with开头;
function withMouse() {}
参数以大写字母开头, 即要包装的组件;
function withMouse(WrappedComponent) {}
在函数内部定义一个类组件, 提供可复用的状态逻辑, 该类组件作为高阶组件的返回值;
function withMouse(WrappedComponent) {
	class Mouse extends React.Component {}
	return Mouse;
}
在该组件中渲染参数组件, 同时将状态通过props传递给参数组件;
function withMouse(WrappedComponent) {
	class Mouse extends React.Component {
		render() {
			// 这里把this.state解包
			return (
				<WrappedComponent
					{...this.state}
				></WrappedComponent>
			);
		}
	}
	return Mouse;
}
定义要复用可复用组件的组件;
const Hello = (props) => {
	return <Hello>props.x, props.y</Hello>;
};
对该组件做装饰;
const HelloMouse = withMouse(Hello);
创建组件实例;
<HelloMouse></HelloMouse>;

// displayName
高阶组件返回的组件所创建的实例实际上都是高阶组件中可复用组件的实例;
该可复用组件负责将其状态逻辑与各种参数组件相结合, 然后将结合体渲染;
由于高阶组件返回的是该组件,
	调用高阶组件得到的组件所创建的实例都是可复用组件的实例;
默认情况下, React使用组件名作为displayName;
需要为高阶组件的返回值设置displayName, 约定名称为With + 复用组件名 + 组件名;
function withMouse(WrappedComponent) {
	class Mouse extends React.Component {
		render() {
			// 这里把this.state解包
			return (
				<WrappedComponent
					{...this.state}
				></WrappedComponent>
			);
		}
	}
	Mouse.displayName = `WithMouse${getDisplayName(WrappedComponent)}`;
	return Mouse;
}
function getDisplayName(WrappedComponent) {
	return (
		WrappedComponent.displayName ||
		WrappedComponent.name ||
		"Component"
	);
}

// props丢失
高阶组件模式本质上使用的是高阶组件中的可复用组件(Mouse);
当使用高阶组件模式进行组件复用时, 向要渲染的组件传递props, props会丢失;
<HelloMouse prop="abc"></HelloMouse>;
原因在于props被传给了可复用组件Mouse, 但可复用组件没有将其继续传给参数组件;
在展开this.state的同时展开this.props;
function withMouse(WrappedComponent) {
	class Mouse extends React.Component {
		render() {
			// 这里把this.state和this.props都解包给参数组件
			return (
				<WrappedComponent
					{...this.state}
					{...this.props}
				></WrappedComponent>
			);
		}
	}
	Mouse.displayName = `WithMouse${getDisplayName(WrappedComponent)}`;
	return Mouse;
}

// setState()是异步的
后面的setState(), 调用不要依赖前面的setState();
handleClick = () => {
	// 相当于只调用了一次
	this.setState({
		count: this.state.count + 1,
	});
	this.setState({
		count: this.state.count + 1,
	});
};
如上的多次setState(), 调用, 只调用一次render();

// 调用setState()
this.setState((state, props) => {
	return {
		count: state.count + 1,
	};
});
文档中提到, 可以传递一个签名为;
(state, props) => newState;
的函数作为参数, 这会将一个原子性的更新操作加入更新队列;
在设置任何值之前, 此操作会查询前一刻的state和props;
这样调用setState(), 并不会立即改变this.state, 同样会创建一个待执行的变动;
此函数的第一个参数是前一刻的state,
	即prevState,
	第二个参数是state更新执行瞬间的props;

// setState()的第二个参数 回调
setState(), 的回调参数在页面重新渲染后立即执行;
this.setState(
	(state, props) => {
		return {};
	},
	() => {
		console.log("在状态更新且页面重新渲染后立即执行");
	}
);

JSX是React.createElement(), 的语法糖;
React.createElement(), 内部会产生一个React元素, 即一个js对象;

组件更新机制;
当组件被渲染时, 重新渲染它的所有子节点;

与渲染相关的数据放在state中, 无关的数据直接放在this里;

通过以下钩子函数的返回值决定是否重新渲染该组件, 返回true表示需要渲染;
shouldComponentUpdate(nextProps, nextState);
更新阶段, 在render(), 前执行;

nextState是最新的state,
	在shouldComponentUpdate(),
	中直接取this.state是旧的state;
props也一样;

// 纯组件
纯组件继承自React.PureComponent;
纯组件内部实现了shouldComponentUpdate();
纯组件内部的对比是浅层对比, 因此更新对象或数组时, 应该先解包;
this.setState((state, props) => {
	return {
		list: [...state.list, /*新数据*/ { newData: data }],
		obj: {
			...state.obj,
			newData: data,
		},
	};
});

// 虚拟DOM
虚拟DOM即React元素, 是一个js对象, 用于描述希望在页面中看到的内容;
初次渲染时, React会根据初始state生成一个虚拟DOM, 然后渲染成真实的DOM;
当通过setState(), 改变state时, 重新生成新的虚拟DOM;
React使用Diff算法比较新的虚拟DOM与上一次的虚拟DOM, 得到需要更新的内容;
当render(), 调用后, 根据state和JSX结构生成虚拟DOM;

// 路由
SPA单页应用程序, 只有一个HTML页面, 前端路由即使用单个页面管理多页面的功能;
React路由是URL路径与组件的对应关系;

导入路由;
import { BrowserRouter as Router, Route, Link } from "react-router-dom";
用Router组件包裹整个应用;
<Router>
	<div className="App"></div>
</Router>;
使用Link组件作为导航菜单即路由入口, to属性;
<Link to="/first">页面1</Link>;
使用Route组件配置路由规则和要展示的组件,
	即路由出口,
	path属性对应Link组件的to属性;
<Route path="/first" component={First}></Route>;

path属性以 / 开头,
	表示绝对路径,
	否则表示相对路径,
	此时意味着前面要加上父Router组件的path属性;

Router组件一个应用只使用一次;
HashRouter, 使用URL的hash值实现, 在地址栏有个; //#
Link组件最终被编译为一个a标签,
	其to属性值被编译为href属性值,
	同时表示地址栏中的pathname(location.pathname);

点击Link组件, 组件修改地址栏的URL;
React路由监听到地址栏URL的变化,
	遍历Route组件,
	使用路由规则与pathname进行匹配,
	一旦匹配则停止遍历;

this.props.history;
history是React路由提供的, 用于获取浏览器历史记录的相关信息;
this.props.history.push("/first");
push(path), 方法, 跳转到某个页面;

this.props.history.go(-1);
go(number), 方法表示根据浏览器历史记录前进和后退页面 - 1, 表示上一个页面;

// 默认路由
Route组件的path属性为 / 则为默认路由;

模糊匹配不是正则表达式意义的前缀匹配, 必须是路径的相同前缀才能匹配;

给Route组件添加无值的exact属性, 该路由必须path完全相同才能匹配;
