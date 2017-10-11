#include<iostream>
#include<stdio.h>
#include<cstdlib>
#include<fstream>

using namespace std;


void print(int[9][9]);

int main(int argc, char* argv[])
{
        string file;
        string out;
        string puzzle;
        int** sudoku;
        sudoku=new int*[9];
        int final_sudoku[9][9];
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
        ofstream input;
        ifstream output;
        f.open(file.c_str());
        o.open(out.c_str());
        int value;
        int q1,q2;
        int total=0;
        int counter=0;
        string str;
        // While puzzles are left
        while(f>>puzzle)
        {
                input.open("temp_input.txt");
                cout<<puzzle<<"\n";
                total=15309;
                
                // Storing each puzzle in the sudoku
                for(i=0;i<9;i++)
                {
                        for(j=0;j<9;j++)
                        {
                                final_sudoku[i][j]=0;
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
                
                input<<"p cnf 729 "<<total<<"\n";
                total=0;
                
                // Adding all the clauses to the text file
                for(int i=0;i<9;i++)
                {
                        for(int j=0;j<9;j++)
                        {
                                for(int k=0;k<9;k++)
                                {
                                        input<<(i*81+j*9+k+1)<<" ";
                                        total++;
                                }
                                input<<0<<"\n";
                                for(int k=0;k<9;k++)
                                {
                                        for(int l=0;l<9;l++)
                                        {
                                                if(l!=i)
                                                {
                                                        input<<(i*81+j*9+k+1)*(-1)<<" "<<(l*81+j*9+k+1)*(-1)<<" "<<0<<"\n";
                                                        total=total+1;
                                                }
                                                if(l!=j)
                                                {
                                                        input<<(i*81+j*9+k+1)*(-1)<<" "<<(i*81+l*9+k+1)*(-1)<<" "<<0<<"\n";
                                                        total=total+1;
                                                }                                        
                                        }
                                        q1=i/3;
                                        q2=j/3;
                                        for(int p=q1*3;p<(q1+1)*3;p++)
                                        {
                                                for(int q=q2*3;q<(q2+1)*3;q++)
                                                {
                                                        if(p!=i && q!=j)
                                                        {
                                                                input<<(i*81+j*9+k+1)*(-1)<<" "<<(p*81+q*9+k+1)*(-1)<<" "<<0<<"\n";       
                                                                total=total+1; 
                                                        }
                                                }
                                        }
                                }
                        }
                }
                
                // Adding the sudoku fixed constrains(for fixed values, different for each sudoku)
                for(int i=0;i<9;i++)
                {
                        for(int j=0;j<9;j++)
                        {
                                if(sudoku[i][j]!=0)
                                {
                                        input<<i*81+j*9+sudoku[i][j]<<" "<<0<<"\n";
                                        total++;
                                }
                        }
                }
                
                
                input.close();
                
                // Calling the MiniSAT solver
                system("./minisat temp_input.txt temp_output.txt");

                
                output.open("temp_output.txt");
                counter=0;
                output>>str;
                
                // Reading from the ouput file from MiniSAT solver and making solution for sudoku
                while(output>>value)
                {
                        if(value==0)
                        {
                                break;
                        }
                        if(value>0)
                        {
                                final_sudoku[counter/81][(counter%81)/9]=(counter%9)+1;        
                        }
                        counter++;
                }
                
                output.close();
                
                
                if(str=="SAT")
                {
                        print(final_sudoku);
                        for(int i=0;i<9;i++) // Storing the answer for the sudoku in the output file
                        {
                                for(int j=0;j<9;j++)
                                {
                                        o<<final_sudoku[i][j];
                                }
                        }
                        o<<"\n";
                        
                }
                else
                {
                        cout<<"UNSATISFIABLE\n";
                        o<<"UNSATISFIABLE\n";
                }
                cout<<"\n\n\n";

        }

        f.close();
        o.close();
        
        return 0;
}



// Function for printing the sudoku
void print(int sudoku[9][9])
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

