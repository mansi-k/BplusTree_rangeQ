#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define MAX_KEYS 2
#define ll long long int

typedef struct Node{
    Node* parent;
    vector<ll> keys;
    vector<Node*> child_ptrs = vector<Node*>(MAX_KEYS+1,nullptr);
    bool leaf_flag = true;
    Node* next = NULL;
} Node;

Node* ROOT = nullptr;

void insertNonLeaf(ll val, Node* cur_node, Node* child_node) {
    if(cur_node->keys.size() < MAX_KEYS) {
        int i=0;
        while (i < cur_node->keys.size() && cur_node->keys[i] < val) {
            i++;
        }
        cur_node->keys.insert(cur_node->keys.begin()+i,val);
        cur_node->child_ptrs.insert(cur_node->child_ptrs.begin()+i+1,child_node);
        cur_node->child_ptrs.pop_back(); // popping last nullptr
    }
    else {
        vector<ll> cur_keys = cur_node->keys;
        vector<Node*> cur_ptrs = cur_node->child_ptrs;
        int i=0;
        while (i < cur_keys.size() && cur_keys[i] < val) {
            i++;
        }
        cur_keys.insert(cur_keys.begin()+i,val);
        cur_ptrs.insert(cur_ptrs.begin()+i+1,child_node);
        int n_left = (MAX_KEYS+1)/2;
        int n_right = MAX_KEYS+1 - n_left;
        cur_node->keys = vector<ll>(cur_keys.begin(),cur_keys.begin()+n_left);
        cur_node->child_ptrs = vector<Node*>(cur_ptrs.begin(),cur_ptrs.begin()+n_left);
        cur_node->child_ptrs.push_back(nullptr);
        Node* new_nonleaf = new Node;
        new_nonleaf->leaf_flag = false;
        new_nonleaf->keys = vector<ll>(cur_keys.begin()+n_left,cur_keys.end());
        new_nonleaf->child_ptrs = vector<Node*>(cur_ptrs.begin()+n_left,cur_ptrs.end());
        if (cur_node == ROOT) {
            Node* new_root = new Node;
            new_root->keys.push_back(new_nonleaf->keys[0]);
            new_root->child_ptrs[0] = cur_node;
            new_root->child_ptrs[1] = new_nonleaf;
            new_root->leaf_flag = false;
            ROOT = new_root;
            cur_node->parent = ROOT;
            new_nonleaf->parent = ROOT;
        }
        else {
            insertNonLeaf(new_nonleaf->keys[0],cur_node->parent,new_nonleaf);
        }
    }
}

void insertLeaf(ll val) {
    if (!ROOT) {
        ROOT = new Node;
        ROOT->keys.push_back(val);
    }
    else {
        Node* cur_node=ROOT;
        Node* parent;
        while (!cur_node->leaf_flag) {
            parent = cur_node;
            for(int i=0;i<cur_node->keys.size();i++) {
                if(cur_node->keys[i] > val) {
                    cur_node = cur_node->child_ptrs[i];
                    break;
                }
                if (i == cur_node->keys.size()-1) {
                    cur_node = cur_node->child_ptrs[i+1];
                    break;
                }
            }
        }
        if(cur_node->keys.size() < MAX_KEYS) {
            int i=0;
            while (i < cur_node->keys.size() && cur_node->keys[i] < val) {
                i++;
            }
            cur_node->keys.insert(cur_node->keys.begin()+i,val);
        }
        else {
            vector<ll> cur_keys = cur_node->keys;
            int i=0;
            while (i < cur_keys.size() && cur_keys[i] < val) {
                i++;
            }
            cur_keys.insert(cur_keys.begin()+i,val);
            int n_left = (MAX_KEYS+1)/2;
            int n_right = MAX_KEYS+1 - n_left;
            cur_node->keys = vector<ll>(cur_keys.begin(),cur_keys.begin()+n_left);
            Node* right_leaf = new Node;
            right_leaf->keys = vector<ll>(cur_keys.begin()+n_left,cur_keys.end());
            cur_node->next = right_leaf;
            if(cur_node == ROOT) {
                Node* new_root = new Node;
                new_root->keys.push_back(right_leaf->keys[0]);
                new_root->child_ptrs[0] = cur_node;
                new_root->child_ptrs[1] = right_leaf;
                new_root->leaf_flag = false;
                ROOT = new_root;
                cur_node->parent = ROOT;
                right_leaf->parent = ROOT;
            }
            else {
                insertNonLeaf(right_leaf->keys[0], parent, right_leaf);
            }
        }
    }
}


int main(int argc, char const *argv[]) {
    if(argc < 2) {
        cout << "Provide input file" << endl;
        return 0;
    }
    string input_file = string(argv[1]);
    ROOT = new Node;
    ifstream in_file(input_file);
    string line;
    ll x, y;
    while(getline(in_file, line))
    {
        if(line.find("INSERT") != string::npos)
        {
            istringstream (line.substr(7)) >> x;
            insertLeaf(x);
        }
//        else if(line.find("FIND") != string::npos)
//        {
//            istringstream (line.substr(5)) >> x;
//            if(find(lookup(root, x, false), x))
//                cout << "YES" << endl;
//            else
//                cout << "NO" << endl;
//        }
//        else if(line.find("COUNT") != string::npos)
//        {
//            istringstream (line.substr(6)) >> x;
//            cout << count(lookup(root, x, false), x) << endl;
//        }
//        else if(line.find("RANGE") != string::npos)
//        {
//            istringstream (line.substr(6)) >> x >> y;
//            range(lookup(root, x, false), x, y);
//        }
        else
            cout << "Invalid command : " << line;
    }
    return 0;
}