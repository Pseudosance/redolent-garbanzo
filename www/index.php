<?php>

<?>

<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<div class="jumbotron">
  <div class="container">
    <h1> The Movie Lover's Ultimate Database </h1>
    <p class="center"> <i>For all your movie needs.</i> </p>
  </div>
</div>

<div class="center">
	<strong><p>Search for your favorite actor or movie: </p></strong>
	<form method="GET" action="./search.php">		<!-- need to implement search.php -->
		<input type="text" name="query">
		<input class="btn btn-primary btn-sm" type="submit" value="search">
	</form>

	 <p>
	 	<a class="btn btn-success btn-sm" href="./addActorOrDirector.php" role="button">Add actor or director</a>
	 	<a class="btn btn-info btn-sm" href="./addMovieInfo.php" role="button">Add movie information</a>
	 	<a class="btn btn-warning btn-sm" href="./addComments.php" role="button">Add comments</a>
	 	<a class="btn btn-danger btn-sm" href="./addActorToMovie.php" role="button">Add actor-to-movie relation</a>
	 	<a class="btn btn-primary btn-sm" href="./addDirectorToMovie.php" role="button">Add director-to-movie relation</a>
	 </p>

	 <p>
	 	<a class="btn btn-success btn-sm" href="./showActorInfo.php" role="button">Show actor information</a>
	 	<a class="btn btn-primary btn-sm" href="./showMovieInfo.php" role="button">Show movie information</a>
	 </p>
</div>

<div class="footer navbar-fixed-bottom"> <strong> CS 118 Project 1C by Mitchell Binning and Tremaine Eto </strong> </div>

</body>
</html>
