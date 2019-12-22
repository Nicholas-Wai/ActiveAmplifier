/*****************************************
                                mute.c
                                zhangtaolei     2016-1-27
******************************************/

/*****************************************
                                include
******************************************/
#include "stm8s_conf.h"
#include "mute.h"
/*****************************************
                                macro
******************************************/

/*****************************************
                                typedef
******************************************/

/*****************************************
                                variables
******************************************/

/*****************************************
                                function
******************************************/
void MuteInit(void)
{
    GPIO_Init(MUTE_A_Port, MUTE_A_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(MUTE_B_Port, MUTE_B_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
}