$(document).ready( () => {
    GetUsers();
});

function GetUsers()
{
    console.log("GET");
    let data = {};
    data['action'] = 'GetUsers';

    AjaxRequest('./webservice.php', 'GET', data, 'json', ShowUsers, Fail);
}

function ShowUsers(data, response)
{
    for (let i = 0; i < data['data'].length; i++)
    {
        let tr = document.createElement('tr');

        let emptyTD = document.createElement('td');
        $(tr).append(emptyTD);

        let usrTD = document.createElement('td');
        $(usrTD).append(data['data'][i]['userID']);
        $(tr).append(usrTD);

        let userNameTD = document.createElement('td');
        $(userNameTD).append(data['data'][i]['username']);
        $(tr).append(userNameTD);

        let encryptTD = document.createElement('td');
        $(encryptTD).append(data['data'][i]['password']);
        $(tr).append(encryptTD);
        
        $('#tableDiv').append(tr);
    }

    $('#status-container').html(`Retrieved: ${data['status']} records`);
}

function Fail(errorMessage)
{
    console.log(errorMessage);
}
