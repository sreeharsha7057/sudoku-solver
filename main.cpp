#include <iostream>
#include <Sudoku.h>
#include <fstream>
using namespace std;

int main()
{
    Sudoku* s = new Sudoku();
    int value;

    ifstream inputfile;
    inputfile.open("input.txt");
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            inputfile>>value;
            s->setValue(value,i,j);
        }
    }
    inputfile.close();

    cout<<endl<<"Input Sudoku: "<<endl;
    s->printSudoku();


    /*if(s->findZeroInPossibilities()){           //implies given sudoku is wrong
        cout<<"Enter correct values:"<<endl;
        break;
    }*/
    s->solveSudoku();

    //Backtracking is employed for hard level sudokus
    while(s->findZeroInSudoku()){
        cout<<endl<<"backtracking is being used to solve"<<endl;
        Sudoku* s2=new Sudoku();
        Sudoku::copy(s2,s);
        bool break_flag=false;
        int possibility[9];
        int possibility_index;
        int i,j;
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                int n=0;
                possibility_index=0;
                for(int k=0;k<9;k++){
                    if(s2->getPossibilities(i,j,k)){
                        n++;
                        possibility[possibility_index++]=k+1;
                    }
                }
                if(n==2){
                    break_flag=true;
                    break;
                }
            }
            if(break_flag){
                break;
            }
        }
        if(break_flag){
            s2->setValue(possibility[0],i,j);
            s2->solveSudoku();
            if(s2->findZeroInPossibilities()){
                s->setValue(possibility[1],i,j);
                s->solveSudoku();
                if(s->findZeroInPossibilities()){           //implies given sudoku is wrong
                    cout<<"Please check the entered values. There might be a mistake in the entered values."<<endl;
                    break;
                }
            }
            else{
                Sudoku* s3= new Sudoku();
                s3->setValue(possibility[1],i,j);
                s3->solveSudoku();
                if( !( s3->findZeroInPossibilities() ) ){
                    cout<<"Haha, This Sudoku has multiple Solutions"<<endl;
                }
                Sudoku::copy(s,s2);
            }
        }

        //if there are no cells with two possibilities in sudoku
        else{
            if(s->findZeroInSudoku()){
                cout<<"The current algorithm cannot solve this Sudoku";
            }
            break;
        }
    }
    //}
    cout<<endl<<"solved sudoku:"<<endl;
    s->printSudoku();

    return 0;
}
