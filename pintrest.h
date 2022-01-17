#ifndef PINTREST_H_INCLUDED
#define PINTREST_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>



// Variable to capture the time details
time_t mytime;


// Keeps a log of activity. Any errors can be checked in this file
FILE *flog;


// Function Declarations
int load();
int loadUser();
int dump();
void open_log();
int pintrest_memory_allocation();
void pintrest_memory_deallocation();
int add_post();
int Display_Post();
int update_post();
int pin_post();
void substring_search_title();
void substring_search_username();
void printList();
int addList();
void close_log();
void sortByPins();
void date();
int searchUser();
int user_login();


#endif