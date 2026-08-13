/*
 * tb_thincore.h — Runtime binding to thinCore.dll (hybrid cdecl / stdcall).
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#ifndef TB_THINCORE_H
#define TB_THINCORE_H

#include <windows.h>

#include "thinCore.h"

int   tb_thincore_init(void);
void  tb_thincore_shutdown(void);

DWORD tb_AddEquate(char *szEquate, char *szStringValue, DWORD dwNumericValue, DWORD dwConstType);
DWORD tb_LoadSymbol(char *szFunctionName, DWORD dwReturnType, void *FunctionCode, DWORD dwForceOverWrite);
DWORD tb_LoadSymbolFB(char *szFunctionName, DWORD dwReturnType, void *FunctionCode, DWORD dwForceOverWrite);
void  tb_ParseLong(LONG *result);
void  tb_ParseString(char **pszString);
DWORD tb_CheckOpenParens(DWORD HideError, DWORD AutoPutBack);
DWORD tb_CheckComma(DWORD HideError, DWORD AutoPutBack);
DWORD tb_CheckCloseParens(DWORD HideError, DWORD AutoPutBack);
void *tb_return_string(const char *sz);

#endif /* TB_THINCORE_H */
