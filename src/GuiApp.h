#pragma once

#include "ofMain.h"
#include "ofxGLEditor.h"
#include "ofxOsc.h"
#include <stdlib.h>

class GuiApp: public ofBaseApp, public ofxGLEditorListener  {
    
public:
    void setup();
    void draw();
    
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    /// ofxGLEditor events
    void saveFileEvent(int &whichEditor);
    void openFileEvent(int &whichEditor);
    void executeScriptEvent(int &whichEditor);
    void evalReplEvent(const string &text);
    
    //editor
    ofxGLEditor editor;
    ofxEditorSyntax syntax;
    ofxEditorColorScheme colorScheme;
    
    std::vector<string> texto;
    
    ofxOscSender osc;
};

