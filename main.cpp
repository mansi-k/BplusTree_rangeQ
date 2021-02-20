#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
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
//    cout << "val" << val << endl;
//    cout << "child_node:" << "size:" << child_node->keys.size() << " ";
//    for(auto it=child_node->keys.begin();it!=child_node->keys.end();it++)
//        cout << *it <<"...";
//    cout << endl;
//    cout << "current_node:"  "size:" << (cur_node==NULL) << endl;
//    for(auto it=cur_node->keys.begin();it!=cur_node->keys.end();it++)
//        cout << *it <<"..." << endl;
//    cout << endl;
    if(cur_node->keys.size() < MAX_KEYS) {
        int i=0;
        while(i < cur_node->keys.size() && cur_node->keys[i] <= val) {
            i++;
        }
//        cout << "here" << val << endl;
        cur_node->keys.insert(cur_node->keys.begin()+i,val);
        cur_node->child_ptrs.insert(cur_node->child_ptrs.begin()+i+1,child_node);
        child_node->parent = cur_node;
    }
    else {
//        cout << "one" << endl;
        vector<ll> cur_keys = cur_node->keys;
        vector<Node*> cur_ptrs = cur_node->child_ptrs;
        int i=0;
        while(i < cur_keys.size() && cur_keys[i] <= val) {
            i++;
        }
        cur_keys.insert(cur_keys.begin()+i,val);
        cur_ptrs.insert(cur_ptrs.begin()+i+1,child_node);
        int n_left = (MAX_KEYS+1)/2;
        int n_right = MAX_KEYS+1 - n_left;
//        cout << "curparent:" << (!cur_node->parent) << endl;
        cur_node->keys = vector<ll>(cur_keys.begin(),cur_keys.begin()+n_left);
        cur_node->child_ptrs = vector<Node*>(cur_ptrs.begin(),cur_ptrs.begin()+n_left+1);
        for(int j=cur_node->child_ptrs.size();j<=MAX_KEYS+1;j++)
            cur_node->child_ptrs.push_back(nullptr);
        Node* new_nonleaf = new Node;
        new_nonleaf->leaf_flag = false;
        new_nonleaf->keys = vector<ll>(cur_keys.begin()+n_left+1,cur_keys.end());
        new_nonleaf->child_ptrs = vector<Node*>(cur_ptrs.begin()+n_left+1,cur_ptrs.end());
        for(int j=new_nonleaf->child_ptrs.size();j<=MAX_KEYS+1;j++)
            new_nonleaf->child_ptrs.push_back(nullptr);
        for(auto jt=cur_node->child_ptrs.begin();jt!=cur_node->child_ptrs.end() && *jt!=nullptr ;jt++) {
            (*jt)->parent = cur_node;
        }
        for(auto jt=new_nonleaf->child_ptrs.begin();jt!=new_nonleaf->child_ptrs.end() && *jt!=nullptr ;jt++) {
            (*jt)->parent = new_nonleaf;
        }
//        cout << "New:";
//        for(auto it=new_nonleaf->keys.begin();it!=new_nonleaf->keys.end();it++)
//            cout << *it <<"...";
//        cout << endl;
        if (cur_node == ROOT) {
            Node* new_root = new Node;
            new_root->keys.push_back(cur_keys[n_left]);
            new_root->child_ptrs[0] = cur_node;
            new_root->child_ptrs[1] = new_nonleaf;
            new_root->leaf_flag = false;
            ROOT = new_root;
            cur_node->parent = ROOT;
            new_nonleaf->parent = ROOT;
        }
        else {
            insertNonLeaf(cur_keys[n_left],cur_node->parent,new_nonleaf);
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
                if(cur_node->keys[i] >= val) {
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
            right_leaf->next = cur_node->next;
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

bool search(ll val) {
    if (!ROOT) {
        return false;
    }
    else {
        Node *cur_node = ROOT;
        while (!cur_node->leaf_flag) {
            for (int i=0; i<cur_node->keys.size(); i++) {
//                cout << cur_node->keys[i] << ",";
                if (cur_node->keys[i] == val) {
                    return true;
                }
                if (cur_node->keys[i] > val) {
                    cur_node = cur_node->child_ptrs[i];
                    break;
                }
                if (i == cur_node->keys.size() - 1) {
                    cur_node = cur_node->child_ptrs[i + 1];
                    break;
                }
            }
        }
//        cout << endl;
        if (!cur_node) {
            return false;
        }
        while (cur_node->next) {
            for (int i = 0; i < cur_node->keys.size() && val >= cur_node->keys[i]; i++) {
//                cout << cur_node->keys[i] << ",";
                if (cur_node->keys[i] == val) {
                    return true;
                }
            }
            cur_node = cur_node->next;
        }
        for (int i = 0; i < cur_node->keys.size() && val >= cur_node->keys[i]; i++) {
//            cout << cur_node->keys[i] << ",";
            if (cur_node->keys[i] == val) {
                return true;
            }
        }
//        cout << endl;
        return false;
    }
}

ll count(ll val) {
    if (!ROOT) {
        return false;
    }
    else {
        ll c = 0;
        Node *cur_node = ROOT;
        while (!cur_node->leaf_flag) {
            for (int i=0; i<cur_node->keys.size(); i++) {
//                cout << cur_node->keys[i] << ",";
                if (cur_node->keys[i] >= val) {
                    cur_node = cur_node->child_ptrs[i];
                    break;
                }
                if (i == cur_node->keys.size() - 1) {
                    cur_node = cur_node->child_ptrs[i + 1];
                    break;
                }
            }
        }
//        cout << endl;
        if (!cur_node) {
            return c;
        }
        while (cur_node->next) {
            for (int i=0; i<cur_node->keys.size() && val>=cur_node->keys[i]; i++) {
//                cout << cur_node->keys[i] << ",";
                if (cur_node->keys[i] == val) {
                    c++;
                }
            }
            cur_node = cur_node->next;
        }
        for (int i = 0; i < cur_node->keys.size() && val >= cur_node->keys[i]; i++) {
//            cout << cur_node->keys[i] << ",";
            if (cur_node->keys[i] == val) {
                c++;
            }
        }
//        cout << endl;
        return c;
    }
}

ll range(ll val1, ll val2) {
    if (!ROOT) {
        return false;
    }
    else {
        ll c = 0;
        Node *cur_node = ROOT;
        while (!cur_node->leaf_flag) {
            for (int i=0; i<cur_node->keys.size(); i++) {
//                cout << cur_node->keys[i] << ",";
                if (cur_node->keys[i] >= val1) {
                    cur_node = cur_node->child_ptrs[i];
                    break;
                }
                if (i == cur_node->keys.size() - 1) {
                    cur_node = cur_node->child_ptrs[i + 1];
                    break;
                }
            }
        }
//        cout << endl;
        if (!cur_node) {
            cout << "null" << endl;
            return c;
        }
        while (cur_node->next) {
            for (int i=0; i<cur_node->keys.size() && val2>=cur_node->keys[i]; i++) {
//                cout << cur_node->keys[i] << ",";
                if (val1 <= cur_node->keys[i])
                    c++;
            }
            cur_node = cur_node->next;
        }
        for (int i=0; i<cur_node->keys.size() && val2>=cur_node->keys[i]; i++) {
//            cout << cur_node->keys[i] << ",";
            if (val1 <= cur_node->keys[i])
                c++;
        }
//        cout << endl;
        return c;
    }
}

void print_tree(Node* root = ROOT) {
    if(root == NULL)
        return;
    if(!root->leaf_flag) {
        for(int i=0; i<root->child_ptrs.size(); i++)
            print_tree(root->child_ptrs[i]);
    }
    else
        cout << "Leaf:";
    cout << "|";
    for (int i=0; i<root->keys.size(); i++) {
        cout << root->keys[i] << "|";
    }
    cout << "\tP:|";
    if(root->parent) {
        for (auto it = root->parent->keys.begin(); it != root->parent->keys.end(); it++)
            cout << *it << "|";
    }
    else
        cout << "no";
    cout << endl;
}

void print_allkeys() {
    if (!ROOT) {
        cout << "Empty" << endl;
    }
    else {
        Node *cur_node = ROOT;
        while (!cur_node->leaf_flag) {
            cur_node = cur_node->child_ptrs[0];
        }
        while (cur_node->next) {
            for (int i=0; i<cur_node->keys.size(); i++) {
                cout << cur_node->keys[i] << " ";
            }
            cur_node = cur_node->next;
        }
        for (int i=0; i<cur_node->keys.size(); i++) {
            cout << cur_node->keys[i] << " ";
        }
        cout << endl;
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
    while(getline(in_file, line)) {
//        cout << line << endl;
        if(line.find("INSERT") != string::npos) {
            istringstream (line.substr(7)) >> x;
            insertLeaf(x);
//            print_allkeys();
        }
        else if(line.find("FIND") != string::npos) {
            istringstream(line.substr(5)) >> x;
            if(search(x))
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        else if(line.find("COUNT") != string::npos) {
            istringstream (line.substr(6)) >> x;
            cout << count(x) << endl;
        }
        else if(line.find("RANGE") != string::npos) {
            istringstream (line.substr(6)) >> x >> y;
            if(x>y)
                swap(x,y);
//            cout << x << "," << y << endl;
            cout << range(x, y) << endl;
        }
        else
            cout << "Invalid command : " << line;
//        cout << "*****************************" << endl;
    }
//    print_tree();
    return 0;
}
