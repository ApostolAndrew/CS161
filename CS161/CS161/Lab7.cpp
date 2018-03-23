)#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip> //cout << setprecision(2) << fixed << f;
#include <stdlib.h>
using namespace std;

#define  BLOCK_SIZE     16

#define  CACHE_1024     1024
#define  CACHE_2048     2048
#define  CACHE_4096     4096
#define  CACHE_8192     8192
#define  CACHE_16384    16384

#define  DIRMAP         1
#define  TWO_WAY        2
#define  FOUR_WAY       4
#define  EIGHT_WAY      8

vector<unsigned long long> cache_1024_DM, cache_2048_DM, cache_4096_DM, 
cache_8192_DM, cache_16384_DM; 
vector<unsigned long long> TW_assoc, FW_assoc, EW_assoc; 

vector<vector<unsigned long long> > cache_1024_FIFO_TW, cache_2048_FIFO_TW,  
cache_4096_FIFO_TW, cache_8192_FIFO_TW, cache_16384_FIFO_TW;

vector<vector<unsigned long long> > cache_1024_FIFO_FW, cache_2048_FIFO_FW, 
cache_4096_FIFO_FW, cache_8192_FIFO_FW, cache_16384_FIFO_FW;

vector<vector<unsigned long long> > cache_1024_FIFO_EW, cache_2048_FIFO_EW, 
cache_4096_FIFO_EW, cache_8192_FIFO_EW, cache_16384_FIFO_EW;

vector<vector<unsigned long long> > cache_1024_LRU_TW, cache_2048_LRU_TW, 
cache_4096_LRU_TW, cache_8192_LRU_TW, cache_16384_LRU_TW;

vector<vector<unsigned long long> > cache_1024_LRU_FW, cache_2048_LRU_FW, 
cache_4096_LRU_FW, cache_8192_LRU_FW, cache_16384_LRU_FW;

vector<vector<unsigned long long> > cache_1024_LRU_EW, cache_2048_LRU_EW, 
cache_4096_LRU_EW, cache_8192_LRU_EW, cache_16384_LRU_EW;

double Miss_1024_DM, Miss_2048_DM, Miss_4096_DM, Miss_8192_DM, Miss_16384_DM;
double Miss_1024_FIFO_TW, Miss_2048_FIFO_TW, Miss_4096_FIFO_TW, 
Miss_8192_FIFO_TW, Miss_16384_FIFO_TW;

double Miss_1024_FIFO_FW, Miss_2048_FIFO_FW, Miss_4096_FIFO_FW, 
Miss_8192_FIFO_FW, Miss_16384_FIFO_FW;

double Miss_1024_FIFO_EW, Miss_2048_FIFO_EW, Miss_4096_FIFO_EW, 
Miss_8192_FIFO_EW, Miss_16384_FIFO_EW;

double Miss_1024_LRU_TW, Miss_2048_LRU_TW, Miss_4096_LRU_TW, 
Miss_8192_LRU_TW, Miss_16384_LRU_TW;

double Miss_1024_LRU_FW, Miss_2048_LRU_FW, Miss_4096_LRU_FW, 
Miss_8192_LRU_FW, Miss_16384_LRU_FW;

double Miss_1024_LRU_EW, Miss_2048_LRU_EW, Miss_4096_LRU_EW, 
Miss_8192_LRU_EW, Miss_16384_LRU_EW;


// check whether the tag is inside the set 
bool findFIFOTag(vector<unsigned long long> set, unsigned long long tag) { 
   
   for(int i = 0; i < set.size() ; i++) { // iterate through the set
      if(set[i] == tag) { // if found, do nothing
         return true;
      }
   }
   return false;
}

int findLRUTag(vector<unsigned long long> set, unsigned long long tag) {
   
   for(int i = 0; i < set.size(); i++) { // iterate through the set
      //cout << "set " << i << ": " << set[i] << " ";
      if(set[i] == tag) { // if found return the position
         return i;
      }
   }
   return -1;
}
  // int c = 0;
void twoWayAssoc(unsigned long long read_line) { //two way associative mapping
   
   // CACHE1024 TW
   // 32 set indexing = 5 bits   
   unsigned long long tag_1024_TW = read_line & 0xFFFFFFE0; // get tag 
   unsigned long long index_1024_TW = read_line & 0x0000001F; // get index
      //FIFO
   if(!(findFIFOTag(cache_1024_FIFO_TW[index_1024_TW], tag_1024_TW))) { // check for miss
      cache_1024_FIFO_TW[index_1024_TW].pop_back(); // pop the first inserted 
      cache_1024_FIFO_TW[index_1024_TW].insert(cache_1024_FIFO_TW[index_1024_TW].begin(),tag_1024_TW); // push new tag to front
      Miss_1024_FIFO_TW++;
   }
      //LRU
   
   int i = findLRUTag(cache_1024_LRU_TW[index_1024_TW], tag_1024_TW);
   //cout << endl;
   if(i < 0) { // if miss
      cache_1024_LRU_TW[index_1024_TW].pop_back();
      cache_1024_LRU_TW[index_1024_TW].insert(cache_1024_LRU_TW[index_1024_TW].begin(),tag_1024_TW); // push new tag to front  
      Miss_1024_LRU_TW++;
   }
   else { // if hit
      cache_1024_LRU_TW[index_1024_TW].insert(cache_1024_LRU_TW[index_1024_TW].begin(),tag_1024_TW); // push new tag to front 
      cache_1024_LRU_TW[index_1024_TW].erase(cache_1024_LRU_TW[index_1024_TW].begin() +  i + 1); // delete old instance of the thing
   }
   // c++;
   // if(c == 100)
   // exit(0);
   
   // CACHE2048 TW
   // 64 set indexing = 6 bits   
   unsigned long long tag_2048_TW = read_line & 0xFFFFFFC0; // get tag 
   unsigned long long index_2048_TW = read_line & 0x0000003F; // get index
   
   if(!(findFIFOTag(cache_2048_FIFO_TW[index_2048_TW], tag_2048_TW))) { // check for miss
      cache_2048_FIFO_TW[index_2048_TW].pop_back();
      cache_2048_FIFO_TW[index_2048_TW].insert(cache_2048_FIFO_TW[index_2048_TW].begin(),tag_2048_TW);
      Miss_2048_FIFO_TW++;
   }
      //LRU
      
   i = findLRUTag(cache_2048_LRU_TW[index_2048_TW], tag_2048_TW);
   if(i < 0) { // if miss
      cache_2048_LRU_TW[index_2048_TW].pop_back();
      cache_2048_LRU_TW[index_2048_TW].insert(cache_2048_LRU_TW[index_2048_TW].begin(),tag_2048_TW); // push new tag to front  
      Miss_2048_LRU_TW++;
   }
   else { // if hit
      cache_2048_LRU_TW[index_2048_TW].insert(cache_2048_LRU_TW[index_2048_TW].begin(),tag_2048_TW); // push new tag to front 
      cache_2048_LRU_TW[index_2048_TW].erase(cache_2048_LRU_TW[index_2048_TW].begin() +  i + 1); // delete old instance of the thing
   }     
   
   // CACHE4096 TW
   // 128 set indexing = 7 bits   
   unsigned long long tag_4096_TW = read_line & 0xFFFFFF80; // get tag 
   unsigned long long index_4096_TW = read_line & 0x0000007F; // get index
   
   if(!(findFIFOTag(cache_4096_FIFO_TW[index_4096_TW], tag_4096_TW))) { // check for miss
      cache_4096_FIFO_TW[index_4096_TW].pop_back();
      cache_4096_FIFO_TW[index_4096_TW].insert(cache_4096_FIFO_TW[index_4096_TW].begin(),tag_4096_TW);
      Miss_4096_FIFO_TW++;
   }
      //LRU
      
   i = findLRUTag(cache_4096_LRU_TW[index_4096_TW], tag_4096_TW);
   if(i < 0) { // if miss
      cache_4096_LRU_TW[index_4096_TW].pop_back();
      cache_4096_LRU_TW[index_4096_TW].insert(cache_4096_LRU_TW[index_4096_TW].begin(),tag_4096_TW); // push new tag to front  
      Miss_4096_LRU_TW++;
   }
   else { // if hit
      cache_4096_LRU_TW[index_4096_TW].insert(cache_4096_LRU_TW[index_4096_TW].begin(),tag_4096_TW); // push new tag to front 
      cache_4096_LRU_TW[index_4096_TW].erase(cache_4096_LRU_TW[index_4096_TW].begin() +  i + 1); // delete old instance of the thing
   }   
   
   // CACHE8192 TW
   // 256 set indexing = 8 bits
   unsigned long long tag_8192_TW = read_line & 0xFFFFFF00; // get tag
   unsigned long long index_8192_TW = read_line & 0x000000FF; // get index
   
   if(!(findFIFOTag(cache_8192_FIFO_TW[index_8192_TW], tag_8192_TW))) { // check for miss
      cache_8192_FIFO_TW[index_8192_TW].pop_back();
      cache_8192_FIFO_TW[index_8192_TW].insert(cache_8192_FIFO_TW[index_8192_TW].begin(),tag_8192_TW);
      Miss_8192_FIFO_TW++;
   }
      //LRU
      
   i = findLRUTag(cache_8192_LRU_TW[index_8192_TW], tag_8192_TW);
   if(i < 0) { // if miss
      cache_8192_LRU_TW[index_8192_TW].pop_back();
      cache_8192_LRU_TW[index_8192_TW].insert(cache_8192_LRU_TW[index_8192_TW].begin(),tag_8192_TW); // push new tag to front  
      Miss_8192_LRU_TW++;
   }
   else { // if hit
      cache_8192_LRU_TW[index_8192_TW].insert(cache_8192_LRU_TW[index_8192_TW].begin(),tag_8192_TW); // push new tag to front 
      cache_8192_LRU_TW[index_8192_TW].erase(cache_8192_LRU_TW[index_8192_TW].begin() +  i + 1); // delete old instance of the thing
   } 
   
   // CACHE16384 TW
   // 512 set indexing = 9 bits
   unsigned long long tag_16384_TW = read_line & 0xFFFFFE00; // get tag
   unsigned long long index_16384_TW = read_line & 0x000001FF; // get index
   
   if(!(findFIFOTag(cache_16384_FIFO_TW[index_16384_TW], tag_16384_TW))) { // check for miss
      cache_16384_FIFO_TW[index_16384_TW].pop_back();
      cache_16384_FIFO_TW[index_16384_TW].insert(cache_16384_FIFO_TW[index_16384_TW].begin(),tag_16384_TW);
      Miss_16384_FIFO_TW++;
   }
   
      //LRU
      
   i = findLRUTag(cache_16384_LRU_TW[index_16384_TW], tag_16384_TW);
   if(i < 0) { // if miss
      cache_16384_LRU_TW[index_16384_TW].pop_back();
      cache_16384_LRU_TW[index_16384_TW].insert(cache_16384_LRU_TW[index_16384_TW].begin(),tag_16384_TW); // push new tag to front  
      Miss_16384_LRU_TW++;
   }
   else { // if hit
      cache_16384_LRU_TW[index_16384_TW].insert(cache_16384_LRU_TW[index_16384_TW].begin(),tag_16384_TW); // push new tag to front 
      cache_16384_LRU_TW[index_16384_TW].erase(cache_16384_LRU_TW[index_16384_TW].begin() +  i + 1); // delete old instance of the thing
   } 
}

void fourWayAssoc(unsigned long long read_line) { //four way associative mapping
   
   // CACHE1024 FW
   // 16 set indexing = 4 bits   
   unsigned long long tag_1024_FW = read_line & 0xFFFFFFF0; // get tag 
   unsigned long long index_1024_FW = read_line & 0x0000000F; // get index
      //FIFO
   if(!(findFIFOTag(cache_1024_FIFO_FW[index_1024_FW], tag_1024_FW))) { // check for miss
      cache_1024_FIFO_FW[index_1024_FW].pop_back(); // pop the first inserted 
      cache_1024_FIFO_FW[index_1024_FW].insert(cache_1024_FIFO_FW[index_1024_FW].begin(),tag_1024_FW); //push new tag to front
      Miss_1024_FIFO_FW++;
   }
   
      //LRU
      
   int i = findLRUTag(cache_1024_LRU_FW[index_1024_FW], tag_1024_FW);
   if(i < 0) { // if miss
      cache_1024_LRU_FW[index_1024_FW].pop_back();
      cache_1024_LRU_FW[index_1024_FW].insert(cache_1024_LRU_FW[index_1024_FW].begin(),tag_1024_FW); // push new tag to front  
      Miss_1024_LRU_FW++;
   }
   else { // if hit
      cache_1024_LRU_FW[index_1024_FW].insert(cache_1024_LRU_FW[index_1024_FW].begin(),tag_1024_FW); // push new tag to front 
      cache_1024_LRU_FW[index_1024_FW].erase(cache_1024_LRU_FW[index_1024_FW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE2048 FW
   // 32 set indexing = 5 bits   
   unsigned long long tag_2048_FW = read_line & 0xFFFFFFE0; // get tag 
   unsigned long long index_2048_FW = read_line & 0x0000001F; // get index
   
   if(!(findFIFOTag(cache_2048_FIFO_FW[index_2048_FW], tag_2048_FW))) { // check for miss
      cache_2048_FIFO_FW[index_2048_FW].pop_back();
      cache_2048_FIFO_FW[index_2048_FW].insert(cache_2048_FIFO_FW[index_2048_FW].begin(),tag_2048_FW);
      Miss_2048_FIFO_FW++;
   }

      //LRU
      
   i = findLRUTag(cache_2048_LRU_FW[index_2048_FW], tag_2048_FW);
   if(i < 0) { // if miss
      cache_2048_LRU_FW[index_2048_FW].pop_back();
      cache_2048_LRU_FW[index_2048_FW].insert(cache_2048_LRU_FW[index_2048_FW].begin(),tag_2048_FW); // push new tag to front  
      Miss_2048_LRU_FW++;
   }
   else { // if hit
      cache_2048_LRU_FW[index_2048_FW].insert(cache_2048_LRU_FW[index_2048_FW].begin(),tag_2048_FW); // push new tag to front 
      cache_2048_LRU_FW[index_2048_FW].erase(cache_2048_LRU_FW[index_2048_FW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE4096 FW
   // 64 set indexing = 6 bits   
   unsigned long long tag_4096_FW = read_line & 0xFFFFFFC0; // get tag 
   unsigned long long index_4096_FW = read_line & 0x0000003F; // get index
   
   if(!(findFIFOTag(cache_4096_FIFO_FW[index_4096_FW], tag_4096_FW))) { // check for miss
      cache_4096_FIFO_FW[index_4096_FW].pop_back();
      cache_4096_FIFO_FW[index_4096_FW].insert(cache_4096_FIFO_FW[index_4096_FW].begin(),tag_4096_FW);
      Miss_4096_FIFO_FW++;
   }
   
      //LRU
      
   i = findLRUTag(cache_4096_LRU_FW[index_4096_FW], tag_4096_FW);
   if(i < 0) { // if miss
      cache_4096_LRU_FW[index_4096_FW].pop_back();
      cache_4096_LRU_FW[index_4096_FW].insert(cache_4096_LRU_FW[index_4096_FW].begin(),tag_4096_FW); // push new tag to front  
      Miss_4096_LRU_FW++;
   }
   else { // if hit
      cache_4096_LRU_FW[index_4096_FW].insert(cache_4096_LRU_FW[index_4096_FW].begin(),tag_4096_FW); // push new tag to front 
      cache_4096_LRU_FW[index_4096_FW].erase(cache_4096_LRU_FW[index_4096_FW].begin() +  i + 1); // delete old instance of the thing
   } 
   
   // CACHE8192 FW
   // 128 set indexing = 7 bits
   unsigned long long tag_8192_FW = read_line & 0xFFFFFF80; // get tag
   unsigned long long index_8192_FW = read_line & 0x0000007F; // get index
   
   if(!(findFIFOTag(cache_8192_FIFO_FW[index_8192_FW], tag_8192_FW))) { // check for miss
      cache_8192_FIFO_FW[index_8192_FW].pop_back();
      cache_8192_FIFO_FW[index_8192_FW].insert(cache_8192_FIFO_FW[index_8192_FW].begin(),tag_8192_FW);
      Miss_8192_FIFO_FW++;
   }

      //LRU
      
   i = findLRUTag(cache_8192_LRU_FW[index_8192_FW], tag_8192_FW);
   if(i < 0) { // if miss
      cache_8192_LRU_FW[index_8192_FW].pop_back();
      cache_8192_LRU_FW[index_8192_FW].insert(cache_8192_LRU_FW[index_8192_FW].begin(),tag_8192_FW); // push new tag to front  
      Miss_8192_LRU_FW++;
   }
   else { // if hit
      cache_8192_LRU_FW[index_8192_FW].insert(cache_8192_LRU_FW[index_8192_FW].begin(),tag_8192_FW); // push new tag to front 
      cache_8192_LRU_FW[index_8192_FW].erase(cache_8192_LRU_FW[index_8192_FW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE16384 FW
   // 256 set indexing = 8 bits
   unsigned long long tag_16384_FW = read_line & 0xFFFFFF00; // get tag
   unsigned long long index_16384_FW = read_line & 0x000000FF; // get index
   
   if(!(findFIFOTag(cache_16384_FIFO_FW[index_16384_FW], tag_16384_FW))) { // check for miss
      cache_16384_FIFO_FW[index_16384_FW].pop_back();
      cache_16384_FIFO_FW[index_16384_FW].insert(cache_16384_FIFO_FW[index_16384_FW].begin(),tag_16384_FW);
      Miss_16384_FIFO_FW++;
   }
   
      //LRU
      
   i = findLRUTag(cache_16384_LRU_FW[index_16384_FW], tag_16384_FW);
   if(i < 0) { // if miss
      cache_16384_LRU_FW[index_16384_FW].pop_back();
      cache_16384_LRU_FW[index_16384_FW].insert(cache_16384_LRU_FW[index_16384_FW].begin(),tag_16384_FW); // push new tag to front  
      Miss_16384_LRU_FW++;
   }
   else { // if hit
      cache_16384_LRU_FW[index_16384_FW].insert(cache_16384_LRU_FW[index_16384_FW].begin(),tag_16384_FW); // push new tag to front 
      cache_16384_LRU_FW[index_16384_FW].erase(cache_16384_LRU_FW[index_16384_FW].begin() +  i + 1); // delete old instance of the thing
   } 
}

void eightWayAssoc(unsigned long long read_line) { //eight way associative mapping
   
   // CACHE1024 EW
   // 8 set indexing = 3 bits   
   unsigned long long tag_1024_EW = read_line & 0xFFFFFFF1; // get tag 
   unsigned long long index_1024_EW = read_line & 0x00000007; // get index
      //FIFO
   if(!(findFIFOTag(cache_1024_FIFO_EW[index_1024_EW], tag_1024_EW))) { // check for miss
      cache_1024_FIFO_EW[index_1024_EW].pop_back(); // pop the first inserted 
      cache_1024_FIFO_EW[index_1024_EW].insert(cache_1024_FIFO_EW[index_1024_EW].begin(),tag_1024_EW); //push new tag to front
      Miss_1024_FIFO_EW++;
   }
   
      //LRU
      
   int i = findLRUTag(cache_1024_LRU_EW[index_1024_EW], tag_1024_EW);
   if(i < 0) { // if miss
      cache_1024_LRU_EW[index_1024_EW].pop_back();
      cache_1024_LRU_EW[index_1024_EW].insert(cache_1024_LRU_EW[index_1024_EW].begin(),tag_1024_EW); // push new tag to front  
      Miss_1024_LRU_EW++;
   }
   else { // if hit
      cache_1024_LRU_EW[index_1024_EW].insert(cache_1024_LRU_EW[index_1024_EW].begin(),tag_1024_EW); // push new tag to front 
      cache_1024_LRU_EW[index_1024_EW].erase(cache_1024_LRU_EW[index_1024_EW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE2048 EW
   // 16 set indexing = 4 bits   
   unsigned long long tag_2048_EW = read_line & 0xFFFFFFF0; // get tag 
   unsigned long long index_2048_EW = read_line & 0x0000000F; // get index
   
   if(!(findFIFOTag(cache_2048_FIFO_EW[index_2048_EW], tag_2048_EW))) { // check for miss
      cache_2048_FIFO_EW[index_2048_EW].pop_back();
      cache_2048_FIFO_EW[index_2048_EW].insert(cache_2048_FIFO_EW[index_2048_EW].begin(),tag_2048_EW);
      Miss_2048_FIFO_EW++;
   }

      //LRU
      
   i = findLRUTag(cache_2048_LRU_EW[index_2048_EW], tag_2048_EW);
   if(i < 0) { // if miss
      cache_2048_LRU_EW[index_2048_EW].pop_back();
      cache_2048_LRU_EW[index_2048_EW].insert(cache_2048_LRU_EW[index_2048_EW].begin(),tag_2048_EW); // push new tag to front  
      Miss_2048_LRU_EW++;
   }
   else { // if hit
      cache_2048_LRU_EW[index_2048_EW].insert(cache_2048_LRU_EW[index_2048_EW].begin(),tag_2048_EW); // push new tag to front 
      cache_2048_LRU_EW[index_2048_EW].erase(cache_2048_LRU_EW[index_2048_EW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE4096 EW
   // 32 set indexing = 5 bits   
   unsigned long long tag_4096_EW = read_line & 0xFFFFFFE0; // get tag 
   unsigned long long index_4096_EW = read_line & 0x0000001F; // get index
   
   if(!(findFIFOTag(cache_4096_FIFO_EW[index_4096_EW], tag_4096_EW))) { // check for miss
      cache_4096_FIFO_EW[index_4096_EW].pop_back();
      cache_4096_FIFO_EW[index_4096_EW].insert(cache_4096_FIFO_EW[index_4096_EW].begin(),tag_4096_EW);
      Miss_4096_FIFO_EW++;
   }
   
      //LRU
      
   i = findLRUTag(cache_4096_LRU_EW[index_4096_EW], tag_4096_EW);
   if(i < 0) { // if miss
      cache_4096_LRU_EW[index_4096_EW].pop_back();
      cache_4096_LRU_EW[index_4096_EW].insert(cache_4096_LRU_EW[index_4096_EW].begin(),tag_4096_EW); // push new tag to front  
      Miss_4096_LRU_EW++;
   }
   else { // if hit
      cache_4096_LRU_EW[index_4096_EW].insert(cache_4096_LRU_EW[index_4096_EW].begin(),tag_4096_EW); // push new tag to front 
      cache_4096_LRU_EW[index_4096_EW].erase(cache_4096_LRU_EW[index_4096_EW].begin() +  i + 1); // delete old instance of the thing
   } 
   
   // CACHE8192 EW
   // 64 set indexing = 6 bits
   unsigned long long tag_8192_EW = read_line & 0xFFFFFFC0; // get tag
   unsigned long long index_8192_EW = read_line & 0x0000003F; // get index
   
   if(!(findFIFOTag(cache_8192_FIFO_EW[index_8192_EW], tag_8192_EW))) { // check for miss
      cache_8192_FIFO_EW[index_8192_EW].pop_back();
      cache_8192_FIFO_EW[index_8192_EW].insert(cache_8192_FIFO_EW[index_8192_EW].begin(),tag_8192_EW);
      Miss_8192_FIFO_EW++;
   }
   
      //LRU
      
   i = findLRUTag(cache_8192_LRU_EW[index_8192_EW], tag_8192_EW);
   if(i < 0) { // if miss
      cache_8192_LRU_EW[index_8192_EW].pop_back();
      cache_8192_LRU_EW[index_8192_EW].insert(cache_8192_LRU_EW[index_8192_EW].begin(),tag_8192_EW); // push new tag to front  
      Miss_8192_LRU_EW++;
   }
   else { // if hit
      cache_8192_LRU_EW[index_8192_EW].insert(cache_8192_LRU_EW[index_8192_EW].begin(),tag_8192_EW); // push new tag to front 
      cache_8192_LRU_EW[index_8192_EW].erase(cache_8192_LRU_EW[index_8192_EW].begin() +  i + 1); // delete old instance of the thing
   } 

   // CACHE16384 EW
   // 128 set indexing = 7 bits
   unsigned long long tag_16384_EW = read_line & 0xFFFFFF10; // get tag
   unsigned long long index_16384_EW = read_line & 0x0000007F; // get index
   
   if(!(findFIFOTag(cache_16384_FIFO_EW[index_16384_EW], tag_16384_EW))) { // check for miss
      cache_16384_FIFO_EW[index_16384_EW].pop_back();
      cache_16384_FIFO_EW[index_16384_EW].insert(cache_16384_FIFO_EW[index_16384_EW].begin(),tag_16384_EW);
      Miss_16384_FIFO_EW++;
   }
   
   i = findLRUTag(cache_16384_LRU_EW[index_16384_EW], tag_16384_EW);
   if(i < 0) { // if miss
      cache_16384_LRU_EW[index_16384_EW].pop_back();
      cache_16384_LRU_EW[index_16384_EW].insert(cache_16384_LRU_EW[index_16384_EW].begin(),tag_16384_EW); // push new tag to front  
      Miss_16384_LRU_EW++;
   }
   else { // if hit
      cache_16384_LRU_EW[index_16384_EW].insert(cache_16384_LRU_EW[index_16384_EW].begin(),tag_16384_EW); // push new tag to front 
      cache_16384_LRU_EW[index_16384_EW].erase(cache_16384_LRU_EW[index_16384_EW].begin() +  i + 1); // delete old instance of the thing
   } 
}

void directMapping(unsigned long long read_line) { // direct mapping of all caches
   
   // CACHE1024 DM
   // 64 block indexing = 6 bits         
   unsigned long long tag_1024_DM = read_line & 0xFFFFFFC0; // get tag 
   unsigned long long index_1024_DM = read_line & 0x0000003F; //get index
        
   if(cache_1024_DM[index_1024_DM] != tag_1024_DM) {  // check for miss
      cache_1024_DM[index_1024_DM] = tag_1024_DM;
      Miss_1024_DM++;
      }
         
   // CACHE2048 DM
   // 128 block indexing = 7 bits         
   unsigned long long tag_2048_DM = read_line & 0xFFFFFF80; // get tag
   unsigned long long index_2048_DM = read_line & 0x000007F; //get index         
         
   if(cache_2048_DM[index_2048_DM] != tag_2048_DM) {  // check for miss
      cache_2048_DM[index_2048_DM] = tag_2048_DM;
      Miss_2048_DM++;
   }
         
   // CACHE4096 DM
         
   unsigned long long tag_4096_DM = read_line & 0xFFFFFF00; // get tag
   unsigned long long index_4096_DM = read_line & 0x00000FF; //get index         
         
   if(cache_4096_DM[index_4096_DM] != tag_4096_DM) { // check for miss
      cache_4096_DM[index_4096_DM] = tag_4096_DM;
      Miss_4096_DM++;
   }
         
   // CACHE8192 DM
         
   unsigned long long tag_8192_DM = read_line & 0xFFFFFE00; // get tag
   unsigned long long index_8192_DM = read_line & 0x00001FF; //get index    
         
   if(cache_8192_DM[index_8192_DM] != tag_8192_DM) { // check for miss
      cache_8192_DM[index_8192_DM] = tag_8192_DM;
      Miss_8192_DM++;
   }

   // CACHE16384 DM
         
   unsigned long long tag_16384_DM = read_line & 0xFFFFFC00; // get tag
   unsigned long long index_16384_DM = read_line & 0x00003FF; //get index    
         
   if(cache_16384_DM[index_16384_DM] != tag_16384_DM) { // check for miss
      cache_16384_DM[index_16384_DM] = tag_16384_DM;
      Miss_16384_DM++;
   }
}

double MissRate(double total, double misses) { // calculate miss percentage

   return misses / total * 100;
}

int main () {

// INITIALIZATION OF OUTPUT MATRICES
   
   double LRU[5][6] = {0};
   LRU[0][1] = CACHE_1024;
   LRU[0][2] = CACHE_2048;
   LRU[0][3] = CACHE_4096;
   LRU[0][4] = CACHE_8192;
   LRU[0][5] = CACHE_16384;
   LRU[1][0] = DIRMAP;
   LRU[2][0] = TWO_WAY;
   LRU[3][0] = FOUR_WAY;
   LRU[4][0] = EIGHT_WAY;
   
   double FIFO[5][6] = {0};
   FIFO[0][1] = CACHE_1024;
   FIFO[0][2] = CACHE_2048;
   FIFO[0][3] = CACHE_4096;
   FIFO[0][4] = CACHE_8192;
   FIFO[0][5] = CACHE_16384;
   FIFO[1][0] = DIRMAP;
   FIFO[2][0] = TWO_WAY;
   FIFO[3][0] = FOUR_WAY;
   FIFO[4][0] = EIGHT_WAY;
   
   
// CACHE BUILDING

   int number_of_blocks_1024 = CACHE_1024 / BLOCK_SIZE;
   int number_of_blocks_2048 = CACHE_2048 / BLOCK_SIZE;
   int number_of_blocks_4096 = CACHE_4096 / BLOCK_SIZE;
   int number_of_blocks_8192 = CACHE_8192 / BLOCK_SIZE;
   int number_of_blocks_16384 = CACHE_16384 / BLOCK_SIZE;

   // INITIALZING VECTORS
   
   // DM   
   for(int i = 0; i < number_of_blocks_1024; ++i) {
      cache_1024_DM.push_back(0);
   }
   for(int i = 0; i < number_of_blocks_2048; ++i) {
      cache_2048_DM.push_back(0);
   }
   for(int i = 0; i < number_of_blocks_4096; ++i) {
      cache_4096_DM.push_back(0);
   }
   for(int i = 0; i < number_of_blocks_8192; ++i) {
      cache_8192_DM.push_back(0);
   }
   for(int i = 0; i < number_of_blocks_16384; ++i) {
      cache_16384_DM.push_back(0);
   }
   
   // TW
   for(int i = 0; i < TWO_WAY; i++) {
      TW_assoc.push_back(0);
   }
      //FIFO
   for(int i = 0; i < number_of_blocks_1024 / TWO_WAY; ++i) {
      cache_1024_FIFO_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / TWO_WAY; ++i) {
      cache_2048_FIFO_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / TWO_WAY; ++i) {
      cache_4096_FIFO_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / TWO_WAY; ++i) {
      cache_8192_FIFO_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / TWO_WAY; ++i) {
      cache_16384_FIFO_TW.push_back(TW_assoc);
   }
      //LRU
   for(int i = 0; i < number_of_blocks_1024 / TWO_WAY; ++i) {
      cache_1024_LRU_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / TWO_WAY; ++i) {
      cache_2048_LRU_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / TWO_WAY; ++i) {
      cache_4096_LRU_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / TWO_WAY; ++i) {
      cache_8192_LRU_TW.push_back(TW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / TWO_WAY; ++i) {
      cache_16384_LRU_TW.push_back(TW_assoc);
   }
   
   // FW
   for(int i = 0; i < FOUR_WAY; i++) {
      FW_assoc.push_back(0);
   }
      //FIFO
   for(int i = 0; i < number_of_blocks_1024 / FOUR_WAY; ++i) {
      cache_1024_FIFO_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / FOUR_WAY; ++i) {
      cache_2048_FIFO_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / FOUR_WAY; ++i) {
      cache_4096_FIFO_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / FOUR_WAY; ++i) {
      cache_8192_FIFO_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / FOUR_WAY; ++i) {
      cache_16384_FIFO_FW.push_back(FW_assoc);
   }   
      //LRU
   for(int i = 0; i < number_of_blocks_1024 / FOUR_WAY; ++i) {
      cache_1024_LRU_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / FOUR_WAY; ++i) {
      cache_2048_LRU_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / FOUR_WAY; ++i) {
      cache_4096_LRU_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / FOUR_WAY; ++i) {
      cache_8192_LRU_FW.push_back(FW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / FOUR_WAY; ++i) {
      cache_16384_LRU_FW.push_back(FW_assoc);
   }  
   
   // EW
   for(int i = 0; i < EIGHT_WAY; i++) {
      EW_assoc.push_back(0);
   }
      //FIFO
   for(int i = 0; i < number_of_blocks_1024 / EIGHT_WAY; ++i) {
      cache_1024_FIFO_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / EIGHT_WAY; ++i) {
      cache_2048_FIFO_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / EIGHT_WAY; ++i) {
      cache_4096_FIFO_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / EIGHT_WAY; ++i) {
      cache_8192_FIFO_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / EIGHT_WAY; ++i) {
      cache_16384_FIFO_EW.push_back(EW_assoc);
   }
      //LRU
   for(int i = 0; i < number_of_blocks_1024 / EIGHT_WAY; ++i) {
      cache_1024_LRU_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_2048 / EIGHT_WAY; ++i) {
      cache_2048_LRU_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_4096 / EIGHT_WAY; ++i) {
      cache_4096_LRU_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_8192 / EIGHT_WAY; ++i) {
      cache_8192_LRU_EW.push_back(EW_assoc);
   }
   for(int i = 0; i < number_of_blocks_16384 / EIGHT_WAY; ++i) {
      cache_16384_LRU_EW.push_back(EW_assoc);
   } 

   unsigned long long read_line;   // read memory accesses from standard input
   int total = 0;                  // track total elements
   ifstream myfile ("Trace.txt");  // read from trace file
   if(myfile.is_open()) {
      while (myfile >> hex >> read_line) { 
         total++;
         directMapping(read_line);
         twoWayAssoc(read_line);
         fourWayAssoc(read_line);
         eightWayAssoc(read_line);
      }
   }
   myfile.close();

   LRU[1][1] = MissRate(total, Miss_1024_DM); // Filling in LRU Output
   LRU[1][2] = MissRate(total, Miss_2048_DM);
   LRU[1][3] = MissRate(total, Miss_4096_DM);
   LRU[1][4] = MissRate(total, Miss_8192_DM);
   LRU[1][5] = MissRate(total, Miss_16384_DM);
   
   LRU[2][1] = MissRate(total, Miss_1024_LRU_TW);
   LRU[2][2] = MissRate(total, Miss_2048_LRU_TW);
   LRU[2][3] = MissRate(total, Miss_4096_LRU_TW);
   LRU[2][4] = MissRate(total, Miss_8192_LRU_TW);
   LRU[2][5] = MissRate(total, Miss_16384_LRU_TW);
   
   LRU[3][1] = MissRate(total, Miss_1024_LRU_FW);
   LRU[3][2] = MissRate(total, Miss_2048_LRU_FW);
   LRU[3][3] = MissRate(total, Miss_4096_LRU_FW);
   LRU[3][4] = MissRate(total, Miss_8192_LRU_FW);
   LRU[3][5] = MissRate(total, Miss_16384_LRU_FW);
   
   LRU[4][1] = MissRate(total, Miss_1024_LRU_EW);
   LRU[4][2] = MissRate(total, Miss_2048_LRU_EW);
   LRU[4][3] = MissRate(total, Miss_4096_LRU_EW);
   LRU[4][4] = MissRate(total, Miss_8192_LRU_EW);
   LRU[4][5] = MissRate(total, Miss_16384_LRU_EW);
   
   FIFO[1][1] = MissRate(total, Miss_1024_DM); // Filling in FIFO Output
   FIFO[1][2] = MissRate(total, Miss_2048_DM);
   FIFO[1][3] = MissRate(total, Miss_4096_DM);
   FIFO[1][4] = MissRate(total, Miss_8192_DM);
   FIFO[1][5] = MissRate(total, Miss_16384_DM);
   
   FIFO[2][1] = MissRate(total, Miss_1024_FIFO_TW);
   FIFO[2][2] = MissRate(total, Miss_2048_FIFO_TW);
   FIFO[2][3] = MissRate(total, Miss_4096_FIFO_TW);
   FIFO[2][4] = MissRate(total, Miss_8192_FIFO_TW);
   FIFO[2][5] = MissRate(total, Miss_16384_FIFO_TW);
   
   FIFO[3][1] = MissRate(total, Miss_1024_FIFO_FW);
   FIFO[3][2] = MissRate(total, Miss_2048_FIFO_FW);
   FIFO[3][3] = MissRate(total, Miss_4096_FIFO_FW);
   FIFO[3][4] = MissRate(total, Miss_8192_FIFO_FW);
   FIFO[3][5] = MissRate(total, Miss_16384_FIFO_FW);
   
   FIFO[4][1] = MissRate(total, Miss_1024_FIFO_EW);
   FIFO[4][2] = MissRate(total, Miss_2048_FIFO_EW);
   FIFO[4][3] = MissRate(total, Miss_4096_FIFO_EW);
   FIFO[4][4] = MissRate(total, Miss_8192_FIFO_EW);
   FIFO[4][5] = MissRate(total, Miss_16384_FIFO_EW);
   
   
// OUTPUT

   cout << endl << "LRU Replacement Policy" << endl;
   
   for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 6; j++) {
         cout << setprecision(2) << fixed << LRU[i][j] << "\t";
      }
      cout << endl;
   }
   
   cout << endl << "FIFO Replacement Policy" << endl;
   
   for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 6; j++) {
         cout << setprecision(2) << fixed << FIFO[i][j] << "\t";
      }
      cout << endl;
   }

   return 0;
}
