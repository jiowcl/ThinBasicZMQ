/*
 * thinCore.h — ThinBasic SDK constants (excerpt).
 *
 * Runtime bindings live in tb_thincore.c (see SDK/C/README.md “ABI contract”).
 * Do not call these names through this header: ParseString is As Ext (ST(0)),
 * AddEquate/LoadSymbol used here are the cdecl _thinBasic_* exports, and
 * ParseDouble is unused (keywords parse with ParseLong).
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

/* Function prototypes are intentionally omitted. Wrong C types here caused AVs
 * (ParseString as DWORD, ParseDouble returning double). Use tb_thincore.h. */

#endif /* THINCORE_H */
