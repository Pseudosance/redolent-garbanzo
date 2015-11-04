<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add movie information </h1>

<form method="POST">
<div class="center">
<h2> Add your movie: </h2>
<p><strong>Title:</strong>
	<input type="text" name="title" maxlength="50"></input></p>
<p><strong>Year:</strong>
	<input type="text" name="year" maxlength="50"></input></p>
<p><strong>Rating:</strong>
	<select name="rating">
		<option value="G">G</option>
		<option value="PG">PG</option>
		<option value="PG-13">PG-13</option>
		<option value="R">R</option>
		<option value="NC-17">NC-17</option>
	</select></p>
<p><strong>Company:</strong>
	<input type="text" name="company" maxlength="50"></input></p>

<p><strong>Genre:</strong></p> <!-- See moviegenre.del -->
	<p><input type="checkbox" name="genre[]" value="action"/> Action
	<input type="checkbox" name="genre[]" value="adult"/> Adult
	<input type="checkbox" name="genre[]" value="adventure"/> Adventure
	<input type="checkbox" name="genre[]" value="animation"/> Animation
	<input type="checkbox" name="genre[]" value="comedy"/> Comedy</p>
	<p><input type="checkbox" name="genre[]" value="crime"/> Crime
	<input type="checkbox" name="genre[]" value="documentary"/> Documentary
	<input type="checkbox" name="genre[]" value="drama"/> Drama
	<input type="checkbox" name="genre[]" value="family"/> Family
	<input type="checkbox" name="genre[]" value="fantasy"/> Fantasy</p>
	<p><input type="checkbox" name="genre[]" value="horror"/> Horror
	<input type="checkbox" name="genre[]" value="musical"/> Musical
	<input type="checkbox" name="genre[]" value="mystery"/> Mystery
	<input type="checkbox" name="genre[]" value="romance"/> Romance
	<input type="checkbox" name="genre[]" value="sci-fi"/> Sci-Fi</p>
	<p><input type="checkbox" name="genre[]" value="short"/> Short
	<input type="checkbox" name="genre[]" value="thriller"/> Thriller
	<input type="checkbox" name="genre[]" value="war"/> War
	<input type="checkbox" name="genre[]" value="western"/> Western</p>
</p>

<p>
	<input type="submit" class="btn btn-primary" name="submit" value="Submit">
</p>
</div>
</form>

</body>
</html>

<?php

	$choices = array("title", "year", "rating", "company");

	$inputError = false;

	foreach ($choices as $choice)
	{
		if (isset($_POST[$choice])) {
			if (empty($_POST[$choice])) {
				$inputError = true;
			}
		}
	}

	if (isset($_POST["submit"])) {
		if ($inputError == true) {
				echo "<strong><p class='red center'>Argh! There was an input error - you probably forgot to enter a value. Try again.</p></strong>";
			}
		else if (!is_numeric($_POST["year"])) {
			echo "<strong><p class='red center'>Ouch! Your year isn't a number value, which is impossible. Try again.</p></strong>";
		}
		else if ($_POST["year"] <= 0) {
			echo "<strong><p class='red center'>Ouch! Your year was less than 0, which is way too early for a movie. Try again.</p></strong>";
		}
		else { // populate our movieGenres array based on the checkbox values the user inputted
			$addedMovieInfo = false;
			$movieGenres = array();

			// Establish database connection and do basic error handling
			$databaseConnection = mysql_connect("localhost", "cs143", "");
			if(!$databaseConnection) {
				echo "<strong><p class='red center'>Unfortunately, we could not connect to the database. Sorry.</p></strong>" . mysql_error();
				exit;
			}

			if (!mysql_select_db("CS143", $databaseConnection)) {
				echo "<strong><p class='red center'>Unfortunately, could not select database.</p></strong>" . mysql_error();
				exit;
			}

			for($i = 0; $i <= 18; $i++) {
				if(isset($_POST["genre"][$i])) {
					array_push($movieGenres, $_POST["genre"][$i]);
				}
			}

			$idFROMmaxMovieId = "SELECT id FROM MaxMovieID";
			$rowOfIdsFromMaxMovieId = mysql_fetch_row(mysql_query($idFROMmaxMovieId, $databaseConnection));
			$id = $rowOfIdsFromMaxMovieId[0]; // id from first row

			$addMovieInfo = "INSERT INTO Movie (id, title, year, rating, company)
							VALUES ($id, '$_POST[title]', $_POST[year], '$_POST[rating]', '$_POST[company]')";

			//echo "$mysql_query($addMovieInfo, $databaseConnection)";

			if (mysql_query($addMovieInfo, $databaseConnection)) {
				//echo"yay";
				foreach($movieGenres as $movieGenre){
					mysql_query("INSERT INTO MovieGenre 
								VALUES ($id, '$movieGenre')", $databaseConnection);
				}

				$addedMovieInfo = true;
				echo "<strong><p class='center green'>Congrats! You added the movie information to the database!</p></strong>";
			}
			else {
				echo "why";
				echo "<strong><p class='center red'>There was a problem adding the movie information to the database. Sorry.</p></strong>";
			}

			$updateMovieInfo = "";
			if($addedMovieInfo == true) {
				$updateMovieInfo = "UPDATE MaxMovieID 
									SET id = id + 1";
				mysql_query($updateMovieInfo, $databaseConnection);
			}

			mysql_close($databaseConnection);
		}
	}
?>