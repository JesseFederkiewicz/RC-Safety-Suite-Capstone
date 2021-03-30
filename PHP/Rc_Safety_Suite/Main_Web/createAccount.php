<?php
    require_once "./functions.php";

    //Default message
    $responseParams['response'] = "Awaiting Account Creation...";
?> 

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="style.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Create Account</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src = "./js/createAccount.js"></script>
    <script src = "./js/ajax.js"></script>
</head>
<body>
    <div class = hdr>
        <h1 class = 'font-effect-3d text-center'>Create Account</h1>
    </div>
        <!-- Site Div -->
        <div id = "loginSite">
            <div id = addUsrLbl class = text-right>UserName : </div><input type = "text" name="user" placeholder="Username" id = addUsrInput>
            <div id = addPswLbl class = text-right>Password : </div><input type = "password" name="password" placeholder="Password" id = addPswInput>            
            <div id = btnDiv><input type = "submit" name = "submit" value = "Create Account" id = createBtn></div>
            <div id = AccCreateOrLoginDiv>Have an account? <br> <a href = "./login.php">Back to login.</a></div>
        </div>

    <div class=text-center id = 'status-container'>
    <?php
        echo "Page Status: {$responseParams['response']}<br>";
    ?>
    </div>

    <div class = "text-center footer">
    <br>&copy; Copyright 2020 by Jesse Federkiewicz & Timothy Hachey<br><script>document.write('Last Modified: ' + document.lastModified)</script>
    </div>
</body>
</html>