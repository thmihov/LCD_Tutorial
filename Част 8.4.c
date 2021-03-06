#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#define LCD_CTRL PORTB //Порта на който са закачени управляващите линии - RS и EN
#define LCD_DATA PORTD //Порта на който са закачени линиите за данни D4÷D7
#define LCD_CTRL_DDR DDRB //DDR на порта за управление
#define LCD_DATA_DDR DDRD //DDR на порта за данните
#define RS PB5 //D13
#define EN PB4 //D12
#define D4 PD0 //D0
#define D5 PD1 //D1
#define D6 PD2 //D2
#define D7 PD3 //D3

//Дефиниции на методи
void PortSetup(); //Инициализира портове и пинове
void LCD_CMD (uint8_t); //Изпраща инструкция към дисплея
void LCDInit(); //Инициализира дисплея
void LCD_DAT(uint8_t); //Изпраща символ към дисплея

int main(void)
{
	PortSetup(); //Инициализираме портвете и пиновете
	LCDInit(); //Инициализираме дисплея.След тази стъпка сме готови да пишем по него
	
	LCD_CMD(0x83); //Курсор на позиция 3 от първи ред
	LCD_DAT('<');
	LCD_DAT('<');
	LCD_DAT('T'); 
        LCD_DAT('M');
        LCD_DAT('i');
        LCD_DAT('h');
	LCD_DAT('o');
	LCD_DAT('v');
	LCD_DAT('>');
	LCD_DAT('>');
	LCD_CMD(0xC6); //Курсор на позиция 6 от втори ред
	LCD_DAT('B');
	LCD_DAT('L');
	LCD_DAT('O');
	LCD_DAT('G');

	while(1)
	{
	}
}

//Методи

//Метода конфигурира портовете и пиновете
void PortSetup()
{
	LCD_CTRL_DDR|=(1<<RS)|(1<<EN); //RS и EN са изходи;
	LCD_DATA_DDR|=(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7); //Изходи за данни;
	LCD_DATA&=~(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7); //Нули на изходите за данни (контролно);
	LCD_CTRL&=~(1<<RS)|(1<<EN); //Нули на контролните изходи;
}

//Метода приема инструкция (command) и я изпраща към дисплея
void LCD_CMD (uint8_t command)
{
	LCD_DATA=(LCD_DATA&0xF0)|((command&0xF0)>>4); //Записва старшия полубайт от инструкцията на порта
	LCD_CTRL&=~(1<<RS); //RS=0
	LCD_CTRL|=(1<<EN); //Вдига EN
	_delay_us(1); //Пауза от 1uS
	LCD_CTRL&=~(1<<EN); //EN=0
	_delay_us(100); //Изчаква 100uS за да възприеме дисплея инфото
	LCD_DATA=(LCD_DATA&0xF0)|(command&0x0F); //Изпраща младшия полубайт от командата на порта
	LCD_CTRL|=(1<<EN); //Вдига EN
	_delay_us(1); //Пауза от 1uS
	LCD_CTRL&=~(1<<EN); //EN=0
	_delay_us(100); //Изчаква 100uS за да възприеме дисплея инфото
}

//Метода инициализира дисплея (изпраща поредица от инструкции)
void LCDInit(void)
{
	_delay_ms(100); //Чака контролно 100ms за да се инициализира дисплея
	LCD_CMD(0x02); //Function Set: 4-бита
	_delay_ms(10); //Пауза
	LCD_CMD(0x2C); //Function Set: 4-бита,2 линии,5х10 размер на символите
	LCD_CMD(0x0F); //Display Control: Дисплея е пуснат,курсора е пуснат,курсора мига
	LCD_CMD(0x06); //Entry Mode Set: Курсора се движи надясно при писане,без отместване на дисплея
	LCD_CMD(0x01);//Изчиства дисплея
	_delay_ms(2);
	LCD_CMD(0x02); //Връща курсова в начална позиция
	_delay_ms(2);
}

//Метода приема символ (sym) и го изпраща към дисплея
void LCD_DAT (uint8_t sym)
{
	LCD_DATA=(LCD_DATA&0xF0)|((sym&0xF0)>>4); //Записва старшия полубайт от инструкцията на порта
	LCD_CTRL|=(1<<RS); //RS=1
	LCD_CTRL|=(1<<EN); //Вдига EN
	_delay_us(1); //Пауза от 1uS
	LCD_CTRL&=~(1<<EN); //EN=0
	_delay_us(100); //Изчаква 100uS за да възприеме дисплея инфото
	LCD_DATA=(LCD_DATA&0xF0)|(sym&0x0F); //Изпраща младшия полубайт от командата на порта
	LCD_CTRL|=(1<<EN); //Вдига EN
	_delay_us(1); //Пауза от 1uS
	LCD_CTRL&=~(1<<EN); //EN=0
	_delay_us(100); //Изчаква 100uS за да възприеме дисплея инфото
}
