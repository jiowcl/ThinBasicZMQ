/*
 * tb_zmq_exec.c — ThinBasic keyword executors for the ZeroMQ SDK module.
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#define WIN32_LEAN_AND_MEAN
#include "tb_parse.h"
#include "zmq_dynload.h"
#include "zmq_enums.h"

#include <stdint.h>
#include <string.h>

/**
 * @brief Convert a double handle to a void pointer.
 * @param handle
 * @return void *
 */
static void *tb_ptr_from_handle(LONG handle)
{
    return (void *)(intptr_t)handle;
}

/**
 * @brief Convert a void pointer to a double handle.
 * @param ptr
 * @return double
 */
static LONG tb_handle_from_ptr(void *ptr)
{
    return (LONG)(intptr_t)ptr;
}

/**
 * @brief Initialize the ZeroMQ library.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqLibraryInit(void)
{
    char *path;
    DWORD len;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_string(&path, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_dyn_load_directory(path, len);
}

/**
 * @brief Shutdown the ZeroMQ library.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqLibraryShutdown(void)
{
    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    zmq_dyn_unload();
    
    return 1;
}

/**
 * @brief Check if the ZeroMQ library is loaded.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqLibraryLoaded(void)
{
    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_dyn_is_loaded();
}

/**
 * @brief Get the last error number.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqErrno(void)
{
    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_errno();
}

/**
 * @brief Get the last error message.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqStrerror(void)
{
    LONG  errnum;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&errnum)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)(intptr_t)zmq_api_strerror((int)errnum);
}

/**
 * @brief Get the last error message as a ThinBasic String (BSTR).
 * @param void
 * @return byte-BSTR in EAX (FreeBASIC LoadSymbol_FB convention)
 */
void * __stdcall Exec_ZmqStrerrorString(void)
{
    LONG errnum;
    const char *msg;

    if (!tb_expect_open_parens()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&errnum)) {
        return tb_return_string("");
    }

    if (!tb_expect_close_parens()) {
        return tb_return_string("");
    }

    msg = zmq_api_strerror((int)errnum);
    
    return tb_return_string(msg);
}

/**
 * @brief Get the major version of the ZeroMQ library.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqVersionMajor(void)
{
    int major = 0;
    int minor = 0;
    int patch = 0;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    zmq_api_version(&major, &minor, &patch);

    return (LONG)major;
}

/**
 * @brief Get the minor version of the ZeroMQ library.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqVersionMinor(void)
{
    int major = 0;
    int minor = 0;
    int patch = 0;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    zmq_api_version(&major, &minor, &patch);

    return (LONG)minor;
}

/**
 * @brief Get the patch version of the ZeroMQ library.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqVersionPatch(void)
{
    int major = 0;
    int minor = 0;
    int patch = 0;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    zmq_api_version(&major, &minor, &patch);
    
    return (LONG)patch;
}

/**
 * @brief Check if a capability is supported.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqHas(void)
{
    char *capability;
    DWORD len;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_string(&capability, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_has(capability);
}

/**
 * @brief Create a new ZeroMQ context.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqCtxNew(void)
{
    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return tb_handle_from_ptr(zmq_api_ctx_new());
}

/**
 * @brief Terminate a ZeroMQ context.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqCtxTerm(void)
{
    LONG  ctx;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&ctx)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_ctx_term(tb_ptr_from_handle(ctx));
}

/**
 * @brief Shutdown a ZeroMQ context.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqCtxShutdown(void)
{
    LONG  ctx;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&ctx)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_ctx_shutdown(tb_ptr_from_handle(ctx));
}

/**
 * @brief Create a new ZeroMQ socket.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqSocket(void)
{
    LONG  ctx;
    LONG  type;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&ctx)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&type)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return tb_handle_from_ptr(zmq_api_socket(tb_ptr_from_handle(ctx), (int)type));
}

/**
 * @brief Close a ZeroMQ socket.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqClose(void)
{
    LONG  socket;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_close(tb_ptr_from_handle(socket));
}

/**
 * @brief Bind a ZeroMQ socket to an endpoint.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqBind(void)
{
    LONG  socket;
    char *endpoint;
    DWORD len;
    char endpoint_copy[MAX_PATH];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&endpoint, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len >= sizeof(endpoint_copy)) {
        return -1;
    }

    memcpy(endpoint_copy, endpoint, len);
    endpoint_copy[len] = '\0';

    return (LONG)zmq_api_bind(tb_ptr_from_handle(socket), endpoint_copy);
}

/**
 * @brief Connect a ZeroMQ socket to an endpoint.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqConnect(void)
{
    LONG  socket;
    char *endpoint;
    DWORD len;
    char endpoint_copy[MAX_PATH];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&endpoint, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len >= sizeof(endpoint_copy)) {
        return -1;
    }

    memcpy(endpoint_copy, endpoint, len);
    endpoint_copy[len] = '\0';

    return (LONG)zmq_api_connect(tb_ptr_from_handle(socket), endpoint_copy);
}

/**
 * @brief Unbind a ZeroMQ socket from an endpoint.
 * @param void
 * @return LONG
 */
LONG __cdecl Exec_ZmqUnbind(void)
{
    LONG  socket;
    char *endpoint;
    DWORD len;
    char endpoint_copy[MAX_PATH];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&endpoint, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len >= sizeof(endpoint_copy)) {
        return -1;
    }

    memcpy(endpoint_copy, endpoint, len);
    endpoint_copy[len] = '\0';

    return (LONG)zmq_api_unbind(tb_ptr_from_handle(socket), endpoint_copy);
}

/**
 * @brief Disconnect a ZeroMQ socket from an endpoint.
 * @param void
 * @return LONG
 */
LONG __cdecl Exec_ZmqDisconnect(void)
{
    LONG  socket;
    char *endpoint;
    DWORD len;
    char endpoint_copy[MAX_PATH];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&endpoint, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len >= sizeof(endpoint_copy)) {
        return -1;
    }

    memcpy(endpoint_copy, endpoint, len);
    endpoint_copy[len] = '\0';

    return (LONG)zmq_api_disconnect(tb_ptr_from_handle(socket), endpoint_copy);
}

/**
 * @brief Send data on a ZeroMQ socket.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqSend(void)
{
    LONG  socket;
    LONG  buf;
    LONG  buflen;
    LONG  flags;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&buf)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&buflen)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&flags)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_send(
        tb_ptr_from_handle(socket),
        tb_ptr_from_handle(buf),
        (size_t)buflen,
        (int)flags);
}

/**
 * @brief Receive data on a ZeroMQ socket.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqRecv(void)
{
    LONG  socket;
    LONG  buf;
    LONG  buflen;
    LONG  flags;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&buf)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&buflen)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&flags)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    return (LONG)zmq_api_recv(
        tb_ptr_from_handle(socket),
        tb_ptr_from_handle(buf),
        (size_t)buflen,
        (int)flags);
}

/**
 * @brief Send a ThinBasic String on a ZeroMQ socket.
 * @param void
 * @return bytes sent, or -1
 */
LONG __cdecl Exec_ZmqSendStr(void)
{
    LONG  socket;
    LONG  flags;
    char *msg;
    DWORD len;
    char  copy[TB_RETURN_STRING_MAX];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&msg, &len)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&flags)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len > sizeof(copy)) {
        return -1;
    }

    if (len > 0) {
        memcpy(copy, msg, len);
    }

    return (LONG)zmq_api_send(
        tb_ptr_from_handle(socket),
        copy,
        (size_t)len,
        (int)flags);
}

/**
 * @brief Receive a message as a ThinBasic String (BSTR).
 * @param void
 * @return byte-BSTR in EAX (FreeBASIC LoadSymbol_FB convention)
 *
 * Empty string on parse failure, timeout/error, or a zero-length frame.
 * maxLen is capped at TB_RETURN_STRING_MAX (4096).
 */
void * __stdcall Exec_ZmqRecvStr(void)
{
    LONG socket;
    LONG max_len;
    LONG flags;
    char buf[TB_RETURN_STRING_MAX];
    int  n;
    size_t copied;

    if (!tb_expect_open_parens()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&socket)) {
        return tb_return_string("");
    }

    if (!tb_expect_comma()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&max_len)) {
        return tb_return_string("");
    }

    if (!tb_expect_comma()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&flags)) {
        return tb_return_string("");
    }

    if (!tb_expect_close_parens()) {
        return tb_return_string("");
    }

    if (max_len <= 0) {
        return tb_return_string("");
    }

    if (max_len > (LONG)TB_RETURN_STRING_MAX) {
        max_len = (LONG)TB_RETURN_STRING_MAX;
    }

    n = zmq_api_recv(
        tb_ptr_from_handle(socket),
        buf,
        (size_t)max_len,
        (int)flags);

    if (n < 0) {
        return tb_return_string("");
    }

    copied = (size_t)n;
    if (copied > (size_t)max_len) {
        copied = (size_t)max_len;
    }

    return tb_return_string_n(buf, copied);
}

/**
 * @brief Set a socket option.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqSetsockoptInt(void)
{
    LONG  socket;
    LONG  option;
    LONG  value;
    int32_t optval;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&option)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&value)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    optval = (int32_t)value;

    return (LONG)zmq_api_setsockopt(
        tb_ptr_from_handle(socket),
        (int)option,
        &optval,
        sizeof(optval));
}

/**
 * @brief Set a socket option.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqSetsockoptStr(void)
{
    LONG  socket;
    LONG  option;
    char *value;
    DWORD len;
    char value_copy[256];

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&option)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&value, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (len >= sizeof(value_copy)) {
        return -1;
    }

    memcpy(value_copy, value, len);
    value_copy[len] = '\0';

    return (LONG)zmq_api_setsockopt(
        tb_ptr_from_handle(socket),
        (int)option,
        value_copy,
        len);
}

/**
 * @brief Get a socket option.
 * @param void
 * @return double
 */
LONG __cdecl Exec_ZmqGetsockoptInt(void)
{
    LONG  socket;
    LONG  option;
    int32_t optval = 0;
    size_t optlen = sizeof(optval);
    int rc;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&socket)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&option)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    rc = zmq_api_getsockopt(tb_ptr_from_handle(socket), (int)option, &optval, &optlen);
    
    if (rc != 0) {
        return -1;
    }

    return (LONG)optval;
}

/**
 * @brief Generate a CURVE Z85 keypair into two caller buffers.
 * @param void
 * @return LONG 0 on success, -1 on failure
 */
LONG __cdecl Exec_ZmqCurveKeypair(void)
{
    LONG public_buf;
    LONG secret_buf;

    if (!tb_expect_open_parens()) {
        return -1;
    }

    if (!tb_parse_int(&public_buf)) {
        return -1;
    }

    if (!tb_expect_comma()) {
        return -1;
    }

    if (!tb_parse_int(&secret_buf)) {
        return -1;
    }

    if (!tb_expect_close_parens()) {
        return -1;
    }

    if (public_buf == 0 || secret_buf == 0) {
        return -1;
    }

    return (LONG)zmq_api_curve_keypair(
        (char *)tb_ptr_from_handle(public_buf),
        (char *)tb_ptr_from_handle(secret_buf));
}

/**
 * @brief Derive a CURVE public Z85 key from a secret string.
 * @param void
 * @return LONG 0 on success, -1 on failure
 */
LONG __cdecl Exec_ZmqCurvePublic(void)
{
    LONG  public_buf;
    char *secret;
    DWORD len;
    char secret_copy[ZMQ_CURVE_KEYSIZE_Z85 + 1];

    if (!tb_expect_open_parens()) {
        return -1;
    }

    if (!tb_parse_int(&public_buf)) {
        return -1;
    }

    if (!tb_expect_comma()) {
        return -1;
    }

    if (!tb_parse_string(&secret, &len)) {
        return -1;
    }

    if (!tb_expect_close_parens()) {
        return -1;
    }

    if (public_buf == 0 || len != (DWORD)ZMQ_CURVE_KEYSIZE_Z85) {
        return -1;
    }

    memcpy(secret_copy, secret, len);
    secret_copy[len] = '\0';

    return (LONG)zmq_api_curve_public(
        (char *)tb_ptr_from_handle(public_buf),
        secret_copy);
}

/**
 * @brief Encode binary data as Z85 into a caller buffer.
 * @param void
 * @return LONG dest pointer on success, 0 on failure
 */
LONG __cdecl Exec_ZmqZ85Encode(void)
{
    LONG  dest;
    LONG  data;
    LONG  size;
    char *out;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&dest)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&data)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&size)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (dest == 0 || data == 0 || size <= 0 || (size & 3) != 0) {
        return 0;
    }

    out = zmq_api_z85_encode(
        (char *)tb_ptr_from_handle(dest),
        tb_ptr_from_handle(data),
        (size_t)size);

    return (LONG)(intptr_t)out;
}

/**
 * @brief Decode Z85 text into a caller buffer.
 * @param void
 * @return LONG dest pointer on success, 0 on failure
 */
LONG __cdecl Exec_ZmqZ85Decode(void)
{
    LONG  dest;
    char *encoded;
    DWORD len;
    char encoded_copy[256];
    unsigned char *out;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_int(&dest)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_string(&encoded, &len)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (dest == 0 || len == 0 || (len % 5) != 0 || len >= sizeof(encoded_copy)) {
        return 0;
    }

    memcpy(encoded_copy, encoded, len);
    encoded_copy[len] = '\0';

    out = zmq_api_z85_decode(
        (unsigned char *)tb_ptr_from_handle(dest),
        encoded_copy);

    return (LONG)(intptr_t)out;
}

/**
 * @brief Encode binary data as a ThinBasic Z85 String (BSTR).
 * @param void
 * @return byte-BSTR in EAX; empty on failure
 *
 * Aligns with Native ZmqZ85EncodeStr(pData, nLen). size must be a multiple of 4.
 * Encoded length is capped so the result fits TB_RETURN_STRING_MAX.
 */
void * __stdcall Exec_ZmqZ85EncodeStr(void)
{
    LONG  data;
    LONG  size;
    char  dest[TB_RETURN_STRING_MAX];
    size_t encoded_len;
    char *out;

    if (!tb_expect_open_parens()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&data)) {
        return tb_return_string("");
    }

    if (!tb_expect_comma()) {
        return tb_return_string("");
    }

    if (!tb_parse_int(&size)) {
        return tb_return_string("");
    }

    if (!tb_expect_close_parens()) {
        return tb_return_string("");
    }

    if (data == 0 || size <= 0 || (size & 3) != 0) {
        return tb_return_string("");
    }

    encoded_len = ((size_t)size * 5) / 4;
    if (encoded_len == 0 || encoded_len >= TB_RETURN_STRING_MAX) {
        return tb_return_string("");
    }

    dest[0] = '\0';
    out = zmq_api_z85_encode(dest, tb_ptr_from_handle(data), (size_t)size);
    if (out == NULL) {
        return tb_return_string("");
    }

    return tb_return_string(dest);
}

/**
 * @brief Decode Z85 text into a caller buffer (Native ZmqZ85DecodeStr).
 * @param void
 * @return LONG dest pointer on success, 0 on failure
 */
LONG __cdecl Exec_ZmqZ85DecodeStr(void)
{
    char *encoded;
    DWORD len;
    LONG  dest;
    LONG  dest_size;
    DWORD need;
    char  encoded_copy[TB_RETURN_STRING_MAX];
    unsigned char *out;

    if (!tb_expect_open_parens()) {
        return 0;
    }

    if (!tb_parse_string(&encoded, &len)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&dest)) {
        return 0;
    }

    if (!tb_expect_comma()) {
        return 0;
    }

    if (!tb_parse_int(&dest_size)) {
        return 0;
    }

    if (!tb_expect_close_parens()) {
        return 0;
    }

    if (dest == 0 || dest_size <= 0 || len == 0 || (len % 5) != 0 ||
        len >= sizeof(encoded_copy)) {
        return 0;
    }

    need = (len * 4) / 5;
    if (need == 0 || (DWORD)dest_size < need) {
        return 0;
    }

    memcpy(encoded_copy, encoded, len);
    encoded_copy[len] = '\0';

    out = zmq_api_z85_decode(
        (unsigned char *)tb_ptr_from_handle(dest),
        encoded_copy);

    return (LONG)(intptr_t)out;
}

/**
 * @brief Derive a CURVE public Z85 key as a ThinBasic String (BSTR).
 * @param void
 * @return byte-BSTR in EAX; empty on failure
 *
 * Aligns with Native ZmqCurvePublicStr(z85Secret). Secret must be 40 Z85 chars.
 */
void * __stdcall Exec_ZmqCurvePublicStr(void)
{
    char *secret;
    DWORD len;
    char  secret_copy[ZMQ_CURVE_KEYSIZE_Z85 + 1];
    char  public_buf[ZMQ_CURVE_KEYSIZE_Z85 + 1];

    if (!tb_expect_open_parens()) {
        return tb_return_string("");
    }

    if (!tb_parse_string(&secret, &len)) {
        return tb_return_string("");
    }

    if (!tb_expect_close_parens()) {
        return tb_return_string("");
    }

    if (len != (DWORD)ZMQ_CURVE_KEYSIZE_Z85) {
        return tb_return_string("");
    }

    memcpy(secret_copy, secret, len);
    secret_copy[len] = '\0';
    public_buf[0] = '\0';

    if (zmq_api_curve_public(public_buf, secret_copy) != 0) {
        return tb_return_string("");
    }

    public_buf[ZMQ_CURVE_KEYSIZE_Z85] = '\0';
    
    return tb_return_string(public_buf);
}

/**
 * @brief Set a context option.
 * @param void
 * @return LONG
 */
LONG __cdecl Exec_ZmqCtxSet(void)
{
    LONG ctx;
    LONG option;
    LONG value;

    if (!tb_expect_open_parens()) {
        return -1;
    }

    if (!tb_parse_int(&ctx)) {
        return -1;
    }

    if (!tb_expect_comma()) {
        return -1;
    }

    if (!tb_parse_int(&option)) {
        return -1;
    }

    if (!tb_expect_comma()) {
        return -1;
    }

    if (!tb_parse_int(&value)) {
        return -1;
    }

    if (!tb_expect_close_parens()) {
        return -1;
    }

    return (LONG)zmq_api_ctx_set(tb_ptr_from_handle(ctx), (int)option, (int)value);
}

/**
 * @brief Get a context option.
 * @param void
 * @return LONG
 */
LONG __cdecl Exec_ZmqCtxGet(void)
{
    LONG ctx;
    LONG option;

    if (!tb_expect_open_parens()) {
        return -1;
    }

    if (!tb_parse_int(&ctx)) {
        return -1;
    }

    if (!tb_expect_comma()) {
        return -1;
    }

    if (!tb_parse_int(&option)) {
        return -1;
    }

    if (!tb_expect_close_parens()) {
        return -1;
    }

    return (LONG)zmq_api_ctx_get(tb_ptr_from_handle(ctx), (int)option);
}
