<?php
    require_once "functions.php"; // add start_session and include fnc Validate()
    
    // Redirect if not authenticated
    if( !isset($_SESSION["user"])) // not authenticated, does exist
    {
        error_log($_SESSION["user"]);
        error_log($_SESSION["userID"]);
        // not logged in, turf him to login.php
        header("location:login.php");// send user to login.php
        die(); // in a hole
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="style.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Past Drives</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="./js/ajax.js" type="text/javascript"></script>
    <script src="./js/pastDrives12.js" type="text/javascript"></script>
</head>
<body>
    <div id = "PastDrivesSite">
        <div class = hdr id = "PastDrivesHeader">
            <h1 class = 'font-effect-3d text-center'>Past Drives</h1>
        </div>

        <div id = "pastDriveContent">
            <div id = "selectCarAndDrive">
                <select id = "yourCarNum"> </select>
                <select id = "yourDriveDate"> </select>
                <form action="login.php" method="post">
                    <input type="submit" name="submit" value="Logout" id = pastDriveLogoutBtn>
                </form>
                <input id = "loadDriveBtn" type = "button" value="Load Drive">
                <input id = "deleteCarDrive" type = "button" value="Delete Drive #X Data">
                <input id = "deleteCarData" type = "button" value="Delete Car #X Data">
                <input id = "toIndexBtn" type = "button" value="To Index">
            </div>

            <div id = "tableDiv">
                <table id = driveStatsRaw>
                    <tr>
                        <th>FL RPM</th>
                        <th>BL RPM</th>
                        <th>FR RPM</th>
                        <th>BR RPM</th>
                        <th>TC Active</th>
                        <th>ABS Active</th>
                        <th>Burnout Active</th>
                        <th>Motion Counter</th>
                        <th>Time Stamp</th>
                    </tr>
                    <tbody id = driveStatsRawBody>
                    </tbody>
                </table>
            </div>

            <!-- left diagram -->
            <div id = "leftDiagram">

            </div>


            <!-- right diagram -->
            <div id = "rightDiagram">

            </div>

        </div>

        <div class = "text-center footer" id = "PastDrivesFooter">
        <br> &copy; Copyright 2020 by Jesse Federkiewicz & Timothy Hachey<br><script>document.write('Last Modified: ' + document.lastModified)</script>
        </div>
    </div>
</body>
</html>