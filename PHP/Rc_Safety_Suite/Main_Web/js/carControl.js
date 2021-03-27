// used as guide: -- https://www.cssscript.com/touch-joystick-controller/ 

//let carNum = 1; //to be determined from dropdown
let lastTime = 0;
let intendedAngle = 0;
let speedAsPercent = 0;
let joystick;
let restartSong = true;
$carID = null;
let audio = null;
let ajaxIntervalID;
let pullDataIntervalID;

class JoystickController
{
	// stickID: ID of HTML element (representing joystick) that will be dragged
	// maxDistance: maximum amount joystick can move in any direction
	// deadzone: joystick must move at least this amount from origin to register value change
	constructor( stickID, maxDistance, deadzone )
	{
		this.id = stickID;
		let stick = document.getElementById(stickID);

		// location from which drag begins, used to calculate offsets
		this.dragStart = null;

		// track touch identifier in case multiple joysticks present
		this.touchId = null;
		
		this.active = false;
		this.value = { x: 0, y: 0 }; 

		let self = this;

		function handleDown(event)
		{
		    self.active = true;

			// all drag movements are instantaneous
			stick.style.transition = '0s';

			// touch event fired before mouse event; prevent redundant mouse event from firing
			event.preventDefault();

		    if (event.changedTouches)
		    	self.dragStart = { x: event.changedTouches[0].clientX, y: event.changedTouches[0].clientY };
		    else
		    	self.dragStart = { x: event.clientX, y: event.clientY };

			// if this is a touch event, keep track of which one
		    if (event.changedTouches)
                self.touchId = event.changedTouches[0].identifier;                            
		}
		
		function handleMove(event) 
		{
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    // also handle multiple simultaneous touchmove events
		    let touchmoveId = null;
		    if (event.changedTouches)
		    {
		    	for (let i = 0; i < event.changedTouches.length; i++)
		    	{
		    		if (self.touchId == event.changedTouches[i].identifier)
		    		{
		    			touchmoveId = i;
		    			event.clientX = event.changedTouches[i].clientX;
		    			event.clientY = event.changedTouches[i].clientY;
		    		}
		    	}

		    	if (touchmoveId == null) return;
		    }

		    const xDiff = event.clientX - self.dragStart.x;
		    const yDiff = event.clientY - self.dragStart.y;
		    const angle = Math.atan2(yDiff, xDiff);
			const distance = Math.min(maxDistance, Math.hypot(xDiff, yDiff));
			const xPosition = distance * Math.cos(angle);
			const yPosition = distance * Math.sin(angle);

			// move stick image to new position
		    stick.style.transform = `translate3d(${xPosition}px, ${yPosition}px, 0px)`;

			// deadzone adjustment
			const distance2 = (distance < deadzone) ? 0 : maxDistance / (maxDistance - deadzone) * (distance - deadzone);
		    const xPosition2 = distance2 * Math.cos(angle);
			const yPosition2 = -1 * distance2 * Math.sin(angle);
		    const xPercent = parseFloat((xPosition2 / maxDistance).toFixed(4));
            const yPercent = parseFloat((yPosition2 / maxDistance).toFixed(4));

			//console.log("");
			//console.log(angle);

			//Offset so 0 is forward TODO

			intendedAngle = parseFloat((angle * (180 / Math.PI) + 90).toFixed(0));
			intendedAngle = intendedAngle > 180 ? intendedAngle - 360 : intendedAngle;
			speedAsPercent = (distance / maxDistance * 100).toFixed(0);
		    
            self.value = { x: (xPercent * 100).toFixed(0), y: (yPercent * 100).toFixed(0)};
		  }

		function handleUp(event) 
		{
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    if (event.changedTouches && self.touchId != event.changedTouches[0].identifier) return;

		    // transition the joystick position back to center
		    stick.style.transition = '.2s';
		    stick.style.transform = `translate3d(0px, 0px, 0px)`;

		    // reset everything
		    self.value = { x: 0, y: 0 };
		    self.touchId = null;
			self.active = false;
			
			
			//If lift put back to 0
			intendedAngle = 0;
			speedAsPercent = 0;
		}

		stick.addEventListener('mousedown', handleDown);
		stick.addEventListener('touchstart', handleDown);
		document.addEventListener('mousemove', handleMove, {passive: false});
		document.addEventListener('touchmove', handleMove, {passive: false});
		document.addEventListener('mouseup', handleUp);
		document.addEventListener('touchend', handleUp);
	}
}

$(document).ready ( () => {
	joyStick = new JoystickController("joyStick", 64, 8);	
	//joyStick = new JoystickController("joyStick", document.querySelector("#joyBase").clientWidth / 2, 8);

	$('#carControl').hide();

	$("#tcSlider").change ( () => {
		$("#tcSliderLbl").html("TC Level [" + $("#tcSlider").val() + "]");
	});

	$("#absSlider").change ( () => {
		$("#absSliderLbl").html("ABS Level [" + $("#absSlider").val() + "]");
	});

	$('#toIndexCarControl').click( () =>{
        window.location.replace("./index.php");
    });

	$('#hardCodedTesting').click( () =>{
		$('#chooseCar').show();
		$('#carControl').hide();

		clearInterval(ajaxIntervalID);
		clearInterval(pullDataIntervalID);
    });

	$('#hardcodeGoBtn').click ( () => {
		let angle = $("#hardCodeAngle").val();
		let speed = $("#hardCodeSpeed").val();
		FixedSendData(speed, angle);
	});

	$('#hardcodeStopBtn').click ( () => {

		document.getElementById("hardCodeAngle").setAttribute("value", 0);
		document.getElementById("hardCodeSpeed").setAttribute("value", 0);

		FixedSendData(0, 0);
	});


	$('#timerHardCode').click ( () => {
		BrakeTestHardCode();
	});



	GetCarCount();
	
	window.addEventListener('resize', SizeGauge);

	//audio = document.getElementById("roundAndRound");

	//Preload mp3
	//audio.volume = 0;

	//Need to update 
	//FillUserCars();
});

window.onload = (event) =>{

	// //Set up the speedo size
	// let gauge = document.getElementById("speedGauge");

	// var speedGaugeHeight = $("#speedGaugeDiv").css('height');
	// speedGaugeHeight = speedGaugeHeight.substring(0, speedGaugeHeight.length - 2);

	// var speedGaugeWidth = $("#speedGaugeDiv").css('width');
	// speedGaugeWidth = speedGaugeWidth.substring(0, speedGaugeWidth.length - 2);

	// gauge.setAttribute("data-height", `${speedGaugeHeight}`);
	// gauge.setAttribute("data-width", `${speedGaugeWidth}`);

};


function update()
{
	document.getElementById("xyCoords").innerText = "Joystick: " + JSON.stringify(joyStick.value);
}

function loop()
{
	requestAnimationFrame(loop);
	update();
	DoGaugeStuff();
}

function DoGaugeStuff()
{
	SizeGauge();

	let FL_RPM = (Number)($("#FL_RPM").html());
	let BL_RPM = (Number)($("#BL_RPM").html());
	let FR_RPM = (Number)($("#FR_RPM").html());
	let BR_RPM = (Number)($("#BR_RPM").html());

	let speed = (FL_RPM + BL_RPM + FR_RPM + BR_RPM) / 4 * 6.5 * 0.001885;
	document.getElementById("speedGauge").setAttribute("data-value", `${speed}`);
}

function GetCarCount()
{
	let data = {};
	data['action'] = "GetCarCount";
	AjaxRequest('./webservice.php', 'POST', data, 'json', SetUpItems, SetUpItems) // fail
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





    FillCarDrives($(select).val());       //to take select value
}

function SetUpItems (data, response)
{
	$('#selectCarNumber').html("");
	$('#selectCarBtn').html("");
	$('#addCarBtnDIV').html("");

	//Make user select car
	let selCar =  $('#selectCarNumber');
	$(selCar).html("");

	let select = document.createElement('select');
	$(select).prop({"name" : "carSelect", "id" : "whichCarSel"});

	for ($i = 1; $i <= data['data']; $i++)
	{
		let option = document.createElement('option');
		$(option).prop({"id" : "option" + $i, "value" : $i, "innerHTML" : "Car " + $i});
		$(select).append(option);
	}


	$(selCar).append(select);

	//set up button
	let selBtnDiv = $('#selectCarBtn');
	let selCarBtn = document.createElement("input");
	$(selCarBtn).prop({"type" : "button", "id" : "selCarBtn", "value" : "Control Car #" + $(select).val()});
	$(selBtnDiv).append(selCarBtn);

		
	//Add option to add another car...

	let addCarBtnDiv = $('#addCarBtnDIV');
	let addCarBtn = document.createElement("input");
	$(addCarBtn).prop({"type" : "button", "id" : "addCarBtn", "value" : "Add Car #" + (data['data'] + 1)});
	$(addCarBtnDiv).append(addCarBtn);

	//Option to delete a car
	let delCarBtnDiv = $('#addCarBtnDIV');			//change div TODO?
	let delCarBtn = document.createElement("input");
	$(delCarBtn).prop({"type" : "button", "id" : "addCarBtn", "value" : "Delete Car #" + $(select).val()});
	$(delCarBtnDiv).append(delCarBtn);

	//Add select changed event handler to update button
	$(select).change( () => {
		$(selCarBtn).prop("value", "Control Car #" + $(select).val());
		$(delCarBtn).prop("value", "Delete Car #" + $(select).val());
		$carID = $(select).val()
	});

	$carID = $(select).val()



	$(addCarBtn).click ( () => {
		let sendData = {};
		sendData['action'] = "addNewCar";
		sendData['carID'] = data['data'] + 1;

		AjaxRequest('./webservice.php', 'POST', sendData, 'json', GetCarCount, Fail)
	});

	$(delCarBtn).click ( () => {
		let sendData = {};
		sendData['action'] = "deleteCar";
		sendData['carID'] = $carID;

		AjaxRequest('./webservice.php', 'POST', sendData, 'json', GetCarCount, Fail)
	});

	$(selCarBtn).click ( () => {

		let data = {};
		data['action'] = "addNewCar";
		data['carID'] = data['data'];

		AjaxRequest('./webservice.php', 'POST', data, 'json', DisplayControls, Fail)

		let interval = 200;
		ajaxIntervalID = setInterval(AjaxData, interval);
		pullDataIntervalID = setInterval(ReceiveData, interval * 3);
		loop();

	
	});
}

function DisplayControls(data, response)
{
	$('#carControlBanner').html("Controlling car #" + $('#whichCarSel').val());

	$('#chooseCar').hide();
	$('#carControl').show();
	
	// window.dispatchEvent(new Event('resize'));
	SizeGauge();
}

function SizeGauge()
{
	//Set up the speedo size
	let gauge = document.getElementById("speedGauge");

	var speedGaugeHeight = $("#speedGaugeDiv").css('height');
	speedGaugeHeight = speedGaugeHeight.substring(0, speedGaugeHeight.length - 2);

	var speedGaugeWidth = $("#speedGaugeDiv").css('width');
	speedGaugeWidth = speedGaugeWidth.substring(0, speedGaugeWidth.length - 2);

	gauge.setAttribute("data-height", `${speedGaugeHeight}`);
	gauge.setAttribute("data-width", `${speedGaugeWidth}`);

	// gauge.setAttribute('width', speedGaugeWidth);
	// gauge.setAttribute('height', speedGaugeHeight);
}

//////////////////////////////////////////////
//  function HandleStatus (data, response)
//  Args:
//          data        : ajax response data
//          response    : ajax response
//////////////////////////////////////////////
function HandleStatus(data, response)
{
    $('#connectionStatus').html("GOOD AJAX REQUEST!");
}

//////////////////////////////////////////////
//  function Fail (errorMessage)
//  Args:
//          errorData : ajax response data
//			message   : message
//////////////////////////////////////////////
function Fail(errorData, message)
{
    $('#connectionStatus').html("BAD AJAX REQUEST!");
	console.log(message);
}

function CarSimReq(data, response)
{
	$("#XYFeedBackTest").html("Angle = ");
	$("#XYFeedBackTest").append(data['intendedAngle']);
	$("#XYFeedBackTest").append("<br>Speed as % = ");
	$("#XYFeedBackTest").append(data['intendedSpeed']);
	$("#XYFeedBackTest").append("<br>TimeStamp = ");
	$("#XYFeedBackTest").append(data['timeStamp']);	
}

//Happens every 250ms send data to webserver
function AjaxData()
{
	//Send Data
	let data = {};
    data['action'] = 'web_to_car_Data';
    data['carID'] = $('#whichCarSel').val();              
	data['intendedAngle'] = intendedAngle;
	data['intendedSpeed'] = speedAsPercent;
	data['brakeStrength'] = $("#brakeSlider").val();
	data['TC_Level'] = $("#tcSlider").val();
	data['ABS_Level'] = $("#absSlider").val();
	
            //Date.now() returns number of ms from January 1, 1970, parsed to keep track of 1 minute in ms
    //For unsigned 16bit int (0 - 65,535), and as little wrapping as little as possible
    //If mem space is required can easily be reduced further
    data['timeStamp'] = Date.now() % 60000;     //returns number of ms from January 1, 1970, good for timeout comparison
	//parsed to last 4 digits for data storage as its essentially acting as a timer

	AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)	
}

let testTimer;

function BrakeTestHardCode()
{
	let angle = $("#hardCodeAngle").val();
	let speed = $("#hardCodeSpeed").val();
	FixedSendData(speed, angle);
	testTimer = setInterval(Brake, $("#timerVal").val());
}

function Brake()
{
	FixedSendData(0, 0);
	clearInterval(testTimer);
}

function FixedSendData(speed, angle)
{	//Send Data
	let data = {};
    data['action'] = 'web_to_car_Data';
    data['carID'] = $('#whichCarSel').val();              
	data['intendedAngle'] = angle;
	data['intendedSpeed'] = speed;
	data['brakeStrength'] = $("#hardCodeBrakeStrength").val();
	data['TC_Level'] = $("#hardCodeBrakeStrength").val();
	data['ABS_Level'] = $("#hardCodeAbs").val();
	
            //Date.now() returns number of ms from January 1, 1970, parsed to keep track of 1 minute in ms
    //For unsigned 16bit int (0 - 65,535), and as little wrapping as little as possible
    //If mem space is required can easily be reduced further
    data['timeStamp'] = Date.now() % 60000;     //returns number of ms from January 1, 1970, good for timeout comparison
	//parsed to last 4 digits for data storage as its essentially acting as a timer

	AjaxRequest('./webservice.php', 'POST', data, 'json', Useless, Fail)	
}

function Useless()
{
	console.log("Hardcoded command sent successfully");
}

function ReceiveData()
{
	let receiveCarData = {};
	receiveCarData['fromGUI'] = "yeppers";
	receiveCarData['carID'] = $carID;

	AjaxRequest('./webservice.php', 'POST', receiveCarData, 'json', UpdateSelfData, Fail);

	let fetchSentData = {};
	fetchSentData['action'] = "GetCarDrives";
	fetchSentData['fromGUI'] = "yeppers";
	fetchSentData['carID'] = $carID;

	AjaxRequest('./webservice.php', 'POST', fetchSentData, 'json', UpdateCarData, Fail);
}

function UpdateCarData(data, response)
{
	$("#FL_RPM").html(data['data'][0]['FL_RPM']);
	$("#FR_RPM").html(data['data'][0]['FR_RPM']);
	$("#BL_RPM").html(data['data'][0]['BL_RPM']);
	$("#BR_RPM").html(data['data'][0]['BR_RPM']);

	if (data['data'][0]['TC_ACTIVE'] == 1)
		$("#TC_ACTIVE").html("TC Active");
	
	else
		$("#TC_ACTIVE").html("TC Not Active");

	if (data['data'][0]['ABS_ACTIVE'] == 1)
		$("#ABS_ACTIVE").html("ABS Active");

	else
		$("#ABS_ACTIVE").html("ABS Not Active");

	if (data['data'][0]['Burnout_In_Progress'] == 1)
		$("#Burnout_Active").html("Wee spinny spinny right");

	else if (data['data'][0]['Burnout_In_Progress'] == 2)
		$("#Burnout_Active").html("Wee spinny spinny left");

	else
		$("#Burnout_Active").html("No Spinny :(");

	if (data['data'][0]['Burnout_In_Progress'] === "1" || data['data'][0]['Burnout_In_Progress'] === "2" && restartSong)
	{
		// audio.volume = 1;
		// burnoutAudio.play();
		restartSong = false;

		document.getElementById("roundAndRound").play();
	}
	else
	{
		restartSong = true;
		document.getElementById("roundAndRound").pause();
	}
}

function UpdateSelfData(data, response)
{
	$("#XYFeedBackTest").html(`Intended Angle: ${data['a']} || Intended Speed: ${data['s']}`);
}

// function FetchData()
// {
// 	let data = {};
// 	data['action'] = 'GrabWebToCar';

// 	AjaxRequest('./webservice.php', 'POST', data, 'json', CarSimReq, Fail);
// }

function sleep(ms){
	return new Promise( resolver => setTimeout(resolver, ms));
};