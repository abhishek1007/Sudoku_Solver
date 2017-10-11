#include<iostream>
#include<fstream>
#include"stdlib.h"

using namespace std;

int backtracks=0;

bool is_consistent(int,int,int,int**);
bool bs(int**,bool[81][9]);
bool rbs(int**,bool[81][9]);

int* cal_restrictions(int,int,bool[81][9]);

void print(int**);
int possible_values(int,int,int**);



int main(int argc, char* argv[])
{
        
        string file;
        string out;
        string puzzle;
        int** sudoku;
        bool domains[81][9];
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
        int index;
        int q1,q2;
        // While puzzles are left
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
                
                
                // Calculating and storing the initial domains of the unassigned variables
                for(i=0;i<9;i++)
                {
                        for(j=0;j<9;j++)
                        {
                                index=i*9+j;
                                if(sudoku[i][j]!=0)            // if assigned then everyting false in the domains
                                {
                                        for(int q=0;q<9;q++)
                                        {
                                                domains[index][q]=false;
                                        }
                                }
                                else                         // if unassigned then calculating the values in the domain
                                {                                
                                        for(int q=0;q<9;q++)
                                        {
                                                domains[index][q]=true;
                                        }
                                        
                                        
                                        for(int q=0;q<9;q++)
                                        {
                                                if(sudoku[i][q]!=0)
                                                {
                                                        domains[index][sudoku[i][q]-1]=false;
                                                }
                                                if(sudoku[q][j]!=0)
                                                {
                                                        domains[index][sudoku[q][j]-1]=false;
                                                }
                                        }
                                        
        
                                        q1=i/3;
                                        q2=j/3;
                                        for(int p=q1*3;p<(q1+1)*3;p++)
                                        {
                                                for(int q=q2*3;q<(q2+1)*3;q++)
                                                {
                                                        if(sudoku[p][q]!=0)
                                                        {
                                                                domains[index][sudoku[p][q]-1]=false;
                                                        }
                                                }
                                        }
                                }
                        }
                }
                
                
                if(bs(sudoku,domains)==true)              // if sudoku is solvable
                {
                        print(sudoku);
                        for(i=0;i<9;i++)                        // printing output to file
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

bool is_consistent(int val,int i,int j,int** sudoku)   // checking if val is consistent at (x,y) in the sudoku
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



bool bs(int** sudoku,bool domains[81][9])
{
        return rbs(sudoku,domains);
}

bool rbs(int** sudoku,bool domains[81][9])
{
        
        int x=-1,y=-1;
        int mrv=10,cur,index;
        int i,j,count=0;
        int val,q1,q2;
        int mini=10;
        int nonzero=0;
        // selecting the unassigned variable with the MRV heuristic
        for(int k=0;k<81;k++)
        {
                if(sudoku[k/9][k%9]==0)
                {
                        nonzero++;
                        count=0;
                        for(int l=0;l<9;l++)
                        {
                                if(domains[k][l]==true)
                                {
                                        count++;
                                }
                        }  
                        if(count==0)
                        {
                                return false;
                        }     
                        if(count<=mini)
                        {
                                mini=count;
                                x=k/9;
                                y=k%9;
                        }
                }
        }
        
        if(nonzero==0)
        {
                return true;
        }
        
        int remaining=9,min=30,minval;
        bool values[9];
        for(i=0;i<9;i++)   // Specifying which all values have been used
        {
                values[i]=false;
        }
        int* restrictions;
        
        // Calculating the restrictions caused by each value
        
        restrictions=cal_restrictions(x,y,domains);
        
        bool changed_domains[9][9];
        bool changed[9];
        bool new_domains[81][9];
         
        while(remaining!=0) // values to check still reamining
        {
                min=30;
                // calculating value with minimum restriction
                for(i=0;i<9;i++)
                {
                        if(values[i]==false && restrictions[i]<min)
                        {
                                min=restrictions[i];
                                minval=i;
                        }
                }
                values[minval]=true;
                val=minval+1;
                
                if(is_consistent(val,x,y,sudoku))                       // if value is consistent
                {
                        
                        // Changing the domains of surroundings
                        
                        for(int i=0;i<9;i++)
                        {
                                for(int j=0;j<9;j++)
                                {
                                        for(int k=0;k<9;k++)
                                        {
                                                new_domains[i*9+j][k]=domains[i*9+j][k];
                                        }       
                                        if(i==x && j==y)
                                        {
                                                for(int k=0;k<9;k++)
                                                {
                                                        new_domains[i*9+j][k]=false;
                                                }        
                                        }
                                }
                        }
                        
                        
                        
                        for(int i=0;i<9;i++)
                        {
                                index=x*9+i;
                                if(domains[index][val-1]==true && i!=y)
                                {
                                        new_domains[index][val-1]=false;
                                }
                                index=i*9+y;
                                if(domains[index][val-1]==true && i!=x) 
                                {
                                        new_domains[index][val-1]=false;
                                }
                        }       
                        q1=x/3;
                        q2=y/3;
                        for(int i=q1*3;i<(q1+1)*3;i++)
                        {
                                for(int j=q2*3;j<(q2+1)*3;j++)
                                {
                                        index=i*9+j;
                                        if(domains[index][val-1]==true && i!=x && j!=y)
                                        {
                                                new_domains[index][val-1]=false;
                                        }
                                }       
                        }
                        
                        sudoku[x][y]=val;
                        
                        // Checking if the sudoku formed now has a solution or not
                        if(rbs(sudoku,new_domains))             
                        {
                                
                                return true;
                        }
                        backtracks++;
                        sudoku[x][y]=0;
                }
                
                remaining--;
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
                //cout<<"\n";
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

// Function for calculatings the restrictions caused in the domains of other variables by entring value in (x,y) for all values and returning them as a list
int* cal_restrictions(int x,int y,bool domains[81][9])
{
        int val;
        int index;
        int count;
        int q1,q2;
        int* restrictions=new int[9];
        for(int r=0;r<9;r++)
        {
                count=0;
                val=r+1;
                for(int i=0;i<9;i++)
                {
                        index=x*9+i;
                        if(domains[index][r]==true && i!=y)
                        {
                                count++;
                        }
                        index=i*9+y;
                        if(domains[index][r]==true && i!=x)
                        {
                                count++;
                        }
                }
                q1=x/3;
                q2=y/3;
                for(int i=q1*3;i<(q1+1)*3;i++)
                {
                        for(int j=q2*3;j<(q2+1)*3;j++)
                        {
                                index=i*9+j;
                                if(domains[index][r]==true && i!=x && j!=y)
                                {
                                        count++;
                                }
                        }
                }
                restrictions[r]=count;
        }
        
        return restrictions;            
}






