#include <windows.h>
#include <cstdio>
#include <math.h>
#include <stdlib.h>
#include <winuser.h>

HINSTANCE hInst;
HWND hWnd;
POINT lastCursorPosition;
BOOL isDrawing = FALSE, loser = FALSE, winner = FALSE;
LONG firstCursorPosX, firstCursorPosY;
INT scorer = NULL;
INT predictedBest = NULL;
INT radiush = NULL;
BOOL moved = FALSE;
BOOL dot = FALSE;
INT total = 0, tmove = 0;
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(!loser && !winner){
    int centerX = 0;
    int centerY = 0;

    if (hWnd) {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        centerX = windowWidth / 2;
        centerY = windowHeight / 2;
        dot = true;
    }
    switch (message) {

    case WM_CREATE:
        lastCursorPosition.x = -1;
        lastCursorPosition.y = -1;
        break;
    case WM_MOUSEMOVE:
        if (isDrawing && lastCursorPosition.x != -1) {
            tmove++;
            HDC hdc = GetDC(hWnd);
            DrawLine(hdc, lastCursorPosition.x, lastCursorPosition.y, LOWORD(lParam), HIWORD(lParam));
            ReleaseDC(hWnd, hdc);
            int currentPos = abs(lastCursorPosition.x - centerX) + abs(lastCursorPosition.y - centerY);
            /*if(!loser && scorer - 50 < radiush || scorer + 50 > radiush) {
                MessageBox(NULL, "nie", "oaoo", MB_ICONEXCLAMATION | MB_OK);
                loser = true;
                CloseWindow(hWnd);
            }*/
            if(currentPos + radiush/1.5 < radiush || currentPos - radiush/1.5 > radiush) {
                loser = true;
                MessageBox(
                        NULL,
                        "Pomyliles sie zbyt bardzo",
                        "Ups!",
                        MB_ICONEXCLAMATION | MB_OK
                );
            }

            if(firstCursorPosX < lastCursorPosition.x + 3 && firstCursorPosX  > lastCursorPosition.x - 3 &&
               firstCursorPosY < lastCursorPosition.y + 3 && firstCursorPosY  > lastCursorPosition.y - 3 && tmove > 10 && !winner) {
                scorer = scorer/total;
                printf("\nTwoj ostateczny wynik to: ");
                printf("%d", scorer);
                printf("\nPrzewidywany najlepszy wynik to: ");
                printf("%d", radiush);
                printf("\nOznacza to, ze srednio pomyliles sie o ");
                printf("%d", abs(radiush-scorer));
                printf(" pikseli");
                winner = true;
                int value = abs(radiush - scorer);
                char buffer[0xff];
                sprintf(buffer, "Srednio pomyliles sie o : %d\n", value, " pikseli");

                MessageBoxA(NULL, buffer, "WYGRANA!", MB_OK);

            } else {
                scorer += abs(currentPos-predictedBest);
                total++;
            }
        }

        lastCursorPosition.x = LOWORD(lParam);
        lastCursorPosition.y = HIWORD(lParam);
        break;
    case WM_LBUTTONDOWN:
        isDrawing = TRUE;
        lastCursorPosition.x = LOWORD(lParam);
        lastCursorPosition.y = HIWORD(lParam);
        if(predictedBest == NULL) {
            firstCursorPosX = lastCursorPosition.x;
            firstCursorPosY = lastCursorPosition.y;
            radiush = abs(lastCursorPosition.x - centerX) + abs(lastCursorPosition.y - centerY);

            predictedBest = 3*radiush^2;
        }
        break;
    case WM_LBUTTONUP:
        isDrawing = FALSE;
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush);
            Ellipse(hdc, centerX - 5, centerY - 5, centerX + 5, centerY + 5);
            DeleteObject(hBrush);

            HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Arial");
            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(0, 0, 0));

            RECT rect;
            GetClientRect(hWnd, &rect);
            rect.top = -400;
            DrawText(hdc, "Narysuj kolo! Jesli pomylisz sie zbyt bardzo, program sie zamknie.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            DeleteObject(hFont);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    } else {
    CloseWindow(hWnd);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = "Gierka";
    RegisterClassEx(&wcex);

    hWnd = CreateWindow(wcex.lpszClassName, "Gra kolo rysowanie", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
