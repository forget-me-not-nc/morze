#define ID_START	9001
#define ID_RICHEDITOUTPUT	9002
#define ID_RICHEDITINPUT	9003
#define	ID_PLAYSOUND	9004

#include <Windows.h>
#include <richedit.h>
#include <map>
#include <string>
#include <clocale>
//---------------------------------------------
using std::string;
using std::map;
//---------------------------------------------
HWND hMainWindow;
HINSTANCE hInstance;

const map<char, string> morzeMap = {
	{'à', ".-"}, {'á', "-..."}, {'â', ".--"}, {'ã', "--."},
	{'ä', "-.."}, {'å', "."}, {'æ', "...-"}, {'ç', "--.."},
	{'è', ".."}, {'é', ".---"}, {'ê', "-.-"}, {'ë', ".-.."},
	{'ì', "--"}, {'í', "-."}, {'î', "---"}, {'ï', ".--."},
	{'ð', ".-."}, {'ñ', "..."}, {'ò', "-"}, {'ó', "..-"},
	{'ô', "..-."}, {'õ', "...."}, {'ö', "-.-."}, {'÷', "---."},
	{'ø', "----"}, {'ù', "--.-"}, {'ú', "-..-"}, {'û', "-.--"},
	{'ü', "-..-"}, {'ý', "..-.."}, {'þ', "..--"}, {'ÿ', ".-.-"},
};
//---------------------------------------------
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//---------------------------------------------
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "Ukrainian");

	MSG			msg{};
	HWND		hwnd;
	WNDCLASSEX	wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = L"MainWindow";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindowEx(NULL, wc.lpszClassName, L"Morze", WS_OVERLAPPEDWINDOW, 350, 100, 650, 450, NULL, NULL, wc.hInstance, NULL); hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
	hMainWindow = hWnd;

	switch (uMsg)
	{
		case WM_CREATE:
		{
			HWND hStartButton = CreateWindow(L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
				263, 60, 100, 40, hWnd, (HMENU)ID_START, hInstance, NULL);

			LoadLibrary(L"riched20.dll");

			HWND hInput = CreateWindowEx(NULL, RICHEDIT_CLASS, NULL,
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_MULTILINE | ES_SAVESEL | ES_SUNKEN,
				25, 150, 275, 150, hWnd, (HMENU)ID_RICHEDITINPUT, hInstance, NULL);
			
			HWND hOutput = CreateWindowEx(NULL, RICHEDIT_CLASS, NULL,
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_MULTILINE | ES_SAVESEL | ES_SUNKEN,
				325, 150, 285, 150, hWnd, (HMENU)ID_RICHEDITOUTPUT, hInstance, NULL);

			HWND hPlaySound = CreateWindow(L"BUTTON", L"PlaySound", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				420, 330, 100, 40, hWnd, (HMENU)ID_PLAYSOUND, hInstance, NULL);

			SendMessage(hWnd, WM_PAINT, 0, 0);

			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_START:
				{
					size_t textSize = GetWindowTextLength((HWND)GetDlgItem(hWnd, ID_RICHEDITINPUT));
					char* buffer = new char[textSize + 1];

					GetWindowTextA((HWND)GetDlgItem(hWnd, ID_RICHEDITINPUT), buffer, textSize + 1);

					string textToMorze = buffer;
					
					delete[] buffer;

					string textToOut = "";
					bool flag = false;

					for (int i = 0; i < textToMorze.length(); i++)
					{
						flag = false;

						for (auto iter : morzeMap)
						{
							if (iter.first == textToMorze[i])
							{
								textToOut += iter.second;
								textToOut += " ";
								flag = true;
								break;
							}
						}

						if (!flag) textToOut += textToMorze[i];
					}

					SetWindowTextA((HWND)GetDlgItem(hWnd, ID_RICHEDITOUTPUT), textToOut.c_str());

					break;
				}
				case ID_PLAYSOUND:
				{
					size_t textSize = GetWindowTextLength((HWND)GetDlgItem(hWnd, ID_RICHEDITOUTPUT));
					char* text = new char[textSize + 1];

					GetWindowTextA((HWND)GetDlgItem(hWnd, ID_RICHEDITOUTPUT), text, textSize + 1);

					string buffer = text;
					
					delete [] text;
					
					for (int i = 0; i < buffer.length(); i++)
					{
						if (buffer[i] == '.')
						{
							Beep(1000, 40);
						}
						else if (buffer[i] == '-')
						{
							Beep(1000, 600);
						}
						else
						{
							Sleep(300);
						}
					}

					break;
				}

				default:break;
			}

			break;
		}
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO min_max = (LPMINMAXINFO)lParam;

			min_max->ptMinTrackSize.x = 650;
			min_max->ptMinTrackSize.y = 450;

			min_max->ptMaxTrackSize.x = 650;
			min_max->ptMaxTrackSize.y = 450;

			break;
		}
		case WM_PAINT:
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(127, 25, 26));
			SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);

			break;
		}
		case WM_DESTROY:
		{
			DestroyWindow(hWnd);
			PostQuitMessage(EXIT_SUCCESS);

			break;
		}

		default: break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
