//
// Created by fightinghawks18 on 12/30/2025.
//

#include "platform/socket.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include "core/log.h"

struct cco_socket_t {
    SOCKET fd;
    i32 af;
};

cco_result get_result_from_wsa_err(const i32 err) {
    switch (err) {
    case WSAEADDRINUSE: {
        return CCO_FAIL_SOCKET_ADDR_IN_USE_ERROR;
    }
    case WSAEACCES: {
        return CCO_FAIL_SOCKET_NO_PERMS_ERROR;
    }
    case WSAEADDRNOTAVAIL: {
        return CCO_FAIL_SOCKET_ADDR_NOT_AVAIL_ERROR;
    }
    case WSAENOTSOCK:
    case WSAEBADF: {
        return CCO_FAIL_SOCKET_BAD_SOCK_ERROR;
    }
    case WSAEINVAL: {
        return CCO_FAIL_SOCKET_BAD_ARGS_ERROR;
    }
    case WSAEFAULT: {
        return CCO_FAIL_SOCKET_BAD_ADDR_ERROR;
    }
    case WSAEAFNOSUPPORT: {
        return CCO_FAIL_SOCKET_AF_NO_SUPPORTED_ERROR;
    }
    case WSAEMFILE:
    case WSAENOBUFS: {
        return CCO_FAIL_SOCKET_TOO_MANY_ERROR;
    }
    case WSAECONNREFUSED: {
        return CCO_FAIL_SOCKET_CONNECT_REJECTED_ERROR;
    }
    case WSAETIMEDOUT: {
        return CCO_FAIL_SOCKET_CONNECT_TIMEOUT_ERROR;
    }
    case WSAENETUNREACH:
    case WSAEHOSTUNREACH: {
        return CCO_FAIL_SOCKET_CONNECT_MIA_HOST_ERROR;
    }
    case WSAECONNRESET: {
        return CCO_FAIL_SOCKET_KICKED_ERROR;
    }
    case WSAENOTCONN: {
        return CCO_FAIL_SOCKET_UNCONNECTED_ERROR;
    }
    case WSAEISCONN: {
        return CCO_FAIL_SOCKET_CONNECTED_ERROR;
    }
    case WSAECONNABORTED:
    case WSAESHUTDOWN: {
        return CCO_FAIL_SOCKET_BROKEN_PIPE_ERROR;
    }
    default: {
        return CCO_FAIL_SOCKET_UNKNOWN_ERROR;
    }
    }
}

const char *cco_result_to_string(cco_result result) {
    const char *str;
    switch (result) {
    case CCO_SUCCESS: {
        str = "Success";
        break;
    }
    case CCO_FAIL_SOCKET_ADDR_IN_USE_ERROR: {
        str = "Address already in use";
        break;
    }
    case CCO_FAIL_SOCKET_NO_PERMS_ERROR: {
        str = "Permission denied";
        break;
    }
    case CCO_FAIL_SOCKET_ADDR_NOT_AVAIL_ERROR: {
        str = "Address not available";
        break;
    }
    case CCO_FAIL_SOCKET_BAD_SOCK_ERROR: {
        str = "Invalid socket";
        break;
    }
    case CCO_FAIL_SOCKET_BAD_ARGS_ERROR: {
        str = "Invalid argument";
        break;
    }
    case CCO_FAIL_SOCKET_BAD_ADDR_ERROR: {
        str = "Invalid address";
        break;
    }
    case CCO_FAIL_SOCKET_AF_NO_SUPPORTED_ERROR: {
        str = "Address family not supported";
        break;
    }
    case CCO_FAIL_SOCKET_TOO_MANY_ERROR: {
        str = "Too many open sockets";
        break;
    }
    case CCO_FAIL_SOCKET_CONNECT_REJECTED_ERROR: {
        str = "Connection refused";
        break;
    }
    case CCO_FAIL_SOCKET_CONNECT_TIMEOUT_ERROR: {
        str = "Connection timed out";
        break;
    }
    case CCO_FAIL_SOCKET_CONNECT_MIA_HOST_ERROR: {
        str = "Host unreachable";
        break;
    }
    case CCO_FAIL_SOCKET_KICKED_ERROR: {
        str = "Connection reset by peer";
        break;
    }
    case CCO_FAIL_SOCKET_UNCONNECTED_ERROR: {
        str = "Socket not connected";
        break;
    }
    case CCO_FAIL_SOCKET_CONNECTED_ERROR: {
        str = "Socket already connected";
        break;
    }
    case CCO_FAIL_SOCKET_BROKEN_PIPE_ERROR: {
        str = "Broken pipe";
        break;
    }
    case CCO_FAIL_SOCKET_INIT_ERROR: {
        str = "Failed to initialize socket system";
        break;
    }
    case CCO_FAIL_SOCKET_CREATE_ERROR: {
        str = "Failed to create socket";
        break;
    }
    default: {
        str = "Unknown error";
        break;
    }
    }
    return str;
}

struct sockaddr_in construct_address(const i32 af, const char *ip, const u16 port) {
    struct sockaddr_in addr;
    addr.sin_family = af;
    addr.sin_port = htons(port);
    inet_pton(af, ip, &addr.sin_addr);
    return addr;
}

i32 bind_socket(cco_socket socket, const char *ip, const u16 port) {
    struct sockaddr_in addr = construct_address(socket->af, ip, port);
    return bind(socket->fd, (struct sockaddr *)&addr, sizeof(addr));
}

i32 connect_socket(cco_socket socket, const char *ip, const u16 port) {
    struct sockaddr_in addr = construct_address(socket->af, ip, port);
    return connect(socket->fd, (struct sockaddr *)&addr, sizeof(addr));
}

#define NEW_SOCKET(sock, addr_family, type, protocol)                                                                  \
    do {                                                                                                               \
        sock->fd = socket(addr_family, type, protocol);                                                                \
        sock->af = addr_family;                                                                                        \
    } while (0)

#define CLOSE_SOCKET(socket)                                                                                           \
    do {                                                                                                               \
        closesocket(socket->fd);                                                                                       \
        socket->fd = INVALID_SOCKET;                                                                                   \
    } while (0)
#define IS_SOCKET(socket) (socket->fd != INVALID_SOCKET)

#define LOG_WINSOCK_ERR(msg)                                                                                           \
    do {                                                                                                               \
        const i32 err_code = WSAGetLastError();                                                                        \
        cco_result err = get_result_from_wsa_err(err_code);                                                            \
        CCO_LOG("%s (%s, ID: %d)", msg, cco_result_to_string(err), err);                                               \
        return err;                                                                                                    \
    } while (0)

#define SOCKET_BIND(socket, ip, port) bind_socket(socket, ip, port)

#define SOCKET_LISTEN(socket, max_connection_requests) listen(socket->fd, max_connection_requests)

#define SOCKET_ACCEPT(listen_sock, client_sock, addr, addrlen)                                                         \
    do {                                                                                                               \
        (client_sock)->fd = accept((listen_sock)->fd, addr, addrlen);                                                  \
        (client_sock)->af = (listen_sock)->af;                                                                         \
    } while (0)

#define SOCKET_CONNECT(socket, ip, port) connect_socket(socket, ip, port)
#define SOCKET_SEND(socket, packet_data, packet_size) send(socket->fd, packet_data, packet_size, 0)
#define SOCKET_RECV(socket, out_packet_data, max_packet_size) recv(socket->fd, out_packet_data, max_packet_size, 0)

cco_result cco_socket_init(void) {
    WSADATA wsa_data;
    const i32 wsa_init_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsa_init_result != 0) {
        CCO_LOG("Failed to initialize Winsock! %d", wsa_init_result);
        return CCO_FAIL_SOCKET_INIT_ERROR;
    }
    return CCO_SUCCESS;
}

void cco_socket_quit(void) { WSACleanup(); }

cco_result cco_socket_open(cco_socket_protocol socket_protocol, cco_socket *out_socket) {
    cco_socket sock = malloc(sizeof(cco_socket_t));
    if (!sock)
        return CCO_FAIL_OUT_OF_MEMORY;

    i32 type = 0;
    switch (socket_protocol) {
    case CCO_SOCKET_PROTOCOL_TCP: {
        type = SOCK_STREAM;
        break;
    }
    case CCO_SOCKET_PROTOCOL_UDP: {
        type = SOCK_DGRAM;
        break;
    }
    }

    NEW_SOCKET(sock, AF_INET, type, 0);
    if (!IS_SOCKET(sock)) {
        free(sock);
        LOG_WINSOCK_ERR("Failed to create Winsock socket!");
    }

    *out_socket = sock;
    return CCO_SUCCESS;
}

void cco_socket_close(cco_socket socket) {
    if (IS_SOCKET(socket)) {
        CLOSE_SOCKET(socket);
    }
    free(socket);
}

cco_result cco_socket_bind_addr(cco_socket socket, const char *ip, const u16 port) {
    if (SOCKET_BIND(socket, ip, port) != 0) {
        LOG_WINSOCK_ERR("Failed to bind Winsock address!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_listen(cco_socket socket, i32 max_connection_requests) {
    if (SOCKET_LISTEN(socket, max_connection_requests) != 0) {
        LOG_WINSOCK_ERR("Failed to listen on Winsock socket!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_accept(cco_socket socket, cco_socket *out_client) {
    cco_socket client = malloc(sizeof(cco_socket_t));
    if (!client)
        return CCO_FAIL_OUT_OF_MEMORY;

    SOCKET_ACCEPT(socket, client, CCO_NIL, CCO_NIL);

    if (!IS_SOCKET(client)) {
        free(client);
        LOG_WINSOCK_ERR("Failed to accept Winsock socket!");
    }

    *out_client = client;
    return CCO_SUCCESS;
}

cco_result cco_socket_connect(cco_socket socket, const char *ip, u16 port) {
    if (SOCKET_CONNECT(socket, ip, port) != 0) {
        LOG_WINSOCK_ERR("Failed to connect to Winsock address!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_send(cco_socket socket, const void *packet, const usize packet_size) {
    if (SOCKET_SEND(socket, packet, packet_size) < 0) {
        LOG_WINSOCK_ERR("Failed to send Winsock packet!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_recv(cco_socket socket, void *out_packet, const usize max_packet_size, ssize *out_bytes_recv) {
    const ssize packet_size = SOCKET_RECV(socket, out_packet, max_packet_size);
    if (packet_size > 0) { // Successfully accessed packet
        *out_bytes_recv = packet_size;
        return CCO_SUCCESS;
    } else if (packet_size == 0) { // Connection closed
        *out_bytes_recv = 0;
        return CCO_FAIL_SOCKET_KICKED_ERROR;
    } else { // Error!
        *out_bytes_recv = 0;
        LOG_WINSOCK_ERR("Error receiving Winsock packets!");
    }
}
