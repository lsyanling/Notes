# gdb 调试

## 断点

### 打断点

#### `break` / `b`

- `b 行号`
- `b 文件:行号`
- `b 行号 if 条件`
- `b 函数名，给所有重载函数打断点`
- `b 函数签名`
- `b 类名:函数名`
- `b 类名:函数签名`
- `b 文件:函数名`
- `b 文件:函数签名`

#### `watch` 表达式

watch 本身也是一个断点

每次值发生变化时会断点，输出变量在断点前和断点后的值

### 查看断点

#### `info breakpoints` / `info break` / `i b`

- `i b 显示所有断点`
- `i b 断点编号`

### 删除断点

#### `clear`

- `clear 函数名`
- `clear 函数签名`
- `clear 行号`
- `clear 文件名:行号`

#### `delete` / `del` / `d`

- `d 断点编号`
- `d 断点范围 xx-yy`
- `d 删除全部断点`

#### `disable` / `dis`

- `dis 断点编号`
- `dis 断点范围 xx-yy`

#### `enable` / `ena` / `en`

- `en 断点编号`
- `en 断点范围 xx-yy`
- `en once 断点编号`
- `en count 次数 断点编号`

#### `ignore`

- `ignore 断点编号 次数`

### 保存断点 / 读取断点

- `save breakpoints 断点文件名`
- `gdb 执行程序名 -x 断点文件名`

## 执行

## 查看

## 远程调试

目标程序在 remote 执行，gdb 在 local 调试

### 直接调试

- remote: `gdbserver remoteIP:port 执行程序名`
- local: `gdb target remote remoteIP:port`

remote 调试不能使用 run，只能使用 continue

### 附加进程调试

- remote: 执行程序，`grep 执行程序名`，先获取进程号 `pid`
- remote: `gdbserver remoteIP:port --attach pid`
- local: `gdb target remote remoteIP:port`

### VS 附加进程调试 ssh