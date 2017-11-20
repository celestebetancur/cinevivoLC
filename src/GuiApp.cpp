#include "ofApp.h"
#include <stdlib.h>

//--------------------------------------------------------------
void GuiApp::setup() {
    
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetWindowTitle("CineVivo - Live Coding");
    
    // let's see what's going on inside
    //ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);
    
    // setup the global editor font before you do anything!
    ofxEditor::loadFont("fonts/font.ttf", 20);
    
    // set a custom Repl banner & prompt (do this before setup())
    //ofxRepl::setReplBanner("CQenze");
    ofxRepl::setReplPrompt("~> ");
    
    // setup editor with event listener
    editor.setup(this);
    
    colorScheme.loadFile("colorScheme.xml");
    syntax.loadFile("CVSyntax.xml");
    
    editor.setColorScheme(&colorScheme);
    editor.getSettings().addSyntax(&syntax);
    //editor.getSettings().printSyntaxes();
    
    // make some room for the bottom editor info text
    editor.resize(ofGetWidth(), ofGetHeight()-ofxEditor::getCharHeight());
    
    // change multi editor settings, see ofxEditorSettings.h for details
    //editor.getSettings().setTextColor(ofColor::red); // main text color
    //editor.getSettings().setCursorColor(ofColor::blue); // current pos cursor
    //editor.getSettings().setAlpha(0.5); // main text, cursor, & highlight alpha
    
    // other settings
    editor.setLineWrapping(true);
    editor.setLineNumbers(true);
    editor.setAutoFocus(false);
    
    // move the cursor to a specific line
    //editor.setCurrentLine(4);
    //ofLogNotice() << "current line: " << editor.getCurrentLine();
    
    osc.setup("127.0.0.1",5612);
    
}

//--------------------------------------------------------------
void GuiApp::draw() {
    
    editor.draw();
    
    // draw current editor info using the same font as the editor
    if(!editor.isHidden() && editor.getCurrentEditor() > 0) {
        
        int bottom = ofGetHeight()-ofxEditor::getCharHeight();
        int right = ofGetWidth()-ofxEditor::getCharWidth()*7; // should be enough room
        ofSetColor(ofColor::gray);
        
        // draw the current editor num
        editor.drawString("Editor: "+ofToString(editor.getCurrentEditor()),
                          0, bottom);
        
        // draw the current line & line pos
        editor.drawString(ofToString(editor.getCurrentLine()+1)+","+
                          ofToString(editor.getCurrentLinePos()),
                          right, bottom);
    }
}

//--------------------------------------------------------------
void GuiApp::keyPressed(int key) {
    
    // note: when using CTRL as the modifier key, CTRL + key might be an ascii
    // control char so check for both the char 'f' and 6, the ascii value for
    // CTRL + f, see also: http://ascii-table.com/control-chars.php
    bool modifier = ofGetKeyPressed(ofxEditor::getSuperAsModifier() ? OF_KEY_SUPER : OF_KEY_CONTROL);
    if(modifier) {
        switch(key) {
            case 'f': case 6:
                ofToggleFullscreen();
                return;
            case 'l': case 12:
                editor.setLineWrapping(!editor.getLineWrapping());
                return;
            case 'n': case 14:
                editor.setLineNumbers(!editor.getLineNumbers());
                return;
            case 'k': case 26:
                editor.setAutoFocus(!editor.getAutoFocus());
                return;
        }
    }
    
    // see ofxGLEditor.h for key commands
    editor.keyPressed(key);
}

//--------------------------------------------------------------
void GuiApp::windowResized(int w, int h) {
    // make some room for the bottom editor info text
    editor.resize(w, h-ofxEditor::getCharHeight());
}

//--------------------------------------------------------------
void GuiApp::openFileEvent(int &whichEditor) {
    // received an editor open via CTRL/Super + o
    
    ofLogNotice() << "received open event for editor " << whichEditor
    << " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void GuiApp::saveFileEvent(int &whichEditor) {
    // received an editor save via CTRL/Super + s or CTRL/Super + d
    
    ofLogNotice() << "received save event for editor " << whichEditor
    << " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void GuiApp::executeScriptEvent(int &whichEditor) {
    // received on editor CTRL/Super + e
    
    string txt = editor.getText(whichEditor);
    texto = ofSplitString(txt, " ");
    ofxOscMessage s;
    
    int numV = ofToInt(texto[0]);
    if(numV >= 0 && numV <= 7){
        if(texto[1] == "load"){
            string temp = "/load " + ofToString(numV) + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/load");
            s.addIntArg(numV);
            s.addStringArg(ofToString(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "size"){
            string temp = "/sizeX " + ofToString(numV) + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/size");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            s.addIntArg(ofToInt(texto[3]));
            osc.sendMessage(s);
        }
        if(texto[1] == "width"){
            string temp = "/width " + ofToString(numV) + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/width");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "height"){
            string temp = "/height 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/height");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "scale"){
            string temp = "/scale 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/scale");
            s.addIntArg(numV);
            s.addFloatArg(ofToFloat(texto[2]));
            s.addIntArg(ofToInt(texto[3]));
            osc.sendMessage(s);
        }
        if(texto[1] == "color"){
            string temp = "/color 1 " + texto[2] + " " + texto[3] + " " + texto[4];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/color");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            s.addIntArg(ofToInt(texto[3]));
            s.addIntArg(ofToInt(texto[4]));
            osc.sendMessage(s);
        }
        if(texto[1] == "blur"){
            string temp = "/blur 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/blur");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "edge"){
            string temp = "/edge 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/edge");
            s.addIntArg(numV);
            s.addBoolArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "speed"){
            string temp = "/speed 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/speed");
            s.addIntArg(numV);
            s.addFloatArg(ofToFloat(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "posX"){
            string temp = "/speed 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/posX");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "posY"){
            string temp = "/speed 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/posY");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "opacity"){
            string temp = "/opacity 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/opacity");
            s.addIntArg(numV);
            s.addIntArg(ofToInt(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "rotX"){
            string temp = "/rotX 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/rotX");
            s.addIntArg(numV);
            s.addFloatArg(ofToFloat(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "rotY"){
            string temp = "/rotY 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/rotY");
            s.addIntArg(numV);
            s.addFloatArg(ofToFloat(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "rotZ"){
            string temp = "/rotZ 1 " + texto[2];
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/rotZ");
            s.addIntArg(numV);
            s.addFloatArg(ofToFloat(texto[2]));
            osc.sendMessage(s);
        }
        if(texto[1] == "unload"){
            string temp = "/unload";
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/unload");
            s.addIntArg(numV);
            osc.sendMessage(s);
        }
        if(texto[1] == "stop"){
            string temp = "/stop";
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/stop");
            s.addIntArg(numV);
            osc.sendMessage(s);
        }
        if(texto[1] == "play"){
            string temp = "/play";
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/play");
            s.addIntArg(numV);
            osc.sendMessage(s);
        }
        if(texto[1] == "pause"){
            string temp = "/pause";
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/pause");
            s.addIntArg(numV);
            osc.sendMessage(s);
        }
        if(texto[1] == "points"){
            string temp = "/points";
            ofLogNotice() << "CineVivo[send]: " << temp;
            s.setAddress("/points");
            s.addIntArg(ofToInt(texto[2]));
            s.addIntArg(ofToInt(texto[3]));
            s.addIntArg(ofToInt(texto[4]));
            s.addIntArg(ofToInt(texto[5]));
            s.addIntArg(ofToInt(texto[6]));
            s.addIntArg(ofToInt(texto[7]));
            s.addIntArg(ofToInt(texto[8]));
            s.addIntArg(ofToInt(texto[9]));
            osc.sendMessage(s);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::evalReplEvent(const string &text) {
    
    if(text == "help"){
        editor.openFile("help.txt", 9);
        editor.evalReplReturn("Help was opened in buffer 9");
    }
}

