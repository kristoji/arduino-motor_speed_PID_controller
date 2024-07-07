/**************************************************\
 * @file plot.c                         
 *
 * @brief Implementation of the functions to plot data
\**************************************************/

#include "plot.h"

FILE *gnuplotPipe = NULL;

void init_gnuplot() { 
    gnuplotPipe = popen("gnuplot -persistent", "w");

    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Error: Cannot open Gnuplot pipe\n");
    } else {
        fprintf(gnuplotPipe, "set terminal wxt size 1000,800\n");
        fprintf(gnuplotPipe, "set title 'Robot Space' font ',20'\n");
        fprintf(gnuplotPipe, "set xlabel 'X' font '10:bold'\n");
        fprintf(gnuplotPipe, "set ylabel 'Y' font '10:bold'\n");
        fprintf(gnuplotPipe, "set yrange [-100:100]\n");
        fprintf(gnuplotPipe, "set xrange [-100:100]\n");
        fprintf(gnuplotPipe, "set key outside\n");
        fprintf(gnuplotPipe, "set label font ',12'\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set lmargin at screen 0.15\n"); // Adjust left margin
        fprintf(gnuplotPipe, "set rmargin at screen 0.85\n"); // Adjust right margin
        fprintf(gnuplotPipe, "set tmargin at screen 0.85\n"); // Adjust top margin
        fprintf(gnuplotPipe, "set bmargin at screen 0.15\n"); // Adjust bottom margin
    
    }
}

void close_gnuplot() {
    if (gnuplotPipe != NULL) {
        pclose(gnuplotPipe);
        gnuplotPipe = NULL;
    }
}

void plot_with_gnuplot(float x, float y, float theta, float v, float w) {

    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Error: Gnuplot pipe is not open\n");
        return;
    }

    // Calculate triangle vertices
    float size = 5; // Size of the triangle
    float x1 = x + size * cos(theta);
    float y1 = y + size * sin(theta);
    float x2 = x + size * cos(theta + 2.0 * M_PI / 3.0);
    float y2 = y + size * sin(theta + 2.0 * M_PI / 3.0);
    float x3 = x + size * cos(theta - 2.0 * M_PI / 3.0);
    float y3 = y + size * sin(theta - 2.0 * M_PI / 3.0);

    // Add the labels for v and w
    fprintf(gnuplotPipe, "unset label 1\n");
    fprintf(gnuplotPipe, "unset label 2\n");
    fprintf(gnuplotPipe, "unset label 3\n");
    fprintf(gnuplotPipe, "set label 1 'Velocity' at screen 0.87,0.8 textcolor rgb 'black' font ',14:bold'\n");
    fprintf(gnuplotPipe, "set label 2 'v: %f' at screen 0.87,0.75 font ',12'\n", v);
    fprintf(gnuplotPipe, "set label 3 'w: %f' at screen 0.87,0.7 font ',12'\n", w);
    // Plot the triangle
    fprintf(gnuplotPipe, "plot '-' with filledcurves closed lt 1 lc rgb 'dark-spring-green' notitle, '-' with points pt 7 ps 1 lc rgb 'red' notitle\n\n");
    fprintf(gnuplotPipe, "%f %f\n", x1, y1);
    fprintf(gnuplotPipe, "%f %f\n", x2, y2);
    fprintf(gnuplotPipe, "%f %f\n", x3, y3);
    fprintf(gnuplotPipe, "%f %f\n", x1, y1);
    fprintf(gnuplotPipe, "e\n");
    fprintf(gnuplotPipe, "%f %f\n", x1, y1);
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe);
}

// int main() {
//     // Example usage:
//     float x = 0, y = 0, theta = -5.038246, v = -1.693074, w = -0.564358;
    
//     init_gnuplot();
//     // plot_with_gnuplot(x, y, theta, v, w);

//     // x = 8.0, y = 14.0, theta = -5.0, v = -1.7, w = -0.56;
//     plot_with_gnuplot(x, y, 0, v, w);
//     sleep(1);
//     plot_with_gnuplot(x+10, y, M_PI, v + 1, w);
//     sleep(1);
//     plot_with_gnuplot(x+10, y+10, M_PI / 2, v+1, w);
//     sleep(1);
//     close_gnuplot();
//     return 0;
// }
