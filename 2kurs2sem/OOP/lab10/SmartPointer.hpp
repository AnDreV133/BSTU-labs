#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

template<typename T>
class SmartPointer {
    T *obj;
public:
    SmartPointer() {}
    SmartPointer(T *obj) {
        this->obj = obj;
    }
    ~SmartPointer() {
        delete obj;
    }
    T* operator->() { return obj; }
    T& operator* () { return *obj; }
};

#endif // SMARTPOINTER_H
