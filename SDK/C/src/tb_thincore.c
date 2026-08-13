/*
 * tb_thincore.c — Load thinCore.dll exports at runtime.
 *
 * Registration: cdecl `_thinBasic_LoadSymbol` / `_thinBasic_AddEquate`
 * Parsing: stdcall `thinBasic_*` (PowerBasic/FreeBASIC SDK declarations)
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#define WIN32_LEAN_AND_MEAN
#include "tb_thincore.h"

#include <stddef.h>
#include <string.h>

typedef DWORD (__cdecl *tb_add_equate_fn)(char *, char *, DWORD, DWORD);
typedef DWORD (__cdecl *tb_load_symbol_fn)(char *, DWORD, void *, DWORD);
typedef DWORD (__stdcall *tb_load_symbol_fb_fn)(char *, DWORD, void *, DWORD);
typedef void  (__stdcall *tb_parse_long_fn)(LONG *);
/* Official SDK: Function thinBasic_ParseString(...) As Ext — value is in ST(0). */
typedef double (__stdcall *tb_parse_string_fn)(char **);
typedef DWORD (__stdcall *tb_check_parens_fn)(DWORD, DWORD);
typedef DWORD (__stdcall *tb_check_comma_fn)(DWORD, DWORD);
/* OLE byte-BSTR used by thinBasic_ReturnString (see FreeBASIC FBGFX sample). */
typedef void *(__stdcall *tb_sys_alloc_string_byte_len_fn)(const char *, UINT);

typedef struct tb_thincore_api {
    HMODULE              module;
    int                  loaded;
    tb_add_equate_fn     add_equate;
    tb_load_symbol_fn    load_symbol;
    tb_load_symbol_fb_fn load_symbol_fb;
    tb_parse_long_fn     parse_long;
    tb_parse_string_fn   parse_string;
    tb_check_parens_fn   check_open_parens;
    tb_check_parens_fn   check_close_parens;
    tb_check_comma_fn    check_comma;
    tb_sys_alloc_string_byte_len_fn alloc_bstr;
} tb_thincore_api;

static tb_thincore_api g_tb;

/**
 * @brief Resolve an export.
 * @param name: The name of the export.
 * @return The address of the export.
 */
static void *tb_resolve_export(const char *name)
{
    FARPROC fn;

    fn = GetProcAddress(g_tb.module, name);
    
    if (fn == NULL) {
        return NULL;
    }

    return (void *)(INT_PTR)fn;
}

/**
 * @brief Initialize the thinCore.dll.
 * @return 1 if the thinCore.dll is initialized, 0 otherwise.
 */
int tb_thincore_init(void)
{
    if (g_tb.loaded) {
        return 1;
    }

    g_tb.module = GetModuleHandleA("thinCore.dll");

    if (g_tb.module == NULL) {
        g_tb.module = LoadLibraryA("thinCore.dll");
    }

    if (g_tb.module == NULL) {
        return 0;
    }

    g_tb.add_equate = (tb_add_equate_fn)tb_resolve_export("_thinBasic_AddEquate");
    g_tb.load_symbol = (tb_load_symbol_fn)tb_resolve_export("_thinBasic_LoadSymbol");
    g_tb.load_symbol_fb = (tb_load_symbol_fb_fn)tb_resolve_export("thinBasic_LoadSymbol_FB");
    g_tb.parse_long = (tb_parse_long_fn)tb_resolve_export("thinBasic_ParseLong");
    g_tb.parse_string = (tb_parse_string_fn)tb_resolve_export("thinBasic_ParseString");
    g_tb.check_open_parens = (tb_check_parens_fn)tb_resolve_export("thinBasic_CheckOpenParens");
    g_tb.check_close_parens = (tb_check_parens_fn)tb_resolve_export("thinBasic_CheckCloseParens");
    g_tb.check_comma = (tb_check_comma_fn)tb_resolve_export("thinBasic_CheckComma");

    {
        HMODULE ole;

        ole = GetModuleHandleA("oleaut32.dll");
        
        if (ole == NULL) {
            ole = LoadLibraryA("oleaut32.dll");
        }
        
        if (ole != NULL) {
            g_tb.alloc_bstr = (tb_sys_alloc_string_byte_len_fn)(void *)(INT_PTR)GetProcAddress(ole, "SysAllocStringByteLen");
        }
    }

    if (g_tb.add_equate == NULL || g_tb.load_symbol == NULL || g_tb.load_symbol_fb == NULL ||
        g_tb.parse_long == NULL || g_tb.parse_string == NULL || g_tb.check_open_parens == NULL ||
        g_tb.check_close_parens == NULL || g_tb.check_comma == NULL) {

        tb_thincore_shutdown();

        return 0;
    }

    g_tb.loaded = 1;
    
    return 1;
}

/**
 * @brief Shutdown the thinCore.dll.
 * @return void
 */
void tb_thincore_shutdown(void)
{
    if (g_tb.module != NULL && g_tb.module != GetModuleHandleA("thinCore.dll")) {
        FreeLibrary(g_tb.module);
    }

    memset(&g_tb, 0, sizeof(g_tb));
}

/**
 * @brief Add an equate.
 * @param szEquate: The name of the equate.
 * @param szStringValue: The value of the equate.
 * @param dwNumericValue: The numeric value of the equate.
 * @param dwConstType: The type of the equate.
 * @return 1 if the equate is added, 0 otherwise.
 */
DWORD tb_AddEquate(char *szEquate, char *szStringValue, DWORD dwNumericValue, DWORD dwConstType)
{
    return g_tb.add_equate(szEquate, szStringValue, dwNumericValue, dwConstType);
}

/**
 * @brief Load a symbol.
 * @param szFunctionName: The name of the function to load.
 * @param dwReturnType: The return type of the function.
 * @param FunctionCode: The code of the function.
 * @param dwForceOverWrite: If 1, force overwrite the function.
 * @return 1 if the function is loaded, 0 otherwise.
 */
DWORD tb_LoadSymbol(char *szFunctionName, DWORD dwReturnType, void *FunctionCode, DWORD dwForceOverWrite)
{
    return g_tb.load_symbol(szFunctionName, dwReturnType, FunctionCode, dwForceOverWrite);
}

/**
 * @brief Load a symbol with the FreeBASIC calling convention.
 * @param szFunctionName
 * @param dwReturnType
 * @param FunctionCode stdcall executor (BSTR in EAX for ReturnString)
 * @param dwForceOverWrite
 * @return symbol id, or 0
 */
DWORD tb_LoadSymbolFB(char *szFunctionName, DWORD dwReturnType, void *FunctionCode, DWORD dwForceOverWrite)
{
    return g_tb.load_symbol_fb(szFunctionName, dwReturnType, FunctionCode, dwForceOverWrite);
}

/**
 * @brief Parse a long.
 * @param result: The long to parse.
 * @return 1 if the long is valid, 0 otherwise.
 */
void tb_ParseLong(LONG *result)
{
    g_tb.parse_long(result);
}

/**
 * @brief Parse a string.
 * @param pszString: Receives a pointer to the parsed text.
 * @return void
 *
 * thinBasic_ParseString returns EXT in ST(0) (numeric value of the text, or 0).
 * Declaring the thunk as `double` makes the compiler pop ST(0). Do not treat
 * that number as success or as the string length.
 */
void tb_ParseString(char **pszString)
{
    volatile double ext_unused;

    ext_unused = g_tb.parse_string(pszString);
    (void)ext_unused;
}

/**
 * @brief Check if the opening parenthesis is valid.
 * @param HideError: If 1, hide the error.
 * @param AutoPutBack: If 1, auto put back the opening parenthesis.
 * @return 1 if the opening parenthesis is valid, 0 otherwise.
 */
DWORD tb_CheckOpenParens(DWORD HideError, DWORD AutoPutBack)
{
    return g_tb.check_open_parens(HideError, AutoPutBack);
}

/**
 * @brief Check if the comma is valid.
 * @param HideError: If 1, hide the error.
 * @param AutoPutBack: If 1, auto put back the comma.
 * @return 1 if the comma is valid, 0 otherwise.
 */
DWORD tb_CheckComma(DWORD HideError, DWORD AutoPutBack)
{
    return g_tb.check_comma(HideError, AutoPutBack);
}

/**
 * @brief Check if the closing parenthesis is valid.
 * @param HideError: If 1, hide the error.
 * @param AutoPutBack: If 1, auto put back the closing parenthesis.
 * @return 1 if the closing parenthesis is valid, 0 otherwise.
 */
DWORD tb_CheckCloseParens(DWORD HideError, DWORD AutoPutBack)
{
    return g_tb.check_close_parens(HideError, AutoPutBack);
}

/**
 * @brief Allocate a ThinBasic return string (byte BSTR).
 * @param sz ASCII text, or NULL for empty
 * @return BSTR pointer for EAX, or NULL
 *
 * Official FreeBASIC modules use SysAllocStringByteLen + thinBasic_ReturnString.
 * thinBasic owns the BSTR and will free it. Do not return a C string pointer.
 */
void *tb_return_string(const char *sz)
{
    size_t len;
    const size_t max_len = 4096;

    if (g_tb.alloc_bstr == NULL) {
        return NULL;
    }

    if (sz == NULL || sz[0] == '\0') {
        return g_tb.alloc_bstr(NULL, 0);
    }

    for (len = 0; len < max_len; len++) {
        if (sz[len] == '\0') {
            break;
        }
    }

    if (len == 0 || len >= max_len) {
        return NULL;
    }

    return g_tb.alloc_bstr(sz, (UINT)len);
}
