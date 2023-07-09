#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename T>

class CircularDynamicArray{
    private:
        int index(int i) {
            if (dir == false) return ((back-i)+cap)%cap;
            return (i+front+cap)%cap;
        }
    public:
        int cap;
        int size;
        int front, back;
        bool dir = true; //true means moving front to back, false is moving back to front
        T* a;
        T type;

    CircularDynamicArray <T>() {
        cap = 2;
        size = 0;
        a = new T[cap];
        front = 0; back = 1;
    }

    CircularDynamicArray <T>(int x) {
        cap = x;
        size = x;
        a = new T[cap];
        front = 0; back = x-1;
    }

    CircularDynamicArray <T>(CircularDynamicArray& x) {
        T* b = new T[x.cap];
        for (int i = 0; i < x.cap; ++i) {
            b[i] = x.a[i];
        }


        cap = x.cap;
        size = x.size;
        front = x.front;
        back = x.back;
        dir = x.dir;
        a = b;
    }

    CircularDynamicArray& operator=(const CircularDynamicArray& x)
    {
        if (this != &x) {
            T* b = new T[x.cap];
            for (int i = 0; i < x.cap; ++i) {
                b[i] = x.a[i];
            }
            delete[] a;

            cap = x.cap;
            size = x.size;
            front = x.front;
            back = x.back;
            dir = x.dir;
            a = b;
        }
        return *this;
    }


    ~CircularDynamicArray <T>(){
        delete[] a;
    }

    void reduce() {
        T* b = new T[cap/2];
        for (int i = 0; i < size; ++i) {
            b[i] = a[index(i)];
        }
        delete[] a;
        cap = cap/2;
        a = b;
        front = 0;
        back = size-1;
        dir = true;
    }

    void d() {
        T* b = new T[cap*2];
        for (int i = 0; i < cap; ++i) {
            b[i] = a[index(i)];
        }
        delete[] a;
        back = cap-1;
        cap *= 2;
        front = 0;
        a = b;
        dir = true;
    }

    void print() {
        for (int i = 0; i < size; ++i) {
            cout << a[index(i)] << " ";
        }
        cout << endl;
    }

    void printall() {
        for (int i = 0; i < cap; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;
    }

    T & operator [](int i){

        if (i > size) {
            cout << "Index out of bounds" << endl;
            return type;
        }
        return a[index(i)];
    }

    void addEnd(T v) {
        if (size == cap) {
            d();
        }
        size += 1;
        if (dir == false) {
            front -= 1;
            if (front < 0) front += cap;
            a[(front+cap)%cap] = v;
            return;
        }
        back += 1;
        a[(back+cap)%cap] = v;
        return;
    }

    void addFront(T v) {
        if (size == cap) {
            d();
        }
        size += 1;
        if (dir == false) {
            back += 1;
            a[(back+cap)%cap] = v;
            return;
        }
        if (front < 0) front += cap;
        front -= 1;
        a[(front+cap)%cap] = v;
        return;
    }

    void delEnd() {
        if (size <= 0) return;
        if (dir == false) front += 1;
        else back -= 1;

        size -= 1;
        if (back < 0) back += cap;
        if (size <= cap/4) reduce();
    }

    void delFront() {
        if (size <= 0) return;
        if (dir == false) back -= 1;
        else front += 1;
        size -= 1;
        if (back < 0) back += cap;
        if (size <= cap/4) reduce();
    }

    void clear() {
        cout << "clear" << endl;
        delete[] a;
        size = 0;
        cap = 2;
        front = back = 0;
        a = new T[cap];
    }
    int capacity() {
        return cap;
    }

    int length() {
        return size;
    }

    int linearSearch(T v) {
        for (int i = 0; i < size; ++i){
            if (this->operator[](i) == v) return i;
        }
        return -1;
    }

    int binSearch(T v) {
        int l = 0;
        int r = size-1;
        int mid;
        while (l <= r) {
            mid = (l+r)/2;
            if (a[mid] == v) {
                return mid;
            }
            else if(a[mid] > v) r = mid-1;
            else l = mid+1;
        }
        return -1;
    }
    
    void mergeSort(T l[], int i, int k) {
        int j;
        if (i < k) {
            j = (i + k) / 2;  // Find the midpoint in the partition
            
            // Recursively sort left and right partitions
            mergeSort(l, i, j);
            mergeSort(l, j + 1, k);
            
            // Merge left and right partition in sorted order
            merge(l, i, j, k);
        }
    }

    void merge(T numbers[], int i, int j, int k) {
        int mergedSize;                            // Size of merged partition
        int mergePos;                              // Position to insert merged number
        int leftPos;                               // Position of elements in left partition
        int rightPos;                              // Position of elements in right partition
        T* mergedNumbers;

        mergePos = 0;
        mergedSize = k - i + 1;
        leftPos = i;                               // Initialize left partition position
        rightPos = j + 1;                          // Initialize right partition position
        mergedNumbers = new T[mergedSize];       // Dynamically allocates temporary array
                                                    // for merged numbers
        
        // Add smallest element from left or right partition to merged numbers
        while (leftPos <= j && rightPos <= k) {
            if (numbers[leftPos] < numbers[rightPos]) {
                mergedNumbers[mergePos] = numbers[leftPos];
                ++leftPos;
            }
            else {
                mergedNumbers[mergePos] = numbers[rightPos];
                ++rightPos;
                
            }
            ++mergePos;
        }
        
        // If left partition is not empty, add remaining elements to merged numbers
        while (leftPos <= j) {
            mergedNumbers[mergePos] = numbers[leftPos];
            ++leftPos;
            ++mergePos;
        }
        
        // If right partition is not empty, add remaining elements to merged numbers
        while (rightPos <= k) {
            mergedNumbers[mergePos] = numbers[rightPos];
            ++rightPos;
            ++mergePos;
        }
        
        // Copy merge number back to numbers
        for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
            numbers[i + mergePos] = mergedNumbers[mergePos];
        }
        delete[] mergedNumbers;
    }

    void stableSort() {
        T arr[size];
        for (int i = 0; i < size; ++i) {
            arr[i] = this->operator[](i);
        }
        /*
        for (int i = 0; i < size; ++i) {
            cout << arr[i] << " ";
        }
        cout << "before sort" << endl;
        */
        mergeSort(arr, 0, size-1);

/*        for (int i = 0; i < size; ++i) {
            cout << arr[i] << " ";
        }
        cout << "after sort" << endl;
        */

        T* b = new T[cap];
        for (int i = 0; i < size; ++i) {
            b[i] = arr[i];
        }

        delete[] a;
        a = b;
        front = 0;
        back = size-1;
        dir = true;
    }

    T pivot(vector<T> arr){
        if (arr.empty()) return 0;
        return arr[arr.size()-1];
        //attempt at optmizing pivot stradegy
        /*
        if (arr.size() <= 5) return arr[arr.size()/2];
        int count = 0;
        vector<T> l;
        while (count <= arr.size()) {
            vector<T> cur;
            for (int i = 0; i<6; ++i) {
                count = count + i;
                if (count >= arr.size()) break;
                cur.push_back(arr[count]);
            }
            sort(cur.begin(), cur.end());
            
            l.push_back(cur[cur.size()/2]);

            cur.clear();
        }
        return Select(l, (1+arr.size()/5)/2);
        */
    }

    T Select(vector<T> b, int v) {
        T p = pivot(b);
        vector<T> L;
        vector<T> E;
        vector<T> G;
        for (int i = 0; i < b.size(); ++i) {
            if (b[i] < p) {
                L.push_back(b[i]);
            }
            else if (b[i] > p) {
                G.push_back(b[i]);
            }
            else E.push_back(b[i]);
        }
        if (v <= L.size()) {
            return Select(L, v);
        }
        else if (v <= L.size() + E.size()) {
            return p;
        }
        else return Select(G, (v-L.size()-E.size()));
    }

    T QuickSelect(int v) {
        if (v >= size || v < 0) return type;
        //chooses random pivot and returns vth smallest element in O(N) time wc
        vector<T> arr;
        for (int i = 0; i < size; ++i) {
            arr.push_back(this->operator[](i));
        }
        return Select(arr, v);
    }

    void reverse() {
        dir = !dir;
    }

 };
