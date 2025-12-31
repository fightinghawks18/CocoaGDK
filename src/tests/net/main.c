//
// Created by fightinghawks18 on 12/31/2025.
//

#include <string.h>

#include "core/log.h"
#include "platform/platform.h"
#include "platform/socket.h"
#include "platform/utils.h"

cco_socket host_socket = CCO_NIL;
cco_socket client_socket = CCO_NIL;

cco_bool hosting = CCO_NO;

typedef struct {
    char name[15];
    char msg[128];
} client_msg;

static void start_host() {
    hosting = CCO_YES;
    cco_socket_open(CCO_SOCKET_PROTOCOL_TCP, &host_socket);
    cco_socket_bind_addr(host_socket, "127.0.0.1", 3000);
    cco_socket_listen(host_socket, 5);

    cco_socket_accept(host_socket, &client_socket);
    CCO_LOG("Accepted client!");

    client_msg msg;
    ssize byte_size;
    cco_socket_recv(client_socket, &msg, sizeof(msg), &byte_size);
    CCO_LOG("%s said '%s' (%d bytes received)", msg.name, msg.msg, byte_size);

    const client_msg host_msg = {
        .name = "Host",
        .msg = "Hello Client!"
    };
    cco_socket_send(client_socket, &host_msg, sizeof(host_msg));
    cco_sleep(1000);
    CCO_LOG("Exiting program..");
}

static void start_client() {
    cco_result res = cco_socket_open(CCO_SOCKET_PROTOCOL_TCP, &client_socket);
    cco_socket_connect(client_socket, "127.0.0.1", 3000);
    CCO_LOG("Connected");

    const client_msg msg = {
        .name = "Client",
        .msg = "Hello Server!"
    };

    CCO_LOG("Sending message with size %llu", sizeof(msg));
    cco_socket_send(client_socket, &msg, sizeof(msg));

    client_msg host_msg;
    ssize byte_size;
    cco_socket_recv(client_socket, &host_msg, sizeof(host_msg), &byte_size);
    CCO_LOG("%s said '%s' (%d bytes received)", host_msg.name, host_msg.msg, byte_size);
    CCO_LOG("Exiting program..");
}

int main(const int argc, char *argv[]) {
    cco_platform_init(CCO_PLATFORM_INIT_SOCKET_BIT);

    for (i32 i = 1; i < argc; i++) {
        if (strcmp(argv[i], "host") == 0) {
            CCO_LOG("Program is hosting");
            start_host();
            break;
        }
    }

    if (!hosting)
        start_client();
}