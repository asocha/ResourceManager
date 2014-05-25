#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include "node.h"

using namespace std;

void loadResources(fstream& file, vector<Node*>& resources);
void displayResources(vector<Node*>& resources);
void deleteResources(vector<Node*>& resources);
void deleteResource(vector<Node*>& resources, string resource);
void saveResources(vector<Node*>& resources, fstream& file);
void createLink(string resource, string dependency, vector<Node*>& resources);

/**
 * Implements a Resource Manager, where resources depend on other resources. Resource dependencies
 * are initially loaded from an input file, but they can be updated manually.
 * 
 * @author Andrew Socha
 */
int main(int argc, char** argv) {
    fstream file("resource.txt");
    if (file){
        vector<Node*> resources;
        loadResources(file, resources);
        
        string input, input2;
        while (true){
            displayResources(resources);
            cout << endl << "Input: ";
            cin >> input;
            
            if (input == "save") saveResources(resources, file);
            else if (input == "q") break;
            else if (input == "new"){
                cin >> input;
                cin >> input2;
                createLink(input, input2, resources);
            }
            else deleteResource(resources, input);
        }
        deleteResources(resources);
        file.close();
    }
    else{
        cout << "Couldn't open resource file." << endl;
    }
    return 0;
}

/**
 * Load the resource graph from the input file.
 * 
 * @param file      the input file
 * @param resources a vector that will contain the list of resources after they are loaded
 */
void loadResources(fstream& file, vector<Node*>& resources){
    string input;
    Node* newResource1, *newResource2;
    
    //read the existing resource nodes
    while(file.good()){
        //the first resource on the line
        file >> input;
        for (int i = 0; i < resources.size(); i++){
            if (resources[i]->getResource() == input){
                newResource1 = resources[i];
                goto resource1Exists;
            }
        }
        newResource1 = new Node(input);
        resources.push_back(newResource1);
resource1Exists:


        //the second resource on the line
        file >> input;
        for (int i = 0; i < resources.size(); i++){
            if (resources[i]->getResource() == input){
                newResource2 = resources[i];
                goto resource2Exists;
            }
        }
        newResource2 = new Node(input);
        resources.push_back(newResource2);
resource2Exists:
        newResource1->addDependency(newResource2);
    }
}

/**
 * Display the resource graph and list of resource usability.
 * 
 * @param resources the list of all resources
 */
void displayResources(vector<Node*>& resources){
    //print the graph
    cout << endl << "Current Resource Graph" << endl;
    for (int i = 0; i < resources.size(); i++){
        vector<string>& dependencyStrings = resources[i]->getDependencyStrings();
        for (int j = 0; j < dependencyStrings.size(); j++){
            //print each resource-dependency combination
            cout << resources[i]->getResource() << " " << dependencyStrings[j] << endl;
        }
    }
    
    //print resource usability
    cout << endl << "Current Resource Usability" << endl;
    for (int i = 0; i < resources.size(); i++){
        vector<Node*>& dependencyNodes = resources[i]->getDependencyNodes();
        vector<string>& dependencyStrings = resources[i]->getDependencyStrings();
        cout << resources[i]->getResource();
        
        for (int j = 0; j < dependencyStrings.size(); j++){
            for (int x = 0; x < dependencyNodes.size() + 1; x++){
                //check if the resource still exists
                if (x == dependencyNodes.size()){
                    cout << " UNUSABLE";
                    goto unusable;
                }
                if (dependencyNodes[x]->getResource() == dependencyStrings[j]) break;
            }
        }
unusable:
        cout << endl;
    }
}

/**
 * Delete a resource node, ensuring that other nodes update based on its deletion.
 * 
 * @param resources the list of all resources
 * @param resource  the resource to remove
 */
void deleteResource(vector<Node*>& resources, string resource){
    Node* correspondingNode = 0;
    for (int i = 0; i < resources.size(); i++){
        if (resources[i]->getResource() == resource){
            //delete the resource's node
            correspondingNode = resources[i];
            resources.erase(resources.begin() + i);
            i--;
        }
        else{
            //remove the resource's node from the dependency nodes of each other node
            vector<Node*>& dependencyNodes = resources[i]->getDependencyNodes();
            for (int j = 0; j < dependencyNodes.size(); j++){
                if (dependencyNodes[j]->getResource() == resource){
                    dependencyNodes.erase(dependencyNodes.begin() + j);
                    break;
                }
            }
        }
    }
    
    delete correspondingNode;
}

/**
 * Creates a link between a resource and a dependency, creating nodes for either as necessary.
 * 
 * @param resource      the resource
 * @param dependency    the dependency
 * @param resources     the list of all resources
 */
void createLink(string resource, string dependency, vector<Node*>& resources){
    Node* newResource1, *newResource2;
    
    //the first resource
    for (int i = 0; i < resources.size(); i++){
        if (resources[i]->getResource() == resource){
            newResource1 = resources[i];
            goto resource1Exists;
        }
    }
    newResource1 = new Node(resource);
    for (int i = 0; i < resources.size(); i++){
        //if this is a new resource, I have to make sure to add it to the list of Dependency Nodes for all Nodes that depend on it
        //for when a node is deleted and then added back
        vector<string>& dependencyStrings = resources[i]->getDependencyStrings();
        for (int j = 0; j < dependencyStrings.size(); j++){
            if (dependencyStrings[j] == resource){
                resources[i]->getDependencyNodes().push_back(newResource1);
                break;
            }
        }
    }
    resources.push_back(newResource1);
resource1Exists:


    //the second resource
    for (int i = 0; i < resources.size(); i++){
        if (resources[i]->getResource() == dependency){
            newResource2 = resources[i];
            goto resource2Exists;
        }
    }
    newResource2 = new Node(dependency);
    for (int i = 0; i < resources.size(); i++){
        //if this is a new resource, I have to make sure to add it to the list of Dependency Nodes for all Nodes that depend on it
        //for when a node is deleted and then added back
        vector<string>& dependencyStrings = resources[i]->getDependencyStrings();
        for (int j = 0; j < dependencyStrings.size(); j++){
            if (dependencyStrings[j] == dependency){
                resources[i]->getDependencyNodes().push_back(newResource2);
                break;
            }
        }
    }
    resources.push_back(newResource2);
resource2Exists:
    newResource1->addDependency(newResource2);
}

/**
 * Save the resource graph to file
 * 
 * @param resources the list of all resources
 * @param file      the input file
 */
void saveResources(vector<Node*>& resources, fstream& file){
    file.close();
    file.open("resource.txt", fstream::out);
    for (int i = 0; i < resources.size(); i++){
        vector<string>& dependencyStrings = resources[i]->getDependencyStrings();
        for (int j = 0; j < dependencyStrings.size(); j++){
            //print each resource-dependency combination
            file << resources[i]->getResource() << " " << dependencyStrings[j];
            if (i < resources.size() - 1 || j < dependencyStrings.size() - 1) file << "\r\n";
        }
    }
}

/**
 * Delete the resources list, freeing its memory.
 * 
 * @param resources the list of all resources
 */
void deleteResources(vector<Node*>& resources){
    for (int i = 0; i < resources.size(); i++){
        delete resources[i];
    }
}