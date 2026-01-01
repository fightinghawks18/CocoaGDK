//
// Created by fightinghawks18 on 12/30/2025.
//

#include "cocoa_gdk/platform/socket.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include "cocoa_gdk/core/log.h"

struct cco_socket_t {
    SOCKET fd;
    i32 af;

    struct ip_mreq multicast_group;
    cco_bool is_multicast_member;
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

i32 send_to_socket(cco_socket socket, const void *packet_data, const ssize packet_size, const char *ip,
                   const u16 port) {
    struct sockaddr_in addr = construct_address(socket->af, ip, port);
    return sendto(socket->fd, packet_data, packet_size, 0, (struct sockaddr *)&addr, sizeof(addr));
}

i32 set_options_socket(cco_socket socket, cco_socket_level level, cco_socket_option option, const void *value,
                       const ssize value_size) {
    i32 slevel = 0;
    switch (level) {
    case CCO_SOCKET_LEVEL_IP: {
        slevel = IPPROTO_IP;
        break;
    }
    case CCO_SOCKET_LEVEL_SOCKET: {
        slevel = SOL_SOCKET;
        break;
    }
    }

    i32 soption = 0;
    switch (option) {
    case CCO_SOCKET_OPTION_BROADCAST: {
        soption = SO_BROADCAST;
        break;
    }
    case CCO_SOCKET_OPTION_RECV_TIME_OUT: {
        soption = SO_RCVTIMEO;
        break;
    }
    case CCO_SOCKET_OPTION_REUSE_ADDR: {
        soption = SO_REUSEADDR;
        break;
    }
    case CCO_SOCKET_OPTION_ADD_MEMBER: {
        soption = IP_ADD_MEMBERSHIP;
        break;
    }
    case CCO_SOCKET_OPTION_DROP_MEMBER: {
        soption = IP_DROP_MEMBERSHIP;
        break;
    }
    }

    return setsockopt(socket->fd, slevel, soption, value, value_size);
}

i32 join_multicast_socket(cco_socket socket, const char *ip) {
    struct ip_mreq mreq;
    inet_pton(socket->af, ip, &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    socket->multicast_group = mreq;
    socket->is_multicast_member = CCO_YES;

    return set_options_socket(socket, CCO_SOCKET_LEVEL_IP, CCO_SOCKET_OPTION_ADD_MEMBER, &mreq, sizeof(mreq));
}

i32 leave_multicast_socket(cco_socket socket) {
    if (!socket->is_multicast_member)
        return 0;

    return set_options_socket(socket, CCO_SOCKET_LEVEL_IP, CCO_SOCKET_OPTION_DROP_MEMBER, &socket->multicast_group,
                              sizeof(socket->multicast_group));
}

#define NEW_SOCKET(sock, addr_family, type, protocol)                                                                  \
    do {                                                                                                               \
        sock->fd = socket(addr_family, type, protocol);                                                                \
        sock->is_multicast_member = CCO_NO;                                                                            \
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
#define SOCKET_SEND_TO(socket, ip, port, packet_data, packet_size)                                                     \
    send_to_socket(socket, packet_data, packet_size, ip, port)
#define SOCKET_SET_OPTIONS(socket, level, option, value, value_size)                                                   \
    set_options_socket(socket, level, option, value, value_size)
#define SOCKET_JOIN_MULTICAST(socket, ip) join_multicast_socket(socket, ip)
#define SOCKET_LEAVE_MULTICAST(socket) leave_multicast_socket(socket);
#define SOCKET_RECV(socket, out_packet_data, max_packet_size) recv(socket->fd, out_packet_data, max_packet_size, 0)
#define SOCKET_RECV_FROM(socket, out_addr, addr_len, out_packet, max_packet_size)                                      \
    recvfrom(socket->fd, out_packet, max_packet_size, 0, (struct sockaddr *)&addr, &addr_len)

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

cco_result cco_socket_send(cco_socket socket, const void *packet, const ssize packet_size) {
    if (SOCKET_SEND(socket, packet, packet_size) < 0) {
        LOG_WINSOCK_ERR("Failed to send Winsock packet!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_send_to(cco_socket socket, const char *ip, const u16 port, const void *packet,
                              const ssize packet_size) {
    if (SOCKET_SEND_TO(socket, ip, port, packet, packet_size) < 0) {
        LOG_WINSOCK_ERR("Failed to send Winsock packet!");
    }
    return CCO_SUCCESS;
}

cco_result cco_socket_join_multicast_group(cco_socket socket, const char *ip) {
    return SOCKET_JOIN_MULTICAST(socket, ip);
}

cco_result cco_socket_leave_multicast_group(cco_socket socket) { return SOCKET_LEAVE_MULTICAST(socket); }

cco_result cco_socket_broadcast_enable(cco_socket socket) {
    const i32 enabled = 1;
    return SOCKET_SET_OPTIONS(socket, CCO_SOCKET_LEVEL_SOCKET, CCO_SOCKET_OPTION_BROADCAST, &enabled, sizeof(i32));
}

cco_result cco_socket_broadcast_disable(cco_socket socket) {
    const i32 enabled = 0;
    return SOCKET_SET_OPTIONS(socket, CCO_SOCKET_LEVEL_SOCKET, CCO_SOCKET_OPTION_BROADCAST, &enabled, sizeof(i32));
}

cco_result cco_socket_set_option(cco_socket socket, const cco_socket_level level, const cco_socket_option option,
                                 const void *value, const ssize value_size) {
    return SOCKET_SET_OPTIONS(socket, level, option, value, value_size);
}

cco_result cco_socket_recv(cco_socket socket, void *out_packet, const ssize max_packet_size, ssize *out_bytes_recv) {
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

cco_result cco_socket_recv_from(cco_socket socket, char *out_ip, u16 *out_port, void *out_packet, ssize max_packet_size,
                                ssize *out_bytes_recv) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    const ssize packet_size = SOCKET_RECV_FROM(socket, addr, addr_len, out_packet, max_packet_size);
    if (packet_size > 0) { // Successfully accessed packet
        inet_ntop(socket->af, &addr.sin_addr, out_ip, INET_ADDRSTRLEN);
        *out_port = ntohs(addr.sin_port);
        *out_bytes_recv = packet_size;
        return CCO_SUCCESS;
    } else if (packet_size == 0) { // Connection closed
        *out_port = 0;
        *out_bytes_recv = 0;
        return CCO_FAIL_SOCKET_KICKED_ERROR;
    } else { // Error!
        *out_port = 0;
        *out_bytes_recv = 0;
        LOG_WINSOCK_ERR("Error receiving Winsock packets with sender information!");
    }
}

cco_result cco_socket_set_recv_timeout(cco_socket socket, const i32 timeout_ms) {
    struct timeval timeout = {timeout_ms, 0};
    return cco_socket_set_option(socket, CCO_SOCKET_LEVEL_SOCKET, CCO_SOCKET_OPTION_RECV_TIME_OUT, &timeout, sizeof(timeout));
}

void cco_socket_write_u32(unsigned char *packet_data, usize *offset, const u32 value) {
    const u32 network_u32 = htonl(value);
    memcpy(packet_data + *offset, &network_u32, sizeof(u32));
    *offset += sizeof(u32);
}

u32 cco_socket_read_u32(const void *packet_data, usize *offset) {
    u32 network_u32;
    memcpy(&network_u32, (const char *)packet_data + *offset, sizeof(u32));
    *offset += sizeof(u32);
    return ntohl(network_u32);
}

void cco_socket_write_u8(unsigned char *packet_data, usize *offset, const u8 value) {
    packet_data[*offset] = value;
    *offset += sizeof(u8);
}

u8 cco_socket_read_u8(const void *packet_data, usize *offset) {
    const u8 network_u8 = ((const u8 *)packet_data)[*offset];
    *offset += sizeof(u8);
    return network_u8;
}

void cco_socket_write_f32(unsigned char *packet_data, usize *offset, const f32 value) {
    u32 temp;
    memcpy(&temp, &value, sizeof(f32));
    const u32 network_f32 = htonl(temp);

    memcpy(packet_data + *offset, &network_f32, sizeof(u32));
    *offset += sizeof(u32);
}

f32 cco_socket_read_f32(const void *packet_data, usize *offset) {
    u32 network_f32;
    memcpy(&network_f32, (const char *)packet_data + *offset, sizeof(u32));
    *offset += sizeof(u32);

    const u32 temp = ntohl(network_f32);
    f32 result;
    memcpy(&result, &temp, sizeof(f32));

    return result;
}

void cco_socket_write_str(unsigned char *packet_data, usize *offset, const char *str) {
    const usize str_length = strlen(str);
    cco_socket_write_u32(packet_data, offset, str_length);

    memcpy(packet_data + *offset, str, str_length);
    *offset += str_length;
}

char *cco_socket_read_str(const void *packet_data, usize *offset) {
    const u32 str_length = cco_socket_read_u32(packet_data, offset);

    char *str = malloc(str_length + 1);
    memcpy(str, (const char *)packet_data + *offset, str_length);
    str[str_length] = '\0';
    *offset += str_length;
    return str;
}