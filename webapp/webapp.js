var myApp = angular.module('myApp', ['ngRoute','mobile-angular-ui','btford.socket-io','angularjs-gauge','ui.toggle']);

myApp.config(function($routeProvider) 
{
    $routeProvider.when('/', {
        templateUrl: 'home.html',
        controller: 'Home'
    });
});

myApp.factory('mySocket', function (socketFactory) 
{
	var myIoSocket = io.connect('/webapp');	//Tên namespace webapp

	mySocket = socketFactory({
		ioSocket: myIoSocket
	});
	return mySocket;
});

myApp.controller('Home', function($scope, mySocket) 
{
    $scope.CamBienMua = 0;
    $scope.relay_status = [false,false,false,false];
	$scope.lcd = ["",""];
	$scope.servoPosition = 0;
	$scope.nhietdo = -20;
	$scope.doam = 0;

	$scope.updateRain  = function() 
	{
		mySocket.emit("RAIN")
	}

	$scope.updateDHT11 = function()
	{
		mySocket.emit("DHT")
	}
	
	$scope.testRain = function(rainPosition)
	{
		var imgrain = document.getElementById('rainicon');
		var imgsun = document.getElementById('sunicon');

		$scope.CamBienMua = rainPosition;

		mySocket.emit("CamBienMua: ",$scope.CamBienMua)

		if ($scope.CamBienMua == 0) 
		{
			imgrain.style.display = 'none';
			imgsun.style.display="block";
		}
		else
		{
			imgsun.style.display="none";
			imgrain.style.display="block";
		}
	}
	
	//Cách gửi tham số 1: dùng biến toàn cục! $scope.<tên biến> là biến toàn cục
	$scope.changeLED = function() 
	{
		console.log("send LED ", $scope.leds_status)
		
		var json = 
		{
			"led": $scope.leds_status
		}
		mySocket.emit("LED", json)
	}

	$scope.changeRelay = function() 
	{
		console.log("send RELAY ", $scope.relay_status)
		
		var json = 
		{
			"relay": $scope.relay_status
		}
		mySocket.emit("RELAY", json)
	}
	
	//cập nhập lcd như một ông trùm 
	$scope.updateLCD = function() 
	{
		var json = 
		{
			"line": $scope.lcd
		}
		console.log("LCD_PRINT ", $scope.lcd)
		mySocket.emit("LCD_PRINT", json)
	}
	
	//Cách gửi tham số 2: dùng biến cục bộ: servoPosition. Biến này đươc truyền từ file home.html, dữ liệu đươc truyền vào đó chính là biến toàn cục $scope.servoPosition. Cách 2 này sẽ giúp bạn tái sử dụng hàm này vào mục đích khác, thay vì chỉ sử dụng cho việc bắt sự kiện như cách 1, xem ở Khu 4 để biết thêm ứng dụng!
	$scope.updateServo = function(servoPosition) 
	{
		var json = 
		{
			"degree": servoPosition
		}
		
		console.log("SEND SERVO", json) //debug chơi à
		mySocket.emit("SERVO", json)
	}
	
	//Khu 3 -- Nhận dữ liệu từ Arduno gửi lên (thông qua ESP8266 rồi socket server truyền tải!)
	//các sự kiện từ Arduino gửi lên (thông qua esp8266, thông qua server)
	mySocket.on('RAIN', function(json) 
	{
		$scope.CamBienMua = json.digital

		var imgrain = document.getElementById('rainicon');
		var imgsun = document.getElementById('sunicon');

		if ($scope.CamBienMua == 0) 
		{
			imgrain.style.display = 'none';
			imgsun.style.display="block";
		}
		else
		{
			imgsun.style.display="none";
			imgrain.style.display="block";
		}
	})

	mySocket.on('DHT',function(json) 
	{
		$scope.nhietdo = json.nhietdo
		$scope.doam = json.doam
	})

	// Khi nhận được lệnh LED_STATUS
	mySocket.on('LED_STATUS', function(json) 
	{
		//Nhận được thì in ra thôi hihi.
		console.log("recv LED", json)
		$scope.leds_status = json.data
	})
	
	
	//// Khu 4 -- Những dòng code sẽ được thực thi khi kết nối với Arduino (thông qua socket server)
	mySocket.on('connect', function() 
	{
		console.log("connected")
		mySocket.emit("RAIN") //Cập nhập trạng thái mưa
		mySocket.emit("DHT")
		$scope.updateServo(0) //Servo quay về góc 0 độ!. Dùng cách 2 
	})
		
});