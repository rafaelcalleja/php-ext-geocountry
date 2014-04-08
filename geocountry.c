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
	/*PHP_ME(Gender, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, connect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, trace, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, country, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, similarNames, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Gender, isNick, NULL, ZEND_ACC_PUBLIC)*/
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


   /* zend_declare_class_constant_long(Gender_ce, "IS_FEMALE", sizeof("IS_FEMALE")-1, IS_FEMALE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IS_MOSTLY_FEMALE", sizeof("IS_MOSTLY_FEMALE")-1, IS_MOSTLY_FEMALE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IS_MALE", sizeof("IS_MALE")-1, IS_MALE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IS_MOSTLY_MALE", sizeof("IS_MOSTLY_MALE")-1, IS_MOSTLY_MALE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IS_UNISEX_NAME", sizeof("IS_UNISEX_NAME")-1, IS_UNISEX_NAME TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IS_A_COUPLE", sizeof("IS_A_COUPLE")-1, IS_A_COUPLE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "NAME_NOT_FOUND", sizeof("NAME_NOT_FOUND")-1, NAME_NOT_FOUND TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ERROR_IN_NAME", sizeof("ERROR_IN_NAME")-1, ERROR_IN_NAME TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "ANY_COUNTRY", sizeof("ANY_COUNTRY")-1, GC_ANY_COUNTRY TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "BRITAIN", sizeof("BRITAIN")-1, GC_BRITAIN TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "IRELAND", sizeof("IRELAND")-1, GC_IRELAND TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "USA", sizeof("USA")-1, GC_USA TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "SPAIN", sizeof("SPAIN")-1, GC_SPAIN TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "PORTUGAL", sizeof("PORTUGAL")-1, GC_PORTUGAL TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ITALY", sizeof("ITALY")-1, GC_ITALY TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "MALTA", sizeof("MALTA")-1, GC_MALTA TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "FRANCE", sizeof("FRANCE")-1, GC_FRANCE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "BELGIUM", sizeof("BELGIUM")-1, GC_BELGIUM TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "LUXEMBOURG", sizeof("LUXEMBOURG")-1, GC_LUXEMBOURG TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "NETHERLANDS", sizeof("NETHERLANDS")-1, GC_NETHERLANDS TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "GERMANY", sizeof("GERMANY")-1, GC_GERMANY TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "EAST_FRISIA", sizeof("EAST_FRISIA")-1, GC_EAST_FRISIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "AUSTRIA", sizeof("AUSTRIA")-1, GC_AUSTRIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "SWISS", sizeof("SWISS")-1, GC_SWISS TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "ICELAND", sizeof("ICELAND")-1, GC_ICELAND TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "DENMARK", sizeof("DENMARK")-1, GC_DENMARK TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "NORWAY", sizeof("NORWAY")-1, GC_NORWAY TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "SWEDEN", sizeof("SWEDEN")-1, GC_SWEDEN TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "FINLAND", sizeof("FINLAND")-1, GC_FINLAND TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ESTONIA", sizeof("ESTONIA")-1, GC_ESTONIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "LATVIA", sizeof("LATVIA")-1, GC_LATVIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "LITHUANIA", sizeof("LITHUANIA")-1, GC_LITHUANIA TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "POLAND", sizeof("POLAND")-1, GC_POLAND TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "CZECH_REP", sizeof("CZECH_REP")-1, GC_CZECH_REP TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "SLOVAKIA", sizeof("SLOVAKIA")-1, GC_SLOVAKIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "HUNGARY", sizeof("HUNGARY")-1, GC_HUNGARY TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ROMANIA", sizeof("ROMANIA")-1, GC_ROMANIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "BULGARIA", sizeof("BULGARIA")-1, GC_BULGARIA TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "BOSNIA", sizeof("BOSNIA")-1, GC_BOSNIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "CROATIA", sizeof("CROATIA")-1, GC_CROATIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "KOSOVO", sizeof("KOSOVO")-1, GC_KOSOVO TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "MACEDONIA", sizeof("MACEDONIA")-1, GC_MACEDONIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "MONTENEGRO", sizeof("MONTENEGRO")-1, GC_MONTENEGRO TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "SERBIA", sizeof("SERBIA")-1, GC_SERBIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "SLOVENIA", sizeof("SLOVENIA")-1, GC_SLOVENIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ALBANIA", sizeof("ALBANIA")-1, GC_ALBANIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "GREECE", sizeof("GREECE")-1, GC_GREECE TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "RUSSIA", sizeof("RUSSIA")-1, GC_RUSSIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "BELARUS", sizeof("BELARUS")-1, GC_BELARUS TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "MOLDOVA", sizeof("MOLDOVA")-1, GC_MOLDOVA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "UKRAINE", sizeof("UKRAINE")-1, GC_UKRAINE TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ARMENIA", sizeof("ARMENIA")-1, GC_ARMENIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "AZERBAIJAN", sizeof("AZERBAIJAN")-1, GC_AZERBAIJAN TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "GEORGIA", sizeof("GEORGIA")-1, GC_GEORGIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "KAZAKH_UZBEK", sizeof("KAZAKH_UZBEK")-1, GC_KAZAKH_UZBEK TSRMLS_CC);

    zend_declare_class_constant_long(Gender_ce, "TURKEY", sizeof("TURKEY")-1, GC_TURKEY TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ARABIA", sizeof("ARABIA")-1, GC_ARABIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "ISRAEL", sizeof("ISRAEL")-1, GC_ISRAEL TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "CHINA", sizeof("CHINA")-1, GC_CHINA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "INDIA", sizeof("INDIA")-1, GC_INDIA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "JAPAN", sizeof("JAPAN")-1, GC_JAPAN TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "KOREA", sizeof("KOREA")-1, GC_KOREA TSRMLS_CC);
    zend_declare_class_constant_long(Gender_ce, "VIETNAM", sizeof("VIETNAM")-1, GC_VIETNAM TSRMLS_CC);

    return SUCCESS;*/
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

        ZVAL_LONG(return_value, get_gender(firstname, mode, gc_country, zgo TSRMLS_CC));

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


/* {{{ proto boolean Gender::trace(void) allow trace mode */
PHP_METHOD(Geocountry, trace)
{
	struct ze_geocountry_obj *zgo;

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

    zgo->internal_mode = TRACE_GENDER;
}
/* }}} */


/* {{{ proto string Gender::country(integer id) Get string country representation from a gender country class constant. */
PHP_METHOD(Geocountry, country)
{
	long country;
	int i;
	struct ze_geocountry_obj *zgo;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &country) == FAILURE) {
		RETURN_FALSE;
	}

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

	array_init(return_value);
	for (i=0; zgo->gc_data[i].country_text != NULL; i++) {
		if(zgo->gc_data[i].gc_country == (int)country) {
			add_assoc_stringl(return_value, "country_short", zgo->gc_data[i].country_short, strlen(zgo->gc_data[i].country_short), 1);
			add_assoc_stringl(return_value, "country", zgo->gc_data[i].country_text, strlen(zgo->gc_data[i].country_text), 1);
			break;
		}
	}
}
/* }}} */

/*{{{ proto string Gender::similarNames(string name[, integer country]) Find similar name*/
PHP_METHOD(Geocountry, similarNames)
{
	long country = GC_ANY_COUNTRY;
	int name_len = 0;
	char *name, *buf, *pch0, *pch1;
	struct ze_geocountry_obj *zgo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &name, &name_len, &country) == FAILURE) {
		RETURN_FALSE;
	}

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

	/* XXX possible overflow */
	buf = (char *) emalloc(sizeof(char)*2048);
	array_init(return_value);

	find_similar_name(name, (int)country, buf, 2048, zgo TSRMLS_CC);

	if (strlen(buf) < 1) {
		efree(buf);
		return;
	}

	pch0 = buf;
	pch1 = strchr(buf, ';');

	if (NULL == pch1) {
		add_next_index_string(return_value, buf, 1);
	} else {
		char *one;

		while(NULL != pch1) {
			int one_len = pch1 - pch0;

			one = estrndup(pch0, one_len);
			one[one_len] = '\0';
			add_next_index_string(return_value, one, 0);

			pch0 = pch1 + 2;
			pch1 = strchr(pch0, ';');
		}
		add_next_index_string(return_value, pch0, 1);
	}

	efree(buf);
}
/*}}}*/

/*{{{ proto string Gender::isNick(string name0, string name1[, int country]) Check if name0 is a nick of name1 */
PHP_METHOD(Geocountry, isNick)
{
	long country = GC_ANY_COUNTRY;
	char *name0, *name1;
	int name0_len, name1_len, check;
	struct ze_geocountry_obj *zgo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|l", &name0, &name0_len, &name1, &name1_len, &country) == FAILURE) {
		RETURN_FALSE;
	}

	zgo = (struct ze_geocountry_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

	check = check_nickname(name0, name1, 0, (int)country, zgo TSRMLS_CC);

	RETURN_BOOL(EQUIVALENT_NAMES == check);
}
/*}}}*/

