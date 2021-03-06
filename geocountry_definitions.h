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
  | Author: Anatol Belski ab@php.net                                     |
  +----------------------------------------------------------------------+
*/


#ifndef GEOCOUNTRY_DEFINITIONS_H
#define GEOCOUNTRY_DEFINITIONS_H

#include <php.h>

struct ze_geocountry_obj;

#define GEOCOUNTRY_VERSION  "version 0.0.1, 2014-04-02"

/****  Defaults for the "main"-function.  ****/
/****  TO-DO: Change them, if necessary:  ****/
#define GENDER_DEFAULT_COUNTRY       GC_ANY_COUNTRY
#define GENDER_DEFAULT_TRACE_MODE    ( GENDER_TRACE_ALL_COUNTRIES | GENDER_TRACE_FULL_COUNTRY_NAME )

/****  TO-DO: Change the following macros if necessary.        ****/
/****  (NOTE: These macros are independent from corresponding  ****/
/****   definitions in the file "FIRST_NAME_FILE")             ****/
#define IS_COUNTRY             'P'
#define IS_CITY                'C'

#define EQUIVALENT_NAMES       '='
#define NOT_EQUAL_NAMES        '!'
#define NAME_NOT_FOUND         ' '
#define ERROR_IN_NAME          'E'
#define INTERNAL_ERROR_GENDER  'I'


/****  TO-DO:  If you use a bigger field length  ****/
/****    for first names, change this macro      ****/
#define LENGTH_FIRST_NAME  40

#if (LENGTH_FIRST_NAME < 20)
  #error "LENGTH_FIRST_NAME < 20  is not allowed"
#endif


/******************************************************************/
/****  macros for "compare_mode" / "trace_mode" (do not change) ***/
/****  (macros for "0" represent the default)                   ***/
/******************************************************************/

#define GENDER_COMPARE_EXPANDED_UMLAUTS     0
#define GENDER_TRACE_ALL_COUNTRIES          0
#define GENDER_TRACE_FULL_COUNTRY_NAME      0

#define GENDER_ALLOW_COMPRESSED_UMLAUTS     1
#define GENDER_ALLOW_COUPLE                 2

#define GENDER_TRACE_ONE_COUNTRY_ONLY       4
#define GENDER_TRACE_SHORT_COUNTRY_NAME     8


/****  list of "normal" letters and umlauts, with upper case  ****/
/****  (char set = iso8859-1) - DO NOT CHANGE THESE MACROS    ****/
#define letters_a_to_z   "abcdefghijklmnopqrstuvwxyz"
#define letters_A_to_Z   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define umlaut_lower   "����������������������������������"
#define umlaut_upper   "�����������������������،�������ݟ"

/****  macro for "lev100.h"  ****/
#define umlaut_conv    "AAAAA��CDEEEEIIIINOOOO���S��UUU�YY"
/****  macros for "gender.c"  ****/
#define umlaut_sort    "AAAAAAACDEEEEIIIINOOOOOOOSSTUUUUYY"
#define umlaut_sort2   "    AEE          H    EEE SH   E  "



/****  TO-DO:  If you want to convert DOS umlauts to iso chars,   ****/
/****     use the following macros to create a translation table  ****/
/**
 #define DOS_chars  "���Ɔ���Њ�����������䛔�藣��옷��Ǐ����Ԑ�����إ���坙�����"
 #define iso_chars  "�������������������������������������������������������������ݟ"
**/


/****  TO-DO:  If you want to convert Mac umlauts to iso chars,   ****/
/****     use the following macros to create a translation table  ****/
/**
 #define Mac_chars  "����������������������������������́�������������ͯ�����"
 #define iso_chars  "���������������������������������������������������������ܟ"
**/


/************************************************************/
/****  macros for "country" (do not change)  ****************/
/************************************************************/

#define GC_ANY_COUNTRY   0

/************************************************************/
/****  function prototypes  *********************************/
/************************************************************/

int get_gender (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC);
int get_gender_unicode (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC);
int get_gender_utf8 (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC);


/*
 void standardize_arabic_name (char dest[], char src[], int len);
*/

/****  internally, this program uses 1 byte chars  ****/
#define HASH_COUNT    256


/****  bit masks for "compare_mode"           ****/
/****  (macros "1" to "8" are in "*.h" file)  ****/
#define SEARCH_GENDER           16
#define SEARCH_EQUIV            32
#define SEARCH_FIRST            64
#define GENDER_COMPARE_EXACT   128
#define COMPARE_ABBREVIATION   256
#define DETAILS_FOR_COUNTRY    512

#define GET_MATCH_OR_NEXT_HIGHER  1024
#define UMLAUT_EXPAND             2048
#define IGNORE_SEPARATOR          4096
#define EXPAND_PARAM_1            8192
#define EXPAND_PARAM_2           16384

#define IGNORE_OTHER_COUNTRIES   32768

/****  bit masks for "internal_mode"  ****/
#define GENDER_INITIALIZED         16
#define GENDER_GET_COUNTRY         32
#define LEV_G_COMPARE_GERMAN       64
#define LEV_G_SKIP_UPEXPAND       128
#define TRACE_GENDER              256
#define TRACE_LEV_G               512

#define CHECK_STRING  "# DO NOT CHANGE:   FILE-FORMAT DEFINITION-DATE = 2008-11-16 "
#define CACHE_LOOKBACK_COUNT       2

#define CHARS_TO_IGNORE    "<>^,�'`~�/"
#define IS_CHAR_TO_IGNORE      '\001'

#define COMPRESSED_UMLAUTS_FOR_SORTING   '-'
#define EXPANDED_UMLAUTS_FOR_SORTING     '+'
#define PSEUDO_VOWEL_FOR_ARABIC_NAMES    '_'

#define CHARSET_ISO_8859_1    0
#define CHARSET_UNICODE       1
#define CHARSET_UTF_8         2


#define DATA_LATITUDE_POS      30
#define DATA_LATITUDE_LENGTH   11
#define DATA_LONGITUDE_POS      42
#define DATA_LONGITUDE_LENGTH   11


/****  position and length of first names in dictionary file  ****/
#define DATA_NAME_POS           3
#define DATA_NAME_LENGTH       26
#define MAX_LINE_SIZE         100

#define POS_UMLAUT_INFO        29

/****  macros needed for the Levenshtein function  ****/
#define MATCHES_ANY_STRING    '*'
#define MATCHES_ANY_CHAR      '?'

#define MATCHES_ALL   -1000

#define WORD_SEPARATORS   "+-/*( )&,'�`.:"

struct PHONETIC_RULES
{
	char *text_1;
	char *text_2;
	int len_1;
	int len_2;
	int ph_diff;     /***  difference in hundredths of points  ***/
	int hash_group;
};

/****  dictionary file info  ****/
struct gc_struct
{
	unsigned int pos;    /***  first column = 0  ***/
	unsigned int n;
	int  quality_of_statistics;
	int  weight;
	int  gc_country;
	char *country_short;
	char *country_text;
};

 
#endif
