#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

const char default_print_char[5] = "#\0";

struct color
{
    uint8_t r, g, b;
};

struct config
{
    char print_char[5]; // Character/string to build the gradient out of (up to 4 characters)
    float width_multilpy, height_multiply; // Multipliers for width and height of the gradient
    int cycle_duration_millis;
    struct color top_left1, top_right1, bottom_left1, bottom_right1;
    struct color top_left2, top_right2, bottom_left2, bottom_right2;
};

const struct config defualt_config =
(struct config)
{
    '#',
    '\0',
    '\0',
    '\0',
    '\0',
    1.0f,
    1.0f,
    1500,
    (struct color) {255, 0, 0},
    (struct color) {0, 255, 0},
    (struct color) {0, 0, 255},
    (struct color) {255, 255, 255},
    (struct color) {255, 255, 0},
    (struct color) {0, 255, 255},
    (struct color) {255, 0, 255},
    (struct color) {255, 255, 255}
};

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

struct color lerp_colors(struct color c1, struct color c2, float t)
{
    return (struct color)
    {
        lerp(c1.r, c2.r, t),
        lerp(c1.g, c2.g, t),
        lerp(c1.b, c2.b, t)
    };
}

struct color bilerp_colors(struct color top_left, struct color top_right, struct color bottom_left, struct color bottom_right, float x, float y)
{
    return lerp_colors(lerp_colors(top_left, top_right, x), lerp_colors(bottom_left, bottom_right, x), y);
}

bool string_includes_space(char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            return true;
        }
    }

    return false;
}

void strip_junk_at_end_of_string(char* str)
{
    int i = strlen(str);

    while (i > 0)
    {
        i--;
        switch (str[i])
        {
        case ' ':
        case '\n':
            str[i] = '\0';
            break;
        default: return;
        }
    }
}

struct config get_current_config()
{
    char* home = getenv("HOME");
    char path[1024];
    memset(path, 0, 1024);
    sprintf(path, "%s/.cmovinggradient.config", home);

    FILE* file = fopen(path, "r");
    if (!file)
    {
        printf("No config file at %s\n", path);
        printf("Create the file and leave it empty to use default settings\n");
        exit(EXIT_FAILURE);
    }

    struct config current_config = defualt_config;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
    {
        strip_junk_at_end_of_string(buffer);
        if (strlen(buffer) == 0) continue;
        char variable[40];
        char value[216];
        if (sscanf(buffer, " %39s %215s", variable, value) < 2)
        {
            printf("Invalid line (not in fomat variable value) %s\n", buffer);
            exit(EXIT_FAILURE);
        }

        if (strcmp(variable, "topleft1") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.top_left1 = (struct color) {r, g, b};
        } else if (strcmp(variable, "topright1") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.top_right1 = (struct color) {r, g, b};
        } else if (strcmp(variable, "bottomleft1") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.bottom_left1 = (struct color) {r, g, b};
        } else if (strcmp(variable, "bottomright1") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.bottom_right1 = (struct color) {r, g, b};
        } else if (strcmp(variable, "topleft2") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.top_left2 = (struct color) {r, g, b};
        } else if (strcmp(variable, "topright2") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.top_right2 = (struct color) {r, g, b};
        } else if (strcmp(variable, "bottomleft2") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.bottom_left2 = (struct color) {r, g, b};
        } else if (strcmp(variable, "bottomright2") == 0)
        {
            int r, g, b;
            if (sscanf(value, "rgb(%d,%d,%d)", &r, &g, &b) < 3)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.bottom_right2 = (struct color) {r, g, b};
        } else if (strcmp(variable, "widthmultiply") == 0)
        {
            float width_mult;
            if (sscanf(value, "%f", &width_mult) < 1)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.width_multilpy = width_mult;
        } else if (strcmp(variable, "heightmultiply") == 0)
        {
            float height_mult;
            if (sscanf(value, "%f", &height_mult) < 1)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.height_multiply = height_mult;
        } else if (strcmp(variable, "cyclemillis") == 0)
        {
            float cyclemillis;
            if (sscanf(value, "%f", &cyclemillis) < 1)
            {
                printf("Invalid line %s\n", buffer);
                exit(EXIT_FAILURE);
            }
            current_config.cycle_duration_millis = cyclemillis;
        }
    }

    fclose(file);
    return current_config;
}

void sleep_ms(unsigned int ms)
{
    struct timespec req, rem;
    req.tv_sec = ms / 1000; // seconds
    req.tv_nsec = (ms % 1000) * 1000000; // nanoseconds
    nanosleep(&req, &rem);
}

float secs()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_usec / 1000000.0f + fmod(time.tv_sec, 100.0f);
}

int main()
{
    printf("\x1b[?25l");
    printf("\x1b[H\x1b[2J\x1b[3J");
    fflush(stdout);

    struct config current_config = get_current_config();
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    ws.ws_col *= current_config.width_multilpy;
    ws.ws_row *= current_config.height_multiply;
    float cycle_time_secs = current_config.cycle_duration_millis / 1000.0f  * 2;

    while (true)
    {
        printf("\x1b[H\x1b[2J\x1b[3J");
        float time = (sin(2 * M_PI * secs() * (1 / cycle_time_secs)) + 1) / 2;
        for (int y = 0; y < ws.ws_row; y++)
        {
            for (int x = 0; x < ws.ws_col; x++)
            {
                struct color c = bilerp_colors(
                    lerp_colors(current_config.top_left1, current_config.top_left2, time),
                    lerp_colors(current_config.top_right1, current_config.top_right2, time),
                    lerp_colors(current_config.bottom_left1, current_config.bottom_left2, time),
                    lerp_colors(current_config.bottom_right1, current_config.bottom_right2, time),
                    x / (float) ws.ws_col, y / (float) ws.ws_row);
                printf("\x1b[38;2;%" PRIu8 ";%" PRIu8 ";%" PRIu8 "m#", c.r, c.g, c.b);
            }
            
            printf("\n");
        }

        sleep_ms(100);
    }
}
