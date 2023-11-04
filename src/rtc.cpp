#include "rtc.h"

RTC_DS3231 rtc;
char msgRtc[100];


void initializeRtc(void) {
    if (! rtc.begin(&Wire1)) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

char* getRtc(void) {
    DateTime now = rtc.now();
    sprintf( msgRtc, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()  );
    // lcd3( msgRtc );
    return msgRtc;
}

DateTime rtcTimestamp(void) {
    return rtc.now();
}

