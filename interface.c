#include "interface.h"

//======================================================================
bool is_number(char number[]){
    for(size_t i = 0; i < strlen(number); ++i){
        if(!isdigit(number[i])){
            if(number[i] != '.' && number[i] != ',')
                return false;
        }
    }
    return true;
}

//======================================================================
bool is_option(char str[]){
    return str[0] == '-' && strlen(str) > 1;
}


//======================================================================
bool is_path_option(char str[]){
    return (strncmp(str, "--src=", 6) == 0) || (strncmp(str, "--dest=", 7) == 0);
}

//======================================================================
bool is_ppm_path_file(char _ppm_path[]){

    if (access(_ppm_path, F_OK) == -1){
        fprintf(stderr, "Error : file not found, or you do not have permissions.\n");
        return false;
    }

    char substring[5] = "";
    memcpy(substring, &_ppm_path[strlen(_ppm_path)]-4, 4);
    if(strcmp(substring, ".ppm") != 0){
        fprintf(stderr, "Warning: the file does not have the .ppm extension.\n");
    }

    return true;
}

//======================================================================
char* add_to_path_name(char _result[], char _path[], char _str_to_add[]){
    char* dot_pos = strrchr(_path, '.');

    if(dot_pos == NULL){
        strcpy(_result, _path);
        strcat(_result, _str_to_add);
    }else{
        memcpy(_result, _path, dot_pos-_path);
        strcat(_result, _str_to_add);
        strcat(_result, ".ppm");
    }

    return _result;
}

//======================================================================
t_option is_valid_option(option _option){

    //Check for each option possible if the number
    //of argument provided is correct and if its type too.

    if(!strcmp(_option.argv[0], "--save-separately")){
        return SAVE_SEPARATELY;
    }else if(!strcmp(_option.argv[0], "-h") || !strcmp(_option.argv[0], "--help")){
        return HELP;
    }else if(!strcmp(_option.argv[0], "-g") || !strcmp(_option.argv[0], "--gray")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return GRAYSCALE;
    }else if(!strcmp(_option.argv[0], "-s") || !strcmp(_option.argv[0], "--sepia")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return SEPIA;
    }else if(!strcmp(_option.argv[0], "-n") || !strcmp(_option.argv[0], "--negative")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return NEGATIVE;
    }else if(!strcmp(_option.argv[0], "-t") || !strcmp(_option.argv[0], "--thresholding")){
        if(_option.argc < 1){
            fprintf(stderr, "Missing argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"threshold\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
        }
        return  THRESHOLDING;
    }else if(!strcmp(_option.argv[0], "--flipx")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return FLIPX;
    }else if(!strcmp(_option.argv[0], "--flipy")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return FLIPY;
    }else if(!strcmp(_option.argv[0], "--shift-left")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return SHIFT_LEFT;
    }else if(!strcmp(_option.argv[0], "--shift-right")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return SHIFT_RIGHT;
    }else if(!strcmp(_option.argv[0], "-P") || !strcmp(_option.argv[0], "--pixelize")){

        if(_option.argc < 2){
            fprintf(stderr, "Missing argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Pixel size\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
        }
        return PIXELIZE;
    }else if(!strcmp(_option.argv[0], "-p") || !strcmp(_option.argv[0], "--posterize")){

        if(_option.argc < 2){
            fprintf(stderr, "Missing argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Pixel size\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
        }
        return POSTERIZE;
    }else if(!strcmp(_option.argv[0], "-r") || !strcmp(_option.argv[0], "--rotate")){

        if(_option.argc < 2){
            fprintf(stderr, "Missing argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Angle\" Argument must be a number for %s command.\n", _option.argv[0]);
            return -1;
        }
        return ROTATE;
    }else if(!strcmp(_option.argv[0], "-b") || !strcmp(_option.argv[0], "--blur")){

        if(_option.argc < 2){
            fprintf(stderr, "Missing argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc > 3){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Box size\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
        }else if(_option.argc == 3){
            if(!is_number(_option.argv[2])){
            fprintf(stderr, "\"Repetition\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
            }
        }
        return BLUR;
    }else if(!strcmp(_option.argv[0], "-L4") || !strcmp(_option.argv[0], "--laplace-4")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return LAPLACE_4;
    }else if(!strcmp(_option.argv[0], "-L8") || !strcmp(_option.argv[0], "--laplace-8")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return LAPLACE_8;
    }else if(!strcmp(_option.argv[0], "-B8") || !strcmp(_option.argv[0], "--laplace-8bis")){
        if(_option.argc > 1){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[0]);
            return -1;
        }
        return LAPLACE_8BIS;
    }else if(!strcmp(_option.argv[0], "-G") || !strcmp(_option.argv[0], "--gaussian")){
        if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[1]);
            return -1;
        }else if(_option.argc == 2){
            if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Repetition\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
            }
        }
        return GAUSSIAN;
    }else if(!strcmp(_option.argv[0], "-S") || !strcmp(_option.argv[0], "--sobel")){
        if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[1]);
            return -1;
        }
        return SOBEL;
    }else if(!strcmp(_option.argv[0], "-R") || !strcmp(_option.argv[0], "--prewitt")){
        if(_option.argc > 2){
            fprintf(stderr, "Unexpected extra argument for %s command.\n", _option.argv[1]);
            return -1;
        }else if(!is_number(_option.argv[1])){
            fprintf(stderr, "\"Pixel size\" Argument must be an integer for %s command.\n", _option.argv[0]);
            return -1;
        }
        return PREWITT;
    }

    fprintf(stderr, "Unrecognised command \"%s\".\n", _option.argv[0]);
    return -1;

}

//======================================================================
bool check_param(param* _param){

    bool res = true;

    //Check if a source file is provided
    if(!strcmp(_param->source_name, "")){
        fprintf(stderr, "Please give source image path.\n");
        return false;
    }

    //Check if a destination file is provided, set it to source path otherwise
    if(strcmp(_param->dest_name, "") == 0){
        printf("No destination file provided the original file will be modified\n");
        strcpy(_param->dest_name, _param->source_name);
    }

    if(!is_ppm_path_file(_param->source_name)){
        return false;
    }

    //Check each options
    for(int i = 0; i < _param->optionc; ++i){

        int option_type = is_valid_option(_param->optionv[i]);
        if(option_type == -1){
            res = false;
        }else if(option_type == SAVE_SEPARATELY){
            _param->save_separately = true;
        }
        _param->optionv[i].type = option_type;
    }
    return res;
}

//======================================================================
void free_param(param _param){
    for(int i = 0; i < _param.optionc; ++i){
        free(_param.optionv[i].argv);
    }
    free(_param.optionv);
}

//======================================================================
size_t count_options(int argc, char *argv[]){
    size_t m_count = 0;
    for(int i = 1; i < argc; ++i){
        if(is_option(argv[i])){
            if(!is_path_option(argv[i])){
                m_count++;
            }
        }
    }
    return m_count;
}

//======================================================================
param parse_arg(int argc, char *argv[]){

    char m_source[255] = "";
    char m_destination[255] = "";

    size_t m_option_count = count_options(argc, argv);
    size_t current_option = 0;
    option* m_options = malloc(m_option_count * sizeof (option));

    for(int i = 1; i < argc; ++i){

        if(argv[i][0] == '-'){

            if(strlen(argv[i]) == 1){
                fprintf(stderr, "Error invalid argument : %s\n", argv[i]);
                break;
            }

            //check if it's the source path argument
            if(strncmp(argv[i], "--src=", 6) == 0){

                char substring2[255];
                memcpy(substring2, &argv[i][6], 255);
                substring2[254] = '\0';

                strcpy(m_source, substring2);

            //check if it's the destination path argument
            }else if(strncmp(argv[i], "--dest=", 7) == 0){

                char substring2[255];
                memcpy(substring2, &argv[i][7], 255);
                substring2[254] = '\0';

                strcpy(m_destination, substring2);
            }

            else{

                //Count the number of argument passed to the option
                int j = 1;
                if(i+j < argc){
                    while(!is_option(argv[i+j])){
                        j++;
                        if(i+j == argc){
                            break;
                        }
                    }
                }

                char** m_optionv = malloc((j)*sizeof (char*));

                for(int k = i; k < j+i; ++k){
                    m_optionv[k-(i)] = argv[k];
                }

                option m_option;
                m_option.argc = j;
                m_option.argv = m_optionv;

                m_options[current_option] = m_option;
                current_option++;
                i = j+i-1;
            }
        }
    }

    param m_param;
    strcpy(m_param.source_name, m_source);
    strcpy(m_param.dest_name, m_destination);
    m_param.save_separately = false;
    m_param.optionc = m_option_count;
    m_param.optionv = m_options;


    return m_param;
}

//======================================================================
void process_option(option _option, image* _image){

    //Apply the filter to the image according to the type of option
    switch (_option.type) {
        case GRAYSCALE:
            to_grayscale(_image);
            break;
        case NEGATIVE:
            to_negative(_image);
            break;
        case SEPIA:
            to_sepia(_image);
            break;
        case THRESHOLDING:
            if(_option.argc == 2)
                thresholding(_image, strtol(_option.argv[1], NULL, 10));
            else
                thresholding(_image, 128);
            break;
        case PIXELIZE:
            pixelize(_image, strtol(_option.argv[1], NULL, 10));
            break;
        case POSTERIZE:
            posterize(_image, strtol(_option.argv[1], NULL, 10));
            break;
        case BLUR:
            if(_option.argc == 2)
                box_blur(_image, strtol(_option.argv[1], NULL, 10), 0);
            else
                box_blur(_image, strtol(_option.argv[1], NULL, 10), strtol(_option.argv[2], NULL, 10));
            break;
        case LAPLACE_4:
            laplace_4(_image);
            break;
        case LAPLACE_8:
            laplace_8(_image);
            break;
        case LAPLACE_8BIS:
            laplace_8Bis(_image);
            break;
        case GAUSSIAN:
            if(_option.argc == 2)
                gaussian_filter(_image, strtol(_option.argv[1], NULL, 10));
            else
                gaussian_filter(_image, 1);
        break;
        case SOBEL:
            sobel_filter(_image);
            break;
        case PREWITT:
            prewitt_filter(_image);
            break;
        case FLIPX:
            flip_x(_image);
            break;
        case FLIPY:
            flip_y(_image);
            break;
        case SHIFT_LEFT:
            shift_component_to_left(_image);
            break;
        case SHIFT_RIGHT:
            shift_component_to_right(_image);
            break;
        case ROTATE:
            rotate(_image, strtol(_option.argv[1], NULL, 10));
            break;
        case SAVE_SEPARATELY:
            break;
        case HELP:
            print_help();
            break;
        default:
            fprintf(stderr, "An error occured.\n");
            break;
    }
}

//======================================================================
int process(param _param){

    if(!check_param(&_param)){
        print_help();
        return -1;
    }

    //Save each result separately
    if(_param.save_separately){

        image* m_image = read_ppm(_param.source_name);
        char current[255] = "";

        for(int i = 0; i < _param.optionc; ++i){

            if(_param.optionv[i].type == HELP){
                print_help();
            }else if(_param.optionv[i].type != SAVE_SEPARATELY){
                image* temp_image = copy_image(m_image);
                process_option(_param.optionv[i], temp_image);

                //Get number of the current image
                current[0] = '\0';
                sprintf(current, "%d", i);
                char path_name[255] = "";

                //Save the image with the modified name
                save_ppm(temp_image, add_to_path_name(path_name, _param.dest_name, current));
                free_image(temp_image);
            }
        }

        free_image(m_image);
    //Save the combination of all filter in one file
    }else{
        image* m_image = read_ppm(_param.source_name);

        for(int i = 0; i < _param.optionc; ++i){
            process_option(_param.optionv[i], m_image);
        }

        save_ppm(m_image, _param.dest_name);
        free_image(m_image);
    }

    return 1;
}


//======================================================================
void print_help(){
    printf("\nUses : image_tool [OPTION...] [dest=DESTINATION_PATH] src=SOURCE_PATH\n\n"
           "    --gray, -g              convert the image in grayscale\n\n"
           "    --sepia, -s             apply a sepia filter to the image\n\n"
           "    --negative, -n          convert the image in negative\n\n"
           "    --thresholding [value]  apply a threshold if [value] on the image\n"
           "    -t [value]              if [value] isn't provided it's by default 127\n\n"
           "    --posterize bits,       decrease the number of colors in the image,\n"
           "    -p bits                 bits correspond to the number of value\n"
           "                            that can be taken by a component\n\n"
           "    --pixelize size,        pixelize the image with pixels of size \"size\"\n"
           "    -P size\n\n"
           "    --shift-right           shift red, green, blue to the right, \n"
           "                            red become blue, green red and blue green\n\n"
           "    --shift-left            the same as --shift-right but to the left\n\n"
           "    --blur size [rep]       apply a low-pass filter with a box size defined by \"size\" argument\n"
           "    -b size [rep]           if [rep] is provided it's repeated [rep] time\n\n"
           "    --laplace-4, -L4        apply the high-pass filter laplace\n\n"
           "    --laplace-8, -L8        apply the high-pass filter laplace\n\n"
           "    --laplace-8bis, -B8     apply the high-pass filter laplace\n\n"
           "    --gaussian [rep]        apply a gaussian low-pass filter\n"
           "    -G [rep]                if [rep] is provided it's repeated [rep] time\n\n"
           "    --sobel, -S             apply the detection contour filter named \"sobel\"\n\n"
           "    --prewitt, -R           apply the detection contour filter named \"prewitt\"\n\n"
           "    --src=SOURCE_PATH       Needed argument to provide the source path image\n\n"
           "    --dest=DEST_PATH        Optional argument to give the destination path\n"
           "                            where the result will be saved\n"
           "                            if not provided the original file will be modified\n\n"
           "    --save-separately       save the result of each option provided in a different file\n\n");
}
