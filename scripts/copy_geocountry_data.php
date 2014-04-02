#!/usr/bin/env php
<?php

class scripts_copy_geocountry_data_postinstall
{
    function init()
    {
	return true;
    }
    
    function run()
    {
	$data_dir = '@data_dir@';
	$args = func_get_args();
	$save_path = $args[0]['savepath'];
    
	if(file_exists($save_path)) {
	    if(is_writeable($save_path)) {
		if(is_dir($save_path)) {
		    $datafile = "nam_dict.txt.bz2";
		    if(copy("$data_dir/gender/data/$datafile", "$save_path/$datafile")) {
			echo "Dictionary file successfully copied\n";
			
			return true;
		    }
		    
		    echo "Unknown error while copying the dictionary file\n";
		    return false;
		}
		
		echo "Given path isn't directory!\n";
		return false;
	    }
	    
	    echo "Given path isn't writeable!\n";
	    return false;
	}
	
	echo "Given path does not exist!\n";
    
	return false;
    }
}
?>
