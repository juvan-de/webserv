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
	echo "		<h1>You Clicked The button!</h1>\n";
	echo "		<h1>Congratulations you have been accepted to hell</h1>\n";
	echo "		<h1>", $result['fname'], " ", $result['lname']," has been added to the list of people to go to hell</h1>\n";
	echo "	</body>\n";
	echo "</html>\n";
?>