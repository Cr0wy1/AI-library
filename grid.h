#ifndef GRID_H
#define GRID_H
#include <QDebug>
#include <math.h>


//TODO make Rows and Cols parameter in Template
template<class T>
class Grid{

    //TODO check with static_assert

protected:

    size_t Rows = 0;
    size_t Cols = 0;
    size_t Size = 0;


public:
    class proxy;
    class iterator;

    T *values = nullptr;

public:

    //Constructors
    Grid(){}

    Grid(size_t _rows, size_t _cols) : Rows(_rows), Cols(_cols), Size(_rows*_cols){
        values = new T[Size]{0};
    }

    Grid(std::initializer_list< std::initializer_list<T> > il2d){

        Rows = il2d.size();
        Cols = il2d.begin()->size();
        Size = Rows * Cols;

        values = new T[Size]{0};

        for(size_t r = 0; r < Rows; r++){
            auto list_Row = il2d.begin()+r;
            std::copy( list_Row->begin(), list_Row->end(), values + r*Cols);
        }

    }

    //Deconstructor
    ~Grid(){
        //qDebug() << "DECONSTRUCT";
        delete[] values;
    }

    //Copy Constructor
    Grid(const Grid &other){
        //qDebug() << "COPY CONSTRUCTOR";
        Rows = other.Rows;
        Cols = other.Cols;
        Size = other.Size;

        values = new T[Size];

        std::copy(other.values, other.values+Size, values);

    }

    template<class T2>
    Grid(const Grid<T2> &other){
        //qDebug() << "COPY CONSTRUCTOR";
        Rows = other.rows();
        Cols = other.cols();
        Size = other.size();

        values = new T[Size];

        std::copy(other.values, other.values+Size, values);
    }

    template<class T2>
    void FillValues(Grid<T2> &other){
        size_t it_rows = Rows <= other.rows() ? Rows : other.rows();
        size_t it_cols = Cols <= other.cols() ? Cols : other.cols();

        for(size_t r = 0; r < it_rows; r++){
            for(size_t c = 0; c < it_cols; c++){
                values[r*Cols+c] = other[r][c];
            }
        }
    }


    Grid<size_t> GetDigits(){
        Grid<size_t> grid(Rows, Cols);
        for(auto it = begin(); it != end();it++){
            grid[it.Row][it.Col] = floor(log10(*it)) +1;
        }
        return grid;
    }

    Grid<size_t> GetDigits(float){
        Grid<size_t> grid(Rows, Cols);
        for(auto it = begin(); it != end();it++){
            if(*it != 0){
                auto value = fabs(*it);
                grid[it.Row][it.Col] = fabs(floor(log10(value)) +1);

            }
        }
        return grid;
    }

    Grid<size_t> GetDigits(std::string){
        Grid<size_t> grid(Rows, Cols);
        for(auto it = begin(); it != end();it++){
            std::string str = *it;
            grid[it.Row][it.Col] = str.size();
        }
        return grid;
    }


    std::vector<size_t> GetLongestValueInCols(){
        Grid<size_t> digits_grid = GetDigits(T());
        std::vector<size_t> longest_values(Cols);
        size_t highestValue = 0;
        for(auto it = begin(false); it != end(false); it++){
            size_t value = digits_grid[it.Row][it.Col];
            highestValue = value > highestValue ? value : highestValue;
            if(it.Row >= Rows -1){
                longest_values[it.Col] = highestValue;
                highestValue = 0;
            }
        }
        return longest_values;
    }

    virtual size_t rows()const{ return Rows; }
    virtual size_t cols()const{ return Cols; }
    virtual size_t size()const{ return Size; }

    T* GetValues(){
        return values;
    }

    T Value(size_t r, size_t c)const { return values[r][c]; }

    QString GetRounded(size_t r, size_t c, float f){
        if(std::is_arithmetic<T>()) return QString::number(round(values[r][c] * 100) / 100);
    }

    QString GetRounded(size_t r, size_t c, std::string s){
        return QString(values[r][c].c_str());
    }

    virtual T GetHighestInCol(size_t col_i){
        T Highest = values[GetIndex(0, col_i)];
        for(size_t r = 0; r < Rows; r++){
            if(values[GetIndex(r, col_i)] > Highest) Highest = values[GetIndex(r, col_i)];
        }
        return Highest;
    }

    virtual Grid<T> GetHighestInCols(){
        Grid<T> highests(1, Cols);

        for(size_t c = 0; c < Cols;c++){
            highests[0][c] = GetHighestInCol(c);
        }
        return highests;
    }

    virtual T GetLowestInCol(size_t col_i){
        T Lowest = values[GetIndex(0, col_i)];
        for(size_t r = 0; r < Rows; r++){
            if(values[GetIndex(r, col_i)] < Lowest) Lowest = values[GetIndex(r, col_i)];
        }
        return Lowest;
    }

    size_t GetIndex(size_t r, size_t c){
        return r*Cols+c;
    }

    T& GetValue(size_t r, size_t c)const{
        return values[r*Cols+c];
    }


    void GetRow(size_t row_i, T* OUT_row[]){
        T* Row_arr[Cols];
        for(size_t c = 0; c < Rows;c++){
            Row_arr[c] = this->values[row_i*Cols+c];
        }
    }

    Grid GetRow(size_t row_i){
        Grid<T> g_row(1, Cols);
        for(size_t c = 0; c < Rows;c++){
            g_row[row_i][c] = this->values[row_i*Cols+c];
        }
        return g_row;
    }


    //OPERATOR OVERLOADING

    proxy operator [](size_t r){
        return proxy(r, *this);
    }


    Grid& operator =(const Grid &other){
        //qDebug() << "ASSIGMENT";

        if(Cols != other.Cols || Rows != other.Rows){
            delete[] values;
        }

        Rows = other.Rows;
        Cols = other.Cols;
        Size = other.Size;

        values = new T[Size]{0};

        std::copy(other.values, other.values+Size, values);
        return *this;
    }

    template<class T2>
    Grid& operator =(const Grid<T2> &other){
        //qDebug() << "ASSIGMENT";
        if(Cols != other.cols() || Rows != other.cols()){
            delete[] values;
        }

        Rows = other.rows();
        Cols = other.cols();
        Size = other.size();

        values = new T[Size]{0};

        std::copy(other.values, other.values+Size, values);
        return *this;
    }

    //Iterator functions
    iterator virtual begin(bool ForCol = true){
        return iterator(0, 0, *this, ForCol);
    }

    iterator virtual end(bool ForCol = true){
        return iterator(Rows - 1, Cols - 1, *this, ForCol);
    }


};

template<class T>
class Grid<T>::proxy{
public:
    Grid &grid;
    size_t Row;

    proxy(size_t _row, Grid& _elem) : Row(_row), grid(_elem){}


    T& operator [](size_t Col){
        if(Row+1 > grid.Rows || Col+1 > grid.Cols) qDebug() << "Proxy ERROR: Grid" << grid.Rows << "x" << grid.Cols << "- Out of Range!";
        return grid.values[Row * grid.Cols + Col];
    }
};


template<class T>
class Grid<T>::iterator{
public:
    size_t Row;
    size_t Col;
    Grid &grid;
    bool bForCol;

    iterator(size_t _row, size_t _col, Grid& _elem, bool _bForCol = true) : Row(_row), Col(_col), grid(_elem), bForCol(_bForCol){

    }

    T& operator*(){
        return grid.GetValue(Row, Col);
    }

    iterator &operator++(int){
        if(bForCol){
            Col++;
            if(Col > grid.Cols - 1 ){
                Row++;
                Col = 0;
            }
        }else {
            Row++;
            if(Row > grid.Rows - 1 ){
                Col++;
                Row = 0;
            }
        }
        return *this;
    }

    iterator &operator++(){
        if(bForCol){
            Col++;
            if(Col > grid.Cols - 1 ){
                Row++;
                Col = 0;
            }
        }else {
            Row++;
            if(Row > grid.Rows - 1 ){
                Col++;
                Row = 0;
            }
        }
        return *this;
    }

    iterator operator +(const int &i){
        if(bForCol){
            Col++;
            if(Col > grid.Cols - 1 ){
                Row+i;
                Col = 0;
            }
        }else {
            Row++;
            if(Row > grid.Rows - 1 ){
                Col+i;
                Row = 0;
            }
        }
        return *this;
    }

    void operator =(T t){
        grid.GetValue(Row, Col) = t;
    }

    inline bool operator !=(const iterator &other){
        return bForCol ? !(Row == other.Row + 1) : !(Col == other.Col + 1);
    }
};


#endif // GRID_H
