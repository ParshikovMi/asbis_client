#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>
#include <winsock2.h>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <stdio.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#pragma warning(disable: 4996)

//ARINC-429

#pragma pack(push, 1)
struct BNR //структура для двоичного числа
{
    unsigned int address : 8; //адрес
    unsigned int data : 20; //данные
    unsigned int sign : 1; //знак
    unsigned int SM : 2; //матрица
    unsigned int P : 1; //бит чётности
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BCD //структура для нескольких параметров
{
    unsigned char address : 8; //адрес
    unsigned char : 3;
    unsigned char s : 6; //секунды
    unsigned char m : 6; //минуты
    unsigned char h : 5; //часы
    unsigned char : 1;
    unsigned char SM : 2; //матрица
    unsigned char P : 1; //бит чётности
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Discrete //структура для дискретных данных
{
    unsigned int address : 8; //адрес
    unsigned int SDI : 2; //инс
    unsigned int preparation : 1; //подготовка по ЗК
    unsigned int control : 1; //контроль
    unsigned int navigation : 1; //навигация
    unsigned int hypercomp : 1; //гиперкомпассирование
    unsigned int : 1;
    unsigned int re_start : 1; //повторный старт
    unsigned int scale : 3; //шкала подготовки
    unsigned int heating : 1; //исправность обогрева
    unsigned int temp : 1; //термостатирование
    unsigned int no_data : 1; //нет начальных данных
    unsigned int no_reception : 1; //нет приёма
    unsigned int ins : 1; //исправность ИНС
    unsigned int fast_readiness : 1; //готовность ускоренная
    unsigned int readiness : 1; //готовность
    unsigned int : 3;
    unsigned int SM : 2; //матрица
    unsigned int P : 1; //бит чётности
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SRNS //структура для СРНС
{
    unsigned int address : 8; //адрес
    unsigned int start_data : 1; //запрос начальных данных
    unsigned int type_work_srns : 3; //тип рабочей СРНС
    unsigned int gps : 1; //альманах GPS
    unsigned int glonass : 1; //альманах ГЛОНАСС
    unsigned int operating_mode : 2; //режим работы
    unsigned int work_signal : 1; //подрежимы работы по сигналам СРНС
    unsigned int time : 1; //признак времени
    unsigned int : 2;
    unsigned int dif_work : 1; //дифференциальный режим измерений
    unsigned int : 1;
    unsigned int fail : 1; //отказ изделия
    unsigned int limit : 1; //порог сигнализации
    unsigned int system_cord : 2; //система координат
    unsigned int : 3;
    unsigned int matrix : 2; //матрица состояния
    unsigned int P : 1; //бит чётности
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DATA // структура слова "ДАТА"
{
    unsigned char address : 8; // адрес
    unsigned char : 2;
    unsigned char year : 4; // год
    unsigned char : 4;
    unsigned char month : 4; // месяц
    unsigned char : 1;
    unsigned char day : 4; // день
    unsigned char : 2;
    unsigned char matrix : 2; // матрица состояния
    unsigned char P : 1; // бит чётности
};
#pragma pack(pop)

union PackageArinc
{
    BNR mes1;
    BCD mes2;
    Discrete mes3;
    SRNS mes4;
    DATA mes5;
};

struct INS_prot
{
    PackageArinc width; // широта
    PackageArinc longitude; // долгота
    PackageArinc height; // высота
    PackageArinc course; // курс истинный
    PackageArinc pitch; // угол тангажа
    PackageArinc roll; // угол крена
    PackageArinc Vns; // скорость север/юг
    PackageArinc Vew; // скорость восток/запад
    PackageArinc Vvert; // скорость вертикальная инерциальная
    PackageArinc ax; // ускорение продольное
    PackageArinc az; // ускорение поперечное
    PackageArinc ay; // ускорение нормальное
    PackageArinc SS_INS; // слово состояния ИНС
};

struct SNS_prot
{
    PackageArinc H; // высота
    PackageArinc HDOP; // горизонтальный геометрический фактор
    PackageArinc VDOP; // вертикальный геометрический фактор
    PackageArinc PU; // путевой угол
    PackageArinc B; // текущая широта
    PackageArinc Bt; // текущая широта (точно)
    PackageArinc L; // текущая долгота
    PackageArinc Lt; // текущая долгота (точно)
    PackageArinc NP; // задержка выдачи обновленных НП относительно МВ
    PackageArinc UTC; // текущее время
    PackageArinc UTC_MB; // текущее время непрерывное между метками МВ
    PackageArinc Vh; // вертикальная скорость
    PackageArinc data; // дата
    PackageArinc P_SNS; // признаки СРНС
};

union protocol
{
    INS_prot prot1;
    SNS_prot prot2;
};


condition_variable cv;
mutex mtx;
queue <protocol> MesBuf;
queue <protocol> MesBufSNS;
int fi, alfa;
SYSTEMTIME st;

int calc(double price, int high, int col, double val) // цена старшего разряда, старший разряд, число значащих разрядов, значение
{
    int* bin = new int[col]{};
    int last{};
    double s{};
    int res{};

    for (int i = 0; i < col - 1; i++)
    {
        s = s + (price / pow(2, i));
        if (s > val) 
        {
            bin[i] = 0;
            s = s - (price / pow(2, i));
        }
        else 
        {
            bin[i] = 1;
        }
        last = i;
    }

    for (int i = 0; i < col; i++)
    {
        res += bin[i] * pow(2, col - (i + 1));
    }

    int shift = 28 - high;
    if (shift > 0)
    {
        res = res >> shift;
    }

    return res;
}

int oct_to_dec(int oct)
{
    int dec = 0;

    for (int e = 1; oct != 0; oct /= 10, e *= 8)
        dec += e * (oct % 10);
    return dec;
}

class Timer
{
public:
    void update(void (*f) (SOCKET, sockaddr_in), chrono::milliseconds Tend, SOCKET SendRecvSocket, sockaddr_in ServerAddr)
    {
        chrono::steady_clock::time_point tend = chrono::steady_clock::now() + Tend;
        while (true)
        {
            if (chrono::steady_clock::now() > tend)
            {
                f(SendRecvSocket, ServerAddr);
                tend = chrono::steady_clock::now() + Tend;
            }
        }
    }
};

class TimerOut
{
public:
    void update(void (*f) (protocol), chrono::milliseconds Tend, protocol Mes)
    {
        chrono::steady_clock::time_point tend = chrono::steady_clock::now() + Tend;
        while (true)
        {
            if (chrono::steady_clock::now() > tend)
            {
                f(Mes);
                tend = chrono::steady_clock::now() + Tend;
            }
        }
    }
};

class TimerIN
{
public:
    void update(void (*f) (protocol), chrono::steady_clock::time_point tend, protocol Mes)
    {
        int stop = 0;
        while (stop == 0)
        {
            if (chrono::steady_clock::now() > tend)
            {
                f(Mes);
                stop = 1;
            }
        }
    }
};

void BufPushINS(protocol PackageMes)
{
    mtx.lock();
    MesBuf.push(PackageMes);
    mtx.unlock();
}

protocol completion_INS(PackageArinc PackageMes)
{
    protocol Protocol{};
    Protocol.prot1.width.mes1.address = oct_to_dec(310);
    Protocol.prot1.width.mes1.data = calc(90, 28, 20, 22.5);
    Protocol.prot1.width.mes1.sign = 0;
    Protocol.prot1.width.mes1.SM = 1;
    Protocol.prot1.width.mes1.P = 1;

    Protocol.prot1.longitude.mes1.address = oct_to_dec(311);
    Protocol.prot1.longitude.mes1.data = calc(90, 28, 20, 22.5);
    Protocol.prot1.longitude.mes1.sign = 1;
    Protocol.prot1.longitude.mes1.SM = 1;
    Protocol.prot1.longitude.mes1.P = 1;

    Protocol.prot1.height.mes1.address = oct_to_dec(361);
    Protocol.prot1.height.mes1.data = calc(19975.3728, 28, 19, 3048);
    Protocol.prot1.height.mes1.sign = 0;
    Protocol.prot1.height.mes1.SM = 1;
    Protocol.prot1.height.mes1.P = 1;

    Protocol.prot1.course.mes1.address = oct_to_dec(314);
    Protocol.prot1.course.mes1.data = calc(90, 28, 16, 10);
    Protocol.prot1.course.mes1.sign = 0;
    Protocol.prot1.course.mes1.SM = 1;
    Protocol.prot1.course.mes1.P = 1;

    Protocol.prot1.pitch.mes1.address = oct_to_dec(324);
    Protocol.prot1.pitch.mes1.data = calc(90, 28, 16, 5);
    Protocol.prot1.pitch.mes1.sign = 0;
    Protocol.prot1.pitch.mes1.SM = 1;
    Protocol.prot1.pitch.mes1.P = 1;

    Protocol.prot1.roll.mes1.address = oct_to_dec(325);
    Protocol.prot1.roll.mes1.data = calc(90, 28, 20, 45);
    Protocol.prot1.roll.mes1.sign = 0;
    Protocol.prot1.roll.mes1.SM = 1;
    Protocol.prot1.roll.mes1.P = 1;

    Protocol.prot1.Vns.mes1.address = oct_to_dec(366);
    Protocol.prot1.Vns.mes1.data = calc(3792.896, 28, 19, 370.4);
    Protocol.prot1.Vns.mes1.sign = 0;
    Protocol.prot1.Vns.mes1.SM = 1;
    Protocol.prot1.Vns.mes1.P = 1;

    Protocol.prot1.Vew.mes1.address = oct_to_dec(367);
    Protocol.prot1.Vew.mes1.data = calc(3792.896, 28, 19, 370.4);
    Protocol.prot1.Vew.mes1.sign = 0;
    Protocol.prot1.Vew.mes1.SM = 1;
    Protocol.prot1.Vew.mes1.P = 1;

    Protocol.prot1.Vvert.mes1.address = oct_to_dec(365);
    Protocol.prot1.Vvert.mes1.data = calc(4993.8432, 28, 19, 182.88);
    Protocol.prot1.Vvert.mes1.sign = 0;
    Protocol.prot1.Vvert.mes1.SM = 1;
    Protocol.prot1.Vvert.mes1.P = 1;

    Protocol.prot1.ax.mes1.address = oct_to_dec(331);
    Protocol.prot1.ax.mes1.data = calc(2, 28, 12, 0.02);
    Protocol.prot1.ax.mes1.sign = 0;
    Protocol.prot1.ax.mes1.SM = 1;
    Protocol.prot1.ax.mes1.P = 1;

    Protocol.prot1.az.mes1.address = oct_to_dec(332);
    Protocol.prot1.az.mes1.data = calc(2, 28, 12, 0.1);
    Protocol.prot1.az.mes1.sign = 0;
    Protocol.prot1.az.mes1.SM = 1;
    Protocol.prot1.az.mes1.P = 1;

    Protocol.prot1.ay.mes1.address = oct_to_dec(333);
    Protocol.prot1.ay.mes1.data = calc(2, 28, 12, 0.1);;
    Protocol.prot1.ay.mes1.sign = 0;
    Protocol.prot1.ay.mes1.SM = 1;
    Protocol.prot1.ay.mes1.P = 1;

    Protocol.prot1.SS_INS = PackageMes;

    return Protocol;
}

struct INS
{
    void Control() // Контроль
    {
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "ИНС: Запущен тест-контроль\n";

        PackageArinc PackageMes{};

        PackageMes.mes3.address = oct_to_dec(270); // Формирование слова данных
        PackageMes.mes3.SDI = 1;
        PackageMes.mes3.preparation = 0;
        PackageMes.mes3.control = 1;
        PackageMes.mes3.navigation = 0;
        PackageMes.mes3.hypercomp = 0;
        PackageMes.mes3.re_start = 0;
        PackageMes.mes3.scale = 0;
        PackageMes.mes3.heating = 0;
        PackageMes.mes3.temp = 0;
        PackageMes.mes3.no_data = 1;
        PackageMes.mes3.no_reception = 0;
        PackageMes.mes3.ins = 1;
        PackageMes.mes3.fast_readiness = 0;
        PackageMes.mes3.readiness = 0;
        PackageMes.mes3.SM = 0;
        PackageMes.mes3.P = 1;

        protocol Protocol{};
        Protocol = completion_INS(PackageMes);

        chrono::steady_clock::time_point tend = chrono::steady_clock::now() + chrono::milliseconds(20000); // отсечка в 20 секунд

        void(*pushmes)(protocol);
        pushmes = BufPushINS;

        TimerIN TimerControl;
        TimerControl.update(pushmes, tend, Protocol);
    }

    void Preparation() // Подготовка
    {
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "ИНС: Запущена подготовка\n";

        PackageArinc PackageMes{};
        PackageMes.mes3.address = oct_to_dec(270); // Формирование слова данных
        PackageMes.mes3.SDI = 1;
        PackageMes.mes3.preparation = 1;
        PackageMes.mes3.control = 0;
        PackageMes.mes3.navigation = 0;
        PackageMes.mes3.hypercomp = 0;
        PackageMes.mes3.re_start = 0;
        PackageMes.mes3.scale = 0;
        PackageMes.mes3.heating = 0;
        PackageMes.mes3.temp = 0;
        PackageMes.mes3.no_data = 1;
        PackageMes.mes3.no_reception = 0;
        PackageMes.mes3.ins = 1;
        PackageMes.mes3.fast_readiness = 0;
        PackageMes.mes3.readiness = 0;
        PackageMes.mes3.SM = 0;
        PackageMes.mes3.P = 1;

        protocol Protocol{};
        Protocol = completion_INS(PackageMes);

        void(*sendmes)(protocol);
        sendmes = BufPushINS;
        
        TimerIN TimerPreparation;
        while (fi == 0 && alfa == 0)
        {
            chrono::steady_clock::time_point tend = chrono::steady_clock::now() + chrono::milliseconds(1000); // частота формирования данных от ИНС (400 Гц)
            TimerPreparation.update(sendmes, tend, Protocol);
        }

        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "ИНС: получены значения fi и alfa\n";
        chrono::steady_clock::time_point End = chrono::steady_clock::now() + chrono::milliseconds(20000); // отсечка в 2 минуты
        
        PackageArinc PackageMes2{};
        PackageMes2.mes3.address = oct_to_dec(270); // Формирование слова данных
        PackageMes2.mes3.SDI = 1;
        PackageMes2.mes3.preparation = 1;
        PackageMes2.mes3.control = 0;
        PackageMes2.mes3.navigation = 0;
        PackageMes2.mes3.hypercomp = 0;
        PackageMes2.mes3.re_start = 0;
        PackageMes2.mes3.scale = 0;
        PackageMes2.mes3.heating = 0;
        PackageMes2.mes3.temp = 0;
        PackageMes2.mes3.no_data = 0;
        PackageMes2.mes3.no_reception = 0;
        PackageMes2.mes3.ins = 1;
        PackageMes2.mes3.fast_readiness = 0;
        PackageMes2.mes3.readiness = 0;
        PackageMes2.mes3.SM = 0;
        PackageMes2.mes3.P = 1;

        protocol Protocol2{};
        Protocol2 = completion_INS(PackageMes2);

        void(*sendmes2)(protocol);
        sendmes2 = BufPushINS;
        
        TimerIN TimerPreparation2;

        while (chrono::steady_clock::now() < End)
        {
            chrono::steady_clock::time_point tend2 = chrono::steady_clock::now() + chrono::milliseconds(1000); // частота формирования данных от ИНС (400 Гц)
            TimerPreparation2.update(sendmes2, tend2, Protocol2);
        }

    }

    void Navigation() // Навигация
    {
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "ИНС: Запущен режим навигации\n";

        PackageArinc PackageMes{};
        PackageMes.mes3.address = oct_to_dec(270); // Формирование слова данных
        PackageMes.mes3.SDI = 1;
        PackageMes.mes3.preparation = 0;
        PackageMes.mes3.control = 0;
        PackageMes.mes3.navigation = 1;
        PackageMes.mes3.hypercomp = 0;
        PackageMes.mes3.re_start = 0;
        PackageMes.mes3.scale = 0;
        PackageMes.mes3.heating = 0;
        PackageMes.mes3.temp = 0;
        PackageMes.mes3.no_data = 0;
        PackageMes.mes3.no_reception = 0;
        PackageMes.mes3.ins = 1;
        PackageMes.mes3.fast_readiness = 0;
        PackageMes.mes3.readiness = 0;
        PackageMes.mes3.SM = 0;
        PackageMes.mes3.P = 1;

        protocol Protocol{};
        Protocol = completion_INS(PackageMes);

        void(*sendmes)(protocol);
        sendmes = BufPushINS;

        TimerOut TimerNavigation;
        TimerNavigation.update(sendmes, chrono::milliseconds(1000), Protocol); // запуск таймера с частотой формирования данных от ИНС (400 Гц)
    }

};

void BufPushSNS(protocol PackageMes)
{
    mtx.lock();
    MesBufSNS.push(PackageMes);
    mtx.unlock();
}

protocol completion_SNS(PackageArinc PackageMes)
{
    protocol Protocol{};
    Protocol.prot2.H.mes1.address = oct_to_dec(76);
    Protocol.prot2.H.mes1.data = calc(65536, 28, 20, 2354);
    Protocol.prot2.H.mes1.sign = 1;
    Protocol.prot2.H.mes1.SM = 1;
    Protocol.prot2.H.mes1.P = 1;

    Protocol.prot2.HDOP.mes1.address = oct_to_dec(101);
    Protocol.prot2.HDOP.mes1.data = calc(512, 28, 15, 23);
    Protocol.prot2.HDOP.mes1.sign = 0;
    Protocol.prot2.HDOP.mes1.SM = 1;
    Protocol.prot2.HDOP.mes1.P = 1;

    Protocol.prot2.VDOP.mes1.address = oct_to_dec(102);
    Protocol.prot2.VDOP.mes1.data = calc(512, 28, 15, 32);
    Protocol.prot2.VDOP.mes1.sign = 0;
    Protocol.prot2.VDOP.mes1.SM = 1;
    Protocol.prot2.VDOP.mes1.P = 1;

    Protocol.prot2.PU.mes1.address = oct_to_dec(103);
    Protocol.prot2.PU.mes1.data = calc(90, 28, 15, 45);
    Protocol.prot2.PU.mes1.sign = 1;
    Protocol.prot2.PU.mes1.SM = 1;
    Protocol.prot2.PU.mes1.P = 1;

    Protocol.prot2.B.mes1.address = oct_to_dec(110);
    Protocol.prot2.B.mes1.data = calc(45, 28, 20, 23);
    Protocol.prot2.B.mes1.sign = 0;
    Protocol.prot2.B.mes1.SM = 1;
    Protocol.prot2.B.mes1.P = 1;

    Protocol.prot2.Bt.mes1.address = oct_to_dec(120);
    Protocol.prot2.Bt.mes1.data = calc(0.0000858, 28, 11, 0.000076);
    Protocol.prot2.Bt.mes1.sign = 0;
    Protocol.prot2.Bt.mes1.SM = 1;
    Protocol.prot2.Bt.mes1.P = 1;

    Protocol.prot2.L.mes1.address = oct_to_dec(111);
    Protocol.prot2.L.mes1.data = calc(90, 28, 20, 64);
    Protocol.prot2.L.mes1.sign = 0;
    Protocol.prot2.L.mes1.SM = 1;
    Protocol.prot2.L.mes1.P = 1;

    Protocol.prot2.Lt.mes1.address = oct_to_dec(121);
    Protocol.prot2.Lt.mes1.data = calc(0.0000858, 28, 11, 0.000034);
    Protocol.prot2.Lt.mes1.sign = 0;
    Protocol.prot2.Lt.mes1.SM = 1;
    Protocol.prot2.Lt.mes1.P = 1;

    Protocol.prot2.NP.mes1.address = oct_to_dec(113);
    Protocol.prot2.NP.mes1.data = calc(512, 28, 20, 73);
    Protocol.prot2.NP.mes1.sign = 0;
    Protocol.prot2.NP.mes1.SM = 1;
    Protocol.prot2.NP.mes1.P = 1;

    GetLocalTime(&st);
    Protocol.prot2.UTC.mes2.address = oct_to_dec(150);
    Protocol.prot2.UTC.mes2.s = calc(16, 28, 5, st.wSecond);
    Protocol.prot2.UTC.mes2.m = calc(32, 23, 6, st.wMinute);
    Protocol.prot2.UTC.mes2.m = calc(32, 17, 6, st.wHour);
    Protocol.prot2.UTC.mes2.SM = 1;
    Protocol.prot2.UTC.mes2.P = 1;

    Protocol.prot2.UTC_MB.mes1.address = oct_to_dec(140);
    Protocol.prot2.UTC_MB.mes1.data = calc(512, 28, 20, 14);
    Protocol.prot2.UTC_MB.mes1.sign = 0;
    Protocol.prot2.UTC_MB.mes1.SM = 1;
    Protocol.prot2.UTC_MB.mes1.P = 1;

    Protocol.prot2.Vh.mes1.address = oct_to_dec(165);
    Protocol.prot2.Vh.mes1.data = calc(16384, 28, 15, 2324);
    Protocol.prot2.Vh.mes1.sign = 0;
    Protocol.prot2.Vh.mes1.SM = 1;
    Protocol.prot2.Vh.mes1.P = 1;

    GetLocalTime(&st);
    Protocol.prot2.data.mes5.address = oct_to_dec(260);
    Protocol.prot2.data.mes5.year = calc(50, 14, 4, st.wYear);
    Protocol.prot2.data.mes5.month = calc(6, 22, 4, st.wMonth);
    Protocol.prot2.data.mes5.day = calc(16, 27, 4, st.wMonth);
    Protocol.prot2.data.mes5.matrix = 1;
    Protocol.prot2.data.mes5.P = 1;

    Protocol.prot2.P_SNS = PackageMes;

    return Protocol;
}

struct SNS
{
    void Control()
    {
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "СНС: Запущен тест-контроль\n";

        PackageArinc PackageMes{};

        PackageMes.mes4.address = oct_to_dec(273);
        PackageMes.mes4.start_data = 0;
        PackageMes.mes4.type_work_srns = 0;
        PackageMes.mes4.gps = 0;
        PackageMes.mes4.glonass = 0;
        PackageMes.mes4.operating_mode = 2;
        PackageMes.mes4.work_signal = 1;
        PackageMes.mes4.time = 0;
        PackageMes.mes4.dif_work = 0;
        PackageMes.mes4.fail = 0;
        PackageMes.mes4.limit = 0;
        PackageMes.mes4.system_cord = 0;
        PackageMes.mes4.matrix = 0;
        PackageMes.mes4.P = 1;

        protocol Protocol{};
        Protocol = completion_SNS(PackageMes);

        chrono::steady_clock::time_point tend1 = chrono::steady_clock::now() + chrono::milliseconds(20000); // отсечка в 20 секунд
        
        TimerIN TimerControl;
        void(*pushmes)(protocol);
        pushmes = BufPushSNS;
        TimerControl.update(pushmes, tend1, Protocol);
        
        chrono::steady_clock::time_point End = chrono::steady_clock::now() + chrono::milliseconds(20000); // отсечка в 2 минуты
        
        while (chrono::steady_clock::now() < End)
        {
            chrono::steady_clock::time_point tend = chrono::steady_clock::now() + chrono::milliseconds(1000); // частота формирования данных от СНС (10 Гц)
            TimerControl.update(pushmes, tend, Protocol);
        }
    }

    void Navigation()
    {
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d.%03d ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

        cout << "СНС: Запущен режим навигации\n";

        fi = 24;
        alfa = 32;

        PackageArinc PackageMes{};
        PackageMes.mes4.address = oct_to_dec(273);
        PackageMes.mes4.start_data = 0;
        PackageMes.mes4.type_work_srns = 0;
        PackageMes.mes4.gps = 0;
        PackageMes.mes4.glonass = 0;
        PackageMes.mes4.operating_mode = 2;
        PackageMes.mes4.work_signal = 0;
        PackageMes.mes4.time = 0;
        PackageMes.mes4.dif_work = 0;
        PackageMes.mes4.fail = 0;
        PackageMes.mes4.limit = 0;
        PackageMes.mes4.system_cord = 1;
        PackageMes.mes4.matrix = 0;
        PackageMes.mes4.P = 1;

        protocol Protocol{};
        Protocol = completion_SNS(PackageMes);

        void(*sendmes)(protocol);
        sendmes = BufPushSNS;

        TimerOut TimerNavigation;
        TimerNavigation.update(sendmes, chrono::milliseconds(1000), Protocol); // запуск таймера с частотой формирования данных от СНС (10 Гц)

    }

};


void SendingINS(SOCKET SendRecvSocket, sockaddr_in ServerAddr)
{

    protocol data;
    protocol type{};
    type.prot1.width.mes1.address = 1;

    mtx.lock();

    // Создание сокета для подключения к серверу
    SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(12346);

    if (!MesBuf.empty())
    {
        // Отправление запроса на сервер
        sendto(SendRecvSocket, (char*)&type, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        data = MesBuf.front();
        sendto(SendRecvSocket, (char*)&data, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        MesBuf.pop();
    }

    closesocket(SendRecvSocket);

    mtx.unlock();
}


void SendDataINS()
{

    WSADATA wsaData;
    SOCKET SendRecvSocket{};  // Сокет для приема и передачи
    sockaddr_in ServerAddr{};  // Адрес сервера и клиентов

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    protocol type{};
    type.prot1.width.mes1.address = 3;

    // Создание сокета для подключения к серверу
    SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(12346);

    sendto(SendRecvSocket, (char*)&type, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    closesocket(SendRecvSocket);

    Timer TimerControl;

    chrono::milliseconds tend = chrono::milliseconds(1000); // Частота отправки данных от ИНС (100 Гц)

    void(*sendmes)(SOCKET, sockaddr_in);
    sendmes = SendingINS;
    TimerControl.update(sendmes, tend, SendRecvSocket, ServerAddr);
}

void SendingSNS(SOCKET SendRecvSocket, sockaddr_in ServerAddr)
{

    protocol data;
    protocol type{};
    type.prot1.width.mes1.address = 2;
    mtx.lock();


    // Создание сокета для подключения к серверу
    SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(12346);

    if (!MesBufSNS.empty())
    {
        // Отправление запроса на сервер
        sendto(SendRecvSocket, (char*)&type, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        data = MesBufSNS.front();
        sendto(SendRecvSocket, (char*)&data, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        MesBufSNS.pop();
    }

    closesocket(SendRecvSocket);

    mtx.unlock();
}


void SendDataSNS()
{

    WSADATA wsaData;
    SOCKET SendRecvSocket{};  // Сокет для приема и передачи
    sockaddr_in ServerAddr{};  // Адрес сервера и клиентов

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    protocol type{};
    type.prot1.width.mes1.address = 4;

    // Создание сокета для подключения к серверу
    SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(12346);

    sendto(SendRecvSocket, (char*)&type, sizeof(protocol), 0, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    closesocket(SendRecvSocket);

    Timer TimerControl;

    chrono::milliseconds tend = chrono::milliseconds(1500); // Частота отправки данных от СНС (1 Гц)

    void(*sendmes)(SOCKET, sockaddr_in);
    sendmes = SendingSNS;
    TimerControl.update(sendmes, tend, SendRecvSocket, ServerAddr);
}

void ModelINS()
{
    INS INS;
    INS.Control();
    INS.Preparation();
    INS.Navigation();
}

void ModelSNS()
{
    SNS SNS;
    SNS.Control();
    SNS.Navigation();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ModelINS, NULL, NULL, NULL);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendDataINS, NULL, NULL, NULL);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ModelSNS, NULL, NULL, NULL);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendDataSNS, NULL, NULL, NULL);

    while (true)
    {
        int p = 0;
    }

    return 0;
}