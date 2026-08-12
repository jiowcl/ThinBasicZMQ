/*
 * tb_parse.h — ThinBasic parameter parsing helpers.
 *
 * @author Ji-Feng Tsai (Jiowcl)
 * @email jiowcl@gmail.com
 * @copyright Copyright (c) 2026 Jiowcl. All rights reserved.
 */

#ifndef TB_PARSE_H
#define TB_PARSE_H

#include "tb_thincore.h"

#include <stddef.h>
#include <string.h>

/**
 * @brief Expect an open parenthesis.
 * @param void
 * @return int
 */
static int tb_expect_open_parens(void)
{
    return tb_CheckOpenParens(thinBasic_FALSE, thinBasic_FALSE) != 0;
}

/**
 * @brief Expect a close parenthesis.
 * @param void
 * @return int
 */
static int tb_expect_close_parens(void)
{
    return tb_CheckCloseParens(thinBasic_FALSE, thinBasic_FALSE) != 0;
}

/**
 * @brief Expect a comma.
 * @param void
 * @return int
 */
static int tb_expect_comma(void)
{
    return tb_CheckComma(thinBasic_FALSE, thinBasic_FALSE) != 0;
}

/**
 * @brief Parse a numeric expression as Long (ByRef stdcall, no FPU).
 * @param out
 * @return int
 */
static int tb_parse_int(LONG *out)
{
    if (out == NULL) {
        return 0;
    }

    *out = 0;
    tb_ParseLong(out);
    
    return 1;
}

/**
 * @brief Parse a string.
 * @param out
 * @param out_len
 * @return int
 */
static int tb_parse_string(char **out, DWORD *out_len)
{
    size_t len;

    if (out == NULL || out_len == NULL) {
        return 0;
    }

    if (tb_ParseString(out) == 0 || *out == NULL) {
        return 0;
    }

    len = strlen(*out);

    if (len == 0 || len >= 0x7FFFFFFF) {
        return 0;
    }

    *out_len = (DWORD)len;

    return 1;
}

#endif /* TB_PARSE_H */
