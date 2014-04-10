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
  |                                   |
  +----------------------------------------------------------------------+
*/
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#include "geocountry_definitions.h"
 
#include <php.h>
#include <php_ini.h>
#include "php_geocountry.h"
 
extern int gender_connect_to_source(struct ze_geocountry_obj *zgo TSRMLS_DC);

/****  Notice: "country_short" is mostly (but not always)  ****/
/****          identical to international country codes    ****/
/* should be const, but some stuff get's owerwritten in the trace mode */
/* NOTE In PHP we have to put this into the gender object to have it
thread safe. */
struct gc_struct gc_data[] = {
    { 30, 0, VERY_GOOD, 60, GC_BRITAIN,   "LS",   "Latitude Sig" },
    { 31, 0, GOOD,       4, GC_IRELAND,   "L1",   "Latitude 1"     },
    { 32, 0, VERY_GOOD,150, GC_USA,       "L2",   "Latitude 2"      },
    { 33, 0, VERY_GOOD, 60, GC_ITALY,     "L3",   "Latitude 3"       },
    { 34, 0, MEDIUM,     1, GC_MALTA,     "L4",   "Latitude 4"       },
    { 35, 0, GOOD,      10, GC_PORTUGAL,  "L5",   "Latitude 5"    },
    { 36, 0, VERY_GOOD, 40, GC_SPAIN,     "L6",   "Latitude 6"       },
    { 37, 0, VERY_GOOD, 60, GC_FRANCE,    "L7",   "Latitude 7"      },
    { 38, 0, VERY_GOOD, 10, GC_BELGIUM,   "L8",   "Latitude 8"     },
    { 39, 0, VERY_GOOD,  1, GC_LUXEMBOURG,"L9",   "Latitude 9"  },
    { 40, 0, VERY_GOOD, 14, GC_NETHERLANDS,"L10",  "Latitude 10" },

    { 41, 0, GOOD,       1, GC_EAST_FRISIA,"LOS", "Longitude Sig" },
    { 42, 0, VERY_GOOD, 80, GC_GERMANY,    "LO1",   "Longitude 1"     },
    { 43, 0, VERY_GOOD,  8, GC_AUSTRIA,    "LO2",   "Longitude 2"     },
    { 44, 0, VERY_GOOD,  7, GC_SWISS,      "LO3",  "Longitude 3"       },
    { 45, 0, VERY_GOOD,  1, GC_ICELAND,    "LO4", "Longitude 4"     },
    { 46, 0, VERY_GOOD,  5, GC_DENMARK,    "LO5",  "Longitude 5"     },
    { 47, 0, GOOD,       4, GC_NORWAY,     "LO6",   "Longitude 6"      },
    { 48, 0, VERY_GOOD,  8, GC_SWEDEN,     "LO7",   "Longitude 7"      },
    { 49, 0, GOOD,       5, GC_FINLAND,    "LO8", "Longitude 8"     },
    { 50, 0, GOOD,       2, GC_ESTONIA,    "LO9", "Longitude 9"     },
    { 51, 0, GOOD,       2, GC_LATVIA,     "L010", "Longitude 10"      },

    { 52, 0, GOOD,       3, GC_LITHUANIA,  "LIT", "Lithuania"   }, 
    { 53, 0, GOOD,      35, GC_POLAND,     "PL",  "Poland"      }, 
    { 54, 0, GOOD,       8, GC_CZECH_REP,  "CZ",  "Czech Republic" }, 
    { 55, 0, GOOD,       7, GC_SLOVAKIA,   "SK",  "Slovakia"    }, 
    { 56, 0, GOOD,      11, GC_HUNGARY,    "H",   "Hungary"     }, 
    { 57, 0, VERY_GOOD, 22, GC_ROMANIA,    "RO",  "Romania"     }, 
    { 58, 0, GOOD,       9, GC_BULGARIA,   "BG",  "Bulgaria"    }, 

    { 59, 0, MEDIUM,     4, GC_BOSNIA,     "BIH","Bosnia and Herzegovina" }, 
    { 60, 0, GOOD,       5, GC_CROATIA,    "CRO", "Croatia"     }, 
    { 61, 0, MEDIUM,     1, GC_KOSOVO,     "KOS", "Kosovo"      }, 
    { 62, 0, MEDIUM,     2, GC_MACEDONIA,  "MK",  "Macedonia"   }, 
    { 63, 0, MEDIUM,     1, GC_MONTENEGRO, "MON", "Montenegro"  }, 
    { 64, 0, MEDIUM,     9, GC_SERBIA,     "SER", "Serbia"      }, 
    { 65, 0, MEDIUM,     2, GC_SLOVENIA,   "SLO", "Slovenia"    }, 
    { 66, 0, GOOD,       3, GC_ALBANIA,    "AL",  "Albania"     }, 
    { 67, 0, GOOD,      10, GC_GREECE,     "GR",  "Greece"      }, 

    { 68, 0, GOOD,     100, GC_RUSSIA,     "RUS", "Russia"      }, 
    { 69, 0, MEDIUM,    10, GC_BELARUS,    "BY",  "Belarus"     }, 
    { 70, 0, MEDIUM,     4, GC_MOLDOVA,    "MOL", "Moldova"     }, 
    { 71, 0, MEDIUM,    45, GC_UKRAINE,    "UKR", "Ukraine"     }, 
    { 72, 0, MEDIUM,     3, GC_ARMENIA,    "ARM", "Armenia"     }, 
    { 73, 0, MEDIUM,     4, GC_AZERBAIJAN, "AZE", "Azerbaijan"  }, 
    { 74, 0, MEDIUM,     5, GC_GEORGIA,    "GEO", "Georgia"     }, 
    { 75, 0, MEDIUM,    15,GC_KAZAKH_UZBEK,"KAZ", "Kazakhstan/Uzbekistan" }, 

    { 76, 0, GOOD,      55, GC_TURKEY,     "TR",  "Turkey"      }, 
    { 77, 0, GOOD,      80, GC_ARABIA,     "AR",  "Arabia/Persia" }, 
    { 78, 0, MEDIUM,     4, GC_ISRAEL,     "ISR", "Israel"      }, 
    { 79, 0, VERY_GOOD,300, GC_CHINA,      "CHN", "China"       }, 
    { 80, 0, GOOD,     250, GC_INDIA,      "IND", "India/Sri Lanka" }, 
    { 81, 0, GOOD,      35, GC_JAPAN,      "JAP", "Japan"       }, 
    { 82, 0, GOOD,      12, GC_KOREA,      "KOR", "Korea"       }, 
    { 83, 0, GOOD,      17, GC_VIETNAM,    "VN",  "Vietnam"     }, 
    { 84, 0, MEDIUM,     1,    -1L,        "XX",  "other countries" }, 
    {  0, 0,  0,         0,    -1L,        NULL,    NULL        }
}; 

static char *umlauts_unicode[]
  =  {
       "256",  "<A/>",
       "257",  "<a/>",
       "258",  "<A^>",
       "258",  "<Â>",   /** <A^> **/
       "259",  "<a^>",
       "259",  "<â>",   /** <a^> **/
       "260",  "<A,>",
       "261",  "<a,>",
       "262",  "<C´>",
       "263",  "<c´>",
       "268",  "<C^>",
       "268",  "<CH>",
       "269",  "<c^>",
       "269",  "<ch>",
       "271",  "<d´>",
       "272",  "<Ð>",
       "272",  "<DJ>",
       "273",  "<ð>",
       "273",  "<dj>",
       "274",  "<E/>",
       "275",  "<e/>",
       "278",  "<E°>",
       "279",  "<e°>",
       "280",  "<E,>",
       "281",  "<e,>",
       "282",  "<E^>",
       "282",  "<Ê>",
       "283",  "<e^>",
       "283",  "<ê>",
       "286",  "<G^>",
       "287",  "<g^>",
       "290",  "<G,>",
       "291",  "<g´>",
       "298",  "<I/>",
       "299",  "<i/>",
       "304",  "<I°>",
       "305",  "<i>",
       "306",  "<IJ>",
       "307",  "<ij>",
       "310",  "<K,>",
       "311",  "<k,>",
       "315",  "<L,>",
       "316",  "<l,>",
       "317",  "<L´>",
       "318",  "<l´>",
       "321",  "<L/>",
       "322",  "<l/>",
       "325",  "<N,>",
       "326",  "<n,>",
       "327",  "<N^>",
       "328",  "<n^>",
       "336",  "<Ö>",
       "337",  "<ö>",
       "338",  "<OE>",
       "338",  "Œ",   /** <OE> **/
       "339",  "<oe>",
       "339",  "œ",   /** <oe> **/
       "344",  "<R^>",
       "345",  "<r^>",
       "350",  "<S,>",
       "351",  "<s,>",
       "352",  "<S^>",
       "352",  "Š",   /** <S^> **/
       "352",  "<SCH>",
       "352",  "<SH>",
       "353",  "<s^>",
       "353",  "š",   /** <s^> **/
       "353",  "<sch>",
       "353",  "<sh>",
       "354",  "<T,>",
       "355",  "<t,>",
       "357",  "<t´>",
       "362",  "<U/>",
       "363",  "<u/>",
       "366",  "<U°>",
       "367",  "<u°>",
       "370",  "<U,>",
       "371",  "<u,>",
       "379",  "<Z°>",
       "380",  "<z°>",
       "381",  "<Z^>",
       "382",  "<z^>",
       "7838", "<ß>",   /***  newly defined "Großes ß"  ***/
       NULL,    NULL
     };
 
static struct PHONETIC_RULES ph_rules_german[] =
  {
    { "IJ",  "I",   0, 0,   10,  1 },
    { "IJ",  "J",   0, 0,   30,  1 },
    { "IY",  "I",   0, 0,   10,  1 },
    { "IY",  "Y",   0, 0,   30,  1 },
    { "I",   "J",   0, 0,   30,  1 },
    { "I",   "Y",   0, 0,   30,  1 },
    { "J",   "Y",   0, 0,   20,  1 },
    { "IE",  "I",   0, 0,    5,  1 },
    { "IE",  "",    0, 0,  120,  1 },
 
    { "EU",  "OI",  0, 0,   10,  2 },
    { "EU",  "OY",  0, 0,   10,  2 },
    { "OI",  "OY",  0, 0,    5,  2 },
    { "OU",  "U",   0, 0,    5,  2 },
 
    { "AH",  "A",   0, 0,    5,  3 },
    { "H",   "",    0, 0,   50,  3 },
    { "Å",   "AA",  0, 0,    0,  3 },
    { "Ä",   "AE",  0, 0,    0,  3 },
    { "Ä",   "A",   0, 0,   40,  3 },
    { "AE",  "A",   0, 0,   60,  3 },
 
    { "Ä",   "E",   0, 0,   20,  4 },
    { "Æ",   "E",   0, 0,   20,  4 },
    { "A",   "ER",  0, 0,   70,  4 },
    { "AE",  "E",   0, 0,   20,  4 },
    { "AE",  "",    0, 0,  110,  4 },
 
    { "AI",  "AY",  0, 0,    5,  4 },
    { "AI",  "EI",  0, 0,    5,  4 },
    { "AI",  "EY",  0, 0,   10,  4 },
    { "AY",  "EI",  0, 0,   10,  4 },
    { "AY",  "EY",  0, 0,    5,  4 },
    { "EI",  "EY",  0, 0,    5,  4 },
 
    { "Å",   "O",   0, 0,   70,  5 },
    { "Ö",   "OE",  0, 0,    0,  5 },
    { "Ö",   "O",   0, 0,   40,  5 },
    { "Ø",   "Ö",   0, 0,    0,  5 },
    { "Ø",   "OE",  0, 0,   10,  5 },
    { "Ø",   "O",   0, 0,   30,  5 },
    { "Œ",   "Ö",   0, 0,    5,  5 },
    { "Œ",   "OE",  0, 0,    0,  5 },
    { "OE",  "O",   0, 0,   60,  5 },
    { "OE",  "",    0, 0,  110,  5 },
 
    { "CHS", "X",   0, 0,  100,  6 },
    { "CKS", "X",   0, 0,   30,  6 },
    { "CK",  "K",   0, 0,    5,  6 },
    { "C",   "K",   0, 0,   30,  6 },
    { "CHS", "",    0, 0,  200,  6 },
    { "CH",  "",    0, 0,  130,  6 },
    { "CKS", "",    0, 0,  200,  6 },
    { "CK",  "",    0, 0,  110,  6 },
 
    { "DT",  "T",   0, 0,    5,  7 },
    { "D",   "T",   0, 0,   30,  7 },
    { "TH",  "T",   0, 0,    5,  7 },
    { "DT",  "",    0, 0,  110,  7 },
 
    { "KS",  "X",   0, 0,    5,  8 },
    { "GS",  "X",   0, 0,   10,  8 },
    { "G",   "K",   0, 0,   50,  8 },
    { "QU",  "KW",  0, 0,   10,  8 },
    { "Q",   "K",   0, 0,   10,  8 },
 
    { "NCH", "NSCH",0, 0,   10,  9 },
    { "NCH", "NSH", 0, 0,   10,  9 },
    { "NTX", "NCH", 0, 0,   20,  9 },
    { "NTX", "NSCH",0, 0,   20,  9 },
    { "NTX", "NSH", 0, 0,   20,  9 },
    { "NG",  "NK",  0, 0,   20,  9 },
 
    { "ß",   "SS",  0, 0,    0, 10 },
    { "ß",   "S",   0, 0,    5, 10 },
    { "SCH", "SH",  0, 0,    5, 10 },
    { "SCH", "SZ",  0, 0,   20, 10 },
    { "SCH", "S",   0, 0,  100, 10 },
    { "SCH", "",    0, 0,  200, 10 },
 
    { "TZ",  "Z",   0, 0,    5, 11 },
    { "TIA", "ZIA", 0, 0,   20, 11 },
    { "Z",   "C",   0, 0,   40, 11 },
    { "Z",   "S",   0, 0,   50, 11 },
 
    { "M",   "N",   0, 0,   70, 12 },
    { "N",   "U",   0, 0,   70, 12 },
 
    { "PH",  "F",   0, 0,    5, 13 },
    { "PF",  "F",   0, 0,    5, 13 },
    { "B",   "P",   0, 0,   40, 13 },
 
    { "F",   "V",   0, 0,   20, 14 },
    { "W",   "V",   0, 0,   20, 14 },
 
    { "Ü",   "UE",  0, 0,    0, 15 },
    { "Ü",   "U",   0, 0,   40, 15 },
    { "Ü",   "Y",   0, 0,   30, 15 },
    { "UE",  "U",   0, 0,   60, 15 },
    { "UE",  "",    0, 0,  110, 15 },
 
    { NULL,  NULL,  0, 0,    0,  0 }
  };
 
 
static char *russian_names[] =
  {
    "Aleksander",   "Oleksander",
    "Aleksandr",    "Aliaksandr",
    "Aleksandr",    "Oleksandr",
    "Aleksandra",   "Aliaksandra",
    "Aleksandra",   "Oleksandra",
    "Aleksey",      "Aliaksei",
    "Aleksey",      "Aliaksey",
    "Aleksey",      "Oleksey",
    "Aleksey",      "Oleksii",
    "Aleksey",      "Oleksij",
    "Aleksey",      "Oleksiy",
    "Alexander",    "Olexander",
    "Alexandr",     "Olexandr",
    "Alexandra",    "Olexandra",
    "Alexey",       "Olexiy",
    "Anatoliy",     "Anatol",
    "Andrey",       "Andrii",
    "Andrey",       "Andrij",
    "Andrey",       "Andriy",
    "Artiom",       "Artsiom",
    "Boris",        "Barys",
    "Boris",        "Borys",
    "Dariya",       "Daryia",
    "Denis",        "Denys",
    "Diana",        "Dziana",
    "Diyana",       "Dziyana",
    "Dmitriy",      "Dmitro",
    "Dmitriy",      "Dmytriy",
    "Dmitriy",      "Dmytro",
    "Dmitriy",      "Dzimitry",
    "Dmitriy",      "Dzmitrij",
    "Dmitriy",      "Dzmitriy",
    "Dmitry",       "Dzmitry",
    "Dzmitri",      "Dzmitri",
    "Elena",        "Ellina",
    "Evgeniy",      "Eugeni",
    "Fedor",        "Fedir",
    "Galina",       "Galyna",
    "Gennadiy",     "Henadz",
    "Gennadiy",     "Henadzi",
    "Gennadiy",     "Hennadij",
    "Gennadiy",     "Hennadiy",
    "Gennadiy",     "Hennadzy",
    "Georgiy",      "Georgii",
    "Grigorij",     "Grygorij",
    "Grigoriy",     "Grigorii",
    "Grigoriy",     "Grygorii",
    "Grigoriy",     "Grygory",
    "Igor",         "Ihar",
    "Igor",         "Ihor",
    "Ilya",         "Illya",
    "Irina",        "Iryna",
    "Jevgenija",    "Jevgenija",
    "Jevgeniy",     "Jevgeniy",
    "Jevgeniy",     "Yevgen",
    "Juri",         "Juriy",
    "Katerina",     "Kateryna",
    "Katerina",     "Katsiaryna",
    "Kiril",        "Kiryl",
    "Kiril",        "Kiryll",
    "Kiril",        "Kyrylo",
    "Konstantin",   "Kanstantsin",
    "Konstantin",   "Konstantyn",
    "Konstantin",   "Kostiantyn",
    "Konstantin",   "Kostyantin",
    "Konstantin",   "Kostyantyn",
    "Kristina",     "Khrystyna",
    "Kristina",     "Kristsina",
    "Kristina",     "Krystina",
    "Kristina",     "Krystsina",
    "Kristina",     "Krystyna",
    "Larisa",       "Larysa",
    "Leonid",       "Leanid",
    "Lubomir",      "Lubomir",
    "Ludmila",      "Ludmyla",
    "Lyudmila",     "Lyudmyla",
    "Margarita",    "Marharita",
    "Margarita",    "Marharyta",
    "Margaryta",    "Margaryta",
    "Marina",       "Maryna",
    "Mariya",       "Maryia",
    "Mariya",       "Naryja",
    "Maxim",        "Maksym",
    "Michail",      "Michailo",
    "Michail",      "Michaylo",
    "Michail",      "Mihail",
    "Mikhail",      "Mikhailo",
    "Mikhail",      "Mikhaylo",
    "Mikhail",      "Mykhailo",
    "Mikhail",      "Mykhajlo",
    "Mikhail",      "Mykhaylo",
    "Miroslav",     "Myroslav",
    "Miroslava",    "Myroslava",
    "Nadya",        "Nadija",
    "Nadya",        "Nadiya",
    "Natalya",      "Nataliia",
    "Natalya",      "Natallia",
    "Nikolay",      "Mikalai",
    "Nikolay",      "Mikalaj",
    "Nikolay",      "Mikola",
    "Nikolay",      "Mykola",
    "Oksana",       "Aksana",
    "Oleg",         "Aleg",
    "Oleg",         "Aleh",
    "Oleg",         "Oleh",
    "Olesia",       "Alesia",
    "Olesja",       "Alesja",
    "Olessja",      "Alessja",
    "Olessya",      "Alessya",
    "Olesya",       "Alesya",
    "Olga",         "Olha",
    "Olga",         "Volha",
    "Roman",        "Raman",
    "Rostislav",    "Rostyslav",
    "Sergej",       "Serhij",
    "Sergey",       "Sergii",
    "Sergey",       "Sergij",
    "Sergey",       "Sergiy",
    "Sergey",       "Serhei",
    "Sergey",       "Serhii",
    "Sergey",       "Serhiy",
    "Sergey",       "Siarhei",
    "Sergey",       "Siarhej",
    "Sergey",       "Siarhey",
    "Snezhana",     "Sniazhana",
    "Stanislav",    "Stanislau",
    "Svetlana",     "Sviatlana",
    "Svetlana",     "Svitlana",
    "Tatyana",      "Tatsiana",
    "Tatyana",      "Tatsyana",
    "Tatyana",      "Tetiana",
    "Tatyana",      "Tetjana",
    "Tatyana",      "Tetyana",
    "Vadim",        "Vadym",
    "Vadim",        "Vadzim",
    "Valentin",     "Valentyn",
    "Valentina",    "Valentyna",
    "Valentina",    "Valiantsina",
    "Valeriy",      "Valerii",
    "Valeriya",     "Valeryia",
    "Vasil",        "Vasyl",
    "Vera",         "Vira",
    "Veronika",     "Veranika",
    "Viacheslav",   "Viachaslau",
    "Viacheslav",   "Viacheslau",
    "Viktor",       "Viktar",
    "Viktoriya",    "Viktoryia",
    "Vladimir",     "Uladzimir",
    "Vladimir",     "Uladzmir",
    "Vladimir",     "Vladymyr",
    "Vladimir",     "Volodimir",
    "Vladimir",     "Volodymyr",
    "Vladislav",    "Uladzislau",
    "Vladislav",    "Vladyslav",
    "Yaroslav",     "Yaraslau",
    "Yevgenia",     "Yauhenia",
    "Yevgeniy",     "Jauhen",
    "Yevgeniy",     "Yauhen",
    "Yevgeniy",     "Yauheni",
    "Yevgeniya",    "Yauheniya",
    "Yuliya",       "Yuliia",
    "Yuri",         "Juryj",
    "Yuri",         "Yurii",
    "Yuri",         "Yurij",
    NULL,           NULL
  };
 
 
 
int initialize_gender (struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int  i,k,n;
  char *s,*s2,*s3;
 
  if (! (zgo->internal_mode & GENDER_INITIALIZED))
    {
      if ((int)strlen (letters_a_to_z) != 26)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: %s  is not allowed\n",
                  "strlen (letters_a_to_z) != 26");
            }
          return (-1);
        }
 
      if ((int)strlen (letters_a_to_z) != (int)strlen (letters_A_to_Z))
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: %s  is not allowed\n",
                  "strlen(letters_a_to_z) != strlen(letters_a_to_z)");
            }
          return (-1);
        }
      if ((int)strlen (umlaut_lower) != (int)strlen (umlaut_upper))
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: %s  is not allowed\n",
                  "strlen(umlaut_lower) != strlen(umlaut_upper)");
            }
          return (-1);
        }
 
      if ((int)strlen (umlaut_lower) != (int)strlen (umlaut_sort)
      ||  (int)strlen (umlaut_lower) != (int)strlen (umlaut_sort2))
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: %s  is not allowed\n",
                  "strlen(umlaut_sort*) != strlen(umlaut_lower)");
            }
          return (-1);
        }
 
      zgo->internal_mode |= GENDER_INITIALIZED;
 
      /****  generate arrays "zgo->sortchar", "zgo->sortchar2" and "zgo->upperchar"  ****/
      for (i=0; i< HASH_COUNT; i++)
        {
          zgo->sortchar[i] = (char) i;
          zgo->sortchar2[i] = '\0';
          zgo->up_and_conv[i] = (char) i;
          zgo->upperchar[i] = (char) i;
        }
      zgo->sortchar [(unsigned char) '-'] = ' ';
      zgo->sortchar [(unsigned char) '\''] = '´';
      zgo->up_and_conv [(unsigned char) '-'] = ' ';
 
      s = CHARS_TO_IGNORE;
      for (i=0; s[i] != '\0'; i++)
        {
          k = (unsigned char) s[i];
          zgo->sortchar[k] = IS_CHAR_TO_IGNORE;
        }
 
      s = letters_a_to_z;
      s2 = letters_A_to_Z;
 
      for (i=0; s[i] != '\0'; i++)
        {
          k = (unsigned char) s2[i];
          zgo->sortchar[k] = s2[i];
          zgo->up_and_conv[k] = s2[i];
          zgo->upperchar[k] = s2[i];
 
          k = (unsigned char) s[i];
          zgo->sortchar[k] = s2[i];
          zgo->up_and_conv[k] = s2[i];
          zgo->upperchar[k] = s2[i];
        }
 
      s = umlaut_lower;
      s2 = umlaut_upper;
 
      for (i=0; s[i] != '\0'; i++)
        {
          n = (unsigned char) umlaut_conv[i];
 
          k = (unsigned char) s2[i];  /** "s2" **/
          zgo->up_and_conv[k] = (char) n;
          zgo->upperchar[k] = s2[i];
 
          k = (unsigned char) s[i];   /** "s" **/
          zgo->up_and_conv[k] = (char) n;
          zgo->upperchar[k] = s2[i];
        }
 
      s = umlaut_lower;
      s2 = umlaut_sort;
      s3 = umlaut_sort2;
 
      for (i=0; s[i] != '\0'; i++)
        {
          k = (unsigned char) s[i];
          zgo->sortchar[k] = s2[i];
          if (s3[i] != ' ')
            {
              zgo->sortchar2[k] = s3[i];
            }
 
          k = (unsigned char) umlaut_upper[i];
          zgo->sortchar[k] = s2[i];
          if (s3[i] != ' ')
            {
              zgo->sortchar2[k] = s3[i];
            }
        }
 
      /****  initialize hash values for phonetic rules  ****/
      for (i=0; i<17; i++)
        {
    	  zgo->ph_hash_group_begin[i] = -1;
    	  zgo->ph_hash_group_end[i] = -1;
        }
      for (i=0; i< HASH_COUNT; i++)
        {
    	  zgo->ph_rules_hash_del_ins[i] = -1;
    	  zgo->ph_rules_hash_replace[i] = 0;
        }
 
      for (i=0; ph_rules_german[i].text_1 != NULL; i++)
        {
          k = ph_rules_german[i].hash_group;
 
          if (zgo->ph_hash_group_begin[k] < 0)
            {
        	  zgo->ph_hash_group_begin[k] = i;
            }
 
          if (zgo->ph_hash_group_end[k] < 0
          ||  strcmp (ph_rules_german[i].text_2,"") != 0)
            {
        	  zgo->ph_hash_group_end[k] = i;
            }
 
          n = (unsigned char) ph_rules_german[i].text_1[0];
          zgo->ph_rules_hash_replace [n] |= 1 << (k-1);
 
          if (zgo->ph_rules_hash_del_ins[n] < 0
          &&  strcmp (ph_rules_german[i].text_2,"") == 0)
            {
        	  zgo->ph_rules_hash_del_ins[n] = i;
            }
 
          n = (unsigned char) ph_rules_german[i].text_2[0];
          zgo->ph_rules_hash_replace [n] |= 1 << (k-1);
 
          n = (int)strlen (ph_rules_german[i].text_1);
          ph_rules_german[i].len_1 = n;
          n = (int)strlen (ph_rules_german[i].text_2);
          ph_rules_german[i].len_2 = n;
        }
    }
 
  return (0);
}
 
 
static int copycut (char dest[], char src[], int len TSRMLS_DC)
{
  int i,k,n;
  char c;
 
  k = 0;
  n = 0;
  i = 1;
  while ((c = src[k]) != '\0'  &&  c != '\n'
  &&  (k < len-1  ||  len < 0))
    {
      if (c == ' ')
        {
          i--;
        }
      else
        {
          i = 2;
        }
 
      if (i > 0)
        {
          dest[n] = c;
          n++;
        }
      k++;
    }
 
  if (n > 0  &&  dest[n-1] == ' ')
    {
      /****  delete trailing space  ****/
      n--;
    }
  dest[n] = '\0';
 
  return (n);
}
 
 
 
static int strcmp_search (char text[], char text_2[],
     int compare_mode, char umlaut_info, int *diff, struct ze_geocountry_obj *zgo TSRMLS_DC)
 
/****  Internal function for "string compare":          ****/
/****  Compare strings case-insensitive and ignore      ****/
/****  different spellings of umlauts.                  ****/
/****  Return value is the same as for normal "strcmp". ****/
/****  First arg  = "external" name                     ****/
/****  Second arg = "internal" name (with "unicode" chars) ****/
 
/****  Important bit masks for "compare_mode":          ****/
/****  EXPAND_PARAM_1  :   expand umlauts in "text"     ****/
/****  EXPAND_PARAM_2  :   expand umlauts in "text_2"   ****/
/****  COMPARE_ABBREVIATION :  compare abbreviation     ****/
/****                          (e.g. "C." and "Carl")   ****/
 
/****  This function converts "src" to upper case,      ****/
/****  compresses successive blanks and (if necessary)  ****/
/****  expands umlauts, i.e. "ä" is converted to "AE".  ****/
/****  The conversion is done "on demand" using the     ****/
/****  arrays "zgo->sortchar" and "zgo->sortchar2". The second    ****/
/****  array is needed only for umlauts and contains    ****/
/****  null values for all other chars.                 ****/
{
  int  i,i2,k,n;
  char c=0,c2=0;
 
  if (umlaut_info == COMPRESSED_UMLAUTS_FOR_SORTING)
    {
      compare_mode &= ~EXPAND_PARAM_2;
    }
  if (umlaut_info == EXPANDED_UMLAUTS_FOR_SORTING)
    {
      compare_mode |= EXPAND_PARAM_2;
    }
 
  i = 0;
  i2 = 0;
  k = 0;
  while (zgo->sortchar [(unsigned char) *text_2] == IS_CHAR_TO_IGNORE)
    {
      text_2++;
    }
 
  while (*text != '\0'  &&  *text_2 != '\0')
    {
      /****  find next char in first string             ****/
      /****  (without "while (c == IS_CHAR_TO_IGNORE) ...") ****/
      c = '\0';
      if (i != 0)
        {
          if (compare_mode & EXPAND_PARAM_1)
            {
              c = zgo->sortchar2 [(unsigned char) *text];
            }
          i = 0;
          text++;
        }
 
      if (c == '\0')
        {
          c = zgo->sortchar [(unsigned char) *text];
          i = 1;
 
          if ((c == ' '  ||  c == '-')
          &&  (compare_mode & IGNORE_SEPARATOR))
            {
              text++;
              c = zgo->sortchar [(unsigned char) *text];
            }
        }
 
      /****  find next char in second string  ****/
 
      c2 = '\0';
      if (i2 != 0)
        {
          if (compare_mode & EXPAND_PARAM_2)
            {
              c2 = zgo->sortchar2 [(unsigned char) *text_2];
            }
          i2 = 0;
          text_2++;
          while (zgo->sortchar [(unsigned char) *text_2] == IS_CHAR_TO_IGNORE)
            {
              text_2++;
            }
        }
 
      if (c2 == '\0')
        {
          c2 = zgo->sortchar [(unsigned char) *text_2];
          i2 = 1;
 
          if ((c2 == ' '  ||  c2 == '-'  ||  *text_2 == '+')
          &&  (compare_mode & IGNORE_SEPARATOR))
            {
              text_2++;
              while (zgo->sortchar [(unsigned char) *text_2] == IS_CHAR_TO_IGNORE)
                {
                  text_2++;
                }
              c2 = zgo->sortchar [(unsigned char) *text_2];
            }
        }
 
      if (c2 == '+')
        {
          /****  "internal separator" ('+') found  ****/
          c2 = '-';
          if (c == ' '  ||  c == '-')
            {
              c2 = c;
              continue;  /****  important  ****/
            }
          else if (i == 1  &&  i2 == 1
          &&  zgo->sortchar2 [(unsigned char) *text] == '\0')
            {
              text_2++;
              while (zgo->sortchar [(unsigned char) *text_2] == IS_CHAR_TO_IGNORE)
                {
                  text_2++;
                }
              c2 = zgo->sortchar [(unsigned char) *text_2];
            }
        }
 
      /****  compare chars  ****/
      if ((compare_mode & COMPARE_ABBREVIATION)
      &&  c == '.'  &&  c2 != '\0')
        {
          /****  strings match like (e.g.) "St." and "Steven"  ****/
          c = c2;
          break;
        }
      if (c != c2)
        {
          /****  strings don't match  ****/
          break;
        }
 
      if (diff != NULL  &&  i == 1  &&  i2 == 1)
        {
          if (zgo->upperchar [(unsigned char) *text] != zgo->upperchar [(unsigned char) *text_2])
            {
              if (compare_mode & GENDER_COMPARE_EXACT)
                {
                  /****  strings don't match exactly  ****/
                  k |= 1024;
                }
 
              c = zgo->sortchar2 [(unsigned char) *text];
              c2 = zgo->sortchar2 [(unsigned char) *text_2];
 
              if (c != c2)
                {
                  if (c == '\0')
                    {
                      c = zgo->sortchar [(unsigned char) *(text+1)];
                    }
                  if (c2 == '\0')
                    {
                      c2 = zgo->sortchar [(unsigned char) *(text_2+1)];
                    }
                }
 
              if (c != c2)
                {
                  k++;
                  if (c == '\0')
                    {
                      k |= 128;
                    }
                  if (c2 == '\0')
                    {
                      k |= 256;
                    }
                  if (c != '\0'  &&  c2 != '\0')
                    {
                      k |= 1024;
                    }
                }
 
              if (c == '\0')
                {
                  c = zgo->sortchar [(unsigned char) *(text+1)];
 
                  if ((c == ' '  ||  c == '-')
                  &&  (compare_mode & IGNORE_SEPARATOR))
                    {
                      c = zgo->sortchar [(unsigned char) *(text+2)];
                    }
                }
 
              if (c2 == '\0')
                {
                  n = 1;
                  while (zgo->sortchar [(unsigned char) *(text_2+n)] == IS_CHAR_TO_IGNORE)
                    {
                      n++;
                    }
                  c2 = zgo->sortchar [(unsigned char) *(text_2+n)];
                }
 
              if (c != c2
              && ((compare_mode & EXPAND_PARAM_2)
	      ||  ! (compare_mode & GENDER_ALLOW_COMPRESSED_UMLAUTS)))
                {
                  /****  strings don't match (e.g. "Káte" vs "Käte")  ****/
                  k |= 1024;
                }
            }
        }
    }
 
  if (diff != NULL)
    {
      if (c != c2  ||  (k & (128+256)) == 128+256)
        {
          /****  strings don't match (e.g. "Katé" vs "Käte")  ****/
          k |= 1024;
        }
      *diff = k;
    }
 
  if (*text == '\0'  ||  *text_2 == '\0')
    {
      if (c == c2)
        {
          if (i != 0  &&  *text != '\0'
          && (zgo->sortchar2 [(unsigned char) *text] == '\0'
          ||  ! (compare_mode & EXPAND_PARAM_1)))
            {
              text++;
            }
          if (i2 != 0  &&  *text_2 != '\0'
          && (zgo->sortchar2 [(unsigned char) *text_2] == '\0'
          ||  ! (compare_mode & EXPAND_PARAM_2)))
            {
              text_2++;
            }
        }
 
      return ((unsigned char) *text - (unsigned char) *text_2);
    }
 
  return ((unsigned char) c - (unsigned char) c2);
}
 
static void read_line (php_stream *fr, char text[], int len TSRMLS_DC)
{
  text[0] = '\0';
  php_stream_gets(fr, text, len-2);
  text[len-1] = '\0';
}
 
 
static void read_unicode_line (php_stream *fr, char text[], int two_byte_len TSRMLS_DC)
{
  int i,k;
  i = 0;
  while (! php_stream_eof(fr)  &&   i < 2 * two_byte_len - 2)
    {
      k = php_stream_getc (fr);
 
      if (php_stream_eof (fr))
        {
          break;
        }
      k += 256 * php_stream_getc (fr);
 
      if (k == 13)
        {
          /****  check for unicode EOL  ****/
          k = php_stream_getc (fr);
          k += 256 * php_stream_getc (fr);
 
          if (k == 10  ||  php_stream_eof (fr))
            {
              /****  unicode EOL found  ****/
              break;
            }
 
          text[i] = (char) (k%256);
          i++;
          text[i] = (char) (k/256);
          i++;
 
          if (i >= 2 * two_byte_len - 2)
            {
              break;
            }
        }
 
      text[i] = (char) (k%256);
      i++;
      text[i] = (char) (k/256);
      i++;
    }
 
  text[i] = '\0';
  text[i+1] = '\0';
}
 
 
static long binary_search (php_stream *fr, char search_name[],
    int offset, int len_s, int compare_mode, struct ze_geocountry_obj *zgo TSRMLS_DC)
 
/****  Do a binary search for the fist match             ****/
/****  (all lines in dictionary file must have equal length) ****/
/****  Return value == file pos of first match           ****/
/****                  ( < 0L: no match found)           ****/
/****  "compare_mode" is the same as for "strcmp_search" ****/
{
  long p,p1,p2;
  int  i,n;
  char text [MAX_LINE_SIZE+1];
  char temp [MAX_LINE_SIZE+1];
 
  if (zgo->line_size == 0  ||  zgo->record_count == 0L)
    {
      /****  determine length of first line  ****/
      i = php_stream_seek(fr, 0L, SEEK_SET);
      if (i != 0)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: Could not position in dictionary file '%s'\n",
                  zgo->dsn);
            }
          return (-10L);
        }
 
      read_line(fr, text, MAX_LINE_SIZE+1 TSRMLS_CC);
      if (strncmp (text, CHECK_STRING, strlen(CHECK_STRING)) != 0)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              i = (int)strlen (text);
              while (i > 0  &&  (text[i-1] == '\n'  ||  text[i-1] == '\r'))
                {
                  i--;
                }
              text[i] = '\0';
 
              php_printf ("Error: Invalid version of dictionary file '%s'.\n",
                   zgo->dsn);
              php_printf ("File header is:  \"%s\"\n", text);
              php_printf ("(this should be:  \"%s\").\n", CHECK_STRING);
            }
          return (-10L);
        }
 
      /****  get line size  ****/
      zgo->line_size = (int) php_stream_tell(fr);
 
      /****  determine file size  ****/
      i = php_stream_seek(fr,0L, SEEK_END);
      if (i != 0)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: Could not position in dictionary file '%s'.\n",
                  zgo->dsn);
            }
          return (-10L);
        }
 
      p = php_stream_tell(fr);
      zgo->record_count = (long) ((p+1L) / zgo->line_size);
 
      /****  reset cache variables  ****/
      for (i=0; i< CACHE_LOOKBACK_COUNT; i++)
        {
          zgo->cache_p[i] = -1L;
          strcpy (zgo->cache_text[i],"");
        }
    }
 
  /****  start binary search  ****/
  p1 = 0L;
  p2 = zgo->record_count;
  i = -1;
  n = -1;
 
  while (p1 <= p2)
    {
      p = (long) ((p1+p2) / 2L);
      n++;
 
      if (n < CACHE_LOOKBACK_COUNT  &&  p == zgo->cache_p[n])
        {
          /****  read data from cache  ****/
          strcpy (text, zgo->cache_text[n]);
        }
      else
        {
          /****  read from dictionary file  ****/
          i = php_stream_seek(fr, (zgo->line_size * p), SEEK_SET);
          if (i != 0)
            {
              if (zgo->internal_mode & TRACE_GENDER)
                {
                  php_printf ("Could not position in dictionary file '%s'.\n",
                      zgo->dsn);
                }
              return (-1L);
            }
 
          read_line(fr, text, MAX_LINE_SIZE+1 TSRMLS_CC);
 
          if (n < CACHE_LOOKBACK_COUNT)
            {
              /****  copy data into cache  ****/
              zgo->cache_p[n] = p;
              strcpy (zgo->cache_text[n], text);
            }
        }
 
      strcpy (temp,"");
      if (text[0] != '#'  &&  (int)strlen (text) > offset)
        {
          copycut (temp, text+offset, len_s TSRMLS_CC);
        }
 
      if (zgo->internal_mode & TRACE_GENDER)
        {
          php_printf ("Range = line %ld - %ld,  guess = %ld ('%s')\n",
              p1+1L, p2+1L, p+1L, temp);
        }
 
      /****  compare first names  ****/
      i = strcmp_search (search_name, temp, (compare_mode | IGNORE_SEPARATOR),
              text[POS_UMLAUT_INFO], NULL, zgo TSRMLS_CC);
 
      if (i == 0)
        {
          if (p == p1)
            {
              /****  first match has been found  ****/
              break;
            }
          p2 = p;
        }
      else if (i < 0)
        {
          p2 = p - 1L;
        }
      else
        {
          p1 = p + 1L;
          p++;   /**** important  ****/
        }
    }
 
  if (i != 0  &&  (compare_mode & GET_MATCH_OR_NEXT_HIGHER))
    {
      if (i > 0)
        {
          /***  read next line  ****/
          read_line (fr, text, MAX_LINE_SIZE+1 TSRMLS_CC);
          copycut (temp, text+offset, len_s TSRMLS_CC);
        }
      i = 0;
    }
 
  if (i == 0)
    {
      /****  match has been found  ****/
      if (zgo->internal_mode & TRACE_GENDER)
        {
          php_printf ("Result: name '%s' found\n", temp);
        }
      return (zgo->line_size * p);
    }
 
  /****  no match found  ****/
  if (zgo->internal_mode & TRACE_GENDER)
    {
      php_printf ("Result: name '%s' not found\n", search_name);
    }
  return (-1L);
}
 
 
static int delete_chars_to_ignore
    (char dest[], char src[], int flag_sh, int len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i,k,n,t;
 
  /****  delete "CHARS_TO_IGNORE"  ****/
  i = 0;
  k = 0;
  n = 0;
  t = 0;
  while (src[i] != '\0')
    {
      if (zgo->sortchar [(unsigned char) src[i]] == IS_CHAR_TO_IGNORE)
        {
          if ((flag_sh & (1|2))
          &&  src[i] == '<'
          && (src[i+1] == 'S'  ||  src[i+1] == 's')
          && (src[i+2] == ','  ||  src[i+2] == '^')
          &&  src[i+3] == '>'  &&  k < len-3)
            {
              dest[k] = src[i+1];
              k++;
              if (flag_sh & 2)
                {
                  dest[k] = 'c';
                  k++;
                }
              dest[k] = 'h';
              k++;
              t += 3;
              i += 3;
            }
          else if (flag_sh == 0
          &&  src[i] == '<'
          && (src[i+1] == 'S'  ||  src[i+1] == 's')
          && (src[i+2] == 'H'  ||  src[i+2] == 'h')
          &&  src[i+3] == '>'  &&  k < len-1)
             {
               dest[k] = src[i+1];
               k++;
               t += 3;
               i += 3;
             }
          else if (src[i] != '\'')
            {
              n++;
              t++;
            }
          else
            {
              dest[k] = src[i];
              k++;
            }
        }
      else
        {
          dest[k] = src[i];
          k++;
        }
 
      while (t > 0  &&  k < len-1
      &&  src[i] == ' '  &&  src[i+1] == ' '  &&  src[i+2] == ' ')
        {
          dest[k] = ' ';
          k++;
          t--;
        }
 
      i++;
    }
 
  dest[k] = '\0';
  return (n);
}
 
 
 
static void conv_from_unicode_line (char dest[], char src[], int len)
{
  char *s,text[11];
  int i,k,n,x;
 
  /****  conversion from unicode line  ****/
  i = 0;
  k = 0;
  while (k < len-1  &&  (src[i] != '\0'  ||  src[i+1] != '\0'))
    {
      x = (unsigned char) src[i];
      i++;
      x += 256 * (unsigned char) src[i];
      i++;
 
      /*** print char ***/
      if (x < 256)
        {
          dest[k] = (char) x;
          k++;
        }
      else
        {
          for (n=0; (s=umlauts_unicode[n]) != NULL; n += 2)
            {
              if (atoi (s) == x)
                {
                  s = umlauts_unicode[n+1];
 
                  while (k < len-1  &&  *s != '\0')
                    {
                      dest[k] = *s;
                      k++;
                      s++;
                    }
 
                  x = -1;
                  break;
                }
            }
 
          if (x > 0)
            {
              /****  unknown unicode char  ****/
              sprintf (text, "<#%03d>", x);
              s = text;
              while (k < len-1  &&  *s != '\0')
                {
                  dest[k] = *s;
                  k++;
                  s++;
                }
            }
        }
    }
 
  dest[k] = '\0';
}
 
 
 
static void make_unicode_line (char dest[], char src[], int two_byte_len)
{
  char *s;
  int i,k,n,t,x,ls;
 
  /****  make unicode line  ****/
  i = 0;
  k = 0;
  t = 0;
  while (src[i] != '\0'  &&  k < 2*two_byte_len -2)
    {
      x = -1;
      n = 0;
      for (n=1; (s=umlauts_unicode[n]) != NULL; n += 2)
        {
          if (*s == src[i])
            {
              ls = (int) strlen (s);
 
              if (strncmp (s,src+i,ls) == 0)
                {
                  t += ls - 1;
                  x = n - 1;
 
                  break;
                }
            }
        }
 
      if (x >= 0)
        {
          n = atoi (umlauts_unicode[x]);
        }
      else
        {
          n = src[i];
          ls = 1;
        }
 
      dest[k] = (char) (n%256);
      k++;
      dest[k] = (char) (n/256);
      k++;
      i += ls;
 
      while (t > 0  &&  k < 2*two_byte_len -2
      &&  src[i] == ' '  &&  src[i+1] == ' '  &&  src[i+2] == ' ')
        {
          n = ' ';
          dest[k] = (char) (n%256);
          k++;
          dest[k] = (char) (n/256);
          k++;
          t--;
        }
    }
 
  dest[k] = '\0';
  dest[k+1] = '\0';
}
 
 
 
static void conv_from_utf8_line (char dest[], char src[], int len)
{
  char *s,text[11];
  int i,k,n,x;
 
  /****  conversion from utf-8 line  ****/
  i = 0;
  k = 0;
  while (k < len-1  &&  src[i] != '\0')
    {
      /****  read char:
          0xxx xxxx
       or 110x xxxx  10xx xxxx
       or 1110 xxxx  10xx xxxx  10xx xxxx
       or 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx
      ****/
 
      x = (unsigned char) src[i];
      i++;
 
      if (x >= 128)
        {
          if ((x & (128+64+32)) == 128+64)
            {
              x = x & ~(128+64);
 
              n = (unsigned char) src[i];
              i++;
              x = 64 * x + (n-128);
            }
          else if ((x & (128+64+32+16)) == 128+64+32)
            {
              x = x & ~(128+64+32);
 
              n = (unsigned char) src[i];
              i++;
              x = 64 * x + (n-128);
 
              n = (unsigned char) src[i];
              i++;
              x = 64 * x + (n-128);
            }
          else
            {
              /****  unknown char  ****/
              x = -123;
            }
        }
 
      /*** print char ***/
      if (x < 0)
        {
          /****  unknown char  ****/
          dest[k] = '.';
          k++;
        }
      else if (x < 256)
        {
          dest[k] = (char) x;
          k++;
        }
      else
        {
          for (n=0; (s=umlauts_unicode[n]) != NULL; n += 2)
            {
              if (atoi (s) == x)
                {
                  s = umlauts_unicode[n+1];
 
                  while (k < len-1  &&  *s != '\0')
                    {
                      dest[k] = *s;
                      k++;
                      s++;
                    }
 
                  x = -1;
                  break;
                }
            }
 
          if (x > 0)
            {
              /****  unknown unicode char  ****/
              sprintf (text, "<#%03d>", x);
              s = text;
              while (k < len-1  &&  *s != '\0')
                {
                  dest[k] = *s;
                  k++;
                  s++;
                }
            }
        }
    }
 
  dest[k] = '\0';
}
 
 
 
static void make_utf8_line (char dest[], char src[], int len)
{
  char *s;
  int i,k,n,t,x,ls;
 
  /****  make utf-8 line  ****/
  i = 0;
  k = 0;
  t = 0;
  while (src[i] != '\0'  &&  k < len-1)
    {
      x = -1;
      n = 0;
      for (n=1; (s=umlauts_unicode[n]) != NULL; n += 2)
        {
          if (*s == src[i])
            {
              ls = (int) strlen (s);
 
              if (strncmp (s,src+i,ls) == 0)
                {
                  t += ls - 1;
                  x = n - 1;
 
                  break;
                }
            }
        }
 
      if (x >= 0)
        {
          n = atoi (umlauts_unicode[x]);
        }
      else
        {
          n = (unsigned char) src[i];
          ls = 1;
        }
 
      if (n < 128)
        {
          /****  "simple" char  ****/
          dest[k] = (char) n;
          k++;
        }
      else if (n < 2048  &&  k < len-2)
        {
          /****  "European" utf8 char  ****/
          x = 128+64 + (int)(n/64);
          dest[k] = (char) x;
          k++;
 
          x = 128 + (n%64);
          dest[k] = (char) x;
          k++;
        }
      else if (k < len-3)
        {
          /****  other utf8 char  ****/
          x = 128+64+32 + (int)(n/4096);
          dest[k] = (char) x;
          k++;
 
          n = n % 4096;
          x = 128 + (int)(n/64);
          dest[k] = (char) x;
          k++;
 
          x = 128 + (n%64);
          dest[k] = (char) x;
          k++;
        }
      i += ls;
 
      while (t > 0  &&  k < len-1
      &&  src[i] == ' '  &&  src[i+1] == ' '  &&  src[i+2] == ' ')
        {
          dest[k] = ' ';
          k++;
          t--;
        }
    }
 
  dest[k] = '\0';
}
 
 
 
 
 
/************************************************************/
/**** Levenshtein function and the like  ********************/
/************************************************************/
 
 
static int up_conv_g (char dest[], char src[], int len, struct ze_geocountry_obj *zgo TSRMLS_DC)
 
/****  Function converts "src" to upper case.               ****/
/****  The conversion is done using the array "up_and_conv" ****/
/****  (len = size of "dest" incl. '\0').                   ****/
/****  This function returns "strlen (dest)".               ****/
{
  int n;
 
  if (! (zgo->internal_mode & GENDER_INITIALIZED))
    {
      n = initialize_gender(zgo TSRMLS_CC);
 
      if (n < 0  ||  ! (zgo->internal_mode & GENDER_INITIALIZED))
        {
          return (INTERNAL_ERROR_GENDER);
        }
    }
 
  n = 0;
  while (*src != '\0'  &&  n < len-1)
    {
      /****  find next char in "src"  ****/
      dest[n] = zgo->up_and_conv [(unsigned char) *src];
      n++;
      src++;
    }
 
  dest[n] = '\0';
  return (n);
}
 
 
 
 
static int calculate_limit_g (char pattern[], char text[])
/****  calculate limit for the Levenshtein function  ****/
{
  char *s;
  int  i,j,k,n,x;
  int  max_points, limit;
  int  asterisk_found, unknown;
 
  char wildcard_any_string = MATCHES_ANY_STRING;
  char wildcard_any_char = MATCHES_ANY_CHAR;
 
  if (pattern == NULL)
    {
     pattern = text;
     text = NULL;
    }
  max_points = 0;
  unknown = 0;
 
  for (j=1; j<=2; j++)
    {
     s = pattern;
     if (j == 2)
       {
        s = text;
       }
 
     if (s == NULL)
       {
        continue;
       }
 
     asterisk_found = 0;
     x = 0;
     i = 0;
     while (s[i] != '\0')
       {
        /****  look for word separators and wildcard_any_string/char  ****/
        k = 0;
        n = 0;
        while (s[i] == wildcard_any_string
        ||  s[i] == '.'  ||  s[i] == wildcard_any_char)
          {
           if (s[i] == wildcard_any_string
           ||  s[i] == '.')
             {
              asterisk_found = 1;
              n = 300;
             }
           if (s[i] == wildcard_any_char)
             {
              k += 100;
             }
 
           if (s[i+1] != wildcard_any_string
           &&  s[i+1] != '.'  &&  s[i] != wildcard_any_char)
             {
              break;
             }
           i++;
          }
 
        if (n > k)
          {
           k = n;
          }
        unknown += k;
 
        if (strchr (WORD_SEPARATORS, s[i]) == NULL)
          {
           /****  count "letters"  ****/
           x += 100;
          }
        else if (s[i] != wildcard_any_string
        &&  s[i] != '.'  &&  s[i] != wildcard_any_char)
          {
           x += 40;
          }
 
        i++;
       }
 
     if (asterisk_found  &&  unknown < 600 - x)
       {
        unknown = 600 - x;
       }
 
     if (j == 1  ||  x < max_points)
       {
        max_points = x;
       }
    }
 
  limit = max_points;
  if (limit > 0)
    {
     if (limit > 1200)
       {
        limit = 1200;
       }
 
     limit = (int) ((limit +40) / 3);
    }
 
  return (limit);
}
 
 
 
static void print_number_g (char *text, int number, int mode)
{
  char *s;
 
  if (mode >= 0  &&  number == MATCHES_ALL)
    {
     printf ("%s = %s", text, "MATCHES_ALL");
     return;
    }
 
  s = "";
  if (number < 0)
    {
     s = "-";
     number = -number;
    }
  printf ("%s = %s%d", text,s, (int)(number/100));
 
  if (number % 100 != 0)
    {
     number = number % 100;
     printf (".%d", (int)(number/10));
 
     if (number % 10 != 0)
       {
        printf ("%d", number % 10);
       }
    }
}
 
 
 
static int lev_diff_g
   (char *pattern, char *text, int limit, int *trigrams, int run_mode, struct ze_geocountry_obj *zgo TSRMLS_DC)
 
/****  Function for calculating Levenshtein distance         ****/
/****  (in hundredths of points) with respect to wildcards   ****/
/****  and phonetic rules.                                   ****/
/****  "text" is compared with "pattern"; allowed wildcards  ****/
/****  are <wildcard_any_string> and <wildcard_any_char>.    ****/
/****  (function works for arbitrary string lengths)         ****/
 
/****  This function does not discriminate between           ****/
/****  lower / upper char, including umlauts                 ****/
/****  and is also "symmetric": shorter string = pattern;    ****/
/****  "limit" may be re-calculated                          ****/
 
/****  If "limit >= 0", the function returns "no match",     ****/
/****  if the minimum distance (as given by "col_min")       ****/
/****  is greater than limit.                                ****/
 
/****  Available values for "run_mode" (bit mask):           ****/
/****  LEV_COMPARE_NORMAL :  "normal" Levenshtein distance   ****/
/****  LEV_G_SKIP_UPEXPAND  :  do not convert strings to       ****/
/****                       upper char (incl. umlaut_expand) ****/
/****  LEV_G_COMPARE_GERMAN :  use simple phonetic algorithm   ****/
/****                        for German                      ****/
/****  TRACE_LEV_G          :  activate trace option           ****/
{
 int  col_min,
      lim_1,
      p,q,r,
      lt,lp,
      d1,d2,
      i,k,n,
      x1,x2,x3,x4,
      y1,y2,y3;
 int  *d;
 int  *pos;
 int  *rep;
 int  *tri;
 char *pp;
 char *tt;
 char ct,cp,cp2,
      *s1,*s2,*s3,*s4;
 
 char wildcard_any_string = MATCHES_ANY_STRING;
 char wildcard_any_char = MATCHES_ANY_CHAR;
 
 static char p_string[51],
             t_string[51];
 static int  d_array[51];
 static int  pos_array[51];
 static int  rep_array[51];
 static int  tri_array[51];
 
 d = d_array;
 pos = pos_array;
 rep = rep_array;
 tri = tri_array;
 pp = p_string;
 tt = t_string;
 
 if (! (zgo->internal_mode & GENDER_INITIALIZED))
   {
     i = initialize_gender(zgo TSRMLS_CC);
 
     if (i < 0  ||  ! (zgo->internal_mode & GENDER_INITIALIZED))
       {
         if (zgo->internal_mode & TRACE_GENDER)
           {
             printf ("Error: initialization failed\n");
           }
         return (INTERNAL_ERROR_GENDER);
       }
   }
 
 if (run_mode & LEV_G_SKIP_UPEXPAND)
   {
    lt = (int) strlen (text);
    lp = (int) strlen (pattern);
   }
 else
   {
    /****  convert strings to upper char  ****/
    lt = up_conv_g (tt, text, 51, zgo TSRMLS_CC);
    lp = up_conv_g (pp, pattern, 51, zgo TSRMLS_CC);
 
    if (lt >= 50)
      {
       lt = (int) strlen (text) + 2;
       tt = (char *) malloc ((unsigned) lt);
 
       if (tt == NULL)
         {
          if (run_mode & (TRACE_GENDER | TRACE_LEV_G))
            {
             printf ("Error: \"malloc\" for %d Bytes failed.\n", lt);
            }
          return (10000);
         }
       lt = up_conv_g (tt, text, lt, zgo TSRMLS_CC);
      }
 
    if (lp >= 50)
      {
       lp = (int) strlen (pattern) + 2;
       pp = (char *) malloc ((unsigned) lp);
 
       if (pp == NULL)
         {
          if (run_mode & (TRACE_GENDER | TRACE_LEV_G))
            {
             printf ("Error: \"malloc\" for %d Bytes failed.\n", lp);
            }
          if (tt != t_string)
            {
             free (tt);
            }
          return (10000);
         }
       lp = up_conv_g (pp, pattern, lp, zgo TSRMLS_CC);
      }
 
    text = tt;
    pattern = pp;
   }
 
 if (lt < 0  ||  lp < 0)
   {
     /****  internal error  ****/
     if (run_mode & (TRACE_GENDER | TRACE_LEV_G))
       {
        printf ("Internal error: could not initialize conv strings.\n");
       }
     return (10000);
   }
 
 if (3*lt < 2*lp
 && (strchr (text,wildcard_any_string) != NULL
 ||  strchr (text,' ') != NULL  ||  strchr (text,'.') != NULL))
   {
    /****  switch "text" and "pattern"  ****/
    s1 = text;
    text = pattern;
    pattern = s1;
    i = lt;
    lt = lp;
    lp = i;
 
    if (limit > 0  &&  limit < 10000)
      {
       /****  re-calculate limit  ****/
       x1 = calculate_limit_g (pattern, NULL);
       if (x1 < limit)
         {
           limit = x1;
         }
      }
   }
 
 if (limit < 0)
   {
    limit = 0;
   }
 lim_1 = limit;
 if (lim_1 >= 7  &&  lim_1 <= 30)
   {
     lim_1 += 10;
   }
 
 if (lt >= 50)
   {
    i = (lt+1) * sizeof (int);
                      /***  factor "4" includes arrays "pos", "rep" and "tri"  ***/
    d = (int *) malloc ((unsigned) (4*i));
 
    if (d == NULL)
      {
       if (run_mode & (TRACE_GENDER | TRACE_LEV_G))
         {
          printf ("Error: \"malloc\" for %d Bytes failed.\n", (4*i));
         }
       if (tt != t_string  &&  tt != p_string)
         {
          free (tt);
         }
       if (pp != p_string  &&  pp != t_string)
         {
          free (pp);
         }
       return (10000);
      }
 
    pos = d + i;
    rep = d + (2*i);
    tri = d + (3*i);
   }
 
 if (run_mode & TRACE_LEV_G)
   {
    printf ("\nLevenshtein:  Strings = '%s'%s'", pattern, text);
    print_number_g (",  Limit", limit,-1);
    printf ("\n\n         ");
 
    for (k=1; k<=lt; k++)
      {
       printf ("   '%c'", text[k-1]);
      }
    printf ("\n");
   }
 
 /****  calculate initial values ( = zero'th column)  ****/
 d[0] = 0;
 pos[0] = -1;
 rep[0] = 10000;
 tri[0] = 0;
 for (k=1; k<=lt; k++)
   {
    d[k] = 10000;
    pos[k] = -1;
    rep[k] = 10000;
    tri[k] = 0;
   }
 
 /****  calculate distance matrix  ****/
 for (i=0; i<=lp; i++)
   {
    cp = (i == 0) ?  '\0' : *(pattern +i-1);
 
    if (cp == wildcard_any_string  ||  cp == '.')
      {
        p = 0;
        q = 0;
        r = 0;
      }
    else
      {
        p = (cp == wildcard_any_char) ?  0 : 100;
        q = 100;
        r = 100;
      }
 
    if (q > 0)
      {
       /****  calculate error points for "delete"  ***/
       if (i >= 2  &&  cp == pattern[i-2])
         {
          /****  "<x><x>"  -->  "<x>"  ****/
          q = 5;
         }
 
       switch (cp)
         {
          case ' ':
          case '-':
          case '+':
          case '/':
          case '&':
          case '\'':  q = 5;
                 break;
         }
 
       if ((run_mode & LEV_G_COMPARE_GERMAN)
       &&  i > 0)
         {
           /****  check phonetic rules for "delete"  ***/
           x3 = zgo->ph_rules_hash_del_ins [(unsigned char) cp];
 
           if (x3 >= 0)
             {
               /****  evaluate phonetic rules  ****/
               while (ph_rules_german[x3].text_2 != NULL
               &&  strcmp (ph_rules_german[x3].text_2,"") == 0)
                 {
                   s1 = ph_rules_german[x3].text_1;
                   y1 = ph_rules_german[x3].len_1;
 
                   s4 = pattern+i-1;
 
                   if (cp == *s1  &&  y1 > 0)
                     {
                       /****  check phonetic rule  ****/
                       if (strncmp (s4, s1,y1) == 0)
                         {
                           y3 = 0;
 
                           if (pos[y3] < 0
                           ||  rep[y3]-100 > d[0] + ph_rules_german[x3].ph_diff)
                             {
                               pos[y3] = i + y1-1;
                               rep[y3] = d[0] + ph_rules_german[x3].ph_diff;
 
                               if (y1 >= 2  &&  y3 < lt
                               &&  ph_rules_german[x3].ph_diff >= 100)
                                 {
                                   pos[y3] |= 2048;
                                 }
                             }
                         }
                     }
 
                   x3++;
                 }
             }
         }
      }
 
    d2 = d[0];
    d[0] = (i == 0) ?  0 : (d2 + q);
 
    if ((run_mode & LEV_G_COMPARE_GERMAN)
    &&  (pos[0] & ~(1024+2048)) == i)
      {
        x1 = rep[0];
        if (x1 < d[0])
          {
            d[0] = x1;
          }
 
        if ((pos[0] & 2048)  &&  rep[1] > rep[0])
          {
            pos[1] = pos[0] & ~2048;
            rep[1] = rep[0];
          }
        pos[0] = -1;
        rep[0] = 10000;
      }
 
    col_min = d[0];
 
    for (k=1; k<=lt; k++)
      {
       /****  d[k] = minimum of three numbers  ****/
       d1 = d2;
       d2 = d[k];
       ct = text[k-1];
       n = (cp == ct) ?  0 : p;
 
       if (tri[k-1] > tri[k])
         {
          tri[k] = tri[k-1];
         }
 
       if (n == 0  &&  p > 0  &&  i <= lp-2  &&  k <= lt-2)
         {
          /****  look for trigram  ****/
          if (pattern[i] == text[k]
          &&  pattern[i+1] == text[k+1]
          &&  text[k] != wildcard_any_string  &&  text[k] != '.'
          &&  text[k] != wildcard_any_char
          &&  text[k+1] != wildcard_any_string  &&  text[k+1] != '.'
          &&  text[k+1] != wildcard_any_char)
            {
              tri[k] += 100;
            }
         }
 
       if (i >= 2  &&  k >= 2  &&  n != 0  &&  limit > 0)
         {
          /****  look for transpositions (e.g. "AB" --> "BA")  ****/
          cp2 = pattern[i-2];
 
          if (cp == text[k-2]  &&  cp2 == text[k-1]
          &&  cp != wildcard_any_string  &&  cp != '.'
           &&  cp != wildcard_any_char
          &&  cp2 != wildcard_any_string  &&  cp2 != '.'
           &&  cp2 != wildcard_any_char)
            {
              /****  transposition found  ****/
              n = 0;
            }
         }
 
       if (n > 0  &&  limit > 0)
         {
          /****  calculate error points for "replacement"  ***/
          switch (cp)
            {
             case ' ':
             case '-':
             case '+':
             case '/':
             case '&':
             case '\'': if (ct == ' '  ||  ct == '-'  ||  ct == '+'
                        ||  ct == '/'  ||  ct == '&'  ||  ct == '\''
                        ||  ct == wildcard_any_string)
                          {
                           /****  separator found  ****/
                           n = 5;
                          }
                   break;
            }
         }
 
       if ((run_mode & LEV_G_COMPARE_GERMAN)
       &&  i > 0)
         {
           /****  check phonetic rules for "replacement"  ***/
           x1 = (zgo->ph_rules_hash_replace [(unsigned char) ct])
               & (zgo->ph_rules_hash_replace [(unsigned char) cp]);
 
           x2 = 1;
 
           if (x1 != 0)
             {
               if (! (x1 & (1+2+4+8+16+32+64+128)))
                 {
                   x1 = x1 >> 8;
                   x2 += 8;
                 }
               if (! (x1 & (1+2+4+8)))
                 {
                   x1 = x1 >> 4;
                   x2 += 4;
                 }
               if (! (x1 & (1+2)))
                 {
                   x1 = x1 >> 2;
                   x2 += 2;
                 }
             }
 
           while (x1 != 0)
             {
               if (x1 & 1)
                 {
                   /****  hash_group found  ****/
                   x3 = zgo->ph_hash_group_begin[x2];
                   x4 = zgo->ph_hash_group_end[x2];
 
                   while (x3 <= x4  &&  x3 >= 0)
                     {
                       s1 = ph_rules_german[x3].text_1;
                       s2 = ph_rules_german[x3].text_2;
 
                       y1 = ph_rules_german[x3].len_1;
                       y2 = ph_rules_german[x3].len_2;
 
                       y3 = y2;
                       if (y1 > y3)
                         {
                           y3 = y1;
                         }
 
                       s3 = text+k-1;
                       s4 = pattern+i-1;
 
                       if (ct == *s1  &&  cp == *s2
                       &&  y1 > 0  &&  y2 > 0)
                         {
                           /****  check phonetic rule  ****/
                           if (strncmp (s3, s1,y1) == 0
                           &&  strncmp (s4, s2,y2) == 0
                           &&  strncmp (s3,s4, y3) != 0)
                             {
                               y3 = k + y1-1;
 
                               if (pos[y3] < 0
                               ||  rep[y3] > d1 + ph_rules_german[x3].ph_diff)
                                 {
                                   pos[y3] = i + y2-1;
                                   rep[y3] = d1 + ph_rules_german[x3].ph_diff;
                                 }
                             }
                         }
 
                       if (ct == *s2  &&  cp == *s1
                       &&  y1 > 0  &&  y2 > 0)
                         {
                           /****  check phonetic rule  ****/
                           if (strncmp (s3, s2,y2) == 0
                           &&  strncmp (s4, s1,y1) == 0
                           &&  strncmp (s3,s4, y3) != 0)
                             {
                               y3 = k + y2-1;
 
                               if (pos[y3] < 0
                               ||  rep[y3] > d1 + ph_rules_german[x3].ph_diff)
                                 {
                                   pos[y3] = i + y1-1;
                                   rep[y3] = d1 + ph_rules_german[x3].ph_diff;
                                 }
                             }
                         }
 
                       x3++;
                     }
                 }
 
               x1 = x1 >> 1;
               x2++;
             }
 
           /****  check phonetic rules for "delete"  ***/
           x3 = zgo->ph_rules_hash_del_ins [(unsigned char) cp];
 
           if (x3 >= 0)
             {
               while (ph_rules_german[x3].text_2 != NULL
               &&  strcmp (ph_rules_german[x3].text_2,"") == 0)
                 {
                   s1 = ph_rules_german[x3].text_1;
                   y1 = ph_rules_german[x3].len_1;
 
                   s4 = pattern+i-1;
 
                   if (cp == *s1  &&  y1 > 0)
                     {
                       /****  check phonetic rule  ****/
                       if (strncmp (s4, s1,y1) == 0)
                         {
                           y3 = k;
 
                           if (pos[y3] < 0
                           ||  rep[y3]-100 > d2 + ph_rules_german[x3].ph_diff)
                             {
                               pos[y3] = i + y1-1;
                               rep[y3] = d2 + ph_rules_german[x3].ph_diff;
 
                               if (y1 >= 2  &&  y3 < lt
                               &&  ph_rules_german[x3].ph_diff >= 100)
                                 {
                                   pos[y3] |= 2048;
                                 }
                             }
                         }
                     }
 
                   x3++;
                 }
             }
         }
 
       if (run_mode & LEV_G_COMPARE_GERMAN)
         {
           /****  check phonetic rules for "insert"  ***/
           x3 = zgo->ph_rules_hash_del_ins [(unsigned char) ct];
 
           if (x3 >= 0)
             {
               while (ph_rules_german[x3].text_2 != NULL
               &&  strcmp (ph_rules_german[x3].text_2,"") == 0)
                 {
                   s1 = ph_rules_german[x3].text_1;
                   y1 = ph_rules_german[x3].len_1;
 
                   s3 = text+k-1;
 
                   if (ct == *s1  &&  y1 > 0)
                     {
                       /****  check phonetic rule  ****/
                       if (strncmp (s3, s1,y1) == 0)
                         {
                           y3 = k + y1-1;
 
                           if (pos[y3] < 0
                           ||  rep[y3] > d[k-1] + ph_rules_german[x3].ph_diff)
                             {
                               pos[y3] = i;
                               rep[y3] = d[k-1] + ph_rules_german[x3].ph_diff;
 
                               if (y1 >= 2  &&  i < lp
                               &&  ph_rules_german[x3].ph_diff >= 100)
                                 {
                                   pos[y3] |= 1024;
                                 }
                             }
                         }
                     }
 
                   x3++;
                 }
             }
         }
 
       if (n >= 100
       &&  zgo->up_and_conv [(unsigned char) ct]
        == zgo->up_and_conv [(unsigned char) cp])
         {
           n = 20;
           if (ct == zgo->up_and_conv [(unsigned char) cp]
           ||  cp == zgo->up_and_conv [(unsigned char) ct])
             {
               n = 5;
             }
         }
 
       x1 = d1 + n;
       x2 = d2 + q;
 
       if (x2 < x1)
         {
          x1 = x2;
         }
 
       n = r;
       if (n > 0  &&  limit > 0)
         {
          /****  calculate error points for "insert"  ***/
          if (k >= 2  &&  ct == text[k-2])
            {
             /****  "<x>"  -->  "<x><x>"  ****/
             n = 5;
            }
 
          if (ct == wildcard_any_string)
            {
             n = 5;
            }
 
          switch (ct)
            {
          /** case wildcard_any_string: **/
             case ' ':
             case '-':
             case '+':
             case '/':
             case '&':
             case '\'':  n = 5;
                   break;
            }
         }
 
       x2 = d[k-1] + n;
       d[k] = (x1 < x2) ?  x1 : x2;
 
       if ((run_mode & LEV_G_COMPARE_GERMAN)
       &&  (pos[k] & ~(1024+2048)) == i)
         {
           x1 = rep[k];
           if (x1 < d[k])
             {
               d[k] = x1;
             }
 
           if (pos[k] & 1024)
             {
               pos[k] = (pos[k] + 1) & ~(1024+2048);
             }
           else
             {
               if ((pos[k] & 2048)  &&  rep[k+1] > rep[k])
                 {
                   pos[k+1] = pos[k] & ~2048;
                   rep[k+1] = rep[k];
                 }
               pos[k] = -1;
               rep[k] = 10000;
             }
         }
 
       if (d[k] < col_min)
         {
          col_min = d[k];
         }
      }
 
    if (run_mode & TRACE_LEV_G)
      {
       if (i == 0)
         {
          printf ("    ");
         }
       else
         {
          printf ("'%c' ",cp);
         }
 
       for (k=0; k<=lt; k++)
         {
          if (d[k] <= limit)
            {
             printf (" %2d.%02d", (int)(d[k]/100), d[k]%100);
            }
          else
            {
             printf ("  ----");
            }
         }
       printf ("\n");
      }
 
    if (col_min > limit + tri[lt])
      {
       break;
      }
   }
 
 
 if (d[lt] >= 150  &&  d[lt] <= 800
 &&  d[lt] <= lim_1)
   {
     /****  check for multiple insert  ****/
     k = lt - lp;
     s1 = text;
     s2 = pattern;
 
     if (k < 0)
       {
         k = - k;
         s1 = pattern;
         s2 = text;
       }
 
     i = 0;
     while (s1[i] == s2[i]  &&  s1[i] != '\0')
       {
         i++;
       }
 
     if (strcmp (s1+i+k, s2+i) == 0)
       {
         /****  multiple insert  ****/
         if ((i > 0  &&  s2[i] == '\0'
          && (s1[i] == ' '  ||  s1[i] == '-'))
         ||  (i == 0  &&  k >= 2
          && (s1[k-1] == ' '  ||  s1[k-1] == '-')))
           {
             k = 50 * k;    /** 100 * (k-1 + 1) / 2 **/
 
             if (k < d[lt])
               {
                 d[lt] = k;
 
                 if (run_mode & TRACE_LEV_G)
                   {
                     printf ("\nMultiple insert found (1):  lev = %d.%02d\n",
                         (int) (d[lt] / 100), d[lt] % 100);
                   }
               }
           }
         else if (i >= 2  &&  k >= 3)
           {
             d[lt] -= 50;
             if (k >= 5)
               {
                 d[lt] -= 50;
               }
 
             if (run_mode & TRACE_LEV_G)
               {
                 printf ("\nMultiple insert found (2):  lev = %d.%02d\n",
                     (int) (d[lt] / 100), d[lt] % 100);
               }
           }
       }
     else
       {
         i = lt;
         k = lp;
         while (text[i-1] == pattern[k-1]  &&  i > 0  &&  k > 0)
           {
             i--;
             k--;
           }
 
         if (100 * i == d[lt]  ||  100 * k == d[lt])
           {
             i = 0;
             while (text[i] == pattern[i]  &&  text[i] != '\0')
               {
                 i++;
               }
 
             if (i == 0)
               {
                 /****  begin of strings differ       ****/
                 /****  (e.g. "Hedwig" and "Ludwig")  ****/
                 d[lt] += 50;
 
                 if (run_mode & TRACE_LEV_G)
                   {
                     printf ("\nBegin of string differ:  lev = %d.%02d\n",
                         (int) (d[lt] / 100), d[lt] % 100);
                   }
               }
           }
       }
   }
 
 /****  check trigrams  ****/
 x1 = 15000;
 if (tri[lt] >= d[lt]  &&  d[lt] > 0)
   {
     i = (int) (15 * (lt+lp));
 
     x1 = i - (int)(tri[lt] / 3);
     if (x1 < 100)
       {
         x1 = 100;
       }
   }
 else if (tri[lt] >= 200  &&  d[lt] > 500)
   {
     x1 = 1000 - (int)(tri[lt] / 2);
     if (x1 < 400)
       {
         x1 = 400;
       }
   }
 
 if (x1 <= limit + 100  &&  x1 < d[lt] - 100)
   {
     d[lt] = x1 + 100;
     if (d[lt] > limit)
       {
         limit = d[lt];
       }
 
     if (run_mode & TRACE_LEV_G)
       {
         printf ("\nTrigams:  lev = %d.%02d\n",
             (int)(d[lt]/100), d[lt]%100);
       }
   }
 
 /****  calculate final result  ****/
 k = tri[lt];
 n = d[lt];
 if (n > limit  &&  n > k)
   {
    n = 10000;
   }
 
 if (tt != t_string  &&  tt != p_string)
   {
    free (tt);
   }
 if (pp != p_string  &&  pp != t_string)
   {
    free (pp);
   }
 if (d != d_array)
   {
    free (d);
   }
 
 if (run_mode & TRACE_LEV_G)
   {
    printf ("\n");
    printf ("Levenshtein distance = %d.%02d\n", (int)(n/100), (n%100));
    print_number_g ("trigram count", k,-1);
    printf ("\n\n");
   }
 
 if (trigrams != NULL)
   {
    *trigrams = k;
   }
 return (n);
}
 
 
static int conv_internal_result (char *text, int compare_mode)
{
  if (compare_mode & SEARCH_GENDER)
    {
      if (compare_mode & SEARCH_FIRST)
        {
          if (strncmp (text,"1F",2) == 0)  return (IS_FEMALE);
          if (strncmp (text,"1M",2) == 0)  return (IS_MALE);
        }
 
      if (strncmp (text,"P ",2) == 0)  return (IS_COUNTRY);
      if (strncmp (text,"C ",2) == 0)  return (IS_CITY);
      if (strncmp (text,"F ",2) == 0)  return (IS_FEMALE);
      if (strncmp (text,"1F",2) == 0)  return (IS_MOSTLY_FEMALE);
      if (strncmp (text,"?F",2) == 0)  return (IS_MOSTLY_FEMALE);
 
      if (strncmp (text,"M ",2) == 0)  return (IS_MALE);
      if (strncmp (text,"1M",2) == 0)  return (IS_MOSTLY_MALE);
      if (strncmp (text,"?M",2) == 0)  return (IS_MOSTLY_MALE);
 
      if (strncmp (text,"? ",2) == 0)  return (IS_UNISEX_NAME);
 
      return (NAME_NOT_FOUND);
    }
 
  if (compare_mode & SEARCH_EQUIV)
    {
      if (strncmp (text,"= ",2) == 0)  return (EQUIVALENT_NAMES);
 
      return (NAME_NOT_FOUND);
    }
 
  return (INTERNAL_ERROR_GENDER);
}
 
 
static int get_frequency (char c)
{
  int x;
 
  x = 0;
  if (isdigit ((unsigned char) c))
    {
      x = (unsigned char) c - '0';
    }
  else if (c != ' ')
    {
      x = (unsigned char) c - 'A' + 10;
      if (x > 13  ||  x < 0)
        {
          x = 13;
        }
    }
 
  return (x);
}
 
 
static int multiply_frequency (int a, int b)
{
  int x;
 
  if (a > 13)
    {
      a = 13;
    }
  if (b > 13)
    {
      b = 13;
    }
 
  if (a < 0)
    {
      return (b);
    }
  if (b < 0)
    {
      return (a);
    }
 
  x = a + b - 15;
  if (x < 0)
    {
      x = 0;
    }
  if (x == 0  &&  (a*b > 0  ||  a+b >= 9))
    {
      x = 1;
    }
 
  return (x);
}
 
 
 
static void trace_info_into_buffer
    (char text[], char name[], char name2[], int res, char data[], struct ze_geocountry_obj *zgo TSRMLS_DC)
/****  "calculate" trace info  ****/
{
  int  i,x,len;
  int  pos_0;
  int  freq,freq_2;
  long p,count;
  char *s,*s2;
 
  switch (res)
    {
      case IS_FEMALE        :  s = "is female";            break;
      case IS_MOSTLY_FEMALE :  s = "is mostly female";     break;
      case IS_MALE          :  s = "is male";              break;
      case IS_MOSTLY_MALE   :  s = "is mostly male";       break;
      case IS_A_COUPLE      :  s = "is a couple";          break;
      case IS_UNISEX_NAME   :  s = "is unisex name";       break;
 
      case EQUIVALENT_NAMES :  s = "names are equivalent"; break;
      case NOT_EQUAL_NAMES  :  s = "names are not equal";  break;
      case NAME_NOT_FOUND   :  s = "name not found";       break;
      case ERROR_IN_NAME    :  s = "error in name";        break;
 
      case INTERNAL_ERROR_GENDER :  s = "internal error";  break;
      default                    :  s = "unknown error";   break;
    }
 
  strcpy (zgo->trace_buffer, "");
  len = 0;
 
  if (name2 != NULL)
    {
	  len += sprintf (zgo->trace_buffer+len, "%s '%s'%s':  '%s'", text, name,name2, s);
    }
  else if (text[0] != '\0')
    {
	  len += sprintf (zgo->trace_buffer+len, "%s '%s':  '%s'", text, name, s);
    }
  else
    {
	  len += sprintf (zgo->trace_buffer+len, "%s:  '%s'", name, s);
    }
 
  if (data != NULL  ||  (zgo->internal_mode & GENDER_GET_COUNTRY))
    {
      /****  print country  ****/
      freq = 0;
      freq_2 = 0;
      pos_0 = -1;
      count = 1L;
 
      for (i=0; zgo->gc_data[i].country_text != NULL; i++)
        {
          x = 0;
          if (data != NULL  &&  *data != '\0')
            {
              x = get_frequency (data[zgo->gc_data[i].pos]);
            }
          else if ((zgo->internal_mode & GENDER_GET_COUNTRY)
          &&  zgo->gc_data[i].n > 0)
            {
              x = (int) (zgo->gc_data[i].n % 16);
            }
 
          if (x > freq)
            {
              freq = x;
            }
        }
 
      freq_2 = (int)((freq+1) / 2);
      if (freq_2 <= 1)
        {
          freq_2 = freq;
        }
      if (freq_2 < freq - 3)
        {
          freq_2 = freq - 3;
        }
      freq = 0;
 
      s = " (country =";
      for (i=0; zgo->gc_data[i].country_text != NULL; i++)
        {
          x = 0;
 
          if (data != NULL  &&  *data != '\0')
            {
              x = get_frequency (data[zgo->gc_data[i].pos]);
            }
          else if ((zgo->internal_mode & GENDER_GET_COUNTRY)
          &&  zgo->gc_data[i].n > 0)
            {
              x = (int) (zgo->gc_data[i].n % 16);
            }
 
          if (x > 0)
            {
              if (! (zgo->internal_mode & GENDER_TRACE_ONE_COUNTRY_ONLY))
                {
                  if (zgo->internal_mode & GENDER_TRACE_SHORT_COUNTRY_NAME)
                    {
                      s2 = zgo->gc_data[i].country_short;
                    }
                  else
                    {
                      s2 = zgo->gc_data[i].country_text;
                    }
                  len += sprintf (zgo->trace_buffer+len, " %s %s[%d]", s,s2,x);
                  s = "or";
                }
 
              if (x >= freq_2)
                {
                  p = (long) zgo->gc_data[i].weight;
                  if (p < 1L)
                    {
                      p = 1L;
                    }
                  if (zgo->gc_data[i].n & 1024)
                    {
                      /****  country has been found  ****/
                      if (p < 60L)
                        {
                          p = 60L;
                        }
                      p = 8L * p;
                    }
                  if (x > 1)
                    {
                      p *= (1L << x);
                    }
 
                  if (p > count  ||  pos_0 < 0
                  || (p == count  &&  x > freq))
                    {
                      pos_0 = i;
                      freq = x;
                      count = p;
                    }
                }
            }
        }
 
      if (! (zgo->internal_mode & GENDER_TRACE_ONE_COUNTRY_ONLY))
        {
          if (strcmp (s,"or") == 0)
            {
        	  len += sprintf (zgo->trace_buffer+len, ")");
            }
        }
      else
        {
          if (pos_0 >= 0)
            {
              if (zgo->internal_mode & GENDER_TRACE_SHORT_COUNTRY_NAME)
                {
                  s2 = zgo->gc_data[pos_0].country_short;
                }
              else
                {
                  s2 = zgo->gc_data[pos_0].country_text;
                }
              len += sprintf (zgo->trace_buffer+len, " (country=%s)", s2);
            }
        }
    }
}
 
 
static void trace_info
    (char text[], char name[], char name2[], int res, char data[], struct ze_geocountry_obj *zgo TSRMLS_DC)
/****  output trace info  ****/
{
  trace_info_into_buffer (text, name, name2, res, data, zgo TSRMLS_CC);
  php_printf ("%s\n", zgo->trace_buffer);
}
 
 
static int internal_search
     (char search_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
/****  This function searches a single given name  ****/
{
  int  i,n,x;
  int  k,k0;
  int  freq,freq_2;
  int  pos_0,pos_x[7];
  int  freq_x[7];
  int  len,res;
  long count,count_2,count_x[7];
  long p,pos_f;
  char text [MAX_LINE_SIZE+1];
  char temp [DATA_NAME_LENGTH+1];
  int  country_matched_once = 0; /* name was found at least once in the country */
  int  restarted = 0; /* whether the loop was already restarted once */

  char tempLat [DATA_LATITUDE_LENGTH+1];
  char tempLon [DATA_LONGITUDE_LENGTH+1];
 
  if (zgo->internal_mode & TRACE_GENDER)
    {
      php_printf ("Searching for name '%s'", search_name);
 
      /****  check for country  ****/
      for (i=0; zgo->gc_data[i].country_text != NULL; i++)
        {
          if (country == zgo->gc_data[i].gc_country)
            {
              php_printf ("  (country = %s)", zgo->gc_data[i].country_text);
              zgo->gc_data[i].n |= 1024;
            }
        }
      php_printf ("\n");
    }
 
  if (zgo->f_names == NULL)
    {
      if(gender_connect_to_source(zgo TSRMLS_CC) == INTERNAL_ERROR_GENDER)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: could not open dictionary file '%s'\n",
                  zgo->dsn);
            }
          return (INTERNAL_ERROR_GENDER);
        }
    }
 
  if (compare_mode & SEARCH_GENDER)
    {
     i = (int)strlen (search_name);
     if (i == 0)
       {
         return (IS_UNISEX_NAME);
       }
 
     if (i >= 2  &&  search_name[i-1] == '.')
       {
         compare_mode |= COMPARE_ABBREVIATION;
         zgo->internal_mode &= ~GENDER_GET_COUNTRY;
       }
    }
 
  if (compare_mode & UMLAUT_EXPAND)
    {
      compare_mode |= EXPAND_PARAM_1;
    }
 
  pos_f = binary_search (zgo->f_names, search_name, DATA_NAME_POS,
            DATA_NAME_LENGTH+1, compare_mode, zgo TSRMLS_CC);
 
  if (pos_f < 0L)
    {
      if (pos_f == -10L)
        {
          return (INTERNAL_ERROR_GENDER);
        }
      return (NAME_NOT_FOUND);
    }
 
  /****  read one or more first names in dictionary file  ****/
  res = NAME_NOT_FOUND;
  php_stream_seek(zgo->f_names, pos_f, SEEK_SET);
 
  for (x=0; x<7; x++)
    {
      pos_x[x] = 0;
      freq_x[x] = 0;
      count_x[x] = 0L;
    }
  k0 = 500;
  n = 0;
 
restart_loop:
  do
    {
      read_line (zgo->f_names,text, MAX_LINE_SIZE+1 TSRMLS_CC);
      len = (int)strlen (text);
 
      strcpy (temp,"");
      if (text[0] != '#'  &&  len > DATA_NAME_POS)
        {
          copycut (temp, text+DATA_NAME_POS, DATA_NAME_LENGTH+1 TSRMLS_CC);
        }
 
      /****  compare first names  ****/
      i = strcmp_search (search_name, temp, compare_mode,
              text [POS_UMLAUT_INFO], &k, zgo TSRMLS_CC);
 
      if (i == 0       /****  names are matching     and     ****/
      &&  k <= k0)     /****  umlauts don't differ too much  ****/
        {
	  int country_found = 0;
          if (k < k0)
            {
              k0 = k;
              k = 0;
            }

          res = conv_internal_result (text, compare_mode);

          if (zgo->internal_mode & TRACE_GENDER)
            {
              trace_info ("evaluating name", temp,NULL, res, text, zgo TSRMLS_CC);
            }
 
          if (compare_mode & COMPARE_ABBREVIATION)
            {
              if (res == IS_FEMALE)  res = IS_MOSTLY_FEMALE;
              if (res == IS_MALE)  res = IS_MOSTLY_MALE;
 
              i = (int)strlen (search_name);
 
              if (i <= 3
              || (i == 4  &&  ((n & 16)  ||  ((n & 2)  &&  (n & 8)))))
                {
                  return (IS_UNISEX_NAME);
                }
            }
 
          /****  check country  ****/
          freq = 0;
          freq_2 = 0;
          pos_0 = -1;
          count = 1L;
 
          for (i=0; zgo->gc_data[i].country_text != NULL; i++)
            {
              if (len <= (int) zgo->gc_data[i].pos)
                {
                  /****  line is too short  ****/
                  return (INTERNAL_ERROR_GENDER);
                }
 
              if (text [zgo->gc_data[i].pos] != ' ' &&
				  (!(compare_mode & IGNORE_OTHER_COUNTRIES) ||
				  ((compare_mode & IGNORE_OTHER_COUNTRIES) && country == zgo->gc_data[i].gc_country) ||
				  country == GC_ANY_COUNTRY))
                {
                  x = get_frequency (text[zgo->gc_data[i].pos]);
 
                  if (x > freq)
                    {
                      freq = x;
                    }
		    country_found = 1;
		    country_matched_once = 1;
                }
            }
	
	if ((compare_mode & IGNORE_OTHER_COUNTRIES) && !country_found) {
		/* The current name item contains no information for the requested
		   country. Try for more luck looking next. */
		goto look_next;
	}
 
          freq_2 = (int)((freq+1) / 2);
          if (freq_2 <= 1)
            {
              freq_2 = freq;
            }
          if (freq_2 < freq - 3)
            {
              freq_2 = freq - 3;
            }
 
          freq = 0;
          for (i=0; zgo->gc_data[i].country_text != NULL; i++)
            {
              if (text [zgo->gc_data[i].pos] != ' ' &&
				  (!(compare_mode & IGNORE_OTHER_COUNTRIES) ||
				  ((compare_mode & IGNORE_OTHER_COUNTRIES) && country == zgo->gc_data[i].gc_country) ||
				  country == GC_ANY_COUNTRY))
                {
                  x = get_frequency (text[zgo->gc_data[i].pos]);
 
                  if (x >= freq_2)    /***  freq_2 !!  ****/
                    {
                      p = (long) zgo->gc_data[i].weight;
                      if (p < 1L)
                        {
                          p = 1L;
                        }
                      if (zgo->gc_data[i].n & 1024)
                        {
                          /****  country has been found  ****/
                          if (p < 60L)
                            {
                              p = 60L;
                            }
                          p = 8L * p;
                        }
                      if (x > 1)
                        {
                          p *= (1L << x);
                        }
 
                      if (p > count  ||  pos_0 < 0
                      || (p == count  &&  x > freq))
                        {
                          pos_0 = i;
                          freq = x;
                          count = p;
                        }
                    }
 
                  if (x > (int) (zgo->gc_data[i].n % 16))
                    {
                      x -= (int) (zgo->gc_data[i].n % 16);
                      zgo->gc_data[i].n += x;
                    }
                }
            }

copycut (zgo->latitude, text+DATA_LATITUDE_POS, DATA_LATITUDE_LENGTH+1 TSRMLS_CC);
copycut (zgo->longitude, text+DATA_LONGITUDE_POS, DATA_LONGITUDE_LENGTH+1 TSRMLS_CC);

 
          switch (res)
            {
              case IS_FEMALE        :  i =  1;  x = 0;  break;
              case IS_MOSTLY_FEMALE :  i =  2;  x = 1;  break;
              case IS_MALE          :  i =  4;  x = 2;  break;
              case IS_MOSTLY_MALE   :  i =  8;  x = 3;  break;
              case IS_UNISEX_NAME   :  i = 16;  x = 4;  break;
              case EQUIVALENT_NAMES :  i = 32;  x = 5;  break;
	      case IS_COUNTRY       :  i = 128;  x = 7;  break;
              case IS_CITY          :  i = 256;  x = 8;  break;
              default               :  i = 64;  x = 6;  break;
            }
 
          n |= i;
          if (count > count_x[x])
            {
              pos_x[x] = pos_0;
              freq_x[x] = freq;
              count_x[x] = count;
            }
        }

look_next: 
      /****  "strcmp_search" with mode "IGNORE_SEPARATOR" (!!)  ****/
      i = strcmp_search (search_name, temp, (compare_mode | IGNORE_SEPARATOR),
             text [POS_UMLAUT_INFO], & k, zgo TSRMLS_CC);
    }while (i == 0);
 
	if (!restarted && (compare_mode & IGNORE_OTHER_COUNTRIES) && !country_matched_once) {
		/* The name/country combination doesn't exist, restore the old behavioul.
		   Lookup in any country should work as we were returned before entering
		   the loop otherwise. */
		country = GC_ANY_COUNTRY;
		restarted = 1;
		goto restart_loop;
	}
 
  i = 0;
  count = 0L;
  count_2 = 0L;
  freq = 0;
  freq_2 = 0;
 
  for (x=0; x<7; x++)
    {
      if (count_x[x] > count
      || (count_x[x] == count  &&  freq_x[x] > freq))
        {
          i = 1 << x;
          count_2 = count;
          count = count_x[x];
          freq_2 = freq;
          freq = freq_x[x];
        }
      else if (count_x[x] > count_2
      || (count_x[x] == count_2  &&  freq_x[x] > freq_2))
        {
          count_2 = count_x[x];
          freq_2 = freq_x[x];
        }
    }
 
  if (freq_2 > 0)
    {
      /****  calculate the (truncated) binary logarithm of (count/count_2)  ****/
      x = 5;
      if (count > 0L  &&  count < count_2)
        {
          x = 0;
          while (count < count_2)
            {
              count *= 2L;
              x--;
            }
        }
      else if (count_2 > 0  &&  count_2 <= count)
        {
          x = -1;
          while (count_2 <= count)
            {
              count_2 *= 2;
              x++;
            }
        }
      k = freq - freq_2;
 
      if (country != GC_ANY_COUNTRY
	  && (k+k + x >= 10
	  || (k >= -1  &&  x >= 7)  ||  (k >= 0  &&  x >= 4)))
        {
          /****  ignore other countries  ****/
          n = i;
        }
      else if (k+k + x >= 6
      ||  (k >= -1  &&  x >= 4)  ||  (k >= 0  &&  x >= 2))
        {
          if (i == 1  ||  i == 2)
            {
              n &= ~(1|4|8|16);
              n |= 2;
            }
          else if (i == 4  ||  i == 8)
            {
              n &= ~(1|2|4|16);
              n |= 8;
            }
        }
    }
 
  if ((n & (1|2|16))  &&  (n & (4|8|16)))
    {
      return (IS_UNISEX_NAME);
    }
 
  if (n & 2)
    {
      return (IS_MOSTLY_FEMALE);
    }
  if (n & 8)
    {
      return (IS_MOSTLY_MALE);
    }
 
  if (n == 1)
    {
      return (IS_FEMALE);
    }
  if (n == 4)
    {
      return (IS_MALE);
    }
 
  return (res);
}
 
 
 
 
static int get_gender_internal
    (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
/****  This function determines the gender of a given first name  ****/
 
/****  Result: IS_FEMALE        :  female first name            ****/
/****          IS_MOSTLY_FEMALE :  mostly female first name     ****/
/****          IS_MALE          :  male first name              ****/
/****          IS_MOSTLY_MALE   :  mostly male first name       ****/
/****          IS_A_COUPLE      :  name is both male and female ****/
/****          IS_UNISEX_NAME   :  unisex name (can be any gender) ****/
/****          NAME_NOT_FOUND   :  name not found               ****/
/****          ERROR_IN_NAME    :  name contains an error       ****/
{
  int  i,k,n;
  int  gender,res;
  char c;
  char temp [LENGTH_FIRST_NAME+52];
 
  if (first_name[0] == '\0')
    {
      return (IS_UNISEX_NAME);
    }
 
  compare_mode &= ~(SEARCH_EQUIV | SEARCH_FIRST);
  compare_mode |= SEARCH_GENDER;
 
  copycut (temp, first_name, LENGTH_FIRST_NAME+51 TSRMLS_CC);
 
  /****  reset statistics  ****/
  for (i=0; zgo->gc_data[i].country_text != NULL; i++)
    {
      zgo->gc_data[i].n = 0;
      if (country == zgo->gc_data[i].gc_country)
        {
          zgo->gc_data[i].n |= 1024;
        }
    }
 
  /****  evaluate name  ****/
  k = 0;
  while (temp[k] != '\0'  &&  temp[k] != ' '
  &&  temp[k] != '-'  &&  temp[k] != '.')
    {
      k++;
    }
  if (temp[k] == '.')
    {
      k++;
    }
 
  if (temp[k] != '\0')
    {
     /****  search whole name  ****/
     gender = internal_search (temp, compare_mode, country, zgo TSRMLS_CC);
 
     if (gender != NAME_NOT_FOUND)
       {
         return (gender);
       }
    }
 
  /****  evaluate "multiple" name  ****/
  n = 0;
  gender = NAME_NOT_FOUND;
  compare_mode |= SEARCH_FIRST;
  res = 0;
 
  while (temp[n] != '\0')
    {
      /****  look for end of word  ****/
      k = n;
      while (temp[k] != '\0'  &&  temp[k] != ' '
      &&  temp[k] != '-'  &&  temp[k] != '.')
        {
          k++;
        }
      if (temp[k] == '.')
        {
          k++;
        }
      c = temp[k];
      temp[k] = '\0';
 
      if (k > n)
        {
          /****  check this name  ****/
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("\n");
            }
          if ((zgo->internal_mode & GENDER_GET_COUNTRY)
          &&  ! (compare_mode & SEARCH_FIRST))
            {
              for (i=0; zgo->gc_data[i].country_text != NULL; i++)
                {
                  zgo->gc_data[i].n &= ~1024;
                  zgo->gc_data[i].n *= 16;
 
                  if (country == zgo->gc_data[i].gc_country)
                    {
                      zgo->gc_data[i].n |= 1024;
                    }
                }
            }
 
          gender = internal_search (temp+n, compare_mode, country, zgo TSRMLS_CC);
          if (zgo->internal_mode & TRACE_GENDER)
            {
              trace_info ("result for", temp+n, NULL, gender, NULL, zgo TSRMLS_CC);
            }
 
          if ((zgo->internal_mode & GENDER_GET_COUNTRY)
          &&  ! (compare_mode & SEARCH_FIRST))
            {
              for (i=0; zgo->gc_data[i].country_text != NULL; i++)
                {
                  n = (int) ((zgo->gc_data[i].n & ~1024) / 16);
                  zgo->gc_data[i].n = multiply_frequency (n, zgo->gc_data[i].n % 16);
 
                  if (country == zgo->gc_data[i].gc_country)
                    {
                      zgo->gc_data[i].n |= 1024;
                    }
 
                }
            }
          compare_mode &= ~SEARCH_FIRST;
 
          switch (gender)
            {
              case IS_FEMALE        :  res |=  1;  break;
              case IS_MOSTLY_FEMALE :  res |=  2;  break;
              case IS_MALE          :  res |=  4;  break;
              case IS_MOSTLY_MALE   :  res |=  8;  break;
              case IS_UNISEX_NAME   :  res |= 16;  break;
              case NAME_NOT_FOUND   :  res |= 32;  break;
	      case IS_COUNTRY       :  res |= 128;  break;
              case IS_CITY          :  res |= 256;  break;
              default               :  res |= 64;  break;
            }
        }
 
      temp[k] = c;
      n = k;
      /****  look for begin of word  ****/
      while (temp[n] == ' '  ||  temp[n] == '-'  ||  temp[n] == '.')
        {
          n++;
        }
    }
 
  if ((res & 1)  &&  (res & 4))
    {
      if (compare_mode & GENDER_ALLOW_COUPLE)
        {
          /****  name is both male and female  ****/
          return (IS_A_COUPLE);
        }
 
      /****  error in mame  ****/
      return (ERROR_IN_NAME);
    }
 
  if (res & 128)
     {
       return (IS_COUNTRY);
     }
  if (res & 256)
     {
       return (IS_CITY);
     }
  if (res & 1)
    {
      return (IS_FEMALE);
    }
  if (res & 4)
    {
      return (IS_MALE);
    }
 
  if ((res & 2)  &&  ! (res & 8))
    {
      return (IS_MOSTLY_FEMALE);
    }
  if ((res & 8)  &&  ! (res & 2))
    {
      return (IS_MOSTLY_MALE);
    }
 
  if (res & (2|8|16))
    {
      return (IS_UNISEX_NAME);
    }
 
  return (gender);
}
 
 
 
 
static int check_nickname_internal (char first_name_1[],
     char first_name_2[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
/****  This function checks if two first names are "equivalent"  ****/
 
/****  Result: EQUIVALENT_NAMES :  names are equivalent  ****/
/****          NAME_NOT_FOUND   :  names not found       ****/
/****         INTERNAL_ERROR_GENDER :  internal error    ****/
{
  int  i,k,n;
  char a_temp [LENGTH_FIRST_NAME+3];
  char a2_temp [LENGTH_FIRST_NAME+3];
 
  n = copycut (a_temp, first_name_1, LENGTH_FIRST_NAME+3 TSRMLS_CC);
  k = copycut (a2_temp,first_name_2, LENGTH_FIRST_NAME+3 TSRMLS_CC);
 
  if (n > LENGTH_FIRST_NAME  ||  k > LENGTH_FIRST_NAME)
    {
      /****  name is too long  ****/
      return (ERROR_IN_NAME);
    }
 
  if (strcmp (a_temp,a2_temp) == 0)
    {
      /****  names are exactly the same  ****/
      return (EQUIVALENT_NAMES);
    }
  if (n == 0  ||  k == 0)
    {
      /****  at least one name is empty  ****/
      return (NOT_EQUAL_NAMES);
    }
 
  if (strchr (a_temp, ' ') != NULL
  ||  strchr (a_temp, '-') != NULL
  ||  strchr (a2_temp,' ') != NULL
  ||  strchr (a2_temp,'-') != NULL)
    {
      /****  at least one "multiple" name  ****/
      return (NOT_EQUAL_NAMES);
    }
 
  i = strcmp_search (a_temp, a2_temp, COMPARE_ABBREVIATION,0, NULL, zgo TSRMLS_CC);
  if (i == 0)
    {
      /****  names are like (e.g.) "Carl" and "C."  ****/
      return (EQUIVALENT_NAMES);
    }
 
  /****  reset statistics  ****/
  for (i=0; zgo->gc_data[i].country_text != NULL; i++)
    {
      zgo->gc_data[i].n = 0;
      if (country == zgo->gc_data[i].gc_country)
        {
          zgo->gc_data[i].n |= 1024;
        }
 
    }
 
  compare_mode &= ~(SEARCH_GENDER | SEARCH_FIRST);
  compare_mode |= SEARCH_EQUIV;
 
  i = NAME_NOT_FOUND;
  if (n <= k)
    {
      php_sprintf (a_temp+n, " %s", a2_temp);
      i = internal_search (a_temp, compare_mode, country, zgo TSRMLS_CC);
      a_temp[n] = '\0';
    }
  if (k <= n  &&  i == NAME_NOT_FOUND)
    {
      php_sprintf (a2_temp+k, " %s", a_temp);
      i = internal_search (a2_temp, compare_mode, country, zgo TSRMLS_CC);
      a2_temp[k] = '\0';
    }
 
  return (i);
}
 
 
 
static void standardize_arabic_name (char dest[], char src[], int len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  char *s;
 
  (void) up_conv_g (dest, src, len, zgo TSRMLS_CC);
 
  while ((s = strchr (dest,'K')) != NULL)
    {
      *s = 'Q';
    }
 
  while ((s = strchr (dest,'J')) != NULL)
    {
      *s = 'Y';
      if (*(s+1) == '\0')
        {
          *s = 'I';
        }
    }
 
  while ((s = strpbrk (dest,"AEIOU")) != NULL  &&  *(s+1) != '\0')
    {
      *s = PSEUDO_VOWEL_FOR_ARABIC_NAMES;
    }
 
  for (i=0; dest[i] != '\0'; i++)
    {
      while (dest[i+1] == dest[i])
        {
          strcpy (dest+i, dest+i+1);
        }
 
      if (dest[i] == '\'')
        {
          strcpy (dest+i, dest+i+1);
        }
      if (dest[i] == 'T'  &&  dest[i+1] == '\0')
        {
          dest[i] = 'D';
        }
    }
 
  if ((s = strchr (dest,'Y')) != NULL
  &&  s > dest  &&  *(s-1) == PSEUDO_VOWEL_FOR_ARABIC_NAMES
  &&  strchr ("AEIOU", *(s+1)) == NULL
  &&  *(s+1) != PSEUDO_VOWEL_FOR_ARABIC_NAMES  && *(s+1) != 0)
    {
      strcpy (s,s+1);
    }
}
 
 
 
 
static char nam_array[20] [DATA_NAME_LENGTH+1];
 
static int find_similar_name_internal
    (char first_name[], int country, char result_string[], int len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  long l0,l2,pos_f;
  int  i,k,n,x,f,p;
  int  comp_arab,flag_sh;
  int  lev_mode,limit;
  int  d_list[20];
  int  p_list[20];
  long l_list[20];
 
  char *s, first_char;
  char *nam_list[20];
  char text [MAX_LINE_SIZE+1];
  char this_name [DATA_NAME_LENGTH+1];
  char this_name_0 [DATA_NAME_LENGTH+1];
  char arab_pseudo_first [DATA_NAME_LENGTH+1];
  char arab_pseudo_this [DATA_NAME_LENGTH+1];
 
  /****  reset statistics  ****/
  s = "any country";
  comp_arab = -1;
  for (i=0; zgo->gc_data[i].country_text != NULL; i++)
    {
      zgo->gc_data[i].n = 0;
      if (country == zgo->gc_data[i].gc_country)
        {
          zgo->gc_data[i].n |= 1024;
          s = zgo->gc_data[i].country_text;
        }
      if (zgo->gc_data[i].gc_country == GC_ARABIA)
        {
          comp_arab = i;
        }
    }
 
  if (zgo->internal_mode & TRACE_GENDER)
    {
      printf ("\n");
      printf ("Searching for similar names and corrections to name '%s'\n",
          first_name);
      printf ("(country = '%s'):\n\n", s);
    }
 
  if (strcmp (first_name,"") == 0)
    {
      if (zgo->internal_mode & TRACE_GENDER)
        {
          printf ("Error: first name is empty.\n");
        }
      return (-1);
    }
  if (len < 30)
    {
      if (zgo->internal_mode & TRACE_GENDER)
        {
          printf ("Error: result string is too short.\n");
        }
      return (-1);
    }
 
  if (zgo->f_names == NULL)
    {
      if(gender_connect_to_source(zgo TSRMLS_CC) == INTERNAL_ERROR_GENDER)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              php_printf ("Error: could not open dictionary file '%s'\n",
                  zgo->dsn);
            }
          return (INTERNAL_ERROR_GENDER);
        }
    }
 
  for (i=0; i<20; i++)
    {
      strcpy (nam_array[i],"");
      nam_list[i] = nam_array[i];
      d_list[i] = 10000;
      p_list[i] = 0;
      l_list[i] = 0L;
    }
  limit = 70;
  n = 0;
 
  first_char = zgo->sortchar [(unsigned char) *first_name];
 
  for (i=0; russian_names[i] != NULL; i+=2)
    {
      if (first_char == russian_names[i+1][0]
      &&  strcmp_search (first_name, russian_names[i+1], 0,0,NULL, zgo TSRMLS_CC) == 0)
        {
          if (zgo->internal_mode & TRACE_GENDER)
            {
              printf ("Notice: '%s' is a Russian name and should be '%s'.\n",
                  russian_names[i+1], russian_names[i]);
              printf ("Search will be continued with this name.\n\n");
            }
 
          strncpy (nam_list[0], russian_names[i+1], DATA_NAME_LENGTH);
          nam_list[0] [DATA_NAME_LENGTH] = '\0';
 
          d_list[0] = 0;
          p_list[0] = 100;
          l_list[0] = 1000L;
          n = 1;
 
          first_name = russian_names[i];
          first_char = zgo->sortchar [(unsigned char) *first_name];
          break;
        }
    }
 
  /****  search first letter in dictionary file  ****/
  sprintf (this_name, "%c", first_char);
 
  pos_f = binary_search (zgo->f_names, this_name, DATA_NAME_POS,
            DATA_NAME_LENGTH+1, GET_MATCH_OR_NEXT_HIGHER, zgo TSRMLS_CC);
 
  if (pos_f < 0L)
    {
      if (pos_f == -10L)
        {
          return (INTERNAL_ERROR_GENDER);
        }
      return (NAME_NOT_FOUND);
    }
 
  /****  read one or more first names in dictionary file  ****/
  php_stream_seek(zgo->f_names, pos_f, SEEK_SET);
 
  if (strchr (first_name,'.') == NULL
  &&  strchr (first_name,MATCHES_ANY_STRING) == NULL)
    {
      standardize_arabic_name
             (arab_pseudo_first, first_name, DATA_NAME_LENGTH+1, zgo TSRMLS_CC);
    }
  else
    {
      comp_arab = -1;
    }
 
  flag_sh = 0;
  if (lev_diff_g ("*SH*", first_name, 0, NULL, EXPAND_PARAM_2, zgo TSRMLS_CC) == 0)
    {
      flag_sh = 1;
    }
  if (lev_diff_g ("*SCH*", first_name, 0, NULL, EXPAND_PARAM_2, zgo TSRMLS_CC) == 0)
    {
      flag_sh = 2;
    }
 
  /****  "Nadia" is both a Russian and an Arabic name  ****/
  s = NULL;
  if (strcmp_search (first_name, "Nadia", 0,0,NULL, zgo TSRMLS_CC) == 0)
    {
      s = "Nadia";
    }
  if (strcmp_search (first_name, "Nadya", 0,0,NULL, zgo TSRMLS_CC) == 0
  ||  strcmp_search (first_name, "Nadja", 0,0,NULL, zgo TSRMLS_CC) == 0)
    {
      s = "Nadya";
    }
  if (s != NULL)
    {
      strcpy (nam_list[0],s);
      d_list[i] = 0;
      p_list[i] = 300;
      l_list[0] = 500L;
      n = 1;
 
      /****  do not read the dictionary file  ****/
      php_stream_seek (zgo->f_names, 0L, SEEK_END);
    }
 
  while (! php_stream_eof (zgo->f_names))
    {
      /****  read first names  ****/
      text [POS_UMLAUT_INFO] = '\0';
      read_line (zgo->f_names,text, MAX_LINE_SIZE+1 TSRMLS_CC);
 
      if (strchr ("FM1?",text[0]) == NULL
      ||  text[POS_UMLAUT_INFO] == COMPRESSED_UMLAUTS_FOR_SORTING)
        {
          /****  ignore this entry  ****/
          continue;
        }
 
      (void) copycut (this_name, text+DATA_NAME_POS, DATA_NAME_LENGTH+1 TSRMLS_CC);
 
      if (first_char != '\0'
      &&  first_char != zgo->sortchar [(unsigned char) this_name[0]]
      &&  zgo->sortchar [(unsigned char) this_name[0]] != IS_CHAR_TO_IGNORE)
        {
          if (n > 0  &&  d_list[0] <= 100
          && (d_list[0] <= 60  ||  p_list[0] >= 190)
          && (d_list[0] >= 0  ||  strchr ("JY",arab_pseudo_first[0]) == NULL))
            {
              /****  fast search was successful  ****/
              break;
            }
          else
            {
              /****  start second search  ****/
              if (zgo->internal_mode & TRACE_GENDER)
                {
                  printf ("\n");
                  printf ("Fast search was not successful.\n");
                  printf ("Starting extended search.\n");
                  printf ("\n");
                }
 
              first_char = '\0';
              n = 0;
              (void) php_stream_seek (zgo->f_names, 0L, SEEK_SET);
              continue;
            }
        }
 
      /****  compare names  ****/
      delete_chars_to_ignore
           (this_name_0, this_name, flag_sh, DATA_NAME_LENGTH+1, zgo TSRMLS_CC);
 
      lev_mode = LEV_G_COMPARE_GERMAN | EXPAND_PARAM_1;
      if (text[POS_UMLAUT_INFO] == EXPANDED_UMLAUTS_FOR_SORTING)
        {
          lev_mode |= EXPAND_PARAM_2;
        }
 
      k = lev_diff_g (this_name_0, first_name, limit+60, NULL, lev_mode, zgo TSRMLS_CC);
 
      if (comp_arab >= 0
      &&  text [zgo->gc_data[comp_arab].pos] != ' ')
        {
          /****  "compare Arabic"  ****/
          standardize_arabic_name
                 (arab_pseudo_this, this_name, DATA_NAME_LENGTH+1, zgo TSRMLS_CC);
 
          if (strcmp (arab_pseudo_first, arab_pseudo_this) == 0)
            {
              switch (text [zgo->gc_data[comp_arab].pos])
                {
                  case '1':  k = (int)(k/2);  break;
                  case '2':  k = (int)(k/3);  break;
                  case '3':  k = (int)(k/4);  break;
                  case '4':  k = (int)(k/5);  break;
                  case '5':  k = (int)(k/7);  break;
                  case '6':  k = (int)(k/10); break;
                  case '7':  k = (int)(k/20); break;
                  default :   k = -1;         break;
                }
            }
        }
 
      if (k <= limit + 60)
        {
          /****  name has been found  ****/
          if (k < limit)
            {
              limit = k;
              if (limit < 40)
                {
                  limit = 40;
                }
            }
 
          l0 = 0L;
          for (i=0; zgo->gc_data[i].country_text != NULL; i++)
            {
              if (text [zgo->gc_data[i].pos] != ' ')
                {
                  x = get_frequency (text[zgo->gc_data[i].pos]);
 
                  p = zgo->gc_data[i].weight;
                  if (p < 1)
                    {
                      p = 1;
                    }
                  if (x <= 1  &&  p > 5)
                    {
                      /****  very rare names need adjustments  ****/
                      p = (int)((p+5)/2);
                    }
 
                  if (zgo->gc_data[i].n & 1024)
                    {
                      /****  country has been found  ****/
                      if (p < 60)
                        {
                          p = 60;
                        }
                      p = 8 * p;
                    }
 
                  /****  de-logarithmize "frequency"  ****/
                  f = 1;
                  while (x > 0)
                    {
                      f *= 2;
                      x--;
                    }
 
                  l2 = (long) f * (long) p;
                  l0 += l2;
                }
            }
 
          /****  look for first name in list of matches  ****/
          i = 0;
          while (i < n  &&  strcmp (this_name,nam_list[i]) != 0)
            {
              i++;
            }
          if (i < n)
            {
              /****  name is already in list --> add points  ****/
              l0 += l_list[i];
            }
 
          /****  logarithmize "l0"  ****/
          l2 = l0;
          f = 0;
          while (l2 < 4L)
            {
              l2 *= 2L;
              f--;
            }
          while (l2 >= 8L)
            {
              l2 /= 2L;
              f++;
            }
 
          f = 10 * f;
          switch (l2)
            {
              case 5L :  f += 3;  break;  /**  10 * 0.322  **/
              case 6L :  f += 6;  break;  /**  10 * 0.585  **/
              case 7L :  f += 8;  break;  /**  10 * 0.807  **/
            }
 
          p = 200 - k + f;
 
          if (i < n)
            {
              /****  increase points and re-sort the list  ****/
              l_list[i] = l0;
              p_list[i] = p;
              while (i > 0  &&  p_list[i] > p_list[i-1])
                {
                  /****  switch entries  ****/
                  s = nam_list[i];
                  nam_list[i] = nam_list[i-1];
                  nam_list[i-1] = s;
 
                  x = d_list[i];
                  d_list[i] = d_list[i-1];
                  d_list[i-1] = x;
 
                  l2 = l_list[i];
                  l_list[i] = l_list[i-1];
                  l_list[i-1] = l2;
 
                  x = p_list[i];
                  p_list[i] = p_list[i-1];
                  p_list[i-1] = x;
 
                  i--;
                }
            }
          else
            {
              /****  new name found  ****/
              x = 0;
              while (x < n  &&  p_list[x] >= p)
                {
                  x++;
                }
 
              if (x >= 20)
                {
                  /****  list is "full",  and      ****/
                  /****  match is not good enough  ****/
                  continue;
                }
              if (n < 20)
                {
                  n++;
                }
 
              s = nam_list[n-1];
              for (i=n-1; i>x; i--)
                {
                  nam_list[i] = nam_list[i-1];
                  d_list[i] = d_list[i-1];
                  l_list[i] = l_list[i-1];
                  p_list[i] = p_list[i-1];
                }
              nam_list[x] = s;
 
              strcpy (nam_list[x], this_name);
              d_list[x] = k;
              l_list[x] = l0;
              p_list[x] = p;
            }
 
          if (zgo->internal_mode & TRACE_GENDER)
            {
              printf ("\n");
              printf ("Name '%s' found: points = %3d, diff = %3d\n",
                  this_name, p,k);
              printf ("Current list of matches:\n");
 
              for (i=0; i<n; i++)
                {
                  printf ("   %2d. name: '%s'  (points = %3d,  diff = %3d)\n",
                     i+1, nam_list[i], p_list[i], d_list[i]);
                }
            }
        }
    }
 
  /****  count "error-free" matches  ****/
  f = 0;
  k = 0;
  p = 0;
  x = 0;
  for (i=0; i<n; i++)
    {
      if (d_list[i] <= 5)
        {
          f++;
        }
      if (d_list[i] <= 15)
        {
          k++;
        }
      if (d_list[i] <= 30)
        {
          p++;
        }
 
      if (d_list[i] <= 50)
        {
          x++;
          if (p_list[i] >= 300)
            {
              x++;
            }
          if (p_list[i] >= 400)
            {
              x++;
            }
        }
    }
  limit = 40;
  if (p >= 4)
    {
      limit = 30;
    }
  if (k >= 4)
    {
      /****  return "error-free" matches only  ****/
      limit = 15;
    }
  if (n >= 2  &&  d_list[0] <= 10  &&  p_list[0] >= p_list[1] + 60)
    {
      /****  return "error-free" matches only  ****/
      limit = 10;
    }
  if (f >= 4)
    {
      /****  return "error-free" matches only  ****/
      limit = 5;
    }
  if (n > 0  &&  d_list[0] == 0)
    {
      /****  best match is 100% error-free  ****/
      limit = 0;
    }
 
 
  /****  determine "min_points"  ****/
  p = p_list[0] - 40;
  i = 0;
  while (first_name[i] != '\0'
  &&  first_name[i] != '.'  &&  first_name[i] != MATCHES_ANY_STRING)
    {
      i++;
    }
  if (i < 7  &&  first_name[i] != '\0')
    {
      /****  Wildcard found  ****/
      p += 5 * (7-i);
    }
 
  if (x >= 3)
    {
      /****  return "good" matches only  ****/
      p = p_list[0] + 100;
    }
 
  strcpy (result_string,"");
  k = 0;
  x = 0;
 
  for (i=0; i<n; i++)
    {
      if (p_list[i] <= p  &&  d_list[i] > limit)
        {
          /****  match is not good enough  ****/
          continue;
        }
 
      f = (int) strlen (nam_list[i]);
      if (k+2+f >= len)
        {
          /****  result_string is too short  ****/
          break;
        }
 
      if (k > 0)
        {
          result_string[k] = ' ';
          k++;
        }
 
      strcpy (result_string+k, nam_list[i]);
      k += f;
 
      result_string[k] = ';';
      k++;
      x++;
    }
  result_string[k] = '\0';
 
  if (zgo->internal_mode & TRACE_GENDER)
    {
      printf ("\n");
      printf ("Final result:\n");
 
      if (x == 0)
        {
          printf ("No names found.\n");
        }
      else
        {
          printf ("%d name(s) found:  '%s'\n", x, result_string);
        }
      printf ("\n");
    }
 
  return (x);
}
 
 
 
int get_gender (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  i = get_gender_internal (first_name, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int check_nickname (char first_name_1[],
         char first_name_2[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  i = check_nickname_internal
         (first_name_1, first_name_2, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int find_similar_name (char first_name[],
        int country, char result_string[], int len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int  i,k,n;
  strcpy (zgo->output_buffer,"");
 
  n = find_similar_name_internal
         (first_name, country, result_string, len, zgo TSRMLS_CC);
 
  (void) delete_chars_to_ignore (result_string, result_string, 0, len, zgo TSRMLS_CC);
 
  if (n > 0)
    {
      k = 0;
      n = 0;
      for (i=0; result_string[i] != '\0'; i++)
        {
          if (result_string[i] == ';')
            {
              k = i;
              n++;
            }
        }
      result_string[k] = '\0';
    }
 
  return (n);
}
 
 
 
int get_gender_unicode
   (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  conv_from_unicode_line (zgo->input_buffer_1, first_name, 101);
 
  i = get_gender_internal (zgo->input_buffer_1, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int check_nickname_unicode (char first_name_1[],
         char first_name_2[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  conv_from_unicode_line (zgo->input_buffer_1, first_name_1, 101);
  conv_from_unicode_line (zgo->input_buffer_2, first_name_2, 101);
 
  i = check_nickname_internal
         (zgo->input_buffer_1, zgo->input_buffer_2, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int find_similar_name_unicode (char first_name[],
        int country, char result_string[], int two_byte_len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i,k,n;
  conv_from_unicode_line (zgo->input_buffer_1, first_name, 101);
  strcpy (zgo->output_buffer,"");
 
  n = find_similar_name_internal
         (zgo->input_buffer_1, country, zgo->output_buffer, 1001, zgo TSRMLS_CC);
  make_unicode_line (result_string, zgo->output_buffer, two_byte_len);
 
  if (n > 0)
    {
      k = 0;
      n = 0;
      for (i=0; result_string[i] != '\0'  ||  result_string[i+1] != '\0'; i+=2)
        {
          if (result_string[i] == ';'  &&  result_string[i+1] == '\0')
            {
              k = i;
              n++;
            }
        }
      result_string[k] = '\0';
      result_string[k+1] = '\0';
    }
 
  return (n);
}
 
 
 
int get_gender_utf8 (char first_name[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  conv_from_utf8_line (zgo->input_buffer_1, first_name, 101);
  i = get_gender_internal (zgo->input_buffer_1, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int check_nickname_utf8 (char first_name_1[],
         char first_name_2[], int compare_mode, int country, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i;
  conv_from_utf8_line (zgo->input_buffer_1, first_name_1, 101);
  conv_from_utf8_line (zgo->input_buffer_2, first_name_2, 101);
 
  i = check_nickname_internal
         (zgo->input_buffer_1, zgo->input_buffer_2, compare_mode, country, zgo TSRMLS_CC);
  return (i);
}
 
 
int find_similar_name_utf8 (char first_name[],
        int country, char result_string[], int one_byte_len, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  int i,k,n;
  conv_from_utf8_line (zgo->input_buffer_1, first_name, 101);
  strcpy (zgo->output_buffer,"");
 
  n = find_similar_name_internal
         (zgo->input_buffer_1, country, zgo->output_buffer, 1001, zgo TSRMLS_CC);
  make_utf8_line (result_string, zgo->output_buffer, one_byte_len);
 
  if (n > 0)
    {
      k = 0;
      n = 0;
      for (i=0; result_string[i] != '\0'; i++)
        {
          if (result_string[i] == ';')
            {
              k = i;
              n++;
            }
        }
      result_string[k] = '\0';
    }
 
  return (n);
}
 
int determine_country (char *text, struct ze_geocountry_obj *zgo TSRMLS_DC)
{
  char *s;
  int  i,k,country;
 
  country = GC_ANY_COUNTRY;
  k = (int)strlen (text);
 
  if (isdigit (*text))
    {
      country = atoi (text);
    }
  for (i=0; zgo->gc_data[i].country_text != NULL; i++)
    {
      if (strcmp_search (text, zgo->gc_data[i].country_text, 0,0, NULL, zgo TSRMLS_CC) == 0
      || ((s = strstr (zgo->gc_data[i].country_text, text)) != NULL
       &&  *text == zgo->upperchar[(unsigned char) *s]
       && (*(s+k) == '\0'  ||  *(s+k) == '/'  ||  *(s+k) == ' ')))
        {
          country = zgo->gc_data[i].gc_country;
        }
    }
 
  return (country);
}

