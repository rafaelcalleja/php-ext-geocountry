--TEST--
Check similar name
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$names = array("Mina", "Jesus", "Basti", "Mostafa", "Caterin", "Johann", "Havoc");
$g = new Gender;

foreach($names as $name) {
	var_dump($g->similarNames($name));
}
?>
DONE
--EXPECT--
array(7) {
  [0]=>
  string(4) "Mona"
  [1]=>
  string(4) "Muna"
  [2]=>
  string(4) "Mina"
  [3]=>
  string(5) "Minna"
  [4]=>
  string(5) "Min+A"
  [5]=>
  string(5) "Miina"
  [6]=>
  string(5) "Minah"
}
array(2) {
  [0]=>
  string(5) "Jesús"
  [1]=>
  string(5) "Jesus"
}
array(4) {
  [0]=>
  string(7) "Bastien"
  [1]=>
  string(5) "Basit"
  [2]=>
  string(5) "Basri"
  [3]=>
  string(5) "Besti"
}
array(3) {
  [0]=>
  string(7) "Mustafa"
  [1]=>
  string(7) "Mostafa"
  [2]=>
  string(8) "Moustafa"
}
array(2) {
  [0]=>
  string(9) "Catherine"
  [1]=>
  string(8) "Caterina"
}
array(2) {
  [0]=>
  string(6) "Johann"
  [1]=>
  string(6) "Jóhann"
}
array(0) {
}
DONE
