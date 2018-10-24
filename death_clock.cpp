#include <windows.h>
#include <atltime.h>

typedef struct _DateTime {
    unsigned long long totalSeconds;
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int days;
    unsigned int months;
    unsigned int years;
} DateTime;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
DateTime CalculateGapTime(CTime, CTime);
LPCTSTR lpszClass = TEXT("Timer");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(30, 30, 30)));
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = NULL;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    //hWnd = CreateWindowEx(WS_EX_TOPMOST, lpszClass, lpszClass, WS_POPUP, 2150, 900, 350, 60, NULL, (HMENU)NULL, hInstance, NULL); // WS_POPUP : 윈도우 타이틀 바 제거, WS_EX_TOPMOST : 창 항상 위에 
    hWnd = CreateWindowEx(WS_EX_TOPMOST, lpszClass, lpszClass, WS_POPUP, 200, 200, 220, 45, NULL, (HMENU)NULL, hInstance, NULL); // 파라미터에서 윈도우의 위치 조정이 필요함. 위의 설정의 X,Y 윈도우 좌표는 200, 200
    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0)){
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    
    return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    //HFONT hFont, OldFont;

    // String array
    //static TCHAR strTimerToday[25];
    //static TCHAR strTimer30[80];
    static TCHAR strTimer80[80];

    // CTime
    CTime currentTime = CTime::GetCurrentTime();
    //CTime endTimeToday(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 23, 59, 59);
    //CTime endTime30(2020, 1, 1, 9, 0, 0);
    CTime endTime80(2070, 1, 1, 9, 0, 0);

    // DateTime
    //DateTime dtToday;
    //DateTime dt30;
    DateTime dt80;

    switch (iMessage){
        case WM_CREATE:
            SetTimer(hWnd, 1, 1000, NULL);
            SendMessage(hWnd, WM_TIMER, 1, 0);
            return 0;
        case WM_TIMER:
            currentTime = CTime::GetCurrentTime();

            //dt30 = CalculateGapTime(currentTime, endTime30);
            dt80 = CalculateGapTime(currentTime, endTime80);
            //dtToday = CalculateGapTime(currentTime, endTimeToday);

            //wsprintf(strTimerToday, TEXT("%d초 남았습니다."), dtToday.totalSeconds);
            //wsprintf(strTimer30, TEXT("%d년 %2d개월 %2d일 %2d시간 %2d분 %2d초 남았습니다."), dt30.years, dt30.months, dt30.days, dt30.hours, dt30.minutes, dt30.seconds);
            wsprintf(strTimer80, TEXT("%10d초 남았습니다."), dt80.totalSeconds);
            //wsprintf(strTimer80, TEXT("%d년 %d개월 %d일 %02d시간 %02d분 %02d초 남았습니다."), dt.years, dt.months, dt.days, dt.hours, dt.minutes, dt.seconds);

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            // Create Font
            //hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));
            //OldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(200, 200, 200));
            SetBkMode(hdc, TRANSPARENT);

            // Text
            //TextOut(hdc, 10, 10, strTimerToday, lstrlen(strTimerToday));
            //TextOut(hdc, 10, 30, strTimer30, lstrlen(strTimer30));

            TextOut(hdc, 15, 15, strTimer80, lstrlen(strTimer80));
            //TextOut(hdc, 10, 30, strTimer80, lstrlen(strTimer80));

            // Remove Font
            //SelectObject(hdc, OldFont);
            //DeleteObject(hFont);

            EndPaint(hWnd, &ps);
            return 0;
        case WM_DESTROY:
            KillTimer(hWnd, 1);
            PostQuitMessage(0);
            return 0;
    }

    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

DateTime CalculateGapTime(CTime currentTime, CTime endTime)
{
    DateTime dt;
    CTimeSpan gapTime;
    unsigned long long temp_1, temp_2;

    gapTime = endTime - currentTime;
    temp_1 = dt.totalSeconds = gapTime.GetTotalSeconds();

    dt.seconds = temp_1 % 60;
    temp_1 = (temp_1 - dt.seconds) / 60; // minutes

    dt.minutes = temp_1 % 60;
    temp_1 = (temp_1 - dt.minutes) / 60; // hours

    dt.hours = temp_1 % 24;
    temp_1 = (temp_1 - dt.hours) / 24; // days

    temp_2 = temp_1 % 364;
    dt.years = (temp_1 - temp_2) / 364; // years

    dt.days = temp_2 % 30;
    dt.months = (temp_2 - dt.days) / 30; // months

    return dt;
}
