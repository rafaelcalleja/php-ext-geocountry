<?php
$g = new Gender\Gender;

class Dummy { public $value = null; }

$a = new Dummy;
$b = clone $a;

var_dump($g, $a, $b); ?>
DONE
