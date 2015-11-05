<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<html>
<body>



<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<div class="center">
	<h1><p>Search Actor/Movie: </p></h1>

	<form action="./search.php" method="GET">
		<input type="text" name="search">
		<input class="btn btn-primary btn-sm" type="submit" value="Search Actors and Movies">
	</form>

</div>

</body>
</html>

<?php
	if (!empty($_GET['search'])) {
		
		$databaseConnection = mysql_connect("localhost", "cs143", "");

		if (!$databaseConnection) {
			echo "<strong><p class='red center'>Unfortunately, we could not connect to the database. Sorry.</p></strong>" . mysql_error();
			exit;
		}

		mysql_select_db("CS143", $databaseConnection);

        $input = $_GET['search'];
        $inputTerms = explode(' ', $input);		// tokenize by spaces
        if (count($inputTerms) == 0 || !$inputTerms) {
            echo "<p class='red'><strong>Ack! You didn't put anything into the search field!</p></strong>";
            exit(1);
        }

        // First, search by movies

        $queryByMovie = "SELECT *
        				FROM Movie 
        				WHERE title 
        				LIKE '%" . $inputTerms[0] . "%'";

        $numberOfInputTerms = count($inputTerms);

        for ($i = 1; $i < $numberOfInputTerms; $i++) {
            $queryByMovie .= "AND title LIKE '%" . $inputTerms[$i] . "%'";
        }

        // add semicolon
        $queryByMovie .= ";";

        //echo "$queryByMovie";

        $returnMovies = mysql_query($queryByMovie);
        
        if ($returnMovies != NULL) {
            while ($rowReturned = mysql_fetch_array($returnMovies)){
            	echo "<p class='center'><strong>Movie: </strong><a href='./showMovieInfo.php?id=" . $rowReturned['id'] . "'>" . "<i>" . $rowReturned['title'] . "</i>" . " (" . $rowReturned['year'] . ")</a></p>";
            }
        }

        else {
            echo "<p class='red center'><strong>Sorry. The movie search failed.</p></strong>";
        }

        // now actor, which is essentially the same code as Movie but with different table info

        if ($numberOfInputTerms == 1) {
        		    $queryByActor = "SELECT * 
	    				FROM Actor 
	    				WHERE first 
	    				LIKE '%" . $inputTerms[0] . "%' OR last LIKE '%" . $inputTerms[0] . "%'";
        }

	    else if ($numberOfInputTerms == 2) { 
	    	$queryByActor = "SELECT * 
	    				FROM Actor 
	    				WHERE first 
	    				LIKE '%" . $inputTerms[0] . "%' AND last LIKE '%" . $inputTerms[1] . "%'";
	    }

	    // for($j = 1; $j < $numberOfInputTerms; $j++) {
	    // 	//echo "entered this";
	    //     $queryByActor .= "OR first LIKE '%" . $inputTerms[$j] . "%' AND last LIKE '%" . $inputTerms[$j] . "%' " ;
	    // }
	    
	    // add a semicolon
	    $queryByActor .= ";";

	    $returnActors = mysql_query($queryByActor);

	    if ($returnActors != NULL) {
	        while($rowReturned = mysql_fetch_array($returnActors)){
	            echo "<p class='center'><strong>Actor: </strong><a href='./showActorInfo.php?id=" . $rowReturned['id'] . "'>" . "<i>" . $rowReturned['first'] . " " . $rowReturned['last'] . "</i>" . ", born on " . $rowReturned['dob'] . "</a></p>";
	        }
	    }
	    else {
	        echo "<p class='red center'><strong>No actors met your criteria. Sorry.</p></strong>";
	    }

	}
	else {
		echo "<p class='red center'><strong>Please enter a search term.</p></strong>";
	}
?>