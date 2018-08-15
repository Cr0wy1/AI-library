#ifndef MATRIX_ITERATOR_H
#define MATRIX_ITERATOR_H
#include <matrix.h>



template<class T>
class iterator{
public:
    class Matrix &m;
    size_t Row;
    size_t Col;

    iterator(size_t _row, size_t _col, Matrix& _elem) : Row(_row), Col(_col), m(_elem){

    }

    T& operator*(){
        return m.values[Row][Col];
    }

    iterator &operator++(int){
        Col++;
        if(Col > m.Cols - 1 ){
            Row++;
            Col = 0;
        }
    }

    iterator &operator++(){
        Col++;
        if(Col > m.Cols - 1 ){
            Row++;
            Col = 0;
        }
    }

    inline bool operator !=(const iterator &other){
        return !(Row == other.Row && Col == other.Col);
    }
};

#endif // MATRIX_ITERATOR_H
