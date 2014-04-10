<?php 

namespace Geocountry;

include dirname(__FILE__) . "/tests/func.inc";

$name = $_SERVER['argv'][1];

$g = new Geocountry;
eval_result($g->getCoords($name), $name);

?>
DONE
