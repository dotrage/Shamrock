<?php 
	require_once($_SERVER['DOCUMENT_ROOT']."/config.php");	
	require_once($_SERVER['DOCUMENT_ROOT']."/inc/native/build.inc");		
	
	$build = new Build;

	$build->get_vars(get_defined_vars());
	
	$build->init();	

	$build->loadPage();			
?>