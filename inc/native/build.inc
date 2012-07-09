<?php 
	class Build {
		function __construct(){
			$this->slug = $this->slug();
			$this->hooks = array();
			$this->scripts = array();			
		}
		
		public function init(){
			$this->load_classes();
		}
		
		public function loadHeader($custom=null){
			global $build;
			
			$this->processBeforeHooks("build","header");
			
			$headerstyles = $this->load_header_styles();
			$headerscripts = $this->load_header_scripts();			
			
			if (!empty($custom) && file_exists($_SERVER['DOCUMENT_ROOT']."/".$custom.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/".$custom.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/header.php");
			}
			
			$this->processAfterHooks("build","header");
		}
		
		public function loadPage($page=null){
			if (!empty($this->page_vars)){
				foreach($this->page_vars as $k=>$v){
					$$k = $v;
				}
			}
			
			if (empty($page)){
				$page = (!empty($this->slug['page']))?$this->slug['page']:"home";
			}
			
			if ($page == "d"){
				$this->loadDataPage();
			}			
			else if (file_exists($_SERVER['DOCUMENT_ROOT']."/pages/".$page.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/pages/".$page.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/pages/home.php");
			}
		}
		
		public function loadDataPage(){
			$args = $this->slug['args'];
			$this->slug['page'] = $args[0];
			array_shift($args);
			$this->slug['args'] = $args;
			
			$build = $this;
			
			if (file_exists($_SERVER['DOCUMENT_ROOT']."/d/".$this->slug['page'].".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/d/".$this->slug['page'].".php");
			}
		}
		
		public function loadFooter($custom=null){
			global $build;
			
			$this->processBeforeHooks("build","footer");
			
			if (!empty($custom) && file_exists($_SERVER['DOCUMENT_ROOT']."/".$custom.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/".$custom.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
			}
			
			$this->processAfterHooks("build","footer");
		}
		
		public function loadTPL($template,$vars=array(),$subdir=null,$echo=false){
			if (!empty($template)){
				if (!empty($subdir)){
					if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/".$subdir."/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/".$subdir."/".$template.".htm";
					}
				}
				else{
					if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/native/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/native/".$template.".htm";
					}
					else if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/pages/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/pages/".$template.".htm";
					}
					else if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/".$template.".htm";
					}
				}
				
				if (!empty($file)){
					$html = file_get_contents($file);
					
					if (!empty($html)){
						if (!empty($vars) && is_array($vars)){
							foreach ($vars as $k=>$v){
								$html = str_replace("{{".$k."}}",$v,$html);
							}
						}
						
						if ($echo){
							echo $html;	
						}
						else{
							return $html;
						}
					}
				}
			}
		}
		
		public function get_vars($vars=array()){
			if (!empty($vars)){
				$this->page_vars = $vars;
			}
		}
		
		public function setPageTitle($title){
			if (!empty($title)){
				$this->page_title($title);
			}
		}
		
		public function hookBefore($target=null,$function=null){
			if (!empty($target) && !empty($function)){
				if (is_array($target)){
					$this->hooks[$target[0]]['before'][$target[1]][] = $function;
				}
				else{
					$this->hooks['build']['before'][$target][] = $function; 
				}
			}
		}
		
		public function hookAfter($target=null,$function=null){
			if (!empty($target) && !empty($function)){
				if (is_array($target)){
					$this->hooks[$target[0]]['after'][$target[1]][] = $function;
				}
				else{
					$this->hooks['build']['after'][$target][] = $function;
				}
			}
		}
		
		public function processBeforeHooks($class=null,$action=null){
			if (!empty($class)){
				if (!empty($this->hooks[$class]['before'][$action]) && is_array($this->hooks[$class]['before'][$action])){
					$hooks = $this->hooks[$class]['before'][$action];
				}
			}
			else{
				if (!empty($this->hooks['build']['before'][$action]) && is_array($this->hooks['build']['before'][$action])){
					$hooks = $this->hooks['build']['before'][$action];
				}
			}
				
			if (!empty($hooks)){
				foreach ($hooks as $hook){
					$this->apply_hook($hook);
				}
			}
		}
		
		public function processAfterHooks($class=null,$action=null){
			if (!empty($class)){
				if (!empty($this->hooks[$class]['after'][$action]) && is_array($this->hooks[$class]['after'][$action])){
					$hooks = $this->hooks[$class]['after'][$action];
				}
			}
			else{
				if (!empty($this->hooks['build']['after'][$action]) && is_array($this->hooks['build']['after'][$action])){
					$hooks = $this->hooks['build']['after'][$action];
				}
			}
		
			if (!empty($hooks)){
				foreach ($hooks as $hook){
					$this->apply_hook($hook);
				}
			}
		}
		
		public function loadClass($obj=null,$args=array(),$name=null){
			if (!empty($obj) && class_exists($obj)){
				if (empty($name)){
					$name = strtolower($obj);
				}				
				
				if (!empty($args)){
					$class = new ReflectionClass($name);					
					$this->$name = $class->newInstanceArgs($args);
				}
				else{
					$this->$name = $obj;
				}
			}
		}
		
		public function enqueueStyle($style){
			$this->styles[] = $style;
		}
		
		public function enqueueScript($script){
			$this->scripts[] = $script;
		}
		
		
		
		
		
		
		
		
		
		
		private function slug(){
			if (!empty($_GET['uri'])){
				if (strpos($_GET['uri'],"?")){
					$tmp = explode("?",trim($_GET['uri'],"/"));
					$tmp = $tmp[0];
					$tmp = explode("/",$tmp);
					$slug['page'] = $tmp[0];
					array_shift($tmp);
					$slug['args'] = $tmp;
										
					return $slug;
				}
				else{
					$tmp = explode("/",trim($_GET['uri'],"/"));
					$slug['page'] = $tmp[0];
					array_shift($tmp);
					$slug['args'] = $tmp;
					return $slug;
				}
			}
			else{
				return array("page"=>"home");
			}
		}
		
		private function load_header_styles(){
			$html = "";
			if (!empty($this->styles)){
				foreach ($this->styles as $style){
					if (is_array($style)){
						$media = "";
						if (!empty($style['media'])){
							$media = " media=\"".$style['media']."\"";
						}						
						if (!empty($style['file'])){
							$html .= "<link href=\"/css/".$style['file']."\"".$media." rel=\"stylesheet\" type=\"text/css\">";
						}						
					}
					else{
						$html .= "<link href=\"/css/".$style."\" rel=\"stylesheet\" type=\"text/css\">";
					}
				}
			}
			
			return $html;
		}
		
		private function load_header_scripts(){
			$html = "";
			if (!empty($this->scripts)){								
				foreach ($this->scripts as $script){
					if (is_array($script)){
						if (!empty($script['require'])){
							$html .= "<script data-main=\"/scripts/".$script['require']."\" src=\"/scripts/require-jquery.js\"></script>\n";
						}						
					}
					else{
						$html .= "<script src=\"/scripts/".$script.".js\"></script>\n";
					}
				}				
			}
			
			return $html;
		}
		
		private function loaded_files(){
			$files = get_included_files();
			$payload = array();
			foreach ($files as $file){
				$payload[$file] = 1;
			}
			
			return $payload;
		}
		
		private function load_classes(){
			global $build;
			
			$loaded_files = $this->loaded_files();
			
			if ($handle = opendir($_SERVER['DOCUMENT_ROOT']."/inc/native")) {
				while (false !== ($entry = readdir($handle))) {
					if ($entry != "." && $entry != ".." && $entry != "build.inc") {
						$file = file_get_contents($_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry);
						$tokens = token_get_all($file);
						$class_token = true;
						$classes = array();
						foreach ($tokens as $token){
							if (is_array($token)){
								if ($token[0] == T_CLASS){
									$class_token = true;
								} 
								else if ($class_token && $token[0] == T_STRING){
									$class_name = strtolower($token[1]);
									if (empty($this->$class_name)){
										$classes[$class_name] = $token[1];
									}									
									$class_token = false;
								}
							}
						}
						if (!empty($classes) && empty($loaded_files[$_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry])){
							require_once($_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry);
							foreach($classes as $k=>$v){
								$this->$k = new $v;
							}
						}
					}
				}
			}
			
			if ($handle2 = opendir($_SERVER['DOCUMENT_ROOT']."/inc/custom")) {
				while (false !== ($entry = readdir($handle2))) {
					if ($entry != "." && $entry != "..") {						
						$file = file_get_contents($_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry);
						$tokens = token_get_all($file);
						$class_token = true;
						$classes = array();
						foreach ($tokens as $token){
							if (is_array($token)){
								if ($token[0] == T_CLASS){
									$class_token = true;
								}
								else if ($class_token && $token[0] == T_STRING){
									$class_name = strtolower($token[1]);
									if (empty($this->$class_name)){
										$classes[$class_name] = $token[1];
									}
									$class_token = false;
								}
							}
						}
						if (!empty($classes) && empty($loaded_files[$_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry])){
							require_once($_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry);
						}
					}
				}
			}
		}
		
		private function apply_hook($hook){
			if (function_exists($hook['function'])){
				if (!empty($hook['args'])){
					$hook;
				}
				else{
					$hook;
				}
			}
		}		
	}
?>