//\ ofxXimea
//\ rev03122015 Nathan Wade 

#pragma once

#ifndef _WIN32_WINNT		             
#define _WIN32_WINNT 0x0501	
#endif						

#ifdef WIN32
// win32 only
#include "xiApi.h"
#include <tchar.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#else
// unix type
#define _tmain(a,b) main(a,b)
#define _TCHAR char
#define _getch()
#include <m3api/xiApi.h>
#endif

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxXimea{

	public:
		void openDialog();
		void openDevice(int index);
		void setup();
		void selectDevice();
		void setDevice(int index);
		void config(int format);
		void configRGB32();
		void configRGB24();
		void configMONO8();
		void configRAW8();
		void configRAW16();
		void startAcquisition();
		void setExposure(int exposure);
		void setAWB(int i);
		void setBinning(int binningX, int binningY);
		void setAWBROI(ofRectangle roi);
		void setROI(ofRectangle roi);
		void setTriggerMode(int triggerMode, int triggerSignalType);
		void setGPOMode(int gpoMode);
		void setFrameRate(float rate);
		void update();
		void draw(int x, int y);
		void draw(int x, int y, int w, int h);
		void stopAcquisition();
		void close();

		string REPORT_ERR(XI_RETURN err);
		void CHECK_FAIL(string operation, XI_RETURN result);

	HANDLE xiH;
	XI_RETURN stat;
	
	XI_IMG image;
	XI_IMG_FORMAT frm;
	
	int deviceID, sensorWidth, sensorHeight, bytesPerPixel;
	float frame_rate;

	ofxCvColorImage ofxcvcolor;
	ofxCvGrayscaleImage ofxcvgray;
};
