#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#define PORT 12345

void sendCommand(const char *command) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将 "127.0.0.1" 替换为服务器的实际IP地址
    if (inet_pton(AF_INET, "192.168.116.162", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return;
    }

    send(sock, command, strlen(command), 0);
    printf("Message sent: %s\n", command);
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    close(sock);
}
// 检查输入的命令是否合法
void is_valid_command(const char *command) {
    // 检查是否是 "ON", "OFF", "SILENT"
    if (strcmp(command, "ON") == 0 || strcmp(command, "OFF") == 0 || strcmp(command, "SILENT") == 0) {
        sendCommand(command);
    }else printf("Command Error\n");
    // 检查是否是 "PERIODIC <ms>" 格式
    if (strncmp(command, "PERIODIC ", 9) == 0) {
        const char *ptr = command + 9;
        while (*ptr) {
            if (!isdigit(*ptr)) {
                    printf("Command Error\n");
                return ;
            }
            ptr++;
        }
        sendCommand(command);
    }
    return ;
}
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
