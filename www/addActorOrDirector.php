<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add a new actor or director </h1>

<div>
	<form class="center" action="addActorOrDirector.php" method="POST">
		<h3> <i> ( * required) </i> </h3>
		<strong><p> (*) Actor or director: </p></strong>	
		<p><input type="radio" name="actorOrDirector" value="Actor" checked="true"> Actor
			<input type="radio" name="actorOrDirector" value="Director"> Director<br/><br/></p>
		<strong><p> (*) First Name: </strong><input type="text" name="first" maxlength="20"><br/><br/>
		<strong>(*) Last Name: </strong><input type="text" name="last" maxlength="20"><br/><br/></p>
		<strong><p>(*) Sex:</p></strong>	
		<p><input type="radio" name="sex" value="Male" checked="true"> Male
			<input type="radio" name="sex" value="Female"> Female<br/><br/></p>

		<strong><p>(*) Date of Birth:</p></strong>

		<p><strong>(*) Month: </strong> 
		<select name="dobMonth" class="center"><option value="0"></option>
			<?php 
			 	$months = array( '01' => 'January',
			 					 '02' => 'February', 
			 					 '03' => 'March',
			 					 '04' => 'April', 
			 					 '05' => 'May', 
			 					 '06' => 'June',
			 					 '07' => 'July', 
			 					 '08' => 'August', 
			 					 '09' => 'September',
		 						 '10' => 'October', 
		 						 '11' => 'November', 
		 						 '12' => 'December' );
				foreach ($months as $number => $month) echo "<option value=\"$number\">$month</option>"; 
			 ?>
		 </select>

	 	<strong>(*) Day: </strong> 
	 	<select name="dobDay" class="center"><option value="0"></option>
		 	<?php 
		 	 	for ($day=01; $day <= 31; $day++) 
		 	 		echo "<option value=\"$day\">$day</option>"; 
		 	?>
	 	</select>

	 	<strong>(*) Year: </strong> 
	 	<select name="dobYear" class="center"><option value="0"></option>
		 	<?php 
		 	 	for ($year=1895; $year <= 2018; $year++) 
		 	 		echo "<option value=\"$year\">$year</option>"; 
		 	?>
	 	</select>

		</p>

		<strong><p>(*) Date of Death: </strong></p>
		<p><i>Do not select anything if actor/director is still alive.</i></p>

		<p><strong>(*) Month: </strong> 
		<select name="dodMonth" class="center"><option value="0"></option>
			<?php 
				foreach ($months as $number => $month) echo "<option value=\"$number\">$month</option>"; 
			 ?>
		 </select>

	 	<strong>(*) Day: </strong> 
	 	<select name="dodDay" class="center"><option value="0"></option>
		 	<?php 
		 	 	for ($day=01; $day <= 31; $day++) 
		 	 		echo "<option value=\"$day\">$day</option>"; 
		 	?>
	 	</select>

	 	<strong>(*) Year: </strong> 
	 	<select name="dodYear" class="center"><option value="0"></option>
		 	<?php 
		 	 	for ($year=1895; $year <= 2018; $year++) 
		 	 		echo "<option value=\"$year\">$year</option>"; 
		 	?>
	 	</select>

		</p>

		<p><input type="submit" name="submit" value="Submit"/></p>
	</form>
</div>

</body>
</html>

<?php

$databaseConnection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $databaseConnection);

if (!$databaseConnection) {
		echo "<strong><p class='red center'>Unfortunately, we could not connect to the database. Sorry.</p></strong>" . mysql_error();
		exit;
}

$choices = array("actorOrDirector", "first", "last", "sex", "dobMonth", "dobDay", "dobYear");
$inputError = false;
$dateOfDeathError = false;

foreach ($choices as $choice)
{
	if (isset($_POST[$choice])) {
		if (empty($_POST[$choice])) {
			$inputError = true;
		}
	}
}

if (!empty($_POST[dobMonth]) && !empty($_POST[dobDay]) && !empty($_POST[dobYear]) && !empty($_POST[dodMonth]) && !empty($_POST[dodDay]) && !empty($_POST[dodYear])){
	if ($_POST[dodYear] < $_POST[dobYear]) {
		$inputError = true;
		$dateOfDeathError = true;
	}
	if ($_POST[dodYear] == $_POST[dobYear]) {
		if ($_POST[dodMonth] < $_POST[dobMonth]) {
			$inputError = true;
			$dateOfDeathError = true;
		}
		if ($_POST[dodMonth] == $_POST[dobMonth]) {
			if ($_POST[dodDay] < $_POST[dobDay]) {
				$inputError = true;
				$dateOfDeathError = true;
			}
		}
	}
	// otherwise, valid date of death!
}

// echo $first;

if (isset($_POST["submit"])) {
	if ($inputError == true) {
		if ($dateOfDeathError == true) {
			echo "<strong><p class='red center'>Oh no! The date of death you entered is impossible.</p></strong>";
		}
		else {
			echo "<strong><p class='red center'>Argh! There was an input error. Try again.</p></strong>";
		}
	}
	else {
			$idFROMmaxPersonId = "SELECT id FROM MaxPersonID";
			$rowOfIdsFromMaxPersonId = mysql_fetch_row(mysql_query($idFROMmaxPersonId, $databaseConnection));
			$id = $rowOfIdsFromMaxPersonId[0]; // id from first row
			$personIsDead = false;
			$dob = "$dobYear-$dobMonth-$dobDay"; 
			$dod = "$dodYear-$dodMonth-$dodDay"; 
			//$resultResource = "";

			if (empty($_POST['dod']))
			{
				$personIsDead = true;
			}

			if ($_POST['actorOrDirector'] == "Director") 
			{
				if ($personIsDead == true) {
					$addDirector = "INSERT INTO $_POST[actorOrDirector] (id, last, first, dob, dod)
									VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[dob]', NULL)";
					//$resultResource = mysql_query($addDirector, $databaseConnection);
				}
				else {
					$addDirector = "INSERT INTO $_POST[actorOrDirector] (id, last, first, dob, dod)
									VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[dob]', '$_POST[dod]')";
					//$resultResource = mysql_query($addDirector, $databaseConnection);
				}
			}
			else // if ($_POST['actorOrDirector'] == "Actor")
			{
				if ($personIsDead == true) {
					$addActor = "INSERT INTO $_POST[actorOrDirector] (id, last, first, sex, dob, dod)
								VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[sex]', '$_POST[dob]', NULL)";
					//$resultResource = mysql_query($addActor, $databaseConnection);
				}
				else {
					$addActor = "INSERT INTO $_POST[actorOrDirector] (id, last, first, sex, dob, dod)
								VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[sex]', '$_POST[dob]', '$_POST[dod]')";
					//$resultResource = mysql_query($addActor, $databaseConnection);
				}
			}

			if (strcmp($_POST['actorOrDirector'], "Director") == 0) {
				if (mysql_query($addDirector, $databaseConnection)) {
					echo "<strong><p class='center green'>Congrats! You added the director to the database!</p></strong>";
				}
				else {
					echo "<strong><p class='center red'>There was a problem adding the director to the database. Sorry.</p></strong>";
				}
			}
			else if (strcmp($_POST['actorOrDirector'], "Actor") == 0) {
				if (mysql_query($addActor, $databaseConnection)) {
					echo "<strong><p class='center green'>Congrats! You added the actor to the database!</p></strong>";
				}
				else {
					echo "<strong><p class='center red'>There was a problem adding the actor to the database. Sorry.</p></strong>";
				}
			}
			 // Successful addition!
			//echo "got here";
	 		$updateActorOrDirector = "UPDATE MaxPersonID 
	 								SET id = id + 1";
	 		$updateResultResource = mysql_query($updateActorOrDirector, $databaseConnection);	
		}
}
mysql_close(databaseConnection);

?>