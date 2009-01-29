#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

// this h file has the definition for an object that uses the threaded addon:

#include "threadedObject.h"

class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		threadedObject	TO;
		
		int				mainAppsCount;
};





#endif	
