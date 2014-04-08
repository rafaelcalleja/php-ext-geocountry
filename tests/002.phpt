--TEST--
Check country name
--SKIPIF--
<?php if (!extension_loaded("geocountry")) print "skip"; ?>
--FILE--
<?php 

namespace Geocountry;

include dirname(__FILE__) . "/func.inc";

$name = "Spain";
$g = new Geocountry;

eval_result($g->get($name), $name);
?>
DONE
--EXPECT--
The name Spain is Country in 40.4165, -3.70256
DONE
