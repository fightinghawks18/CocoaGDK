//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "export.h"

typedef struct cco_socket_t cco_socket_t;
typedef cco_socket_t *cco_socket;

/// @brief Socket protocol types
typedef enum {
    CCO_SOCKET_PROTOCOL_UDP, ///< @brief Connection-less, unreliable order, fast-focused delivery of data
    CCO_SOCKET_PROTOCOL_TCP  ///< @brief Connection-orientated, reliable order, error-checked delivery of data
} cco_socket_protocol;

typedef enum { CCO_SOCKET_LEVEL_SOCKET, CCO_SOCKET_LEVEL_IP } cco_socket_level;

typedef enum {
    CCO_SOCKET_OPTION_BROADCAST,     ///< @brief Broadcasting
    CCO_SOCKET_OPTION_RECV_TIME_OUT, ///< @brief Timeout for receives
    CCO_SOCKET_OPTION_REUSE_ADDR,    ///< @brief Reusing socket address
    CCO_SOCKET_OPTION_ADD_MEMBER,    ///< @brief Joining multicast group
    CCO_SOCKET_OPTION_DROP_MEMBER,   ///< @brief Leaving multicast group
} cco_socket_option;

/// @brief Initializes the socket system (if the platform needs to)
/// @return CCO_SUCCESS if initialization succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_init(void);

/// @brief Closes the socket system (if the platform needs to)
CCO_PLATFORM_API void cco_socket_quit(void);

/// @brief Creates a new socket
/// @param socket_protocol The socket protocol to use with this socket
/// @param out_socket The created socket, or CCO_NIL if it failed
/// @return CCO_SUCCESS if opening succeeded, or check error result
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

/// @brief Connects to an internet address
/// @param socket The socket to connect with
/// @param ip The socket host's internet address
/// @param port The socket host's internet port
/// @return CCO_SUCCESS if connecting succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_connect(cco_socket socket, const char *ip, u16 port);

/// @brief Sends packet data to the socket
/// @param socket The socket to receive the packet data
/// @param packet The packet data to send
/// @param packet_size The total byte count of the packet data
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_send(cco_socket socket, const void *packet, ssize packet_size);

/// @brief Sends packet data to the socket to a specific address
/// @param socket The socket to receive the packet data
/// @param ip The internet address to send packet data to
/// @param port The internet port for that internet address
/// @param packet The packet data to send
/// @param packet_size The total byte count of the packet data
/// @return CCO_SUCCESS if sending succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_send_to(cco_socket socket, const char *ip, u16 port, const void *packet,
                                               ssize packet_size);

/// @brief Joins a multicast group
/// @param socket The socket joining the multicast group
/// @param ip The address of the multicast group
/// @return CCO_SUCCESS if joining succeeded, or check error result
/// @note A multicast group is a group of devices that send/recv messages to multiple recipients at once
CCO_PLATFORM_API cco_result cco_socket_join_multicast_group(cco_socket socket, const char *ip);

/// @brief Joins a multicast group
/// @param socket The socket joining the multicast group
/// @param ip The address of the multicast group
/// @return CCO_SUCCESS if leaving succeeded, or check error result
/// @note A multicast group is a group of devices that send/recv messages to multiple recipients at once
CCO_PLATFORM_API cco_result cco_socket_leave_multicast_group(cco_socket socket);

/// @brief Enables the ability to broadcast packets
/// @param socket The socket to enable broadcasting for
/// @return CCO_SUCCESS if broadcast enabling succeeded, or check error result
/// @note This is best for LAN discovery for example
CCO_PLATFORM_API cco_result cco_socket_broadcast_enable(cco_socket socket);

/// @brief Disables the ability to broadcast packets
/// @param socket The socket to disable broadcasting for
/// @return CCO_SUCCESS if broadcast disabling succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_broadcast_disable(cco_socket socket);

/// @brief Configures socket property
/// @param socket The socket to configure
/// @param level The protocol layer the property uses
/// @param option The type of option to configure
/// @param value The value this option will use
/// @param value_size The size of the value
/// @return CCO_SUCCESS if option configuration succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_set_option(cco_socket socket, cco_socket_level level, cco_socket_option option,
                                                  const void *value, ssize value_size);

/// @brief Receives the next packet in line
/// @param socket The socket to receive packets from
/// @param out_packet The packet data received
/// @param max_packet_size The max amount of bytes allowed to be written
/// @param out_bytes_recv The amount of bytes received
/// @return CCO_SUCCESS if receiving succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_recv(cco_socket socket, void *out_packet, ssize max_packet_size,
                                            ssize *out_bytes_recv);

/// @brief Receives the next packet in line with sender's information
/// @param socket The socket to receive packets from
/// @param out_ip The ip that sent this packet
/// @param out_port The port that sent this packet
/// @param out_packet The packet data received
/// @param max_packet_size The max amount of bytes allowed to be written
/// @param out_bytes_recv The amount of bytes received
/// @return CCO_SUCCESS if receiving succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_recv_from(cco_socket socket, char *out_ip, u16 *out_port, void *out_packet, ssize max_packet_size,
                                            ssize *out_bytes_recv);

/// @brief Sets the amount of time before recv drops if no packets are available
/// @param socket The socket to set time out on
/// @param timeout_ms The amount of time in milliseconds before recv drops
/// @return CCO_SUCCESS if configuration succeeded, or check error result
CCO_PLATFORM_API cco_result cco_socket_set_recv_timeout(cco_socket socket, i32 timeout_ms);

/// @brief Writes a unsigned 32-bit integer to packet data
/// @param packet_data The packet data to write the integer to
/// @param offset The current offset in the packet data
/// @param value The value to write to the packet data
/// @note Offset will be incremented by the byte count of this integer (4)
CCO_PLATFORM_API void cco_socket_write_u32(unsigned char *packet_data, usize *offset, u32 value);

/// @brief Converts packet data to an unsigned 32-bit integer
/// @param packet_data The packet data to read from
/// @param offset The current offset in the packet data
/// @return Unsigned 32-bit integer converted from packet data
/// @note Offset will be incremented by the byte count of this integer (4)
CCO_PLATFORM_API u32 cco_socket_read_u32(const void *packet_data, usize *offset);

/// @brief Writes an unsigned 8-bit integer to packet data
/// @param packet_data The packet data to write the integer to
/// @param offset The current offset in the packet data
/// @param value The value to write to the packet data
/// @note Offset will be incremented by the byte count of this integer (1)
CCO_PLATFORM_API void cco_socket_write_u8(unsigned char *packet_data, usize *offset, u8 value);

/// @brief Converts packet data to an unsigned 8-bit integer
/// @param packet_data The packet data to read from
/// @param offset The current offset in the packet data
/// @return Unsigned 8-bit integer converted from packet data
/// @note Offset will be incremented by the byte count of this integer (1)
CCO_PLATFORM_API u8 cco_socket_read_u8(const void *packet_data, usize *offset);

/// @brief Writes a 32-bit float to packet data
/// @param packet_data The packet data to write the float to
/// @param offset The current offset in the packet data
/// @param value The value to write to the packet data
/// @note Offset will be incremented by the byte count of this float (4)
CCO_PLATFORM_API void cco_socket_write_f32(unsigned char *packet_data, usize *offset, f32 value);

/// @brief Converts packet data to a 32-bit float
/// @param packet_data The packet data to read from
/// @param offset The current offset in the packet data
/// @return 32-bit float converted from packet data
/// @warning This method allocates string data, you must free this after you're done
/// @note Offset will be incremented by the byte count of this 32-bit float (4)
CCO_PLATFORM_API f32 cco_socket_read_f32(const void *packet_data, usize *offset);

/// @brief Writes a string to packet data
/// @param packet_data The packet data to write the string to
/// @param offset The current offset in the packet data
/// @param str The string to write to the packet
/// @note Offset will be incremented by the length of this string
CCO_PLATFORM_API void cco_socket_write_str(unsigned char *packet_data, usize *offset, const char *str);

/// @brief Converts packet data to a readable string
/// @param packet_data The packet data to read from
/// @param offset The current offset in the packet data
/// @return Readable string converted from packet data
/// @warning This method allocates string data, you must free this after you're done
/// @note Offset will be incremented by the length of this string
CCO_PLATFORM_API char *cco_socket_read_str(const void *packet_data, usize *offset);