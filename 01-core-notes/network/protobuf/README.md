# Protobuf
## 1.定义与核心功能

Protobuf 是一种**结构化数据序列化协议**。主要用于将程序中的对象编码成二进制数据，然后在网络、文件、消息队列中传输，接收方再根据相同的协议定义将二进制数据解析回对象。

它解决的核心问题是：

1. 不同语言之间如何描述同一份数据结构
2. 对象如何变成适合网络传输的二进制数据
3. 接收方如何根据字段编号还原出对应字段
4. 协议字段变更后如何尽量保持兼容

和 JSON 相比，Protobuf 不是文本协议，序列化后的数据里通常不保存字段名，而是保存**字段编号**和**字段类型**，所以体积更小，解析也更快。


---

## 2.Protobuf 的组成

从使用角度看，Protobuf 主要由三部分组成：

1. `.proto` 文件：描述数据结构
2. `protoc` 编译器：根据 `.proto` 生成 C++、Go、Java 等语言代码
3. 运行时库：提供序列化、反序列化、字段访问等能力

从手搓 mini-protobuf 的角度看，需要重点关心的是二进制编码部分：

1. `field_number`：字段编号
2. `wire_type`：字段的二进制编码类型
3. `tag`：由字段编号和编码类型组合出来
4. `varint`：变长整数编码
5. `length-delimited`：字符串、bytes、嵌套 message 的编码方式

---

## 3.基本使用流程

### 定义 .proto 文件

`.proto` 文件主要用来描述消息结构。比如定义一个用户：

```proto
syntax = "proto3";

package user;

message User {
  int32 id = 1;
  string name = 2;
  int32 age = 3;
}
```

这里的 `User` 类似 C++ 里的结构体：

```cpp
struct User {
    int id;
    std::string name;
    int age;
};
```

但是 `.proto` 里每个字段后面都有一个数字：

```proto
int32 id = 1;
string name = 2;
int32 age = 3;
```

这个数字不是默认值，而是**字段编号**。Protobuf 序列化后的二进制数据里不直接保存 `id`、`name`、`age` 这些字段名，而是用字段编号来区分字段。

### 通过 protoc 生成代码

真实 Protobuf 使用时，会通过 `protoc` 生成 C++ 代码：

```bash
protoc --cpp_out=. user.proto
```

生成结果通常是：

```text
user.pb.h
user.pb.cc
```

之后 C++ 就可以直接使用生成的类：

```cpp
user::User u;
u.set_id(1001);
u.set_name("zhangsan");
u.set_age(20);
```

### 序列化和反序列化

序列化就是把对象转成二进制数据：

```cpp
std::string data;
u.SerializeToString(&data);
```

反序列化就是把二进制数据还原成对象：

```cpp
user::User other;
other.ParseFromString(data);
```

后面如果自己手搓 mini-protobuf，本质上就是自己实现类似的过程：

```text
User 对象
    -> 根据字段编号和字段类型编码
    -> 得到二进制数据
    -> 根据 tag 逐个字段解析
    -> 还原 User 对象
```

---


## 4.和 RPC 的关系

Protobuf 只负责数据的编码和解码，不负责网络通信。

一个 RPC 框架除了序列化，还需要处理：

1. 请求和响应如何对应
2. 一个 TCP 连接上如何区分多条消息
3. 超时如何处理
4. 服务端如何根据方法名分发请求
5. 连接断开后请求如何失败返回

所以在 RPC 里，Protobuf 通常只是协议体的一部分：

```text
RPC Header
    request_id
    method_name
    payload_size

RPC Body
    protobuf payload
```

也就是说，Protobuf 负责 `payload`，而 RPC 框架还要在外层加自己的消息头。

---


