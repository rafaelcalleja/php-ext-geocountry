--TEST--
Check multiple gender objects
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$name = array(
	array('Max', 'Jacky', 'Maria'),
	array('Maria', 'Sam', 'Nam'),
	array('Mary', 'Albert', 'Leo'),
);

foreach ($name as $n) {
	$g0 = new Gender;
	eval_result($g0->get($n[0]), $n[0]);
	$g1 = new Gender;
	eval_result($g1->get($n[1]), $n[1]);
	$g2 = new Gender;
	eval_result($g2->get($n[2]), $n[2]);
}
?>
DONE
--EXPECT--
The name Max is mostly male in 0
The name Jacky is mostly male in 0
The name Maria is female in 0
The name Maria is female in 0
The name Sam is unisex in 0
The name Nam is unisex in 0
The name Mary is female in 0
The name Albert is male in 0
The name Leo is mostly male in 0
DONE
