#include "node.h"

/**
* Node constructor
* 
* @param res the resource's name
*/
Node::Node(string res){
    resource = res;
}

string Node::getResource() const{
    return resource;
}

vector<Node*>& Node::getDependencyNodes(){
    return dependencyNodes;
}

vector<string>& Node::getDependencyStrings(){
    return dependencyStrings;
}

/**
 * Add a new dependency to this node
 * 
 * @param dependency the new dependency
 */
void Node::addDependency(Node* dependency){
    dependencyNodes.push_back(dependency);
    dependencyStrings.push_back(dependency->getResource());
}