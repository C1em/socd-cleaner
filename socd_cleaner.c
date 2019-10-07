/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socd_cleaner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coremart <coremart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 19:25:54 by coremart          #+#    #+#             */
/*   Updated: 2019/10/06 19:25:54 by coremart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socd_cleaner.h"
#include <stdbool.h>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#pragma comment(lib,"user32.lib")

bool	hard[2];// keys on hardware (keyboard)
bool	soft[2];// keys send to software (all apps)

char	left;
char	right;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char key;
	KBDLLHOOKSTRUCT *pKeyBoard;
	INPUT ip;

	if (nCode != HC_ACTION || ((KBDLLHOOKSTRUCT*)lParam)->flags & 16)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		key = (char)((KBDLLHOOKSTRUCT*)lParam)->vkCode;
		if (key == left)
		{
			hard[KEY_1] = IS_DOWN;
			if (hard[KEY_2] == IS_DOWN)
			{
				if (soft[KEY_2] == IS_DOWN)
				{
					ip.type = INPUT_KEYBOARD;
					ip.ki = (KEYBDINPUT){right, 0, KEYEVENTF_KEYUP, 0, 0};
					SendInput(1, &ip, sizeof(INPUT));
					soft[KEY_2] = IS_UP;
				}
				return (1);
			}
			soft[KEY_1] = IS_DOWN;
		}
		else if (key == right)
		{
			hard[KEY_2] = IS_DOWN;
			if (hard[KEY_1] == IS_DOWN)
			{
				if (soft[KEY_1] == IS_DOWN)
				{
					ip.type = INPUT_KEYBOARD;
					ip.ki = (KEYBDINPUT){left, 0, KEYEVENTF_KEYUP, 0, 0};
					SendInput(1, &ip, sizeof(INPUT));
					soft[KEY_1] = IS_UP;
				}
				return (1);
			}
			soft[KEY_2] = IS_DOWN;
		}
	}
	else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
	{
		key = (char)((KBDLLHOOKSTRUCT*)lParam)->vkCode;
		if (key == left)
		{
			hard[KEY_1] = IS_UP;
			if (hard[KEY_2] == IS_DOWN)
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki = (KEYBDINPUT){right, 0, 0, 0, 0};
				SendInput(1, &ip, sizeof(INPUT));
				soft[KEY_2] = IS_DOWN;
				return (1);
			}
		}
		else if (key == right)
		{
			hard[KEY_2] = IS_UP;
			if (hard[KEY_1] == IS_DOWN)
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki = (KEYBDINPUT){left, 0, 0, 0, 0};
				SendInput(1, &ip, sizeof(INPUT));
				soft[KEY_1] = IS_DOWN;
				return (1);
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int  main(void)
{
	HHOOK hhook;
	MSG msg;

	printf("This socd cleaner do not clean the bottom and up inputs.\nOnly the left and right directions will be set to neutral when input at the same time.\n\nOnly letters are suported\n\n");
	printf("enter the left key: ");
	scanf(" %1c", &left);
	left = toupper(left);
	printf("enter the right key: ");
	scanf(" %1c", &right);
	right = toupper(right);
	printf("\n\nPress ctrl + c in this window to exit\n");
	printf("\n\nsocd cleaner running !\n");
	hard[KEY_1] = IS_UP;
	hard[KEY_2] = IS_UP;
	soft[KEY_1] = IS_UP;
	soft[KEY_2] = IS_UP;
	hhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL) , 0);
		while (GetMessage(&msg, NULL, 0, 0))
		;
	return (0);
}
