#include "ofGraphics.h"
#include "ofxFlowGraph.h"
#include "ofPath.h"

void ofxFlowGraph::addNode (ofxFlowNodePtr node)
{
	_nodes.push_back(node);
}

void ofxFlowGraph::update()
{
	for (vector<ofxFlowNodePtr>::iterator n = _nodes.begin(); n != _nodes.end(); n++)
	{
		ofxFlowNodePtr node = (*n);
		
		if (node->_outputConnections.size() == 0) // this is an endpoint
		{
			_updateInputs(node.get());
		}
	}
}

void ofxFlowGraph::_updateInputs(ofxFlowNode *node)
{
	map<string, ofxFlowNode::Connection>::iterator c = node->_inputConnections.begin();
	
	for (; c != node->_inputConnections.end(); c++)
	{
		_updateInputs(c->second.node);
		node->setInputValue(c->first, c->second.node->getOutputValue(c->second.paramName));
	}
	
	node->evaluate();
}

void ofxFlowGraph::draw ()
{
	for (vector<ofxFlowNodePtr>::iterator n = _nodes.begin(); n != _nodes.end(); n++)
	{
		ofxFlowNodePtr node = (*n);
		
		if (node->_outputConnections.size() == 0) // this is an endpoint
		{
			_drawInputConnections(node.get());
		}
		
		node->draw();
	}
}

void ofxFlowGraph::_drawInputConnections(ofxFlowNode *node)
{
	map<string, ofxFlowNode::Connection>::iterator c = node->_inputConnections.begin();
	
	ofSetColor(255);
	
	for (; c != node->_inputConnections.end(); c++)
	{
		ofxFlowNode *node2 = c->second.node;
		
		int outIndex = node2->getOutputIndex(c->second.paramName);
		int inIndex = node->getInputIndex(c->first);
		ofPoint p1 = node2->rect.position;
		p1.x += node2->rect.width;
		p1.y += 35 + (15*outIndex);
		ofPoint p2 = node->rect.position;
		p2.y += 35 + (15*inIndex);
		
		glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(p1.x, p1.y);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(p2.x, p2.y);
		glEnd();
		
		_drawInputConnections(node2);
	}
}
