/*
 *  TheJourneyOfColorApp.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#include "AppManager.h"

#include "TheJourneyOfColorApp.h"

//--------------------------------------------------------------
void TheJourneyOfColorApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void TheJourneyOfColorApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void TheJourneyOfColorApp::draw(){
    AppManager::getInstance().draw();
}

void TheJourneyOfColorApp::exit()
{
    ofLogNotice() <<"TheJourneyOfColorApp::exit";

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::keyPressed(int key){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::keyReleased(int key){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void TheJourneyOfColorApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void TheJourneyOfColorApp::dragEvent(ofDragInfo dragInfo){

}
