<?php
// // do requires
// // ?? function/session stuff
require_once "./inc/db.php";
require_once "./functions.php";
// // if we make it here.. connection was good

$gblData = array();   //gbl array?
$status = "Failed Ajax Call";

//////////////////////////////////////////////
//  function Done()
//  package up data
//////////////////////////////////////////////
function Done()
{
    global $gblData, $status;

    $jsonRep['data'] = $gblData;
    $jsonRep['status'] = $status;
    echo json_encode($jsonRep);
    die();
}

//$allUserData = $allUserData->fetsh_assoc();
if (isset($_POST['action']) && $_POST['action'] == 'AddUser')
{
    global $status;

    //Lowercase the usn
    $userName = strtolower($mysqli->real_escape_string($_POST['user']));

    //Get real password and hash it (sql protection)
    $password = password_hash($mysqli->real_escape_string($_POST['pass']), PASSWORD_DEFAULT);
    
    //Insert query and row response
    $query = mysqliNonQuery("INSERT INTO users (username, password)
                VALUES (\"$userName\",\"$password\")");

    $status = "$query : records inserted";

    //Pack up data
    Done();
}

if (isset($_GET['action']) && $_GET['action'] == 'GetUsers')
{
    //Get all users
    $allUserData = mysqliQuery("select * from users");

    //Check if query was good
    if ($allUserData)
    {
        //Foreach through all users and add to gblArr
        foreach ($allUserData as $key => $value)
            array_push($gblData, $value);
        

        //Get rows in table
        $status = $allUserData->num_rows;

        //Pack up data
        Done();
    }

    //Bad query
    else
    {
        error_log("GET Query Failed");
        $status = "GET Query Failed";
    }
}



//Delete user
if (isset($_POST['action']) && $_POST['action'] == 'DeleteUser')
{
    global $status;
    
    //Filter userId to delete
    $userID = $mysqli->real_escape_string($_POST['userID']);

    //Dont let them delete themselves
    if ($_SESSION['userID'] != $userID)
    {
        //Deletion query
        $query = mysqliNonQuery("DELETE from users where userID like \"$userID\"");
        $status = "$query : records deleted";
    }
    
    //Dont delete yourself
    else
        $status = "Don't delete yourself :( -facebook";

    //Pack up
    Done();
}

Done();
