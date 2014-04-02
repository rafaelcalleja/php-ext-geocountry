--TEST--
Check unisex name
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$name = "Sasha";
$g = new Gender; 

eval_result($g->get($name), $name);
?>
DONE
--EXPECT--
The name Sasha is unisex in 0
DONE
