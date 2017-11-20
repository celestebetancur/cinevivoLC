#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "GuiApp.h"
#include "ofxCv.h"
#define NUM 1280
#define LIM 8

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        string FILE;
    
        //OSC
        ofxXmlSettings XML;
        ofxOscReceiver osc;
        int port;
    
        // CineVivo LiveCoding
        //ofPoint one[LIM],two[LIM],three[LIM],four[LIM];
        ofVec2f one[LIM],two[LIM],three[LIM],four[LIM];
        ofTexture texVideoOne;
        ofPixels pix;
    
        ofVideoGrabber cam;
        bool camON[LIM];
        ofVideoPlayer videoLC[LIM];
        int vX[LIM];
        int vY[LIM];
        int vIndex[LIM];
        int vIndexPlaying[LIM];
        float vScaleX[LIM];
        float vScaleY[LIM];
        ofColor vColor[LIM];
        int vOpacity[LIM];
        float vRotX [LIM];
        float vRotY [LIM];
        float vRotZ [LIM];
        float vSpeed [LIM];
        int vW [LIM];
        int vH [LIM];
    
        // Blur
        int vBlur[LIM];
        ofShader shaderBlurX[LIM];
        ofShader shaderBlurY[LIM];
        ofFbo fboBlurOnePass[LIM];
        ofFbo fboBlurTwoPass[LIM];
    
        // Edge
        bool edges[LIM];
        ofPixels previous, previous1, previous2, previous3, previous4, previous5, previous6, previous7, previous8;
        ofImage diff, diff1,  diff2, diff3, diff4, diff5, diff6, diff7, diff8;
        cv::Scalar diffMean[LIM];
    
        // CineVivo RUIDO version
    
        ofVideoPlayer video, video1;
        ofPixels pixels, pixels1, r,g,b,a, R, G, B, A;
        ofTexture texture, texture1;
    
        ofPixels Pixels[NUM], Pixels1[NUM];
        ofTexture textures[NUM], textures1[NUM];
    
        int W, H, W1, H1, distance, modul, state, toMove, div, div1, divOn, div1On;
        int multiply, mstate;
        int source;
        int move[16], move1[16];
        int up[16], up1[16];
    
        bool speedState, speedState1;
    
        float r1, g1, b1, a1, r2, g2, b2, a2, mix;
    
        float scaleX, scaleY;
    
        ofTrueTypeFont text;
    
        shared_ptr<GuiApp> gui;
        int tp = 1;
        int tp1 = 1;
};
