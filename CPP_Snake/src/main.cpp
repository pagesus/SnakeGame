#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "snake.h"
#include <ncurses.h>
#include "get_conf.h"
#include <cstring>
#include <fstream>
static global_data g_data;
static 
void get_keyboard(Snake &snake)
{
    int key;
    Direct new_direct;
    if(ERR != (key=getch()))
    {
        switch(key)
        {
            case KEY_LEFT:
                new_direct = Direct(0);
                break;
            case KEY_UP:
                new_direct = Direct(1);
                break;
            case KEY_RIGHT:
                new_direct = Direct(2);
                break;
            case KEY_DOWN:
                new_direct = Direct(3);
                break;   
            default:
                return;                                             
        }
        if(2 == abs(snake.get_direct() - new_direct))
        {
            return;
        }
        else
        {
            snake.set_direct(new_direct);
        }
    }
}

static
void getConf(global_data *g_data)
{
    std::ifstream input("./conf/config.ini");
    if(input)
    {
        int value = 0;
        if(!getIntConf(input , "color_mode", value))
        {
            g_data->is_color_mode = value;
        }
        for(int i = 0; i < 3; ++i)
        {
            value = 0;
            std::string key_word = std::string("top_records_") + std::to_string(i+1);
            if(!getIntConf(input , key_word, value))
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
    const char* prompts[3] = {"Start game", 
                              "Show the records", 
                              "Quit the game"};
    int width, height;
    getmaxyx(stdscr, height, width);

    const char* title = "Sanke Game";
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

    const char* title = "Sanke Game Score Records";
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
void update_records(Snake &snake)
{
    //Update the record
    int update_flag = 0;
    
    if(snake.get_score() > g_data.top_records[0])
    {
        update_flag = 1;
        g_data.top_records[2] = g_data.top_records[1];
        g_data.top_records[1] = g_data.top_records[0];
        g_data.top_records[0] = snake.get_score();
    }
    else if(snake.get_score() > g_data.top_records[1])
    {
        update_flag = 1;
        g_data.top_records[2] = g_data.top_records[1];
        g_data.top_records[1] = snake.get_score();
    }
    else if(snake.get_score() > g_data.top_records[2])
    {
        update_flag = 1;
        g_data.top_records[2] = snake.get_score();
    }
    if(update_flag)
    {
        std::ofstream out("./conf/config.ini", std::ios::out);
        if(out)
        {
            out << "color_mode " << g_data.is_color_mode << std::endl;
            for(int i = 0; i < 3; ++i)
            {
                out << "top_records_" << i+1 << " " << g_data.top_records[i] << std::endl;
            }
            out.close();
            std::cout << "Update Success !" << std::endl;
        }
        else
        {
            std::cerr << "Failed to open config file:" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

static 
void start_game()
{
    clear();
    halfdelay(1);           //non-block IO
    Snake snake;
    snake.start_scene();
    getch();
    clear();
    box(stdscr, 0, 0);
    snake.draw();
    refresh();

    while(ALIVE == snake.get_lifestatus())
    {
        get_keyboard(snake);
        snake.crawl();
        snake.draw();
        refresh();
    }
    snake.end_scene();
	clear();
	snake.print_score();
    clear();    

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