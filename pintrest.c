#include "pintrest.h"
#include "mod.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "flags.h"

typedef struct table
{
    char data[20];
    struct table *key;
    struct table *prop;
} NODE;

NODE *head;

// Function:     open_log
// Description:  opens the log file to write the success or failure status
// Input param:  NULL
// Return Type:  NULL
void open_log()
{
    // Open the file
    flog = fopen("log.txt", "a+");

    // If unable to open the file, alert the user
    if (flog == NULL)
        printf("Unable to open log file. No log status will be captured.");
    else
        fprintf(flog, "%s", "_________________________START OF ITERATION________________________");

    fprintf(flog, "%s", "\n\n");
}

// Function:     close_log
// Description:  closes the log file if opened
// Input param:  NULLw
// Return Type:  NULL
void close_log()
{
    if (flog != NULL)
    {
        // a message to indicate end of a iteration
        fprintf(flog, "%s", "_________________________END OF ITERATION________________________");
        fprintf(flog, "%s", "\n\n");

        // Close the file pointer
        fclose(flog);
    }
}

// Function:     write_log
// Description:  logs the status with success or failure message
// Input param:  function name, status and message to display
// Return Type:  NULL
void write_log(char *function_name, char *status, char *message)
{
    // Write the appropriate log message with associated run time
    mytime = time(NULL);
    fprintf(flog, "%s %s : %s --> %s\n\n", ctime(&mytime), function_name, status, message);
}

// Function:     file_empty_check
// Description:  Check whether file has content or not
// Input param:  name of the file
// Return Type:  integer
//               success state is returned if file has content
//               FILE_EMPTY_ERROR otherwise

int file_empty_check(char *filename)
{
    // Declaring "stat" a system call to know about content of file.
    struct stat filestat;
    stat(filename, &filestat);

    // Variables to hodl messages
    char message_failure[40];
    char message_success[40];

    strcpy(message_failure, filename);
    strcat(message_failure, " is empty");

    strcpy(message_success, filename);
    strcat(message_success, " has content");

    // check size of file and return appropriate status.
    if (filestat.st_size == 0)
    {
        write_log("file_empty_check", "FILE_EMPTY_ERROR", message_failure);
        return FILE_EMPTY_ERROR;
    }
    else
    {
        write_log("file_empty_check", "SUCCESS", message_success);
        return SUCCESS;
    }
}

// Function:     pintrest_memory_allocation
// Description:  dynamically allocates the memory to the pintrest table
// Input param:  NULL
// Return Type:  integer
//               success status on successful memory allocation
//               MEMORY_ALLOCATION_ERROR on failure.
int pintrest_memory_allocation()
{
    // Local variables
    int index1;
    int index2;

    // Calculate the required size for keywords and properties.
    // We allocate the the current count plus some additional
    // for the new incoming values for the table.
    property_size = property_size + property_count;
    key_size = key_size + key_count;

    pintrest = malloc(property_size * sizeof(char **));
    if (pintrest == NULL)
        return MEMORY_ALLOCATION_ERROR;

    for (index1 = 0; index1 < property_size; index1++)
    {
        pintrest[index1] = malloc(key_size * sizeof(char *));
        if (pintrest[index1] == NULL)
            return MEMORY_ALLOCATION_ERROR;
        for (index2 = 0; index2 < key_size; index2++)
        {
            pintrest[index1][index2] = malloc(word_length * sizeof(char));
            if (pintrest[index1][index2] == NULL)
                return MEMORY_ALLOCATION_ERROR;
        }
    }

    write_log("pintrest_memory_allocation", "SUCCESS", "Memory allocation Successful");
    return SUCCESS;
}

// Function:     load
// Description:  loads the key_count and property_count from the respective file
//               loads the pintrest table data from secondary memory to primary
// Input param:  NULL
// Return Type:  integer
//               success status on successful operation
//               respective error status code otherwise
int load()
{

    // Start the log file
    open_log();

    // Use a file pointer to open various files to load the values
    FILE *fp;

    // Local variables
    int index = 0;
    int key_index = 0;
    int status = 0;
    int file_status = 0;

    // check whether key_count.txt file is empty or not.
    file_status = file_empty_check("key_count.txt");
    if (file_status == 1006)
        return FAILURE;

    // check whether property_count.txt file is empty or not.
    file_status = file_empty_check("property_count.txt");
    if (file_status == 1006)
        return FAILURE;

    // check whether pintrest_file.txt file is empty or not
    file_status = file_empty_check("data.txt");
    if (file_status == 1006)
        return FAILURE;

    file_status = file_empty_check("user_count.txt");
    if (file_status == 1006)
        return FAILURE;

        file_status = file_empty_check("user.txt");
    if (file_status == 1006)
        return FAILURE;

    // Open the key_count file to read the number of keywords
    fp = fopen("key_count.txt", "r");
    if (fp == NULL)
    {
        write_log("load", "FILE_OPEN_ERROR", "Unable to open the key_count file");
        return FAILURE;
    }

    fscanf(fp, "%d", &key_count);
    if (key_count <= 0)
    {
        write_log("load", "FAILURE", "Key count is 0 or less than 0");
        return FAILURE;
    }
    write_log("load", "SUCCESS", "Key count read successfully");
    fclose(fp);

    // Open the property_count file to read the number of properties
    fp = fopen("property_count.txt", "r");
    if (fp == NULL)
    {
        write_log("load", "FILE_OPEN_ERROR", "Unable to open the property_count file");
        return FAILURE;
    }

    fscanf(fp, "%d", &property_count);
    if (property_count <= 0)
    {
        write_log("load", "FAILURE", "property count is 0 or less than 0");
        return FAILURE;
    }
    write_log("load", "SUCCESS", "Property count read successfully");
    fclose(fp);

    // Open the user_count file to read the number of user
    fp = fopen("user_count.txt", "r");
    if (fp == NULL)
    {
        write_log("load", "FILE_OPEN_ERROR", "Unable to open the user_count file");
        return FAILURE;
    }

    fscanf(fp, "%d", &user_count);
    if (user_count <= 0)
    {
        write_log("load", "FAILURE", "User count is 0 or less than 0");
        return FAILURE;
    }
    write_log("load", "SUCCESS", "User count read successfully");
    fclose(fp);

    // Open the pintrest_file to read the available pintrest details
    fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        write_log("load", "FILE_OPEN_ERROR", "Error in opening the pintrest_file");
        return FAILURE;
    }

    // Allocate the memory for pintrest table
    status = pintrest_memory_allocation();

    if (status == 1002)
    {
        write_log("load", "MEMORY_ALLOCATION_ERROR", "No memory for pintrest table");
        return FAILURE;
    }

    int res;
    // Load the details from file to main memory
    while (!feof(fp))
    {
        for (index = 0; index < key_count; index++)
        {

            // res = convertChar(key_index, index);

            // if(res == 0){
            fscanf(fp, "%s ", pintrest[key_index][index]);
            //}
            // else{
            //  pintrest[key_index][index] = res;
            //}
        }
        key_index++;
    }
    fclose(fp);

    write_log("load", "SUCCESS", "pintrest Load Successful");
    return SUCCESS;
}

// Function:     Display Posts
// Description:  Display the posts of each user
//               Prints the data of pintrest table
// Input param:  NULL
// Return Type:  integer
//               success status on successful operation
//               respective error status code otherwise

int Display_Post()
{
    for (int i = 0; i < property_count; i++)
    {
        printf("\033[0;33m Post number by  %d %s\n", i + 1, pintrest[i][5]);
        printf("\033[0;31m caption: %s \n", pintrest[i][0]);
        printf("\033[0;31m topic: \033[0;36m #%s  \033[0;31m ******image URL******: \033[0;36m %s\n", pintrest[i][1], pintrest[i][2]);
        printf("\033[0;31m Number of pins-> \033[0;36m %s", pintrest[i][3]);
        printf("\033[0;31m Posted on : \033[0;36m %s", pintrest[i][4]);
        
        printf(" \n \n");
        usleep(250000);
    }
}

int add_post()
{
    for (int i = 0; i < key_count - 2; i++)
    {
        switch (i)
        {
        case 0:
            printf("Enter the name \n");
            break;
        case 1:
            printf("Enter the title \n");
            break;
        case 2:
            printf("Enter the topic \n");
            break;
        case 3:
            printf("Enter the date \n");
            break;
        case 4:
            printf("enter the pin \n");
            break;
        }
        scanf("%s", pintrest[property_count][i]);
        // strcpy(pintrest[property_count + 1][i], str);
    }
    int day, month, year;

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    day = local->tm_mday;      // get day of month (1 to 31)
    month = local->tm_mon + 1; // get month of year (0 to 11)
    year = local->tm_year + 1900;

    char buf[12];
    snprintf(buf, 12, "%d/%d/%d", day, month, year);

    strcpy(pintrest[property_count][4], buf);
    strcpy(pintrest[property_count][3], "0");
    property_count = property_count + 1;
    return 0;
}

int update_post()
{

    printf("Enter the number of post to be edited \n");
    int j;
    scanf("%d", &j);

    printf("Enter 0 to edit title\n");
    printf("Enter 1 to edit image url \n");
    printf("Enter 2 to change topic \n");

    char str[20];
    int i;
    scanf("%d", &i);

    switch (i)
    {
    case 0:
        printf("Enter the new Title \n");

        break;
    case 1:
        printf("Enter the new Image url \n");

        break;
    case 2:
        printf("Enter the new topic \n");

        break;
    }

    scanf("%s", str);
    strcpy(pintrest[j - 1][i], str);

    // scanf("%s", pintrest[property_count][i]);
}

int delete_post()
{

    printf("Enter the number of post to be deleted \n");
    int j;
    scanf("%d", &j);

    printf("Are you sure you want to delete the post, this process can't be reverted back \n");
    printf("press 1 - to confirm\n press 0 - to return to main menu\n");
    int i;
    scanf("%d", &i);

    if (i == 1)
    {
        if (j < property_count)
            for (int z = j - 1; z < property_count; z++)
            {
                for (int p = 0; p < key_count; p++)
                {
                    strcpy(pintrest[z][p], pintrest[z + 1][p]);
                }
            }
        property_count = property_count - 1;
    }

    return 0;
}

void sortByPins()
{

    int arr[5];

    // change in datatype

    for (int i = 0; i < property_count; i++)
    {
        arr[i] = atoi(pintrest[i][3]);
    }

    // selection sort

    int temp = 0;
    for (int i = 0; i <= property_count - 2; i++)
    {
        int min = i;
        for (int j = i + 1; j <= property_count - 1; j++)
        {
            if (arr[j] < arr[min])
            {
                min = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }

    // Print the name of post

    for (int i = 0; i < property_count; i++)
    {
        for (int j = 0; j < property_count; j++)
        {
            if (arr[i] == atoi(pintrest[j][3]))
            {
                printf("%s\n", pintrest[j][0]);
            }
        }
    }
}

void date()
{
    int day[2];
    int month[2];
    int year[4];
    int temp[10] = {0};
    int tptp = 0;
    int i, j = 0;

    for (int i = 0; i < property_count; i++)
    {
        temp[i] = atoi(pintrest[i][3]);
    }

    for (int i = 0; i < property_count; i++)
    {
        year[i] = temp[i] % 10000;
        temp[i] = temp[i] / 10000;
        month[i] = temp[i] % 100;
        temp[i] = temp[i] / 100;
        day[i] = temp[i];
    }

    for (int i = 0; i < property_count; i++)
    {
        printf("%d %d %d\n", day[i], month[i], year[i]);
    }
}

int pin_post()
{
    printf("Enter the number of post you want to pin\n");

    int j;
    scanf("%d", &j);
    char str[10];
    strcpy(str, pintrest[j][3]);
    int x = atoi(str);
    x = x + 1;

    itoa(x, str, 10);

    strcpy(pintrest[j][3], str);

    printf("Post Pinned Successfully *\n");
    Display_Post();
}

// Function:     search by title
// Description:  display the pins of the searched title if found , else display no results found
// Input param:  NULL
// Return Type:  return the the desired result's pins or return "No Results Found".

void substring_search_title()
{
    // reading the data from the user
    char a[20];
    printf("\nEnter the title to be searched\n");
    scanf("%s", a);

    // Local variables
    int flag = 0;
    int temp_index = 0;
    int x = 0;

    // Convert the supplied input to lower cases
    for (int temp_index = 0; temp_index < strlen(a); temp_index++)
        a[temp_index] = tolower(a[temp_index]);

    // Search for the required keyword
    for (int i = 0; i < property_count; i++)
    {
        if ((strcmp(pintrest[i][1], a)) == 0)
        {
            x = i;
            flag = 1;
            break;
        }
    }

    // Return the status based on the search result and also gives the output as
    // no results found if the there exist no such data
    // else prints the number of pins with the searched title
    if (flag == 1)
    {
        write_log("search_key", "SUCCESS", "Key index returned");
        printf("Results found: %s \nPins: %s", a, pintrest[x][4]);
    }
    // when flag=0
    else
    {
        write_log("search_key", "RESULT_NOT_FOUND", "Invalid search Key");
        printf("No data found:(\n");
    }
}

// Function:     search by username
// Description:  display all the posts of the searched username if found , else display no results found
// Input param:  NULL
// Return Type:  return the the desired username's post and details of the postsor  return "No Results Found".

void substring_search_username()
{

    // reading the input from the user
    char a[20];
    printf("\nEnter the username to be searched\n");
    scanf("%s", a);

    // Local variables
    int flag = 0;
    int temp_index = 0;
    int x = 0;

    // Convert the supplied input to lower cases
    for (int temp_index = 0; temp_index < strlen(a); temp_index++)
        a[temp_index] = tolower(a[temp_index]);

    // Search for the required keyword
    for (int i = 0; i < property_count; i++)
    {
        if ((strcmp(pintrest[i][5], a)) == 0)
        {
            x = i;
            flag = 1;
            break;
        }
    }

    // Return the status based on the search result and also gives the output as
    // No results found if the there exist no such data
    // Else prints all the details of the post with the searched username
    if (flag == 1)
    {
        write_log("search_key", "SUCCESS", "Key index returned");
        printf("Results found: %s \n", a);
        // checks and prints all the post of the searched username (if there exists many)
        for (int i = 0; i <= property_count; i++)
        {
            if ((strcmp(pintrest[i][0], a)) == 0)
            {
                printf("The post of the searched username are:\n");
                printf("Username: %s\n", pintrest[i][0]);
                printf("Title: %s\n", pintrest[i][1]);
                printf("Topic: %s\n", pintrest[i][2]);
                printf("Date: %s\n", pintrest[i][3]);
                printf("Pins: %s\n", pintrest[i][4]);
            }
        }
    }
    // when flag =0
    // updates the log and prints "No such username found:("
    else
    {
        write_log("search_key", "RESULT_NOT_FOUND", "Invalid search Key");
        printf("No such username found:(\n");
    }
}

// Function:     pintrest_memory_deallocation
// Description:  free the allocated memory for pintrest table
// Input param:  NULL
// Return Type:  NULL
void pintrest_memory_deallocation()
{
    // Local variables
    int index1;
    int index2;

    for (index1 = 0; index1 < property_size; index1++)
    {
        for (index2 = 0; index2 < key_size; index2++)
        {
            free(pintrest[index1][index2]);
        }
        free(pintrest[index1]);
    }
    free(pintrest);

    write_log("pintrest_memory_deallocation", "SUCCESS", "Memory deallocation Successful");
}

int dump()
{
    // Local variables
    int spec_index;
    int key_index;

    // File pointer
    FILE *fp;
    FILE *tfp;

    // Open the file in write mode and dump the latest key count
    fp = fopen("key_count.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the key_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "key_count.txt\n");
        fprintf(tfp, "%d", key_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp, "%d", key_count);
    write_log("dump", "SUCCESS", "Key count dumped successfully");
    fclose(fp);

    // Open the file in write mode and dump the latest properties count
    fp = fopen("property_count.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the property_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "property_count.txt\n");
        fprintf(tfp, "%d", property_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp, "%d", property_count);
    write_log("dump", "SUCCESS", "Property count dumped successfully");
    fclose(fp);

    // Open the file in write mode and dump the latest properties count
    fp = fopen("user_count.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the user_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "user_count.txt\n");
        fprintf(tfp, "%d", user_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp, "%d", user_count);
    write_log("dump", "SUCCESS", "User count dumped successfully");
    fclose(fp);

    fp = fopen("user_count.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the user_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "user_count.txt\n");
        fprintf(tfp, "%d", user_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp, "%d", user_count);
    write_log("dump", "SUCCESS", "User count dumped successfully");
    fclose(fp);

    // Open the user file and update the values from the main memory
    // Into the secondary storage
    fp = fopen("user.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the pintrest_file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "pintrest_file.txt\n");
        for (spec_index = 0; spec_index <= property_count; spec_index++)
        {
            for (key_index = 0; key_index <= key_count; key_index++)
            {
                fprintf(tfp, "%s ", pintrest[spec_index][key_index]);
            }
            fprintf(tfp, "%s", "\n");
        }
        fclose(tfp);
        return FAILURE;
    }

    NODE *temp = head;

    while (temp != NULL)
    {
        NODE *cur = temp;
        fprintf(fp, "%s ", temp->data);
        do
        {
            fprintf(fp, "%s ", cur->key->data);
            cur = cur->key;
        } while (cur->key != NULL);
        fprintf(fp, "%s", "\n");
        temp = temp->prop;
    }

    fclose(fp);

    // Free the memory for pintrest
    pintrest_memory_deallocation();

    write_log("dump", "SUCCESS", "pintrest table dumped successfully");

    // Close the log file
    close_log();
    return SUCCESS;

    // Open the pintrest file and update the values from the main memory
    // Into the secondary storage
    fp = fopen("data.txt", "w+");
    if (fp == NULL)
    {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the pintrest_file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s", "\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "pintrest_file.txt\n");
        for (spec_index = 0; spec_index <= property_count; spec_index++)
        {
            for (key_index = 0; key_index <= key_count; key_index++)
            {
                fprintf(tfp, "%s ", pintrest[spec_index][key_index]);
            }
            fprintf(tfp, "%s", "\n");
        }
        fclose(tfp);
        return FAILURE;
    }

    for (spec_index = 0; spec_index < property_count; spec_index++)
    {
        for (key_index = 0; key_index < key_count; key_index++)
        {
            fprintf(fp, "%s ", pintrest[spec_index][key_index]);
        }
        fprintf(fp, "%s", "\n");
    }
    fclose(fp);

    // Free the memory for pintrest
    pintrest_memory_deallocation();

    write_log("dump", "SUCCESS", "pintrest table dumped successfully");

    // Close the log file
    close_log();
    return SUCCESS;
}

int loadUser()
{

    open_log();
    FILE *fp;

    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        write_log("load", "FILE_OPEN_ERROR", "Error in opening the inventory_file");
        return FAILURE;
        printf("FIle os NULL");
    }

    for (int i = 0; i < user_count; i++)
    {
        NODE *newNode = (NODE *)malloc(sizeof(NODE));
        NODE *temp = head;

        fscanf(fp, "%s ", newNode->data);

        //printf("%s \n", newNode->data);
        newNode->key = NULL;
        newNode->prop = NULL;

        if (head != NULL)
        {
            while (temp->prop != NULL)
            {
                temp = temp->prop;
            }

            temp->prop = newNode;
        }

        if (head == NULL)
        {
            head = newNode;
        }
        for (int j = 0; j < 2; j++)
        {
            NODE *newNode2 = (NODE *)malloc(sizeof(NODE));
            NODE *temp2 = newNode;
            newNode2->key = NULL;
            fscanf(fp, "%s ", newNode2->data);
          //  printf("%s ", newNode2->data);
            while (temp2->key != NULL)
            {
                temp2 = temp2->key;
            }
            temp2->key = newNode2;
        }
    }

    write_log("load", "SUCCESS", "list load sucessful");
    return SUCCESS;
    printf("table created /n");
    fclose(fp);
}

void printList()
{

    NODE *temp = head;

    while (temp != NULL)
    {
        NODE *cur = temp;
        printf("%s ", temp->data);
        do
        {
            printf("%s ", cur->key->data);
            cur = cur->key;
        } while (cur->key != NULL);
        printf("\n");
        temp = temp->prop;
    }
}

int addListKey()
{
    int j = 0;
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    NODE *temp = head;

    printf("enter a Username \n");
    scanf("%s", newNode->data);

    //printf("%s \n", newNode->data);

    j = searchUser(newNode->data);
    newNode->key = NULL;
    newNode->prop = NULL;

    if (j == 0)
    {
        if (head != NULL)
        {

            do
            {
                temp = temp->prop;
            } while (temp->prop != NULL);

            // printf("%s", temp->data);
            temp->prop = newNode;
        }

        if (head == NULL)
        {
            head = newNode;
        }

        for (int j = 0; j < 2; j++)
        {
            if(j == 0){
                printf("Enter The Password \n");
            }else if(j == 1){
                printf("Enter Your Email: \n");
            }
            NODE *newNode2 = (NODE *)malloc(sizeof(NODE));
            NODE *temp2 = newNode;
            newNode2->key = NULL;
            scanf("%s", newNode2->data);
            // printf("%s ", newNode2->data);
            while (temp2->key != NULL)
            {
                temp2 = temp2->key;
            }
            temp2->key = newNode2;
        }
        user_count = user_count + 1;

        dump();
    }else{
        printf("Username Taken  \n");
    }

    return 1;
}

int searchUser(char a[20])
{
    // reading the input from the user
    // char a[20];
    // printf("\nEnter the username to be searched\n");
    // scanf("%s", a);

    // Local variables
    int flag = 0;
    int temp_index = 0;
    int x = 0;

    // Convert the supplied input to lower cases
    for (int temp_index = 0; temp_index < strlen(a); temp_index++)
        a[temp_index] = tolower(a[temp_index]);

    // Search for the required keyword

    NODE *temp = head;

    while (temp->prop != NULL)
    {
        NODE *cur = temp;
        // printf("%s ", temp->data);

        if (strcmp(temp->data, a) == 0)
        {
            // printf("user Exists \n");
            return 1;
        }
        temp = temp->prop;
    }

    return 0;
}

int user_login()
{

    char a[20];
    printf("\nEnter the username\n");
    scanf("%s", a);

    // Local variables
    int flag = 0;
    int x = 1;

    // Convert the supplied input to lower cases
    for (int temp_index = 0; temp_index < strlen(a); temp_index++)
        a[temp_index] = tolower(a[temp_index]);

    // Search for the required keyword

    NODE *temp = head;

    while (temp != NULL)
    {
        NODE *cur = temp;
        //printf("%s ", temp->data);
        if (strcmp(temp->data, a) == 0)
        {
            char pass[20];
            printf("\nEnter the password\n");
            scanf("%s", pass);

            for (int temp_index = 0; temp_index < strlen(a); temp_index++)
                a[temp_index] = tolower(a[temp_index]);

            temp = temp->key;
            //printf("%s \n", temp->data);
            if (strcmp(temp->data, pass) == 0)
            {
                printf("user Loggedin successsfully \n");
                return 2;
            }
            else
            {
                printf("\033[0;31m Oops, wrong password, please login again \n");
                return 1;
            }
        }


         x = strcmp(temp->data, a);         
        temp = temp->prop;
    }

    if(x != 0 ){
        printf(" \033[0;31m user doesn't exist, please sign in first \n");
    }


}
