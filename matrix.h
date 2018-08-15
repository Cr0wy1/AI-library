#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>
#include <functional>
#include <string>
#include <grid.h>
#include <QDebug>
#include <QString>

template<class T>
class Grid;


template<class T>
class Matrix: public Grid<T> {

public:

    //Constructors
    Matrix():Grid<T>(){}
    Matrix(size_t _rows, size_t _cols) : Grid<T>(_rows, _cols){}
    Matrix(std::initializer_list< std::initializer_list<T> > il2d) : Grid<T>(il2d){}

    //Destructor
    ~Matrix(){}

    //Copy Constructor
    template<class T2>
    Matrix(const Matrix<T2> &other) : Grid<T>(other){}

    void Setvalues(std::initializer_list< std::initializer_list<T> > list){
        auto inner_list = list.begin();
        auto value = inner_list->begin();
        for(auto it = this->begin(); it != this->end();it++){
            if(it.Col == 0){
                value = inner_list->begin();
                inner_list++;
            }
            it = *value;
            value++;
        }
    }


    void ARange(T start, T interval = 0){
        if(interval != 0){
            for(auto it = this->begin(); it != this->end();it++){
                it = start;
                start += interval;
            }
        }else{
            std::iota(this->begin(), this->end(), start);
        }

    }

    void Random(T Start, T End){
        for(auto it = this->begin(); it != this->end();it++){
            it = fmod(rand(), End-Start) + End;
        }
    }

    //OPERATOR OVERLOADING
    template<class T2>
    Matrix<T> operator +(const Matrix<T2> &other){
        if(this->Rows != other.Rows|| this->Cols != other.Cols){
            qDebug() << "ERROR: Matrix::operator+=, Size not Equal!";
            throw std::bad_array_new_length();
        }
        return AddSub(other, std::plus<T>());
    }

    template<class T2>
    Matrix<T> &operator+=(const Matrix<T2> &other){
        if(this->Rows != other.Rows|| this->Cols != other.Cols){
            qDebug() << "ERROR: Matrix::operator+=, Size not Equal!";
            throw std::bad_array_new_length();
        }
        *this = AddSub(other, std::plus<T>());
        return *this;
    }

    template<class T2>
    Matrix<T> operator -(const Matrix<T2> &other){
        return AddSub(other, std::minus<T>());
    }

    template<class T2>
    Matrix<T> operator* (const Matrix<T2> &other){
        if(this->Cols != other.rows()){
            qDebug("WARNING: Matrix have not the same size for multiplication!");
            throw;
        }
        Matrix<T> m = Matrix<T>(this->Rows, other.cols());

        //qDebug() << "T: " << typeid(T).name() << "T2: " << typeid(T2).name();
        for(auto it = m.begin(); it != m.end();it++){
            for(size_t c = 0; c < this->Cols;c++){
                *it += this->values[it.Row*this->Cols+c] * other.GetValue(c, it.Col);
            }
        }

        return m;
    }

    template<class T2>
    Matrix operator* (T2 number){
        Matrix m = Matrix(this->Rows, this->Cols);
        for(auto it = this->begin(); it != this->end();it++){
            m[it.Row][it.Col] = *it * number;
        }
        return m;
    }

    Matrix &operator =(const Matrix &other){
        Grid<T>::operator =(other);
        return *this;
    }

    template<class T2>
    Matrix &operator =(const Matrix<T2> &other){
        Grid<T>::operator =(other);
        return *this;
    }

    template<class T2, class OPERATOR>
    Matrix AddSub(const Matrix<T2> &other, OPERATOR op){
        if(this->Rows != other.Rows || this->Cols != other.Cols){
            qDebug("WARNING: Matrix have not the same size for addition!");
            throw;
        }

        Matrix m = Matrix(this->Rows, this->Cols);
        for(auto it = this->begin(); it != this->end();it++){
            m[it.Row][it.Col] = op(*it, other.GetValue(it.Row, it.Col));
        }
        return m;
    }


    void OneValues(){
        for(auto it = this->begin(); it != this->end();it++){
            *it = 1;
        }
    }

    bool isSquare(){ return this->Rows == this->Cols;}

    Matrix<T> GetUnitMatrix(){
        if(isSquare()){
            Matrix<T> UM(this->Rows, this->Cols);
            for(size_t i = 0; i < this->Rows; i++){
                UM[i][i] = 1;
            }
            return UM;
        }
        qDebug() << "WARNING: Matrix GetUnitMatrix()"<<this->Rows<<"x"<<this->Cols<<": Matrix is not squared";
    }

    T Sum(){
        T sum = 0;
        for(auto it = this->begin(); it != this->end();it++){
            sum += *it;
        }
        return sum;
    }

    void RowMultiply(size_t row_index, T factor){
        for(size_t c = 0; c < this->Cols; c++){
            this->values[row_index*this->Cols+c] *= factor;
        }
    }

    void RowDivide(size_t row_index, T factor){
        if(factor != 0){
            for(size_t c = 0; c < this->Cols; c++){
                this->values[row_index*this->Cols+c] /= factor;
            }
        }
    }


    void RowAdd(size_t row_index, T factor){
        for(size_t c = 0; c < this->Cols; c++){
            this->values[row_index*this->Cols+c] += factor;
        }
    }

    void RowSubtract(size_t row_index, T factor){
        for(size_t c = 0; c < this->Cols; c++){
            this->values[row_index*this->Cols+c] -= factor;
        }
    }

    void SwitchRows(size_t r1, size_t r2){
        for(size_t c = 0; c < this->Cols;c++){
            T temp;
            temp = this->values[r1*this->Cols+c];
            this->values[r1*this->Cols+c] = this->values[r2*this->Cols+c];
            this->values[r2*this->Cols+c] = temp;
        }
    }

    void SwitchCols(size_t c1, size_t c2){
        for(size_t r = 0; r < this->Rows;r++){
            T temp;
            temp = this->values[r*this->Cols+c1];
            this->values[r*this->Cols+c1] = this->values[r*this->Cols+c2];
            this->values[r*this->Cols+c2] = temp;
        }
    }

    T determinant3x3(){
        T det = 0;

        for(size_t line = 0; line < 6;line++){
            T value = 1;
            for(size_t rc = 0; rc < 3;rc++){
                int col = (rc+line) % 3;
                col = line > 2 ? 2 - col : col;

                value *= this->values[rc*this->Cols+col];
            }
            det = line < 3 ? det + value : det - value;
        }

        return det;
    }

    //Turn maybe in a better algorithm
    T determinant(){

        if(isSquare()){
            T Det = 0;
            if(this->Rows == 3) return this->determinant3x3();
            else{

                for(int r = 0; r < this->Rows; r++){
                    Matrix<T> detMrc(this->Rows-1, this->Cols-1);
                    auto itdet = detMrc.begin();
                    for(auto it = this->begin(); it != this->end();it++){
                        if(it.Row != r && it.Col != 0){
                            *itdet = *it;
                            itdet++;
                        }
                    }
                    Det += detMrc.determinant() * (powf((-1), r) * this->values[r*this->Cols]);
                }
            }
            return Det;
        }
        qDebug() << "Matrix determinante(): Is not Squared!";
        throw;
    }

    Matrix<T> transpose(){
        Matrix<T> transposedMatrix(this->cols(), this->rows());
        for(auto it = this->begin(); it != this->end();it++){
            transposedMatrix[it.Col][it.Row] = *it;
        }
        return transposedMatrix;
    }

    Matrix<T> inverse(){
        if(isSquare()){
            Matrix<T> TempM = *this;
            Matrix<T> IM = this->GetUnitMatrix();

            for(size_t rc = 0; rc < this->Rows; rc++){
                //Calculate KeyElement
                if(TempM[rc][rc] == -1){
                    TempM.RowMultiply(rc, -1);
                    IM.RowMultiply(rc, -1);
                }else if(TempM[rc][rc] != 1){
                    //Check if 1 exist in Column
                    bool bisOneInCol = false;
                    size_t RowIndexOfOne;
                    for(size_t r_inner = 0; r_inner < IM.rows();r_inner++){
                        if(rc != r_inner){
                            if(TempM[r_inner][rc] == 1 || TempM[r_inner][rc] == -1){
                                bisOneInCol = true;
                                RowIndexOfOne = r_inner;
                                break;
                            }
                        }
                    }

                    if(bisOneInCol){
                        TempM.SwitchRows(rc, RowIndexOfOne);
                        IM.SwitchRows(rc, RowIndexOfOne);
                        if(TempM[rc][rc] < 0){
                            TempM.RowMultiply(rc, -1);
                            IM.RowMultiply(rc, -1);
                        }
                    }else{
                        T rowfactor = TempM[rc][rc];
                        TempM.RowDivide(rc, rowfactor);
                        IM.RowDivide(rc, rowfactor);
                    }
                 }
                TempM.eliminateCol(rc, rc, false, &IM);
            }

            return IM;
        }
        qDebug() << "Matrix inverse(): Is not Squared!";
        throw;
    }

    size_t Rank(){
        size_t Rank = this->Rows;
        Matrix<T> TempM = *this;
        for(int rc = 0; rc < this->Rows; rc++){
            //Calculate KeyElement
            if(TempM[rc][rc] != 1 && TempM[rc][rc] != 0){
                TempM.RowDivide(rc, TempM[rc][rc]);
            }
            else if(TempM[rc][rc] == 0){

                int RowIndexOfAny;
                bool bisAnyInCol = TempM.IsAnyNumberInCol(rc, RowIndexOfAny, rc+1);

                if(bisAnyInCol) TempM.SwitchRows(rc, RowIndexOfAny);
                else Rank--;
             }


            //Eliminate Col
            if(TempM[rc][rc] == 1){
                TempM.eliminateCol(rc, rc, true);
            }
        }
        return Rank;
    }

    void eliminateCol(size_t main_row_i, size_t col_i, bool bOnlyUnder = false, Matrix *Use_Operations = nullptr){
        if(this->values[main_row_i*this->Cols+col_i] != 1){
            qDebug() << "ERROR: Main row Index must have the value of 1!";
            throw;
        }
        for(int r = bOnlyUnder ? main_row_i:0 ; r < this->Rows; r++){
            if(this->values[r*this->Cols+col_i] != 0 && r != main_row_i){
                T rowFactor = this->values[r*this->Cols+col_i];
                for(int c = 0; c < this->Cols; c++){
                    this->values[r*this->Cols+c] -= rowFactor * this->values[main_row_i*this->Cols+c];
                    if(Use_Operations){
                        Use_Operations->GetValue(r,c) -= rowFactor * Use_Operations->GetValue(main_row_i, c);
                    }
                }
            }
        }
    }

    bool IsAnyNumberInCol(size_t col_i, int &OutRowIndex = -1, size_t start_row_i = 0, int end_row_i = -1 ){
        end_row_i = end_row_i < 0 ? this->Rows-1 : end_row_i;
        for(int r = start_row_i; r <= end_row_i;r++){
            if(this->values[r*this->Cols+col_i] != 0){
                OutRowIndex = r;
                return true;
            }
        }
        return false;
    }

    bool IsNumberInCol(T number, size_t col_i, int &OutRowIndex = -1, bool ignoreSign = false, size_t start_row_i = 0, int end_row_i = -1 ){
        end_row_i = end_row_i < 0 ? this->Rows-1 : end_row_i;
        for(int r = start_row_i; r <= end_row_i;r++){
            if(this->values[r*this->Cols+col_i] == number || (ignoreSign && this->values[r*this->Cols+col_i] == -number)){
                OutRowIndex = r;
                return true;
            }
        }
        return false;
    }

    //Not working
    /*
    size_t* IsNumberInCol(const SSearchFilter2D<T> &filter){
        size_t indicies[this->Rows];
        size_t row_end_i = filter.row_end_i < 0 ? this->Rows-1 : filter.row_end_i;
        for(int r = filter.row_start_i; r <= row_end_i;r++){
            for(auto elem : filter.search_elements){
                if(this->values[r][filter.col_start_i] == elem || (filter.bIgnoreSign && this->values[r][filter.col_start_i] == -elem)){
                    indicies[r] = r;
                }
            }

        }
        return indicies;
    }
*/

    T GetMeanOfRow(size_t row_i){
        T mean = 0;
        for(int c = 0; c < this->Cols; c++){
            mean += this->values[row_i*this->Cols+c];
        }
        return mean / this->Cols;
    }

    T GetMeanOfCol(size_t col_i){
        T mean = 0;
        for(int r = 0; r < this->Rows; r++){
            mean += this->values[r*this->Cols+col_i];
        }
        return mean / this->Rows;
    }

    void CenteredRow(size_t row_i){
        T mean = GetMeanOfRow(row_i);
        for(int c = 0; c < this->Cols; c++){
            this->values[row_i*this->Cols+c] -= mean;
        }
    }

    Matrix<T> PullCol(size_t col_i){
        Matrix<T> pulledM(this->Rows, 1);
        Matrix<T> newM(this->Rows, this->Cols-1);

        for(size_t r = 0; r < this->Rows; r++){
            int shift = 0;
            for(size_t c = 0; c < this->Cols; c++){
                if(c == col_i){
                    pulledM[r][0] = this->values[r*this->Cols+c];
                    shift++;
                }else{
                    newM[r][c-shift] = this->values[r*this->Cols+c];
                }
            }
        }
        *this = newM;
        return pulledM;
    }

    void AddCol(size_t col_i, T fill_number = 0){

        if(col_i > this->Rows) col_i = this->Cols;
        Matrix<T> newM(this->Rows, this->Cols+1);

        for(size_t r = 0; r < newM.Rows; r++){
            int shift = 0;
            for(size_t c = 0; c < newM.Cols; c++){
                if(c == col_i){
                    newM[r][c] = fill_number;
                    shift++;
                }else{
                    newM[r][c] = this->values[r*this->Cols+c-shift];
                }
            }
        }

        *this = newM;
    }

};


#endif // MATRIX_H
