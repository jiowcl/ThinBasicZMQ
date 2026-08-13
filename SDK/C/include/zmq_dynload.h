/*
 * zmq_dynload.h — Runtime loader for libzmq.dll (x86).
 *
 * the SDK module owns DLL discovery so scripts do not need LibPath.inc.
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#ifndef ZMQ_DYNLOAD_H
#define ZMQ_DYNLOAD_H

#include <windows.h>

int  zmq_dyn_load_directory(const char *dir, DWORD dir_len);
void zmq_dyn_unload(void);
int  zmq_dyn_is_loaded(void);

void       *zmq_api_ctx_new(void);
int         zmq_api_ctx_term(void *context);
int         zmq_api_ctx_shutdown(void *context);
int         zmq_api_ctx_set(void *context, int option, int optval);
int         zmq_api_ctx_get(void *context, int option);
void       *zmq_api_socket(void *context, int type);
int         zmq_api_close(void *socket);
int         zmq_api_bind(void *socket, const char *endpoint);
int         zmq_api_unbind(void *socket, const char *endpoint);
int         zmq_api_connect(void *socket, const char *endpoint);
int         zmq_api_disconnect(void *socket, const char *endpoint);
int         zmq_api_send(void *socket, const void *buf, size_t len, int flags);
int         zmq_api_recv(void *socket, void *buf, size_t len, int flags);
int         zmq_api_errno(void);
const char *zmq_api_strerror(int errnum);
void        zmq_api_version(int *major, int *minor, int *patch);
int         zmq_api_has(const char *capability);
int         zmq_api_setsockopt(void *socket, int option, const void *optval, size_t optvallen);
int         zmq_api_getsockopt(void *socket, int option, void *optval, size_t *optvallen);
int         zmq_api_curve_keypair(char *z85_public, char *z85_secret);
int         zmq_api_curve_public(char *z85_public, const char *z85_secret);
char       *zmq_api_z85_encode(char *dest, const void *data, size_t size);
unsigned char *zmq_api_z85_decode(unsigned char *dest, const char *string);

#endif /* ZMQ_DYNLOAD_H */
