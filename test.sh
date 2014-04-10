#/bin/bash
php create_data_file.php data/country_dict.txt > country_dic.c
make clean
make install
./error.sh
