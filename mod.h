#ifndef MOD_H_INCLUDED
#define MOD_H_INCLUDED


// Variable to hold the pintrest data containing the
// keywords and properties
char ***pintrest;

// These variables will be initialized from the files
// which maintains the count of keywords and properties
int key_count       = 0;
int property_count  = 0;
int user_count = 0;


// Memory allocated to the pintrest table will be as per following:
// 1. key_count + value initialized for key_size for keywords
// 2. property_count + value initialized for property_size for properties
//    The property count is initialized to 2 as currently the properties
//    cannot be added once the value is set in this version release.
// 3. length of each word in table as set in word_length variable.
// Any modified settings can be done to respective variables
// accordingly.
int key_size        = 20;
int property_size   = 2;
int word_length     = 25;


#define SUCCESS                     1
#define FAILURE                     -1
#define FILE_OPEN_ERROR             1001
#define MEMORY_ALLOCATION_ERROR     1002
#define FILE_DELETE_ERROR           1003
#define COUNT_EXCEED_ERROR          1004
#define RESULT_NOT_FOUND            1005
#define FILE_EMPTY_ERROR            1006

#endif 
