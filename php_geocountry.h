/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2009 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  |                                    |
  +----------------------------------------------------------------------+
*/

/* $Id: php_geocountry.h  $ */

#ifndef PHP_GEOCOUNTRY_H
#define PHP_GEOCOUNTRY_H

extern zend_module_entry geocountry_module_entry;
#define phpext_geocountry_ptr &geocountry_module_entry

#ifdef PHP_WIN32
#define PHP_GEOCOUNTRY_API __declspec(dllexport)
#else
#define PHP_GEOCOUNTRY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_GEOCOUNTRY_VERSION "0.0.1"

PHP_MINIT_FUNCTION(geocountry);
PHP_MSHUTDOWN_FUNCTION(geocountry);
PHP_MINFO_FUNCTION(geocountry);

PHP_METHOD(Geocountry, __construct);
PHP_METHOD(Geocountry, get);
PHP_METHOD(Geocountry, getCoords);
PHP_METHOD(Geocountry, connect);
PHP_METHOD(Geocountry, trace);
PHP_METHOD(Geocountry, country);
PHP_METHOD(Geocountry, similarNames);
PHP_METHOD(Geocountry, isNick);

#ifdef ZTS
#define GEOCOUNTRY_G(v) TSRMG(geocountry_globals_id, zend_geocountry_globals *, v)
#else
#define GEOCOUNTRY_G(v) (geocountry_globals.v)
#endif

/* data modes list */
#define DATA_MODE_ORIGINAL 0
#define DATA_MODE_SQLITE 1
#define DATA_MODE_MYSQL 2
#define DATA_MODE_POSTGRES 3

ZEND_BEGIN_MODULE_GLOBALS(geocountry)
	int dummy;
ZEND_END_MODULE_GLOBALS(geocountry)

ZEND_EXTERN_MODULE_GLOBALS(geocountry)

#define GENDER_GC_DATA_SIZE 56

struct ze_geocountry_obj {
	zend_object zo;
    int  internal_mode;
    php_stream *f_names;
    int  line_size;
    long record_count;
    long cache_p [CACHE_LOOKBACK_COUNT];
    char cache_text [CACHE_LOOKBACK_COUNT] [MAX_LINE_SIZE+1];

    char up_and_conv[HASH_COUNT];
    char sortchar [HASH_COUNT];
    char sortchar2[HASH_COUNT];
    char upperchar[HASH_COUNT];

    int ph_hash_group_begin[17];
    int ph_hash_group_end[17];
    int ph_rules_hash_del_ins [HASH_COUNT];
    int ph_rules_hash_replace [HASH_COUNT];

    char trace_buffer[2001];
    char input_buffer_1 [101];
    char input_buffer_2 [101];
    char output_buffer [2002];

    int data_mode;
    char *dsn;
    int internal_dict;
    struct gc_struct gc_data[GENDER_GC_DATA_SIZE];
    char latitude[DATA_LATITUDE_LENGTH+1];
    char longitude[DATA_LONGITUDE_LENGTH+1];

};



#endif	/* PHP_GENDER_H */
