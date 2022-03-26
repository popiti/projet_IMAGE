#ifndef INTERFACE_H
#define INTERFACE_H

#include "utility.h"
#include "image.h"

/*====================================================================*/
/*======================== TYPE DEFINITION ===========================*/
/*====================================================================*/

typedef enum{
    GRAYSCALE,
    NEGATIVE,
    SEPIA,
    THRESHOLDING,
    PIXELIZE,
    POSTERIZE,
    BLUR,
    LAPLACE_4,
    LAPLACE_8,
    LAPLACE_8BIS,
    GAUSSIAN,
    SOBEL,
    PREWITT,
    FLIPX,
    FLIPY,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    ROTATE,
    SAVE_SEPARATELY,
    HELP
}t_option;


typedef struct {
    t_option type;
    int argc;
    char **argv;
} option;


typedef struct {
    char source_name[255];
    char dest_name[255];
    bool save_separately;
    int optionc;
    option *optionv;
} param;

/*====================================================================*/
/*============================ UTILITY ===============================*/
/*====================================================================*/

//======================================================================
bool is_number(char number[]);

//======================================================================
bool is_option(char str[]);

//======================================================================
bool is_path_option(char str[]);

//======================================================================
bool is_ppm_path_file(char _ppm_path[]);

//======================================================================
char *add_to_path_name(char _result[], char _path[], char _str_to_add[]);

//======================================================================
t_option is_valid_option(option _option);

//======================================================================
bool check_param(param* _param);

//======================================================================
void free_param(param _param);

//======================================================================
size_t count_options(int argc, char *argv[]);

//======================================================================
param parse_arg(int argc, char *argv[]);

//======================================================================
void process_option(option _option, image *_image);

//======================================================================
int process(param _param);

//======================================================================
int execute(int argc, char *argv[]);

//======================================================================
void print_help();

#endif
