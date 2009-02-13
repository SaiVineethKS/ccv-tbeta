#ifndef _Calibration_H
#define _Calibration_H
/*
 *  Calibration.h
 *  tbeta
 *
 *  Created by Artem Titoulenko on 2/1/09.
 *  Copyright 2009 NUI Inc.. All rights reserved.
 *
 */
#define OF_ADDON_USING_OFXXMLSETTINGS  // LOAD CONFIG.XML

#include "ofAddons.h"
#include "ofMain.h"

#include "calibrationB.h"
#include "../TouchMessenger.h"
#include "../Tracking/ofxTBetaCvContourFinder.h"
#include "../Tracking/tracking.h"


class Calibration : public TouchListener, public ofSimpleApp {

	public:

		Calibration() {
			TouchEvents.addRAWListener(this);
			calibrating = false;
		}

		/****************************************************************
		 *						Public
        ****************************************************************/
		//Basic Methods
		void setup();

		//Key Events
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

        //Touch Events
		void RAWTouchDown( ofxTBetaCvBlob b );
		void RAWTouchMoved( ofxTBetaCvBlob b );
		void RAWTouchUp( ofxTBetaCvBlob b );

		void drawFingerOutlines();
        void doCalibration();
		void drawCalibrationBlobs();
		void drawCalibrationPointsAndBox();
		void saveConfiguration();
	
		void passInContourFinderAndTracker(ofxTBetaCvContourFinder &finder, BlobTracker &trackerIn);

		ofxTBetaCvContourFinder	contourFinder;
		bool                calibrating;

		/****************************************************************
		 *						Private
        ****************************************************************/

	private:

        bool				bW;
		bool				bS;
		bool				bA;
		bool				bD;

        int 				camWidth;
		int 				camHeight;

		//---------------------------------------Fonts
		ofTrueTypeFont		verdana;
		ofTrueTypeFont		calibrationText;

		//---------------------------------------Blob Tracker
//		BlobTracker			tracker;

		//---------------------------------------Calibration Stuff
		ofImage calibrationParticle;
		bool	bShowTargets;
		float	downColor;
		std::map<int, int> blobcolor;

		calibrationB calibrate;
		BlobTracker tracker;

		ofxXmlSettings		calibrationXML;
};

#endif
