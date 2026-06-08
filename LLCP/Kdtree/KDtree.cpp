#include<iostream>
#include<bits/stdc++.h>
#include<cmath>

using namespace std;

class KDtree{
    private:
    struct Node{
        vector<double> point;

        Node* left;
        Node* right;

        Node(const vector<double> &p){
            point=p;
            left=nullptr;
            right=nullptr;
        }
    };

    Node* root;
    int k;

    //Build tree
    Node* build(vector<vector<double>> &points, int depth, int start, int end){
        if(start>end){
            return nullptr;
        }

        int axis=depth%k;

        int mid=(start + end)/2;

        sort(points.begin() + start, points.begin() + end + 1, [axis](const vector<double> &a, const vector<double> &b){
            return a[axis] < b[axis];
        });

        Node* node=new Node(points[mid]);

        node -> left=build(points, depth+1, start, mid-1);

        node -> right=build(points, depth + 1, mid + 1, end);

        return node;
    }

    //squared distance
    double distanceSquared(const vector<double> &a, const vector<double> &b){
        double sum=0;

        for(int i=0; i<k; i++){
            double diff=a[i] - b[i];
            sum+=diff * diff;
        }

        return sum;
    }

    //nearest neighbour search
    void nearestNeighbor(Node* node, const vector<double> & target, int depth, Node* &best, double &bestdist){
        if(node==nullptr) return;

        double currentdist=distanceSquared(node -> point, target);

        if(currentdist < bestdist){
            bestdist=currentdist;
            best=node;
        }

        int axis=depth % k;

        Node* nearbranch;
        Node* farbranch;

        if(target[axis] < node-> point[axis]){
            nearbranch=node->right;
            farbranch=node->left;
        }
        else{
            nearbranch=node->right;
            farbranch=node->left;
        }

        //search near side first
        nearestNeighbor(nearbranch, target, depth + 1, best, bestdist);

        //pruning check
        double planeDistance= target[axis] - node->point[axis];

        double planeDistanceSquared=planeDistance * planeDistance;

        if(planeDistanceSquared < bestdist){
            nearestNeighbor(farbranch, target, depth + 1, best, bestdist);
        }

    }
    
    //print tree
    void print(Node* node, int depth){
        if(node==nullptr) return;

        for(int i=0; i<depth; i++){
            cout<<" ";
        }
        cout<<"(";

        for(int i=0; i<k; i++){
            cout<<node->point[i];

            if(i != k-1){
                cout<<",";
            }
        }

        cout<<")"<<endl;

        print(node->left, depth+1);
        print(node->right, depth+1);
    }

    public:

    KDtree(int dimensions){
        k=dimensions;
        root=nullptr;
    }

    void buildTree(vector<vector<double>> &points){
        root=build(points, 0, 0, points.size()-1);
    }

    void findNearest(const vector<double> &target){
        Node* best=nullptr;

        double bestdist=numeric_limits<double>::max();

        nearestNeighbor(root, target, 0, best, bestdist);

        cout<<"\nNearest Point =(";

        for(int i=0; i<k; i++){
            cout<<best->point[i];

            if(i != k-1) cout<<",";
        }

        cout<<")"<<endl;
        cout<<"Distance = "<<sqrt(bestdist)<<endl;
    }

    void display(){
        cout<<"\nKD tree\n"<<endl;
        print(root, 0);
    }
};

int main(){
    int k;

    cout<<"Enter dimensions: ";
    cin>>k;

    int n;

    cout<<"Enter number of points: ";
    cin>>n;

    vector<vector<double>> points(n, vector<double> (k));

    cout<<"\nENter points:\n";

    for(int i=0; i<n; i++){
        for(int j=0; j<k; j++){
            cin>>points[i][j];
        }
    }

    KDtree tree(k);

    tree.buildTree(points);

    tree.display();

    vector<double> query(k);

    cout<<"\nEnter query point:\n";

    for(int i=0; i<k; i++){
        cin>>query[i];
    }

    tree.findNearest(query);

    return 0;
}
