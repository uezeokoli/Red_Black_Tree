//-----------------------------------------------------------------------------
//  WordFrequency.cpp
//  Parses through each line of an input file and inserts each alphabetical only 
//  words into dictionary (RBT) with key representing the # of occurences of the word in the file
//-----------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include<string>
#include"Dictionary.h"

using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

   int token_length;
   Dictionary D;
   size_t begin, end, len;
   ifstream in;
   ofstream out;
   string line;
   string token;
   string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$^&%*()-_=+0123456789";

   // check command line for correct number of arguments
   if( argc != 3 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }

   // open files for reading and writing 
   in.open(argv[1]);
   if( !in.is_open() ){
      cerr << "Unable to open file " << argv[1] << " for reading" << endl;
      return(EXIT_FAILURE);
   }

   out.open(argv[2]);
   if( !out.is_open() ){
      cerr << "Unable to open file " << argv[2] << " for writing" << endl;
      return(EXIT_FAILURE);
   }

   // read each line of input file, then count and print tokens 
   while( getline(in, line) )  {
      len = line.length();
      
      // get first token
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = line.substr(begin, end-begin);
      //put token in lowercase

      while( token!="" ){  // we have a token
         // update token buffer
        //put token in lowercase
         token_length = token.length();
         for (int i = 0 ;i<token_length ;i++){
            token[i] = tolower(token[i]);
         }
         if (D.contains(token)){
            D.setValue(token, D.getValue(token) + 1);
         }
         else{
            D.setValue(token,1);
         }

         // get next token
         begin = min(line.find_first_not_of(delim, end+1), len);
         end   = min(line.find_first_of(delim, begin), len);
         token = line.substr(begin, end-begin);
      }

   }

   out<<D;
   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}
