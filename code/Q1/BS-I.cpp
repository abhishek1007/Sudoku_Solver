#include<iostream>
#include<fstream>
#include"stdlib.h"

using namespace std;

int backtrack=0;

bool is_consistent(int,int,int,int**);
bool bs(int**);
bool rbs(int**);


void print(int**);
int possible_values(int,int,int**);



int main(int argc, char* argv[])
{
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
                // input and output file names
                file=argv[1];
                out=argv[2];
                
        }
        
        ifstream f;
        ofstream o;
        f.open(file.c_str());
        o.open(out.c_str());
        // While there are puzzles left
        while(f>>puzzle)
        {
                cout<<puzzle<<"\n";
                
                // storing each puzzles in sudoku
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
                if(bs(sudoku)==true)            // if sudoku is slovable
                {
                        print(sudoku);
                        for(i=0;i<9;i++)                 // writing output to the file
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
        
        f.close();
        o.close();
        cout<<"No of backtracks: "<<backtrack<<"\n";
        return 0;
}

bool is_consistent(int val,int i,int j,int** sudoku) // checking if val is consistent at (x,y) in the sudoku
{
        int x,y;
        for(x=0;x<9;x++)
        {
                if(val==sudoku[i][x] || val==sudoku[x][j])
                {
                        return false;
                }
        }

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

        int x=-1,y=-1;
        int mrv=10,cur;
        int i,j;
        int val;
        // selecting the unassigned variable with the MRV heuristic
        for(i=0;i<9;i++)
        {
                for(j=0;j<9;j++)
                {
                        if(sudoku[i][j]==0)
                        {
                                cur=possible_values(i,j,sudoku);
                                if(cur<=mrv)
                                {
                                        x=i;
                                        y=j;
                                        mrv=cur;
                                }                               
                        }
                }
        }
        if(x==-1 && y==-1)
        {
                return true;
        }
        
        // For all values of variable checking if that branch contains a solution
        for(val=1;val<=9;val++)
        {
                if(is_consistent(val,x,y,sudoku))   // is value is consistent
                {
                        sudoku[x][y]=val;
                        if(rbs(sudoku)) 
                        {
                                return true;
                        }
                        backtrack++;
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

// Function for calculating the number of values possible at (x,y) in the sudoku
int possible_values(int x,int y,int** sudoku)
{
        bool a[9];
        for(int i=0;i<9;i++)
        {
                a[i]=true;
        }
        for(int i=0;i<9;i++)
        {
                if(sudoku[x][i]!=0)
                {
                        a[sudoku[x][i]-1]=false;
                }
                if(sudoku[i][y]!=0)
                {
                        a[sudoku[i][y]-1]=false;
                }
        }
        int q1,q2;
        
        q1=x/3;
        q2=y/3;
        for(int i=q1*3;i<(q1+1)*3;i++)
        {
                for(int j=q2*3;j<(q2+1)*3;j++)
                {
                        if(sudoku[i][j]!=0)
                        {
                                a[sudoku[i][j]-1]=false;
                        }
                }
        }
        int count=0;
        for(int i=0;i<9;i++)
        {
                if(a[i]==true)
                {
                        count++;
                }
        }
        return count;
}



