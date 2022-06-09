#!/usr/bin/env php

<?php
	$env = getenv('CONTENT_LENGTH');
	$len = intval($env, 10);
	$f = fread(STDIN, $len);
	// echo $f;

	parse_str($f, $result);
	echo "<html>\n";
	echo "	<head>\n";
	echo "		<title>My Page Title</title>\n";
	echo "		<link rel=\"icon\" type=\"image/x-icon\" href=\"/images/favicon.ico\">\n";
	echo "	</head>\n";
	echo "	<body>\n";
	echo "		<h3>Congratulations you have been accepted to hell</h3>\n";
	echo "		<p>", $result['fname'], " ", $result['lname'], " will be allowed to enter hell</p>\n";
	echo "	</body>\n";
	echo "</html>\n";
?>