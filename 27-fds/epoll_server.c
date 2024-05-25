#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <sys/timerfd.h>

#define MAX_EVENTS 10
#define PORT 33333

int create_and_bind_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    listen(sockfd, 5);

    return sockfd;
}

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int setup_timer(int epoll_fd) {
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

    struct itimerspec new_value;
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;

    timerfd_settime(tfd, 0, &new_value, NULL);

    struct epoll_event event;
    event.data.fd = tfd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tfd, &event);

   return tfd;
}

void handle_client_events(int epoll_fd, struct epoll_event *events, int i) {
    int fd = events[i].data.fd;

    if (events[i].events & EPOLLHUP) {
        printf("Disconnected. Freed fd: %d\n", fd);
        close(fd);
    } else if (events[i].events & EPOLLIN) {
        char buffer[1024];
        ssize_t count;
        while ((count = read(fd, buffer, sizeof(buffer))) > 0) {
            printf("[MSG%d] %.*s\n", fd, (int)count, buffer);
            if (write(fd, buffer, count) == -1) {
                perror("write");
                close(fd);
                break;
            }
        }
        if (count == -1 && errno != EAGAIN) {
            perror("read");
            close(fd);
        } else if (count == 0) {
            printf("Disconnected fd: %d\n", fd);
            close(fd);
        }
    }
}

void handle_new_client(int epoll_fd, struct epoll_event *events, int i) {
    int server_fd = events[i].data.fd;

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            if (errno == EAGAIN) {
                break;
            }
        }

        set_nonblocking(client_fd);
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLET | EPOLLHUP;
        event.data.fd = client_fd;

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
        printf("Connected fd: %d\n", client_fd);
    }
}

void handle_print_log(int fd) {
    int expired_count;
    read(fd, &expired_count, sizeof(expired_count));
    printf("[LOG] everything works cool\n");
}

int main() {
    int server_fd = create_and_bind_socket();
    set_nonblocking(server_fd);

    int epoll_fd = epoll_create1(0);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = server_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    int timer_fd = setup_timer(epoll_fd);

    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        printf("event\n");

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                handle_new_client(epoll_fd, events, i);
            } else if (events[i].data.fd == timer_fd) {
                handle_print_log(timer_fd);
            } else {
                handle_client_events(epoll_fd, events, i);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}

