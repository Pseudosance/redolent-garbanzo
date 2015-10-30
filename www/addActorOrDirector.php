<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add a new actor or director </h1>

<div>
	<form class="center" action="addActorOrDirector.php" method="POST">
		<strong><p>Actor or director:</p></strong>	
		<p><input type="radio" name="actorOrDirector" value="Actor" checked="true"> Actor
			<input type="radio" name="actorOrDirector" value="Director"> Director<br/><br/></p>
		<strong><p>First Name: </strong><input type="text" name="first" maxlength="20"><br/><br/>
		<strong>Last Name: </strong><input type="text" name="last" maxlength="20"><br/><br/></p>
		<strong><p>Sex:</p></strong>	
		<p><input type="radio" name="sex" value="Male" checked="true"> Male
			<input type="radio" name="sex" value="Female"> Female<br/><br/></p>
		<strong><p>Date of Birth (yyyymmdd):</p></strong>
		<p><input type="text" name="dob"><br/><br/></p>
		<strong><p>Date of Death (blank if still alive):</p></strong>	
		<p><input type="text" name="dod"><br/><br/></p>
		<p><input type="button" class="btn btn-primary" name="submit" value="Submit"></p>
	</form>
</div>

</body>
</html>

<?php

$databaseConnection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $databaseConnection);

$choices = array("actorOrDirector", "first", "last", "sex", "dob");

if (isset($_POST["submit"])) {

		$idFROMmaxPersonId = "SELECT id FROM MaxPersonID";
		$rowOfIdsFromMaxPersonId = mysql_fetch_row(mysql_query($idFROMmaxPersonId, $databaseConnection));
		$id = $rowOfIdsFromMaxPersonId[0]; // id from first row

		if ($_POST['actorOrDirector'] == "Director") 
		{
			$addDirector = "INSERT INTO $_POST[actorOrDirector] (id, last, first, dob, dod)
							VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[dob]', '$_POST[dod]')";
			$resultResource = mysql_query($addDirector, $databaseConnection);
		}
		else // if ($_POST['actorOrDirector'] == "Actor")
		{
			$addActor = "INSERT INTO $_POST[actorOrDirector] (id, last, first, sex, dob, dod)
						VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[sex]', '$_POST[dob]', '$_POST[dod]')";
			$resultResource = mysql_query($addActor, $databaseConnection);
		}

		// if (!$result)
 
 		// Successful addition
 		$updateActorOrDirector = "UPDATE MaxPersonID 
 								SET id = id + 1";
 		$updateResultResource = mysql_query($updateActorOrDirector, $databaseConnection);
}

mysql_close(databaseConnection);

?>