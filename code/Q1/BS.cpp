#include<iostream>
#include<fstream>
#include"stdlib.h"

using namespace std;

bool is_consistent(int,int,int,int**);
bool bs(int**);
bool rbs(int**);


void print(int**);
int backtracks=0;


int main(int argc, char* argv[])
{
        //cout<<"hahahaha\n";
        string file;
        string out;
        string puzzle;
        int** sudoku;
        sudoku=new int*[9];
        for(int k=0;k<9;k++)
        {
                sudoku[k]=new int[9];
        }
        int i,j;
        if(argc<=2)
        {
                cout<<"File names not specified given!\n";
                return 0;
        }
        else
        {
                // Input file names
                file=argv[1];
                out=argv[2];
                
        }
        ifstream f;
        ofstream o;
        f.open(file.c_str());
        o.open(out.c_str());
        cout<<"starts\n";
        while(f>>puzzle)
        {
                cout<<puzzle<<"\n";
                // storing each puzzle in sudoku
                for(i=0;i<9;i++)
                {
                        for(j=0;j<9;j++)
                        {
                                if(puzzle[i*9+j]=='.')
                                {
                                        sudoku[i][j]=0;
                                }
                                else
                                {
                                        sudoku[i][j]=puzzle[i*9+j]-'0';
                                }
                        }
                }
                
                if(bs(sudoku)==true)                  // if sudoku is solvable
                {
                        print(sudoku);
                        for(i=0;i<9;i++)                    // printing output to file
                        {
                                for(j=0;j<9;j++)
                                {
                                        o<<sudoku[i][j];
                                }
                        }
                        o<<"\n";
                        
                }
                else
                {
                        cout<<"UNSATISFIABLE\n";
                        o<<"UNSATISFIABLE\n";
                }
        }
        
        cout<<"Number of backtracks: "<<backtracks<<"\n";
        f.close();
        o.close();
        return 0;
}


bool is_consistent(int val,int i,int j,int** sudoku)             // checking if val is consistent at (x,y) in the sudoku
{
        int x,y;
        //cout<<"here1\n";
        for(x=0;x<9;x++)
        {
                if(val==sudoku[i][x] || val==sudoku[x][j])
                {
                        return false;
                }
        }
        //cout<<"here2\n";
        int q1,q2;
        q1=i/3;
        q2=j/3;
        for(x=q1*3;x<(q1+1)*3;x++)
        {
                for(y=q2*3;y<(q2+1)*3;y++)
                {
                        if(val==sudoku[x][y])
                        {
                                return false;
                        }
                }
        }
        return true;
}



bool bs(int** sudoku)
{
        return rbs(sudoku);
}

bool rbs(int** sudoku)
{
        //print(sudoku);
        int x=-1,y=-1;
        int i,j;
        int val;
        
        // selecting the next unassigned variable
        for(i=0;i<9;i++)
        {
                for(j=0;j<9;j++)
                {
                        if(sudoku[i][j]==0)
                        {
                                x=i;
                                y=j;
                                break;
                        }
                }
                if(x!=-1 || y!=-1)
                {
                        break;
                }
        }
        if(x==-1 && y==-1)
        {
                return true;
        }
        
        
        for(val=1;val<=9;val++)  // For each value for the variable
        {
                if(is_consistent(val,x,y,sudoku))         // If value is consistent
                {
                        sudoku[x][y]=val;
                        
                        // Checking if the sudoku formed now has a solution or not
                        if(rbs(sudoku))
                        {
                                return true;
                        }
                        backtracks++;
                        sudoku[x][y]=0;
                }
        }
        return false;
}

// Function for printing the sudoku
void print(int** sudoku)
{
        for(int i=0;i<9;i++)
        {
                for(int j=0;j<9;j++)
                {
                        cout<<sudoku[i][j];
                }
        }
        cout<<"\n";
}




