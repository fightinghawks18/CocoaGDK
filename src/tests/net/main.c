//
// Created by fightinghawks18 on 12/31/2025.
//

#include <string.h>

#include "cocoa_gdk/core/log.h"
#include "cocoa_gdk/platform/platform.h"
#include "cocoa_gdk/platform/socket.h"
#include "cocoa_gdk/platform/utils.h"

#include <stdlib.h>

cco_socket host_socket = CCO_NIL;
cco_socket client_socket = CCO_NIL;

cco_bool hosting = CCO_NO;

typedef struct {
    char name[15];
    char msg[128];
} client_msg;

static unsigned char *construct_msg_buffer(const char *name, const char *msg, usize *out_bytes) {
    usize msg_buffer_size = 0;
    msg_buffer_size += sizeof(u32) + sizeof(u32); // Writing strings appends u32s to describe string length
    msg_buffer_size += strlen(name) + strlen(msg);

    unsigned char *msg_buffer = malloc(msg_buffer_size);
    usize msg_buffer_offset = 0;

    cco_socket_write_str(msg_buffer, &msg_buffer_offset, name);
    cco_socket_write_str(msg_buffer, &msg_buffer_offset, msg);

    *out_bytes = msg_buffer_offset;
    return msg_buffer;
}

static void print_from_msg_buffer(void *msg, ssize byte_size) {
    usize msg_offset = 0;
    char *name = cco_socket_read_str(msg, &msg_offset);
    char *msg_contents = cco_socket_read_str(msg, &msg_offset);

    CCO_LOG("%s said '%s' (%d bytes received)", name, msg_contents, byte_size);
    free(name);
    free(msg_contents);
}

static void start_host() {
    hosting = CCO_YES;
    cco_socket_open(CCO_SOCKET_PROTOCOL_TCP, &host_socket);
    cco_socket_bind_addr(host_socket, "127.0.0.1", 3000);
    cco_socket_listen(host_socket, 5);

    cco_socket_accept(host_socket, &client_socket);
    CCO_LOG("Accepted client!");

    unsigned char buffer[512];
    ssize byte_size;
    cco_socket_recv(client_socket, buffer, 512, &byte_size);
    CCO_LOG("Received data! %d bytes", byte_size);

    print_from_msg_buffer(buffer, byte_size);

    usize msg_size;
    unsigned char *msg_buffer = construct_msg_buffer("Server", "Hello Client!", &msg_size);
    cco_socket_send(client_socket, msg_buffer, (ssize)msg_size);
    free(msg_buffer);

    cco_sleep(1000);
    CCO_LOG("Exiting program..");
}

static void start_client() {
    cco_socket_open(CCO_SOCKET_PROTOCOL_TCP, &client_socket);
    cco_socket_connect(client_socket, "127.0.0.1", 3000);
    CCO_LOG("Connected");

    usize msg_size;
    unsigned char *msg_buffer = construct_msg_buffer("Client", "Hello Server!", &msg_size);
    CCO_LOG("SENDING %llu bytes of data to host!", msg_size);

    cco_socket_send(client_socket, msg_buffer, (ssize)msg_size);
    free(msg_buffer);

    unsigned char buffer[512];
    ssize byte_size;
    cco_socket_recv(client_socket, buffer, 512, &byte_size);
    print_from_msg_buffer(buffer, byte_size);

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