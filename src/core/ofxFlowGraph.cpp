#include "ofGraphics.h"
#include "ofxFlowGraph.h"
#include "ofPath.h"

ofxFlowGraph::ofxFlowGraph ()
{
	ofRegisterMouseEvents(this);
}

void ofxFlowGraph::addNode (ofxFlowNodePtr node)
{
	_nodes.push_back(node);
	ofAddListener(node->inputMouseDown, this, &ofxFlowGraph::_inputMouseDown);
	ofAddListener(node->inputMouseUp, this, &ofxFlowGraph::_inputMouseUp);
	ofAddListener(node->outputMouseDown, this, &ofxFlowGraph::_outputMouseDown);
	ofAddListener(node->outputMouseUp, this, &ofxFlowGraph::_outputMouseUp);
}

void ofxFlowGraph::update()
{
	for (vector<ofxFlowNodePtr>::iterator n = _nodes.begin(); n != _nodes.end(); n++)
	{
		ofxFlowNodePtr &node = (*n);
		
		if (node->_outputConnections.size() == 0) // this is an endpoint so start evaluating backwards from here
		{
			_evaluateInputs(node.get());
		}
	}
}

void ofxFlowGraph::_evaluateInputs(ofxFlowNode *node)
{
	map<string, ofxFlowNode::Connection>::iterator c = node->_inputConnections.begin();
	
	for (; c != node->_inputConnections.end(); c++)
	{
		// traverese the tree backwards and evaluate all inputs and pass the values on down the tree
		_evaluateInputs(c->second.node);
		node->setInputValue(c->first, c->second.node->getOutputValue(c->second.paramName));
	}
	
	if (node->validate())
	{
		cout << node->name << " is valid: " << node->validate() << endl;
		node->evaluate();
	}
}

void ofxFlowGraph::draw ()
{
	for (vector<ofxFlowNodePtr>::iterator n = _nodes.begin(); n != _nodes.end(); n++)
	{
		ofxFlowNodePtr &node = (*n);
		
		if (node->_outputConnections.size() == 0) // this is an endpoint
		{
			_drawInputConnections(node.get());
		}
		
		node->draw();
	}
	
	ofSetColor(255, 255, 255, 50);
	for (vector<TempConnection>::iterator c = _tempConnections.begin(); c != _tempConnections.end(); c++)
	{
		ofDrawLine(c->p1.x, c->p1.y, ofGetMouseX(), ofGetMouseY());
	}
}

void ofxFlowGraph::_drawInputConnections(ofxFlowNode *node)
{
	map<string, ofxFlowNode::Connection>::iterator c = node->_inputConnections.begin();
	
	ofSetColor(255);
	
	for (; c != node->_inputConnections.end(); c++)
	{
		ofxFlowNode *node2 = c->second.node;
		
		ofRectangle r1 = node2->getOutputRect(c->second.paramName);
		ofRectangle r2 = node->getInputRect(c->first);
		ofPoint p1 = node2->rect.position + r1.position + (r1.width*.5f);
		ofPoint p2 = node->rect.position + r2.position + (r1.width*.5f);
		
		glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(p1.x, p1.y);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(p2.x, p2.y);
		glEnd();
		
		_drawInputConnections(node2);
	}
}

void ofxFlowGraph::mousePressed(ofMouseEventArgs &e)
{
	_prevMousePos = e;
	
	for (vector<ofxFlowNodePtr>::reverse_iterator n = _nodes.rbegin(); n != _nodes.rend(); n++)
	{
		ofxFlowNodePtr &node = *n;
		if (node->rect.inside(e))
		{
			ofPoint p = e - node->rect.position;
			node->mousePressed(p);
			
			if (node->isDraggableAtPoint(p))
			{
				_nodesBeingDragged.push_back(node);
				_nodeOrigRect[node] = node->rect;
			}
			
			break;
		}
	}
}

void ofxFlowGraph::mouseReleased(ofMouseEventArgs &e)
{
	_nodesBeingDragged.clear();
	
	for (vector<ofxFlowNodePtr>::reverse_iterator n = _nodes.rbegin(); n != _nodes.rend(); n++)
	{
		ofxFlowNodePtr &node = *n;
		if (node->rect.inside(e))
		{
			ofPoint p = e - node->rect.position;
			node->mouseReleased(p);
			break;
		}
	}
	
	_tempConnections.clear();
}

void ofxFlowGraph::mouseMoved(ofMouseEventArgs &e)
{
	
}

void ofxFlowGraph::mouseScrolled(ofMouseEventArgs &e)
{
    
}

void ofxFlowGraph::mouseEntered(ofMouseEventArgs &e)
{
    
}

void ofxFlowGraph::mouseExited(ofMouseEventArgs &e)
{
    
}


void ofxFlowGraph::mouseDragged(ofMouseEventArgs &e)
{
	for (vector<ofxFlowNodePtr>::iterator n = _nodesBeingDragged.begin(); n != _nodesBeingDragged.end(); n++)
	{
		ofxFlowNodePtr &node = *n;
		node->rect.position = _nodeOrigRect[node].position - (_prevMousePos - e);
	}
}

void ofxFlowGraph::_inputMouseDown(ofxFlowNode::ofxFlowNodeEventArgs &e)
{
	TempConnection t;
	t.p1 = e.node->rect.position + e.node->getInputRect(e.paramName).getCenter();
	t.node = e.node;
	t.paramName = e.paramName;
	t.portType = ofxFlowNode::PORT_TYPE_INPUT;
	_tempConnections.push_back(t);
}

void ofxFlowGraph::_outputMouseDown(ofxFlowNode::ofxFlowNodeEventArgs &e)
{
	TempConnection t;
	t.p1 = e.node->rect.position + e.node->getOutputRect(e.paramName).getCenter();
	t.node = e.node;
	t.paramName = e.paramName;
	t.portType = ofxFlowNode::PORT_TYPE_OUTPUT;
	_tempConnections.push_back(t);
}

void ofxFlowGraph::_inputMouseUp(ofxFlowNode::ofxFlowNodeEventArgs &e)
{
	if (_tempConnections.size() > 0) // make the connection!
	{
		TempConnection c = _tempConnections[0];
		
		if (c.portType == ofxFlowNode::PORT_TYPE_INPUT)
		{
			ofLog(OF_LOG_ERROR) << "can't connect input to input" << endl;
		}
		else
		{
			ofxFlowNode *outNode = c.node;
			string outputName = c.paramName;
			ofxFlowNode *inNode = e.node;
			string inputName = e.paramName;
			
			outNode->connectOutputTo(outputName, inNode, inputName);
		}
	}
}

void ofxFlowGraph::_outputMouseUp(ofxFlowNode::ofxFlowNodeEventArgs &e)
{
	if (_tempConnections.size() > 0)
	{
		TempConnection c = _tempConnections[0];
		
		if (c.portType == ofxFlowNode::PORT_TYPE_OUTPUT)
		{
			ofLog(OF_LOG_ERROR) << "can't connect output to output" << endl;
		}
		else
		{
			ofxFlowNode *inNode = c.node;
			string inputName = c.paramName;
			ofxFlowNode *outNode = e.node;
			string outputName = e.paramName;
			
			outNode->connectOutputTo(outputName, inNode, inputName);
		}
	}
}


