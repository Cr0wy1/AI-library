#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <stddef.h>
#include <string>
#include <matrix.h>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QRandomGenerator>

struct SCols{
    Matrix<float>* f_matrices;
    Matrix<double>* d_matrices;
    Matrix<std::string>* s_matrices;
};


class Dataframe {

private:
    const size_t HEAD_LENGTH = 10;

    size_t Rows = 0;
    size_t Cols = 0;

public:
    Matrix<char*>* content;

public:
    class proxy;

    std::vector<std::string> Col_Titles;
    std::vector<int> FileTitlesIndices;

    Dataframe(){}
    Dataframe(size_t _rows, size_t _cols, std::vector<std::string> _col_titles = {}) : Col_Titles(_col_titles){}

    Dataframe(std::string filename, std::initializer_list<std::string> include_Cols = {},  std::initializer_list<std::string> exclude_Cols = {}, size_t max_rows = 0){
        QFile file(filename.c_str());
        if(!file.open(QIODevice::ReadOnly)) qDebug() << file.errorString();

        size_t FileLength = GetFileLength(file);


        QList<QByteArray> titles = file.readLine().split(',');
        SetColTitles(titles, include_Cols, exclude_Cols);

        qDebug() << Col_Titles.size();
        content = new Matrix<char*>[Col_Titles.size()]{};
        std::fill_n(content, Col_Titles.size(), Matrix<char*>(FileLength, 1));


        SetValuesFromFile(file);
        file.close();
    }

    ~Dataframe(){}


    Dataframe(const Dataframe &other){
        Col_Titles = other.Col_Titles;
    }


    Dataframe operator [](std::initializer_list<std::string> titles){
        Dataframe df_col(this->Rows, titles.size(), titles);

        for(size_t c = 0;  c < titles.size();c++){
            size_t col_i = GetColIndex(*(titles.begin()+c));

            df_col.content[col_i] = content[col_i];
        }
        qDebug() << QString::fromStdString(df_col.content[0][0][0]);
        return df_col;
    }
    /*
    proxy operator [](size_t r){
        return proxy(r, *this);
    }

    Dataframe &operator =(const Dataframe &other){
        Col_Titles = other.Col_Titles;
        return *this;
    }

    */
    void SetValuesFromFile(QFile &opened_file){
        size_t line_i = 0;
        while(!opened_file.atEnd()){
            QList<QByteArray> lineSplit = opened_file.readLine().split(',');


            size_t col_i = 0;
            for(auto i : FileTitlesIndices){
                std::copy(lineSplit[i].data(), sizeof(lineSplit[i].data()), content[col_i][line_i][0]);
                //content[col_i][line_i][0] = lineSplit[i].data();
                col_i++;
            }

            line_i++;
        }
        opened_file.reset();
    }

    void SetColTitles(QList<QByteArray> titles, std::vector<std::string> include_titles, std::vector<std::string> exclude_titles){
        size_t title_i = 0;
        for(auto title : titles){
            bool bExcluded = false;
            for(auto exclude_title : exclude_titles){
               bExcluded = title.data() == exclude_title;
            }
            if(!bExcluded){
                if(!include_titles.empty()){
                    for(auto Col_title : include_titles){
                        if(title.data() == Col_title){
                            Col_Titles.emplace_back(Col_title);
                            FileTitlesIndices.emplace_back(title_i);
                        }
                    }
                }else{
                    Col_Titles.emplace_back(title.data());
                    FileTitlesIndices.emplace_back(title_i);
                }
            }
            title_i++;
        }
    }
/*
    virtual T GetHighestInCol(size_t col_i)override{
        T Highest = this->values[col_i*this->Cols];
        for(size_t i = 0; i < this->Cols; i++){
            Highest = this->values[col_i*this->Cols +i] > Highest ? this->values[col_i*this->Cols +i] : Highest;
        }
        return Highest;
    }

    virtual T GetLowestInCol(size_t col_i)override{
        T Lowest = this->values[col_i*this->Cols];
        for(size_t i = 0; i < this->Cols; i++){
            Lowest = this->values[col_i*this->Cols +i] < Lowest ? this->values[col_i*this->Cols +i] : Lowest;
        }
        return Lowest;
    }
*/
    int GetColIndex(std::string value){
        int i = 0;
        for(std::string it : Col_Titles){
            if(it == value) return i;
            i++;
        }
        return -1;
    }


    int GetFileLength(QFile &opened_file){
        opened_file.reset();
        size_t lineCount = 0;
        while (!opened_file.atEnd()) {
            opened_file.readLine();
            lineCount++;
        }
        opened_file.reset();
        return lineCount;
    }
    /*
    std::vector<std::string> GetVector(QList<QByteArray> &qlist){
        std::vector<std::string> vec;
        for(auto elem : qlist){
            vec.emplace_back(elem.data());
        }
        return vec;
    }

    std::initializer_list<std::string>* GetColNames(){ return &Col_Titles; }

    Dataframe head(){
        Dataframe<T> df_head(HEAD_LENGTH, this->Cols, Col_Titles);
        df_head.FillValues(*this);
        return df_head;
    }

    Dataframe PullRandom(float percent){
        size_t RowsToPulled = (size_t)(this->Rows * percent);
        size_t RowsNew = this->Rows - RowsToPulled;
        Dataframe<T> Pulled(RowsToPulled, this->Cols, Col_Titles);
        Dataframe<T> New(RowsNew, this->Cols, Col_Titles);

        size_t Pulled_r_i = 0;
        size_t New_r_i = 0;
        for(size_t r = 0; r < this->Rows; r++){
            int random = QRandomGenerator::global()->bounded(0, 100);
            T* it = nullptr;
            if( (random < percent*100 || New_r_i >= RowsNew) && Pulled_r_i < RowsToPulled){
                it = &Pulled[Pulled_r_i][0];
                Pulled_r_i++;
            }else{
                it = &New[New_r_i][0];
                New_r_i++;
            }

            for(size_t c = 0; c < this->Cols; c++){
                *it = this->values[r*this->Cols +c];
                it++;
            }
        }

        *this = New;
        return Pulled;

    }
    */
};

class Dataframe::proxy{
public:
    proxy(size_t _row, Dataframe& _elem){}
};


#endif // DATAFRAME_H
