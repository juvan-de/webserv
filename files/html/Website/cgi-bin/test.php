#!/usr/bin/env php

<?php
	echo "*************************************\n";
	parse_str(getenv('QUERY_STRING'), $result);
	echo getenv('QUERY_STRING');
	echo getenv('GATEWAY_INTERFACE');
	lol haha
	// echo $result['value1'], "\n";
	// echo $result['value2'], "\n";
	// echo $result['value3'], "\n";
	echo "<html>\n";
	echo "	<head>\n";
	echo "		<title>My Page Title</title>\n";
	echo "		<link rel=\"icon\" type=\"image/x-icon\" href=\"/images/favicon.ico\">\n";
	echo "	</head>\n";
	echo "	<body>\n";
	echo "		<h1>You Clicked The button!</h1>\n";
	echo "		<h1>", $result['value1'], " ", $result['value2'], "!</h1>\n";
	echo "	</body>\n";
	echo "</html>\n";
?>