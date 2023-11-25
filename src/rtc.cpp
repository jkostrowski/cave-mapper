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

int rtcFullLog(char* buff) {
    TRACE("rtcLog");
    DateTime now = rtc.now();
    return sprintf( buff, "%04d-%02d-%02d,%02d:%02d:%02d,", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );
}

char msgRtc2[30];
char* rtcTimeLog(void) {
    TRACE("rtcTime");
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

