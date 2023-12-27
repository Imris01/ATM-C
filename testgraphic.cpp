#include "ATM.h"

// 全局变量
HWND hEdit;
char* g_pTextBuffer = NULL;

// 绘制按钮的函数
void drawButton(Button btn) 
{
    rectangle(btn.left, btn.top, btn.right, btn.bottom); // 绘制按钮的矩形
    outtextxy((btn.left + btn.right) / 2 -40, (btn.top + btn.bottom) / 2, btn.text); // 绘制按钮的文本
}

// 检查鼠标点击是否在按钮上的函数
int isMouseOnButton(int x, int y, Button btn) 
{
    return x > btn.left && x < btn.right && y > btn.top && y < btn.bottom;
}


int main_interface()
{
    // 初始化图形界面
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    // 创建按钮
    Button userBtn = { 100, 200, 250, 250, "用户" };
    Button adminBtn = { 400, 200, 550, 250, "管理员" };

    // 绘制按钮
    settextstyle(16, 0, "黑体");
    drawButton(userBtn);
    drawButton(adminBtn);

    // 消息循环
    MOUSEMSG m; // 鼠标消息
    while (true) 
    {
        m = GetMouseMsg(); // 获取一条鼠标消息
        switch (m.uMsg) 
        {
        case WM_LBUTTONDOWN: 
            // 鼠标左键按下
            if (isMouseOnButton(m.x, m.y, userBtn)) 
            {
                // 用户按钮被点击
                closegraph();
                return MAIN_USER;
            }
            else if (isMouseOnButton(m.x, m.y, adminBtn)) 
            {
                // 管理员按钮被点击
                closegraph();
                return MAIN_ADMIN;
            }
            break;
        }
    }

    // 关闭图形界面
    closegraph();
    return 0;
}

int user_interface(simple_list* cardlist, int* id)
{
    // 初始化图形界面
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    // 创建按钮
    Button loginBtn = { 100, 200, 250, 250, "登录" };
    Button openBtn = { 400, 200, 550, 250, "开户" };
    Button forgetBtn = { 200, 350, 440, 400, "忘记密码？" };
    Button backBtn = { 270, 410, 370, 460, "返回" };
    Button backBtn2 = { 380, 410, 470, 460, "返回" };

    bool showMain = true;

    // 绘制按钮
    settextstyle(16, 0, "黑体");
    drawButton(loginBtn);
    drawButton(openBtn);
    drawButton(forgetBtn);
    drawButton(backBtn);

    // 消息循环
    MOUSEMSG m; // 鼠标消息
    while (true)
    {
        m = GetMouseMsg(); // 获取一条鼠标消息
        switch (m.uMsg)
        {
        case WM_LBUTTONDOWN:
            // 鼠标左键按下
            if (isMouseOnButton(m.x, m.y, loginBtn) && showMain)
            {
                // 登录按钮被点击
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "用户操作中...");
                char login_id[20] = { 0 };
                char login_password[20] = { 0 };

                CreateInputWindow(login_id, "输入ID", "CWindowClass", WndProcC);
                CreateInputWindow(login_password, "输入密码", "HWindowClass", WndProcH);

                int logstatus = login_U(login_id, login_password, cardlist, id);

                if (logstatus == LOG_SUC)
                {
                    closegraph();
                    return U_TO_F;
                }
                else
                {
                    cleardevice();
                    outtextxy(100, 100, "登录失败！");
                    drawButton(backBtn2);
                }
            }
            
            else if (isMouseOnButton(m.x, m.y, openBtn) && showMain)
            {
                // 开户按钮被点击
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "用户操作中...");
                int openstatus = OpenRequest("PendingRequest.txt", cardlist);

                if (!openstatus)
                {
                    outtextxy(100, 100, "请求发送失败！");
                    drawButton(backBtn2);
                }
                else
                {
                    char idstr[15];
                    sprintf(idstr, "您的ID:%d", id_assignment(cardlist, "PendingRequest.txt"));
                    outtextxy(100, 100, idstr);
                    outtextxy(100, 100, "请求发送成功！");
                    drawButton(backBtn2);
                }
            }
            else if (isMouseOnButton(m.x, m.y, forgetBtn) && showMain)
            {
                // 忘记密码按钮被点击
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "用户操作中...");
                int fgtstatus = ForgetRequest_U("PendingRequest.txt", cardlist);

                if (fgtstatus == 0)
                {
                    outtextxy(100, 100, "请求发送失败！");
                    drawButton(backBtn2);
                }
                else if(fgtstatus == 1)
                {
                    outtextxy(100, 100, "请求发送成功！");
                    drawButton(backBtn2);
                }
            }
            else if (isMouseOnButton(m.x, m.y, backBtn) && showMain)
            {
                closegraph();
                return F_TO_U;
            }
            else if (isMouseOnButton(m.x, m.y, backBtn2) && !showMain)
            {
                cleardevice();
                showMain = true;

                drawButton(loginBtn);
                drawButton(openBtn);
                drawButton(forgetBtn);
                drawButton(backBtn);
            }
            break;
        }
    }

    // 关闭图形界面
    closegraph();
    return F_TO_U;
}

int user_function(simple_list* cardlist,int id)
{
    // 初始化图形界面
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    DATA* dst;
    dst = search(cardlist, id);

    // 创建按钮
    Button queryBtn = { 20, 20, 100, 70, "余额" };
    Button depositBtn = { 20, 150, 100, 200, "存款" };
    Button withdrawBtn = { 20, 280, 100, 330, "取款" };
    Button transferBtn = { 20, 410, 100, 460, "转账" };
    Button recordBtn = { 560, 280, 620, 330, "记录" };
    Button exitBtn = { 560, 410, 620, 460, "退出" };
    Button backBtn = { 270, 410, 370, 460, "返回" };
    Button nextBtn = { 550, 440, 630, 480, "下一页" };
    Button lastBtn = { 10, 440, 90, 480, "上一页" };

    //显示状态变量
    bool showMain = true;

    char welcomeText[30] = { 0 };
    sprintf(welcomeText, "Hello,%s!", dst->userinfo.username);

    // 绘制按钮
    settextstyle(16, 0, "黑体");
    drawButton(queryBtn);
    drawButton(depositBtn);
    drawButton(withdrawBtn);
    drawButton(transferBtn);
    drawButton(recordBtn);
    drawButton(exitBtn);
    outtextxy(250, 240, welcomeText);

    // 消息循环
    MOUSEMSG m; // 鼠标消息
    while (true)
    {
        m = GetMouseMsg(); // 获取一条鼠标消息
        switch (m.uMsg)
        {
        case WM_LBUTTONDOWN:
            // 鼠标左键按下
            if (isMouseOnButton(m.x, m.y, queryBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                char balanceText[30] = { 0 };
                rectangle(50, 50, 590, 300);
                sprintf(balanceText, "您的余额：%.2f", dst->money);
                outtextxy(200, 175, balanceText);
                drawButton(backBtn);
            }
            
            else if (isMouseOnButton(m.x, m.y, depositBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                char money1[10];
                float DMoney;
                CreateInputWindow(money1, "输入金额", "CWindowClass", WndProcC);
                DMoney = atof(money1);
                if (DMoney <= 0)
                {
                    outtextxy(200, 175, "非法输入，存款失败！");
                    drawButton(backBtn);
                }
                else
                {
                    dst->money += DMoney;
                    char crttimeD[20] = { 0 };
                    getCurrentTimeAsString(crttimeD, 20);
                    addRecordToFile(dst->id, crttimeD, "deposit", DMoney);
                    saveListToFile(cardlist,"userdata.txt");
                    outtextxy(200, 175, "存款成功！");
                    drawButton(backBtn);
                }
            }
            else if (isMouseOnButton(m.x, m.y, withdrawBtn)&& showMain)
            {
                cleardevice();
                showMain = false;
                char money2[10];
                float WMoney;
                CreateInputWindow(money2, "输入金额", "FWindowClass", WndProc_Float);
                WMoney = atof(money2);
                if (WMoney <= 0)
                {
                    outtextxy(200, 175, "非法输入，取款失败！");
                    drawButton(backBtn);
                }
                else if(WMoney > dst->money)
                {
                    outtextxy(200, 175, "余额不足，取款失败！");
                    drawButton(backBtn);
                }
                else 
                {
                    dst->money -= WMoney;
                    char crttimeW[20] = { 0 };
                    getCurrentTimeAsString(crttimeW, 20);
                    addRecordToFile(dst->id, crttimeW, "withdraw", -WMoney);
                    saveListToFile(cardlist,"userdata.txt");
                    outtextxy(200, 175, "取款成功！");
                    drawButton(backBtn);
                }
            }
            else if (isMouseOnButton(m.x, m.y, transferBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                char Trans_id[10];
                char money3[10];
                int TID;
                float TMoney;
                CreateInputWindow(Trans_id, "转账卡号", "CWindowClass", WndProcC);
                TID = atoi(Trans_id);
                CreateInputWindow(money3, "转账金额", "CWindowClass", WndProcC);
                TMoney = atof(money3);
                DATA* Transdst = search(cardlist, TID);

                if (TMoney <= 0 || Transdst==NULL)
                {
                    outtextxy(200, 175, "非法输入，转账失败！");
                    drawButton(backBtn);
                }
                else if (TMoney > dst->money && Transdst)
                {
                    outtextxy(200, 175, "余额不足，转账失败！");
                    drawButton(backBtn);
                }
                else
                {
                    dst->money -= TMoney;
                    Transdst->money += TMoney;
                    char crttimeT[20] = { 0 };
                    getCurrentTimeAsString(crttimeT, 20);
                    addRecordToFile(dst->id, crttimeT, "transfer", -TMoney);
                    addRecordToFile(Transdst->id, crttimeT, "transfer", TMoney);
                    saveListToFile(cardlist, "userdata.txt");
                    outtextxy(200, 175, "转账成功！");
                    drawButton(backBtn);
                }
            }
            
            else if (isMouseOnButton(m.x, m.y, recordBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                Record *prev = loadRecordsFromFile(dst->id);
                outtextxy(130, 30, "操作时间");
                outtextxy(240, 30, "操作类型");
                outtextxy(350, 30, "操作金额");
                int yt = 30;
                while (prev)
                {
                    char opmoney[10] = { 0 };
                    yt += 30;
                    snprintf(opmoney, sizeof(opmoney), "%.2f", prev->money);
                    outtextxy(40, yt, prev->op_time);
                    outtextxy(240, yt, prev->op_type);
                    outtextxy(350, yt, opmoney);
                    prev = prev->next;
                }
                drawButton(backBtn);
            }
            else if (isMouseOnButton(m.x, m.y, backBtn) && !showMain)
            {
                cleardevice();
                showMain = true;
                drawButton(queryBtn);
                drawButton(depositBtn);
                drawButton(withdrawBtn);
                drawButton(transferBtn);
                drawButton(recordBtn);
                drawButton(exitBtn);
                outtextxy(250, 240, welcomeText);
            }
            else if (isMouseOnButton(m.x, m.y, exitBtn) && showMain)
            {
                return EXIT;
            }
            break;
        }
    }

    // 关闭图形界面
    closegraph();
    return 0;
}

// 窗口过程函数
LRESULT CALLBACK WndProcH(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButton;
    static char savedText[20] = { 0 }; // 用于保存输入的字符串

    switch (message) {
    case WM_CREATE:
        // 创建密码输入框
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
            10, 10, 200, 20, hWnd, (HMENU)IDC_EDITBOX, GetModuleHandle(NULL), NULL);
        // 创建确认按钮
        hButton = CreateWindow("BUTTON", "确认",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 40, 100, 24, hWnd, (HMENU)IDC_SUBMIT_BUTTON, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SUBMIT_BUTTON)
        {
            // 当确认按钮被点击时，弹出确认对话框
            int msgboxID = MessageBox(
                NULL,
                (LPCSTR)"确认密码?",
                (LPCSTR)"Confirmation",
                MB_ICONQUESTION | MB_YESNO
            );

            switch (msgboxID)
            {
            case IDYES:
                // 用户点击"Yes"，保存输入的字符串并销毁窗口
                GetWindowText(hEdit, g_pTextBuffer, 20);
                DestroyWindow(hWnd);
                break;
            case IDNO:
                // 用户点击"No"，清空输入框以便用户重新输入
                SetWindowText(hEdit, "");
                break;
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 窗口过程函数
LRESULT CALLBACK WndProcC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButton;

    switch (message) {
    case WM_CREATE:
        // 创建编辑控件
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
            10, 10, 200, 20, hWnd, (HMENU)IDC_EDITBOX, GetModuleHandle(NULL), NULL);
        // 创建按钮控件
        hButton = CreateWindow("BUTTON", "确认",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 40, 100, 24, hWnd, (HMENU)IDC_SUBMIT_BUTTON, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SUBMIT_BUTTON)
        {
            // 当按钮被点击时
            GetWindowText(hEdit, g_pTextBuffer, 20);
            DestroyWindow(hWnd);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc_Float(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButton;
    // ...

    switch (message) {
        // ...
    case WM_CHAR:
    {
        TCHAR ch = (TCHAR)wParam;
        if ((ch < '0' || ch > '9') && ch != '\b' && ch != '.') {
            // 不是数字、退格键或小数点，则不处理
            return 0;
        }

        if (ch == '.') {
            // 检查是否已经有小数点
            int length = GetWindowTextLength(hEdit);
            TCHAR* buffer = (TCHAR*)malloc((length + 1) * sizeof(TCHAR));
            GetWindowText(hEdit, buffer, length + 1);

            if (_tcschr(buffer, '.') != NULL || length == 0) {
                // 如果已经有小数点或者是第一个字符，则不处理
                free(buffer);
                return 0;
            }
            free(buffer);
        }
        else if (ch >= '0' && ch <= '9') {
            // 检查小数点后的数字数量
            int length = GetWindowTextLength(hEdit);
            TCHAR* buffer = (TCHAR*)malloc((length + 2) * sizeof(TCHAR));
            GetWindowText(hEdit, buffer, length + 1);
            buffer[length] = ch;
            buffer[length + 1] = '\0';

            TCHAR* point = _tcschr(buffer, '.');
            if (point != NULL && (buffer + length - point > 2)) {
                // 如果小数点后的数字超过两位，则不处理
                free(buffer);
                return 0;
            }
            free(buffer);
        }
        break;
    }
    // ...
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 创建并显示窗口的函数
void CreateInputWindow(char* Text, const char* windowname, const char* className, WNDPROC wndproc)
{
    WNDCLASSEX wc = { 0 };
    g_pTextBuffer = Text;

    // 检查是否已经注册了该类名
    if (!GetClassInfoEx(GetModuleHandle(NULL), className, &wc))
    {
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = 0;
        wc.lpfnWndProc = wndproc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = className;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc))
        {
            MessageBox(NULL, "Window Registration Failed!", "Error!",
                MB_ICONEXCLAMATION | MB_OK);
            return;
        }
    }

    HWND hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        className,
        windowname,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hWnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // 消息循环
    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

int main()
{
    simple_list* userlist = create_userlist();
    userlist = loadListFromFile(userlist, "userdata.txt");
    if (userlist == NULL)return 0;
    int flag = 0;
    int flagU = 0;
    int* userid = (int*)malloc(sizeof(int));

    // 隐藏控制台窗口
    /*HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_HIDE);*/

    while (true)
    {
        flag = main_interface();
        if (flag == MAIN_USER)
        {
            flagU = user_interface(userlist, userid);
        }
        if (flagU == U_TO_F)
        {
            if (user_function(userlist, *userid) == EXIT)
                break;
        }

    }
    return 0;
}