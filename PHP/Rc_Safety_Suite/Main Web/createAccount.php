<?php

    require_once "./functions.php";

    $responseParams['response'] = "Awaiting Account Creation...";
    //Form Processing//

    // //logout processing
    // if (isset($_POST['submit']) && $_POST['submit'] == 'logout') 
    // {
    //     session_unset();
    //     session_destroy();
    // }   //done logout let login.php show

    // //login processing
    // //proper login request
    // else if (isset($_POST['submit']) && $_POST['submit'] == 'login'
    // && isset($_POST['user']) && strlen($_POST['user']) > 0  &&
    // isset($_POST['password']) && strlen($_POST['password']) > 0) 
    // {
    //     //Populate Validate() params, then invoke 
    //     $loginParams = array();

    //     $loginParams['user'] = strip_tags($_POST['user']);
    //     $loginParams['password'] = strip_tags($_POST['password']);

    //     $responseParams = Validate($loginParams);

    //     //Successful login?
    //     if ($responseParams['status'])
    //     {
    //         //Set the session
    //         $_SESSION['user'] = $loginParams['user'];
    //         $_SESSION['valid'] = true;
            
    //         //Redirect to main page
    //         header("location:index.php");

    //         die();
    //     }

    // }   //done login let index.php show
?> 

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="style.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ica04 - login</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src = "./js/createAcount.js"></script>
    <script src = "./js/ajax.js"></script>
</head>
<body>
    <div class = hdr>
        <h1 class = 'font-effect-3d text-center'>Create Account</h1>
    </div>


        <!-- <form action="login.php" method="post" id = "loginSite"> -->
        <div id = "loginSite">
            <div id = addUsrLbl class = text-right>UserName : </div><input type = "text" name="user" placeholder="Username" id = addUsrInput>
            <div id = addPswLbl class = text-right>Password : </div><input type = "password" name="password" placeholder="Password" id = addPswInput>

            <!-- 2 submits (login and logout buttons) -->
            
            <!-- <div id = btnDiv><a href = "./login.php"><input type = "submit" name = "submit" value = "Create Account" id = createBtn></a></div> -->
            <div id = btnDiv><input type = "submit" name = "submit" value = "Create Account" id = createBtn></div>
            <div id = AccCreateOrLoginDiv>Have an account? <br> <a href = "./login.php">Back to login.</a></div>
            
            <!-- <input type = "submit" name = "submit" value = "logout"> -->
        </div>
        <!-- </form> -->


    <div class=text-center id = 'status-container'>
    <?php
        echo "Page Status: {$responseParams['response']}<br>";
    ?>
    </div>

    <div class = "text-center footer">
            &copy;2020 by Jesse Federkiewicz<br><script>document.write('Last Modified: ' + document.lastModified)</script>
    </div>
</body>
</html>