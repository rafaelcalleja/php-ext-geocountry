/* This is a generated file, do not modify */
/* Usage: php create_data_file.php /path/to/name_dict.txt > nam_dict.c */
/* Adopted from the fileinfo extension */

<?php
	$dta = file_get_contents( $argv[1] );
	$dta = "data://text/plain;base64," . base64_encode($dta);
	$dta_l = strlen($dta);
	$j = 0;

	echo "const char php_geocountry_name_dict_data[" . ($dta_l+1) . "] = {\n";
	for ($i = 0; $i < $dta_l; $i++) {
		printf("0x%02X, ", ord($dta[$i]));
		if ($j % 16 == 15) {
			echo "\n";
		}
		$j++;
	}
	echo "};\n";
?>
