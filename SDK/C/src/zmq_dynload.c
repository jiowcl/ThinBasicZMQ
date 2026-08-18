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
typedef int   (__cdecl *zmq_ctx_set_fn)(void *, int, int);
typedef int   (__cdecl *zmq_ctx_get_fn)(void *, int);
typedef void *(__cdecl *zmq_socket_fn)(void *, int);
typedef int   (__cdecl *zmq_close_fn)(void *);
typedef int   (__cdecl *zmq_bind_fn)(void *, const char *);
typedef int   (__cdecl *zmq_unbind_fn)(void *, const char *);
typedef int   (__cdecl *zmq_connect_fn)(void *, const char *);
typedef int   (__cdecl *zmq_disconnect_fn)(void *, const char *);
typedef int   (__cdecl *zmq_send_fn)(void *, const void *, size_t, int);
typedef int   (__cdecl *zmq_recv_fn)(void *, void *, size_t, int);
typedef int   (__cdecl *zmq_errno_fn)(void);
typedef const char *(__cdecl *zmq_strerror_fn)(int);
typedef void  (__cdecl *zmq_version_fn)(int *, int *, int *);
typedef int   (__cdecl *zmq_has_fn)(const char *);
typedef int   (__cdecl *zmq_setsockopt_fn)(void *, int, const void *, size_t);
typedef int   (__cdecl *zmq_getsockopt_fn)(void *, int, void *, size_t *);
typedef int   (__cdecl *zmq_curve_keypair_fn)(char *, char *);
typedef int   (__cdecl *zmq_curve_public_fn)(char *, const char *);
typedef char *(__cdecl *zmq_z85_encode_fn)(char *, const unsigned char *, size_t);
typedef unsigned char *(__cdecl *zmq_z85_decode_fn)(unsigned char *, const char *);

typedef struct zmq_api_table {
    HMODULE             module;
    int                 loaded;
    zmq_ctx_new_fn      ctx_new;
    zmq_ctx_term_fn     ctx_term;
    zmq_ctx_shutdown_fn ctx_shutdown;
    zmq_ctx_set_fn      ctx_set;
    zmq_ctx_get_fn      ctx_get;
    zmq_socket_fn       socket;
    zmq_close_fn        close;
    zmq_bind_fn         bind;
    zmq_unbind_fn       unbind;
    zmq_connect_fn      connect;
    zmq_disconnect_fn   disconnect;
    zmq_send_fn         send;
    zmq_recv_fn         recv;
    zmq_errno_fn        errno_fn;
    zmq_strerror_fn     strerror_fn;
    zmq_version_fn      version;
    zmq_has_fn          has;
    zmq_setsockopt_fn   setsockopt;
    zmq_getsockopt_fn   getsockopt;
    zmq_curve_keypair_fn curve_keypair;
    zmq_curve_public_fn  curve_public;
    zmq_z85_encode_fn    z85_encode;
    zmq_z85_decode_fn    z85_decode;
} zmq_api_table;

static zmq_api_table g_zmq;

/**
 * @brief Resolve a libzmq export without a direct FARPROC-to-fn-ptr cast.
 * @param name
 * @return void *
 */
static void *zmq_resolve(const char *name)
{
    FARPROC fn;

    fn = GetProcAddress(g_zmq.module, name);
    if (fn == NULL) {
        return NULL;
    }

    return (void *)(INT_PTR)fn;
}

/**
 * @brief Bind required symbols.
 * @param void
 * @return int
 */
static int zmq_bind_required_symbols(void)
{
    g_zmq.ctx_new = (zmq_ctx_new_fn)zmq_resolve("zmq_ctx_new");
    g_zmq.ctx_term = (zmq_ctx_term_fn)zmq_resolve("zmq_ctx_term");
    g_zmq.ctx_shutdown = (zmq_ctx_shutdown_fn)zmq_resolve("zmq_ctx_shutdown");
    g_zmq.ctx_set = (zmq_ctx_set_fn)zmq_resolve("zmq_ctx_set");
    g_zmq.ctx_get = (zmq_ctx_get_fn)zmq_resolve("zmq_ctx_get");
    g_zmq.socket = (zmq_socket_fn)zmq_resolve("zmq_socket");
    g_zmq.close = (zmq_close_fn)zmq_resolve("zmq_close");
    g_zmq.bind = (zmq_bind_fn)zmq_resolve("zmq_bind");
    g_zmq.unbind = (zmq_unbind_fn)zmq_resolve("zmq_unbind");
    g_zmq.connect = (zmq_connect_fn)zmq_resolve("zmq_connect");
    g_zmq.disconnect = (zmq_disconnect_fn)zmq_resolve("zmq_disconnect");
    g_zmq.send = (zmq_send_fn)zmq_resolve("zmq_send");
    g_zmq.recv = (zmq_recv_fn)zmq_resolve("zmq_recv");
    g_zmq.errno_fn = (zmq_errno_fn)zmq_resolve("zmq_errno");
    g_zmq.strerror_fn = (zmq_strerror_fn)zmq_resolve("zmq_strerror");
    g_zmq.version = (zmq_version_fn)zmq_resolve("zmq_version");
    g_zmq.has = (zmq_has_fn)zmq_resolve("zmq_has");
    g_zmq.setsockopt = (zmq_setsockopt_fn)zmq_resolve("zmq_setsockopt");
    g_zmq.getsockopt = (zmq_getsockopt_fn)zmq_resolve("zmq_getsockopt");
    /* Optional: older builds may lack CURVE / Z85 helpers. */
    g_zmq.curve_keypair = (zmq_curve_keypair_fn)zmq_resolve("zmq_curve_keypair");
    g_zmq.curve_public = (zmq_curve_public_fn)zmq_resolve("zmq_curve_public");
    g_zmq.z85_encode = (zmq_z85_encode_fn)zmq_resolve("zmq_z85_encode");
    g_zmq.z85_decode = (zmq_z85_decode_fn)zmq_resolve("zmq_z85_decode");

    return g_zmq.ctx_new && g_zmq.ctx_term && g_zmq.ctx_shutdown &&
           g_zmq.ctx_set && g_zmq.ctx_get && g_zmq.socket &&
           g_zmq.close && g_zmq.bind && g_zmq.unbind && g_zmq.connect && g_zmq.disconnect &&
           g_zmq.send && g_zmq.recv &&
           g_zmq.errno_fn && g_zmq.strerror_fn && g_zmq.version && g_zmq.has &&
           g_zmq.setsockopt && g_zmq.getsockopt;
}

/**
 * @brief Trim trailing slashes from a bounded C string.
 * @param path
 * @param max_len including space for the terminating NUL
 * @return void
 */
static void zmq_trim_trailing_slashes(char *path, size_t max_len)
{
    size_t len;

    if (path == NULL || max_len == 0) {
        return;
    }

    for (len = 0; len < max_len; len++) {
        if (path[len] == '\0') {
            break;
        }
    }

    if (len >= max_len) {
        path[0] = '\0';
        return;
    }

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
    zmq_trim_trailing_slashes(folder, sizeof(folder));

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
 * @brief Set a context option.
 * @param context
 * @param option
 * @param optval
 * @return int
 */
int zmq_api_ctx_set(void *context, int option, int optval)
{
    return g_zmq.loaded ? g_zmq.ctx_set(context, option, optval) : -1;
}

/**
 * @brief Get a context option.
 * @param context
 * @param option
 * @return int
 */
int zmq_api_ctx_get(void *context, int option)
{
    return g_zmq.loaded ? g_zmq.ctx_get(context, option) : -1;
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
 * @brief Unbind a ZeroMQ socket from an endpoint.
 * @param socket
 * @param endpoint
 * @return int
 */
int zmq_api_unbind(void *socket, const char *endpoint)
{
    return g_zmq.loaded ? g_zmq.unbind(socket, endpoint) : -1;
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
 * @brief Disconnect a ZeroMQ socket from an endpoint.
 * @param socket
 * @param endpoint
 * @return int
 */
int zmq_api_disconnect(void *socket, const char *endpoint)
{
    return g_zmq.loaded ? g_zmq.disconnect(socket, endpoint) : -1;
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

/**
 * @brief Generate a CURVE keypair (Z85, 40 chars + NUL in each buffer).
 * @param z85_public
 * @param z85_secret
 * @return int
 */
int zmq_api_curve_keypair(char *z85_public, char *z85_secret)
{
    if (!g_zmq.loaded || g_zmq.curve_keypair == NULL || z85_public == NULL || z85_secret == NULL) {
        return -1;
    }

    return g_zmq.curve_keypair(z85_public, z85_secret);
}

/**
 * @brief Derive a CURVE public key from a Z85 secret.
 * @param z85_public
 * @param z85_secret
 * @return int
 */
int zmq_api_curve_public(char *z85_public, const char *z85_secret)
{
    if (!g_zmq.loaded || g_zmq.curve_public == NULL || z85_public == NULL || z85_secret == NULL) {
        return -1;
    }

    return g_zmq.curve_public(z85_public, z85_secret);
}

/**
 * @brief Encode binary data as Z85.
 * @param dest
 * @param data
 * @param size
 * @return char *
 */
char *zmq_api_z85_encode(char *dest, const void *data, size_t size)
{
    if (!g_zmq.loaded || g_zmq.z85_encode == NULL || dest == NULL || data == NULL) {
        return NULL;
    }

    return g_zmq.z85_encode(dest, (const unsigned char *)data, size);
}

/**
 * @brief Decode Z85 text into binary.
 * @param dest
 * @param string
 * @return unsigned char *
 */
unsigned char *zmq_api_z85_decode(unsigned char *dest, const char *string)
{
    if (!g_zmq.loaded || g_zmq.z85_decode == NULL || dest == NULL || string == NULL) {
        return NULL;
    }

    return g_zmq.z85_decode(dest, string);
}
