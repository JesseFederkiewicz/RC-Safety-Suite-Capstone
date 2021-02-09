// 
// 
// 

#include "webservice.h"
#include <Wifi.h>
#include <BridgeHttpClient.h>
#include <Bridge.h>
#include <HardwareSerial.h>
#include <esp_http_client.h>
#include <WiFi.h>

void Main()
{
	char* jessessidHOT = "Unhackable II";
	const char* jessepasswordHOT = "plsdontguess";
	char* jessessid = "Cappy";
	const char* jessepassword = "ThisIs@nAdequateP@ss123";
	char* timssid = "hachey wifi";
	const char* timpassword = "38hachey";
	const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* server = "thor.net.nait.ca";

	Serial.begin(115200);
	Serial.println("Serial has begun");
	//Serial.println("Serial Has begun");
	WiFi.begin(jessessid, jessepassword);

	while (WiFi.status() != WL_CONNECTED) {
	}


	Serial.println("Wifi connected");
	esp_http_client_config_t config;
	config.url = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php?action=GrabWebToCar&carID=2";
	//config.event_handler = _http_event_handle;
	
	Serial.println("Config set up");

	esp_http_client_handle_t client = esp_http_client_init(&config);

	Serial.println("Client assigned");

	//esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
	////esp_err_t err = esp_http_client_perform(client);
	//Serial.println("Header assigned");


	////if (err == ESP_OK)
	////{
	////	esp_http_client_set_url(client, "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php");
	////	
	////}

	//esp_http_client_set_method(client, HTTP_METHOD_GET);
	//Serial.println("GET SET");

	//esp_err_t err = esp_http_client_open(client, 0);

	//Serial.println("Client opened");
	//
	//if (err == ESP_OK)
	//{
	//	Serial.println("ESP OK");
	//	char buff[50] = {};

	//	int content_length = esp_http_client_fetch_headers(client);
	//	Serial.println("Client About to read...");
	//	int dataLength = esp_http_client_read(client, buff, 50);

	//	Serial.println("dataLength");
	//	Serial.println(dataLength);
	//	Serial.println("output");
	//	Serial.println(buff);
	//	//int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);

	//	//if (dataLength >= 0) {
	//	//		esp_http_client_get_status_code(client),
	//	//		esp_http_client_get_content_length(client);
	//	//	ESP_LOG_BUFFER_HEX(TAG, output_buffer, strlen(output_buffer));
	//	//}
	//}


	////

	////POST Request
	//	const char* post_data = "{\"field1\":\"value1\"}";
	//esp_http_client_set_url(client, "http://httpbin.org/post");
	//esp_http_client_set_method(client, HTTP_METHOD_POST);
	//esp_http_client_set_header(client, "Content-Type", "application/json");
	//err = esp_http_client_open(client, strlen(post_data));
	//if (err != ESP_OK) {
	//	ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
	//}
	//else {
	//	int wlen = esp_http_client_write(client, post_data, strlen(post_data));
	//	if (wlen < 0) {
	//		ESP_LOGE(TAG, "Write failed");
	//	}
	//	int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
	//	if (data_read >= 0) {
	//		ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
	//			esp_http_client_get_status_code(client),
	//			esp_http_client_get_content_length(client));
	//		ESP_LOG_BUFFER_HEX(TAG, output_buffer, strlen(output_buffer));
	//	}
	//	else {
	//		ESP_LOGE(TAG, "Failed to read response");
	//	}
	//}
	//esp_http_client_cleanup(client);
//}
}

//BridgeHttpClient client;

//void Main()
//{
//	Serial.begin(115200);
//	Serial.println("Serial has begun"); 
//
//	Serial.println("b4 bridge begin");
//
//	Bridge.begin(9600);
//
//	Serial.println("Bridge started");
//
//	client.enableInsecure();
//
//	client.addHeader("Content-Type: application/x-www-form-urlencoded");
//
//	Serial.println("Before POST");
//
//
//	for (;;)
//	{
//		client.post("https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php?action=GrabWebToCar&carID=2", "action=GrabWebToCar&carID=2");
//
//
//		Serial.println("After POST");
//
//		String response;
//
//		while (client.available() > 0)
//		{
//			char c = client.read();
//			response += c;
//		}
//
//		Serial.println("After while");
//
//		Serial.println(response);
//		
//	}
//	//HttpClient http;
//
//	////GS_ID = "AKfycby1-hJ1aWN_gQ7QSF_AcjhdfhlkdfljLE8RBilnC0ph4"; // Replace by your GAS service id
//
//
//	//String url = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
//	//http.begin(url); //Specify the URL and certificate
//
//	//for (;;)
//	//{
//
//	//	http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//	//	int httpCode = http.POST("action=GrabWebToCar&carID=2");
//
//
//	//	if (httpCode > 0) {
//	//		// HTTP header has been send and Server response header has been handled
//	//		Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//
//	//		// file found at server
//	//		if (httpCode == HTTP_CODE_OK) { //
//
//	//			/*Stream way
//	//			// get lenght of document (is -1 when Server sends no Content-Length header)
//	//			int len = http.getSize();
//
//	//			Serial.println(len);
//
//	//			// create buffer for read
//	//			uint8_t buff[128] = { 0 };
//
//	//			// get tcp stream
//	//			WiFiClient* stream = http.getStreamPtr();
//
//
//
//	//			// read all data from server
//	//			while (http.connected() && (len > 0 || len == -1)) {
//	//				// get available data size
//	//				size_t size = stream->available();
//
//	//				if (size) {
//	//					// read up to 128 byte
//	//					int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
//
//	//					// write it to Serial
//	//					Serial.write(buff, c);
//
//	//					if (len > 0) {
//	//						len -= c;
//	//					}
//	//				}
//	//			}
//
//	//			Serial.println();
//	//			Serial.print("End of loop.\n");
//
//	//			*/
//
//	//			//Load response data
//	//			String payload = http.getString();
//	//			//String payload = http->getString();
//	//			Serial.println(payload);
//
//	//			//Parse response into our boy jason
//	//			JSONVar jason = JSON.parse(payload);
//
//	//			//Get data if timestamp has changed
//	//			if (_timeStamp != atoi(jason["timeStamp"]) && atoi(jason["timeStamp"]) != 0)
//	//			{
//	//				_intendedAngle = atoi(jason["intendedAngle"]);
//	//				_intendedSpeedPercent = atoi(jason["intendedSpeed"]);
//	//				_timeStamp = atoi(jason["timeStamp"]);
//
//	//				//likely want to count missed pings here
//	//			}
//
//
//
//	//		}
//	//	}
//	//	else {
//	//		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//	//	}
//
//	//}
//}