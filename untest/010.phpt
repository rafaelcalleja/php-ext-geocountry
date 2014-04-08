--TEST--
Test bug #64740 Gender ignores country for some names, variation 1
--SKIPIF--
<?php if (!extension_loadad('gender')) print 'skip'; ?>
--FILE--
<?php
$g = new Gender\Gender;

var_dump($g->get("Dan", Gender\Gender::BRITAIN));
var_dump($g->get("Dan", Gender\Gender::VIETNAM));
var_dump($g->get("Dan", Gender\Gender::CHINA));

var_dump($g->get("Ben", Gender\Gender::BRITAIN));
var_dump($g->get("Ben", Gender\Gender::CHINA));
?>
+++DONE+++
--EXPECT--
int(77)
int(109)
int(63)
int(77)
int(63)
+++DONE+++

