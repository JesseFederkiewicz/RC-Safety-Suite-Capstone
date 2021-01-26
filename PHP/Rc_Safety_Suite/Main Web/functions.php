<?php
require_once "./inc/db.php";
// session_start();

function Validate($params)
{
    global $mysqli;

    $username = $mysqli->real_escape_string($params['user']);

    $query = "select *";
    $query .= " from users ";
    $query .= "where username like '{$username}'";

    //Init to no
    $params["response"] = "User {$params['user']} Not Authorized<br>with password: {$params['password']}";

    $params['status'] = false;  //init to no

    //If successful
    if ($rowData = mysqliQuery($query))
    {
        if ($rowData->num_rows == 1)
        {
            $row = $rowData->fetch_assoc();

            $password = $mysqli->real_escape_string($params["password"]);

            if (password_verify($password, $row['password']))
            {
                $params["response"] = "{$username} logged in";  

                //Save userID
                $_SESSION['userID'] = $row['userID'];
        
                $params['status'] = true; // authention outcome, true = success
            }
            else
            {
                $params["response"] = "Wrong Password";
            }
        }
        else if ($rowData->num_rows < 1)
        {
            $params["response"] = "No User Found!";
        }
        else
        {
            $params["response"] = "Uhm, admin??? Too many users found pAnIC!1!!1!";
        }
    }

    return $params;
}
?>