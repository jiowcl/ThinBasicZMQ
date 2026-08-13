/*
 * zmq_enums.h — ZeroMQ constants registered into ThinBasic via thinBasic_AddEquate.
 *
 * Values match ThinBasicZMQ\Core\Enums.inc (libzmq 4.3.x).
 * SDK registers a practical subset (socket types, common opts, PLAIN/CURVE).
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#ifndef ZMQ_ENUMS_H
#define ZMQ_ENUMS_H

/* Socket types */
#define ZMQ_PAIR   0
#define ZMQ_PUB    1
#define ZMQ_SUB    2
#define ZMQ_REQ    3
#define ZMQ_REP    4
#define ZMQ_DEALER 5
#define ZMQ_ROUTER 6
#define ZMQ_PULL   7
#define ZMQ_PUSH   8
#define ZMQ_XPUB   9
#define ZMQ_XSUB   10
#define ZMQ_STREAM 11

/* Common socket options */
#define ZMQ_SUBSCRIBE      6
#define ZMQ_UNSUBSCRIBE    7
#define ZMQ_LINGER        17
#define ZMQ_RCVTIMEO      27
#define ZMQ_SNDTIMEO      28
#define ZMQ_MECHANISM     43
#define ZMQ_PLAIN_SERVER  44
#define ZMQ_PLAIN_USERNAME 45
#define ZMQ_PLAIN_PASSWORD 46
#define ZMQ_CURVE_SERVER  47
#define ZMQ_CURVE_PUBLICKEY 48
#define ZMQ_CURVE_SECRETKEY 49
#define ZMQ_CURVE_SERVERKEY 50
#define ZMQ_ZAP_DOMAIN    55

/* Send / recv flags */
#define ZMQ_DONTWAIT 1
#define ZMQ_SNDMORE  2

/* Security mechanisms (ZMQ_MECHANISM values) */
#define ZMQ_NULL   0
#define ZMQ_PLAIN  1
#define ZMQ_CURVE  2

/* CURVE key sizes */
#define ZMQ_CURVE_KEYSIZE      32
#define ZMQ_CURVE_KEYSIZE_Z85  40

/* Context options */
#define ZMQ_IO_THREADS   1
#define ZMQ_MAX_SOCKETS  2

#define ZMQ_IO_THREADS_DFLT  1
#define ZMQ_MAX_SOCKETS_DFLT 1023

#endif /* ZMQ_ENUMS_H */
