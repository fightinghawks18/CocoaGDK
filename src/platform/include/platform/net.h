//
// Created by fightinghawks18 on 12/30/2025.
//

#pragma once

#include "core/core_types.h"
#include "core/results.h"
#include "export.h"

/// @brief Initializes the networking system
/// @return CCO_SUCCESS if it successfully initialized, or check error result if it isn't
CCO_PLATFORM_API cco_result cco_net_init(void);

/// @brief Closes the networking system
CCO_PLATFORM_API void cco_net_quit(void);

// OBSERVER / INTERACTS WITH SERVERS WITHOUT A CONNECTION

/// @brief Origin of internet traffic for a room
typedef struct {
    const char *ip; ///< @brief Host's internet address
    u16 port;       ///< @brief Special id where internet traffic is routed to in the host's internet address
} cco_room_addr;

/// @brief Extra data appended to a room for identity
typedef struct {
    char key[32];
    char value[128];
} cco_room_metadata;

/// @brief Information of a server room
/// @note This is acquired from pinging a server for room info
typedef struct {
    cco_room_addr addr;  ///< @brief Origin of internet traffic for this room
    char room_name[128]; ///< @brief Customizable display name for this room
    u32 current_players; ///< @brief Amount of players that are connected to this room
    u32 max_players;     ///< @brief Max amount of players that are allowed to connect to this room
    u32 ping_ms;         ///< @brief The amount of time it takes for this room to receive this ping

    cco_room_metadata metadata[8]; ///< @brief Extra data appended to this room
    u32 metadata_count;            ///< @brief How much metadata is appended to this room
} cco_room_info;

typedef struct cco_net_observer_socket_t cco_net_observer_socket_t;
typedef cco_net_observer_socket_t
    *cco_net_observer_socket; ///< @brief Socket that interacts with rooms, usually for probing/discovery

CCO_PLATFORM_API cco_result cco_net_observer_create(void);
CCO_PLATFORM_API cco_result cco_net_observer_probe_for_rooms(cco_net_observer_socket observer, cco_room_info *out_rooms,
                                                             u32 max_rooms, u32 timeout_ms, u32 *out_room_count);

// GUEST / CONNECTION TO A ROOM

typedef struct cco_net_guest_connection_t cco_net_guest_connection_t;
typedef cco_net_guest_connection_t *cco_net_guest_connection; ///< @brief Socket that represents a connection to a room

CCO_PLATFORM_API cco_result cco_net_guest_join_room(cco_room_addr addr, cco_net_guest_connection *out_guest);
CCO_PLATFORM_API void cco_net_guest_leave_room(cco_net_guest_connection guest);

// ROOMS / SERVERS

typedef struct cco_net_room_addr_t cco_net_room_addr_t;
typedef cco_net_room_addr_t *cco_net_room_addr; ///< @brief Points to a port inside a computer address, this is where
                                                ///< network traffic is directed to

typedef struct cco_net_room_socket_t cco_net_room_socket_t;
typedef cco_net_room_socket_t *cco_net_room_socket; ///< @brief Socket that hosts a port for connections to connect to

typedef struct {
    char ip[64];   ///< @brief IP address of guest
    u16 port;      ///< @brief Port of guest
    u64 timestamp; ///< @brief When this request arrived
} cco_guest_request;

typedef enum {
    CCO_NET_ROOM_GUEST_REQUEST_DENIED = 0,
    CCO_NET_ROOM_GUEST_REQUEST_ACCEPTED
} cco_net_room_guest_request_result;

typedef cco_net_room_guest_request_result (*cco_net_room_on_guest_join_request)(const cco_guest_request *request,
                                                                                void *user_data);
typedef void (*cco_net_room_on_guest_join)(cco_net_guest_connection guest, const cco_guest_request *request,
                                           void *user_data);

CCO_PLATFORM_API cco_result cco_net_room_host(cco_room_addr addr, cco_net_room_socket *out_room);
CCO_PLATFORM_API void cco_net_room_close(cco_net_room_socket room);
CCO_PLATFORM_API void cco_net_room_kick_guest(cco_net_room_socket room, u32 guest_id);
CCO_PLATFORM_API void cco_net_room_set_guest_request_callback(cco_net_room_socket room,
                                                              cco_net_room_on_guest_join_request fun, void *user_data);
CCO_PLATFORM_API void cco_net_room_set_guest_join_callback(cco_net_room_socket room, cco_net_room_on_guest_join fun,
                                                           void *user_data);
CCO_PLATFORM_API void cco_net_room_get_room_info(cco_net_room_socket room, cco_room_info *out_room);
