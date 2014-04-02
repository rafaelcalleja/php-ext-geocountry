--TEST--
Check nickname
--SKIPIF--
<?php if (!extension_loaded("gender")) print "skip"; ?>
--FILE--
<?php 

namespace Gender;

include dirname(__FILE__) . "/func.inc";

$list = array(
	array("Max", "Maximilian"),
	array("Alexander", "Alex"),
	array("Ziska", "Franziska"),
	array("Yevgeni", "Zhenya"),
	array("Jacob", "Max"),
);
$g = new Gender;

foreach($list as $names) {
	var_dump($g->isNick($names[0], $names[1]));
}
?>
DONE
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
DONE
