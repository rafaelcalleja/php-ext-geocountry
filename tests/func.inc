<?php

namespace Geocountry;

function eval_result($result, $name , $country = "40.4165, -3.70256") {
if(is_array($result)) {
var_dump($result);
return;
}
	switch($result) {

		case Geocountry::IS_COUNTRY:
			printf("The name %s is Country in %s\n", $name, $country);
		break;

        case Geocountry::IS_CITY:
            printf("The name %s is city in %s\n", $name, $country);
        break;

	 
		/*case Gender::IS_MOSTLY_FEMALE:
			printf("The name %s is mostly female in %s\n", $name, $country);
		break;

	 
		case Gender::IS_MALE:
			printf("The name %s is male in %s\n", $name, $country);
		break;

	 
		case Gender::IS_MOSTLY_MALE:
			printf("The name %s is mostly male in %s\n", $name, $country);
		break;

	 
		case Gender::IS_UNISEX_NAME:
			printf("The name %s is unisex in %s\n", $name, $country);
		break;

	 
		case Gender::IS_A_COUPLE:
			printf("The name %s is both male and female in %s\n", $name, $country);
		break;

	 
		case Gender::NAME_NOT_FOUND:
			printf("The name %s was not found for %s\n", $name, $country);
		break;

	 
		case Gender::ERROR_IN_NAME:
			echo "There is an error in the given name!\n";
		break;*/
	 
		default:
            printf("RESULT: %s\nExpected: %s\n", $result, Geocountry::IS_COUNTRY);
            printf("Buscado: %s\nLocation: %s\n", $name, $country);
			echo "An error occured!\n";
		break;

	}
}
