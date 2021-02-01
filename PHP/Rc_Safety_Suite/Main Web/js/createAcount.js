$(document).ready( () => {

    //On user creation click
    $('#createBtn').click( () => {
        let data = {};
        data['action'] = 'AddUser';
        data['user'] = $('#addUsrInput').val();
        data['pass'] = $('#addPswInput').val();
    
        //Empty status container as ajax goes to fetsh new updated status
        $('#status-container').html("");
        
        //Send ajax request
        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

});
//////////////////////////////////////////////
//  function HandleStatus (data, response)
//  Args:
//          data        : ajax response data
//          response    : ajax response
//////////////////////////////////////////////
function HandleStatus(data, response)
{
    //Prompt status output
    $('#status-container').html("Page Status:<br>");

    //If user already exists (query returns -1)
    if (data['status'].includes('-1'))
        $('#status-container').append("User Already Exists.");
    
    //If new user
    else
    {
        //Would display 1 row inserted but will redirect before most people notice
        $('#status-container').append(data['status']);
        window.location.replace("./login.php");
    }
}

//////////////////////////////////////////////
//  function Fail (errorMessage)
//  Args:
//          errormessage : ajax response data
//////////////////////////////////////////////
function Fail(errorMessage)
{
    console.log(errorMessage);
}