#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include "screen.h"
#include "unistd.h"

#define DRY_WINTER 0
#define DRY_SUMMER 1
#define DRY_NONE   2

#ifdef JP
#define ERROR_NAN    "エラー : 数字ではありません\n"
#define CONTINUE_MSG "続行しますか?(y/n) : "
#define THANKS_MSG   "ご使用ありがとうございました!!\n"
#define INTRODUCTION "一つ前の月を再入力するにはCtrl+Cを入力してください。\n"
#else
#define ERROR_NAN "Error : Not a number\n"
#define CONTINUE_MSG "Continue?(y/n) : "
#define THANKS_MSG   "Thank you for using this software!!\n"
#define INTRODUCTION "Ctrl+C for retyping previous value.\n"
#endif

void inputActivity(double* temps, double* rains);
void judgeClimate(double* temps, double* rains, char* char3);
char* getClimateStringBy3Char(const char* c3);
void onCtrlC(int i);

#ifndef JP
const char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
#endif

static bool CtrlCPressed = false;

int main(void) {
    double temps[12] = {0};
    double rains[12] = {0};
    
    if (signal(SIGINT, onCtrlC) == SIG_ERR) exit(3); // set Ctrl+C signal handler
    
    printf("Keppen Judger\n");
    printf("Copyright 2018 (c) QCFium\n\n");
    
    inputActivity(temps, rains);
   
    char char3[4];
    judgeClimate(temps, rains, char3);
    char* climateStr = getClimateStringBy3Char(char3);
    if (climateStr)
        printf("%s\n", climateStr);
    else
        printf("NULL\n");
    sleep((unsigned int) 5);
    printf(THANKS_MSG);
    system("pause");
   
    return 0;
}

void inputActivity(double* temps, double* rains) {
    /*
        double temps[12], double rains[12]
        Handle user input and store the result to the pointed arrays
    */
    
    // Introductions
    setTerminalColor(FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
    printf(INTRODUCTION);
    resetTerminalColor();
    
    // input(both temperature and rainfall)
    bool isRain = false;
    for (int i = 0; i < 12; i++) {
#ifdef JP
        printf("%d月の%s : ", i+1, (isRain) ? "降水量" : "気温");
#else
        printf("%s in %s : ", (isRain) ? "Rainfall" : "Temperature", months[i]);
#endif
        bool continueFlag = false;
        while (scanf("%9lf", (isRain) ? &rains[i] : &temps[i]) != 1) {
            Sleep(10);
            if (CtrlCPressed) {
                CtrlCPressed = false;
                if (i > 0) i -= 2;
				else if (isRain) {
					isRain = false;
					i = 10;
				}
                else i--; // entering january's temperature so stay
                printf("\n");
                continueFlag = true;
                break;
            }
            scanf("%*s"); // clear input buffer
            
            setTerminalColor(FOREGROUND_RED|FOREGROUND_INTENSITY); // red
            printf(ERROR_NAN);
            resetTerminalColor();
            
#ifdef JP
            printf("%d月の%s : ", i+1, (isRain) ? "降水量" : "気温");
#else
            printf("%s in %s : ", (isRain) ? "Rainfall" : "Temperature", months[i]);
#endif
        }
        if (continueFlag) continue;
        
        // too hot/cold much/negative rainfall
        if ((isRain && (rains[i] < 0  || rains[i] > 10000)) ||
            (!isRain && (temps[i] > 100 || temps[i] < -100))) {
            setTerminalColor(FOREGROUND_INTENSITY|FOREGROUND_GREEN);
            
            char res;
            while(getchar() != '\n'); // clear stdin
#ifdef JP
            if (isRain)
                printf("%s : %dmm\n続行しますか?(y/n) : ", (rains[i] > 100) ? "雨が降りすぎです" : "雨量がマイナスです", (int)rains[i]);
            else
                printf("%s : %d℃\n続行しますか?(y/n) : ", (temps[i] > 100) ? "暑すぎです" : "寒すぎです", (int)temps[i]);
#else    
            if (isRain)
                printf("%s rainfall : %dmm\nContinue(y/n) : ", (rains[i] > 100) ? "Too much" : "Negative", (int)rains[i]);
            else
                printf("Too %s : %d℃\nContinue(y/n) : ", (temps[i] > 100) ? "hot" : "cold", (int)temps[i]);
#endif
            while ((res = getchar()) != 'y' && res != 'Y' && res != 'n' && res != 'N') {
                while(getchar() != '\n');
                printf(CONTINUE_MSG);
            }
            
            resetTerminalColor();
            if (res == 'n' || res == 'N') {
                // re-input
                i--;
                continue;
            }
        }
		
		if (i >= 11 && !isRain) {
			// enter rainfall inputting
			isRain = true;
			i = -1;
			continue;
		}
    }
}

void judgeClimate(double* temps, double* rains, char* char3) {
    ////// Preparations
    // get # of months warmer than 10
    int warm_months = 0;
    for (int i = 0; i < 12; i++)
        if (temps[i] > 10) warm_months++;
    
    // get rainfall of the year
    double rain_year = 0.0;
    for (int i = 0; i < 12; i++)
        rain_year += rains[i];
    
    // get average temperature
    double temp_ave = 0.0;
    for (int i = 0; i < 12; i++)
        temp_ave += temps[i];
    temp_ave /= 12;
    
    // get the coldest month and the temperature
    int month_coldest = 0;
    for (int i = 0; i < 12; i++)
        if (temps[i] < temps[month_coldest]) month_coldest = i;
    double temp_min = temps[month_coldest];
    
    // get the warmest month and the temperature
    int month_warmest = 0;
    for (int i = 0; i < 12; i++)
        if (temps[i] > temps[month_warmest]) month_warmest = i;
    double temp_max = temps[month_warmest];
    
    // get the driest month and the rainfall
    int month_driest = 0;
    for (int i = 0; i < 12; i++)
        if (rains[i] < rains[month_driest]) month_driest = i;
    double rain_min = rains[month_driest];
    
    // get the wettest month and the rainfall
    int month_wettest = 0;
    for (int i = 0; i < 12; i++)
        if (rains[i] > rains[month_wettest]) month_wettest = i;
    double rain_max = rains[month_wettest];
    
    
    ////// Actual operations
    memset(char3, '\0', 4);
    
    //// Judge E(The Frigid Zone)
    if (temp_max < 10.0) {
        //// E(The Frigid Zone)
        char3[0] = 'E';
        if (temp_max < 0.0) char3[1] = 'F'; // EF(froste climate)
        else char3[1] = 'T'; // ET(tundra climate)
        return;
    }
    
    //// check if it's the Northern Hemisphere
    bool isNorthern;
    if ((temps[6] + temps[7]) > (temps[0] + temps[1])) isNorthern = true; // the Northern Hemisphere
    else isNorthern = false; // the Southern Hemisphere
    
    //// get the dry season(DRY_WINTER, DRY_SUMMER, DRY_NONE)
    int dry_season = DRY_NONE; // default : none
#ifdef FIX_DRY
    if (month_wettest <= 8 && month_wettest >= 5 && (rain_min*10 < rain_max)) 
        dry_season = DRY_WINTER; // 6 - 9 is the wettest and min*10<max -> dry in winter
    else if ((month_driest <= 2 || month_driest == 11) && (rain_min*10 < rain_max))
        dry_season = DRY_WINTER; // 12 - 3 is the driest and min*10<max -> dry in winter
    
    if ((month_wettest <= 2 || month_wettest == 11) && (rain_min*3 < rain_max)) 
        dry_season = DRY_SUMMER; // 12 - 3 is the wettest and min*3<max -> dry in summer
    else if (month_driest <= 8 && month_driest >= 5 && (rain_min*3 < rain_max))
        dry_season = DRY_SUMMER; // 6 - 9 is the driest and min*3<max -> dry in summer
#else
    if (month_wettest <= 8 && month_wettest >= 5 && (rain_min*10 < rain_max)) 
        dry_season = DRY_WINTER; // 6 - 9 is the wettest and min*10<max -> dry in winter
    
    if ((month_wettest <= 2 || month_wettest == 11) && (rain_min*3 < rain_max)) 
        dry_season = DRY_SUMMER; // 12 - 3 is the wettest and min*3<max -> dry in summer
#endif
    // turn season if it's southern
    if (!isNorthern && dry_season == DRY_SUMMER) dry_season = DRY_WINTER;
    else if (!isNorthern && dry_season == DRY_WINTER) dry_season = DRY_SUMMER;
    
    //// get the aridity boundary
    double arid_bound;
    if (dry_season == DRY_SUMMER) arid_bound = 20*temp_ave;
    else if (dry_season == DRY_NONE) arid_bound = 20*(temp_ave+7);
    else if (dry_season == DRY_WINTER) arid_bound = 20*(temp_ave+14);
    
    //// Judge B(The Dry Zone)
    if (rain_year < arid_bound) {
        //// B(The Dry Zone)
        char3[0] = 'B';
        if (rain_year < (arid_bound/2)) char3[1] = 'W'; // BW(Desert Climate)
        else char3[1] = 'S'; // BS(Steppe Climate)
        
        if (temp_ave < 18.0) char3[2] = 'k';
        else char3[2] = 'h';
        
        return;
    }
    
    //// Judge A(The Torrid Zone)
    if (temp_min > 18.0) {
        //// A(The Torrid Zone)
        char3[0] = 'A';
      if (rain_min > 60) char3[1] = 'f'; // Af(Torrid Feucht Climate)
      else {
         if (rain_min < (100 - rain_year*0.04)) {
            if (dry_season == DRY_SUMMER && rain_min < 30.0) char3[2] = 's'; // As(Torrid Sommertrocken Climate)
            else char3[2] = 'w'; // Aw(Savannah Climate)
         } else char3[2] = 'm'; // Am(Torrid Monsoon Climate)
      }
      return;
    }
    
    //// D&C(The Subfrigid Zone & The Temprate Zone)
    if (temp_min < -3.0) char3[0] = 'D'; //// D(The Subfrigid Zone)
    else char3[0] = 'C'; //// C(The Temprate Zone)
    // get the second char
    if (dry_season == DRY_WINTER) char3[1] = 'w';
    else if (dry_season == DRY_SUMMER && rain_min < 30.0) char3[1] = 's';
    else char3[1] = 'f';
    // get char to be the third
    if (temp_max > 22.0) char3[2] = 'a';
    else if (warm_months >= 4) char3[2] = 'b';
    else if (temp_min > -38.0) char3[2] = 'c';
    else char3[2] = 'd';
    
    return;
}

#ifdef JP
char* getClimateStringBy3Char(const char* c3) {
    if (c3[0] == 'A') {
        if (c3[1] == 'f') return "熱帯雨林気候";
        else if (c3[1] == 's') return "熱帯夏季少雨気候";
        else if (c3[1] == 'w') return "サバナ気候";
        else if (c3[1] == 'm') return "熱帯モンスーン気候";
        else return NULL;
    } else if (c3[0] == 'B') {
        if (c3[1] == 'W') return "砂漠気候";
        else if (c3[1] == 'S') return "ステップ気候";
        else return NULL;
    } else if (c3[0] == 'C') {
        if (c3[1] == 'w') return "温暖冬季少雨気候";
        else if (c3[1] == 's') return "地中海性気候";
        else if (c3[1] == 'f') {
            if (c3[2] == 'a') return "温暖湿潤気候";
            else return "西岸海洋性気候";
        }
        else return NULL;
    } else if (c3[0] == 'D') {
        if (c3[1] == 'w') return "亜寒帯冬季少雨気候";
        else if (c3[1] == 's') return "高地地中海性気候";
        else if (c3[1] == 'f') return "亜寒帯湿潤気候";
        else return NULL;
    } else if (c3[1] == 'E') {
        if (c3[1] == 'F') return "氷雪気候";
        else if (c3[1] == 'T') return "ツンドラ気候";
        else return NULL;
    } else return NULL;
}
#else
char* getClimateStringBy3Char(const char* c3) {
    if (c3[0] == 'A') {
        if (c3[1] == 'f') return "Torrid Feucht Climate";
        else if (c3[1] == 's') return "Torrid Sommertrocken Climate";
        else if (c3[1] == 'w') return "Savannah Climate";
        else if (c3[1] == 'm') return "Torrid Monsoon Climate";
        else return NULL;
    } else if (c3[0] == 'B') {
        if (c3[1] == 'W') return "Desert Climate";
        else if (c3[1] == 'S') return "Steppe Climate";
        else return NULL;
    } else if (c3[0] == 'C') {
        if (c3[1] == 'w') return "Temprate Wintertrocken Climate";
        else if (c3[1] == 's') return "Temprate Sommertrocken Climate";
        else if (c3[1] == 'f') {
            if (c3[2] == 'a') return "West Coast Marine Climate";
            else return "Temprate Feucht Climate";
        }
        else return NULL;
    } else if (c3[0] == 'D') {
        if (c3[1] == 'w') return "Subfrigid Wintertrocken Climate";
        else if (c3[1] == 's') return "Subfrigid Sommertrocken Climate";
        else if (c3[1] == 'f') return "Subfrigid Feucht Climate";
        else return NULL;
    } else if (c3[1] == 'E') {
        if (c3[1] == 'F') return "Froste Climate";
        else if (c3[1] == 'T') return "Tundra Climate";
        else return NULL;
    } else return NULL;
}
#endif

void onCtrlC(int i) {
    CtrlCPressed = true;
    if (signal(SIGINT, onCtrlC) == SIG_ERR) exit(3); // set Ctrl+C signal handler
}
