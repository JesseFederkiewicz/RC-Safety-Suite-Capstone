let _carDriveBins = [];

$(document).ready( () => {

    $("#driveStatsRaw").hide();

    // //On user creation click
    // $('#createBtn').click( () => {
    //     let data = {};
    //     data['action'] = 'AddUser';
    //     data['user'] = $('#addUsrInput').val();
    //     data['pass'] = $('#addPswInput').val();
    
    //     //Empty status container as ajax goes to fetsh new updated status
    //     $('#status-container').html("");
        
    //     //Send ajax request
    //     AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    // });
    
    $('#loadDriveBtn').click( () =>{
        _carDriveBins.forEach( e => {
            if (e[0]["timeEntry"] != null && ($("#yourDriveDate").val()).includes(e[0]["timeEntry"]))
            {
                FillRawTableDataForDrive(e);
                return;
            }
        });
    });

    $('#toIndexBtn').click( () =>{
        window.location.replace("./index.php");
    });

    $('#deleteCarData').click( () =>{
        let data = {};
        data['action'] = 'deleteCarDrives';
        data['carID'] = $('#yourCarNum').val();
            
        //Send ajax request
        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

    $('#deleteCarDrive').click( () =>{

        _carDriveBins.forEach( e => {
            if (e[0]["timeEntry"] != null && ($("#yourDriveDate").val()).includes(e[0]["timeEntry"]))
            {
                let data = {};
                data['action'] = 'deleteDrive';
                data['driveData'] = e;
                            
                //Send ajax request
                AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)

                return;
            }
        });
    });

    FillUserCars();

    $('#yourCarNum').change( () => {
        FillCarDrives($('#yourCarNum').val());
    });
});

function FillRawTableDataForDrive(driveData)
{
    let table = $("#driveStatsRawBody");
    $("#driveStatsRaw").show();
    
    $(table).html("");
    
    driveData.forEach( e => {

        if (e["timeEntry"] == null)
            return;
            
        let tr = document.createElement("tr");
        
        let flRpmTd = document.createElement("td");
        $(flRpmTd).html(e["FL_RPM"]);
        $(tr).append(flRpmTd);

        let blRpmTd = document.createElement("td");
        $(blRpmTd).html(e["BL_RPM"]);
        $(tr).append(blRpmTd);

        let frRpmTd = document.createElement("td");
        $(frRpmTd).html(e["FR_RPM"]);
        $(tr).append(frRpmTd);

        let brRpmTd = document.createElement("td");
        $(brRpmTd).html(e["BR_RPM"]);
        $(tr).append(brRpmTd);

        let tcActiveTd = document.createElement("td");
        $(tcActiveTd).html(e["TC_ACTIVE"]);
        $(tr).append(tcActiveTd);

        let absActiveTd = document.createElement("td");
        $(absActiveTd).html(e["ABS_ACTIVE"]);
        $(tr).append(absActiveTd);

        let burnoutTd = document.createElement("td");
        $(burnoutTd).html(e["Burnout_In_Progress"]);
        $(tr).append(burnoutTd);

        let motionCountTd = document.createElement("td");
        $(motionCountTd).html(e["Ground_Speed_Count"]);
        $(tr).append(motionCountTd);

        let timeEntryTd = document.createElement("td");
        $(timeEntryTd).html(e["timeEntry"]);
        $(tr).append(timeEntryTd);

        $(table).append(tr);
    });
}

function FillUserCars()
{
    let data = {};
    data['action'] = 'GetUserCars';

    //Send ajax request
    AjaxRequest('./webservice.php', 'POST', data, 'json', FillCarsSelect, Fail)
}

function FillCarsSelect(data, response)
{
    let select = $('#yourCarNum');
    (select).html("");

    let isFirst = true;

    data['data'].forEach(e => {
        let option = document.createElement("option");

        $(option).prop({"value" : e['carID'], "innerHTML" : "Car " + e['carID']})

        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        $(select).append(option);
    });


    if (data['status'] == "GetUserCars: 0 cars retreived")
    {
        let option = document.createElement("option");

        $(option).prop({"value" : "", "innerHTML" : "No Cars Found"})

        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        $(select).append(option);
    }



    FillCarDrives($(select).val());       //to take select value
}

function FillCarDrives(carID)
{
    let data = {};
    data['action'] = 'GetCarDrives';
    data['carID'] = carID;

    //Send ajax request
    AjaxRequest('./webservice.php', 'POST', data, 'json', FillCarDrivesSelect, Fail)
}

function FillCarDrivesSelect(data, response)
{
    let select = $('#yourDriveDate');
    (select).html("");
    let isFirst = true;
    let tempBin = [];

    if (data['status'] == "GetUserCars: 0 entries retreived")
    {
        let option = document.createElement("option");

        $(option).prop({"value" : "", "innerHTML" : "No Entries for the vehicle"})

        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        $(select).append(option);

        return;
    }

    var lastDate;// = data["data"][0]["timeEntry"];
    var curDate;// = data["data"][0]["timeEntry"];

    data['data'].forEach(e => {

        if (!isFirst)
            lastDate = curDate;

        curDate = new Date(e["timeEntry"]);
         
        if (!isFirst)
        {           
            //See if entries are more than a minute apart, if they are bin it.
            if (curDate - lastDate > 60000)
            {

                _carDriveBins.push(tempBin);
                tempBin = [];
                isFirst = true;
            }
            else
                tempBin.push(e);
        }

        else
        {
            isFirst = false;
            tempBin.push(e);
        }

        //_carDriveBins.push();

        
        // let option = document.createElement("option");

        // $(option).prop({"value" : e['timeEntry'], "innerHTML" : "StartTime: " + e['timeEntry']})

        // if (isFirst)
        // {
        //     $(option).prop("selected", true);
        //     isFirst = false;
        // }

        // $(select).append(option);
    });

    _carDriveBins.push(tempBin);

    _carDriveBins.forEach(e => {        
        let option = document.createElement("option");

        $(option).prop({"value" : e['timeEntry'], "innerHTML" : "StartTime: " + e[0]['timeEntry']})

        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        $(select).append(option);
    });
}

function Fail(data, message)
{
    console.log(message);

    //Update selects
    FillUserCars();
}

function HandleStatus(data, response)
{
    console.log(response);

    //Update selects
    FillUserCars();
}
