dnl $Id: config.m4 327273 2012-08-25 20:29:27Z ab $
dnl config.m4 for extension geocountry

PHP_ARG_ENABLE(geocountry, whether to enable geocountry support,
[  --enable-geocountry           Enable geocountry support])

if test "$PHP_GEOCOUNTRY" != "no"; then
  PHP_NEW_EXTENSION(geocountry, [geocountry_functions.c geocountry.c], $ext_shared)
fi
