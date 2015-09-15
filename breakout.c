//
// breakout.c
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);
    
    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
     double velocity= 2.0 + drand48() ;
     double vely = 2.0;
     updateScoreboard(window, label, points);
     waitForClick();
    while (lives > 0 && bricks > 0)
    {
        
        updateScoreboard(window, label, points);
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
            {
            
                
                if (getEventType(event) == MOUSE_MOVED)
                {
                    double x = getX(event) - getWidth(paddle) / 2;
                    setLocation(paddle, x, getY(paddle));                                              
                }
            }
                 
        move(ball, velocity, vely);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocity = -velocity;
        }
        else if (getX(ball) <= 0)
        {
            velocity = -velocity;
          
        }
        if( getY(ball) <= 0)
        {
            vely = -vely;
        }
        else if (getY(ball)+getWidth(ball) >= HEIGHT)
        {      
           
            lives--;
            setLocation(ball, (WIDTH/2)-10, HEIGHT/2);
            waitForClick();
           
            
        }   
        
        GObject obj = detectCollision(window, ball);
        if(obj != NULL)
        {
            if(strcmp(getType(obj), "GRect")==0)
            {
                vely = -vely;
                if(obj != paddle)
                {
                    removeGWindow(window, obj);
                    bricks--;
                    points++;
                    
                    
                }
            }
        }
        pause(10);   
    }
         
    

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    
    for(int i=0; i< 5; i++)
    {
        for(int j=0; j< 10; j++)
        {
        
            GRect brick = newGRect(0,0,35,10);
            switch(i)
            {
            case 0:
            {
            setColor(brick,"RED");
            break;
            }
            case 1:
            {
            setColor(brick,"GREEN");
            break;
            }
            case 2:
            {
            setColor(brick,"YELLOW");
            break;
            }
            case 3:
            {
            setColor(brick,"BLUE");
            break;
            }
            case 4:
            {
            setColor(brick,"ORANGE");
            break;
            }
            }
            
            setFilled(brick, true);
            setLocation(brick,(2.5) + 40*j,50 + (i*15) );
            add(window,brick);
        
            
        } 
          
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    
    GOval ball = newGOval((WIDTH/2)-10, HEIGHT/2, 20, 20);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
   
   GRect paddle = newGRect((WIDTH/2) - 30 ,HEIGHT - 50 ,60 ,10);
   add(window,paddle);
   setColor(paddle,"BLACK");
   setFilled(paddle, true);
   return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    char t[20] = "Score: ";
    sprintf(s, "%i", points);
    strcat(t,s);
    setLabel(label, t);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

