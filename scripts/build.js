String.prototype.replaceAll = function(str1, str2, ignore){
	return this.replace(new RegExp(str1.replace(/([\/\,\!\\\^\$\{\}\[\]\(\)\.\*\+\?\|\<\>\-\&])/g,"\\$&"),(ignore?"gi":"g")),(typeof(str2)=="string")?str2.replace(/\$/g,"$$$$"):str2);
}

function Build() {
    this.loadTPL = function(obj){
    	//template = name of template file
    	//variables = variables to fill
    	//data_source = ajax url to call
    	//path = custom path directory
    	//directory = subdir for template
    	//target = target to send data
    	//mode = mode to use (html,after,before,append,return)
    	//callback = callback function	
    	
    	if (obj.data_source != undefined && typeof(obj.data_source) == "object"){
    		this.processDataSource(obj);
    		return;
    	}
    	
    	if (obj.template != undefined && obj.template != null && obj.target != undefined && obj.target != null){
    		if (obj.method != undefined && obj.method != null){
    			var method = obj.method;
    		}
    		else{
    			var method = "GET";
    		}
    		
    		var path = "/templates/";
    		
    		if (obj.path != undefined && obj.path != null){
    			path += obj.path+"/";    			
    		}
    		else{
    			path += "partials/";
    		}
    		
    		if (obj.directory != undefined && obj.directory != null){
    			obj.directory = obj.directory.replace("/","");
    			path += obj.directory + "/";
    		}
    		var ts = Math.round((new Date()).getTime() / 1000);
    		path += obj.template.replaceAll("/","") + ".htm?ts="+ts;	
    		
    		$.ajax({
    			type: method,
    			url: path,
    			success: function(html){    				
    				if (obj.variables != undefined && obj.variables != null){
    					for(var k in obj.variables){    						
    						html = html.replaceAll("{{"+k+"}}",obj.variables[k]);    						
    					}    					
    				}
    				if (obj.mode == undefined || obj.mode == null)
    					obj.mode = "html";
    				
    				switch (obj.mode){
    					case "html":
    						obj.target.html(html);
    						break;
    					case "append":
    						obj.target.append(html);
    						break;
    					case "after":
    						obj.target.after(html);
    						break;
    					case "before":
    						obj.target.before(html);
    						break;
    					case "return":
    						return html;
    						break;
    				}
    				
    				//callback
    				if (obj.callback != undefined && obj.callback != null && typeof obj.callback == "function"){
    					var cb = obj.callback;
    					cb(obj.variables);
    				}
    			},
    			error: function (jqXHR, textStatus, errorThrown){
    				alert("An error has occurred.  Unable to load page at this time.");
    				console_log(textStatus);
    				console_log(errorThrown);
    			}
    		})
    	}
    }
    
    this.iterateTPL = function(obj){
    	//template = name of template file
    	//variables = variables to fill
    	//path = custom path directory
    	//directory = subdir for template
    	//target = target to send data
    	//mode = mode to use (html,after,before,append)
    	//callback = callback function	
    	
    	if (obj.template != undefined && obj.template != null && obj.target != undefined && obj.target != null){
    		if (obj.method != undefined && obj.method != null){
    			var method = obj.method;
    		}
    		else{
    			var method = "GET";
    		}
    		
    		var path = "/templates/";
    		
    		if (obj.path != undefined && obj.path != null){
    			path += obj.path+"/";    			
    		}
    		else{
    			path += "partials/";
    		}
    		
    		if (obj.directory != undefined && obj.directory != null){
    			obj.directory = obj.directory.replace("/","");
    			path += obj.directory + "/";
    		}
    		var ts = Math.round((new Date()).getTime() / 1000);
    		path += obj.template.replaceAll("/","") + ".htm?ts="+ts;	
    		
    		$.ajax({
    			type: method,
    			url: path,
    			success: function(tmp_html){
    				html = "";
    				
    				if (obj.variables != undefined && obj.variables != null && obj.variables.length > 0){    					
    					for (i=0;i<obj.variables.length;i++){    						
    						this_html = tmp_html;
	    					for (var k in obj.variables[i]){    
	    						this_html = this_html.replaceAll("{{"+k+"}}",obj.variables[i][k]);    						
	    					}
	    					html += this_html;
    					}    					
    				}
    				else{
    					return false;
    				}
    				
    				if (obj.mode == undefined || obj.mode == null)
    					obj.mode = "html";
    				
    				switch (obj.mode){
    					case "html":
    						obj.target.html(html);
    						break;
    					case "append":
    						obj.target.append(html);
    						break;
    					case "after":
    						obj.target.after(html);
    						break;
    					case "before":
    						obj.target.before(html);
    						break;
    				}
    				
    				//callback
    				if (obj.callback != undefined && obj.callback != null && typeof obj.callback == "function"){
    					var cb = obj.callback;
    					cb(obj.variables);
    				}
    			},
    			error: function (jqXHR, textStatus, errorThrown){
    				alert("An error has occurred.  Unable to load page at this time.");
    				console_log(textStatus);
    				console_log(errorThrown);
    			}
    		})
    	}
    }
    
    this.processDataSource = function(obj){
    	if (obj.data_source.script != undefined){
    		var url = "/d/"+obj.data_source.script;
    		var type = "GET";
    		var data = null;
    		var up = this;
    		
    		if (obj.variables != undefined && obj.variables != null){
    			var vars = obj.variables;
    		}
    		else{
    			var vars = {};    			
    		}
    		
    		if (obj.data_source.type != undefined){
    			type = obj.data_source.type;
    		}
    		if (obj.data_source.data != undefined){
    			data = obj.data_source.data;
    		}
    		
    		$.ajax({
    			url: url,
    			type: type,
    			data: data,
    			success: function(o){
    				var out = $.parseJSON(o);
    				
    				obj.variables = merge_options(vars,out);
    				obj.data_source = null;
    				up.loadTPL(obj);
    			}
    		});    		
    	}
    }
};


function merge_options(obj1,obj2){
    var obj3 = {};
    for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
    for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
    return obj3;
}
