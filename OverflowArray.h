//
// Created by egor0 on 4/27/2020.
//

#ifndef CPP_LIFE_OVERFLOWARRAY_H
#define CPP_LIFE_OVERFLOWARRAY_H
template<typename T>
class OverflowArray{
private:
    T* ptr;
    long int size;
public:
    T& operator[](long int index){
        if(index<0){
//            std::cout<<"UnderFlow "<< index <<std::endl;
            index = size + index;
            return operator[](index);
        }

        if(index>=size){
//            std::cout<<"Overflow "<< index <<std::endl;
            index = index - size;
            return operator[](index);
        }

        return ptr[index];
    }
    explicit OverflowArray(T* p = nullptr, int s = 0)
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
