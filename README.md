# client
这段代码是一个简单的C语言程序，用于模拟发送命令给服务器的过程，并根据不同的命令执行相应的操作。下面我会逐步解释这段代码的主要部分。

1. **头文件和宏定义**
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <unistd.h>
   #include <arpa/inet.h>
   #include <ctype.h>
   #define PORT 12345
   ```
   这里包含了一些标准库和系统库的头文件，并定义了一个端口号 `PORT`，用于指定服务器的端口号。

2. **`sendCommand` 函数**
   ```c
   void sendCommand(const char *command) {
       // 创建 socket 和设置服务器地址等操作
       ...
       send(sock, command, strlen(command), 0); // 发送命令
       ...
       close(sock); // 关闭 socket
   }
   ```
   这个函数用于创建一个 socket，并向服务器发送命令。它首先创建一个 socket，然后设置服务器的地址和端口号，连接到服务器，发送命令，接收服务器返回的消息，最后关闭 socket。

3. **`is_valid_command` 函数**
   ```c
   void is_valid_command(const char *command) {
       // 检查命令是否合法，并调用 sendCommand 发送命令
       ...
   }
   ```
   这个函数用于检查输入的命令是否合法，并根据命令调用 `sendCommand` 函数发送命令给服务器。它首先检查命令是否是 "ON", "OFF", "SILENT" 这三种中的一种，如果是则直接发送命令；然后检查是否是 "PERIODIC <ms>" 格式的命令，如果是则发送命令；否则输出 "Command Error"。

4. **`main` 函数**
   ```c
   int main() {
       for(;;)
       {
           is_valid_command("ON");
           sleep(2);
           is_valid_command("PERIODIC 3000");
           sleep(2);
           is_valid_command("abc");
           sleep(2);
           is_valid_command("SILENT");
           sleep(4);
       }
       return 0;
   }
   ```
   这个 `main` 函数是程序的入口点，它包含一个无限循环，不断地调用 `is_valid_command` 函数发送不同的命令给服务器。每次发送完命令后，通过 `sleep` 函数让程序休眠一段时间，再发送下一个命令。这样循环执行，模拟了周期性地向服务器发送命令的过程。

总体来说，这段代码是一个简单的网络通信模拟程序，用于向服务器发送不同的命令并接收服务器返回的消息。

# server
这段代码是一个使用了多线程和套接字编程的简单服务器程序，可以接收客户端的命令控制蜂鸣器的开关和周期性响。下面逐步详细讲解每个部分的功能和实现。

1. **头文件和宏定义**
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <unistd.h>
   #include <arpa/inet.h>
   #include <pthread.h>
   #include <stdbool.h>
   #define PORT 12345
   ```
   这里包含了一些标准库和系统库的头文件，以及使用到的宏定义，其中 `PORT` 定义了服务器的端口号。

2. **全局变量和线程函数**
   ```c
   bool running = true;
   bool buzzer_on = false;
   int period = 1000;  // 默认周期为1000毫秒（1秒）

   void *buzzerThread(void *arg) {
       // 蜂鸣器线程函数实现
       ...
   }
   ```
   定义了全局变量 `running`、`buzzer_on` 和 `period`，用于控制服务器运行状态、蜂鸣器的开关状态和周期。还定义了一个名为 `buzzerThread` 的线程函数，用于模拟蜂鸣器的周期性响和停。

3. **`handleClient` 函数**
   ```c
   void handleClient(int new_socket) {
       // 处理客户端连接和命令的函数
       ...
   }
   ```
   这个函数用于处理客户端的连接和命令，通过读取客户端发送的命令来控制蜂鸣器的开关和周期性响。

4. **`main` 函数**
   ```c
   int main() {
       // 主函数，创建服务器套接字、处理客户端连接和启动蜂鸣器线程等
       ...
   }
   ```
   主函数中包含了创建服务器套接字、设置服务器参数、监听客户端连接、创建蜂鸣器线程和处理客户端连接的逻辑。

整体运行逻辑是，服务器通过 `socket` 创建套接字并绑定端口，然后监听客户端连接。当有客户端连接时，创建一个新的线程来处理客户端的命令，同时主线程继续监听其他客户端连接。蜂鸣器线程在后台运行，模拟蜂鸣器的周期性响和停。当服务器接收到关闭信号时，停止蜂鸣器线程并关闭服务器套接字。

这个程序演示了一个简单的服务器和客户端通信的例子，并利用多线程实现了对蜂鸣器的控制和周期性响。
