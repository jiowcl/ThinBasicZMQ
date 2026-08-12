/*
 * thinCore.h — ThinBasic SDK interface (excerpt).
 *
 * Pelles C / thinCore.dll uses __stdcall exports (thinBasic_*).
 * MinGW thinCore.lib uses __cdecl exports (_thinBasic_*); use build.bat path for that.
 *
 * Copyright (c) Eros Olmi / ThinBasic project.
 * Copyright (c) Ji-Feng Tsai. MIT License.
 */

#ifndef THINCORE_H
#define THINCORE_H

#include <windows.h>

struct tagVARIANT;
typedef struct tagVARIANT VARIANT;
typedef VARIANT *LPVARIANT;

#define thinBasic_DoNotForceOverWrite      0
#define thinBasic_ForceOverWrite           1

#define thinBasic_ReturnNone               0
#define thinBasic_ReturnNumber            20
#define thinBasic_ReturnString            30
#define thinBasic_ReturnVariant           50
#define thinBasic_ReturnUDT               60

#define thinBasic_ReturnCodeByte           1
#define thinBasic_ReturnCodeInteger        2
#define thinBasic_ReturnCodeWord           3
#define thinBasic_ReturnCodeDWord          4
#define thinBasic_ReturnCodeLong           5
#define thinBasic_ReturnCodeQuad           6
#define thinBasic_ReturnCodeSingle         7
#define thinBasic_ReturnCodeDouble         8
#define thinBasic_ReturnCodeCurrency       9
#define thinBasic_ReturnCodeExt           10

#define thinBasic_ConstTypeAuto            0
#define thinBasic_ConstTypeNumeric         3
#define thinBasic_ConstTypeString          5

#define thinBasic_TRUE                     1
#define thinBasic_FALSE                    0
#define thinBasic_ON                       1
#define thinBasic_OFF                      0

DWORD thinBasic_AddEquate(char *szEquate, char *szStringValue, DWORD dwNumericValue, DWORD dwConstType);
DWORD thinBasic_LoadSymbol(char *szFunctionName, DWORD dwReturnType, void *FunctionCode, DWORD dwForceOverWrite);
double thinBasic_ParseDouble(void);
DWORD thinBasic_ParseString(char **pszString);
DWORD thinBasic_CheckOpenParens(DWORD HideError, DWORD AutoPutBack);
DWORD thinBasic_CheckComma(DWORD HideError, DWORD AutoPutBack);
DWORD thinBasic_CheckCloseParens(DWORD HideError, DWORD AutoPutBack);

#endif /* THINCORE_H */
