#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include "snake.h"
#include "get_conf.h"
#include <stdio.h>
#include <error.h>
#include <string.h>

static global_data g_data;
static 
void get_keyboard(Snake *snake)
{
    int key;
    Direct new_direct;
    if(ERR != (key=getch()))
    {
        switch(key)
        {
            case KEY_LEFT:
                new_direct = 0;
                break;
            case KEY_UP:
                new_direct = 1;
                break;
            case KEY_RIGHT:
                new_direct = 2;
                break;
            case KEY_DOWN:
                new_direct = 3;
                break;
            default:
                return;
        }
        if(2 == abs(snake->get_direct(snake) - new_direct))
        {
            return;
        }
        else
        {
            snake->set_direct(snake, new_direct);
        }
    }
}

static
void getConf(global_data *g_data)
{
    FILE *fp = NULL;
    char* path = "./conf/config.ini";
    if((fp = fopen(path, "r")))
    {
        int value = 0;
        if(!getIntConf(fp , "color_mode", &value))
        {
            g_data->is_color_mode = value;
        }
        for(int i = 0; i < 3; ++i)
        {
            value = 0;
            char key_word[80] = {0};
            snprintf(key_word, 79, "top_records_%d", i+1);
            if(!getIntConf(fp , key_word, &value))
            {
                g_data->top_records[i] = value;
            }
        }
    }
    else
    {
        perror("Failed to open config file:");
        exit(EXIT_FAILURE);
    }
}

static 
void display_menu(int selection)
{
    clear();
    char* prompts[3] = {"Start game", 
                       "Show the records", 
                       "Quit the game"};
    int width, height;
    getmaxyx(stdscr, height, width);

    char* title = "Sanke Game";
    mvprintw(height/2-3, (width-strlen(title))/2, "%s", title);

    for(int i = 0; i < 3; ++i)
    {
        if(i == selection)
        {
            attron(A_STANDOUT);
        }
        mvprintw(height/2-1+i, (width-strlen(title))/2, "%s", prompts[i]);
        if(i == selection)
        {
            attroff(A_STANDOUT);
        }
    }
    refresh();
}

static 
int start_screen()
{
    static int choice = 0;
    int key = 0;
    do
    {
        if(KEY_DOWN == key)
        {
            ++choice;
        }
        else if(KEY_UP == key)
        {
            --choice;
        }
        choice = (choice+3) % 3;
        display_menu(choice);
    }while((key=getch()) && key != ERR && key != '\n');

    return choice;


}

static 
void display_records()
{
    
    clear();
    
    int width, height;
    getmaxyx(stdscr, height, width);

    char* title = "Sanke Game Score Records";
    mvprintw(0, (width-strlen(title))/2, "%s", title);

    char name[][80] = {"1st", "2nd", "3rd"};
    int i;
    for(i = 0; i < 3; ++i)
    {
        char conntent[80];
        memset(conntent, 0, 80);
        snprintf(conntent, 79, "%s score is %d", name[i], g_data.top_records[i]);
        mvprintw(i+2, (width-strlen(title))/2, "%s", conntent);
    }

    attron(A_STANDOUT);
    mvprintw(i+3, (width-strlen(title))/2, "Press enter to return");
    attroff(A_STANDOUT);
    refresh();
    int key = 0;
    while((key=getch()) && key != ERR && key != '\n');

    
}

static 
void update_records(const Snake *snake)
{
    //Update the record
    int update_flag = 0;
    
    if(snake->score > g_data.top_records[0])
    {
        update_flag = 1;
        g_data.top_records[2] = g_data.top_records[1];
        g_data.top_records[1] = g_data.top_records[0];
        g_data.top_records[0] = snake->score;
    }
    else if(snake->score > g_data.top_records[1])
    {
        update_flag = 1;
        g_data.top_records[2] = g_data.top_records[1];
        g_data.top_records[1] = snake->score;
    }
    else if(snake->score > g_data.top_records[2])
    {
        update_flag = 1;
        g_data.top_records[2] = snake->score;
    }
    if(update_flag)
    {
        FILE *fp = NULL;
        char* path = "./conf/config.ini";
        if((fp = fopen(path, "w")))
        {
            fprintf(fp, "color_mode %d\n", g_data.is_color_mode);
            for(int i = 0; i < 3; ++i)
            {
                fprintf(fp, "top_records_%d %d\n", i+1, g_data.top_records[i]);
            }
            fclose(fp);
        }
        else
        {
            perror("Failed to open config file:");
            exit(EXIT_FAILURE);
        }
    }
}

static void start_game()
{
    clear();
    halfdelay(1);           //non-block IO
    Snake *snake = init_snake();
    start_scene(snake);
    clear();
    box(stdscr, 0, 0);
    snake->draw(snake);
    refresh();

    while(ALIVE == snake->get_lifestatus(snake))
    {
        get_keyboard(snake);
        snake->crawl(snake);
        snake->draw(snake);
        refresh();
    }
    end_scene(snake);
    
    clear();
    print_score(snake);
    delete_snake(snake);
    update_records(snake);

}
int main(int argc, char* argv[])
{
    memset(&g_data, 0, sizeof(g_data));
    getConf(&g_data);
    initscr();
    
    if(1 == g_data.is_color_mode)
    {
        if(FALSE == has_colors())
        {
            endwin();
            fprintf(stderr, "Your terminal doesn't support color\n");
            exit(EXIT_FAILURE);
        }
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    }

    noecho();
    clear();
    curs_set(0);
    keypad(stdscr, true);   //Enable the function key
    
    while(1)
    {
        int choice = start_screen();
        switch(choice)
        {
            case 0:     //start the game
            {
                start_game();
                break;
            }
            case 1:     //display the records
            {
                display_records();
                break;
            }
            case 2:     //Quit the Game
            {
                endwin();
                return EXIT_SUCCESS;
                break;
            }
            default:
                break;
        }
    }
    sleep(1);
    endwin();
    return 0;

}
