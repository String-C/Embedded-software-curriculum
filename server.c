#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 12345

bool running = true;
bool buzzer_on = false;
int period = 1000;  // 默认周期为1000毫秒（1秒）

void *buzzerThread(void *arg) {
    while (running) {
        if (buzzer_on) {
            printf("Buzzer ON\n");
        } else {
            printf("Buzzer OFF\n");
        }
        usleep(period * 1000);
    }
    return NULL;
}

void handleClient(int new_socket) {
    char buffer[1024] = {0};
    int valread;

    while ((valread = read(new_socket, buffer, 1024)) > 0) {
        buffer[valread] = '\0';
        printf("Received: %s\n", buffer);

        // 处理蜂鸣器控制命令
        if (strcmp(buffer, "ON") == 0) {
            buzzer_on = true;
            printf("Buzzer ON\n");
        } else if (strcmp(buffer, "OFF") == 0) {
            buzzer_on = false;
            printf("Buzzer OFF\n");
        } else if (strncmp(buffer, "PERIODIC ", 9) == 0) {
            sscanf(buffer + 9, "%d", &period);
            buzzer_on = true;
            printf("Buzzer PERIODIC with period %d ms\n", period);
        } else if (strcmp(buffer, "SILENT") == 0) {
            buzzer_on = false;
            printf("Buzzer SILENT\n");
        } else {
            printf("Invalid command: %s\n", buffer);
        }

        send(new_socket, "Command received", strlen("Command received"), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    if (valread == 0) {
        printf("Client disconnected\n");
    } else {
        perror("Read error");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t buzzer_thread;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // 创建一个线程来模拟蜂鸣器的周期性响和停
    pthread_create(&buzzer_thread, NULL, buzzerThread, NULL);

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        printf("Client connected\n");
        handleClient(new_socket);
        close(new_socket);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 结束程序时停止蜂鸣器线程
    running = false;
    pthread_join(buzzer_thread, NULL);

    close(server_fd);
    return 0;
}