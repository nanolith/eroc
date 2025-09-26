/**
 * \file eroc/config.h
 *
 * \brief Generated configuration file for eroc.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#define MAKE_C_VERSION(X,Y) V ## X ## _ ## Y
#define EROC_VERSION_SYM \
    MAKE_C_VERSION(@EROC_VERSION_MAJOR@, @EROC_VERSION_MINOR@)

#define EROC_VERSION_STRING \
    "@EROC_VERSION_MAJOR@.@EROC_VERSION_MINOR@.@EROC_VERSION_REL@"
