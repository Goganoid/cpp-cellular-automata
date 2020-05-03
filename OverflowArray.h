//
// Created by egor0 on 4/27/2020.
//

#ifndef CPP_LIFE_OVERFLOWARRAY_H
#define CPP_LIFE_OVERFLOWARRAY_H
template<typename T>
class OverflowArray{
private:
    T* ptr;
    int size;
public:
    T& operator[](int index){
        if(index<0){
            index = size + index;
            return operator[](index);
        }

        if(index>=size){
            index = index - size;
        }

        return ptr[index];
    }
    OverflowArray(T* p = nullptr, int s = 0)
    {
        size = s;
        ptr = nullptr;
        if (s != 0) {
            ptr = new T[s];
            if(p!=nullptr) {
                for (int i = 0; i < s; i++)
                    ptr[i] = p[i];
            }
        }
    }
    void Init(T* p = nullptr, int s = 0){
        size = s;
        ptr = nullptr;
        if (s != 0) {
            ptr = new T[s];
            if(p!=nullptr) {
                for (int i = 0; i < s; i++)
                    ptr[i] = p[i];
            }
        }
}

};
#endif //CPP_LIFE_OVERFLOWARRAY_H
