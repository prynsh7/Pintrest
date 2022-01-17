#include "pintrest.c"
#include <stdio.h>
#include <unistd.h>

void welcome()
{
    unsigned int mSeconds = 100000;

    printf("\033[1;33m");
    printf("| +++          ++++          +++ ++++++++ ++           ++++         +++            ++++           ++++       ++++++++ |\n");
    usleep(mSeconds);
    printf("|   ++        ++  ++        ++   ++       ++         ++    ++     ++   ++         ++  ++         ++  ++      ++       |\n");
    usleep(mSeconds);
    printf("|    ++      ++    ++      ++    ++       ++       ++           ++       ++      ++    ++       ++    ++     ++       |\n");
    usleep(mSeconds);
    printf("|     ++    ++      ++    ++     +++++    ++        ++            ++   ++       ++      ++    ++       ++    +++++    |\n");
    usleep(mSeconds);
    printf("|      ++  ++        ++  ++      ++       ++         ++    ++       +++        ++        ++  ++         ++   ++       |\n");
    usleep(mSeconds);
    printf("|       ++++          ++++       ++++++++ ++++++++     ++++                  +++          ++++           ++  ++++++++ |\n\n");
    usleep(500000);
    printf("|                                              ++++++++      +++                                                      |\n");
    usleep(mSeconds);
    printf("|                                                 ++       ++   ++                                                    |\n");
    usleep(mSeconds);
    printf("|                                                 ++     ++       ++                                                  |\n");
    usleep(mSeconds);
    printf("|                                                 ++       ++   ++                                                    |\n");
    usleep(mSeconds);
    printf("|                                                 ++         +++                                                      |\n");
    usleep(mSeconds);
    printf("|                                                 ++                                                                  |\n\n");
    usleep(500000);
    printf("\033[0;31m");
    printf(" _____________________________________________________________________________________________________________________\n");
    printf("|             +++++++    ++++++++  +++      ++   ++++++++  ++++++++  ++++++    ++++++++     ++++  ++++++++++          |\n");
    usleep(mSeconds);
    printf("|             ++    ++      ++     ++ ++    ++      ++     ++        ++   ++   ++         ++    +     ++              |\n");
    usleep(mSeconds);
    printf("|             ++    ++      ++     ++  ++   ++      ++     ++        ++++++    ++          ++         ++              |\n");
    usleep(mSeconds);
    printf("|             +++++++       ++     ++   ++  ++      ++     +++++     ++  ++    +++++         ++       ++              |\n");
    usleep(mSeconds);
    printf("|             ++            ++     ++    ++ ++      ++     ++        ++   ++   ++         +    ++     ++              |\n");
    usleep(mSeconds);
    printf("|             ++         ++++++++  ++      +++      ++     ++++++++  ++    ++  ++++++++     +++       ++              |\n");
    usleep(mSeconds);
    printf("|_____________________________________________________________________________________________________________________|\n\n");
    usleep(1000000);
}

void menuSignIn()
{
    printf("\033[0m");
    printf("----------------------------------\n");
    printf("\n\t\tMENU\n");
    printf("----------------------------------\n");
    printf("1-> Log in\n");
    printf("2-> sign In\n");
    printf("--------------------------------\n");
    printf("\033[0;32m");
}

void menu()
{

    printf("\033[0m");

    printf("----------------------------------\n");
    printf("\t\tMENU\n");
    printf("----------------------------------\n");
    printf("0-> Close Program\n");
    printf("1-> Show Posts\n");
    printf("2-> Add Post\n");
    printf("3-> Update a Post\n");
    printf("4-> Delete a Post\n");
    printf("5-> Sort Post By Pins\n");
    printf("6-> Sort Post By Date \n");
    printf("7-> Pin A Post \n");
    printf("8-> Filter Post By User \n");
    printf("9-> Filter Post By topic\n");
    printf("--------------------------------\n");
    printf("\033[0;32m");
}

int main()
{   welcome();
    status = load();
    loadUser();

    int z = 0;

    if (status != 1)
    {
        printf("Load Failed\n");
        return 0;
    }

    while (1 && logStatus != 2)
    {
        menuSignIn();
        printf("Enter your choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            logStatus = user_login();
            break;
        case 2:
            addListKey();
            break;
        }
    }

    while (logStatus == 2)
    {   
        if( z == 0 ){
        printf("\033[0;33m");
        printf("__________________________________________Your Feed__________________________________________\n");
        Display_Post();
        z = 1;
        }
        menu();
        printf("Enter your choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 0:
            printf("Post Saved to file\n");
            status = dump();
            if (status != 1)
                printf("Save Failed\n. Please see the tempfile.txt\n");
            return 0;
            break;
        case 1:
            Display_Post();
            break;
        case 2:
            add_post();
            break;
        case 3:
            update_post();
            break;
        case 4:
            delete_post();
            break;
        case 5:
            sortByPins();
            break;
        case 6:
            date();
            break;
        case 7:
            pin_post();
            break;
        case 8:
            substring_search_title();
            break;
        case 9:
            substring_search_username();
            break;
        }
    }
}