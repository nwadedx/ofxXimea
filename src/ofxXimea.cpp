//\ ofxXimea
//\ Nathan Wade 03.12.2015
//\ http://nwadedx.info/

#include "ofxXimea.h"

//---------
void ofxXimea::openDialog() {
	setup();
	selectDevice();
}

//---------
void ofxXimea::openDevice(int index) {
	setup();
	setDevice(index);
}

//---------
void ofxXimea::setup() {
	// XIMEA API 
	xiH = NULL;
	stat = XI_OK;

	//ximea image buffer
	image.size = sizeof(XI_IMG);
	image.bp = NULL;
	image.bp_size = 0;
}

//---------
void ofxXimea::selectDevice(){
	//poll connected devices
	DWORD dwNumberOfDevices;
	CHECK_FAIL("xiGetNumberDevices", xiGetNumberDevices(&dwNumberOfDevices));
	int NumberOfDevices = (int)dwNumberOfDevices;

	char info_c [256];
	int info_i;
	float info_f;
	
	if (!NumberOfDevices){
		cout<<"No Ximea device found\n";
	}else{
		if (NumberOfDevices == 1){
			deviceID = 0;
			cout<<"["<<deviceID<<"] : ";
				xiGetDeviceInfoString(deviceID, XI_PRM_DEVICE_NAME, info_c, 255);
				cout<<info_c<<" : ";
				xiGetDeviceInfoString(deviceID, XI_PRM_DEVICE_TYPE, info_c, 255);
				cout <<info_c<<"\n";
				cout<<"\n\n";
		}else{
			//selection dialog
			for(int i = 0; i<NumberOfDevices; i++){
				cout<<"["<<i<<"] : ";
				xiGetDeviceInfoString(i, XI_PRM_DEVICE_NAME, info_c, 255);
				cout<<info_c<<" : ";
				xiGetDeviceInfoString(i, XI_PRM_DEVICE_TYPE, info_c, 255);
				cout <<info_c<<"\n";
			}

			cout<<"xiOpenDevice at index = ";
			cin>>(int)deviceID;
			cout<<"\n\n";
		}
		//retrieving a handle to the camera device 
		CHECK_FAIL("xiOpenDevice", xiOpenDevice(deviceID, &xiH));
		
	}

	//print device params
    CHECK_FAIL("get XI_PRM_DEVICE_NAME", xiGetParamString(xiH, XI_PRM_DEVICE_NAME, &info_c, 255));
	cout<<"XI_PRM_DEVICE_NAME: "<< &info_c <<"\n";

	CHECK_FAIL("get XI_PRM_DEVICE_SN", xiGetParamInt(xiH, XI_PRM_DEVICE_SN, &info_i));
	cout<<"XI_PRM_DEVICE_SN: "<< info_i <<"\n";

	CHECK_FAIL("get XI_PRM_DEVICE_MODEL_ID", xiGetParamInt(xiH, XI_PRM_DEVICE_MODEL_ID, &info_i));
	cout<<"XI_PRM_DEVICE_MODEL_ID: "<< info_i <<"\n";

	CHECK_FAIL("get XI_PRM_DEVICE_TYPE", xiGetParamString(xiH, XI_PRM_DEVICE_TYPE, &info_c, 255));
	cout<<"XI_PRM_DEVICE_TYPE: "<< info_c <<"\n";

    CHECK_FAIL("get XI_PRM_WIDTH", xiGetParamInt(xiH, XI_PRM_WIDTH, &info_i));
	cout<<"XI_PRM_WIDTH: "<< info_i <<"\n";
	sensorWidth = info_i;
        
    CHECK_FAIL("get XI_PRM_HEIGHT", xiGetParamInt(xiH, XI_PRM_HEIGHT, &info_i));
	cout<<"XI_PRM_HEIGHT: "<< info_i <<"\n";
	sensorHeight = info_i;

	cout<<"\n";
}

//---------
void ofxXimea::setDevice(int index){
		
	char info_c [256];
	int info_i;
	float info_f;

	deviceID = index;
	
		//meta
		cout<<"["<<deviceID<<"] : ";
		xiGetDeviceInfoString(deviceID, XI_PRM_DEVICE_NAME, info_c, 255);
		cout<<info_c<<" : ";
		xiGetDeviceInfoString(deviceID, XI_PRM_DEVICE_TYPE, info_c, 255);
		cout <<info_c<<"\n";
		cout<<"\n\n";

		///retrieving a handle to the camera device 
		CHECK_FAIL("xiOpenDevice", xiOpenDevice(deviceID, &xiH));

		//print device params
		CHECK_FAIL("get XI_PRM_DEVICE_NAME", xiGetParamString(xiH, XI_PRM_DEVICE_NAME, &info_c, 255));
		cout<<"XI_PRM_DEVICE_NAME: "<< &info_c <<"\n";

		CHECK_FAIL("get XI_PRM_DEVICE_SN", xiGetParamInt(xiH, XI_PRM_DEVICE_SN, &info_i));
		cout<<"XI_PRM_DEVICE_SN: "<< info_i <<"\n";

		CHECK_FAIL("get XI_PRM_DEVICE_MODEL_ID", xiGetParamInt(xiH, XI_PRM_DEVICE_MODEL_ID, &info_i));
		cout<<"XI_PRM_DEVICE_MODEL_ID: "<< info_i <<"\n";

		CHECK_FAIL("get XI_PRM_DEVICE_TYPE", xiGetParamString(xiH, XI_PRM_DEVICE_TYPE, &info_c, 255));
		cout<<"XI_PRM_DEVICE_TYPE: "<< info_c <<"\n";

		CHECK_FAIL("get XI_PRM_WIDTH", xiGetParamInt(xiH, XI_PRM_WIDTH, &info_i));
		cout<<"XI_PRM_WIDTH: "<< info_i <<"\n";
		sensorWidth = info_i;
        
		CHECK_FAIL("get XI_PRM_HEIGHT", xiGetParamInt(xiH, XI_PRM_HEIGHT, &info_i));
		cout<<"XI_PRM_HEIGHT: "<< info_i <<"\n";
		sensorHeight = info_i;

		cout<<"\n";


}

//---------
void ofxXimea::config(int format) {

	//available modes dialog
	int isColor = 0;
	CHECK_FAIL("get XI_PRM_IMAGE_IS_COLOR", xiGetParamInt(xiH, XI_PRM_IMAGE_IS_COLOR, &isColor));

	if(isColor) {
		if(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8) == XI_OK){
			cout << "Color mode XI_OK. Available Formats:\n"<< "XI_API_VW_DATA_FRM_RAW8 (XI_RAW8)\n"<<"XI_API_VW_DATA_FRM_MONO8 (XI_MONO8)\n"<<"XI_API_VW_DATA_FRM_RGB32 (XI_RGB32)\n"<<"XI_API_VW_DATA_FRM_RGB24 (XI_RGB24)\n";
		}else{
			cout << "Color mode XI_OK. Available Format:\n"<< "XI_API_VW_DATA_FRM_RAW8 (XI_RGB32)\n";
		}

		/*
		#if SUPPORT_FOR_RAW16_ENABLED // SUPPORT_FOR_RAW16_ENABLED
			m_format.InsertString(XI_API_VW_DATA_FRM_RAW16, "RAW16");
		#endif // SUPPORT_FOR_RAW16_ENABLED
		*/
	}else{
		cout << "Mono mode XI_OK. Available Formats:\n"<< "XI_API_VW_DATA_FRM_RAW8 (XI_MONO8)\n";

		/*
		#if SUPPORT_FOR_RAW16_ENABLED // SUPPORT_FOR_RAW16_ENABLED
			m_format.InsertString(XI_API_VW_DATA_FRM_RAW16, "RAW16");
		#endif // SUPPORT_FOR_RAW16_ENABLED
		*/
	}

	// config logic
	switch (format){
		case XI_RGB32: 
			frm = XI_RGB32;
			cout<<"Setting color format to XI_RGB32\n";
			break;
		case XI_RGB24: 
			frm = XI_RGB24;
			cout<<"Setting color format to XI_RGB24\n";
			break;
		case XI_MONO8:
			frm = XI_MONO8;
			cout<<"Setting color format to XI_MONO8\n";
			break;
		case XI_RAW8:
			frm = XI_RAW8;
			cout<<"Setting color format to XI_RAW8\n";
			break;
		case XI_RAW16:
			frm = XI_RAW16;
			cout<<"Setting color format to XI_RAW16\n";
			break;
		default:
			frm = XI_RGB32;
			cout<<"Setting color format to XI_RGB32\n";
			break;
	}
	CHECK_FAIL("set XI_PRM_IMAGE_DATA_FORMAT", xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, frm));
	
	cout<<"\n";


	//pre-aquisition settings

	cout<<"setting XI_PRM_AUTO_BANDWIDTH_CALCULATION: XI_ON\n\n";
	CHECK_FAIL("XI_PRM_AUTO_BANDWIDTH_CALCULATION", xiSetParamInt(xiH, XI_PRM_AUTO_BANDWIDTH_CALCULATION, XI_ON)); //one camera, one controller
	
	CHECK_FAIL("set XI_PRM_GAIN", xiSetParamFloat(xiH, XI_PRM_GAIN, 0)); //Description: Sets gain in dB., Type: Floating., Default: 0

	CHECK_FAIL("set XI_PRM_AEAG", xiSetParamInt(xiH, XI_PRM_AEAG, 0));//Description: Automatic exposure/gain., Type: Integer., Default: 0 (disabled)
	CHECK_FAIL("set XI_PRM_AUTO_WB", xiSetParamInt(xiH, XI_PRM_AUTO_WB, 0));//Description: Automatic white balance., Type: Integer., Default: 0 (disabled)
	
	/*XI_PRM_DOWNSAMPLING
	Description: Changes image resolution by binning or skipping.
	Parameter downsampling_rate control the mapping of sensor pixels to output data.
	downsampling_rate 	result image
	1 	1 sensor pixel = 1 image pixel
	2 	2x2 sensor pixels = 1 image pixel
	4 	4x4 sensor pixels = 1 image pixel
	*/
	CHECK_FAIL("set XI_PRM_DOWNSAMPLING", xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING, 1)); //Type: Integer., Default: 1
	
	//fire it up
	startAcquisition();

	//fork to param defualts
	switch (format){
		case XI_RGB32: 
			configRGB32();
			break;
		case XI_RGB24: 
			configRGB24();
			break;
		case XI_MONO8:
			configMONO8();
			break;
		case XI_RAW8:
			configRAW8();
			break;
		case XI_RAW16:
			configRAW16();
			break;
		default:
			break;
	}

	//Setting XI_PRM_IMAGE_DATA_FORMAT can reset current region of interest.
	//setROI(ofRectangle(0,0,image.width,image.height));

}

//---------
void ofxXimea::configRGB32(){
	//todo
}

//---------
void ofxXimea::configRGB24(){

	CHECK_FAIL("set XI_PRM_ACQ_TIMING_MODE",xiSetParamInt(xiH,XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE));
	
	//GAMMA
	CHECK_FAIL("set XI_PRM_GAMMAY", xiSetParamFloat(xiH, XI_PRM_GAMMAY, 0.47));
	CHECK_FAIL("set XI_PRM_GAMMAC", xiSetParamFloat(xiH, XI_PRM_GAMMAC, 0.5));
	CHECK_FAIL("set XI_PRM_SHARPNESS", xiSetParamFloat(xiH, XI_PRM_SHARPNESS, 0.0));
	
	//ROI
	int tmp = 0;
	CHECK_FAIL("get XI_PRM_WIDTH", xiGetParamInt(xiH, XI_PRM_WIDTH, &tmp));
	int m_roi_cx = tmp;
	CHECK_FAIL("get XI_PRM_HEIGHT",  xiGetParamInt(xiH, XI_PRM_HEIGHT, &tmp));
	int m_roi_cy = tmp;
	CHECK_FAIL("set XI_PRM_AEAG_ROI_OFFSET_X", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_OFFSET_X, 0));
	CHECK_FAIL("set XI_PRM_AEAG_ROI_OFFSET_Y", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_OFFSET_Y, 0)); 
	CHECK_FAIL("set XI_PRM_AEAG_ROI_WIDTH", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_WIDTH, m_roi_cx)); 
	CHECK_FAIL("set XI_PRM_AEAG_ROI_HEIGHT", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_HEIGHT, m_roi_cy)); 
	
	//EXPOSURE
	int ivalue_min = 0;
	int ivalue_max = 0;
	int ivalue = 200000;
	CHECK_FAIL("get XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MIN", xiGetParamInt(xiH, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MIN, &ivalue_min));
	CHECK_FAIL("get XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MAX", xiGetParamInt(xiH, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MAX, &ivalue_max));
	CHECK_FAIL("set XI_PRM_AE_MAX_LIMIT", xiSetParamInt(xiH, XI_PRM_AE_MAX_LIMIT, ivalue));
	
	//GAIN
	float fvalue_min = 0;
	float fvalue_max = 0;
	float fvalue = 16;
	CHECK_FAIL("get XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MIN", xiGetParamFloat(xiH, XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MIN, &fvalue_min));
	CHECK_FAIL("get XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MAX", xiGetParamFloat(xiH, XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MAX, &fvalue_max));
	CHECK_FAIL("set XI_PRM_AG_MAX_LIMIT", xiSetParamFloat(xiH, XI_PRM_AG_MAX_LIMIT, fvalue));
	CHECK_FAIL("get XI_PRM_EXP_PRIORITY XI_PRM_INFO_MIN", xiGetParamFloat(xiH, XI_PRM_EXP_PRIORITY XI_PRM_INFO_MIN, &fvalue_min));
	CHECK_FAIL("get XI_PRM_EXP_PRIORITY XI_PRM_INFO_MAX", xiGetParamFloat(xiH, XI_PRM_EXP_PRIORITY XI_PRM_INFO_MAX, &fvalue_max));
	fvalue = 1;
	CHECK_FAIL("set XI_PRM_EXP_PRIORITY", xiSetParamFloat(xiH, XI_PRM_EXP_PRIORITY, fvalue));
	
	//Bad PIXEL CORRECTION
	CHECK_FAIL("set XI_PRM_BPC", xiSetParamInt(xiH, XI_PRM_BPC, 1));

	

}

//---------
void ofxXimea::configMONO8(){
	setExposure(10000); 
}

//---------
void ofxXimea::configRAW8(){
	//todo
}

//---------
void ofxXimea::configRAW16(){
	//todo
}

//---------
void ofxXimea::startAcquisition() {
	CHECK_FAIL("start acquisition",xiStartAcquisition(xiH));
}

//----------
void ofxXimea::setExposure(int exposure) {
	CHECK_FAIL("set exposure", xiSetParamInt(xiH, XI_PRM_EXPOSURE, exposure));
}

//----------
void ofxXimea::setAWB(int i) {
	CHECK_FAIL("set XI_PRM_AEAG", xiSetParamInt(xiH, XI_PRM_AEAG, i));
	CHECK_FAIL("set XI_PRM_AUTO_WB", xiSetParamInt(xiH, XI_PRM_AUTO_WB, i));
}

//----------
void ofxXimea::setBinning(int binningX, int binningY) {
	if (binningX != binningY) {
		cout << "ofxXimea's api does not allow asymmetrical binning (" << binningX << "," << binningY << ")";
		binningY = binningX;
		cout << "Setting binning to (" << binningX << "," << binningY << ")";
	}

	if (!(binningX == 1 || binningX == 2 || binningX == 4)) {
		cout << "ofxXimea's api does not allow binning rates other than {1,2,4}";
		return;
	}

	CHECK_FAIL("set binning type", xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING_TYPE, XI_SKIPPING));
	CHECK_FAIL("set binning resolution", xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING, binningX));
	cout << "XI_PRM_DOWNSAMPLING set to "<< binningX << "\n";
}

//----------
void ofxXimea::setAWBROI(ofRectangle roi) {
	CHECK_FAIL("set XI_PRM_AEAG_ROI_WIDTH", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_WIDTH, (int) roi.width));
	CHECK_FAIL("set XI_PRM_AEAG_ROI_HEIGHT", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_HEIGHT, (int) roi.height));
	CHECK_FAIL("set XI_PRM_AEAG_ROI_WIDTH", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_OFFSET_X, (int) roi.x));
	CHECK_FAIL("set XI_PRM_AEAG_ROI_HEIGHT", xiSetParamInt(xiH, XI_PRM_AEAG_ROI_OFFSET_Y, (int) roi.y));
}

//----------
void ofxXimea::setROI(ofRectangle roi) {
	int info_i =0;
	xiGetParamInt(xiH, XI_PRM_HEIGHT XI_PRM_INFO_INCREMENT, &info_i);

	CHECK_FAIL("set ROI width", xiSetParamInt(xiH, XI_PRM_WIDTH, (int) roi.width));
	CHECK_FAIL("set ROI height", xiSetParamInt(xiH, XI_PRM_HEIGHT, (int) roi.height));
	CHECK_FAIL("set XI_PRM_OFFSET_X", xiSetParamInt(xiH, XI_PRM_OFFSET_X, (int) roi.x));
	CHECK_FAIL("set XI_PRM_OFFSET_Y", xiSetParamInt(xiH, XI_PRM_OFFSET_Y, (int) roi.y));
}
//----------
void ofxXimea::setTriggerMode(int triggerMode, int triggerSignalType) {

    switch (triggerMode) {
        case 0:
            break;
        case 1:
			CHECK_FAIL("Set GPI port to 1", xiSetParamInt(this->xiH, XI_PRM_GPI_SELECTOR, 1));
			CHECK_FAIL("Set GPI port as input trigger", xiSetParamInt(this->xiH, XI_PRM_GPI_MODE, XI_GPI_TRIGGER));

            switch (triggerSignalType) {
                case XI_TRG_EDGE_RISING:
                    CHECK_FAIL("Set trigger to GPIO input rising edge", xiSetParamInt(xiH, XI_PRM_TRG_SOURCE, XI_TRG_EDGE_RISING));
                    break;
                case  XI_TRG_EDGE_FALLING:
                    CHECK_FAIL("Set trigger to GPIO input falling edge", xiSetParamInt(xiH, XI_PRM_TRG_SOURCE, XI_TRG_EDGE_FALLING));
                    break;
                default:
                    break;
            }
            break;
        case XI_TRG_SOFTWARE:
            CHECK_FAIL("Set trigger to software", xiSetParamInt(xiH, XI_PRM_TRG_SOURCE, XI_TRG_SOFTWARE));
            break;
        default:
            break;
    }
}
    
//----------
void ofxXimea::setGPOMode(int gpoMode) {
  
  CHECK_FAIL("Turn GPO on for port 0", xiSetParamInt(xiH, XI_PRM_GPO_SELECTOR, 1));
		
	int xiParam = -1;

	switch(gpoMode) {
	case XI_GPO_ON:
		xiParam = XI_GPO_ON;
		break;
	case  XI_GPO_OFF:
		xiParam = XI_GPO_OFF;
		break;
	case XI_GPO_EXPOSURE_ACTIVE:
		xiParam = XI_GPO_EXPOSURE_ACTIVE;
		break;
	case XI_GPO_FRAME_ACTIVE:
		xiParam = XI_GPO_FRAME_ACTIVE;
		break;
	case XI_GPO_EXPOSURE_ACTIVE_NEG:
		xiParam = XI_GPO_EXPOSURE_ACTIVE_NEG;
		break;
	case XI_GPO_FRAME_ACTIVE_NEG:
		xiParam = XI_GPO_FRAME_ACTIVE_NEG;
		break;
	}

	CHECK_FAIL("Set GPO mode", xiSetParamInt(xiH, XI_PRM_GPO_MODE, xiParam));
}

//---------
void ofxXimea::setFrameRate(float rate){
	float info_f = 0;

	cout<<"\n";

	CHECK_FAIL("get XI_PRM_FRAMERATE XI_PRM_INFO_MIN", xiGetParamFloat(xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MIN, &info_f));
	cout<<"XI_PRM_FRAMERATE MIN: "<< info_f <<"\n";
	CHECK_FAIL("get XI_PRM_FRAMERATE XI_PRM_INFO_MAX", xiGetParamFloat(xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &info_f));
	cout<<"XI_PRM_FRAMERATE MAX: "<< info_f <<"\n";

	CHECK_FAIL("set XI_PRM_ACQ_TIMING_MODE", xiSetParamInt(xiH,XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE));
	CHECK_FAIL("set XI_PRM_FRAMERATE",xiSetParamFloat(xiH, XI_PRM_FRAMERATE, rate));
	
	CHECK_FAIL("get XI_PRM_FRAMERATE",xiGetParamFloat(xiH, XI_PRM_FRAMERATE, &info_f));
	cout<<"XI_PRM_FRAMERATE set to: "<<info_f<<"\n\n";

	frame_rate = info_f;
}


//---------
void ofxXimea::update() {

	/*
	value 	    one pixel data in memory [one_byte]
	XI_RGB32 	[Blue][Green][Red][0]* (Note1)
	XI_MONO8 	[Intensity] (Note1)
	XI_MONO16 	[Intensity LSB] [Intensity MSB] (Note1)
	XI_RGB24 	[Blue][Green][Red] (Note1)
	XI_RGB32 	[Blue][Green][Red][0]* (Note1)
	XI_RGB_PLANAR 	[Red][Red]...[Green][Green]...[Blue][Blue]... (Note1)
	XI_RAW8 	[pixel byte] raw data from sensor 	
	XI_RAW16 	[pixel byte low] [pixel byte high] 16 bits raw data from sensor 

	Note: For color modes XI_RGB32 and XI_RGB24 the image from sensor should be pre-processed. CPU load is higher in these modes. Setting this parameter will reset current region of interest. XI_RGB24 is being processed from the XI_RGB32 by removing the unused Alpha channel creating a slightly higher CPU load then the XI_RGB32 format.
	Note1: Higher CPU processing is required when this mode is selected because color filter array processing is implemented on PC. This processing is serialized when multiple cameras is used at once. The most effective way to get data from camera is to use XI_RAW8, where no additional processing is done in API.
	*/

	// getting image from camera
	CHECK_FAIL("get xiGetImage", xiGetImage(xiH, 500, &image));

	if(stat == XI_OK){
		
		switch (frm){

			case XI_MONO8:
				bytesPerPixel = 2;
				if (image.height != ofxcvgray.getHeight() || image.width != ofxcvgray.getWidth()) {
					ofxcvgray.allocate(image.width, image.height);
				}
				ofxcvgray.setFromPixels((const unsigned char *)image.bp, image.width, image.height);
				break;

			case XI_RGB24:
				bytesPerPixel = 3;
				if (image.height != ofxcvcolor.getHeight() || image.width != ofxcvcolor.getWidth()) {
					ofxcvcolor.allocate(image.width, image.height); 
				}
				ofxcvcolor.setFromPixels((const unsigned char *)image.bp, image.width, image.height);
				ofxcvcolor.getPixelsRef().swapRgb();
				break;
				
			case XI_RGB32:
				bytesPerPixel = 4;
				cout<<"ERROR: TODO: XI_RGB32 not implemented. Use mode XI_RGB24 or XI_MONO8.\n";
				break;
			
			case XI_RAW16:
				bytesPerPixel = 2;
				cout<<"ERROR: TODO: XI_RAW16 not implemented. Use mode XI_RGB24 or XI_MONO8.\n";
				break;

			case XI_RAW8:
				bytesPerPixel = 1;
				cout<<"ERROR: TODO: XI_RAW8 not implemented. Use mode XI_RGB24 or XI_MONO8.\n";
				break;

			default:
				bytesPerPixel = 1;
				break;
		}
	}


}

//---------
void ofxXimea::draw(int x, int y) {
	
	if(ofxcvcolor.bAllocated){
		ofxcvcolor.draw(x,y);
	}
	else if(ofxcvgray.bAllocated){
		ofxcvgray.draw(x,y);
	}

}

//---------
void ofxXimea::draw(int x, int y, int w, int h){

	if(ofxcvcolor.bAllocated){
		ofxcvcolor.draw(x,y,w,h);
	}
	else if(ofxcvgray.bAllocated){
		ofxcvgray.draw(x,y,w,h);
	}

}

//---------
void ofxXimea::stopAcquisition() {
	CHECK_FAIL("stop acquisition", xiStopAcquisition(xiH));
}

//---------
void ofxXimea::close() {

	// Close device
	if (xiH)
		xiCloseDevice(xiH);
	printf("Done\n");
	#ifdef WIN32
		Sleep(2000);
	#endif
}

//----------
#include "m3ErrorCodes.h"
string ofxXimea::REPORT_ERR(XI_RETURN err){

	switch (err){
		case MM40_OK: return string("Function call succeeded"); break;
		case MM40_INVALID_HANDLE: return string("Invalid handle"); break;
		case MM40_READREG: return string("Register read error"); break;
		case MM40_WRITEREG: return string("Register write error"); break;
		case MM40_FREE_RESOURCES: return string("Freeing resiurces error"); break;
		case MM40_FREE_CHANNEL: return string("Freeing channel error"); break;
		case MM40_FREE_BANDWIDTH: return string("Freeing bandwith error"); break;
		case MM40_READBLK: return string("Read block error"); break;
		case MM40_WRITEBLK: return string("Write block error"); break;
		case MM40_NO_IMAGE: return string(" No image"); break;
		case MM40_TIMEOUT: return string("Timeout"); break;
		case MM40_INVALID_ARG: return string("Invalid arguments supplied"); break;
		case MM40_NOT_SUPPORTED: return string("Not supported"); break;
		case MM40_ISOCH_ATTACH_BUFFERS: return string("Attach buffers error"); break;
		case MM40_GET_OVERLAPPED_RESULT: return string("Overlapped result"); break;
		case MM40_MEMORY_ALLOCATION: return string("Memory allocation error"); break;
		case MM40_DLLCONTEXTISNULL: return string("DLL context is NULL"); break;
		case MM40_DLLCONTEXTISNONZERO: return string("DLL context is non zero"); break;
		case MM40_DLLCONTEXTEXIST: return string("DLL context exists"); break;
		case MM40_TOOMANYDEVICES: return string("Too many devices connected"); break;
		case MM40_ERRORCAMCONTEXT: return string("Camera context error"); break;
		case MM40_UNKNOWN_HARDWARE: return string("Unknown hardware"); break;
		case MM40_INVALID_TM_FILE: return string("Invalid TM file"); break;
		case MM40_INVALID_TM_TAG: return string("Invalid TM tag"); break;
		case MM40_INCOMPLETE_TM: return string("Incomplete TM"); break;
		case MM40_BUS_RESET_FAILED: return string("Bus reset error"); break;
		case MM40_NOT_IMPLEMENTED: return string("Not implemented"); break;
		case MM40_SHADING_TOOBRIGHT: return string("Shading too bright"); break;
		case MM40_SHADING_TOODARK: return string("Shading too dark"); break;
		case MM40_TOO_LOW_GAIN: return string("Gain is too low"); break;
		case MM40_INVALID_BPL: return string("Invalid bad pixel list"); break;
		case MM40_BPL_REALLOC: return string("Bad pixel list realloc error"); break;
		case MM40_INVALID_PIXEL_LIST: return string("Invalid pixel list"); break;
		case MM40_INVALID_FFS: return string("Invalid Flash File System"); break;
		case MM40_INVALID_PROFILE: return string("Invalid profile"); break;
		case MM40_INVALID_CALIBRATION: return string("Invalid calibration"); break;
		case MM40_INVALID_BUFFER: return string("Invalid buffer"); break;
		case MM40_INVALID_DATA: return string("Invalid data"); break;
		case MM40_TGBUSY: return string("Timing generator is busy"); break;
		case MM40_IO_WRONG: return string("Wrong operation open/write/read/close"); break;
		case MM40_ACQUISITION_ALREADY_UP: return string(" Acquisition already started"); break;
		case MM40_OLD_DRIVER_VERSION: return string("Old version of device driver installed to the system."); break;
		case MM40_GET_LAST_ERROR: return string("To get error code please call GetLastError function."); break;
		case MM40_CANT_PROCESS: return string("Data can't be processed"); break;
		case MM40_ACQUISITION_STOPED: return string("Acquisition has been stopped. It should be started before GetImage."); break;
		case MM40_ACQUISITION_STOPED_WERR: return string("Acquisition has been stoped with error."); break;
		case MM40_INVALID_INPUT_ICC_PROFILE: return string("Input ICC profile missed or corrupted"); break;
		case MM40_INVALID_OUTPUT_ICC_PROFILE: return string("Output ICC profile missed or corrupted"); break;
		case MM40_DEVICE_NOT_READY: return string("Device not ready to operate"); break;
		case MM40_SHADING_TOOCONTRAST: return string("Shading too contrast"); break;
		case MM40_ALREADY_INITIALIZED: return string("Modile already initialized"); break;
		case MM40_NOT_ENOUGH_PRIVILEGES: return string("Application doesn't enough privileges"); break;
		case MM40_NOT_COMPATIBLE_DRIVER: return string("Installed driver not compatible with current software"); break;
		case MM40_TM_INVALID_RESOURCE: return string("TM file was not loaded successfully from resources"); break;
		case MM40_DEVICE_HAS_BEEN_RESETED: return string("Device has been reseted, abnormal initial state"); break;
		case MM40_NO_DEVICES_FOUND: return string("No Devices Found"); break;
		case MM40_RESOURCE_OR_FUNCTION_LOCKED: return string("Resource(device) or function locked by mutex"); break;
		case MM40_BUFFER_SIZE_TOO_SMALL: return string("Buffer provided by user is too small"); break;
		case MM40_COULDNT_INIT_PROCESSOR: return string("Couldn't initialize processor."); break;
		default: return string("ERROR UNKNOWN"); break;
	}

}

void ofxXimea::CHECK_FAIL(string operation, XI_RETURN result){
	 if (result != XI_OK) {
		 cout << "Error [" << REPORT_ERR(result) << "] during " << operation<<"\n";
	 }
}

