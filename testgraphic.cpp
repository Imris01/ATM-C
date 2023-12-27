#include "ATM.h"

// ȫ�ֱ���
HWND hEdit;
char* g_pTextBuffer = NULL;

// ���ư�ť�ĺ���
void drawButton(Button btn) 
{
    rectangle(btn.left, btn.top, btn.right, btn.bottom); // ���ư�ť�ľ���
    outtextxy((btn.left + btn.right) / 2 -40, (btn.top + btn.bottom) / 2, btn.text); // ���ư�ť���ı�
}

// ���������Ƿ��ڰ�ť�ϵĺ���
int isMouseOnButton(int x, int y, Button btn) 
{
    return x > btn.left && x < btn.right && y > btn.top && y < btn.bottom;
}


int main_interface()
{
    // ��ʼ��ͼ�ν���
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    // ������ť
    Button userBtn = { 100, 200, 250, 250, "�û�" };
    Button adminBtn = { 400, 200, 550, 250, "����Ա" };

    // ���ư�ť
    settextstyle(16, 0, "����");
    drawButton(userBtn);
    drawButton(adminBtn);

    // ��Ϣѭ��
    MOUSEMSG m; // �����Ϣ
    while (true) 
    {
        m = GetMouseMsg(); // ��ȡһ�������Ϣ
        switch (m.uMsg) 
        {
        case WM_LBUTTONDOWN: 
            // ����������
            if (isMouseOnButton(m.x, m.y, userBtn)) 
            {
                // �û���ť�����
                closegraph();
                return MAIN_USER;
            }
            else if (isMouseOnButton(m.x, m.y, adminBtn)) 
            {
                // ����Ա��ť�����
                closegraph();
                return MAIN_ADMIN;
            }
            break;
        }
    }

    // �ر�ͼ�ν���
    closegraph();
    return 0;
}

int user_interface(simple_list* cardlist, int* id)
{
    // ��ʼ��ͼ�ν���
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    // ������ť
    Button loginBtn = { 100, 200, 250, 250, "��¼" };
    Button openBtn = { 400, 200, 550, 250, "����" };
    Button forgetBtn = { 200, 350, 440, 400, "�������룿" };
    Button backBtn = { 270, 410, 370, 460, "����" };
    Button backBtn2 = { 380, 410, 470, 460, "����" };

    bool showMain = true;

    // ���ư�ť
    settextstyle(16, 0, "����");
    drawButton(loginBtn);
    drawButton(openBtn);
    drawButton(forgetBtn);
    drawButton(backBtn);

    // ��Ϣѭ��
    MOUSEMSG m; // �����Ϣ
    while (true)
    {
        m = GetMouseMsg(); // ��ȡһ�������Ϣ
        switch (m.uMsg)
        {
        case WM_LBUTTONDOWN:
            // ����������
            if (isMouseOnButton(m.x, m.y, loginBtn) && showMain)
            {
                // ��¼��ť�����
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "�û�������...");
                char login_id[20] = { 0 };
                char login_password[20] = { 0 };

                CreateInputWindow(login_id, "����ID", "CWindowClass", WndProcC);
                CreateInputWindow(login_password, "��������", "HWindowClass", WndProcH);

                int logstatus = login_U(login_id, login_password, cardlist, id);

                if (logstatus == LOG_SUC)
                {
                    closegraph();
                    return U_TO_F;
                }
                else
                {
                    cleardevice();
                    outtextxy(100, 100, "��¼ʧ�ܣ�");
                    drawButton(backBtn2);
                }
            }
            
            else if (isMouseOnButton(m.x, m.y, openBtn) && showMain)
            {
                // ������ť�����
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "�û�������...");
                int openstatus = OpenRequest("PendingRequest.txt", cardlist);

                if (!openstatus)
                {
                    outtextxy(100, 100, "������ʧ�ܣ�");
                    drawButton(backBtn2);
                }
                else
                {
                    char idstr[15];
                    sprintf(idstr, "����ID:%d", id_assignment(cardlist, "PendingRequest.txt"));
                    outtextxy(100, 100, idstr);
                    outtextxy(100, 100, "�����ͳɹ���");
                    drawButton(backBtn2);
                }
            }
            else if (isMouseOnButton(m.x, m.y, forgetBtn) && showMain)
            {
                // �������밴ť�����
                cleardevice();
                showMain = false;
                outtextxy(100, 100, "�û�������...");
                int fgtstatus = ForgetRequest_U("PendingRequest.txt", cardlist);

                if (fgtstatus == 0)
                {
                    outtextxy(100, 100, "������ʧ�ܣ�");
                    drawButton(backBtn2);
                }
                else if(fgtstatus == 1)
                {
                    outtextxy(100, 100, "�����ͳɹ���");
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

    // �ر�ͼ�ν���
    closegraph();
    return F_TO_U;
}

int user_function(simple_list* cardlist,int id)
{
    // ��ʼ��ͼ�ν���
    initgraph(640, 480);
    setbkcolor(BLACK);
    cleardevice();

    DATA* dst;
    dst = search(cardlist, id);

    // ������ť
    Button queryBtn = { 20, 20, 100, 70, "���" };
    Button depositBtn = { 20, 150, 100, 200, "���" };
    Button withdrawBtn = { 20, 280, 100, 330, "ȡ��" };
    Button transferBtn = { 20, 410, 100, 460, "ת��" };
    Button recordBtn = { 560, 280, 620, 330, "��¼" };
    Button exitBtn = { 560, 410, 620, 460, "�˳�" };
    Button backBtn = { 270, 410, 370, 460, "����" };
    Button nextBtn = { 550, 440, 630, 480, "��һҳ" };
    Button lastBtn = { 10, 440, 90, 480, "��һҳ" };

    //��ʾ״̬����
    bool showMain = true;

    char welcomeText[30] = { 0 };
    sprintf(welcomeText, "Hello,%s!", dst->userinfo.username);

    // ���ư�ť
    settextstyle(16, 0, "����");
    drawButton(queryBtn);
    drawButton(depositBtn);
    drawButton(withdrawBtn);
    drawButton(transferBtn);
    drawButton(recordBtn);
    drawButton(exitBtn);
    outtextxy(250, 240, welcomeText);

    // ��Ϣѭ��
    MOUSEMSG m; // �����Ϣ
    while (true)
    {
        m = GetMouseMsg(); // ��ȡһ�������Ϣ
        switch (m.uMsg)
        {
        case WM_LBUTTONDOWN:
            // ����������
            if (isMouseOnButton(m.x, m.y, queryBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                char balanceText[30] = { 0 };
                rectangle(50, 50, 590, 300);
                sprintf(balanceText, "������%.2f", dst->money);
                outtextxy(200, 175, balanceText);
                drawButton(backBtn);
            }
            
            else if (isMouseOnButton(m.x, m.y, depositBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                char money1[10];
                float DMoney;
                CreateInputWindow(money1, "������", "CWindowClass", WndProcC);
                DMoney = atof(money1);
                if (DMoney <= 0)
                {
                    outtextxy(200, 175, "�Ƿ����룬���ʧ�ܣ�");
                    drawButton(backBtn);
                }
                else
                {
                    dst->money += DMoney;
                    char crttimeD[20] = { 0 };
                    getCurrentTimeAsString(crttimeD, 20);
                    addRecordToFile(dst->id, crttimeD, "deposit", DMoney);
                    saveListToFile(cardlist,"userdata.txt");
                    outtextxy(200, 175, "���ɹ���");
                    drawButton(backBtn);
                }
            }
            else if (isMouseOnButton(m.x, m.y, withdrawBtn)&& showMain)
            {
                cleardevice();
                showMain = false;
                char money2[10];
                float WMoney;
                CreateInputWindow(money2, "������", "FWindowClass", WndProc_Float);
                WMoney = atof(money2);
                if (WMoney <= 0)
                {
                    outtextxy(200, 175, "�Ƿ����룬ȡ��ʧ�ܣ�");
                    drawButton(backBtn);
                }
                else if(WMoney > dst->money)
                {
                    outtextxy(200, 175, "���㣬ȡ��ʧ�ܣ�");
                    drawButton(backBtn);
                }
                else 
                {
                    dst->money -= WMoney;
                    char crttimeW[20] = { 0 };
                    getCurrentTimeAsString(crttimeW, 20);
                    addRecordToFile(dst->id, crttimeW, "withdraw", -WMoney);
                    saveListToFile(cardlist,"userdata.txt");
                    outtextxy(200, 175, "ȡ��ɹ���");
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
                CreateInputWindow(Trans_id, "ת�˿���", "CWindowClass", WndProcC);
                TID = atoi(Trans_id);
                CreateInputWindow(money3, "ת�˽��", "CWindowClass", WndProcC);
                TMoney = atof(money3);
                DATA* Transdst = search(cardlist, TID);

                if (TMoney <= 0 || Transdst==NULL)
                {
                    outtextxy(200, 175, "�Ƿ����룬ת��ʧ�ܣ�");
                    drawButton(backBtn);
                }
                else if (TMoney > dst->money && Transdst)
                {
                    outtextxy(200, 175, "���㣬ת��ʧ�ܣ�");
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
                    outtextxy(200, 175, "ת�˳ɹ���");
                    drawButton(backBtn);
                }
            }
            
            else if (isMouseOnButton(m.x, m.y, recordBtn) && showMain)
            {
                cleardevice();
                showMain = false;
                Record *prev = loadRecordsFromFile(dst->id);
                outtextxy(130, 30, "����ʱ��");
                outtextxy(240, 30, "��������");
                outtextxy(350, 30, "�������");
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

    // �ر�ͼ�ν���
    closegraph();
    return 0;
}

// ���ڹ��̺���
LRESULT CALLBACK WndProcH(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButton;
    static char savedText[20] = { 0 }; // ���ڱ���������ַ���

    switch (message) {
    case WM_CREATE:
        // �������������
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
            10, 10, 200, 20, hWnd, (HMENU)IDC_EDITBOX, GetModuleHandle(NULL), NULL);
        // ����ȷ�ϰ�ť
        hButton = CreateWindow("BUTTON", "ȷ��",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 40, 100, 24, hWnd, (HMENU)IDC_SUBMIT_BUTTON, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SUBMIT_BUTTON)
        {
            // ��ȷ�ϰ�ť�����ʱ������ȷ�϶Ի���
            int msgboxID = MessageBox(
                NULL,
                (LPCSTR)"ȷ������?",
                (LPCSTR)"Confirmation",
                MB_ICONQUESTION | MB_YESNO
            );

            switch (msgboxID)
            {
            case IDYES:
                // �û����"Yes"������������ַ��������ٴ���
                GetWindowText(hEdit, g_pTextBuffer, 20);
                DestroyWindow(hWnd);
                break;
            case IDNO:
                // �û����"No"�����������Ա��û���������
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

// ���ڹ��̺���
LRESULT CALLBACK WndProcC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    static HWND hButton;

    switch (message) {
    case WM_CREATE:
        // �����༭�ؼ�
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
            10, 10, 200, 20, hWnd, (HMENU)IDC_EDITBOX, GetModuleHandle(NULL), NULL);
        // ������ť�ؼ�
        hButton = CreateWindow("BUTTON", "ȷ��",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 40, 100, 24, hWnd, (HMENU)IDC_SUBMIT_BUTTON, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SUBMIT_BUTTON)
        {
            // ����ť�����ʱ
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
            // �������֡��˸����С���㣬�򲻴���
            return 0;
        }

        if (ch == '.') {
            // ����Ƿ��Ѿ���С����
            int length = GetWindowTextLength(hEdit);
            TCHAR* buffer = (TCHAR*)malloc((length + 1) * sizeof(TCHAR));
            GetWindowText(hEdit, buffer, length + 1);

            if (_tcschr(buffer, '.') != NULL || length == 0) {
                // ����Ѿ���С��������ǵ�һ���ַ����򲻴���
                free(buffer);
                return 0;
            }
            free(buffer);
        }
        else if (ch >= '0' && ch <= '9') {
            // ���С��������������
            int length = GetWindowTextLength(hEdit);
            TCHAR* buffer = (TCHAR*)malloc((length + 2) * sizeof(TCHAR));
            GetWindowText(hEdit, buffer, length + 1);
            buffer[length] = ch;
            buffer[length + 1] = '\0';

            TCHAR* point = _tcschr(buffer, '.');
            if (point != NULL && (buffer + length - point > 2)) {
                // ���С���������ֳ�����λ���򲻴���
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

// ��������ʾ���ڵĺ���
void CreateInputWindow(char* Text, const char* windowname, const char* className, WNDPROC wndproc)
{
    WNDCLASSEX wc = { 0 };
    g_pTextBuffer = Text;

    // ����Ƿ��Ѿ�ע���˸�����
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

    // ��Ϣѭ��
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

    // ���ؿ���̨����
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