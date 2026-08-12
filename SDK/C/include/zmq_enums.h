/*
 * zmq_enums.h — ZeroMQ constants registered into ThinBasic via thinBasic_AddEquate.
 *
 * Values match ThinBasicZMQ\Core\Enums.inc (libzmq 4.3.x).
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

/* Common socket options */
#define ZMQ_SUBSCRIBE   6
#define ZMQ_LINGER     17
#define ZMQ_RCVTIMEO   27
#define ZMQ_SNDTIMEO   28

/* Send / recv flags */
#define ZMQ_DONTWAIT 1
#define ZMQ_SNDMORE  2

#endif /* ZMQ_ENUMS_H */
