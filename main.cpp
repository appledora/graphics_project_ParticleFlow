#include <stdlib.h>
#include<GL/freeglut.h>
#include <math.h>
#include <iostream>

using namespace std;

const int MaxParticles = 200;        //Constant for Particles array maximum
int currentParticles = 50;            //number of particles on the screen

double mouseX;                        //Mouse x and y coordinate, initialized by function callbacks
double mouseY;
double sizeChange = 1.0;                //Size and Speed of particle change variable
double speedChange = 20.0;
int range = 8000;                        //Pull Push range
int pullPush;
int pause;

typedef struct {
    double XCoor;
    double YCoor;
    double dX;                //dX and dY = velocity/direction
    double dY;
    double Red;
    double Green;
    double Blue;
    double size;
    bool affected;            //if the particle has been affected by pull or push
} PARTICLE;

PARTICLE Particles[MaxParticles];

void initParticles(void) {
    int i;
    for (i = 0; i < currentParticles; i++) {            //for each particle we set random variables for all parameters
        Particles[i].XCoor = rand() % 600 * 1.0;
        Particles[i].YCoor = rand() % 600 * 1.0;
        Particles[i].Blue = rand() % 2;
        Particles[i].Red = rand() % 2;
        Particles[i].Green = rand() % 2;
        Particles[i].dX = (rand() % 10 * 0.006) * pow(-1.0, rand() % 2);
        Particles[i].dY = (rand() % 10 * 0.006) * pow(-1.0, rand() % 2);
        Particles[i].size = (rand() % 12 + 5);
        Particles[i].affected = false;
    }
    pause = 1;                //set pause to 1 in case of restart at pause
}

void display(void) {
    if (pause == 0) { // when paused prints the keymap
        glRasterPos2f(250.0, 500.0);   //When paused, specify the raster position for pixel operations
        glColor3f(1.0, 1.0, 1.0); // basically the window background color

        string str1("PAUSED");
        for (int i = 0; i < str1.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,
                                str1[i]);    //Write each character of the string as bitmapCharacter
        }
        glRasterPos2f(150.0, 400.0);
        string str2("Controls");
        for (int i = 0; i < str2.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);
        }
        glRasterPos2f(200.0, 350.0);
        string str3("q-e = Increase/Decrease Speed");
        for (int i = 0; i < str3.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str3[i]);
        }
        glRasterPos2f(200.0, 300.0);
        string str4("a-d = Add/Delete Particle");
        for (int i = 0; i < str4.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str4[i]);
        }
        glRasterPos2f(200.0, 250.0);
        string str5("z-c Increase/Decrease Size");
        for (int i = 0; i < str5.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str5[i]);
        }
        glRasterPos2f(200.0, 200.0);
        string str6("w-x Increase/Decrease Range");
        for (int i = 0; i < str6.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str6[i]);
        }
        glRasterPos2f(200.0, 150.0);
        string str7("Left Click = Pull");
        for (int i = 0; i < str7.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str7[i]);
        }
        glRasterPos2f(200.0, 100.0);
        string str8("Right Click = Push");
        for (int i = 0; i < str8.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str8[i]);
        }
        glRasterPos2f(500.0, 50.0);
        string str9("Alt + Q = Quit");
        for (int i = 0; i < str9.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str9[i]);
        }
        glRasterPos2f(500.0, 75.0);
        string str10("R = Reset");
        for (int i = 0; i < str10.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str10[i]);
        }
    } else if (pause == 1) {

        int i;                            //if not paused, run display function normally
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.15f, 0.15f, 0.3f, 0.0f);
        glRasterPos2f(535.0, 5.0);
        string str11("P - Pause");
        for (int j = 0; j < str11.size(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str11[j]);
        }
        for (i = 0; i < currentParticles; i++) {

            Particles[i].XCoor += Particles[i].dX * speedChange;    //Logic for moving the particle
            Particles[i].YCoor += Particles[i].dY * speedChange;

            if ((Particles[i].XCoor <= 1) | (Particles[i].XCoor >= 599)) { //Logic for bouncing off the wall
                Particles[i].dX = Particles[i].dX * (1);
            }
            if ((Particles[i].YCoor <= 1) | (Particles[i].YCoor >= 599)) {
                Particles[i].dY = Particles[i].dY * (-1);
            }

            /*mouseX/Y is set to the mouse location in x and y and used here, it is then reversed to mouseX\Y = (-1, -1)
            for no interference, this is used when the LMB is pressed and the particle stops at the cursor instead of keep going*/
            if (Particles[i].XCoor + 1 >= mouseX & Particles[i].XCoor - 1 <= mouseX & Particles[i].YCoor + 1 >= mouseY &
                Particles[i].YCoor - 1 <= mouseY) {
                Particles[i].dX = 0;
                Particles[i].dY = 0;
            }

            //this loop produces the sense of particles going through one end of the window and returning back through
            //the opposite end of the window.
            for (int j = 0; j <= currentParticles; j++) {
                if ((Particles[i].YCoor > 601) || (Particles[i].YCoor < -1)) {
                    Particles[i].YCoor = 599;
                }
                if (Particles[i].XCoor < 1) {
                    Particles[i].XCoor = 599;
                } else if (Particles[i].XCoor > 599) {
                    Particles[i].XCoor = 1;
                }
            }
            if (Particles[i].affected == true) {    //If the particle is affected by pull or push, make it move properly
                double temp1;
                double temp2;
                if (pullPush == 0) {                    //pullPush == 0 is pull, 1 is for push
                    for (int i = 0; i < currentParticles; i++) {   /*Calculate relative distance in both x and y between
                                                                     * each particle and mouse*/
                        temp1 = Particles[i].XCoor - mouseX;
                        temp2 = (Particles[i].YCoor) - mouseY;

                        if (pow(temp1, 2) + pow(temp2, 2) <= range) {    //Relate the distance in between with the range
                            Particles[i].dX = +temp1 * 0.0015;        //Change direction of particle
                            Particles[i].dY = +temp2 * 0.0015;
                        }
                    }
                }

                if (pullPush == 1) {


                    for (int i = 0; i < currentParticles; i++) {
                        temp1 = Particles[i].XCoor - mouseX;
                        temp2 = Particles[i].YCoor - mouseY;

                        if (pow(temp1, 2) + pow(temp2, 2) <= range) {
                            Particles[i].dX = -temp1 * 0.0015;
                            Particles[i].dY = -temp2 * 0.0015;
                        }
                    }
                }
            }

            // define each point with their already initialized values
            glPointSize(Particles[i].size * sizeChange);    //Set size
            glBegin(GL_POINTS);                            //Draw particle with openGL
            glColor3f(Particles[i].Red, Particles[i].Green, Particles[i].Blue);
            glVertex2f(Particles[i].XCoor, Particles[i].YCoor);
            glRasterPos2f(400.0, 400.0);
            glEnd();
            glColor3f(0.8, 1.0, 0.8);
        }
    }

    glutSwapBuffers();
}

void kbd(unsigned char key, int x, int y) {
    y = 600 - y;        //600 - y = fixed coordinate system; 600 is window h&d

    //Keyboard logic for all keys used
    if (glutGetModifiers() == GLUT_ACTIVE_ALT && (key == 'q' || key == 'Q')) {
        exit(0);    //Exit Program
    }

    if (key == 'p' || key == 'P') {    //Set pause variable to active or inactive
        if (pause == 1) {
            pause = 0;
        } else if (pause == 0) {
            pause = 1;
        }
    }
/*    //Be able to change the pull push range between 6500 and 30000
    if (key == 'w' || key == 'W') { //increase
        range += 500;
        if (range >= 30500) {
            range = 30000;
        }
    }
    if (key == 'x' || key == 'X') {
        range -= 500;
        if (range <= 6000) {
            range = 6500;
        }
    }*/
//q and e Change the speed
    if (key == 'q' || key == 'Q') {        // decrease speed
        speedChange += 1;
        if (speedChange >= 800) {
            speedChange = 499;
        }
    }

    if (key == 'e' || key == 'E') { //increase speed
        speedChange -= 1;
        if (speedChange <= -800) {
            speedChange = -499;
        }
    }
//z and c change the particle size
    if (key == 'z' || key == 'Z') {    //increase size
        sizeChange += 0.2;
        if (sizeChange >= 5.2) {
            sizeChange = 5;
        }
    }
    if (key == 'c' || key == 'C') { //decrease size
        sizeChange -= 0.2;
        if (sizeChange <= 0.0) {
            sizeChange = 0.2;
        }
    }
    if (key == 'r' || key == 'R') {    // reset frame
        currentParticles = 50;
        initParticles();
    }


    if (key == 'a' || key == 'A') {    //Add a particle
        glBegin(GL_POINTS);            //Draws a particle with the next element in the array by setting its parameters to randomly
        glColor3f(Particles[currentParticles].Red = rand() % 2, Particles[currentParticles].Green = rand() % 2,
                  Particles[currentParticles].Blue = rand() % 2); //randomized color values
        Particles[currentParticles].dX = (rand() % 10 * 0.01) * pow(-1.0, rand() % 2) * pause;
        Particles[currentParticles].dY = (rand() % 10 * 0.01) * pow(-1.0, rand() % 2) * pause;
        Particles[currentParticles].size = rand() % 12 + 5;
        //location is not random so we set it next to the mouse coordinates
        glVertex2i(Particles[currentParticles].XCoor = x,
                   Particles[currentParticles].YCoor = y); //x and y are cursor positions
        glEnd();
        currentParticles += 1;        //We add 1 to current particles to keep track of how many particles in the array to draw
        if (currentParticles > MaxParticles) {    //Set max limit on particles
            currentParticles = MaxParticles;
        }
    }
    if (key == 'd' || key == 'D') {    //Delete nearest particle
        int i;
        double temp1;
        double temp2;
        double temp3 = 1000;        //Temp3 set to 1000 as buffer
        int slot = 0;
        /*This loop goes through all current particles, calculates distance between particle and cursor,
         * then it compares the result with the previous particles result, if the distance is lower,
         * then record its slot number in slot*/
        for (i = 0; i < currentParticles; i++) {
            temp1 = Particles[i].XCoor - x;
            temp2 = Particles[i].YCoor - y;
            if ((fabs(temp1) + fabs(temp2)) < temp3) {
                slot = i;
                temp3 = fabs(temp1) + fabs(temp2);
            }
        }
        for (i = slot; i < currentParticles; i++) {        //Starting from the particle assigned to Particles[slot],
            // bring one element forward,
            Particles[i] = Particles[i + 1];            //rewriting the one we wish to delete and reorganizing the array
        }
        currentParticles -= 1;        //Decrease array by one and check lower limit
        if (currentParticles < 0) {
            currentParticles = 0;
        }
    }

}

void idle(void) {         /* If you don’t tell the mainloop it needs to draw the next frame with a glutPostRedisplay
                          then your animation will look as if it is stuck*/
    glutPostRedisplay();
}

//for push and pull actions

void mouse(int btn, int state, int x, int y) {

    y = 600 - y; //Y Coordinate fixed; without it the range calculation gets confusing and the particles fly inside

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //When LMB is pushed set mouseX\Y for (-1,-1) to mouse coordinates
        pullPush = 0;
        mouseX = x;
        mouseY = y;

        int i;
        double temp1;
        double temp2;
        //Logic for moving particle towards mouse
        for (i = 0; i < currentParticles; i++) {
            temp1 = Particles[i].XCoor - x;
            temp2 = Particles[i].YCoor - y;

            if (pow(temp1, 2) + pow(temp2, 2) <= range) {
                Particles[i].dX = -temp1 * 0.0015 * pause;
                Particles[i].dY = -temp2 * 0.0015 * pause;
                Particles[i].affected = true; // in the range of pull
            }

        }

    }

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        //Set mouseX\Y back to their default (-1,-1)
        mouseX = -1;
        mouseY = -1;

        //When LMB depressed randomized particles speed and direction
        int i;
        for (i = 0; i < currentParticles; i++) {
            if (Particles[i].affected == true) {
                Particles[i].dX = (rand() % 10 * 0.005) * pow(-1.0, rand() % 2) * pause; // randomized location
                Particles[i].dY = (rand() % 10 * 0.005) * pow(-1.0, rand() % 2) * pause; // randomized location
                Particles[i].affected = false; // neutral position
            }
        }
    }

    if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        //Viceversa to LMB
        pullPush = 1;
        mouseX = x;
        mouseY = y;
        int i;
        double temp1;
        double temp2;

        for (i = 0; i < currentParticles; i++) {
            temp1 = Particles[i].XCoor - x;
            temp2 = Particles[i].YCoor - y;

            if (pow(temp1, 2) + pow(temp2, 2) <= range) {
                Particles[i].dX = temp1 * 0.00125 * pause;
                Particles[i].dY = temp2 * 0.00125 * pause;
                Particles[i].affected = true;
            }
        }

    }

    if (btn == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        mouseX = -1;
        mouseY = -1;
        int i;
        for (i = 0; i < currentParticles; i++) {
            if (Particles[i].affected == true) {
                Particles[i].dX = (rand() % 10 * 0.005) * pow(-1.0, rand() % 2) * pause;
                Particles[i].dY = (rand() % 10 * 0.005) * pow(-1.0, rand() % 2) * pause;
                Particles[i].affected = false;
            }
        }
    }
}

void motion(int x, int y) {
    y = 600 - y;
    mouseX = x;
    mouseY = y;
    int i;
    double temp1;
    double temp2;
    if (pullPush == 0) {        /*When pullPush = Pull (0), move particles towards mouse when mouse in motion
                                 * if particle in range*/
        for (i = 0; i < currentParticles; i++) {
            temp1 = Particles[i].XCoor - x;
            temp2 = Particles[i].YCoor - y;

            if (pow(temp1, 2) + pow(temp2, 2) <= range) { //pulling by decreasing the render position of each particle
                Particles[i].dX = -temp1 * 0.0015 * pause;
                Particles[i].dY = -temp2 * 0.0015 * pause;
                Particles[i].affected = true;
            }
        }
    }

    if (pullPush == 1) {        //Move particles away from mouse if in range, then it is affected
        for (i = 0; i < currentParticles; i++) {
            temp1 = Particles[i].XCoor - x;
            temp2 = Particles[i].YCoor - y;

            if (pow(temp1, 2) + pow(temp2, 2) <= range) {
                Particles[i].dX = temp1 * 0.0015 * pause;
                Particles[i].dY = temp2 * 0.0015 * pause;
                Particles[i].affected = true;
            }
        }
    }
}


int main(int argc, char **argv) {

    glutInit(&argc, argv);        //starts up GLUT

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 600);
    initParticles();
    glutCreateWindow("Particle System");    //creates the window
    glutInitDisplayMode(GLUT_DOUBLE); /*double buffered scenario (GLUT_DOUBLE), there exist two framebuffer.
                                        *  One is used for drawing, the other one for display. At the end of each frame,
                                        *  these buffers are swapped*/
    glutDisplayFunc(display); //set the  base function to display
    glutKeyboardFunc(kbd);
    glutMotionFunc(motion); /*The motion callback for a window is called when the mouse moves within the window while
                             * one or more mouse buttons are pressed.*/
    glutMouseFunc(mouse); /*When a user presses and releases mouse buttons in the window, each press and each releas
                           * e generates a mouse callback*/
    glutReshapeWindow(600, 600);
    gluOrtho2D(0, 600, 0, 600); // sets up a two-dimensional orthographic viewing region
    glutIdleFunc(idle); /*sets the global idle callback to be func so a GLUT program can perform background processing
                         * tasks or continuous animation when window system events are not being received. If enabled, the idle callback
                         * is continuously called when events are not being received. */

    glutMainLoop();                //starts the event loop

    return (0);
}