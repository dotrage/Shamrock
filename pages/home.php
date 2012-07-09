<?php
	$build->enqueueStyle("style.css");

	$build->loadHeader();
	
	$build->loadTPL("home",null,null,true);
	
	$build->loadFooter();	
?>