#include <iostream>
using namespace std;

template <typename keytype>

class BHeap {
    private:
        struct Node{
            keytype key;
            int d;
            Node* child;//points to left most child
            Node* parent; //points to parent
            Node* siblings;// points to right child 
        };
        Node* min;
        Node* head;
    public:
        BHeap() {
            head = nullptr;
            min = nullptr;
        }

        ~BHeap() {
            deleteNode(head);
        }

        BHeap(const BHeap &obj) {
            Node* tmp = new Node();
            tmp->key = obj.head->key;
            tmp->d = obj.head->d;
            this->head = tmp;
            node_copy(head, obj.head);
            this->min = setMin(head);
        }

        void node_copy(Node *h, Node *cur) {
            if (cur == nullptr) return;

            if (cur->siblings != nullptr) {
                Node* tmp = new Node();
                tmp->key = cur->key;
                tmp->d = cur->d;
                h->siblings = tmp;
                node_copy(h->siblings, cur->siblings);
            }

            if (cur->child != nullptr) {
                Node* tmp = new Node();
                tmp->key = cur->key;
                tmp->d = cur->d;
                h->child = tmp;
                node_copy(h->child, cur->child);
            }
        }

        BHeap &operator=(const BHeap &cur) {
            Node* curHead = cur.head;
            if (this != &cur) {
                deleteNode(head);
                Node* tmp = new Node();
                tmp->key = curHead->key;
                tmp->d = curHead->d;
                head = tmp;
                node_copy(head, curHead);
                min = setMin(head);
            }
            return *this;
        }

        Node* setMin(Node* cur) {
            Node* curMin = cur;
            while (cur != nullptr) {
                if (cur->key <= curMin->key) curMin = cur;
                cur = cur->siblings;
            }
            return curMin;
        } 

        void deleteNode(Node* cur) {
            if (cur == nullptr) return;
            deleteNode(cur->siblings);
            deleteNode(cur->child);
            delete cur;
            return;
        }

        BHeap(keytype k[], int s) {
            head = nullptr;
            min = nullptr;
            for (int i = 0; i < s; ++i) insert(k[i]);
        }

        void insert(keytype k) {
            Node* curHead = make_heap();
            curHead->key = k;
            if (min == nullptr || min->key > curHead->key) {
                min = curHead;
            }
            if (head == nullptr) {
                head = curHead;
                return;
            }
            head = union_heap(head, curHead);
        }

        Node* heap_merge(Node* h1, Node* h2) {
            Node *curHead= nullptr;
            Node **pos = &curHead;
            while (h1 && h2) {
                if (h1->d > h2->d) {
                    *pos = h2;
                    h2 = h2->siblings;
                }
                else {
                    *pos = h1;
                    h1 = h1->siblings;
                }
                pos = &(*pos)->siblings;
            }
            if (h2) *pos = h2;
            else *pos = h1;

            return curHead;
        }

        Node* union_heap(Node* h1, Node* h2) { // merging two binomial heaps,
            Node* h = heap_merge(h1, h2);
            if (h == nullptr) return h;
            Node* prev = nullptr;
            Node* x = h;
            Node* next = x->siblings;
            while (next != nullptr) {
                if ((x->d != next->d) || ((next->siblings != nullptr) && next->siblings->d == x->d)) {
                    prev = x;
                    x = next;
                }
                else {
                    if (x->key <= next->key) {
                        x->siblings = next->siblings;
                        link(next, x);
                    }
                    else {
                        if (prev == nullptr) {
                            h = next;
                        }
                        else {
                            prev->siblings = next;
                        }
                        link(x, next);
                        x = next;
                    }
                }
                next = x->siblings;
            }
            return h;
            
        }

        Node* link(Node* a, Node* b) { //merging two btrees of the same degree 
            if (a == nullptr) return b;
            else if (b == nullptr) return a;
            else if (a->key < b->key) {
                b->parent = a;
                b->siblings = a->child;
                a->child = b;
                a->d = a->d + 1;
                return a;
            }
            else {
                a->parent = b;
                a->siblings = b->child;
                b->child = a;
                b->d = b->d + 1;
                return b;
            }
        }

        Node* make_heap() {
            Node* h = new Node();
            h->parent = nullptr;
            h->child = nullptr;
            h->siblings = nullptr;
            h->d = 0;
            return h;
        }

        keytype peekKey() {
            return min->key;
        }

        Node* reverse_list_children(Node* curChild) {
            //return a reverse list of these children, get to practice leetcode reverse linked list lol
            Node* prev = nullptr;
            Node* next = nullptr;
            while (curChild != nullptr) {
                next = curChild->siblings;
                curChild->siblings = prev;

                prev = curChild;
                curChild = next;
            }
            return prev;
        }

        keytype extractMin() {
            Node* oldMin = min;

            Node* h;
            Node* child = min->child;

            Node* tmp = head;
            //remove the heap that is attatched to min from the root list
            if (tmp->key == min->key) {
                head = min->siblings;
            }
            else {
                while (tmp != nullptr && tmp->siblings != min) tmp = tmp->siblings;
                if ((tmp->siblings)->siblings == nullptr) tmp->siblings = nullptr;
                else tmp->siblings = tmp->siblings->siblings;
            }
            h = reverse_list_children(child);
            /*
            cout << "Printing the reversed list of children" << endl;
            printTreeInternal(h);
            cout << "Ending the reversed list of children" << endl;
            */
            head = union_heap(head, h);
            //update new min from root list
            min = setMin(head);
            keytype c = oldMin->key;
            return c;
        }

        void merge(BHeap &H2) {
            Node* cur = H2.head;
            head = union_heap(head, cur);
            H2.head = nullptr;
            min = setMin(head);
        }

        void printKey() {
            Node* cur = head;
            while (cur != nullptr) {
                cout << "B" << cur->d << endl;
                print_tree(cur, true);
                cur = cur->siblings;
                cout << endl << endl;
            }
        }

        void print_tree(Node* cur, bool isRootList) { //input is the root of the tree we need to print
            if (cur == nullptr) return;
            cout << cur->key << " ";
            print_tree(cur->child, false);
            if (!isRootList) print_tree(cur->siblings, false);
        }
};