#include "ofApp.h"
#include <iostream>

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetWindowShape(2560, 1440);
    ofSetWindowShape(1280, 768);
    ofSetWindowTitle("CineVivo");
    ofSetBackgroundColor(125);
    //ofSetFrameRate(30);
    ofSetVerticalSync(false);
    cam.setup(cam.getWidth(), cam.getHeight());
    
    scaleX = 1;
    scaleY = 1;
    
    r1 = 0;
    g1 = 0;
    b1 = 0;
    a1 = 0;
    r2 = 0;
    g2 = 0;
    b2 = 0;
    a2 = 0;
    toMove = 1;
    mix = 0;
    source = 1;
    div = 1;
    div1 = 1;
    divOn = 48;
    div1On = 56;
    multiply = 1;
    mstate = 68;
    distance = 0;
    speedState = false;
    speedState1 = false;
    
    //OSC in
    XML.load ("OSCConf.xml");
    port = XML.getValue("PORT:NAME:PUERTO",5612);
    osc.setup(port);
    
    //-----------------------------------
    
    for(int i = 0; i < LIM; i++){
#ifdef TARGET_OPENGLES
        shaderBlurX[i].load("shadersES2/shaderBlurX");
        shaderBlurY[i].load("shadersES2/shaderBlurY");
#else
        if(ofIsGLProgrammableRenderer()){
            shaderBlurX[i].load("shadersGL3/shaderBlurX");
            shaderBlurY[i].load("shadersGL3/shaderBlurY");
        }else{
            shaderBlurX[i].load("shadersGL2/shaderBlurX");
            shaderBlurY[i].load("shadersGL2/shaderBlurY");
        }
#endif
        vX[i] = 0;
        vY[i] = 0;
        vIndex[i] = 0;
        vIndexPlaying[i] = 0;
        vColor[i] = ofColor(255);
        vScaleX[i] = 1;
        vScaleY[i] = 1;
        vOpacity[i] = 255;
        vRotX[i] = 0;
        vRotY[i] = 0;
        vRotZ[i] = 0;
        vSpeed[i] = 1;
        vBlur[i] = 0;
        edges[i] = false;
        camON[i] = false;
        one[i].set(0,0);
    }
    pix.allocate(1280, 720, OF_PIXELS_RGBA);
    texVideoOne.allocate(pix);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cam.update();
    
    while (osc.hasWaitingMessages()){
        
        ofxOscMessage m;
        osc.getNextMessage(&m);
        
        for(int i  = 0; i < m.getNumArgs(); i++){
            if(m.getArgAsInt(0) >= 0 &&  m.getArgAsInt(0) < LIM){
                if (m.getAddress() == "/load"){
                    if (m.getArgAsString(1) == "camera"){
                        vIndex[m.getArgAsInt(0)] = 1;
                        vIndexPlaying[m.getArgAsInt(0)] = 1;
                        fboBlurOnePass[m.getArgAsInt(0)].allocate(cam.getWidth(),cam.getHeight(),GL_RGBA);
                        fboBlurTwoPass[m.getArgAsInt(0)].allocate(cam.getWidth(),cam.getHeight(),GL_RGBA);
                        vW[m.getArgAsInt(0)] = cam.getWidth();
                        vH[m.getArgAsInt(0)] = cam.getHeight();
                        camON[m.getArgAsInt(0)] = true;
                        four[m.getArgAsInt(0)].set(ofPoint(0,vH[m.getArgAsInt(0)]));
                        three[m.getArgAsInt(0)].set(ofPoint( vW[m.getArgAsInt(0)] ,vH[m.getArgAsInt(0)]));
                        two[m.getArgAsInt(0)].set(ofPoint(vW[m.getArgAsInt(0)],0));
                        pix.allocate(cam.getWidth() , cam.getHeight(), OF_PIXELS_RGB);
                        texVideoOne.allocate(pix);
                    } else {
                        string temp = "videos/"+m.getArgAsString(1);
                        vIndex[m.getArgAsInt(0)] = 1;
                        vIndexPlaying[m.getArgAsInt(0)] = 1;
                        videoLC[m.getArgAsInt(0)].load(temp);
                        videoLC[m.getArgAsInt(0)].play();
                        fboBlurOnePass[m.getArgAsInt(0)].allocate(videoLC[m.getArgAsInt(0)].getWidth(),videoLC[m.getArgAsInt(0)].getHeight(),GL_RGBA);
                        fboBlurTwoPass[m.getArgAsInt(0)].allocate(videoLC[m.getArgAsInt(0)].getWidth(),videoLC[m.getArgAsInt(0)].getHeight(),GL_RGBA);
                        vW[m.getArgAsInt(0)] = videoLC[m.getArgAsInt(0)].getWidth();
                        vH[m.getArgAsInt(0)] = videoLC[m.getArgAsInt(0)].getHeight();
                        four[m.getArgAsInt(0)].set(ofPoint(0,vH[m.getArgAsInt(0)]));
                        three[m.getArgAsInt(0)].set(ofPoint( vW[m.getArgAsInt(0)] ,vH[m.getArgAsInt(0)]));
                        two[m.getArgAsInt(0)].set(ofPoint(vW[m.getArgAsInt(0)],0));
                        pix.allocate(vW[0] , vH[0], OF_PIXELS_RGBA);
                        texVideoOne.allocate(pix);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 0){
                        imitate(previous, videoLC[m.getArgAsInt(0)]);
                        imitate(diff,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 1){
                        imitate(previous1, videoLC[m.getArgAsInt(0)]);
                        imitate(diff1,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 2){
                        imitate(previous2, videoLC[m.getArgAsInt(0)]);
                        imitate(diff2,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 3){
                        imitate(previous3, videoLC[m.getArgAsInt(0)]);
                        imitate(diff3,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 4){
                        imitate(previous4, videoLC[m.getArgAsInt(0)]);
                        imitate(diff4,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 5){
                        imitate(previous5, videoLC[m.getArgAsInt(0)]);
                        imitate(diff5,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 6){
                        imitate(previous6, videoLC[m.getArgAsInt(0)]);
                        imitate(diff6,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 7){
                        imitate(previous7, videoLC[m.getArgAsInt(0)]);
                        imitate(diff7,  videoLC[m.getArgAsInt(0)]);
                    }
                    if(edges[m.getArgAsInt(0)] == true && m.getArgAsInt(0) == 8){
                        imitate(previous8, videoLC[m.getArgAsInt(0)]);
                        imitate(diff8,  videoLC[m.getArgAsInt(0)]);
                    }
                }
                if (m.getAddress() == "/pos"){
                    vX[m.getArgAsInt(0)] = m.getArgAsInt(1);
                    vY[m.getArgAsInt(0)] = m.getArgAsInt(2);
                }
                if (m.getAddress() == "/posX"){
                    vX[m.getArgAsInt(0)] = m.getArgAsInt(1);
                }
                if (m.getAddress() == "/posY"){
                    vY[m.getArgAsInt(0)] = m.getArgAsInt(1);
                }
                if (m.getAddress() == "/rotX"){
                    vRotX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/rotY"){
                    vRotY[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/rotZ"){
                    vRotZ[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/size"){
                    vW[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                    vH[m.getArgAsInt(0)] = m.getArgAsFloat(2);
                }
                if (m.getAddress() == "/width"){
                    vW[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/height"){
                    vW[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/scale"){
                    vScaleX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                    vScaleY[m.getArgAsInt(0)] = m.getArgAsFloat(2);
                }
                if (m.getAddress() == "/speed"){
                    vSpeed[m.getArgAsInt(0)] = m.getArgAsFloat(1);
                }
                if (m.getAddress() == "/play"){
                    vIndexPlaying[m.getArgAsInt(0)] = 1;
                    videoLC[m.getArgAsInt(0)].play();
                }
                if (m.getAddress() == "/unload"){
                    videoLC[m.getArgAsInt(0)].stop();
                    vIndex[m.getArgAsInt(0)] = 0;
                }
                if (m.getAddress() == "/stop"){
                    vIndexPlaying[m.getArgAsInt(0)] = 0;
                    videoLC[m.getArgAsInt(0)].stop();
                }
                if (m.getAddress() == "/pause"){
                    vIndexPlaying[m.getArgAsInt(0)] = 0;
                    videoLC[m.getArgAsInt(0)].setPaused(true);
                }
                if (m.getAddress() == "/frame"){
                    videoLC[m.getArgAsInt(0)].setFrame(m.getArgAsInt(1));
                }
                if (m.getAddress() == "/color"){
                    vColor[m.getArgAsInt(0)] = ofColor(m.getArgAsInt(1),m.getArgAsInt(2),m.getArgAsInt(3));
                }
                if (m.getAddress() == "/opacity"){
                    vOpacity[m.getArgAsInt(0)] = m.getArgAsInt(1);
                }
                if (m.getAddress() == "/blur"){
                    vBlur[m.getArgAsInt(0)] = m.getArgAsInt(1);
                }
                if (m.getAddress() == "/edge"){
                    edges[m.getArgAsInt(0)] = m.getArgAsBool(1);
                }
                if (m.getAddress() == "/points"){
                    one[m.getArgAsInt(0)].set(ofPoint(m.getArgAsInt(1),m.getArgAsInt(2)));
                    two[m.getArgAsInt(0)].set(ofPoint(m.getArgAsInt(3),m.getArgAsInt(4)));
                    three[m.getArgAsInt(0)].set(ofPoint(m.getArgAsInt(5),m.getArgAsInt(6)));
                    four[m.getArgAsInt(0)].set(ofPoint(m.getArgAsInt(7),m.getArgAsInt(8)));
                }
            }
        }
    }
    // TODO: edge for cams
    for(int i = 0; i < LIM; i++){
        if(vIndexPlaying[i] != 0){
            videoLC[i].update();
            pix = videoLC[i].getPixels();
            texVideoOne.loadData(pix);
            if(edges[i] == true && i == 0){
                absdiff(videoLC[i], previous, diff);
                diff.update();
                copy(videoLC[i], previous);
            }
        }
    }
         /*   if(edges[i] == true && i == 1){
                absdiff(videoLC[i], previous1, diff1);
                diff1.update();
                copy(videoLC[i], previous1);
            }
            if(edges[i] == true && i == 2){
                absdiff(videoLC[i], previous2, diff2);
                diff2.update();
                copy(videoLC[i], previous2);
            }
            if(edges[i] == true && i == 3){
                absdiff(videoLC[i], previous3, diff3);
                diff3.update();
                copy(videoLC[i], previous3);
            }
            if(edges[i] == true && i == 4){
                absdiff(videoLC[i], previous4, diff4);
                diff4.update();
                copy(videoLC[i], previous4);
            }
            if(edges[i] == true && i == 5){
                absdiff(videoLC[i], previous5, diff5);
                diff5.update();
                copy(videoLC[i], previous5);
            }
            if(edges[i] == true && i == 6){
                absdiff(videoLC[i], previous2, diff6);
                diff6.update();
                copy(videoLC[i], previous6);
            }
            if(edges[i] == true && i == 7){
                absdiff(videoLC[i], previous7, diff7);
                diff7.update();
                copy(videoLC[i], previous7);
            }
            if(edges[i] == true && i == 8){
                absdiff(videoLC[i], previous8, diff8);
                diff8.update();
                copy(videoLC[i], previous8);
            }
            diffMean[i] = mean(toCv(diff));
            diffMean[i] *= Scalar(50);
        }
    }*/
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    
    for(int i = 0; i < LIM; i++){
        //std::cout << vIndex[i];
        if(vIndex[i] != 0){
            ofSetColor(vColor[i],vOpacity[i]);
            ofPushMatrix();
            ofTranslate(vX[i],vY[i]);
            ofRotateX(vRotX[i]);
            ofRotateY(vRotY[i]);
            ofRotateZ(vRotZ[i]);
            ofScale(1*vScaleX[i],1*vScaleY[i]);
            /*fboBlurOnePass[i].begin();
            shaderBlurX[i].begin();
            shaderBlurX[i].setUniform1f("blurAmnt", vBlur[i]);*/
            videoLC[i].setSpeed(vSpeed[i]);
            if(edges[i] == false && camON[i] == false)
                texVideoOne.draw(one[i], two[i], three[i], four[i]);
            if(edges[i] == false && camON[i] == true)
                //cam.draw(0,0);
                cam.getTexture().draw(one[i],two[i],three[i],four[i]);
            if(edges[i] == true && i == 0)
                diff.getTexture().draw(one[i], two[i], three[i], four[i]);
            if(edges[i] == true && i == 1)
                diff1.draw(0,0);
            if(edges[i] == true && i == 2)
                diff2.draw(0,0);
            if(edges[i] == true && i == 3)
                diff3.draw(0,0);
            if(edges[i] == true && i == 4)
                diff4.draw(0,0);
            if(edges[i] == true && i == 5)
                diff5.draw(0,0);
            if(edges[i] == true && i == 6)
                diff6.draw(0,0);
            if(edges[i] == true && i == 7)
                diff7.draw(0,0);
            if(edges[i] == true && i == 8)
                diff8.draw(0,0);
            /*shaderBlurX[i].end();
            fboBlurOnePass[i].end();
            fboBlurTwoPass[i].begin();
            shaderBlurY[i].begin();
            shaderBlurY[i].setUniform1f("blurAmnt", vBlur[i]);
            fboBlurOnePass[i].draw(0, 0);
            shaderBlurY[i].end();
            fboBlurTwoPass[i].end();
            fboBlurTwoPass[i].draw(0, 0,vW[i],vH[i]);*/
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}


void ofApp::exit() {

}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
