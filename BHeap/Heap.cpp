#include <iostream>
#include <vector>
using namespace std;

template <typename keytype>

class Heap {
    private:
        vector<keytype> list;
    public:
    
        Heap() {}
        ~Heap() {}

        Heap& operator=(const Heap& x) {
            if (this != &x){
                list = x.list;
            }
            return *this;
        }

        Heap (const Heap& x) {
            list = x.list;
        }

        Heap(keytype K[], int s) {
            for (int i = 0; i < s; ++i) {
                list.push_back(K[i]);
            }
            for (int i = s; i >= 0; i--) {
                heapify(i);
            }
        }
        
        keytype peekKey() {
            return list[0];
        }

        keytype extractMin() {
            keytype minKey = list[0];
            list[0] = list[list.size()-1];
            list.pop_back();
            heapify(0);
            return minKey;
        }

        void insert(keytype k) {
            list.push_back(k);
            int i = list.size()-1;
            while (i > 0 && (list[(i - 1)/2] > list[i])) {
                swap(i, (i-1)/2);
                i = (i - 1)/2;
            }
        }

        void swap (int c, int k) {
            keytype tmp = list[c];
            list[c] = list[k];
            list[k] = tmp;
        }

        void heapify(int i) {
            int s = list.size();
            int l = (2*i)+1;
            int r = (2*i)+2;
            int smallest = i;
            if (l < s && list[l] < list[i]) smallest = l;
            if (r < s && list[r] < list[smallest]) smallest = r;
            if (smallest != i) {
                swap(i, smallest);
                heapify(smallest);
            }
        }

        void printKey() {
            for (int i = 0; i < list.size(); ++i) {
                cout << list[i] << " ";
            }
            cout << endl;
        }

};