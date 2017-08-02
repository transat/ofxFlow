#include "ofApp.h"
#include "ofxFlowNodeAdd.h"
#include "ofxFlowNodeValue.h"
#include "ofxFlowNodeElapsedTime.h"
#include "ofxFlowNodeSin.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	
    auto valueNode = std::make_shared<ofxFlowNodeValue<float>>("One point five", 1.5f);
    valueNode->rect = ofRectangle(100, 100, 200, 40);
    _graph.addNode(valueNode);
    
    auto valueNode2 = std::make_shared<ofxFlowNodeValue<float>>("Two point five", 2.5f);
    valueNode2->rect = ofRectangle(100, 200, 200, 40);
    _graph.addNode(valueNode2);
    
    auto timeNode = std::make_shared<ofxFlowNodeElapsedTime<float>>();
    timeNode->rect = ofRectangle(100, 300, 200, 40);
    _graph.addNode(timeNode);
    
    auto additionNode = std::make_shared<ofxFlowNodeAdd<float>>();
    additionNode->rect = ofRectangle(350, 100, 200, 150);
    _graph.addNode(additionNode);
    
    auto additionNode2 = std::make_shared<ofxFlowNodeAdd<float>>();
    additionNode2->rect = ofRectangle(600, 250, 200, 150);
    _graph.addNode(additionNode2);
    
    // TODO: add validation
    auto additionNode3 = std::make_shared<ofxFlowNodeAdd<float>>();
    additionNode3->rect = ofRectangle(850, 400, 200, 150);
    _graph.addNode(additionNode3);
    
    auto sinNode = std::make_shared<ofxFlowNodeSin<float>>();
    sinNode->rect = ofRectangle(350, 400, 200, 150);
    _graph.addNode(sinNode);
	
	additionNode->connectInputTo("value1", valueNode.get(), "value");
	additionNode->connectInputTo("value2", valueNode.get(), "value");
	
	/*
	additionNode2->connectInputTo("value1", additionNode.get(), "result");
	additionNode2->connectInputTo("value2", valueNode.get(), "value");
	
	additionNode3->connectInputTo("value1", sinNode.get(), "result");
	additionNode3->connectInputTo("value2", additionNode2.get(), "result");
	sinNode->connectInputTo("value1", timeNode.get(), "value");
	 */
	
}

//--------------------------------------------------------------
void ofApp::update()
{
	_graph.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(30);
	_graph.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	
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
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
