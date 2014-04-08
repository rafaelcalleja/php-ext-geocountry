--TEST--
Test __clone() breakage
--SKIPIF--
<?php if (!extension_loadad('gender')) print 'skip'; ?>
--FILE--
<?php
$g = new Gender\Gender;

class Dummy { public $value = null; }

$a = new Dummy;
$b = clone $a;

var_dump($g, $a, $b); ?>
DONE
--EXPECT--
object(Gender\Gender)#1 (0) {
}
object(Dummy)#2 (1) {
  ["value"]=>
  NULL
}
object(Dummy)#3 (1) {
  ["value"]=>
  NULL
}
DONE
