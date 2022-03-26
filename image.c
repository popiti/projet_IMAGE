#include "image.h"

/*====================================================================*/
/*============================ UTILITY ===============================*/
/*====================================================================*/


//======================================================================
int clamp(int _value, int _min, int _max){
    if(_value > _max){
        return _max;
    }else if(_value < _min){
        return _min;
    }
    return _value;
}

//======================================================================
float clampf(float _value, float _min, float _max){
    if(_value > _max){
        return _max;
    }else if(_value < _min){
        return _min;
    }
    return _value;
}

//======================================================================
bool skip_comment(FILE* _file, char _string[]){
    if(_string[0] == '#'){
        char c = '\0';
        do {
            fscanf(_file, "%c", &c);
        } while (c != '\n');
        return true;
    }
    return false;
}


/*====================================================================*/
/*========================= IMAGE UTILITY ============================*/
/*====================================================================*/


//======================================================================
image* create_image(size_t size_x, size_t size_y, size_t _depth, color _color){

    //Alloc a two-dimensional array for pixels
    pixel** m_pixels = (pixel**)malloc(sizeof(pixel*) * size_x);
    for(unsigned int i = 0; i < size_x; ++i){
        m_pixels[i] = malloc(sizeof(pixel) * size_y);
    }

    //Alloc the image and filled it properties
    image* m_image = (image*)malloc(sizeof(image));

    m_image->pixels = m_pixels;
    m_image->sizeX = size_x;
    m_image->sizeY = size_y;
    m_image->depth = _depth;

    //Fill the pixel array
    for(unsigned int i = 0; i < size_x; ++i){
        for(unsigned int j = 0; j < size_y; ++j){
            m_pixels[i][j].r = _color.r;
            m_pixels[i][j].g = _color.g;
            m_pixels[i][j].b = _color.b;
        }
    }

    return m_image;
}

//======================================================================
image* copy_image(image* _image){
    color m_color = {0, 0, 0};
    image* m_image = create_image(_image->sizeX, _image->sizeY, _image->depth, m_color);

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            m_image->pixels[i][j].r = _image->pixels[i][j].r;
            m_image->pixels[i][j].g = _image->pixels[i][j].g;
            m_image->pixels[i][j].b = _image->pixels[i][j].b;
        }
    }

    return m_image;
}

//======================================================================
void free_image(image* _image){
    for(size_t i = 0; i < _image->sizeX; ++i){
        free(_image->pixels[i]);
    }
    free(_image->pixels);
    free(_image);
}

//======================================================================
void save_ppm(image* _image, char name_file[]){

    //Create file stream and check if it's correctly opened
    FILE *m_file = NULL;
    m_file = fopen(name_file, "w+");

    if(m_file == NULL){
        fprintf(stderr, "Unable to open file : \"%s\".\n", name_file);
        return;
    }

    //Write image property on the file
    fprintf(m_file, "P3\n");
    fprintf(m_file, "%u ", _image->sizeX);
    fprintf(m_file, "%u\n", _image->sizeY);
    fprintf(m_file, "255\n");

    //Write pixel data
    for(unsigned int j = 0; j < _image->sizeY; ++j){
        for(unsigned int i = 0; i < _image->sizeX; ++i){

            fprintf(m_file, "%4d", (_image->pixels[i][j]).r);
            fprintf(m_file, "%4d", (_image->pixels[i][j]).g);
            fprintf(m_file, "%4d", (_image->pixels[i][j]).b);

            fprintf(m_file, "\n");

        }
    }

    fclose(m_file);
}


//======================================================================
image* read_ppm(char name_file[]){

    //open file and check if it's correctly opened
    FILE *m_file = NULL;
    m_file = fopen(name_file, "r");

    color m_color = {0, 0, 0};
    if(m_file == NULL){
        fprintf(stderr, "Unable to open file.");
        return create_image(3, 3, 255, m_color);
    }

    //Get first word, it must be "P3" for a ppm file
    char m_str[255] = "";
    do{
        fscanf(m_file, "%s", m_str);
    }
    while(skip_comment(m_file, m_str));


    if(!(strcmp(m_str, "P3") == 0)){
        fprintf(stderr, "Error : file isn't a valid ppm format, or may be corrupted.\n");
        fclose(m_file);
        return create_image(3, 3, 255, m_color);
    }

    //Create and filled image property avoiding possible comments
    size_t size_x = 0;
    size_t size_y = 0;
    size_t depth = 0;

    do{
        fscanf(m_file, "%s", m_str);
    }
    while(skip_comment(m_file, m_str));
    size_x = strtol(m_str, NULL, 10);

    do{
        fscanf(m_file, "%s", m_str);
    }
    while(skip_comment(m_file, m_str));
    size_y = strtol(m_str, NULL, 10);

    do{
        fscanf(m_file, "%s", m_str);
    }
    while(skip_comment(m_file, m_str));
    depth = strtol(m_str, NULL, 10);


    //Create the image and filled his pixel array with the file data
    image* m_image = create_image(size_x, size_y, depth, m_color);
    char m_component[3];

    for(size_t j = 0; j < size_y; ++j){
        for(size_t i = 0; i < size_x; ++i){
            fscanf(m_file, "%s", m_component);
            m_image->pixels[i][j].r = strtol(m_component, NULL, 10);

            fscanf(m_file, "%s", m_component);
            m_image->pixels[i][j].g = strtol(m_component, NULL, 10);

            fscanf(m_file, "%s", m_component);
            m_image->pixels[i][j].b = strtol(m_component, NULL, 10);
        }
    }

    fclose(m_file);
    return m_image;
}


/*====================================================================*/
/*============================ FILTERS ===============================*/
/*====================================================================*/


//======================================================================
void to_grayscale(image* _image){

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;

            byte average = (r+b+g)/3;

            _image->pixels[i][j].r = average;
            _image->pixels[i][j].g = average;
            _image->pixels[i][j].b = average;
        }
    }
}

//======================================================================
void to_sepia(image* _image){

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;


            float sr = 0.393*r + 0.769*g + 0.189*b;
            float sg = 0.349*r + 0.686*g + 0.168*b;
            float sb = 0.272*r + 0.534*g + 0.131*b;

            _image->pixels[i][j].r = clampf(sr, 0, 255);
            _image->pixels[i][j].g = clampf(sg, 0, 255);
            _image->pixels[i][j].b = clampf(sb, 0, 255);
        }
    }
}

//======================================================================
void to_negative(image* _image){

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;

            _image->pixels[i][j].r = 255-r;
            _image->pixels[i][j].g = 255-g;
            _image->pixels[i][j].b = 255-b;
        }
    }
}

//======================================================================
void thresholding(image* _image, size_t _threshold){

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            float r = _image->pixels[i][j].r;
            float g = _image->pixels[i][j].g;
            float b = _image->pixels[i][j].b;

            if((r+b+g)/3.f < _threshold){
                _image->pixels[i][j].r = 0;
                _image->pixels[i][j].g = 0;
                _image->pixels[i][j].b = 0;
            }else{
                _image->pixels[i][j].r = 255;
                _image->pixels[i][j].g = 255;
                _image->pixels[i][j].b = 255;
            }
        }
    }
}

//======================================================================
void pixelize(image* _image, size_t _pixel_size){

    if(_pixel_size < 2){
        fprintf(stderr, "Pixel size must be greater than 1, exit filter.\n");
        return;
    }

    image *m_image = copy_image(_image);

    unsigned int current_pixels_sum[3] = {0, 0, 0};
    unsigned int pixel_count = 0;

    for(unsigned int i = 0; i < m_image->sizeX; i+=_pixel_size){
        for(unsigned int j = 0; j < m_image->sizeY; j+=_pixel_size){

            current_pixels_sum[0] = 0;
            current_pixels_sum[1] = 0;
            current_pixels_sum[2] = 0;

            pixel_count = 0;

            for(unsigned int k = i; k < i+_pixel_size; ++k){
                for(unsigned int l = j; l < j+_pixel_size; ++l){
                    if((k < _image->sizeX) && (l < _image->sizeY)){

                        current_pixels_sum[0] += m_image->pixels[k][l].r;
                        current_pixels_sum[1] += m_image->pixels[k][l].g;
                        current_pixels_sum[2] += m_image->pixels[k][l].b;

                        pixel_count++;
                    }
                }
            }

            for(unsigned int k = i; k < i+_pixel_size; ++k){
                for(unsigned int l = j; l < j+_pixel_size; ++l){

                    if((k < _image->sizeX) && (l < _image->sizeY)){

                        _image->pixels[k][l].r = current_pixels_sum[0]/pixel_count;
                        _image->pixels[k][l].g = current_pixels_sum[1]/pixel_count;
                        _image->pixels[k][l].b = current_pixels_sum[2]/pixel_count;

                    }
                }
            }
        }
    }

    free_image(m_image);
}

//======================================================================
void posterize(image* _image, size_t _depth){

    byte* colors = malloc(_depth*sizeof (byte));
    for(size_t i = 0; i < _depth-1; ++i){
        colors[i] = ((float)_image->depth/(_depth-1))*(i);
    }

    colors[_depth-1] = 255;

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;

            unsigned int index_r = roundf((float)r/_image->depth*(_depth-1));
            unsigned int index_g = roundf((float)g/_image->depth*(_depth-1));
            unsigned int index_b = roundf((float)b/_image->depth*(_depth-1));
            _image->pixels[i][j].r = colors[index_r];
            _image->pixels[i][j].g = colors[index_g];
            _image->pixels[i][j].b = colors[index_b];
        }
    }

    free(colors);
}

//======================================================================
void shift_component_to_left(image* _image){
    for(int i = 0; i < (int)_image->sizeX; ++i){
        for(int j = 0; j < (int)_image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;

            _image->pixels[i][j].r = b;
            _image->pixels[i][j].g = r;
            _image->pixels[i][j].b = g;
        }
    }
}

//======================================================================
void shift_component_to_right(image* _image){
    for(int i = 0; i < (int)_image->sizeX; ++i){
        for(int j = 0; j < (int)_image->sizeY; ++j){
            byte r = _image->pixels[i][j].r;
            byte g = _image->pixels[i][j].g;
            byte b = _image->pixels[i][j].b;

            _image->pixels[i][j].r = g;
            _image->pixels[i][j].g = b;
            _image->pixels[i][j].b = r;
        }
    }
}

//======================================================================
void apply_3x3_convolution(image* _image, float conv_matrix[3][3]){

    image *m_image = copy_image(_image);

    for(int i = 0; i < (int)m_image->sizeX; ++i){
        for(int j = 0; j < (int)m_image->sizeY; ++j){

            float r = 0;
            float g = 0;
            float b = 0;

            for(int k = i-1; k <= i+1; ++k){
                for(int l = j-1; l <= j+1; ++l){

                    int mirrored_k = k;
                    int mirrored_l = l;


                    if(k < 0){
                        mirrored_k = 1;
                    }
                    if(l < 0){
                        mirrored_l = 1;
                    }
                    if(k == (int)_image->sizeX){
                        mirrored_k = _image->sizeX-2;
                    }
                    if(l == (int)_image->sizeY){
                        mirrored_l = _image->sizeY-2;
                    }

                    r += (float)m_image->pixels[mirrored_k][mirrored_l].r*conv_matrix[k-i+1][l-j+1];
                    g += (float)m_image->pixels[mirrored_k][mirrored_l].g*conv_matrix[k-i+1][l-j+1];
                    b += (float)m_image->pixels[mirrored_k][mirrored_l].b*conv_matrix[k-i+1][l-j+1];

                }

            }

            _image->pixels[i][j].r = clampf(r, 0, 255);
            _image->pixels[i][j].g = clampf(g, 0, 255);
            _image->pixels[i][j].b = clampf(b, 0, 255);


        }
    }

    free_image(m_image);
}


/*====================================================================*/
/*======================== LOW-PASS FILTERS ==========================*/
/*====================================================================*/


//======================================================================
void box_blur(image* _image, size_t _box_size, size_t _repetitions){

    if(_repetitions == 0){
        return;
    }

    if(_box_size < 3){
        fprintf(stderr, "Box size must be greater than 2, exit filter.\n");
    }

    image *m_image = copy_image(_image);

    unsigned int current_pixels_sum[3] = {0, 0, 0};
    unsigned int pixel_count = 0;

    for(unsigned int i = 0; i < m_image->sizeX; ++i){
        for(unsigned int j = 0; j < m_image->sizeY; ++j){

            current_pixels_sum[0] = 0;
            current_pixels_sum[1] = 0;
            current_pixels_sum[2] = 0;

            pixel_count = 0;

            for(int k = (int)i-((int)_box_size-1)/2; k <= (int)i+((int)_box_size-1)/2; ++k){
                for(int l = (int)j-((int)_box_size-1)/2; l <= (int)j+((int)_box_size-1)/2; ++l){

                    if((k < (int)_image->sizeX) && (l < (int)_image->sizeY)
                        && (k >= 0) && (l >= 0)){

                        current_pixels_sum[0] += m_image->pixels[k][l].r;
                        current_pixels_sum[1] += m_image->pixels[k][l].g;
                        current_pixels_sum[2] += m_image->pixels[k][l].b;

                        pixel_count++;
                    }
                }
            }

            _image->pixels[i][j].r = current_pixels_sum[0]/pixel_count;
            _image->pixels[i][j].g = current_pixels_sum[1]/pixel_count;
            _image->pixels[i][j].b = current_pixels_sum[2]/pixel_count;

        }
    }

    free_image(m_image);
    box_blur(_image, _box_size, _repetitions-1);
}

//======================================================================
void gaussian_filter(image* _image, size_t _repetition){

    if(_repetition == 0){
        return;
    }

    float conv_matrix[3][3] = {{1./16., 2./16., 1./16.},
                               {2./16., 4./16., 2./16.},
                               {1./16., 2./16., 1./16.},};

    apply_3x3_convolution(_image, conv_matrix);
    gaussian_filter(_image, _repetition-1);
}


/*====================================================================*/
/*======================== HIGH-PASS FILTERS =========================*/
/*====================================================================*/


//======================================================================
void laplace_4(image* _image){
    float conv_matrix[3][3] = {{ 0, -1,  0},
                               {-1,  4, -1},
                               { 0, -1,  0},};

    apply_3x3_convolution(_image, conv_matrix);
}


//======================================================================
void laplace_8(image* _image){
    float conv_matrix[3][3] = {{-1, -1, -1},
                               {-1,  8, -1},
                               {-1, -1, -1},};

    apply_3x3_convolution(_image, conv_matrix);
}

//======================================================================
void laplace_8Bis(image* _image){
    float conv_matrix[3][3] = {{ 1, -2,  1},
                               {-2,  4, -2},
                               { 1, -2,  1},};
    apply_3x3_convolution(_image, conv_matrix);
}

//======================================================================
void sobel_filter(image* _image){
    image* G1 = copy_image(_image);
    image* G2 = copy_image(_image);

    float g1_matrix[3][3] = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1},};

    float g2_matrix[3][3] = {{-1, -2, -1},
                             { 0,  0,  0},
                             { 1,  2,  1},};

    apply_3x3_convolution(G1, g1_matrix);
    apply_3x3_convolution(G2, g2_matrix);

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            float r1 = G1->pixels[i][j].r;
            float r2 = G2->pixels[i][j].r;
            float g1 = G1->pixels[i][j].g;
            float g2 = G2->pixels[i][j].g;
            float b1 = G1->pixels[i][j].b;
            float b2 = G2->pixels[i][j].b;

            _image->pixels[i][j].r = clampf(sqrt(r1*r1 + r2*r2), 0, 255);
            _image->pixels[i][j].g = clampf(sqrt(g1*g1 + g2*g2), 0, 255);
            _image->pixels[i][j].b = clampf(sqrt(b1*b1 + b2*b2), 0, 255);
        }
    }

    free_image(G1);
    free_image(G2);

}

//======================================================================
void prewitt_filter(image* _image){
    image* G1 = copy_image(_image);
    image* G2 = copy_image(_image);

    float g1_matrix[3][3] = {{-1, 0, 1},
                             {-1, 0, 1},
                             {-1, 0, 1},};

    float g2_matrix[3][3] = {{-1, -1, -1},
                             { 0,  0,  0},
                             { 1,  1,  1},};

    apply_3x3_convolution(G1, g1_matrix);
    apply_3x3_convolution(G2, g2_matrix);

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            float r1 = G1->pixels[i][j].r;
            float r2 = G2->pixels[i][j].r;
            float g1 = G1->pixels[i][j].g;
            float g2 = G2->pixels[i][j].g;
            float b1 = G1->pixels[i][j].b;
            float b2 = G2->pixels[i][j].b;

            _image->pixels[i][j].r = clampf(sqrt(r1*r1 + r2*r2), 0, 255);
            _image->pixels[i][j].g = clampf(sqrt(g1*g1 + g2*g2), 0, 255);
            _image->pixels[i][j].b = clampf(sqrt(b1*b1 + b2*b2), 0, 255);
        }
    }

    free_image(G1);
    free_image(G2);
}


/*====================================================================*/
/*========================= IMAGE TRANSFORM ==========================*/
/*====================================================================*/


//======================================================================
void flip_x(image* _image){
    image *m_image = copy_image(_image);

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            _image->pixels[_image->sizeX-1-i][j] = m_image->pixels[i][j];
        }
    }
}

//======================================================================
void flip_y(image* _image){
    image *m_image = copy_image(_image);

    for(unsigned int i = 0; i < _image->sizeX; ++i){
        for(unsigned int j = 0; j < _image->sizeY; ++j){
            _image->pixels[i][_image->sizeY-1-j] = m_image->pixels[i][j];
        }
    }
}

//======================================================================
void rotate(image* _image, float _angle){

    _angle = _angle*M_PI/180;
    int center_x = _image->sizeX/2;
    int center_y = _image->sizeY/2;

    image *m_image = copy_image(_image);

    for(int i = 0; i < (int)_image->sizeX; ++i){
        for(int j = 0; j < (int)_image->sizeY; ++j){

            _image->pixels[i][j].r = 0;
            _image->pixels[i][j].g = 0;
            _image->pixels[i][j].b = 0;
        }
    }


    for(int i = 0; i < (int)_image->sizeX; ++i){
        for(int j = 0; j < (int)_image->sizeY; ++j){

            int n_x = (cos(_angle)*(float)(i - center_x) + sin(_angle)*(float)(j - center_y) + (float)center_x);
            int n_y = (-sin(_angle)*(float)(i - center_x) + cos(_angle)*(float)(j - center_y) + (float)center_y);


            if((n_x < (int)m_image->sizeX-1) && (n_y < (int)m_image->sizeY-1)
                && (n_x >= 1) && (n_y >= 1)){

                _image->pixels[n_x][n_y] = m_image->pixels[i][j];
                _image->pixels[n_x][n_y-1] = m_image->pixels[i][j];
                _image->pixels[n_x][n_y+1] = m_image->pixels[i][j];
                _image->pixels[n_x-1][n_y] = m_image->pixels[i][j];
                _image->pixels[n_x+1][n_y] = m_image->pixels[i][j];
            }
        }
    }

    free_image(m_image);
}
