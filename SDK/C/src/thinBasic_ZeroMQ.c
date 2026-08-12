/*
 * thinBasic_ZeroMQ.c — ThinBasic SDK module entry point.
 *
 * Builds: thinBasic_ZeroMQ.dll
 * Usage:  USES "ZeroMQ"
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "thinCore.h"
#include "tb_thincore.h"
#include "zmq_dynload.h"
#include "zmq_enums.h"

/* Exec_* symbols are implemented in tb_zmq_exec.c */
LONG __cdecl Exec_ZmqLibraryInit(void);
LONG __cdecl Exec_ZmqLibraryShutdown(void);
LONG __cdecl Exec_ZmqLibraryLoaded(void);
LONG __cdecl Exec_ZmqErrno(void);
LONG __cdecl Exec_ZmqStrerror(void);
LONG __cdecl Exec_ZmqVersionMajor(void);
LONG __cdecl Exec_ZmqVersionMinor(void);
LONG __cdecl Exec_ZmqVersionPatch(void);
LONG __cdecl Exec_ZmqHas(void);
LONG __cdecl Exec_ZmqCtxNew(void);
LONG __cdecl Exec_ZmqCtxTerm(void);
LONG __cdecl Exec_ZmqCtxShutdown(void);
LONG __cdecl Exec_ZmqSocket(void);
LONG __cdecl Exec_ZmqClose(void);
LONG __cdecl Exec_ZmqBind(void);
LONG __cdecl Exec_ZmqConnect(void);
LONG __cdecl Exec_ZmqSend(void);
LONG __cdecl Exec_ZmqRecv(void);
LONG __cdecl Exec_ZmqSetsockoptInt(void);
LONG __cdecl Exec_ZmqSetsockoptStr(void);
LONG __cdecl Exec_ZmqGetsockoptInt(void);

/**
 * @brief Register a symbol.
 * @param name
 * @param return_type
 * @param fn
 * @return void
 */
static void tb_zmq_register_symbol(const char *name, DWORD return_type, LONG (__cdecl *fn)(void))
{
    tb_LoadSymbol((char *)name, return_type, (void *)fn, thinBasic_ForceOverWrite);
}

/**
 * @brief Register an equate.
 * @param name
 * @param value
 * @return void
 */
static void tb_zmq_register_equate(const char *name, DWORD value)
{
    tb_AddEquate((char *)name, "", value, thinBasic_ConstTypeNumeric);
}

/**
 * @brief Register symbols.
 * @param void
 * @return void
 */
static void tb_zmq_register_symbols(void)
{
    tb_zmq_register_symbol("ZmqLibraryInit", thinBasic_ReturnCodeLong, &Exec_ZmqLibraryInit);
    tb_zmq_register_symbol("ZmqLibraryShutdown", thinBasic_ReturnCodeLong, &Exec_ZmqLibraryShutdown);
    tb_zmq_register_symbol("ZmqLibraryLoaded", thinBasic_ReturnCodeLong, &Exec_ZmqLibraryLoaded);

    tb_zmq_register_symbol("ZmqErrno", thinBasic_ReturnCodeLong, &Exec_ZmqErrno);
    tb_zmq_register_symbol("ZmqStrerror", thinBasic_ReturnCodeLong, &Exec_ZmqStrerror);
    tb_zmq_register_symbol("ZmqVersionMajor", thinBasic_ReturnCodeLong, &Exec_ZmqVersionMajor);
    tb_zmq_register_symbol("ZmqVersionMinor", thinBasic_ReturnCodeLong, &Exec_ZmqVersionMinor);
    tb_zmq_register_symbol("ZmqVersionPatch", thinBasic_ReturnCodeLong, &Exec_ZmqVersionPatch);
    tb_zmq_register_symbol("ZmqHas", thinBasic_ReturnCodeLong, &Exec_ZmqHas);

    tb_zmq_register_symbol("ZmqCtxNew", thinBasic_ReturnCodeLong, &Exec_ZmqCtxNew);
    tb_zmq_register_symbol("ZmqCtxTerm", thinBasic_ReturnCodeLong, &Exec_ZmqCtxTerm);
    tb_zmq_register_symbol("ZmqCtxShutdown", thinBasic_ReturnCodeLong, &Exec_ZmqCtxShutdown);
    tb_zmq_register_symbol("ZmqSocket", thinBasic_ReturnCodeLong, &Exec_ZmqSocket);
    tb_zmq_register_symbol("ZmqClose", thinBasic_ReturnCodeLong, &Exec_ZmqClose);
    tb_zmq_register_symbol("ZmqBind", thinBasic_ReturnCodeLong, &Exec_ZmqBind);
    tb_zmq_register_symbol("ZmqConnect", thinBasic_ReturnCodeLong, &Exec_ZmqConnect);
    tb_zmq_register_symbol("ZmqSend", thinBasic_ReturnCodeLong, &Exec_ZmqSend);
    tb_zmq_register_symbol("ZmqRecv", thinBasic_ReturnCodeLong, &Exec_ZmqRecv);
    tb_zmq_register_symbol("ZmqSetsockoptInt", thinBasic_ReturnCodeLong, &Exec_ZmqSetsockoptInt);
    tb_zmq_register_symbol("ZmqSetsockoptStr", thinBasic_ReturnCodeLong, &Exec_ZmqSetsockoptStr);
    tb_zmq_register_symbol("ZmqGetsockoptInt", thinBasic_ReturnCodeLong, &Exec_ZmqGetsockoptInt);
}

/**
 * @brief Register equates.
 * @param void
 * @return void
 */
static void tb_zmq_register_equates(void)
{
    tb_zmq_register_equate("%ZMQ_PAIR", ZMQ_PAIR);
    tb_zmq_register_equate("%ZMQ_PUB", ZMQ_PUB);
    tb_zmq_register_equate("%ZMQ_SUB", ZMQ_SUB);
    tb_zmq_register_equate("%ZMQ_REQ", ZMQ_REQ);
    tb_zmq_register_equate("%ZMQ_REP", ZMQ_REP);
    tb_zmq_register_equate("%ZMQ_DEALER", ZMQ_DEALER);
    tb_zmq_register_equate("%ZMQ_ROUTER", ZMQ_ROUTER);

    tb_zmq_register_equate("%ZMQ_SUBSCRIBE", ZMQ_SUBSCRIBE);
    tb_zmq_register_equate("%ZMQ_LINGER", ZMQ_LINGER);
    tb_zmq_register_equate("%ZMQ_RCVTIMEO", ZMQ_RCVTIMEO);
    tb_zmq_register_equate("%ZMQ_SNDTIMEO", ZMQ_SNDTIMEO);
    tb_zmq_register_equate("%ZMQ_DONTWAIT", ZMQ_DONTWAIT);
    tb_zmq_register_equate("%ZMQ_SNDMORE", ZMQ_SNDMORE);
}

/**
 * @brief DLL main function.
 * @param instance
 * @param reason
 * @param reserved
 * @return BOOL
 */
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)instance;
    (void)reserved;

    switch (reason) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }

    return TRUE;
}

/**
 * @brief Unload local symbols.
 * @param void
 * @return DWORD
 */
DWORD __declspec(dllexport) __cdecl _UnLoadLocalSymbols(void)
{
    zmq_dyn_unload();
    tb_thincore_shutdown();
    
    return 0;
}

/**
 * @brief Load local symbols.
 * @param app_path
 * @return DWORD
 */
DWORD __declspec(dllexport) __cdecl _LoadLocalSymbols(char *app_path)
{
    (void)app_path;

    if (!tb_thincore_init()) {
        return 0;
    }

    tb_zmq_register_symbols();
    tb_zmq_register_equates();

    return 0;
}
