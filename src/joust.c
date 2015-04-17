/*
  joust.c
  Created as a solution for reddit.com/r/dailyprogrammer Hard Challenge #123
  Calculates when and if robots collide with eachother when heading down a corridor
  Written in ANSI C
  Joshua Tyler 2014
  Released under MIT license
*/
	
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVAL 1 /* Number of milliseconds to iterate each time */
#define ROBOT_WIDTH 0.25 /* In metres */
#define WALL_DAMPING_FACTOR 0.9 /* Proportion that speed is reduced by upon colliding with the wall */
#define PI 3.141592654

typedef struct
{
  /* Position co-ordinates in metres, (0,0) is far top left */
  double x;
  double y;

  double speed; /* In metres per millisecond */

  double angle; /* In radians anticlockwise from horizontally right */

} robot;

/* Function prototypes */
int iterateRobot(robot *robotToIterate, int interval);
int collisionDetect(robot *robot1, robot *robot2);

int boardWidth, boardHeight; /* In metres */

int main(int argc, char  *argv[])
{
  if(argc != 2){
    fprintf(stderr, "Incorrect arguments.\n"
       "Usage: %s <path to input file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  fp = fopen(argv[1], "r");

  if(fp == NULL){
    fputs("Error opening input file.\n", stderr);
    exit(EXIT_FAILURE);
  }

  /* Scan input file to varibles.
     No error checking in this section! */

  extern int boardWidth, boardHeight;
  robot leftRobot, rightRobot;

 /* Board dimensions */
  fscanf(fp, "%d%d", &boardWidth, &boardHeight);

  /* Left Robot */
  fscanf(fp, "%lf%lf", &leftRobot.angle, &leftRobot.speed);

  leftRobot.x = ROBOT_WIDTH;
  leftRobot.y = (double)boardHeight/2.0;
  leftRobot.speed = leftRobot.speed * 1e-6; /* Convert to m/ms */
  leftRobot.angle = -(leftRobot.angle) * ((double)PI/180); /* Convert to radians anticlockwise from right */

  /* Right Robot */
  fscanf(fp, "%lf%lf", &rightRobot.angle, &rightRobot.speed);

  fclose(fp);

  rightRobot.x = (double)boardWidth - ROBOT_WIDTH;
  rightRobot.y = (double)boardHeight/2.0;
  rightRobot.speed = rightRobot.speed * 1e-6; /* Convert to m/ms */
  rightRobot.angle = PI - (rightRobot.angle) * ((double)PI/180); /* Convert to radians anticlockwise from right */

  int time; /*In milliseconds (thousandths of a second)*/

  int collision; /* 1 if a collision occured, 0 if it did not */

  for(time = 0, collision = 1; !collisionDetect(&leftRobot, &rightRobot) ; ) {
    if(iterateRobot(&leftRobot, INTERVAL)){
      collision = 0;
      break;
    }

    if(iterateRobot(&rightRobot, INTERVAL)){
      collision = 0;
      break;
    }

    time += INTERVAL;

    }

  if(collision){
    if(abs(leftRobot.speed) > abs(rightRobot.speed))
      printf("Left robot wins at %lf seconds\n", (double)time/1000.0);
    else
      printf("Right robot wins at %lf seconds\n", (double)time/1000.0);
  }
  else
    puts("No winner found");

  return EXIT_SUCCESS;
}

   /* Iterates the robot given to it by it's current speed, for the interval given.
       Returns 1 if the robot hits the left or right wall.
       Returns 0 if it doesn't. */
int iterateRobot(robot *robotToIterate, int interval)
{

  extern int boardWidth, boardHeight;

  robotToIterate->x = robotToIterate->x + robotToIterate->speed * cos(robotToIterate->angle);

  if( (robotToIterate->x + ROBOT_WIDTH > boardWidth) || (robotToIterate->x - ROBOT_WIDTH < 0) )
    return 1;

  robotToIterate->y = robotToIterate->y + robotToIterate->speed * sin(robotToIterate->angle);

  if(robotToIterate->y - ROBOT_WIDTH < 0){
    robotToIterate->y = ROBOT_WIDTH;
    robotToIterate->speed *= 0.9;
    robotToIterate->angle = -robotToIterate->angle;
  }

  if(robotToIterate->y + ROBOT_WIDTH > boardHeight){
    robotToIterate->y = boardHeight - ROBOT_WIDTH;
    robotToIterate->speed *= 0.9;
    robotToIterate->angle = -robotToIterate->angle;
  }

  return 0;
}

/* Checks if the two robots have collided
   Returns 1 if they have.
   Returns 0 if they have not. */
int collisionDetect(robot *robot1, robot *robot2)
{
  double distance;
  double xDiff = (robot1->x - robot2->x);
  double yDiff = (robot1->y - robot2->y);

  distance = sqrt( xDiff*xDiff + yDiff*yDiff );

  if(distance <= (ROBOT_WIDTH * 2.0) )
    return 1;

  return 0;
}
