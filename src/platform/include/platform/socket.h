//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#include "export.h"
#include "core/results.h"
#include "core/core_types.h"

typedef struct cco_socket_t cco_socket_t;
typedef cco_socket_t *cco_socket;

/// @brief Socket protocol types
typedef enum {
    CCO_SOCKET_PROTOCOL_UDP, ///< @brief Connection-less, unreliable order, fast-focused delivery of data
    CCO_SOCKET_PROTOCOL_TCP ///< @brief Connection-orientated, reliable order, error-checked delivery of data
} cco_socket_protocol;

/// @brief Initializes the socket system (if the platform needs to)
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_init(void);

/// @brief Closes the socket system (if the platform needs to)
CCO_PLATFORM_API void cco_socket_quit(void);

/// @brief Creates a new socket
/// @param socket_protocol The socket protocol to use with this socket
/// @param out_socket The created socket, or CCO_NIL if it failed
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_open(cco_socket_protocol socket_protocol, cco_socket *out_socket);

/// @brief Closes a socket
/// @param socket The socket to close
CCO_PLATFORM_API void cco_socket_close(cco_socket socket);

/// @brief Claims an ip:port for this socket
/// @note This is usually for listening for incoming requests and packets
/// @param socket The socket to claim this address
/// @param ip The internet address to claim (use "0.0.0.0" to listen to all internet addresses this computer can access)
/// @param port Unique id to claim for this internet address
/// @return CCO_SUCCESS if binding succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_bind_addr(cco_socket socket, const char *ip, u16 port);

/// @brief Opens the socket's ip:port for listening
/// @param socket The socket to open for listening
/// @param max_connection_requests The size of the client connection request queue
/// @note The client connection request queue frees up after accepting clients
/// @warning It's a good idea to place a reasonable number (5-128).
/// If the request queue fills up, clients will be rejected until you cco_socket_accept() pending connections.
/// @return CCO_SUCCESS if listening succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_listen(cco_socket socket, i32 max_connection_requests);

/// @brief Accepts the next incoming connect request
/// @param socket The socket to accept with
/// @param out_client The new client connection
/// @return CCO_SUCCESS if accepting succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_accept(cco_socket socket, cco_socket *out_client);

/// @param Connects to an internet address
/// @param socket The socket to connect with
/// @param ip The socket host's internet address
/// @param port The socket host's internet port
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_connect(cco_socket socket, const char *ip, u16 port);

/// @brief Sends packet data to the socket
/// @param socket The socket to receive the packet data
/// @param packet_size The total byte count of the packet data
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_send(cco_socket socket, const void *packet, usize packet_size);

/// @brief Receives the next packet in line
/// @param socket The socket to receive packets from
/// @param out_packet The packet data received
/// @param max_packet_size The max amount of bytes allowed to be written
/// @param out_bytes_recv The amount of bytes received
/// @return CCO_SUCCESS if receiving succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_recv(cco_socket socket, void *out_packet, usize max_packet_size, ssize *out_bytes_recv);