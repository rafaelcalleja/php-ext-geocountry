<?php
$g = new Gender\Gender;

var_dump($g->get("Jan", Gender\Gender::BRITAIN));
var_dump($g->get("Jan", Gender\Gender::USA));
var_dump($g->get("Jan", Gender\Gender::DENMARK));
var_dump($g->get("Jan", Gender\Gender::ISRAEL));

var_dump($g->get("Val", Gender\Gender::BRITAIN));
var_dump($g->get("Val", Gender\Gender::USA));

var_dump($g->get("Chris", Gender\Gender::SWEDEN));
var_dump($g->get("Chris", Gender\Gender::GERMANY));

var_dump($g->get("Brynn", Gender\Gender::BRITAIN));
var_dump($g->get("Brynn", Gender\Gender::USA));

var_dump($g->get("Carol", Gender\Gender::ISRAEL));
var_dump($g->get("Carol", Gender\Gender::FRANCE));
var_dump($g->get("Carol", Gender\Gender::ROMANIA));
var_dump($g->get("Carol", Gender\Gender::BRITAIN));

var_dump($g->get("Fran", Gender\Gender::BRITAIN));
var_dump($g->get("Fran", Gender\Gender::ALBANIA));

var_dump($g->get("Ronny", Gender\Gender::NETHERLANDS));
var_dump($g->get("Ronny", Gender\Gender::BRITAIN));

var_dump($g->get("Gene", Gender\Gender::BRITAIN));
var_dump($g->get("Gene", Gender\Gender::USA));
var_dump($g->get("Gene", Gender\Gender::LITHUANIA));

var_dump($g->get("Rene", Gender\Gender::BRITAIN));
var_dump($g->get("Rene", Gender\Gender::USA));
var_dump($g->get("Rene", Gender\Gender::SLOVAKIA));
var_dump($g->get("Rene", Gender\Gender::SWEDEN));
?>
+++DONE+++
