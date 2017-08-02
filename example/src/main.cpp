#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings windowSettings;
    
    windowSettings.width = 1280;
    windowSettings.height = 1024;
    windowSettings.windowMode = OF_WINDOW; // can be OF_WINDOW or OF_FULLSCREEN
    
    ofCreateWindow(windowSettings);
	
	ofRunApp( new ofApp()); // this kicks off the running of my app
    
}
