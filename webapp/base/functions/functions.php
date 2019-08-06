<?php
function add_db_config($db_host, $db_user, $db_pass, $database){
    $file = fopen($_SERVER["DOCUMENT_ROOT"]."/includes/constants.php","w");
	$config = '<?php';
	$config .= "\ndefine('DB_HOST','". $db_host."');\n";
	$config .= "define('DB_USER','". $db_user."');\n";
	$config .= "define('DB_PASS','". $db_pass."');\n";
	$config .= "define('DATABASE','". $database."');\n";
	$config .= "?>";
	
	fwrite($file, $config);
	fclose($file);
	sleep(3);
	echo "HERE";
    $link = $_SERVER["DOCUMENT_ROOT"]. 'php/base/config.php?mod=base&menu=db&edit=true';
    header("Location: {$link}"); 
	exit();
    }
?>