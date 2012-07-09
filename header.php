<?php 
	$vars = array();
	
	if (!empty($build->page_title)){
		$vars['page_title'] = $build->page_title;
	}
	else if (defined("SITE_TITLE")){
		$vars['page_title'] = SITE_TITLE;
	}
	else{
		$vars['page_title'] = "Shamrock Framework";
	}
	
	$vars['styles'] = $headerstyles;
	$vars['scripts'] = $headerscripts;

	$build->loadTPL("header",$vars,null,true);
?>