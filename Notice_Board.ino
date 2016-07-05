#include <SPI.h>
#include <SD.h>
#include "LedControl.h"
#include <SoftwareSerial.h>

int MatrixNumber = 8;
char incomingByte;
char Buffer[2000];
char Buffer_inverse[2000];
LedControl lc = LedControl(12, 11, 10, MatrixNumber);
const int chipSelect = 53;
File myFile;
int length;
char a, b;
char phrase[1000];
bool flag = 1;
bool latch = 1;
int z = 0;

int fontDefinitions[480] = {
	0x00,0x00,0x00,0x00,0x00,/*space*/ // is 32 in ASCII
	0x00,0xF6,0xF6,0x00,0x00,/*!*/
	0x00,0xE0,0x00,0xE0,0x00,/*"*/
	0x28,0xFE,0x28,0xFE,0x28,/*#*/
	0x00,0x64,0xD6,0x54,0x08,/*$*/
	0xC2,0xCC,0x10,0x26,0xC6,/*%*/
	0x4C,0xB2,0x92,0x6C,0x0A,/*&*/
	0x00,0x00,0xE0,0x00,0x00,/*'*/
	0x00,0x38,0x44,0x82,0x00,/*(*/
	0x00,0x82,0x44,0x38,0x00,/*)*/
	0x88,0x50,0xF8,0x50,0x88,/***/
	0x08,0x08,0x3E,0x08,0x08,/*+*/
	0x00,0x00,0x05,0x06,0x00,/*,*/
	0x08,0x08,0x08,0x08,0x08,/*-*/
	0x00,0x00,0x06,0x06,0x00,/*.*/
	0x02,0x0C,0x10,0x60,0x80,/*/*/
	0x7C,0x8A,0x92,0xA2,0x7C,/*0*/
	0x00,0x42,0xFE,0x02,0x00,/*1*/
	0x42,0x86,0x8A,0x92,0x62,/*2*/
	0x44,0x82,0x92,0x92,0x6C,/*3*/
	0x10,0x30,0x50,0xFE,0x10,/*4*/
	0xE4,0xA2,0xA2,0xA2,0x9C,/*5*/
	0x3C,0x52,0x92,0x92,0x0C,/*6*/
	0x80,0x86,0x98,0xE0,0x80,/*7*/
	0x6C,0x92,0x92,0x92,0x6C,/*8*/
	0x60,0x92,0x92,0x94,0x78,/*9*/
	0x00,0x00,0x36,0x36,0x00,/*:*/
	0x00,0x00,0x35,0x36,0x00,/*;*/
	0x10,0x28,0x44,0x82,0x00,/*<*/
	0x28,0x28,0x28,0x28,0x28,/*=*/
	0x00,0x82,0x44,0x28,0x10,/*>*/
	0x40,0x80,0x8A,0x90,0x60,/*?*/
	0x7C,0x82,0xBA,0xBA,0x62,/*@*/
	0x3E,0x48,0x88,0x48,0x3E,/*A*/
	0xFE,0x92,0x92,0x92,0x6C,/*B*/
	0x7C,0x82,0x82,0x82,0x44,/*C*/
	0xFE,0x82,0x82,0x82,0x7C,/*D*/
	0xFE,0x92,0x92,0x92,0x82,/*E*/
	0xFE,0x90,0x90,0x90,0x80,/*F*/
	0x7C,0x82,0x82,0x8A,0x4E,/*G*/
	0xFE,0x10,0x10,0x10,0xFE,/*H*/
	0x82,0x82,0xFE,0x82,0x82,/*I*/
	0x84,0x82,0xFC,0x80,0x80,/*J*/
	0xFE,0x10,0x28,0x44,0x82,/*K*/
	0xFE,0x02,0x02,0x02,0x02,/*L*/
	0xFE,0x40,0x20,0x40,0xFE,/*M*/
	0xFE,0x60,0x10,0x0C,0xFE,/*N*/
	0x7C,0x82,0x82,0x82,0x7C,/*O*/
	0xFE,0x90,0x90,0x90,0x60,/*P*/
	0x7C,0x82,0x82,0x86,0x7E,/*Q*/
	0xFE,0x90,0x98,0x94,0x62,/*R*/
	0x64,0x92,0x92,0x92,0x4C,/*S*/
	0x80,0x80,0xFE,0x80,0x80,/*T*/
	0xFC,0x02,0x02,0x02,0xFC,/*U*/
	0xF8,0x04,0x02,0x04,0xF8,/*V*/
	0xFC,0x02,0x0C,0x02,0xFC,/*W*/
	0xC6,0x28,0x10,0x28,0xC6,/*X*/
	0xC0,0x20,0x1E,0x20,0xC0,/*Y*/
	0x86,0x8A,0x92,0xA2,0xC2,/*Z*/
	0x00,0x00,0xFE,0x82,0x00,/*[*/
	0x00,0x00,0x00,0x00,0x00,/*this should be / */
	0x80,0x60,0x10,0x0C,0x02,/*]*/
	0x20,0x40,0x80,0x40,0x20,/*^*/
	0x01,0x01,0x01,0x01,0x01,/*_*/
	0x80,0x40,0x20,0x00,0x00,/*`*/
	0x04,0x2A,0x2A,0x2A,0x1E,/*a*/
	0xFE,0x12,0x22,0x22,0x1C,/*b*/
	0x1C,0x22,0x22,0x22,0x14,/*c*/
	0x1C,0x22,0x22,0x12,0xFE,/*d*/
	0x1C,0x2A,0x2A,0x2A,0x18,/*e*/
	0x10,0x7E,0x90,0x80,0x40,/*f*/
	0x18,0x25,0x25,0x25,0x1E,/*g*/
	0xFE,0x10,0x10,0x10,0x0E,/*h*/
	0x00,0x12,0x5E,0x02,0x00,/*i*/
	0x02,0x01,0x01,0x11,0x5E,/*j*/
	0xFE,0x08,0x08,0x14,0x22,/*k*/
	0x00,0x82,0xFE,0x02,0x00,/*l*/
	0x3E,0x20,0x1C,0x20,0x1E,/*m*/
	0x3E,0x20,0x20,0x20,0x1E,/*n*/
	0x1C,0x22,0x22,0x22,0x1C,/*o*/
	0x3F,0x24,0x24,0x24,0x18,/*p*/
	0x18,0x24,0x24,0x3F,0x01,/*q*/
	0x3E,0x10,0x20,0x20,0x10,/*r*/
	0x12,0x2A,0x2A,0x2A,0x04,/*s*/
	0x00,0x10,0x3C,0x12,0x04,/*t*/
	0x3C,0x02,0x02,0x02,0x3E,/*u*/
	0x30,0x0C,0x02,0x0C,0x30,/*v*/
	0x38,0x06,0x18,0x06,0x38,/*w*/
	0x22,0x14,0x08,0x14,0x22,/*x*/
	0x38,0x05,0x05,0x05,0x3E,/*y*/
	0x22,0x26,0x2A,0x32,0x22,/*z*/
	0x00,0x10,0x6C,0x82,0x82,/*{*/
	0x00,0x00,0xFF,0x00,0x00,/*|*/
	0x04,0x02,0xFF,0x02,0x04,/*|, arrow*/
	0x82,0x82,0x6C,0x10,0x00,/*}*/
	0x08,0x10,0x18,0x08,0x10/*~*/
};

void BufferBuilding(char * stringToDisplay, byte stringLength)
{
	int i = 0;

	//loop on every characters (on the string)
	for (int k = 0; k < stringLength; k++) {
		//here we call the characters library  
		int caract = k;
		Buffer[i + 0] = fontDefinitions[((stringToDisplay[caract] - 32) * 5) + 0];
		Buffer[i + 1] = fontDefinitions[((stringToDisplay[caract] - 32) * 5) + 1];
		Buffer[i + 2] = fontDefinitions[((stringToDisplay[caract] - 32) * 5) + 2];
		Buffer[i + 3] = fontDefinitions[((stringToDisplay[caract] - 32) * 5) + 3];
		Buffer[i + 4] = fontDefinitions[((stringToDisplay[caract] - 32) * 5) + 4];
		Buffer[i + 5] = B00000000;  //one blank column of leds between two characters
		i = i + 6;
	}

	// here, we create a second buffer, to display the message on the switched matrixes
	for (int k = 0; k < stringLength * 6; k++) {
		for (int i = 0; i < 8; i++) {
			bitWrite(Buffer_inverse[k], i, bitRead(Buffer[k], 7 - i));
		}
	}
}

void DrawText(byte stringLength) {

	for (int k = 0; k < stringLength * 6; k++) {


		for (int m = 0; m < MatrixNumber; m++) {
			if (m % 2 == 0) //check is the module is upside-down or not...
			{
				//here, the matrix is flipped upside-down                
				for (int i = 0; i < 8; i++) {  //loop on the columns

					lc.setRow(m, 7 - i, Buffer[i + k + 8 * m]);
				}
			}
			else
			{
				//classical   
				for (int i = 0; i < 8; i++) {  //loop on the columns


					lc.setRow(m, i, Buffer_inverse[i + k + 8 * m]);
				}
			}
		}
	}

}

void setup() {

	Serial1.begin(9600);
	Serial1.println("Write Something to file.");
	SD.begin(chipSelect);

	if (SD.exists("GOOGLE.TXT"))
	{
		myFile = SD.open("GOOGLE.TXT");

		while (myFile.available())
		{
			//Serial1.write(myFile.read());
			phrase[z] = myFile.read();
			z++;
		}
		myFile.close();
	}

	Serial1.println(phrase);

	//matrix modules init
	for (int adresse = 0; adresse<MatrixNumber; adresse++) {
		/*The MAX72XX is in power-saving mode on startup*/
		lc.shutdown(adresse, false);
		/* Set the brightness to a medium values */
		lc.setIntensity(adresse, 4);
		/* and clear the display */
		lc.clearDisplay(adresse);
	}

	BufferBuilding(phrase, z);

	Serial1.println("Hi ! I'm waiting for a message.");

}

void loop()
{

	DrawText(z);
	if (Serial1.available())
	{

		myFile = SD.open("GOOGLE.TXT", FILE_WRITE);
		length = Serial1.available();
		for (int i = 0; i < length; i++)
		{
			a = Serial1.read();
			myFile.print(a);
			if (a == '~')
			{
				myFile.close();
				if (SD.exists("GOOGLE.TXT"))
				{
					SD.remove("GOOGLE.TXT");
					Serial1.println("ReWrite");


				}
			}


		}

		myFile.close();
		Serial1.println("Writing Complete!");


		asm volatile ("  jmp 0");

	}


}
