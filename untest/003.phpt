--TEST--
Check male name
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$name = "Saimon";
$g = new Gender; 

eval_result($g->get($name), $name);
?>
DONE
--EXPECT--
The name Saimon was not found for 0
DONE
