--TEST--
Check female name
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$name = "Milene";
$g = new Gender; 

eval_result($g->get($name), $name);
?>
DONE
--EXPECT--
The name Milene is female in 0
DONE
