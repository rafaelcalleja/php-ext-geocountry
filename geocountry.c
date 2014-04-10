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
  |                                |
  +----------------------------------------------------------------------+
*/

/* $Id: geocountry.c 2014-04-02 ab $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <ext/pcre/php_pcre.h>
#include <zend_exceptions.h>

#include "geocountry_definitions.h"
#include "php_geocountry.h"

/* True global resources - no need for thread safety here */
static int le_gender;

/* class definitions */
static zend_class_entry *Geocountry_ce;

extern struct gc_struct gc_data[];
 
zend_object_handlers default_geocountry_handlers;

#include "country_dic.c"

extern int initialize_gender (struct ze_geocountry_obj *zgo TSRMLS_DC);
extern int get_gender (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC);
extern int determine_country (char *text, struct ze_geocountry_obj *zgo TSRMLS_DC);

/* Off set length for gender_parse_dsn */
#define OFFSETS_LEN 21

int gender_connect_to_source(struct ze_geocountry_obj *zgo TSRMLS_DC)
{/*{{{*/

	zgo->data_mode = DATA_MODE_ORIGINAL;

	if (zgo->f_names == NULL) {
		if (NULL != zgo->dsn) {
			zgo->f_names = php_stream_open_wrapper(zgo->dsn, "r",  STREAM_MUST_SEEK | USE_PATH, NULL);
			zgo->internal_dict = 0;
		} else {
			zgo->dsn = (char*)php_geocountry_name_dict_data;
			zgo->f_names = php_stream_open_wrapper(zgo->dsn, "r",  0, NULL);
			zgo->internal_dict = 1;
		}
		if (zgo->f_names == NULL) {
			zend_throw_exception_ex(Geocountry_ce, 0 TSRMLS_CC, "Could not load data file");

			return INTERNAL_ERROR_GENDER;
		}
	}

	return 0;
}/*}}}*/

ZEND_DECLARE_MODULE_GLOBALS(geocountry);

/* {{{ geocountry_functions[]
 *
 * Every user visible function must have an entry in geocountry_functions[].
 */
zend_function_entry geocountry_functions[] = {
    {NULL, NULL, NULL}
};
/* }}} */

void geocountry_globals_dtor(zend_geocountry_globals *gecountry_globals TSRMLS_DC)
{/*{{{*/
}/*}}}*/

void geocountry_globals_ctor(zend_geocountry_globals *gecountry_globals TSRMLS_DC)
{/*{{{*/
}/*}}}*/

void
php_geocountry_obj_destroy(void *obj TSRMLS_DC)
{/*{{{*/
	struct ze_geocountry_obj *zgo = (struct ze_geocountry_obj *)obj;

	zend_object_std_dtor(&zgo->zo TSRMLS_CC);

	if (!zgo->internal_dict) {
		efree(zgo->dsn);
	}

	if (zgo->f_names != NULL) {
		php_stream_close(zgo->f_names);
		zgo->f_names = NULL;
	}

	zgo->line_size = 0;
	zgo->record_count = 0L;

	efree(zgo);

}/*}}}*/

zend_object_value
php_geocountry_obj_init(zend_class_entry *ze TSRMLS_DC)
{
	zend_object_value ret;
	struct ze_geocountry_obj *zgo;
#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 4
	zval *tmp;
#endif
	int i;

	zgo = (struct ze_geocountry_obj *)emalloc(sizeof(struct ze_geocountry_obj));
	memset(&zgo->zo, 0, sizeof(zend_object));

	zend_object_std_init(&zgo->zo, ze TSRMLS_CC);
#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 4
	zend_hash_copy(zgo->zo.properties, &ze->default_properties, (copy_ctor_func_t) zval_add_ref,
				                    (void *) &tmp, sizeof(zval *));
#else
	object_properties_init(&zgo->zo, ze);
#endif

    zgo->internal_mode = GENDER_DEFAULT_TRACE_MODE;
    zgo->f_names = NULL;
    zgo->line_size = 0;
    zgo->record_count = 0L;

    zgo->data_mode = DATA_MODE_ORIGINAL;
    zgo->dsn = NULL;
	zgo->internal_dict = 1;


	for (i=0; i < GENDER_GC_DATA_SIZE; i++) {
		zgo->gc_data[i].pos = gc_data[i].pos;
		zgo->gc_data[i].n = gc_data[i].n;
		zgo->gc_data[i].quality_of_statistics = gc_data[i].quality_of_statistics;
		zgo->gc_data[i].weight = gc_data[i].weight;
		zgo->gc_data[i].gc_country = gc_data[i].gc_country;
		zgo->gc_data[i].country_short = gc_data[i].country_short;
		zgo->gc_data[i].country_text = gc_data[i].country_text;
	}

    ret.handle = zend_objects_store_put(zgo, NULL,
				(zend_objects_free_object_storage_t) php_geocountry_obj_destroy,
				NULL TSRMLS_CC);

	ret.handlers = &default_geocountry_handlers;

	return ret;
}/*}}}*/

/* {{{ client class methods[] */
zend_function_entry Geocountry_methods[] = {
    PHP_ME(Geocountry, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Geocountry, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Geocountry, getCoords, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(Gender_get, 0, 0, 2)
    ZEND_ARG_INFO(0, firstname)
    ZEND_ARG_INFO(0, country)
ZEND_END_ARG_INFO()

/* {{{ gender_module_entry
 */
zend_module_entry geocountry_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "geocountry",
    geocountry_functions,
    PHP_MINIT(geocountry),
    PHP_MSHUTDOWN(geocountry),
    NULL,
    NULL,
    PHP_MINFO(geocountry),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_GEOCOUNTRY_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEOCOUNTRY
ZEND_GET_MODULE(geocountry)
#endif


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geocountry)
{

    zend_class_entry ce;

#ifdef ZTS
    ts_allocate_id(&gecountry_globals_id,
                   sizeof(zend_geocountry_globals),
                   (ts_allocate_ctor)geocountry_globals_ctor,
                   (ts_allocate_dtor)geocountry_globals_dtor);
#else
    geocountry_globals_ctor(&geocountry_globals TSRMLS_CC);
#endif

	memcpy(&default_geocountry_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	default_geocountry_handlers.clone_obj = NULL;

#if PHP_MAJOR_VERSION >= 5 && PHP_MINOR_VERSION >= 3
    INIT_CLASS_ENTRY(ce, "Geocountry\\Geocountry", Geocountry_methods);
#else
    INIT_CLASS_ENTRY(ce, "Geocountry", Geocountry_methods);
#endif
	ce.create_object = php_geocountry_obj_init;
    Geocountry_ce = zend_register_internal_class(&ce TSRMLS_CC);


    zend_declare_class_constant_long(Geocountry_ce, "IS_COUNTRY", sizeof("IS_COUNTRY")-1, IS_COUNTRY TSRMLS_CC);
    zend_declare_class_constant_long(Geocountry_ce, "IS_CITY", sizeof("IS_CITY")-1, IS_CITY TSRMLS_CC);

    zend_declare_class_constant_long(Geocountry_ce, "NAME_NOT_FOUND", sizeof("NAME_NOT_FOUND")-1, NAME_NOT_FOUND TSRMLS_CC);
    zend_declare_class_constant_long(Geocountry_ce, "ERROR_IN_NAME", sizeof("ERROR_IN_NAME")-1, ERROR_IN_NAME TSRMLS_CC);

    zend_declare_class_constant_long(Geocountry_ce, "ANY_COUNTRY", sizeof("ANY_COUNTRY")-1, GC_ANY_COUNTRY TSRMLS_CC);

    return SUCCESS;
}
/* }}}

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geocountry)
{


#ifdef ZTS
    ts_free_id(gecountry_globals_id);
#else
    geocountry_globals_dtor(&geocountry_globals TSRMLS_CC);
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geocountry)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "geocountry support", "enabled");
    php_info_print_table_header(2, "Version ", PHP_GEOCOUNTRY_VERSION);
    php_info_print_table_header(2, "Geocountry API ", GEOCOUNTRY_VERSION);
    php_info_print_table_end();
}
/* }}} */


/* {{{ proto Geocountry Geocountry::Geocountry() */
PHP_METHOD(Geocountry, __construct)
{
	struct ze_geocountry_obj *zgo;
    int i;
    char *dsn = NULL;
    int dsn_len = 0;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &dsn, &dsn_len) == FAILURE) {
            return;
    }

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if(dsn_len > 0) {
        zgo->dsn = estrdup(dsn);
        if(gender_connect_to_source(zgo TSRMLS_CC) == INTERNAL_ERROR_GENDER) {
            RETURN_NULL();
        }
    }



    if (! (zgo->internal_mode & GENDER_INITIALIZED)) {
        i = initialize_gender(zgo TSRMLS_CC);

        if (i < 0  ||  ! (zgo->internal_mode & GENDER_INITIALIZED)) {
            zend_throw_exception(Geocountry_ce, "Initialization failed", 0 TSRMLS_CC);
            RETURN_NULL();
        }
    }
}
/* }}} */


/* {{{ proto integer Geocountry::getCoords(string $name) get coords for the country */
PHP_METHOD(Geocountry, getCoords)
{
	struct ze_geocountry_obj *zgo;

    char *firstname;
    int firstname_len;
    zval *country = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &firstname, &firstname_len) == FAILURE) {
            return;
    }

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if(firstname_len) {
		int mode = 0;
		int gc_country = GC_ANY_COUNTRY;

        php_printf ("Searching Country... %s\n",firstname );
        get_gender(firstname, mode, gc_country, zgo TSRMLS_CC );

        array_init(return_value);
        add_assoc_double(return_value,"latitude",  atof(zgo->latitude));
        add_assoc_double(return_value,"longitude", atof(zgo->longitude));

        php_printf ("--------------------------------__%s-----------------------------------", zgo->latitude);
        php_printf ("--------------------------------__%s-----------------------------------", zgo->longitude);

        return;
    }

    RETURN_FALSE;
}
/* }}} */

/* {{{ proto integer Gender::get(string $name, string $country) get gender for the name in the country */
PHP_METHOD(Geocountry, get)
{
	struct ze_geocountry_obj *zgo;
    char *firstname;
    int firstname_len;
    zval *country = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &firstname, &firstname_len, &country) == FAILURE) {
            return;
    }

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);


    if(firstname_len) {
		int mode = 0;
		int gc_country = GC_ANY_COUNTRY;

        if(country) {
            if(Z_TYPE_P(country) == IS_LONG) {
                gc_country = Z_LVAL_P(country);
            } else if(Z_TYPE_P(country) == IS_STRING) {
                gc_country = determine_country(Z_STRVAL_P(country), zgo TSRMLS_CC);
            }
			mode = IGNORE_OTHER_COUNTRIES;
        }

        php_printf ("Searching Country... %s\n",firstname );
        ZVAL_LONG(return_value, get_gender(firstname, mode, gc_country, zgo TSRMLS_CC ));

        return;
    }

    RETURN_FALSE;
}
/* }}} */


/* {{{ proto boolean Gender::connect(string $dsn) Open a data source, will support several sources*/
PHP_METHOD(Geocountry, connect)
{
	struct ze_geocountry_obj *zgo;
    char *dsn;
    int dsn_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &dsn, &dsn_len) == FAILURE) {
            RETURN_FALSE;
    }

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if(dsn_len) {
		if (zgo->dsn && !zgo->internal_dict) {
			efree(zgo->dsn);
			zgo->dsn = estrdup(dsn);
		}

        if(gender_connect_to_source(zgo TSRMLS_CC) != INTERNAL_ERROR_GENDER) {
            RETURN_TRUE;
        }
    }

    RETURN_FALSE;

}
/* }}} */




