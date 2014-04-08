--TEST--
Check determine country
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$ca = array(
	Gender::GERMANY,
	Gender::FINLAND,
	Gender::SERBIA,
);

$g = new Gender; 
foreach($ca as $c) {
	var_dump($g->country($c));
}

?>
DONE
--EXPECT--
array(2) {
  ["country_short"]=>
  string(1) "D"
  ["country"]=>
  string(7) "Germany"
}
array(2) {
  ["country_short"]=>
  string(3) "FIN"
  ["country"]=>
  string(7) "Finland"
}
array(2) {
  ["country_short"]=>
  string(3) "SER"
  ["country"]=>
  string(6) "Serbia"
}
DONE
