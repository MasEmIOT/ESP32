<?php

$hostname = "localhost";
$username = "root";
$password = "123";
$database = "Denso_Vibration";

$connect = @mysqli_connect($hostname, $username, $password, $database);
if(!$connect){
	echo "Error: " . mysqli_connect_error();
        exit();
}

echo "Connection Success!<br>";

	$b1 = $_GET["SW420"];
	$b2 = $_GET["AccX"];
	$b3 = $_GET["AccY"];
	$b4 = $_GET["AccZ"];
  $sql = "INSERT INTO Acc (SW420,AccX,AccY,AccZ) VALUES (".$b1.",".$b2.",".$b3.",".$b4.")";
$result = mysqli_query($connect,$sql);
echo "Insertion Success!<bt>"
?>
