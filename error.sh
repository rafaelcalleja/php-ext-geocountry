#!/bin/bash
for line in $(cat data/country_dict.txt |awk '{print $2}'); do ./check.sh "$line" ; done |grep -B3 "float(0)" |grep "Searching Country"
