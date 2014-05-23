#ifndef NODE_H
#define	NODE_H

#include <vector>
#include <string>

using namespace std;

/**
 * Creates a node for a single Resource in the Resource Manager.
 * 
 * @author Andrew
 */
class Node{
    public:
        Node(string res);
        
        string getResource() const;
        vector<Node*>& getDependencyNodes();
        vector<string>& getDependencyStrings();
        
        void addDependency(Node* dependency);
        
    private:
        string resource;
        vector<Node*> dependencyNodes;
        vector<string> dependencyStrings;
};

#endif	/* NODE_H */
