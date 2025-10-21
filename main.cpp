#include <windows.h>
#include <cmath>
#define FILE_SECTION 4

void menuControls(HWND);
void draw(HWND);
LRESULT CALLBACK MainWindowProcedure(HWND, UINT, WPARAM, LPARAM);
int width = 600;
int height = 600;
int WINAPI WinMain(HINSTANCE windowData, HINSTANCE windowsView, LPSTR args, int visible) {
    HBRUSH WINDOWCOLOR = CreateSolidBrush(RGB(0, 0, 0));
    WNDCLASSW windowClass = {0};
    windowClass.hbrBackground = (HBRUSH) WINDOWCOLOR;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = windowData;
    windowClass.lpszClassName = L"WindowClass";
    windowClass.lpfnWndProc = MainWindowProcedure;
    if (!RegisterClassW(&windowClass)) {
        MessageBox(NULL, "Could Not Open", "Report", MB_OK);
        return -1;
    }
    CreateWindowW(L"WindowClass", L"Window APP", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 1920 / 3, 1080 / 4, width, height, NULL, NULL, NULL, NULL);
    MSG message = {0};
    while (GetMessage(&message, nullptr, NULL, NULL)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return 0;
}
void menuControls(HWND render) {
    HMENU menu = CreateMenu();
    AppendMenuW(menu, NULL, FILE_SECTION, L"File");
    SetMenu(render, menu);
}
// varaibles sketch
int x = 0;
int y  = 0;
int xVelocity = 15;
int yVelocity = 15;
float rotationAngle = 0;

void draw(HWND ctx) {
    int radius = 68;
    PAINTSTRUCT paint;
    HDC hdc = BeginPaint(ctx, &paint);
    SetGraphicsMode(hdc, GM_ADVANCED);
    XFORM tranform;
    double xCenter = width / 2;
    double yCenter = height / 2;
    // matrix
    tranform.eM11 = cos(rotationAngle);
    tranform.eM12 = sin(rotationAngle);
    tranform.eM21 = -sin(rotationAngle);
    tranform.eM22 = cos(rotationAngle);
    tranform.eDx = xCenter;
    tranform.eDy = yCenter;
    SetWorldTransform(hdc, &tranform);
    // drawing
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(pen);
    DeleteObject(brush);
    EndPaint(ctx, &paint);
    if (width / 2 + x + radius > width || width / 2 + x - radius < 0 )
        xVelocity *= -1;
    if (height / 2 + y + radius < height || height / 2 + y - radius > 0)
        yVelocity *= -1;
    x += xVelocity;
    y += yVelocity;
}
LRESULT CALLBACK MainWindowProcedure(HWND windowRender, UINT message, WPARAM windowParam, LPARAM windowLP) {
    SetTimer(windowRender, 365726, 0, nullptr);
    switch (message) {
        case WM_TIMER:
            InvalidateRect(windowRender, nullptr, TRUE);
            UpdateWindow(windowRender);
            rotationAngle += 0.01;
            break;
        case WM_DESTROY:
            DestroyWindow(windowRender);
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            draw(windowRender);
        case WM_CREATE:
            menuControls(windowRender);
        default:
            return DefWindowProcW(windowRender, message, windowParam, windowLP);
    }
    return 0;
}
