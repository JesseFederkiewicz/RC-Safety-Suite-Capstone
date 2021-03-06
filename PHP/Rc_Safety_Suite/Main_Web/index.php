<?php
require_once "functions.php"; // add start_session and include fnc Validate()

// Redirect if not authenticated
if( !isset($_SESSION["user"])) // not authenticated, does exist
{
  // not logged in, turf him to login.php
  header("location:login.php");// send user to login.php
  die(); // in a hole
}
?>
<!DOCTYPE html>
<html lang="en">
  <head>
  <link rel="stylesheet" href="style.css">
    <title>Index Page</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="./js/ajax.js" type="text/javascript"></script>
  </head>
  <body>
    <div class='hdr'>
      <h1 class='font-effect-3d text-center'>
        <!-- show user - if possible here -->
        <?php echo "User: {$_SESSION['user']}"; ?>
      </h1>
    </div>
    <!-- Form Posts to login -->
    <form action="login.php" method="post" id = "indexSite">
        <div id = "tagSettings" class = text-right>
        <a href = "./settings.php" class = "text-center">Admin Settings</a><br>
        <?php
          echo "Tag {$_SESSION['user']}";
        ?>
        </div>
        <div id = "messages">
        <a href = "./pastDrives.php">Past Drives</a><br><a href = "./carControl.php">Car Control</a>
        </div>

        <input type="submit" name="submit" value="Logout" id = logoutBtn>
    </form>

    <div class=text-center id = 'status-container'>
    <?php
        echo "Page Status: {$responseParams['response']}<br>";
    ?>
    </div>
    <div class="text-center">
    <br>&copy Copyright 2020 by Jesse Federkiewicz & Timothy Hachey<br/>
      <script>document.write('Last Modified:' + document.lastModified);</script>
    </div>
  </body>
</html>