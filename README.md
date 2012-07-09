Shamrock Framework
==================

Shamrock is a lightweight PHP framework that utilizes jQuery to create optimal user experiences through fast loading AJAX calls.  With Shamrock, rapid development of fast data-driven web applications is easy.  The framework's core components are a central build object, jQuery delivered via Require.JS, PHP and JavaScript template loaders, a collection of front end pages and URIs that return data to jQuery ajax requests.

Data Source Indifference
------------------------

While Shamrock was designed to be a data driven framework, it is indifferent to where the data comes from and how data it's passed to processes.  Shamrock was originally developed to work with a separate API, where the framework itself did not possess a connection to the application's database.  Interactions with the application's data source should utilize a native class or import one using Shamrock's **loadClass** function.

Getting Started
---------------

Creating your first page is simple.  Any page can be created and instantly get picked up in a cleaner URL format.  Below is an example of a page that will load through http://example.com/hello-world

Example for `/pages/hello-world.php`:
``` php
$build->enqueueScript(array("require" => "page-hello-world"));

$build->loadHeader();

$in['time'] = date("m/d/Y g:i:s a");
$build->loadTPL("hello-world",$in,null,true);

$build->loadFooter();
```

Example for `/templates/pages/hello-world.htm`:
``` html
<h1>Hello World</h1>
<p>This is your first page. The current date & time is: {{time}}</p>
<div id="ip"></div>
```

Example for `/scripts/page-hello-world.js`:
``` javascript
require(["jquery", "jquery.cookie", "bootstrap", "build"], function($) {
    $(function() {
        var build = new Build;

        build.loadTPL({
			template : "test-ip",
			data_source: {script:"hello-world-ip"},
			target : $("#ip")
		});
    });
});
```

Example for ajax script located at `/d/hello-world-ip.php`, addressable at `/d/hello-world-ip`:
``` php
$ip_address = $_SERVER['REMOTE_ADDR'];

$data = array("ip_address" => $ip_address);

echo json_encode($data);
```

The collection of examples above will produce the following html output at `http://example.com/hello-world`:
``` html
<!doctype html>
<html lang="en">
<head>
<title>Shamrock Framework</title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1.0, minimum-scale=1.0">
<script data-main="/scripts/page-hello-world" src="/scripts/require-jquery.js"></script>

</head>
<body>
<h1>Hello World</h1>
<p>This is your first page. The current date & time is: 07/08/2012 12:48:29 am</p>
<div id="ip">127.0.0.1</div>
<hr>
Shamrock is an open source project of Function Interactive LLC
</body>
</html>
```

Native Elements
---------------
Shamrock comes with a number of native content blocks, php classes, and JavaScript scripts.  If you notice the html output from the examples above, you'll notice the output contains full html tags for header and footer areas.  These are native templates that are located in `/templates/native/header.htm` and `/templates/native/footer.htm`.  Require.js is a standard of Shamrock for modularizing and streamlining JavaScript assets.  We highly recommend you checkout [Require.js][0] before working with JavaScript in Shamrock.

Customizing JavaScript
----------------------
Enqueueing JavaScript on Shamrock is easy and can be quickly implemented from your php page file.  Take a look at the example for `/pages/hello-world.php` above.  The first line of this example shows the following code:
``` php
$build->enqueueScript(array("require" => "page-hello-world"));
```
This example shown above will cause the following to be added to the page's `HEAD` tag:
``` html
<script data-main="/scripts/page-hello-world" src="/scripts/require-jquery.js"></script>
```
If you don't wish to use the `data-main` attribute via Require.js, you can enqueue a script using the following:
``` php
$build->enqueueScript("page-hello-world");
```
This example shown above will cause the following to be added to the page's `HEAD` tag:
``` html
<script src="/scripts/page-hello-world.js"></script>
```

For more developer documentation, visit the full [Shamrock Framework Documentation][1].

## Credits ##
	
* Chris Ennis - [dotrage][2]
	
	
	

[0]: http://requirejs.org/docs/jquery.html
[1]: https://github.com/dotrage/Shamrock/wiki
[2]: https://github.com/dotrage