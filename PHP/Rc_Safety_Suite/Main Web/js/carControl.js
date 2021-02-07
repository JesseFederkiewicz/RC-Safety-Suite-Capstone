// used as guide: -- https://www.cssscript.com/touch-joystick-controller/ 

//let carNum = 1; //to be determined from dropdown
let lastTime = 0;
let intendedAngle = 0;
let speedAsPercent = 0;

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
  
	let joyStick = new JoystickController("joyStick", document.querySelector("#joyBase").clientWidth / 2, 8);	//joystick now resizes based on how big base image is
	

    function update()
    {
        document.getElementById("status1").innerText = "Joystick: " + JSON.stringify(joyStick.value);
    }

    function loop()
    {
        requestAnimationFrame(loop);
		update();
    }

	loop();
	let interval = 50;
	setInterval(SendData, interval);   
	//setInterval(FetchData, interval * .8);
});

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
//          errormessage : ajax response data
//////////////////////////////////////////////
function Fail(errorMessage)
{
    $('#connectionStatus').html("BAD AJAX REQUEST!");
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
function SendData()
{
	let data = {};
    data['action'] = 'web_to_car_Data';
    data['carID'] = 1;              
	data['intendedAngle'] = intendedAngle;
	data['intendedSpeed'] = speedAsPercent;
	
            //Date.now() returns number of ms from January 1, 1970, parsed to keep track of 1 minute in ms
    //For unsigned 16bit int (0 - 65,535), and as little wrapping as little as possible
    //If mem space is required can easily be reduced further
    data['timeStamp'] = Date.now() % 60000;     //returns number of ms from January 1, 1970, good for timeout comparison
	//parsed to last 4 digits for data storage as its essentially acting as a timer

	AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
	
}

function FetchData()
{
	let data = {};
	data['action'] = 'GrabWebToCar';
	
	AjaxRequest('./webservice.php', 'POST', data, 'json', CarSimReq, Fail);
}

function sleep(ms){
	return new Promise( resolver => setTimeout(resolver, ms));
};