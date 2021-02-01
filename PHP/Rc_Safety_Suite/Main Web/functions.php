<?php
require_once "./inc/db.php";
// session_start();

//////////////////////////////////////////////
//  function Validate($params)
//  Args:
//          params : user input and resp data
//////////////////////////////////////////////
function Validate($params)
{
    global $mysqli;

    //Filter data, param is already lowercase
    $username = $mysqli->real_escape_string($params['user']);

    $query = "select *";
    $query .= " from users ";
    $query .= "where username like '{$username}'";

    //Init to no
    $params["response"] = "User {$params['user']} Not Authorized<br>with password: {$params['password']}";
    $params['status'] = false;  

    //If successful
    if ($rowData = mysqliQuery($query))
    {
        //If one row inserted
        if ($rowData->num_rows == 1)
        {
            //Fetsh row data
            $row = $rowData->fetch_assoc();

            //Clean up and assign psw
            $password = $mysqli->real_escape_string($params["password"]);

            //Check to see if password matches user
            if (password_verify($password, $row['password']))
            {
                //Update response
                $params["response"] = "{$username} logged in";  

                //Save userID
                $_SESSION['userID'] = $row['userID'];
        
                //Outcome is successful (true)
                $params['status'] = true; 
            }
            //Else bad password
            else
                $params["response"] = "Wrong Password";
            
        }

        //If no user
        else if ($rowData->num_rows < 1)
            $params["response"] = "No User Found!";
        
        //If more than one user, should be impossible with insertion checks on create page
        else
            $params["response"] = "Uhm, admin??? Too many users found pAnIC!1!!1!";
        
    }

    return $params;
}
?>