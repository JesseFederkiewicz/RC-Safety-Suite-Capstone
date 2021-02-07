<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="./js/carControl.js" type="text/javascript"></script>
    <script src = "./js/ajax.js"></script>
</head>
<body>
<div style="border: 1px solid red; width: 128px; position: absolute; left:10px; top:150px;">
	<img src="images/joystick-base.png" id = 'joyBase'/>
	<div id="joyStick" style="position: absolute; left:32px; top:32px;">
	<img src="images/joystick-red.png"/>		
	</div>
    <hr>
    <div id="status1" style="color: red;">Joystick</div>

    <div id = connectionStatus>
    </div>

    <div id = XYMAX>
    
    </div>
    <div id = XYFeedBackTest>
    HERE
    </div>

</div>
</body>
</html>


