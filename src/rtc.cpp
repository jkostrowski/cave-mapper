#include "rtc.h"

RTC_DS3231 rtc;

void rtcInitialize(void) {
    if (! rtc.begin(&Wire1)) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // reboot = rtcTimestamp();
}

char msgRtc1[30];
char* rtcLog(void) {
    DateTime now = rtc.now();
    sprintf( msgRtc1, "%04d-%02d-%02dT%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );
    return msgRtc1;
}

char msgRtc2[30];
char* rtcTime(void) {
    DateTime now = rtc.now();
    sprintf( msgRtc2, "%02d:%02d:%02d", now.hour(), now.minute(), now.second() );
    return msgRtc2;
}

void rtcSet(DateTime now) {
    return rtc.adjust(now);
}


DateTime rtcTimestamp(void) {
    return rtc.now();
}

