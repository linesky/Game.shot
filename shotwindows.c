
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 16
#define CELL_SIZE 40
#define NUM_CIRCLES 16

typedef struct {
    int x;
    int y;
} Circle;

Circle circles[NUM_CIRCLES];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawGrid(HDC hdc);
void DrawCircles(HDC hdc);
void InitializeCircles();
void AddNewCircle();
int CheckCircleHit(int x, int y);
void RemoveCircle(int index);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] ="CircleShootingGame";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
   wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    RegisterClass(&wc);

     HWND hwnd = CreateWindowEx(
        0,                            
        CLASS_NAME,                   
        "Circle Shooting Game",      
        WS_OVERLAPPEDWINDOW,          
        CW_USEDEFAULT, CW_USEDEFAULT, 
        GRID_SIZE * CELL_SIZE + 16,   
        GRID_SIZE * CELL_SIZE + 39,   
        NULL,                         
        NULL,                         
        hInstance,                    
        NULL                          
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    srand((unsigned int)time(NULL));
    InitializeCircles();

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            DrawGrid(hdc);
            DrawCircles(hdc);
            EndPaint(hwnd, &ps);
        } break;

        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam) / CELL_SIZE;
            int y = HIWORD(lParam) / CELL_SIZE;
            int hitIndex = CheckCircleHit(x, y);
            if (hitIndex != -1) {
                RemoveCircle(hitIndex);
                AddNewCircle();
                InvalidateRect(hwnd, NULL, TRUE);
            }
        } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void DrawGrid(HDC hdc) {
    for (int i = 0; i <= GRID_SIZE; ++i) {
        MoveToEx(hdc, i * CELL_SIZE, 0, NULL);
        LineTo(hdc, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
        MoveToEx(hdc, 0, i * CELL_SIZE, NULL);
        LineTo(hdc, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
    }
}

void DrawCircles(HDC hdc) {
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH oldBrush = SelectObject(hdc, brush);

    for (int i = 0; i < NUM_CIRCLES; ++i) {
        int x1 = circles[i].x * CELL_SIZE + 5;
        int y1 = circles[i].y * CELL_SIZE + 5;
        int x2 = (circles[i].x + 1) * CELL_SIZE - 5;
        int y2 = (circles[i].y + 1) * CELL_SIZE - 5;
        Ellipse(hdc, x1, y1, x2, y2);
    }

    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}

void InitializeCircles() {
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        AddNewCircle();
    }
}

void AddNewCircle() {
    int x, y;
    int unique;
    do {
        unique = 1;
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        for (int i = 0; i < NUM_CIRCLES; ++i) {
            if (circles[i].x == x && circles[i].y == y) {
                unique = 0;
                break;
            }
        }
    } while (!unique);
    circles[NUM_CIRCLES - 1].x = x;
    circles[NUM_CIRCLES - 1].y = y;
}

int CheckCircleHit(int x, int y) {
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        if (circles[i].x == x && circles[i].y == y) {
            return i;
        }
    }
    return -1;
}

void RemoveCircle(int index) {
    for (int i = index; i < NUM_CIRCLES - 1; ++i) {
        circles[i] = circles[i + 1];
    }
    circles[NUM_CIRCLES - 1].x = -1;
    circles[NUM_CIRCLES - 1].y = -1;
}
