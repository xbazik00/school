<?php
/**
 * CHA script file
 *
 * Script for IPP Project1 C header file analysis (CHA)
 *
 * PHP version 5.6
 *
 * Date: 1.3.2017
 * @author     Martin Bazik <xbazik00@stud.fit.vutbr.cz>
 */


// todo: kontrola prepinacov - =


/**
 * Prints help info. Info how to use this script
 */
function print_help(){
	echo 
"Option			Description
--help			Displays help.
--input=fileordir	Given file or directory containing files in programming language C
			If not used, searches CWD.
--output=filename	Output file in XML format.
			If not used, uses standard output.
--pretty-xml=k		Indents each embedded block by \"k\" spaces.
			If no \"k\" is used k is set to default 4.
			If not used, the blocks are not indented.
--no-inline		Does not consider inline functions.
--max-par=n		Only shows functions with maximum\"n\" parameters.
--no-duplicates		Shows only first occurence of the function with the same name.
--remove-whitespace	Removes unnecessary whitespaces in return type and parameter type.\n";
}

/**
 * Parses file name from the given path.
 *
 * @param  String $f_path
 * @param  String $file_dir
 * @return String 
 */
function parse_file_name($f_path,$file_dir){
	$file_name = "";
	if($file_dir == "")
		$file_name = $f_path;
	elseif($file_dir == "./"){
		if(substr($f_path,0,2) == "./")
			$file_name = substr($f_path,2);
		else{
			$f_pos = strpos($f_path,getcwd() );
			$file_name = substr($f_path, $f_pos + strlen(getcwd()) + 1);
		}	
	}else{
		$f_pos = strpos($f_path, $file_dir);
		$file_name = substr($f_path, $f_pos + strlen($file_dir));
	}
	return $file_name;
}

/**
 * Takes input paramters, checks for duplicit parameters and parses them into an array.
 *
 * @param  array $argv
 * @param  int $argc
 * @return array 
 */
function get_arg_array($argv,$argc){
	// Array of parameters
	$ar_imp = implode($argv);

	// Array of all parameter descriptors
	$arg_ar = array("--help","--input","--output","--pretty-xml","--no-inline",
	"--max-par","--no-duplicates","--remove-whitespace");

	// Duplicit parameters
	foreach($arg_ar as $a){
		if(substr_count($ar_imp,$a) > 1){
			fwrite(STDERR,"error: There is a parameter set twice!\n");
			exit(1);
		}
	}

	// Parse args
	if($argc>1)
		parse_str(implode('&',array_slice($argv, 1)), $_GET);

	if(!empty(array_diff_key($_GET, array("--help" => "", "--input" => "", 
	"--output" => "", "--pretty-xml" => "", "--no-inline" => "",
       	"--max-par" => "", "--no-duplicates" => "", "--remove-whitespace" => "")))){
		fwrite(STDERR,"error: Illegal arguments!\n");
		exit(1);	
	}
	return $_GET;
}

/**
 * Checks whether the "--help" parameter was put in correctly.
 *
 * @param  String $argc  
 */
function check_help($argc){
	if(array_key_exists("--help",$_GET)){
		if($_GET["--help"] != "" or $argc >2){
			fwrite(STDERR,"error: Too many parameters along with --help\n");
			exit(1);
		}
		print_help();
		exit();
	}
}

/**
 * Checks whether the "--input" parameter was put in correctly. Returns 
 * array containing Strings of input input directory $file_dir, all the 
 * subdirectories and files included in variable $dirs, and paths to 
 * all files in $files.
 *
 * @return array
 */
function check_input(){
	$files = array();
	$dirs = array();
	$file_dir = "";
	if(array_key_exists("--input",$_GET)){
		if(empty($_GET["--input"])){
			fwrite(STDERR,"error: No path was given\n");
			exit(1);
		}
		$in_path = $_GET["--input"];
		if(is_dir($in_path)){
			$file_dir = $in_path;
			if($file_dir[strlen($file_dir)-1] != "/")
				$file_dir .= "/";
			$dirs = search_dir($in_path);
			$files = parse_h($dirs);
		}elseif(is_file($in_path)){
			$file_dir = "";
			array_push($files,$in_path);
		}else{
			fwrite(STDERR,"error: Input path does not exist!\n");
			exit(2);
		}
	}else{
		$file_dir = "./";
		$dirs = search_dir(getcwd());
		$files = parse_h($dirs);
	}
	return array($file_dir,$dirs,$files);
}

/**
 * Searches given directory for subdirectories and files.
 *
 * @param  String $dir 
 * @return array
 */
function search_dir($dir){
	if($dir[strlen($dir)-1] == "/") $dir = substr($dir, 0, -1);
	$par_dir = scandir($dir);
	if (($key = array_search('.', $par_dir)) !== false) {
 		unset($par_dir[$key]);
	}
	if (($key = array_search('..', $par_dir)) !== false) {
 		unset($par_dir[$key]);
	}
	foreach ($par_dir as &$d)
        	$d = $dir."/".$d;
	foreach($par_dir as $ch_dir){
		if(is_dir($ch_dir))
			$par_dir = array_merge($par_dir,search_dir($ch_dir));
	}
	return $par_dir;
}

/**
 * Parse paths for C header files.
 * 
 * @param  array $path 
 * @return array 
 */
function parse_h($path){
	$final = array();
	foreach ($path as $p)
		if(is_file($p) and substr($p,-2) == ".h")
        		array_push($final,$p);
	return $final;
}

/**
 * Checks whether the "--output" parameter was put in correctly. Returns
 * path to the output file.
 *
 * @return String
 */
function check_output_file(){
	if(array_key_exists("--output",$_GET)){

		// Empty parameter
		if(empty($_GET["--output"])){
			fwrite(STDERR,"error: No path was given\n");
			exit(1);
		}


		$out_path = $_GET["--output"];
		if(substr_count($out_path,"/") != 0){
			$out_pos = strrpos($out_path, "/");
			$out_dir = substr($out_path,0 , $out_pos+1);
		
			if(is_dir($out_path) or !is_dir($out_dir)){
				fwrite(STDERR,"error: Could not open output file!\n");
				exit(3);
			}
		}
		$outfile = fopen($_GET["--output"], "w");
	}else{
		$outfile = fopen('php://stdout', "w");
	}
	
	// no filename
	if(!$outfile){
		fwrite(STDERR,"error: Could not open output file!\n");
		exit(3);
	}
	return $outfile;
}

/**
 * Checks whether the certain parameter given by key String was put in correctly.
 * Returns bool "TRUE" if parameter is present, else "FALSE"
 *
 * @param string $par_name
 * @return bool
 */
function check_param($par_name){
	if(array_key_exists($par_name,$_GET)){
		if($_GET[$par_name] != "")
			exit(1);
		return TRUE;
	}else{
		return FALSE;
	}
}


/**
 * Checks whether the "--pretty-xml" parameter was put in correctly. Returns 
 * array of string $n giving possible "\n" needed, string $off representing 
 * whitespaces ,and number of spaces we need to add, -1 if there is no 
 * "--pretty-xml" parameter.
 *
 * @return array
 */
function check_p_xml(){
	if(array_key_exists("--pretty-xml",$_GET)){
		if($_GET["--pretty-xml"] == "")
			$p = 4;
		else
			$p = intval($_GET["--pretty-xml"]);
		if($p<0){
			fwrite(STDERR,"error: Illegal arguments!\n");
			exit(1);
		}
		$off = "";
		for($i=0;$i<$p;$i++)
			$off .= " ";
		$n="\n";
	
	}else{
		$n="";
		$off = "";
		$p = -1;
	}
	return array($n,$off,$p);
}

/**
 * Checks whether the "--max-par" parameter was put in correctly. Returns
 * max number of parameters, -1 if there is no limitation.
 *
 * @return int
 */
function check_max_p(){
	if(array_key_exists("--max-par",$_GET)){
		if($_GET["--max-par"] == "")
			exit(1);
		$p = intval($_GET["--max-par"]);
		if($p<0){
			fwrite(STDERR,"error: Illegal arguments!\n");
			exit(1);
		}
		return $p;
	}else{
		return -1;
	}
}

/**
 * Checks permissions to read from all the file in the given directory.
 *
 * @param array $files
 */
function check_perm($files){
	foreach($files as $path){
		if (!is_readable($path)) {
			fwrite(STDERR,"error: Could not open input file!\n");
			exit(2);
		}
	}
}

/**
 * Removes macros and comments from the file
 *
 * @param  String $file_content 
 * @return String 
 */
function reduce($file_content){
	$result = "";
	$state = "start";
	$block = 0;
	for ($i=0; $i<strlen($file_content); $i++) {  
		switch ($state){
			case "start":
				if($file_content[$i] == "/")
					$state = "lom_com";
				elseif($file_content[$i] == "{")
					$state = "block";
				elseif($file_content[$i] == "#")
					$state = "macro";
				else
					$result .= $file_content[$i];
				break;
			case "lom_com":
				if($file_content[$i] == "*")
					$state = "ml_com";
				elseif($file_content[$i] == "/")
					$state = "com";
				else{
					$result .= "/";
					$result .= $file_content[$i];
					$state = "start";
				}	
				break;
			case "com":
				if($file_content[$i] == "\n"){
					$result .= "\n";
					$state = "start";
				}	
				break;
			case "ml_com":
				if($file_content[$i] == "*")
					$state = "ml_end";
				break;
			case "ml_end":
				if($file_content[$i] == "*")
					$state = "ml_end";
				elseif($file_content[$i] == "/")
					$state = "start";
				else{
					$state = "ml_com";
				}	
				break;
			case "block":
				if($file_content[$i] == "{")
					$block += 1;
				elseif($file_content[$i] == "}"){
					if($block == 0){
						$result .= ";";
						$state = "start";
					}else
						$block -= 1;
				}
				break;
			case "macro":
				if($file_content[$i] == "\n"){
					$result .= "\n";
					$state = "start";
				}
				elseif($file_content[$i] == "\\")
					$state = "macro_break";	
				break;
			case "macro_break":
				$state = "macro";
				break;
		}
	}
       return $result;	
}

/**
 * Removes unnecessary  whitespaces
 *
 * @param  String $string
 * @return String 
 */
function no_white($string){
	$string = preg_replace('/\s+/',' ',$string);
	$string = preg_replace('/ $/','',$string);
	$string = preg_replace('/ \*/','*',$string);
	$string = preg_replace('/\* /','*',$string);
	$string = preg_replace('/ \[/','[',$string);
	$string = preg_replace('/\[ /','[',$string);
	$string = preg_replace('/ \]/','[',$string);
	$string = preg_replace('/\] /','[',$string);
	return $string;
}

/**
 * Scans an input file and parses it into 3 main arrays that are returned. 
 * Array $match contains 3 main parts: "ret_type" consisting of
 * the function return type, "name" = name of the function, "params" = parameters,
 * "arr" might include array expression if present.
 * Array $par_sep consists of separated parameters.
 * Array $pam_type consists of types of separated parameters.
 *
 * @param  String $content
 * @return array
 */
function process_input($content){
	$match = array();
	$par_sep = array();

	// Regex of indentifier
	$ident = '[a-zA-Z_]\w*';

	/* Parses return type in $match["ret_type"], function name in $match["name"] 
	   and parameters in $match["params"]*/
	preg_match_all('/\s*(?P<ret_type>(?:'.$ident.'[\s\*]+)+)(?P<name>'.$ident.
	')\s*\((?P<params>[\s\S]*?)\)(?P<arr>\s*\[\d*\])?\s*;/', $content, $match);

	// Concatenates return type with array sign
	for($i=0;$i<=count($match["ret_type"])-1;$i++){
		$match["ret_type"][$i] .= $match["arr"][$i];

		// Parse parameters
		array_push($par_sep,explode(",",$match["params"][$i]));
	}

	// Parameter arrays
	$pam_type = array();
	$p_match = array();

	// Parses parameters
	for($i=0;$i<=count($match["ret_type"])-1;$i++){
		for($j=0;$j<=count($par_sep[$i])-1;$j++){
			preg_match_all('/\s*(?P<type>(?:'.$ident.'[\s\*]+)+)(?P<name>(?:'.
			$ident.'))(?P<arr>\s*\[\d*\])?\s*/', $par_sep[$i][$j], $p_match);

			// Concatenates return type with array sign
			if(!empty($p_match["type"])and !empty($p_match["arr"]))
				$p_match["type"][0] .= $p_match["arr"][0];
			$pam_type[$i][$j] = $p_match["type"];

			// In case of no parameters or variable number of arguments
			if(empty($pam_type[$i][$j])){
				if(preg_match('/\s*\.\.\.\s*/',$par_sep[$i][$j]) != 1)
					$pam_type[$i][$j][0] = $par_sep[$i][$j];
				else
					$pam_type[$i][$j][0] = "";

			}	
		}
	}
	return array($match,$par_sep,$pam_type);
}

/**
 * Generates beginning of XML file. depending on script parameters.
 *
 * @param int $p_xml
 * @param String $outfile
 * @param String $file_dir 
 */
function generate_start_xml($p_xml,$outfile,$file_dir){
	if($p_xml == -1)
		fwrite($outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	else	
		fwrite($outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fwrite($outfile,"<functions dir=\"".$file_dir."\">");
}

/**
 * Generates beginning of XML file. depending on script parameters.
 *
 * @param array $match
 * @param String $outfile
 * @param array $par_sep 
 * @param bool $no_in
 * @param int $max_p
 * @param bool $rm_wh
 * @param bool $no_dup
 * @param int $p_xml
 * @param string $offset
 * @param string $nl
 * @param array $pam_type
 * @param string $file_name
 */
function generate_xml_body($match,$outfile,$par_sep,$no_in,$max_p,$rm_wh,
$no_dup,$p_xml,$offset,$nl,$pam_type,$file_name){
	$dup_arr = array();
	for($i=0;$i<=count($match["ret_type"])-1;$i++){

		// Replaces "\n" and "\t"
		$match["ret_type"][$i] = preg_replace("/[\t\n]/"," ",$match["ret_type"][$i]);

		// Trims beginning and end of string
		$match["ret_type"][$i] = trim($match["ret_type"][$i]);

		// Removes inline functions
		if($no_in and preg_match('/(^|\s+)inline\s+/',$match["ret_type"][$i]) == 1)
			continue;
		$var = "no";

		// Contains var_args?
		if(preg_match('/\s*\.\.\.\s*/',$par_sep[$i][count($par_sep[$i])-1]) == 1)
			$var = "yes";

		// Removes void parameter from number of parameters and set number
		// of parameters
		if((count($pam_type[$i]) == 1) and (preg_match('/^\s*(?:void)?\s*$/',
		$par_sep[$i][0]) == 1))
			$cnt = 0;
		else
			$cnt = count($pam_type[$i]);

		// Removes functions with number of parameters higher than limited by
		// script parameter
		if($max_p >= 0){
			if($var == "yes"){
				if((count($pam_type[$i])-1) > $max_p){
					continue;
				}
			}
			else{
				if($cnt > $max_p){
					continue;
				}
			}
		}

		// Removes whitespaces
		if($rm_wh)
			$match["ret_type"][$i] = no_white($match["ret_type"][$i]);
		if($no_dup){
			if(in_array($match["name"][$i],$dup_arr))
				continue;
			else
				array_push($dup_arr,$match["name"][$i]);
		}

		// Inserts function and its parameters to XML file
		fwrite($outfile,$nl.$offset."<function file=\"".$file_name."\" name=\"".
		$match["name"][$i]."\" varargs=\"".$var."\" rettype=\"".
		$match["ret_type"][$i]."\">");
		for($j=0;$j<=count($par_sep[$i])-1;$j++){
			if(!empty($pam_type[$i][$j][0])){
				if( !preg_match('/\s*(?:void)\s*/',$par_sep[$i][0]) == 1){
					if($rm_wh){
						$pam_type[$i][$j][0] = no_white(
						$pam_type[$i][$j][0]);
					}
					$pam_type[$i][$j][0] = preg_replace("/[\t\n]/"," ",
					$pam_type[$i][$j][0]);
					
					$pam_type[$i][$j][0] = trim($pam_type[$i][$j][0]);
					fwrite($outfile,$nl.$offset.$offset.
					"<param number=\"".($j+1)."\" type=\"".
					$pam_type[$i][$j][0]."\" />");
				}
			}
		}
		fwrite($outfile,$nl.$offset."</function>");
	}
}


/**
 * Main section
 */

// Globals
$files = array();
$_GET = get_arg_array($argv,$argc);

// Help
check_help($argc);

// Testing and parsing arguments
$file_dir = "";
$dirs = array();
$files = array();
list($file_dir,$dirs,$files) = check_input();
$outfile = check_output_file();
$nl="";
$offset = "";
$p_xml = -1;
list($nl,$offset,$p_xml) = check_p_xml();
$no_in = check_param("--no-inline");
$no_dup = check_param("--no-duplicates");
$rm_wh = check_param("--remove-whitespace");
$max_p = check_max_p();

// Checks input file for permisssions
check_perm($files);

// Generates xml head
generate_start_xml($p_xml,$outfile,$file_dir);

// Iterates files and parses all essential information
foreach($files as $f_path){

	// Reads and reduces input file
	$file_content = file_get_contents($f_path);
	$file_content = reduce($file_content);

	// Processes input file
	$pam_type = array();
	$match = array();
	$par_sep = array();
	list($match,$par_sep,$pam_type) = process_input($file_content);
	
	// Parses file name to string
	$file_name = parse_file_name($f_path,$file_dir);

	// Generates body of XML file
	generate_xml_body($match,$outfile,$par_sep,$no_in,$max_p,$rm_wh,$no_dup,
	$p_xml,$offset,$nl,$pam_type,$file_name);
}

// Inserts last line of XML file
if($p_xml == -1)
	fwrite($outfile,"</functions>\n");
else
	fwrite($outfile,"\n</functions>\n");

fclose($outfile);
?>
