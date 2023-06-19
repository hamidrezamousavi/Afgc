/*

#include <windows.h>
#include <iostream>
#include <future> 
#include "lib.h"
using namespace std;


int main(){
  Channle* result = new Channle;
  
  future<void> fu =  async(read_port,result);
  cout << "begin of for" << endl;
  for(int i{0}; i<10000000; i++){
    auto data = result -> get_next();
    
    if(data.first){
    cout << data.second ;
    
    }
  }
  result->close();
  cout << "end of this line" << endl;
 
  return 0; 
}

*/
