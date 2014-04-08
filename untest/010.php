<?php
$g = new Gender\Gender;

var_dump($g->get("Dan", Gender\Gender::BRITAIN));
var_dump($g->get("Dan", Gender\Gender::VIETNAM));
var_dump($g->get("Dan", Gender\Gender::CHINA));

var_dump($g->get("Ben", Gender\Gender::BRITAIN));
var_dump($g->get("Ben", Gender\Gender::CHINA));
?>
+++DONE+++
