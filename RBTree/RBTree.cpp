#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
using namespace std;

template <typename keytype, typename valuetype>

class RBTree{
        struct Node {
            Node *left, *right, *parent;
            bool color; //true == red, false == black
            keytype key;
            valuetype value;
            int size;

        Node(){
            color = true;
            size = 1;
        }
    };

    private:
        int count;
        Node *root;
        Node *nil;

    public:
    RBTree() {
        nil = new Node();
        nil->color = false;
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        nil->size = 0;
        root = nil;
        root->parent = nil;
        count = 0;
    }

    RBTree (RBTree &x) {
        count = x.count;
        nil = new Node();
        nil->color = false;
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = copyNode(x.root, nil, x);
    }

    RBTree& operator=(const RBTree& x) {
        if (this != &x) {
            clear(root);
            delete nil;
            nil = new Node();
            nil->color = false;
            nil->left = nil;
            nil->right = nil;
            nil->parent = nil;
            nil->size = 0;
            root = copyNode(x.root, nil, x);

            count = x.count;
        }
        return *this;
    }

    Node* copyNode(Node* x, Node* n, const RBTree& r) {
        if (x == r.nil || x == nullptr) return n;

        Node* z = new Node();

        z->left = copyNode(x->left, n, r);

        if (z->left != n) z->left->parent = z;

        z->right = copyNode(x->right, n, r);
        if (z->left != n) z->right->parent = z;

        z->value = x->value;
        z->size = x->size;
        z->key = x->key;
        z->color = x->color;

        return z;
    }

    RBTree(keytype k[], valuetype V[], int s) {
        nil = new Node();
        nil->color = false;
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        nil->size = 0;
        root = nil;
        root->parent = nil;
        bool inorder = true;
        for (int i = 1; i < s; i++) {
            if (k[i-1] > k[i]) {
                inorder = false;
                break;
            }
        }
        if (inorder == true) {
            //do special kind of insert for optimized time
            root = insertSpecial(k, V, 0, log2(s), 0, s-1);
            count = s;
        }
        else { // standard insert after insert
            for (int i = 0; i < s; i++) {
                insert(k[i], V[i]);
                count = s;
            }
        }
    }
    
    Node* insertSpecial(keytype k[], valuetype V[], int curLev, int prevLev, int l, int r) {
        int s = r-l;
        int g = floor(s/2)+l;
        if (r==l) g = r;
        else if (s<=0) return nil;

        Node* cur = new Node();
        cur->key = k[g];
        cur->value = V[g];
        cur->size = 1;

        if (curLev == prevLev) cur->color = true;
        else cur->color = false;

        cur->right = insertSpecial(k, V, curLev+1, prevLev, g+1, r);
        if (cur->right !=nil) cur->right->parent = cur;

        cur->left = insertSpecial(k, V, curLev+1, prevLev, l, g-1); 
        if (cur->left !=nil) cur->left->parent = cur;

        cur->size = cur->size + cur->right->size + cur->left->size;

        return cur;
    }

    void clear (Node *z) {
        if (z==nullptr||z==NULL||z==nil) return;
        //cout << "destorying node:" << z->key << endl;
        if (z->left != nil) clear(z->left);
        if (z->right != nil) clear(z->right);
        delete z;
    }

    ~RBTree() {
        //cout << "destorying tree" << endl;
        clear(root);
        delete nil;
    }

    valuetype * search(keytype k) {
        Node *cur = searchPointer(k);
        valuetype *res;
        res = &cur->value;
        return res;
    }

    void insertNode(Node *cur) {
        Node *y = nil;
        Node *x;
        x = root;
        while (x != nil) {
            x->size = x->size+1;
            y = x;
            if (cur->key < x->key) x = x->left;
            else x = x->right;
        }
        cur->parent = y;
        if (y == nil) root = cur;
        else if (cur->key < y->key) {
            y->left = cur;
        }
        else y->right = cur;
        cur->left = nil;
        cur->right = nil;
        cur->color = true;
        insertFixup(cur);
    }

    void insertFixup (Node *cur) {
        while (cur->parent != nil && cur->parent->color == true) {
            if (cur->parent == cur->parent->parent->left) {
                Node *y = cur->parent->parent->right;
                if (y->color == true) {
                    cur->parent->color = false;
                    y->color = false;
                    cur->parent->parent->color = true;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->right) {
                        cur = cur->parent;
                        leftRotate(cur);
                        //left rotate
                    }
                cur->parent->color = false;
                cur->parent->parent->color = true;
                rightRotate(cur->parent->parent);
                }
                //right rotate
             }
             else {
                Node *y = cur->parent->parent->left;
                if (y->color == true) {
                    cur->parent->color = false;
                    y->color = false;
                    cur->parent->parent->color = true;
                    cur = cur->parent->parent;
                }
                else {
                    if (cur == cur->parent->left) {
                        cur = cur->parent;
                        rightRotate(cur);
                        //left rotate
                    }
                cur->parent->color = false;
                cur->parent->parent->color = true;
                leftRotate(cur->parent->parent);
                }
                //right rotate
                if (cur == root) break;
             }
        }
        root->color = false;
    }

    void leftRotate(Node *cur) {
        Node *y = cur->right;
        cur->right = y->left;
        if (y->left != nil) y->left->parent = cur;

        y->parent = cur->parent;
        if (cur->parent == nil) root = y;

        else if (cur == cur->parent->left) {
            cur->parent->left = y;
        }

        else {
            cur->parent->right = y;
        }

        y->left = cur;
        cur->parent = y;

        y->size = cur->size;
        cur->size = cur->left->size + cur->right->size + 1;
    }

    void rightRotate(Node *cur) {
        Node *y = cur->left;
        cur->left = y->right;
        if (y->right != nil) y->right->parent = cur;

        y->parent = cur->parent;
        if (cur->parent == nil) root = y;

        else if (cur == cur->parent->right) cur->parent->right = y;

        else cur->parent->left = y;
        
        y->right = cur;
        cur->parent = y;

        y->size = cur->size;
        cur->size = cur->left->size + cur->right->size + 1;
    }


    void insert(keytype k, valuetype v) {
        Node *z = new Node();
        z->key = k;
        z->value = v;
        z->color = true;
        count++;
        insertNode(z);
        root->parent=nil;
    }

    void updateNodeCountInsert(Node* x) {
        while (x != nil) {
            x->size = x->size + 1;
            x = x->parent;
        }
        return;
    }
    
    int remove(keytype k) {
        root->parent = nil;
        Node *j = searchPointer(k);
        if (j == nil || j == nullptr) return 0;
        updateSizeDelete(j);
        deleteNode(j);
        delete j;
        count--;
        root->parent=nil;
        return 1;
    }

    void updateSizeDelete(Node *x) {
        while (x != nil) {
            x->size = x->size - 1;
            x = x->parent;
        }
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        }

        else if (u == u->parent->left) {
            u->parent->left = v;
            //u->parent->size = u->parent->size - u->size + v->size;
        }
        else {
            u->parent->right = v;
            //u->parent->size = u->parent->size - u->size + v->size;
        }
        v->parent = u->parent;
        //u->size = u->size - v->size;
    }

    Node* treeMin(Node *x) {
        while (x->left != nil) {
            x = x->left;
        }
        return x;
    }

    void deleteNode(Node* z) {
        Node* y = z;
        Node *x;
        bool origColor = y->color;
        if (z->left == nil) {

            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = predecessorPointerRemove(z);
            origColor = y->color;
            x = y->left;
            if (y->parent == z) x->parent = y;
            else {
                transplant(y, y->left);
                y->left = z->left;
                y->left->parent = y;
            }
            transplant(z, y);
            y->right = z->right;
            y->right->parent = y;
            y->color = z->color;
        }
        if (origColor == false) deleteFixup(x);
    }

    void deleteFixup(Node* x) {
        
        while (x != root && x->color == false) {
            if (x == x->parent->left) {
                Node *w = x->parent->right;
                if (w->color == true) {
                    w->color = false;
                    x->parent->color = true;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == false && w->right->color == false) {
                    w->color = true;
                    x = x->parent;
                }
                else {
                    if (w->right->color == false) {
                    w->left->color = false;
                    w->color = true;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = false;
                w->right->color = false;
                leftRotate(x->parent);
                x = root;
                }
            }
            else {
                Node *w = x->parent->left;
                if (w->color == true) {
                    w->color = false;
                    x->parent->color = true;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == false && w->left->color == false) {
                    w->color = true;
                    x = x->parent;
                }
                else {
                    if (w->left->color == false) {
                    w->right->color = false;
                    w->color = true;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = false;
                w->left->color = false;
                rightRotate(x->parent);
                x = root;
                }
            }
        }
        x->color = false;
        root->parent = nil;
    }
    
    int countNodes(Node* x) {
        if (x == nil) {
            return 0;
        }
        return 1 + countNodes(x->left) + countNodes(x->right);
    }

    int findRank(Node* x) {
        int r = x->left->size+1;
        Node *y = x;
        while (y != root) {
            if (y==y->parent->right) {
                r = r + y->parent->left->size+1;
            }
            y = y->parent;
        }
        return r;
    }

    Node* searchPointer(keytype k) {
        Node *cur = root;
        while (cur != nil) {
            if (cur->key == k) {
                return cur;
            }
            if (cur->key < k) {
                cur = cur->right;
            }
            else {
                cur = cur->left;
            }
        }
        return nil;
    }

    int rank(keytype k) {
        Node* c = searchPointer(k);
        if (c == nil) return 0;
        return findRank(c);
    }

    keytype select(int m) {
        Node* c = selectK(root, m);
        return c->key;
    }

    Node* selectK(Node* x, int i) {
        int r = x->left->size + 1;
        if (r == i) {
            return x;
        }
        else if (i < r) return selectK(x->left, i);
        else return selectK(x->right, i-r);
    }

    keytype* successor(keytype k) {
        root->parent = nil;
        keytype *r = NULL;
        Node *cur = searchPointer(k);
        if (cur == nil) return r;
        if (cur->right != nil) {
            cur = cur->right;

            while (cur->left != nil) {
                cur = cur->left;
            }
            if (cur == nil) return r;           
            r = &cur->key;
            return r;
        }
        else {
            Node* p = cur->parent;
            while (p!=nil){
                if (p->right && cur != p->right) break;
                if (p == nil) return r;
                cur = p;
                p = p->parent;
            }
            if (p == nil) return r;
            r = &p->key;
            }
        return r;
    }

    keytype* predecessor(keytype k) {
        root->parent = nil;
        keytype *r = NULL;
        Node *cur = searchPointer(k);
        if (cur == nil) return r;
        if (cur->left != nil) {
            cur = cur->left;

            while (cur->right != nil) {
                cur = cur->right;
            }
            if (cur == nil) return r;           
            r = &cur->key;
            return r;
        }
        else {
            Node* p = cur->parent;
            while (p!=nil){
                if (p->left && cur != p->left) break;
                if (p == nil) return r;
                cur = p;
                p = p->parent;
            }
            if (p == nil) return r;
            r = &p->key;
            }
            return r;
    }

   Node *predecessorPointerRemove(Node* cur) {
        int s = cur->size;
        root->parent = nil;
        if (cur == nil) return nil;
        if (cur->left != nil) {
            cur = cur->left;

            while (cur->right != nil) {
                cur->size = cur->size-1;
                cur = cur->right;
            }
            if (cur == nil) return nil;            
            cur->size = s;
            return cur;
        }
        else {
            return nil;
            Node* p = cur->parent;
            while (cur!= nil && p != nil && cur == p->left) {
                cur = p;
                p = p->parent;
            }
            if (p == nil) return nil;      
            p->size = s;        
            return p;
            }
            return nil;
    }

    int size() {
        return count;
    }

    // ALL PRINTING FUNCTIONS

    void printPreOrder(Node *cur) {
        if (cur == nil || cur==nullptr) return;
        cout << cur->key << " ";
        printPreOrder(cur->left);
        printPreOrder(cur->right);
    }
    void printPostOrder(Node *cur) {
        if (cur == nil) return;
        printPostOrder(cur->left);
        printPostOrder(cur->right);
        cout << cur->key << " ";
    }
    void printInOrder(Node *cur) {
        if (cur == nil || cur == NULL) return;
        printInOrder(cur->left);
        cout << cur->key << " ";
        printInOrder(cur->right);
    }

    void preorder() {
        printPreOrder(root);
        cout << endl;
    }

    void inorder() {
        printInOrder(root);
        cout << endl;
    }

    void postorder() {
        printPostOrder(root);
        cout << endl;
    }
    valuetype* getRootVal() {return &root->value;}
    Node* getRoot() {return root;}
};