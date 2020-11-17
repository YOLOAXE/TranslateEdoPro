#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <math.h> 
#include <regex>

#include "proc.h"
#include "InputManager.h"
#include "translate.h"

unsigned __int64 timeMils = 0;
unsigned __int64 nowMils = 0;
double deltaTime = 0.0;

double rateRead = 0.2;
double timerRead = 0.0;

char buffer[3000];
std::string texte;
std::string texteFinal;
unsigned int endChaine = 0;
unsigned int i = 0;
InputManager * Input = new InputManager();
translate * t = new translate();

void ChangePolice()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtain the Console handle
	CONSOLE_FONT_INFOEX ConsoleCurrentFontEx;
	ConsoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, 0, &ConsoleCurrentFontEx);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &ConsoleCurrentFontEx); //retrive all console font informations

	swprintf(ConsoleCurrentFontEx.FaceName, 32, L"Arial");
	ConsoleCurrentFontEx.dwFontSize.Y = 20;
	ConsoleCurrentFontEx.FontWeight = 1000;
	if (SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &ConsoleCurrentFontEx) == 0)
	{
		printf("SetCurrentConsoleFontEx failed");
	}
	SetCurrentConsoleFontEx(hConsole, 0, &ConsoleCurrentFontEx);
}

void cleanUp()
{
	delete[] Input;
	delete[] t;
}

void deltaTimeCalc()
{
	nowMils = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	deltaTime = (double)(nowMils - timeMils) / 1000.0;
	timeMils = nowMils;
}

int main()
{
	DWORD procId = GetProcId(L"EDOPro.exe");
	if (procId == 0)
	{
		std::cout << "EdoPro n'est pas executer." << std::endl;
		return 1;
	}
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"EDOPro.exe");
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x00660E88;
	std::vector<unsigned int> Offsets = { 0x0,0xFC,0x6C,0x20,0xE8,0x0,0xC,0x8,0x90,0x0 };
	uintptr_t valueAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, Offsets);
	while (GetProcId(L"EDOPro.exe") != 0 && !Input->GetButton(0x08))
	{
		deltaTimeCalc();
		timerRead -= deltaTime;
		if (timerRead < 0)
		{
			timerRead = rateRead;
			ReadProcessMemory(hProcess, (BYTE*)valueAddr, &buffer, sizeof(buffer), nullptr);
			for (i = 0, endChaine = 0, texte = ""; i < 3000 && endChaine < 3; i++)
			{
				if (buffer[i] != 0)
				{
					endChaine = 0;
					if (buffer[i] < 123 && buffer[i] > 64 || buffer[i] == 32 || buffer[i] < 58 && buffer[i] > 47)
					{
						texte = texte + buffer[i];
					}
					if (buffer[i] == 59)
					{
						i+=2;
						texte = texte + ' ';
					}
				}
				else
				{
					endChaine++;
				}
			}
			if (texte != texteFinal)
			{
				texteFinal = texte;
				texte = std::regex_replace(texte, std::regex(" or"), " ou");
				texte = std::regex_replace(texte, std::regex(" OR"), " ou");
				texte = std::regex_replace(texte, std::regex(" And"), " et");
				texte = std::regex_replace(texte, std::regex(" from"), " depuis");
				texte = std::regex_replace(texte, std::regex(" in"), " dans");
				system("CLS");
				std::cout << std::flush;
				system("CLS");
				std::cout << t->translateTO(texte) << std::endl << std::endl << "-----------------------------------------------------------------------------------------------------" << std::endl << std::endl;
				std::cout << texte << std::endl;
				std::cout << std::endl << "-----------------------------------------------------------------------------------------------------" << std::endl;
			}
		}
	}
	cleanUp;
}
