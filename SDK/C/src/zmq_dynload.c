/*
 * zmq_dynload.c — Dynamic binding to libzmq.dll.
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#define WIN32_LEAN_AND_MEAN
#include "zmq_dynload.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef void *(__cdecl *zmq_ctx_new_fn)(void);
typedef int   (__cdecl *zmq_ctx_term_fn)(void *);
typedef int   (__cdecl *zmq_ctx_shutdown_fn)(void *);
typedef void *(__cdecl *zmq_socket_fn)(void *, int);
typedef int   (__cdecl *zmq_close_fn)(void *);
typedef int   (__cdecl *zmq_bind_fn)(void *, const char *);
typedef int   (__cdecl *zmq_connect_fn)(void *, const char *);
typedef int   (__cdecl *zmq_send_fn)(void *, const void *, size_t, int);
typedef int   (__cdecl *zmq_recv_fn)(void *, void *, size_t, int);
typedef int   (__cdecl *zmq_errno_fn)(void);
typedef const char *(__cdecl *zmq_strerror_fn)(int);
typedef void  (__cdecl *zmq_version_fn)(int *, int *, int *);
typedef int   (__cdecl *zmq_has_fn)(const char *);
typedef int   (__cdecl *zmq_setsockopt_fn)(void *, int, const void *, size_t);
typedef int   (__cdecl *zmq_getsockopt_fn)(void *, int, void *, size_t *);

typedef struct zmq_api_table {
    HMODULE             module;
    int                 loaded;
    zmq_ctx_new_fn      ctx_new;
    zmq_ctx_term_fn     ctx_term;
    zmq_ctx_shutdown_fn ctx_shutdown;
    zmq_socket_fn       socket;
    zmq_close_fn        close;
    zmq_bind_fn         bind;
    zmq_connect_fn      connect;
    zmq_send_fn         send;
    zmq_recv_fn         recv;
    zmq_errno_fn        errno_fn;
    zmq_strerror_fn     strerror_fn;
    zmq_version_fn      version;
    zmq_has_fn          has;
    zmq_setsockopt_fn   setsockopt;
    zmq_getsockopt_fn   getsockopt;
} zmq_api_table;

static zmq_api_table g_zmq;

/**
 * @brief Bind required symbols.
 * @param void
 * @return int
 */
static int zmq_bind_required_symbols(void)
{
    g_zmq.ctx_new = (zmq_ctx_new_fn)GetProcAddress(g_zmq.module, "zmq_ctx_new");
    g_zmq.ctx_term = (zmq_ctx_term_fn)GetProcAddress(g_zmq.module, "zmq_ctx_term");
    g_zmq.ctx_shutdown = (zmq_ctx_shutdown_fn)GetProcAddress(g_zmq.module, "zmq_ctx_shutdown");
    g_zmq.socket = (zmq_socket_fn)GetProcAddress(g_zmq.module, "zmq_socket");
    g_zmq.close = (zmq_close_fn)GetProcAddress(g_zmq.module, "zmq_close");
    g_zmq.bind = (zmq_bind_fn)GetProcAddress(g_zmq.module, "zmq_bind");
    g_zmq.connect = (zmq_connect_fn)GetProcAddress(g_zmq.module, "zmq_connect");
    g_zmq.send = (zmq_send_fn)GetProcAddress(g_zmq.module, "zmq_send");
    g_zmq.recv = (zmq_recv_fn)GetProcAddress(g_zmq.module, "zmq_recv");
    g_zmq.errno_fn = (zmq_errno_fn)GetProcAddress(g_zmq.module, "zmq_errno");
    g_zmq.strerror_fn = (zmq_strerror_fn)GetProcAddress(g_zmq.module, "zmq_strerror");
    g_zmq.version = (zmq_version_fn)GetProcAddress(g_zmq.module, "zmq_version");
    g_zmq.has = (zmq_has_fn)GetProcAddress(g_zmq.module, "zmq_has");
    g_zmq.setsockopt = (zmq_setsockopt_fn)GetProcAddress(g_zmq.module, "zmq_setsockopt");
    g_zmq.getsockopt = (zmq_getsockopt_fn)GetProcAddress(g_zmq.module, "zmq_getsockopt");

    return g_zmq.ctx_new && g_zmq.ctx_term && g_zmq.ctx_shutdown && g_zmq.socket &&
           g_zmq.close && g_zmq.bind && g_zmq.connect && g_zmq.send && g_zmq.recv &&
           g_zmq.errno_fn && g_zmq.strerror_fn && g_zmq.version && g_zmq.has &&
           g_zmq.setsockopt && g_zmq.getsockopt;
}

/**
 * @brief Trim trailing slashes.
 * @param path
 * @return void
 */
static void zmq_trim_trailing_slashes(char *path)
{
    size_t len;

    if (path == NULL) {
        return;
    }

    len = strlen(path);

    while (len > 0 && (path[len - 1] == '\\' || path[len - 1] == '/')) {
        path[len - 1] = '\0';
        len--;
    }
}

/**
 * @brief Load the ZeroMQ library from a directory.
 * @param dir
 * @param dir_len
 * @return int
 */
int zmq_dyn_load_directory(const char *dir, DWORD dir_len)
{
    char folder[MAX_PATH];
    char dll_path[MAX_PATH];

    if (dir == NULL || dir_len == 0 || dir_len >= sizeof(folder)) {
        return 0;
    }

    if (g_zmq.loaded) {
        return 1;
    }

    memcpy(folder, dir, dir_len);
    folder[dir_len] = '\0';
    zmq_trim_trailing_slashes(folder);

    if (_snprintf(dll_path, sizeof(dll_path), "%s\\libzmq.dll", folder) < 0) {
        return 0;
    }

    SetDllDirectoryA(folder);
    g_zmq.module = LoadLibraryA(dll_path);
    
    if (g_zmq.module == NULL) {
        SetDllDirectoryA(NULL);
        
        return 0;
    }

    if (!zmq_bind_required_symbols()) {
        FreeLibrary(g_zmq.module);
        g_zmq.module = NULL;
        SetDllDirectoryA(NULL);
        memset(&g_zmq, 0, sizeof(g_zmq));

        return 0;
    }

    g_zmq.loaded = 1;

    return 1;
}

/**
 * @brief Unload the ZeroMQ library.
 * @param void
 * @return void
 */
void zmq_dyn_unload(void)
{
    if (g_zmq.module != NULL) {
        FreeLibrary(g_zmq.module);
    }

    memset(&g_zmq, 0, sizeof(g_zmq));
    SetDllDirectoryA(NULL);
}

/**
 * @brief Check if the ZeroMQ library is loaded.
 * @param void
 * @return int
 */
int zmq_dyn_is_loaded(void)
{
    return g_zmq.loaded;
}

/**
 * @brief Create a new ZeroMQ context.
 * @param void
 * @return void *
 */
void *zmq_api_ctx_new(void)
{
    return g_zmq.loaded ? g_zmq.ctx_new() : NULL;
}

/**
 * @brief Terminate a ZeroMQ context.
 * @param context
 * @return int
 */
int zmq_api_ctx_term(void *context)
{
    return g_zmq.loaded ? g_zmq.ctx_term(context) : -1;
}

/**
 * @brief Shutdown a ZeroMQ context.
 * @param context
 * @return int
 */
int zmq_api_ctx_shutdown(void *context)
{
    return g_zmq.loaded ? g_zmq.ctx_shutdown(context) : -1;
}

/**
 * @brief Create a new ZeroMQ socket.
 * @param context
 * @param type
 * @return void *
 */
void *zmq_api_socket(void *context, int type)
{
    return g_zmq.loaded ? g_zmq.socket(context, type) : NULL;
}

/**
 * @brief Close a ZeroMQ socket.
 * @param socket
 * @return int
 */
int zmq_api_close(void *socket)
{
    return g_zmq.loaded ? g_zmq.close(socket) : -1;
}

/**
 * @brief Bind a ZeroMQ socket to an endpoint.
 * @param socket
 * @param endpoint
 * @return int
 */
int zmq_api_bind(void *socket, const char *endpoint)
{
    return g_zmq.loaded ? g_zmq.bind(socket, endpoint) : -1;
}

/**
 * @brief Connect a ZeroMQ socket to an endpoint.
 * @param socket
 * @param endpoint
 * @return int
 */
int zmq_api_connect(void *socket, const char *endpoint)
{
    return g_zmq.loaded ? g_zmq.connect(socket, endpoint) : -1;
}

/**
 * @brief Send data on a ZeroMQ socket.
 * @param socket
 * @param buf
 * @param len
 * @param flags
 * @return int
 */
int zmq_api_send(void *socket, const void *buf, size_t len, int flags)
{
    return g_zmq.loaded ? g_zmq.send(socket, buf, len, flags) : -1;
}

/**
 * @brief Receive data on a ZeroMQ socket.
 * @param socket
 * @param buf
 * @param len
 * @param flags
 * @return int
 */
int zmq_api_recv(void *socket, void *buf, size_t len, int flags)
{
    return g_zmq.loaded ? g_zmq.recv(socket, buf, len, flags) : -1;
}

/**
 * @brief Get the last error number.
 * @param void
 * @return int
 */
int zmq_api_errno(void)
{
    return g_zmq.loaded ? g_zmq.errno_fn() : -1;
}

/**
 * @brief Get the last error message.
 * @param errnum
 * @return const char *
 */
const char *zmq_api_strerror(int errnum)
{
    return g_zmq.loaded ? g_zmq.strerror_fn(errnum) : "libzmq not loaded";
}

/**
 * @brief Get the version of the ZeroMQ library.
 * @param major
 * @param minor
 * @param patch
 * @return void
 */
void zmq_api_version(int *major, int *minor, int *patch)
{
    if (!g_zmq.loaded) {
        if (major) {
            *major = 0;
        }

        if (minor) {
            *minor = 0;
        }

        if (patch) {
            *patch = 0;
        }

        return;
    }

    g_zmq.version(major, minor, patch);
}

/**
 * @brief Check if a capability is supported.
 * @param capability
 * @return int
 */
int zmq_api_has(const char *capability)
{
    return g_zmq.loaded ? g_zmq.has(capability) : 0;
}

/**
 * @brief Set a socket option.
 * @param socket
 * @param option
 * @param optval
 * @param optvallen
 * @return int
 */
int zmq_api_setsockopt(void *socket, int option, const void *optval, size_t optvallen)
{
    return g_zmq.loaded ? g_zmq.setsockopt(socket, option, optval, optvallen) : -1;
}

/**
 * @brief Get a socket option.
 * @param socket
 * @param option
 * @param optval
 * @param optvallen
 * @return int
 */
int zmq_api_getsockopt(void *socket, int option, void *optval, size_t *optvallen)
{
    return g_zmq.loaded ? g_zmq.getsockopt(socket, option, optval, optvallen) : -1;
}
