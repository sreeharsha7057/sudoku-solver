#ifndef SUDOKU_H
#define SUDOKU_H
#include<iostream>
#include<vector>
#include <fstream>
using namespace std;

class Sudoku
{
    private:
        int values[9][9];
        bool possibilities[9][9][9];

        void modifyPossibilities(int i,int j){
            modifyBoxPossibilities(i,j);
            modifyRowPossibilities(i,j);
            modifyColumnPossibilities(i,j);
            modifyCellPossibilities(i,j);
        }

        void modifyBoxPossibilities(int r, int c){
            int box_row = r/3;
            int box_col = c/3;
            for(int i=box_row*3;i<box_row*3+3;i++){
                for(int j=box_col*3;j<box_col*3+3;j++){
                    possibilities[i][j][values[r][c]-1]=false;
                }
            }
        }

        void modifyRowPossibilities(int r,int c){
            for(int j=0;j<9;j++){
                possibilities[r][j][values[r][c]-1]=false;
            }
        }

        void modifyColumnPossibilities(int r,int c){
            for(int i=0;i<9;i++){
                possibilities[i][c][values[r][c]-1]=false;
            }
        }

        void modifyCellPossibilities(int i,int j){
            for(int k=0;k<9;k++){
                possibilities[i][j][k]=false;
            }
            possibilities[i][j][values[i][j]-1]=true;
        }


        int countNoOfTrue(int i,int j){
            int count=0;
            for(int k=0;k<9;k++){
                if(possibilities[i][j][k]){
                    count++;
                }
            }
            return count;
        }

        int indexOftrue(int i,int j){
            int k;
            for(k=0;k<9;k++){
                if(possibilities[i][j][k]){
                    break;
                }
            }
            return k;
        }

        bool checkRowPossibilities(){
            bool flag = false;
            int index[9];
            for(int i=0;i<9;i++){
                for(int k=0;k<9;k++){
                    int count=0;
                    int ind=0;
                    for(int j=0;j<9;j++){
                        if(possibilities[i][j][k]){
                            count++;
                            index[ind++]=j;
                        }
                    }
                    if( count==1 && values[i][index[0]]==0 ){
                        setValue(k+1,i,index[0]);
                        flag = true;
                    }
                    else if(count==2){
                        if(index[0]/3==index[1]/3){
                            flag |= modifyPossibilities3(i,index[0],i,index[1],k,k);
                        }
                    }
                }
            }
            return flag;
        }

        bool checkColumnPossibilities(){
            bool flag = false;
            int index[9];
            for(int j=0;j<9;j++){
                for(int k=0;k<9;k++){
                    int count=0;
                    int ind=0;
                    for(int i=0;i<9;i++){
                        if(possibilities[i][j][k]){
                            count++;
                            index[ind++]=i;
                        }
                    }
                    if( count==1 && values[index[0]][j]==0 ){
                        setValue(k+1,index[0],j);
                        flag = true;
                    }
                    else if(count==2){
                        if(index[0]/3==index[1]/3){
                            flag |= modifyPossibilities3(index[0],j,index[1],j,k,k);
                        }
                    }
                }
            }
            return flag;
        }

        bool checkBoxPossibilities(){
            bool flag=false;
            int row_index[9];
            int col_index[9];
            for(int k=0;k<9;k++){
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        int count=0;
                        int ind=0;
                        for(int r=i*3;r<i*3+3;r++){
                            for(int c=j*3;c<j*3+3;c++){
                                if(possibilities[r][c][k]){
                                    count++;
                                    row_index[ind]=r;
                                    col_index[ind++]=c;
                                }
                            }
                        }
                        if( count==1 && values[row_index[0]][col_index[0]]==0 ){
                            setValue(k+1,row_index[0],col_index[0]);
                            flag = true;
                        }
                        else if(count==2){
                            if(row_index[0]==row_index[1]){
                                flag |= modifyPossibilities4(row_index[0],col_index[0],row_index[1],col_index[1],k,k);
                            }
                            if(col_index[0]==col_index[1]){
                                flag |= modifyPossibilities5(row_index[0],col_index[0],row_index[1],col_index[1],k,k);
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromPossibilities(){
            bool flag = false;
            flag |= learnFromRowPossibilities();
            flag |= learnFromColPossibilities();
            flag |= learnFromBoxPossibilities();
            return flag;
        }

        bool learnFromRowPossibilities(){
            bool flag = false;
            int col_index[9];
            for(int i=0;i<9;i++){
                int arr[45];
                int arr_index=0;
                int arr_size=0;
                for(int k=0;k<9;k++){
                    int count=0;
                    int ind=0;
                    for(int j=0;j<9;j++){
                        if(possibilities[i][j][k]){
                            count++;
                            col_index[ind++]=j;
                        }
                    }
                    if( count==2 ){
                        arr[arr_index++] = k;
                        arr[arr_index++] = i;
                        arr[arr_index++] = col_index[0];
                        arr[arr_index++] = i;
                        arr[arr_index++] = col_index[1];
                        arr_size++;
                    }
                }
                if(arr_size>1){
                    for(int x=0;x<arr_size-1;x++){
                        for(int y=x+1;y<arr_size;y++){
                            if(arr[5*x+2]==arr[5*y+2] && arr[5*x+4]==arr[5*y+4]){
                                flag |= modifyPossibilities2(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                if(arr[5*x+2]/3==arr[5*x+4]/3){
                                    flag |= modifyPossibilities3(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromColPossibilities(){
            bool flag = false;
            int row_index[9];
            for(int j=0;j<9;j++){
                int arr[45];
                int arr_index=0;
                int arr_size=0;
                for(int k=0;k<9;k++){
                    int count=0;
                    int ind=0;
                    for(int i=0;i<9;i++){
                        if(possibilities[i][j][k]){
                            count++;
                            row_index[ind++]=i;
                        }
                    }
                    if( count==2 ){
                        arr[arr_index++] = k;
                        arr[arr_index++] = row_index[0];
                        arr[arr_index++] = j;
                        arr[arr_index++] = row_index[1];
                        arr[arr_index++] = j;
                        arr_size++;
                    }
                }
                if(arr_size>1){
                    for(int x=0;x<arr_size-1;x++){
                        for(int y=x+1;y<arr_size;y++){
                            if(arr[5*x+1]==arr[5*y+1] && arr[5*x+3]==arr[5*y+3]){
                                flag |= modifyPossibilities2(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                if(arr[5*x+1]/3==arr[5*x+3]/3){
                                    flag |= modifyPossibilities3(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromBoxPossibilities(){
            bool flag=false;
            int row_index[9];
            int col_index[9];
            for(int k=0;k<9;k++){
                for(int i=0;i<3;i++){
                    int arr[45];
                    int arr_index=0;
                    int arr_size=0;
                    for(int j=0;j<3;j++){
                        int count=0;
                        int ind=0;
                        for(int r=i*3;r<i*3+3;r++){
                            for(int c=j*3;c<j*3+3;c++){
                                if(possibilities[r][c][k]){
                                    count++;
                                    row_index[ind]=r;
                                    col_index[ind++]=c;
                                }
                            }
                        }
                        if( count==2 ){
                            arr[arr_index++]=k;
                            arr[arr_index++]=row_index[0];
                            arr[arr_index++]=col_index[0];
                            arr[arr_index++]=row_index[1];
                            arr[arr_index++]=col_index[1];
                            arr_size++;
                        }
                    }
                    if(arr_size>1){
                        for(int x=0;x<arr_size-1;x++){
                            for(int y=x+1;y<arr_size;y++){
                                if(arr[5*x+1]==arr[5*y+1] && arr[5*x+2]==arr[5*y+2] && arr[5*x+3]==arr[5*y+3] && arr[5*x+4]==arr[5*y+4]){
                                    flag |= modifyPossibilities2(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                    if(arr[5*x+1]==arr[5*x+3]){
                                        flag |= modifyPossibilities4(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                    }
                                    if(arr[5*x+2]==arr[5*x+4]){
                                        flag |= modifyPossibilities5(arr[5*x+1],arr[5*x+2],arr[5*x+3],arr[5*x+4],arr[5*x],arr[5*y]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool modifyPossibilities2(int i1,int j1, int i2, int j2, int k1, int k2){
            bool flag=false;
            for(int k=0;k<9;k++){
                if(k!=k1 && k!=k2){
                    if(possibilities[i1][j1][k] ){
                        possibilities[i1][j1][k]=false;
                        flag=true;
                    }
                    if(possibilities[i2][j2][k] ){
                        possibilities[i2][j2][k]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1 and 2 in the possibilities of remaining cells in the Box
        bool modifyPossibilities3(int i1, int j1, int i2, int j2, int k1, int k2){
            int box_row= i1/3;
            int box_col= j1/3;
            bool flag=false;
            for(int r=box_row*3;r<box_row*3+3;r++){
                for(int c=box_col*3;c<box_col*3+3;c++){
                    if( (r!=i1 || c!=j1) && (r!=i2 || c!=j2) ){
                        if(possibilities[r][c][k1]){
                            possibilities[r][c][k1]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k2]){
                            possibilities[r][c][k2]=false;
                            flag= true;
                        }
                    }
                }
            }
            return flag;
        }

        //remove keys 1 and 2 in the possibilities of remaining cells in the row
        bool modifyPossibilities4(int i1, int j1, int i2, int j2, int k1, int k2){
            bool flag=false;
            for(int j=0;j<9;j++){
                if(j!=j1 && j!=j2){
                    if(possibilities[i1][j][k1]){
                        possibilities[i1][j][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k2]){
                        possibilities[i1][j][k2]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1 and 2 in the possibilities of remaining cells in the column
        bool modifyPossibilities5(int i1, int j1, int i2, int j2, int k1, int k2){
            bool flag=false;
            for(int i=0;i<9;i++){
                if(i!=i1 && i!=i2){
                    if(possibilities[i][j1][k1]){
                        possibilities[i][j1][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k2]){
                        possibilities[i][j1][k2]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2 and 3 in the possibilities of remaining cells in the row
        bool modifyPossibilities6(int i1,int j1,int i2,int j2,int i3,int j3,int k1,int k2,int k3){
            bool flag=false;
            for(int j=0;j<9;j++){
                if(j!=j1 && j!=j2 && j!=j3){
                    if(possibilities[i1][j][k1]){
                        possibilities[i1][j][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k2]){
                        possibilities[i1][j][k2]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k3]){
                        possibilities[i1][j][k3]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2 and 3 in the possibilities of remaining cells in the column
        bool modifyPossibilities7(int i1,int j1,int i2,int j2,int i3,int j3,int k1,int k2,int k3){
            bool flag=false;
            for(int i=0;i<9;i++){
                if(i!=i1 && i!=i2 && i!=i3){
                    if(possibilities[i][j1][k1]){
                        possibilities[i][j1][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k2]){
                        possibilities[i][j1][k2]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k3]){
                        possibilities[i][j1][k3]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2 and 3 in the possibilities of remaining cells in the 3x3 box
        bool modifyPossibilities8(int i1,int j1,int i2,int j2,int i3,int j3,int k1,int k2,int k3){
            int box_row= i1/3;
            int box_col= j1/3;
            bool flag=false;
            for(int r=box_row*3;r<box_row*3+3;r++){
                for(int c=box_col*3;c<box_col*3+3;c++){
                    if( (r!=i1 || c!=j1) && (r!=i2 || c!=j2) && (r!=i3 || c!=j3) ){
                        if(possibilities[r][c][k1]){
                            possibilities[r][c][k1]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k2]){
                            possibilities[r][c][k2]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k3]){
                            possibilities[r][c][k3]=false;
                            flag= true;
                        }
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2, 3 and 4 in the possibilities of remaining cells in the row
        bool modifyPossibilities9(int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4, int k1,int k2,int k3, int k4){
            bool flag=false;
            for(int j=0;j<9;j++){
                if(j!=j1 && j!=j2 && j!=j3 && j!=j4){
                    if(possibilities[i1][j][k1]){
                        possibilities[i1][j][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k2]){
                        possibilities[i1][j][k2]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k3]){
                        possibilities[i1][j][k3]=false;
                        flag=true;
                    }
                    if(possibilities[i1][j][k4]){
                        possibilities[i1][j][k4]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2, 3 and 4 in the possibilities of remaining cells in the column
        bool modifyPossibilities10(int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4, int k1,int k2,int k3, int k4){
            bool flag=false;
            for(int i=0;i<9;i++){
                if(i!=i1 && i!=i2 && i!=i3 && i!=i4){
                    if(possibilities[i][j1][k1]){
                        possibilities[i][j1][k1]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k2]){
                        possibilities[i][j1][k2]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k3]){
                        possibilities[i][j1][k3]=false;
                        flag=true;
                    }
                    if(possibilities[i][j1][k4]){
                        possibilities[i][j1][k4]=false;
                        flag=true;
                    }
                }
            }
            return flag;
        }

        //remove keys 1, 2, 3 and 4 in the possibilities of remaining cells in the 3x3 box
        bool modifyPossibilities11(int i1,int j1,int i2,int j2,int i3,int j3,int i4, int j4, int k1,int k2,int k3, int k4){
            int box_row= i1/3;
            int box_col= j1/3;
            bool flag=false;
            for(int r=box_row*3;r<box_row*3+3;r++){
                for(int c=box_col*3;c<box_col*3+3;c++){
                    if( (r!=i1 || c!=j1) && (r!=i2 || c!=j2) && (r!=i3 || c!=j3) && (r!=i4 || c!=j4) ){
                        if(possibilities[r][c][k1]){
                            possibilities[r][c][k1]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k2]){
                            possibilities[r][c][k2]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k3]){
                            possibilities[r][c][k3]=false;
                            flag= true;
                        }
                        if(possibilities[r][c][k4]){
                            possibilities[r][c][k4]=false;
                            flag= true;
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromPossibilities2(){
            bool flag = false;
            flag |= learnFromRowPossibilities2();
            flag |= learnFromColPossibilities2();
            flag |= learnFromBoxPossibilities2();
            return flag;
        }

        bool learnFromRowPossibilities2(){
            bool flag = false;
            int index[9];
            for(int i=0;i<9;i++){
                int arr[36];
                int arr_index=0;
                int arr_size=0;
                int arr2[45];
                int arr2_index=0;
                int arr2_size=0;
                int arr3[54];
                int arr3_size=0;
                int arr3_index=0;
                for(int j=0;j<9;j++){
                    int ind=0;
                    int count=0;
                    for(int k=0;k<9;k++){
                        if(possibilities[i][j][k]){
                            count++;
                            index[ind++]=k;
                        }
                    }
                    if(count==2){
                        arr[arr_index++]=i;
                        arr[arr_index++]=j;
                        arr[arr_index++]=index[0];
                        arr[arr_index++]=index[1];
                        arr_size++;
                    }
                    else if(count==3){
                        arr2[arr2_index++]=i;
                        arr2[arr2_index++]=j;
                        arr2[arr2_index++]=index[0];
                        arr2[arr2_index++]=index[1];
                        arr2[arr2_index++]=index[2];
                        arr2_size++;
                    }
                    else if(count==4){
                        arr3[arr3_index++]=i;
                        arr3[arr3_index++]=j;
                        arr3[arr3_index++]=index[0];
                        arr3[arr3_index++]=index[1];
                        arr3[arr3_index++]=index[2];
                        arr3[arr3_index++]=index[3];
                        arr3_size++;
                    }
                }
                if(arr_size>1){
                    for(int x=0;x<arr_size-1;x++){
                        for(int y=x+1;y<arr_size;y++){
                            if(arr[4*x+2]==arr[4*y+2] && arr[4*x+3]==arr[4*y+3]){
                                flag |= modifyPossibilities4(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                if(arr[4*x]/3==arr[4*y]/3 && arr[4*x+1]/3 == arr[4*y+1]/3){
                                    flag |= modifyPossibilities3(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                }
                            }
                        }
                    }
                }
                if(arr_size+arr2_size>2 && arr2_size>1){
                    int array1[5],array2[5],array3[5],array1_size;
                    int h[9]={0};
                    for(int x=0;x<arr_size+arr2_size-2;x++){
                        for(int y=(x+1>arr_size)?x+1:arr_size;y<arr_size+arr2_size-1;y++){
                            for(int z=y+1;z<arr_size+arr2_size;z++){
                                if(x<arr_size){
                                    for(int d=0;d<4;d++){
                                        array1[d]=arr[4*x+d];
                                    }
                                    array1_size=4;
                                }
                                else{
                                    for(int d=0;d<5;d++){
                                        array1[d]=arr2[5*(x-arr_size)+d];
                                    }
                                    array1_size=5;
                                }
                                for(int d=0;d<5;d++){
                                    array2[d]=arr2[5*(y-arr_size)+d];
                                }
                                for(int d=0;d<5;d++){
                                    array3[d]=arr2[5*(z-arr_size)+d];
                                }

                                for(int d=0;d<9;d++){
                                    h[d]=0;
                                }
                                if(array1_size==4){
                                    h[array1[2]] = 1;
                                    h[array1[3]] = 1;
                                }
                                else{
                                    h[array1[2]] = 1;
                                    h[array1[3]] = 1;
                                    h[array1[4]] = 1;
                                }
                                h[array2[2]] = 1;
                                h[array2[3]] = 1;
                                h[array2[4]] = 1;
                                h[array3[2]] = 1;
                                h[array3[3]] = 1;
                                h[array3[4]] = 1;
                                int ct=0;
                                for(int d=0;d<9;d++){
                                    if(h[d]){
                                        ct++;
                                    }
                                }
                                if(ct==3){
                                    flag |= modifyPossibilities6(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                    if( (array1[1]/3==array2[1]/3) && (array1[1]/3 == array3[1]/3) ){
                                        flag |= modifyPossibilities8(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                    }
                                }
                            }
                        }
                    }
                }
                if(arr_size+arr2_size+arr3_size>3 && arr3_size>1){
                    int array1[6],array2[6],array3[6],array4[6],array1_size,array2_size;
                    int h[9]={0};
                    for(int x=0; x<arr_size+arr2_size+arr3_size-3; x++){
                        for(int y=x+1; y<arr_size+arr2_size+arr3_size-2; y++){
                            for(int z=(y+1>arr_size+arr2_size)?y+1:arr_size+arr2_size; z<arr_size+arr2_size+arr3_size-1; z++){
                                for(int w=z+1;w<arr_size+arr2_size+arr3_size;w++){
                                    if(x<arr_size){
                                        array1_size=4;
                                        for(int d=0;d<4;d++){
                                            array1[d]=arr[4*x+d];
                                        }
                                    }
                                    else if(x<arr_size+arr2_size){
                                        array1_size=5;
                                        for(int d=0;d<5;d++){
                                            array1[d]=arr2[5*(x-arr_size)+d];
                                        }
                                    }
                                    else{
                                        array1_size=6;
                                        for(int d=0;d<6;d++){
                                            array1[d]=arr3[6*(x-arr_size-arr2_size)+d];
                                        }
                                    }
                                    if(y<arr_size){
                                        array2_size=4;
                                        for(int d=0;d<4;d++){
                                            array2[d]=arr[4*y+d];
                                        }
                                    }
                                    else if(y<arr_size+arr2_size){
                                        array2_size=5;
                                        for(int d=0;d<5;d++){
                                            array2[d]=arr2[5*(y-arr_size)+d];
                                        }
                                    }
                                    else{
                                        array2_size=6;
                                        for(int d=0;d<6;d++){
                                            array2[d]=arr3[6*(y-arr_size-arr2_size)+d];
                                        }
                                    }
                                    for(int d=0;d<6;d++){
                                        array3[d]=arr3[6*(z-arr_size-arr2_size)+d];
                                    }
                                    for(int d=0;d<6;d++){
                                        array4[d]=arr3[6*(w-arr_size-arr2_size)+d];
                                    }

                                    for(int d=0;d<9;d++){
                                        h[d]=0;
                                    }
                                    if(array1_size==4){
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                    }
                                    else if(array1_size==5){
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                        h[array1[4]] = 1;
                                    }
                                    else{
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                        h[array1[4]] = 1;
                                        h[array1[5]] = 1;
                                    }
                                    if(array2_size==4){
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                    }
                                    else if(array2_size==5){
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                        h[array2[4]] = 1;
                                    }
                                    else{
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                        h[array2[4]] = 1;
                                        h[array2[5]] = 1;
                                    }
                                    h[array3[2]] = 1;
                                    h[array3[3]] = 1;
                                    h[array3[4]] = 1;
                                    h[array3[5]] = 1;
                                    h[array4[2]] = 1;
                                    h[array4[3]] = 1;
                                    h[array4[4]] = 1;
                                    h[array4[5]] = 1;

                                    int ct=0;
                                    for(int d=0;d<9;d++){
                                        if(h[d]){
                                            ct++;
                                        }
                                    }
                                    if(ct==4){
                                        flag |= modifyPossibilities9(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array4[0],array4[1],array4[2],array4[3],array4[4],array4[5]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromColPossibilities2(){
            bool flag = false;
            int index[9];
            for(int j=0;j<9;j++){
                int arr[36];
                int arr_index=0;
                int arr_size=0;
                int arr2[45];
                int arr2_index=0;
                int arr2_size=0;
                int arr3[54];
                int arr3_index=0;
                int arr3_size=0;
                for(int i=0;i<9;i++){
                    int ind=0;
                    int count=0;
                    for(int k=0;k<9;k++){
                        if(possibilities[i][j][k]){
                            count++;
                            index[ind++]=k;
                        }
                    }
                    if(count==2){
                        arr[arr_index++]=i;
                        arr[arr_index++]=j;
                        arr[arr_index++]=index[0];
                        arr[arr_index++]=index[1];
                        arr_size++;
                    }
                    else if(count==3){
                        arr2[arr2_index++]=i;
                        arr2[arr2_index++]=j;
                        arr2[arr2_index++]=index[0];
                        arr2[arr2_index++]=index[1];
                        arr2[arr2_index++]=index[2];
                        arr2_size++;
                    }
                    else if(count==4){
                        arr3[arr3_index++]=i;
                        arr3[arr3_index++]=j;
                        arr3[arr3_index++]=index[0];
                        arr3[arr3_index++]=index[1];
                        arr3[arr3_index++]=index[2];
                        arr3[arr3_index++]=index[3];
                        arr3_size++;
                    }
                }
                if(arr_size>1){
                    for(int x=0;x<arr_size-1;x++){
                        for(int y=x+1;y<arr_size;y++){
                            if(arr[4*x+2]==arr[4*y+2] && arr[4*x+3]==arr[4*y+3]){
                                flag |= modifyPossibilities5(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                if(arr[4*x]/3==arr[4*y]/3 && arr[4*x+1]/3 == arr[4*y+1]/3){
                                    flag |= modifyPossibilities3(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                }
                            }
                        }
                    }
                }
                if(arr_size+arr2_size>2 && arr2_size>1){
                    int array1[5],array2[5],array3[5],array1_size;
                    int h[9]={0};
                    for(int x=0;x<arr_size+arr2_size-2;x++){
                        for(int y=(x+1>arr_size)?x+1:arr_size;y<arr_size+arr2_size-1;y++){
                            for(int z=y+1;z<arr_size+arr2_size;z++){
                                if(x<arr_size){
                                    for(int d=0;d<4;d++){
                                        array1[d]=arr[4*x+d];
                                    }
                                    array1_size=4;
                                }
                                else{
                                    for(int d=0;d<5;d++){
                                        array1[d]=arr2[5*(x-arr_size)+d];
                                    }
                                    array1_size=5;
                                }
                                for(int d=0;d<5;d++){
                                    array2[d]=arr2[5*(y-arr_size)+d];
                                }
                                for(int d=0;d<5;d++){
                                    array3[d]=arr2[5*(z-arr_size)+d];
                                }

                                for(int d=0;d<9;d++){
                                    h[d]=0;
                                }
                                if(array1_size==4){
                                    h[array1[2]] = 1;
                                    h[array1[3]] = 1;
                                }
                                else{
                                    h[array1[2]] = 1;
                                    h[array1[3]] = 1;
                                    h[array1[4]] = 1;
                                }
                                h[array2[2]] = 1;
                                h[array2[3]] = 1;
                                h[array2[4]] = 1;
                                h[array3[2]] = 1;
                                h[array3[3]] = 1;
                                h[array3[4]] = 1;
                                int ct=0;
                                for(int d=0;d<9;d++){
                                    if(h[d]){
                                        ct++;
                                    }
                                }
                                if(ct==3){
                                    flag |= modifyPossibilities7(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                    if( (array1[0]/3 == array2[0]/3) && (array1[0]/3 == array3[0]/3) ){
                                        flag |= modifyPossibilities8(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                    }
                                }
                            }
                        }
                    }
                }


                if(arr_size+arr2_size+arr3_size>3 && arr3_size>1){
                    int array1[6],array2[6],array3[6],array4[6],array1_size,array2_size;
                    int h[9]={0};
                    for(int x=0; x<arr_size+arr2_size+arr3_size-3; x++){
                        for(int y=x+1; y<arr_size+arr2_size+arr3_size-2; y++){
                            for(int z=(y+1>arr_size+arr2_size)?y+1:arr_size+arr2_size; z<arr_size+arr2_size+arr3_size-1; z++){
                                for(int w=z+1;w<arr_size+arr2_size+arr3_size;w++){

                                    if(x<arr_size){
                                        array1_size=4;
                                        for(int d=0;d<4;d++){
                                            array1[d]=arr[4*x+d];
                                        }
                                    }
                                    else if(x<arr_size+arr2_size){
                                        array1_size=5;
                                        for(int d=0;d<5;d++){
                                            array1[d]=arr2[5*(x-arr_size)+d];
                                        }
                                    }
                                    else{
                                        array1_size=6;
                                        for(int d=0;d<6;d++){
                                            array1[d]=arr3[6*(x-arr_size-arr2_size)+d];
                                        }
                                    }
                                    if(y<arr_size){
                                        array2_size=4;
                                        for(int d=0;d<4;d++){
                                            array2[d]=arr[4*y+d];
                                        }
                                    }
                                    else if(y<arr_size+arr2_size){
                                        array2_size=5;
                                        for(int d=0;d<5;d++){
                                            array2[d]=arr2[5*(y-arr_size)+d];
                                        }
                                    }
                                    else{
                                        array2_size=6;
                                        for(int d=0;d<6;d++){
                                            array2[d]=arr3[6*(y-arr_size-arr2_size)+d];
                                        }
                                    }
                                    for(int d=0;d<6;d++){
                                        array3[d]=arr3[6*(z-arr_size-arr2_size)+d];
                                    }
                                    for(int d=0;d<6;d++){
                                        array4[d]=arr3[6*(w-arr_size-arr2_size)+d];
                                    }

                                    for(int d=0;d<9;d++){
                                        h[d]=0;
                                    }
                                    if(array1_size==4){
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                    }
                                    else if(array1_size==5){
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                        h[array1[4]] = 1;
                                    }
                                    else{
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                        h[array1[4]] = 1;
                                        h[array1[5]] = 1;
                                    }
                                    if(array2_size==4){
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                    }
                                    else if(array2_size==5){
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                        h[array2[4]] = 1;
                                    }
                                    else{
                                        h[array2[2]] = 1;
                                        h[array2[3]] = 1;
                                        h[array2[4]] = 1;
                                        h[array2[5]] = 1;
                                    }
                                    h[array3[2]] = 1;
                                    h[array3[3]] = 1;
                                    h[array3[4]] = 1;
                                    h[array3[5]] = 1;
                                    h[array4[2]] = 1;
                                    h[array4[3]] = 1;
                                    h[array4[4]] = 1;
                                    h[array4[5]] = 1;

                                    int ct=0;
                                    for(int d=0;d<9;d++){
                                        if(h[d]){
                                            ct++;
                                        }
                                    }
                                    if(ct==4){
                                        flag |= modifyPossibilities10(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array4[0],array4[1],array4[2],array4[3],array4[4],array4[5]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

        bool learnFromBoxPossibilities2(){
            bool flag = false;
            int index[9];
            for(int r=0;r<3;r++){
                for(int c=0;c<3;c++){
                    int arr[36];
                    int arr_index=0;
                    int arr_size=0;
                    int arr2[45];
                    int arr2_index=0;
                    int arr2_size=0;
                    int arr3[54];
                    int arr3_size=0;
                    int arr3_index=0;
                    for(int i=3*r;i<3*r+3;i++){
                        for(int j=3*c;j<3*c+3;j++){
                            int count=0;
                            int ind=0;
                            for(int k=0;k<9;k++){
                                if(possibilities[i][j][k]){
                                    count++;
                                    index[ind++]=k;
                                }
                            }
                            if(count==2){
                                arr[arr_index++]=i;
                                arr[arr_index++]=j;
                                arr[arr_index++]=index[0];
                                arr[arr_index++]=index[1];
                                arr_size++;
                            }
                            else if(count==3){
                                arr2[arr2_index++]=i;
                                arr2[arr2_index++]=j;
                                arr2[arr2_index++]=index[0];
                                arr2[arr2_index++]=index[1];
                                arr2[arr2_index++]=index[2];
                                arr2_size++;
                            }
                            else if(count==4){
                                arr3[arr3_index++]=i;
                                arr3[arr3_index++]=j;
                                arr3[arr3_index++]=index[0];
                                arr3[arr3_index++]=index[1];
                                arr3[arr3_index++]=index[2];
                                arr3[arr3_index++]=index[3];
                                arr3_size++;
                            }
                        }
                    }
                    if(arr_size>1){
                        for(int x=0;x<arr_size-1;x++){
                            for(int y=x+1;y<arr_size;y++){
                                if(arr[4*x+2]==arr[4*y+2] && arr[4*x+3]==arr[4*y+3]){
                                    flag |= modifyPossibilities3(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                    if(arr[4*x]==arr[4*y]){
                                        flag |= modifyPossibilities4(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                    }
                                    else if(arr[4*x+1]==arr[4*y+1]){
                                        flag |= modifyPossibilities5(arr[4*x],arr[4*x+1],arr[4*y],arr[4*y+1],arr[4*x+2],arr[4*x+3]);
                                    }
                                }
                            }
                        }
                    }
                    if(arr_size+arr2_size>2 && arr2_size>1){
                        int array1[5],array2[5],array3[5],array1_size;
                        int h[9]={0};
                        for(int x=0;x<arr_size+arr2_size-2;x++){
                            for(int y=(x+1>arr_size)?x+1:arr_size;y<arr_size+arr2_size-1;y++){
                                for(int z=y+1;z<arr_size+arr2_size;z++){
                                    if(x<arr_size){
                                        for(int d=0;d<4;d++){
                                            array1[d]=arr[4*x+d];
                                        }
                                        array1_size=4;
                                    }
                                    else{
                                        for(int d=0;d<5;d++){
                                            array1[d]=arr2[5*(x-arr_size)+d];
                                        }
                                        array1_size=5;
                                    }
                                    for(int d=0;d<5;d++){
                                        array2[d]=arr2[5*(y-arr_size)+d];
                                    }
                                    for(int d=0;d<5;d++){
                                        array3[d]=arr2[5*(z-arr_size)+d];
                                    }

                                    for(int d=0;d<9;d++){
                                        h[d]=0;
                                    }
                                    if(array1_size==4){
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                    }
                                    else{
                                        h[array1[2]] = 1;
                                        h[array1[3]] = 1;
                                        h[array1[4]] = 1;
                                    }
                                    h[array2[2]] = 1;
                                    h[array2[3]] = 1;
                                    h[array2[4]] = 1;
                                    h[array3[2]] = 1;
                                    h[array3[3]] = 1;
                                    h[array3[4]] = 1;
                                    int ct=0;
                                    for(int d=0;d<9;d++){
                                        if(h[d]){
                                            ct++;
                                        }
                                    }
                                    if(ct==3){
                                        flag |= modifyPossibilities8(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                        if( (array1[0] == array2[0]) && (array1[0] == array3[0]) ){
                                            flag |= modifyPossibilities6(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                        }
                                        else if( (array1[1] == array2[1]) && (array1[1] == array3[1]) ){
                                            flag |= modifyPossibilities7(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array3[2],array3[3],array3[4]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(arr_size+arr2_size+arr3_size>3 && arr3_size>1){
                        int array1[6],array2[6],array3[6],array4[6],array1_size,array2_size;
                        int h[9]={0};
                        for(int x=0; x<arr_size+arr2_size+arr3_size-3; x++){
                            for(int y=x+1; y<arr_size+arr2_size+arr3_size-2; y++){
                                for(int z=(y+1>arr_size+arr2_size)?y+1:arr_size+arr2_size; z<arr_size+arr2_size+arr3_size-1; z++){
                                    for(int w=z+1;w<arr_size+arr2_size+arr3_size;w++){
                                        if(x<arr_size){
                                            array1_size=4;
                                            for(int d=0;d<4;d++){
                                                array1[d]=arr[4*x+d];
                                            }
                                        }
                                        else if(x<arr_size+arr2_size){
                                            array1_size=5;
                                            for(int d=0;d<5;d++){
                                                array1[d]=arr2[5*(x-arr_size)+d];
                                            }
                                        }
                                        else{
                                            array1_size=6;
                                            for(int d=0;d<6;d++){
                                                array1[d]=arr3[6*(x-arr_size-arr2_size)+d];
                                            }
                                        }
                                        if(y<arr_size){
                                            array2_size=4;
                                            for(int d=0;d<4;d++){
                                                array2[d]=arr[4*y+d];
                                            }
                                        }
                                        else if(y<arr_size+arr2_size){
                                            array2_size=5;
                                            for(int d=0;d<5;d++){
                                                array2[d]=arr2[5*(y-arr_size)+d];
                                            }
                                        }
                                        else{
                                            array2_size=6;
                                            for(int d=0;d<6;d++){
                                                array2[d]=arr3[6*(y-arr_size-arr2_size)+d];
                                            }
                                        }
                                        for(int d=0;d<6;d++){
                                            array3[d]=arr3[6*(z-arr_size-arr2_size)+d];
                                        }
                                        for(int d=0;d<6;d++){
                                            array4[d]=arr3[6*(w-arr_size-arr2_size)+d];
                                        }

                                        for(int d=0;d<9;d++){
                                            h[d]=0;
                                        }
                                        if(array1_size==4){
                                            h[array1[2]] = 1;
                                            h[array1[3]] = 1;
                                        }
                                        else if(array1_size==5){
                                            h[array1[2]] = 1;
                                            h[array1[3]] = 1;
                                            h[array1[4]] = 1;
                                        }
                                        else{
                                            h[array1[2]] = 1;
                                            h[array1[3]] = 1;
                                            h[array1[4]] = 1;
                                            h[array1[5]] = 1;
                                        }
                                        if(array2_size==4){
                                            h[array2[2]] = 1;
                                            h[array2[3]] = 1;
                                        }
                                        else if(array2_size==5){
                                            h[array2[2]] = 1;
                                            h[array2[3]] = 1;
                                            h[array2[4]] = 1;
                                        }
                                        else{
                                            h[array2[2]] = 1;
                                            h[array2[3]] = 1;
                                            h[array2[4]] = 1;
                                            h[array2[5]] = 1;
                                        }
                                        h[array3[2]] = 1;
                                        h[array3[3]] = 1;
                                        h[array3[4]] = 1;
                                        h[array3[5]] = 1;
                                        h[array4[2]] = 1;
                                        h[array4[3]] = 1;
                                        h[array4[4]] = 1;
                                        h[array4[5]] = 1;

                                        int ct=0;
                                        for(int d=0;d<9;d++){
                                            if(h[d]){
                                                ct++;
                                            }
                                        }
                                        if(ct==4){
                                            flag |= modifyPossibilities11(array1[0],array1[1],array2[0],array2[1],array3[0],array3[1],array4[0],array4[1],array4[2],array4[3],array4[4],array4[5]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return flag;
        }

    public:
        Sudoku();
        virtual ~Sudoku();

        void setValue(int v, int i, int j){
            if(v<10 && v>0){
                values[i][j] = v;
                modifyPossibilities(i,j);
            }
            else{
                values[i][j]=0;
            }
        }

        void printSudoku(){
            ofstream myfile;
            myfile.open("output.txt");
            myfile<<"Output Sudoku:"<<endl;
            for(int i=0;i<9;i++){
                for( int j=0;j<9;j++){
                    cout<<values[i][j]<<" ";
                    myfile<<values[i][j]<<" ";
                }
                cout<<endl;
                myfile<<endl;
            }
            myfile.close();
        }

        void printPossibilities(){
            for (int i=0; i<9 ; i++){
                for (int j=0; j<9; j++){
                    for(int k=0;k<9;k++){
                        if(possibilities[i][j][k]){
                            cout<<k+1<<" ";
                        }
                    }
                    cout<<endl;
                }
                cout<<endl;
            }
        }

        void solveSudoku(){
            while(1){
                bool flag= false;
                for(int i=0;i<9;i++){
                    for(int j=0;j<9;j++){
                        if(values[i][j]==0){
                            if(countNoOfTrue(i,j)==1){
                                setValue( indexOftrue(i,j)+1 , i ,j );
                                flag = true;
                            }
                        }
                    }
                }
                flag |=checkRowPossibilities();
                flag |=checkColumnPossibilities();
                flag |=checkBoxPossibilities();
                if(flag){
                    continue;
                }
                flag |= learnFromPossibilities();
                flag |= learnFromPossibilities2();
                if(!flag){
                    break;
                }
            }
        }

        bool getPossibilities(int i, int j, int k){
            return possibilities[i][j][k];
        }

        int getValue(int i, int j){
            return values[i][j];
        }

        static void copy(Sudoku* output,Sudoku* input){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    output->values[i][j]=input->values[i][j];
                }
            }
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    for(int k=0;k<9;k++){
                        output->possibilities[i][j][k]=input->possibilities[i][j][k];
                    }
                }
            }
        }

        bool findZeroInPossibilities(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    int c=0;
                    for(int k=0;k<9;k++){
                        if(possibilities[i][j][k]){
                            c++;
                        }
                    }
                    if(c==0){
                        return true;
                    }
                }
            }
            return false;
        }

        bool findZeroInSudoku(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(values[i][j]==0){
                        return true;
                    }
                }
            }
            return false;
        }
};

#endif // SUDOKU_H
