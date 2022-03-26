#include "utility.h"
#include "image.h"
#include "interface.h"

int main(int argc, char* argv[])
{
    param m_param = parse_arg(argc, argv);
    //printf("Check param : %d\n", check_param(m_param));

    /*printf("\n\n\n");
    printf("==========================================\n");
    printf("Source path : %s\n", m_param.source_name);
    printf("Destination path : %s\n", m_param.dest_name);

    printf("Options :\n");
    printf("Number of options %d\n", m_param.optionc);

    for(int i = 0; i < m_param.optionc; ++i){
        printf("Option %d : ", i);
        for(int j = 0; j < m_param.optionv[i].argc; ++j){
            printf(" %s", m_param.optionv[i].argv[j]);
        }
        printf("\n");
    }

    printf("==========================================");
    printf("\n\n\n");*/

    process(m_param);
    free_param(m_param);

    return EXIT_SUCCESS;
}
