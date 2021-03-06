#include "app_menu.h"
#include "app_i2c.h"
#include "string.h"
#include "app_gpio.h"
#include "app_beep.h"
#include "app.h"

#include "stdlib.h"

const struct item_table DPA_item_table[] = {
        {NULL, NULL, 0, 7,""},

	{NULL, NULL, (HAVE_CHILD), 6, "Display Meters "},
 	{NULL, NULL, (HAVE_CHILD), 3, "Input Source   "},
 	{NULL, NULL, (HAVE_CHILD), 3, "Output Mode    "},
	{NULL, NULL, (HAVE_CHILD), 3, "Max Output Volt"},
	{NULL, NULL, (HAVE_CHILD), 3, "Max output Curt"},
	{NULL, NULL, (HAVE_CHILD), 2, "Fans Start set "},
	{NULL, NULL, (HAVE_CHILD), 2, "Reset Setting  "},
	//{NULL, NULL, (HAVE_CHILD), 3, "LIMIT TEST     "},

	{NULL, enckey_confirm_temperatures, (HAVE_CHILD), 4, "Temperatures   "},
	{NULL, enckey_confirm_mains_meters, (HAVE_CHILD), 4, "Mains meters   "},
	{NULL, enckey_confirm_audio_output_meters, (HAVE_CHILD), 4, "Output meters  "},
	{NULL, enckey_confirm_audio_impedance, (HAVE_CHILD), 4, "Impedance      "},
	{NULL, enckey_confirm_hardware_info, (HAVE_CHILD), 4, "Hardware info  "},
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_input_source, enckey_confirm_input_source, (IS_LEAF	), 0, "CH1/2: XLR   "},
	{enc_change_input_source, enckey_confirm_input_source, (IS_LEAF	), 0, "CH3/4: XLR   "},
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_output_mode, enckey_confirm_output_mode, (IS_LEAF	), 0, "CH1/2: STR   "},
	{enc_change_output_mode, enckey_confirm_output_mode, (IS_LEAF	), 0, "CH3/4: STR   "},
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_max_output_volt, enckey_confirm_max_output_volt, (IS_LEAF	), 0, "CH3/4: 070 V "},
	{enc_change_max_output_volt, enckey_confirm_max_output_volt, (IS_LEAF	), 0, "CH3/4: 070 V "},//config值改变了，同时还按下了确认按键 退出时候在主循环设置模块参数
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_max_output_curt, enckey_confirm_max_output_curt, (IS_LEAF	), 0, "CH1/2: 001 A "},
	{enc_change_max_output_curt, enckey_confirm_max_output_curt, (IS_LEAF	), 0, "CH3/4: 001 A "},
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_temperature_gate, enckey_confirm_temperature_gate, (IS_LEAF	), 0, "Gate: 050 Cel"},//两个模块一起设置
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

 	{enc_change_reset_setting, enckey_confirm_reset_setting, (IS_LEAF	), 0, "Reset        "},
 	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	//{enc_change_STDDT_val, enckey_confirm_STDDT_val, (IS_LEAF	),0,  "STDDT  000   "},
	//{enc_change_STPDT_val, enckey_confirm_STPDT_val, (IS_LEAF	),0,  "STPDT  000   "},
	//{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

	{enc_change_temperatures_page, NULL, (IS_LEAF|HORIZ_PAGE), 0, "Temperature    "},
    {NULL, NULL, (BIG_CHAR			), 0, "25oC 077oF"},
    {NULL, NULL, (IS_VOID			), 0, "             "},//填充一个空白行--显示一半
    {NULL, NULL, (IS_BACK   ), 0, "BACK           "},

 	{enc_change_mains_meters_page, NULL, (IS_LEAF|HORIZ_PAGE), 0, "Mains meters   "},
 	{NULL, NULL, (BIG_CHAR			), 0, "220V 095A"},
 	{NULL, NULL, (IS_VOID			), 0, "               "},//填充一个空白行
 	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},

    {enc_change_audio_output_meters_page, NULL, (IS_LEAF|HORIZ_PAGE), 0, "CH4 Output    "},
    {NULL, NULL, (BIG_CHAR			), 0, "070V 057A"},
    {NULL, NULL, (IS_VOID			), 0, "               "},//填充一个空白行
    {NULL, NULL, (IS_BACK   ), 0, "BACK           "},

    {enc_change_audio_impedance_page, NULL, (IS_LEAF|HORIZ_PAGE), 0, "CH1 Impedance"},
    {NULL, NULL, (BIG_CHAR			), 0, "000.00"},
    {NULL, NULL, (IS_VOID			), 0, "             "},//填充一个空白行
    {NULL, NULL, (IS_BACK   ), 0, "BACK           "},

    {enc_change_hardware_info_page, NULL, (IS_LEAF|HORIZ_PAGE), 0, "Hardware info  "},
	{NULL, NULL, (BIG_CHAR			), 0, "00000"},
	{NULL, NULL, (IS_VOID			), 0, "             "},//填充一个空白行
	{NULL, NULL, (IS_BACK   ), 0, "BACK           "},
};

/******************************
*菜单关联操作函数
*
**********************************/
extern DeviceParameter_TypeDef g_prmt;
extern struct device_global_prmt g_dev;

//拷贝字符串函数
void replace_str(char *destin_str, char *source_str, uint8_t start_position, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++) {
		destin_str[start_position + i] = source_str[i];
	}
}

//用数字直接替换字符串   --需要重写  start_position 无符号数的起始
void replace_str_in_number(char *destin_str, int16_t integer_number, uint8_t end_position, uint8_t digit_cnt)
{
    int8_t i;
    int16_t num;
    num = integer_number;
    if (num < 0) {
        num = num * (-1);
    }

    for (i = end_position; digit_cnt > 0 && i >= 0; i--, digit_cnt--) {
        destin_str[i] = num % 10 + '0';
        num /= 10;
    }
    if (destin_str[i] == ' ')
        destin_str[i] = integer_number < 0 ? '-' : ' ';
}

//-------------------------------------------------------
char input_source_str[][MAX_SHOW_LENGTH] = {
	{"CH1/2: XLR   "},
	{"CH3/4: XLR   "}
};

char input_source_replace_str[][MAX_SHOW_LENGTH] = {
	{"XLR  "},
	{"DANTE"}
};

void enckey_confirm_input_source(struct item *item)
{
	//uint8_t chGroupIdx = item->item_page_index;

	//set_input_source(chGroupIdx, dev_prmt.is_dante_input[chGroupIdx]);
	MusicCtrl_SetInputSourceAndOutputMode(&g_prmt.MusicData);
}

void enc_change_input_source(struct item *item, encoder_dir_TypeDef turn_dir)
{
	uint8_t chGroupIdx = item->item_page_index;

	if (turn_dir != TURN_NONE) 
	{

		if (g_prmt.MusicData.InputSource[chGroupIdx] != DANTE)
		{
			g_prmt.MusicData.InputSource[chGroupIdx] = DANTE;
		}
		else
		{
			g_prmt.MusicData.InputSource[chGroupIdx] = XLR;
		}
		//hMusicCtrl.InputSource[chGroupIdx] = dev_prmt.MusicData.InputSource[chGroupIdx];

		//dev_prmt.is_dante_input[chGroupIdx] = (bool)!dev_prmt.is_dante_input[chGroupIdx];
	}

	if (g_prmt.MusicData.InputSource[chGroupIdx] == DANTE) {
		replace_str(input_source_str[chGroupIdx], input_source_replace_str[1], 7, 5);
	} else {
		replace_str(input_source_str[chGroupIdx], input_source_replace_str[0], 7, 5);
	}

	item->item_str = input_source_str[chGroupIdx];
}

//----------------------------------------------------------------------------------
char output_mode_str[][MAX_SHOW_LENGTH] = {
	{"CH1/2: STR   "},
	{"CH3/4: STR   "}
};

char output_mode_replace_str[][MAX_SHOW_LENGTH] = {
	{"STR  "},
	{"BRG  "}
};

void enckey_confirm_output_mode(struct item *item)
{
	//uint8_t chGroupIdx = item->item_page_index;

	//set_output_mode(chGroupIdx, dev_prmt.is_bridge_mode[chGroupIdx]);
	MusicCtrl_SetInputSourceAndOutputMode(&g_prmt.MusicData);
}

void enc_change_output_mode(struct item *item, encoder_dir_TypeDef turn_dir)
{
	uint8_t chGroupIdx = item->item_page_index;

	if (turn_dir != TURN_NONE) 
	{
		if (g_prmt.MusicData.OutputMode[chGroupIdx] != BRIDGE)
		{
			g_prmt.MusicData.OutputMode[chGroupIdx] = BRIDGE;
		}
		else
		{
			g_prmt.MusicData.OutputMode[chGroupIdx] = STEREO;
		}
		//hMusicCtrl.OutputMode[chGroupIdx] = dev_prmt.MusicData.OutputMode[chGroupIdx];
		//dev_prmt.is_bridge_mode[chGroupIdx] = (bool)!dev_prmt.is_bridge_mode[chGroupIdx];
	}

	if (g_prmt.MusicData.OutputMode[chGroupIdx] == BRIDGE) {
		replace_str(output_mode_str[chGroupIdx], output_mode_replace_str[1], 7, 5);
	} else {
		replace_str(output_mode_str[chGroupIdx], output_mode_replace_str[0], 7, 5);
	}

	item->item_str = output_mode_str[chGroupIdx];
}

//----------------------------------------------------------------------------------
char max_output_volt_str[][MAX_SHOW_LENGTH] = {
	{"CH1/2: 000 V "},
	{"CH3/4: 000 V "}
};

void enckey_confirm_max_output_volt(struct item *item)
{
	uint8_t chGroupIdx = item->item_page_index;
	uint8_t ret;

	ret = CtrlerBoard_ConfigWithNewData(chGroupIdx, I2C_DATA_OVV_LIMIT, g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit);
	if (ret != 0)
	{
		CtrlerBoard_ConfigWithNewData(chGroupIdx, I2C_DATA_OVV_LIMIT, g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit);
	}
}

void enc_change_max_output_volt(struct item *item, encoder_dir_TypeDef turn_dir)
{
	uint8_t chGroupIdx = item->item_page_index;

	if ((turn_dir == TURN_LEFT) && (g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit > 35)) { 			//减少
		g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit--;
	} else if ((turn_dir == TURN_RIGHT) && (g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit < 70)){  //增加
		g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit++;
	}

	//直接转换成字符串
	replace_str_in_number(max_output_volt_str[chGroupIdx], g_prmt.CtrlBoard[chGroupIdx].Config.OVVLimit, 9, 3);

	item->item_str = max_output_volt_str[chGroupIdx];
}

//----------------------------------------------------------------------------------
char max_output_curt_str[][MAX_SHOW_LENGTH] = {
	{"CH1/2: 000 A "},
	{"CH3/4: 000 A "}
};

void enckey_confirm_max_output_curt(struct item *item)
{
	uint8_t chGroupIdx = item->item_page_index;  //可以判断要更新那个控制器
	uint8_t ret;

	ret = CtrlerBoard_ConfigWithNewData(chGroupIdx,I2C_DATA_OCV_LIMIT, g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit);
	if (ret != 0)
	{
		CtrlerBoard_ConfigWithNewData(chGroupIdx,I2C_DATA_OCV_LIMIT, g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit);
	}
}

void enc_change_max_output_curt(struct item *item, encoder_dir_TypeDef turn_dir)
{
	uint8_t chGroupIdx = item->item_page_index;

	if ((turn_dir == TURN_LEFT) && (g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit > 1)) { 			//减少
		g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit--;
	} else if ((turn_dir == TURN_RIGHT) && (g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit < 30)){  //增加
		g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit++;
	}

	//直接转换成字符串
	replace_str_in_number(max_output_curt_str[chGroupIdx], g_prmt.CtrlBoard[chGroupIdx].Config.OCVLimit, 9, 3);
	item->item_str = max_output_curt_str[chGroupIdx];
}

//----------------------------------------------------------------------------------
char temperature_gate_str[MAX_SHOW_LENGTH] = {"Gate: 00,C   "};//逗号替换为摄氏度符号�

void enckey_confirm_temperature_gate(struct item *item)
{
	
	CtrlerBoard_ConfigWithNewData(0, I2C_DATA_TEMP_GATE, g_prmt.CtrlBoard[0].Config.TemperatureGate);
	CtrlerBoard_ConfigWithNewData(1, I2C_DATA_TEMP_GATE, g_prmt.CtrlBoard[1].Config.TemperatureGate);
}

void enc_change_temperature_gate(struct item *item, encoder_dir_TypeDef turn_dir)
{
	//编码器更改参数值
	if ((turn_dir == TURN_LEFT) && (g_prmt.CtrlBoard[0].Config.TemperatureGate > 40)) { 			//减少
		g_prmt.CtrlBoard[0].Config.TemperatureGate--;
		g_prmt.CtrlBoard[1].Config.TemperatureGate--;
	} else if ((turn_dir == TURN_RIGHT) && (g_prmt.CtrlBoard[0].Config.TemperatureGate < 50)){  //增加
		g_prmt.CtrlBoard[0].Config.TemperatureGate++;
		g_prmt.CtrlBoard[1].Config.TemperatureGate++;
	}
	
	//整数转字符串用于显示
	replace_str_in_number(temperature_gate_str, g_prmt.CtrlBoard[0].Config.TemperatureGate, 7, 2);
	item->item_str = temperature_gate_str;
}

//----------------------------------------------------------------------------------
char reset_setting_str[][MAX_SHOW_LENGTH] = {
	{"Reset        "},
	{"Cancel       "}
};

void enckey_confirm_reset_setting(struct item *item)
{
	if (g_prmt.Op.Reset) {
		g_dev.reset_dev = TRUE;
	}
}

void enc_change_reset_setting(struct item *item, encoder_dir_TypeDef turn_dir)
{
	if (turn_dir != TURN_NONE) {
		g_prmt.Op.Reset = (bool)!g_prmt.Op.Reset;
	}

	item->item_str = reset_setting_str[g_prmt.Op.Reset? 0 : 1];
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
char mains_meters_value_str[] = {"000V 00.0A"};

char mains_meters_caption_str[][MAX_SHOW_LENGTH] = {
    {"Main Meter 1 "},
	{"Main Meter 2 "}
};

void enckey_confirm_mains_meters(struct item *item)
{

}

void enc_change_mains_meters_page(struct item *item, encoder_dir_TypeDef turn_dir)
{
	static uint8_t chGroupIdx = 0;   //定位记录所在页面

	g_dev.refresh_menu = TRUE;

	if ((turn_dir == TURN_LEFT) && (chGroupIdx > 0)) { 			//减少
		chGroupIdx--;
	} else if ((turn_dir == TURN_RIGHT) && (chGroupIdx < 1)){  //增加
		chGroupIdx++;
	}

	//更新数据
	CtrlerBoard_ReadNewData(chGroupIdx, I2C_DATA_POWER_VOLTAGE, &g_prmt.CtrlBoard[chGroupIdx].Meters.PowerVoltage);
	CtrlerBoard_ReadNewData(chGroupIdx, I2C_DATA_POWER_CURRENT, &g_prmt.CtrlBoard[chGroupIdx].Meters.PowerCurrent);

	item->item_str = mains_meters_caption_str[chGroupIdx];

	replace_str_in_number(mains_meters_value_str, g_prmt.CtrlBoard[chGroupIdx].Meters.PowerVoltage,2 ,3);
	replace_str_in_number(mains_meters_value_str, g_prmt.CtrlBoard[chGroupIdx].Meters.PowerCurrent/10,6 ,2);
	replace_str_in_number(mains_meters_value_str, g_prmt.CtrlBoard[chGroupIdx].Meters.PowerCurrent%10,8 ,1);

	//找数值行  通过属性找
	for (uint8_t index = 0;  index < item->belong_page->items_num; index++) {
		if(item->belong_page->items[index].item_property & BIG_CHAR) {
			item->belong_page->items[index].item_str = mains_meters_value_str;
		}
	}
}

//-------------------------------------------------------------------------------------
char audio_output_meters_value_str[] = {"000V 00.0A"};

char audio_output_meters_caption_str[][MAX_SHOW_LENGTH] = {
    {"CH1  Output  "},
	{"CH2  Output  "},
	{"CH3  Output  "},
	{"CH4  Output  "}
};

void enckey_confirm_audio_output_meters(struct item *item)
{

}

void enc_change_audio_output_meters_page(struct item *item, encoder_dir_TypeDef turn_dir)
{
	static uint8_t chanIdx = 0;   //定位记录所在通道

	uint8_t CtrlBoardIndex;
	uint8_t BoardChanIndex;

	uint16_t AudioVoltage;
	uint16_t AudioCurrent;
	
	g_dev.refresh_menu = TRUE;

	if ((turn_dir == TURN_LEFT) && (chanIdx > 0)) { 			//减少
		chanIdx--;
	} else if ((turn_dir == TURN_RIGHT) && (chanIdx < 3)){  //增加
		chanIdx++;
	}

	CtrlBoardIndex = chanIdx / 2;
	BoardChanIndex = chanIdx % 2;


	if (BoardChanIndex == 0) //模块的通道1
	{
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH1_VOLTAGE, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Voltage);
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH1_CURRENT, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Current);
		AudioVoltage = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Voltage;
		AudioCurrent = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Current;
	}
	else 										//模块的通道2
	{
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH2_VOLTAGE, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Voltage);
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH2_CURRENT, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Current);
		AudioVoltage = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Voltage;
		AudioCurrent = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Current;
	}

	item->item_str = audio_output_meters_caption_str[chanIdx];

	replace_str_in_number(audio_output_meters_value_str, AudioVoltage,2 ,3);
	replace_str_in_number(audio_output_meters_value_str, AudioCurrent/10, 6 ,2);
	replace_str_in_number(audio_output_meters_value_str, AudioCurrent%10, 8, 1);

	for (uint8_t index = 0;  index < item->belong_page->items_num; index++) {
		if(item->belong_page->items[index].item_property & BIG_CHAR) {
			item->belong_page->items[index].item_str = audio_output_meters_value_str;
		}
	}
}

//-------------------------------------------------------------------------------------
char audio_impedance_value_str[] = {"  0000.0"};

char audio_impedance_caption_str[][MAX_SHOW_LENGTH] = {
    {"CH1 Impedance"},
	{"CH2 Impedance"},
	{"CH3 Impedance"},
	{"CH4 Impedance"}
};

void enckey_confirm_audio_impedance(struct item *item)
{

}

void enc_change_audio_impedance_page(struct item *item, encoder_dir_TypeDef turn_dir)
{
	static uint8_t chanIdx = 0;   //定位记录所在通道
	uint8_t CtrlBoardIndex;
	uint8_t BoardChanIndex;

	uint16_t AudioImpedence;

	g_dev.refresh_menu = TRUE;

	if ((turn_dir == TURN_LEFT) && (chanIdx > 0)) { 			//减少
		chanIdx--;
	} else if ((turn_dir == TURN_RIGHT) && (chanIdx < 3)){  //增加
		chanIdx++;
	}

	CtrlBoardIndex = chanIdx / 2;
	BoardChanIndex = chanIdx % 2;

	if (BoardChanIndex == 0) //模块的通道1
	{
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH1_IMPEDANCE, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Impedence);
		AudioImpedence = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh1Impedence;
	}
	else
	{
		CtrlerBoard_ReadNewData(CtrlBoardIndex, I2C_DATA_AUDIO_CH2_IMPEDANCE, &g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Impedence);
		AudioImpedence = g_prmt.CtrlBoard[CtrlBoardIndex].Meters.AudioCh2Impedence;
	}

	item->item_str = audio_impedance_caption_str[chanIdx];

	if (AudioImpedence == 0xffff) {//65536
		replace_str_in_number(audio_impedance_value_str, 9999, 5 ,4);
		replace_str_in_number(audio_impedance_value_str, 9,    7 ,1);
	}
	else {//模块错误 正常读取数据 和 0
		replace_str_in_number(audio_impedance_value_str, AudioImpedence/10,5 ,4);//去除小数点
		replace_str_in_number(audio_impedance_value_str, AudioImpedence%10,7 ,1);//取最后一位
	}

	for (uint8_t index = 0;  index < item->belong_page->items_num; index++) {
		if(item->belong_page->items[index].item_property & BIG_CHAR) {
			item->belong_page->items[index].item_str = audio_impedance_value_str;
		}
	}
}

//-------------------------------------------------------------------------------------
char temperatures_value_str[] = {"00oC 000oF"};

char temperatures_caption_str[][MAX_SHOW_LENGTH] = {
    {"Temperature 1"},
	{"Temperature 2"}
};

void enckey_confirm_temperatures(struct item *item)
{

}

void enc_change_temperatures_page(struct item *item, encoder_dir_TypeDef turn_dir)
{
	static uint8_t chGroupIdx = 0;   //定位记录所在页面

	g_dev.refresh_menu = TRUE;

	if ((turn_dir == TURN_LEFT) && (chGroupIdx > 0)) { 			//减少
		chGroupIdx--;
	} else if ((turn_dir == TURN_RIGHT) && (chGroupIdx < 1)){  //增加
		chGroupIdx++;
	}

	CtrlerBoard_ReadNewData(chGroupIdx, I2C_DATA_DEVICE_TRMPERATURE, &g_prmt.CtrlBoard[chGroupIdx].Meters.DeviceTemperature);

	item->item_str = temperatures_caption_str[chGroupIdx];

	replace_str_in_number(temperatures_value_str, g_prmt.CtrlBoard[chGroupIdx].Meters.DeviceTemperature,1 ,2);
	replace_str_in_number(temperatures_value_str, (uint16_t)(g_prmt.CtrlBoard[chGroupIdx].Meters.DeviceTemperature * 9/5 + 32), 7 ,3);

	for (uint8_t index = 0;  index < item->belong_page->items_num; index++) {
		if(item->belong_page->items[index].item_property & BIG_CHAR) {
			item->belong_page->items[index].item_str = temperatures_value_str;
		}
	}
}

//-------------------------------------------------------------------------------------
char hardware_info_str[] = {"  00000"};

char hardware_info_caption_str[][MAX_SHOW_LENGTH] = {
    {" AMP1 OCP1   "},	//通道过流
    {" AMP1 OCP2   "},
  	{" AMP1 DCP    "},	//直流输入保护
	{" AMP1 HFP    "},	//高频保护
	{" AMP1 UVP    "},	//欠压保护

    {" AMP2 OCP1   "},
    {" AMP2 OCP2   "},
    {" AMP2 DCP    "},
    {" AMP2 HFP    "},
    {" AMP2 UVP    "}
};

void enckey_confirm_hardware_info(struct item *item)
{

}

void enc_change_hardware_info_page(struct item *item, encoder_dir_TypeDef turn_dir)
{
	uint16_t *pExtiCnt;

	static uint8_t infoIdx = 0;   //定位记录所在通道
	I2C_Slave_CmdTypeDef CtrlerReadCmd;

//	g_dev_prmt.clean_hardware_info = TRUE;
	g_dev.refresh_menu = TRUE;

	if ((turn_dir == TURN_LEFT) && (infoIdx > 0)) { 			//减少
		infoIdx--;
	} else if ((turn_dir == TURN_RIGHT) && (infoIdx < 9)){  //增加
		infoIdx++;
	}

//0 1 2 3 4 5 6 7 8 9 10
//0 1 2 3 4 5
 	if ((infoIdx%5) == 0)  
	{
		CtrlerReadCmd = I2C_DATA_EXTI_OCP1_CNT; 
		pExtiCnt = &g_prmt.CtrlBoard[infoIdx%5].Meters.ExtiOCP1Cnt;
	}
	else if ((infoIdx%5) == 1) 
	{
		CtrlerReadCmd = I2C_DATA_EXTI_OCP2_CNT; 
		pExtiCnt = &g_prmt.CtrlBoard[infoIdx%5].Meters.ExtiOCP2Cnt;
	}
	else if ((infoIdx%5) == 2) 
	{
		CtrlerReadCmd = I2C_DATA_EXTI_DCP_CNT; 
		pExtiCnt = &g_prmt.CtrlBoard[infoIdx%5].Meters.ExtiDCPCnt;

	}
	else if ((infoIdx%5) == 3) 
	{
		CtrlerReadCmd = I2C_DATA_EXTI_HFP_CNT; 
		pExtiCnt = &g_prmt.CtrlBoard[infoIdx%5].Meters.ExtiHFPCnt;

	}
	else 
	{
		CtrlerReadCmd = I2C_DATA_EXTI_UVP_CNT; 
		pExtiCnt = &g_prmt.CtrlBoard[infoIdx%5].Meters.ExtiUVPCnt;
	}

	CtrlerBoard_ReadNewData(infoIdx/5, CtrlerReadCmd, pExtiCnt);

	item->item_str = hardware_info_caption_str[infoIdx];

	replace_str_in_number(hardware_info_str, *pExtiCnt,6 ,5);

	for (uint8_t index = 0;  index < item->belong_page->items_num; index++) {
		if(item->belong_page->items[index].item_property & BIG_CHAR) {
			item->belong_page->items[index].item_str = hardware_info_str;
		}
	}
}

//--------------------------------------------------------------------------------------
char board_limit_str[][MAX_SHOW_LENGTH] = {
	{"STPDT  000   "},
	{"STDDT  000   "},
};

void enckey_confirm_STDDT_val(struct item *item)
{
	uint8_t ret;

	ret = CtrlerBoard_ConfigWithNewData(0, I2C_DATA_LIMIT_STDDT, g_prmt.STDDT_val);
	ret += CtrlerBoard_ConfigWithNewData(1, I2C_DATA_LIMIT_STDDT, g_prmt.STDDT_val);
	if (ret != 0)
	{
		CtrlerBoard_ConfigWithNewData(0, I2C_DATA_LIMIT_STDDT, g_prmt.STDDT_val);
		CtrlerBoard_ConfigWithNewData(1, I2C_DATA_LIMIT_STDDT, g_prmt.STDDT_val);
	}
}

void enc_change_STDDT_val(struct item *item, encoder_dir_TypeDef turn_dir)
{
	if ((turn_dir == TURN_LEFT) && (g_prmt.STDDT_val > 0)){ 			//减少
		g_prmt.STDDT_val--;
	} else if ((turn_dir == TURN_RIGHT)  && (g_prmt.STDDT_val < 100)){  //增加
		g_prmt.STDDT_val++;
	}

	replace_str_in_number(board_limit_str[1], g_prmt.STDDT_val, 9 ,3);

	item->item_str = board_limit_str[1];
}

//--------------------------------------------------------------------------------------
void enckey_confirm_STPDT_val(struct item *item)
{
	CtrlerBoard_ConfigWithNewData(0, I2C_DATA_LIMIT_STPDT, g_prmt.STPDT_val);
	CtrlerBoard_ConfigWithNewData(1, I2C_DATA_LIMIT_STPDT, g_prmt.STPDT_val);
}

void enc_change_STPDT_val(struct item *item, encoder_dir_TypeDef turn_dir)
{
	if ((turn_dir == TURN_LEFT) && (g_prmt.STPDT_val > 0)){ 			//减少
		g_prmt.STPDT_val--;
	} else if ((turn_dir == TURN_RIGHT)  && (g_prmt.STPDT_val < 100)){  //增加
		g_prmt.STPDT_val++;
	}

	replace_str_in_number(board_limit_str[0], g_prmt.STPDT_val, 9 ,3);

	item->item_str = board_limit_str[0];
}
//--------------------------------------------------------------------------------------

/******************************
*菜单构建初始化方法
*
**********************************/
uint8_t get_chile_menu_page_first_item_index(uint8_t curr_item_index)
{
    char i, chile_menu_page_first_item_index = 0;

    for  (i = 0; i < curr_item_index; i++) {
        chile_menu_page_first_item_index += DPA_item_table[i].sub_items_num;
    }
    chile_menu_page_first_item_index++;
    return chile_menu_page_first_item_index;
}

//创建一个菜单页面的items信息  申请内存 用数组参数初始化items信息
static void create_items(struct menu_page *page)  //传进要创建的page指针
{
	struct item *item;
	struct item_table *item_table;
	uint8_t item_prmt_index;

	//申请相应空间
	page->items = (struct item *)malloc(sizeof(struct item) * page->items_num);//申请到一组菜单,数量为 listHead->itemNum
	if (page->items == NULL) {
		//set_pwr_led_off();
		BEEP_BSP_SetOn();
		TIM3_Cmd(DISABLE);
		while(1);
	}

	//获取菜单页面数组第一个下标编号
	item_prmt_index = get_chile_menu_page_first_item_index(page->paren_item_index);

	//赋值初始化
	for(int item_page_index = 0; item_page_index < page->items_num; item_page_index++) {
		item_table = (struct item_table *)&DPA_item_table[item_prmt_index];
		item = &(page->items[item_page_index]);

		item->rolling_selected_item = item_table->rolling_selected_item;
		item->press_curr_item = item_table->press_curr_item;
		item->item_property = item_table->item_property;                //增加菜单属性
		item->item_prmt_index = item_prmt_index;
		item->item_page_index = item_page_index;//加个页面编号
		item->item_str = item_table->show_str;
		item->belong_page = page;
		item_prmt_index++;
	}
}

//创建上一级菜单的items信息 ---可以用于判断是首页?
struct menu_page *create_prev_page_items(struct menu_page *page)
{
    if (page->items != NULL)
        return page;
    create_items(page);
    return page;
}

//创建下一级子菜单页面
struct menu_page *create_child_menu_page(struct menu_page *curr_page, struct item *curr_item)//curr_item准备要创建的该菜单的字菜单信息
{
	struct menu_page *child_page = NULL;
	uint8_t items_num;

	struct menu_page *prev_page = curr_page;//换个名字      如果是父菜单页面应该不需要创建新的页面
    uint8_t paren_item_index = curr_item ? curr_item->item_prmt_index : 0;   //通过传入参数找到起始页面菜单数组编号

	//数组下标超限 错误处理
    if (paren_item_index >= (sizeof(DPA_item_table) / sizeof(struct item_table)))
        return NULL;

	//获取子页面菜单数量
    items_num = DPA_item_table[paren_item_index].sub_items_num;
    if (items_num <= 0)
        return NULL;

	//如果父菜单为空 或者当前项存在
 	if (!prev_page || curr_item) {
        child_page = (struct menu_page *)malloc(sizeof(struct menu_page));//创建新的page
        if (child_page == NULL) {
            //set_pwr_led_off();
			BEEP_BSP_SetOn();
			TIM3_Cmd(DISABLE);
            while(1);
        }
        child_page->prev_page = prev_page;
        child_page->paren_item_index = paren_item_index;
        child_page->items_num = items_num;
        if (!curr_item)
            prev_page = child_page;

    } else if (curr_item) {
      ;

    } else
        child_page = prev_page;

    child_page->top_item_id = 0;
    child_page->curr_line = 0;
	child_page->item_offset = 0;

    create_items(child_page);
    return child_page;
}

void init_menu_page(struct menu_head *menu)
{
   // while (menu->curr_page && menu->curr_page->prev_page) {// 循环释放
   //    menu->curr_page = menu->curr_page->prev_page;
   //}

    menu->curr_page = create_child_menu_page(menu->curr_page, NULL);
}

/******************************
*菜单显示方法
*
**********************************/

//显示item 菜单的一行 不包括大字符行      head_flag  是否为当前行是则显示 否则删除   leaf_selected  是否选择上
void display_item_str(struct item *item, uint8_t line, bool isCurr_line, bool leaf_selected)//行高亮  末尾符号高亮
{
	Display_HandlerTypeDef hDisplay;

	hDisplay.Line = line;
	if (item->item_property & BIG_CHAR) 
	{
		hDisplay.HorizontalStart = 0;
		hDisplay.Str = item->item_str;
		hDisplay.Reverse = FALSE;
		Display_Show12x32BigString(&hDisplay);
	}
	else 
	{
		//显示行首旗标 当前行号
		hDisplay.Reverse = isCurr_line;
		Display_ShowPointSymbol(&hDisplay);

		//显示菜单内容
		hDisplay.Reverse = FALSE;
		hDisplay.Str = item->item_str;
		hDisplay.HorizontalStart = 1;
	  Display_ShowString(&hDisplay);

		//显示行尾选择标志
    if (item->item_property & IS_LEAF) 
		{
			hDisplay.Reverse = leaf_selected;
			Display_ShowSelectSymbol(&hDisplay);
    }
	}
}

//显示整个菜单页面  包括大字体行和back行
void display_menu_page(struct menu_page *page)
{
	uint8_t line = 0;
	uint8_t top_item_id;

  // struct menu_page *curr_page;
    struct item *item;

    //curr_page = page;

    if (page) {
        top_item_id = page->top_item_id;

		//显示可以显示的行
		while (line + top_item_id < page->items_num && line < MAX_SHOW_LINE) {
            item = &page->items[line + top_item_id];//内容变
			if (page->items[line + top_item_id].rolling_selected_item)  {
				page->items[line + top_item_id].rolling_selected_item(&page->items[line + top_item_id], TURN_NONE);
			}

			if (!(item->item_property & IS_VOID)){ //空白行不显示
				display_item_str(item, line, (bool)(line == page->curr_line), g_dev.is_leaf_select);
			}

			line++;
		}

		//清除剩余部分
		if (page->items_num < MAX_SHOW_LINE) {
			Screen_DrawAreaToWhiteOrBlack(0, line * 2, 128, (MAX_SHOW_LINE - page->items_num) * 2, FALSE); //不兼容包含大字体页面
		}
	}
}

//刷新整个菜单行  有数据刷新的页面的要数新一下数据----所以转动方向要传递none
void refresh_menu_page(struct menu_page *page)
{
	uint8_t item_id = 0;

	for(item_id = 0; item_id < page->items_num; item_id++) {
		if (page->items[item_id].rolling_selected_item)  {
			page->items[item_id].rolling_selected_item(&page->items[item_id], TURN_NONE);
		}
	}

	display_menu_page(page);
}


void display_home_page(DeviceParameter_TypeDef *DevPrmts)
{
    char channel;
    for (channel = 0; channel < 4; channel++) {
		Display_ShowHomepageGainValue(channel, DevPrmts->MusicData.CurrGain[channel]);
        if (DevPrmts->MusicData.Mute[channel])
            Display_ShowHomepageMute(channel);
        else
			Display_ShowHomepageChannel(channel, DevPrmts->Op.ChannelSelect[channel]);
    }
}
