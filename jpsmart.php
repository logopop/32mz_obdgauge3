<?php
$date 	= $_GET['date'];    
$time 	= $_GET['time']; 
$lat 	= $_GET['lat'];    
$lon 	= $_GET['lon']; 
$speed 	= $_GET['spd'];
$dir 	= $_GET['dir'];
$alti   = $_GET['alt'];
$rad    = $_GET['rad'];
$temp   = $_GET['tmp'];
$used   = $_GET['usd'];
$seen   = $_GET['sn'];
$satm   = $_GET['sm'];
$random = $_GET['rand'];

//$ip	= $REMOTE_ADDR; 
$ip = $_SERVER['REMOTE_ADDR'];
$netname = gethostbyaddr($ip);

$logstring = "$date $time Pos: $lat,$lon Speed:$speed Bearing:$dir Alt:$alti\n$rad uSi $temp c Sats:$used/$seen Mode:$satm Rand:$random\nIp: $ip ($netname)\n\n";
@file_put_contents('jpsmart_log.txt', $logstring, FILE_APPEND);

$logstring = "$date $time http://maps.google.com/maps?q=$lat,$lon  Speed:$speed  Bearing:$dir \n";
@file_put_contents('jpsmart_lastpos.txt', $logstring);

// Prepare and send reply
header('Content-Type: application/txt');
$command = @file_get_contents('command.txt'); 
echo $command;

//----------------------------------------------
// Create or modify xml file for web map
$xml = new DOMDocument("1.0", "ISO-8859-15");

$xml_container = $xml->createElement("jpdata");
$xml_position = $xml->createElement("position");

$xml_time = $xml->createElement("time", $time);
$xml_date = $xml->createElement("date", $date);
$xml_lat = $xml->createElement("lat", $lat);
$xml_lon = $xml->createElement("lon", $lon);
$xml_spd = $xml->createElement("spd", $speed);
$xml_hdg = $xml->createElement("hdg", $dir);
$xml_rad = $xml->createElement("radiation", $rad);

$xml_position->appendChild($xml_date); 
$xml_position->appendChild($xml_time); 
$xml_position->appendChild($xml_lat); 
$xml_position->appendChild($xml_lon); 
$xml_position->appendChild($xml_spd); 
$xml_position->appendChild($xml_hdg); 
$xml_position->appendChild($xml_rad); 

$xml_container->appendChild($xml_position);
$xml->appendChild($xml_container);

$xml->formatOutput = true;
$xml->save("pos.xml");

//To re-open and write:

//$xml = new DOMDocument();
//$xml->formatOutput = true;
//$xml->load('/tmp/test.xml');
//$nodes = $xml->getElementsByTagName('Album') ;
//if ($nodes->length > 0) 
//{
   //insert some stuff using appendChild()
//}
//re-save
//$xml->save("/tmp/test.xml");









?>













 

