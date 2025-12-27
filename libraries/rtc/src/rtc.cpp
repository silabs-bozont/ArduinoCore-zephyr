#include "rtc.h"

#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/sys/timeutil.h>

RTCClass::RTCClass() {
	;
}

bool RTCClass::begin() {
#if DT_HAS_CHOSEN(zephyr_rtc)
	const struct device *dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_rtc));
#elif DT_NODE_HAS_STATUS(DT_NODELABEL(rtc0), okay)
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(rtc0));
#else
#error "No RTC device found in devicetree"
#endif

	if (!device_is_ready(dev)) {
		return false;
	}
	this->rtc_device = dev;
    this->setUnixTime(1732359600); // 2024.11.23. 12:00:00 UTC
	return true;
}

void RTCClass::end() {
	this->rtc_device = nullptr;
}

// Expects years in full (e.g. '2025')
// Expects actual month number - (e.g. '1' for January, '2' for February)
bool RTCClass::set(uint16_t year, uint8_t month, uint8_t day, uint8_t day_of_week,
						 uint8_t hour, uint8_t minute, uint8_t second) {

	if (!this->rtc_device) {
		return false;
	}
	struct rtc_time r;

	r.tm_sec = second;
	r.tm_min = minute;
	r.tm_hour = hour;
	r.tm_mday = day;
	r.tm_mon = month - 1;
	r.tm_year = year - 1900;
	r.tm_wday = day_of_week;

	if (rtc_set_time(this->rtc_device, &r) != 0) {
		return false;
	}
	return true;
}

int64_t RTCClass::getUnixTime() {
    if (!this->rtc_device) {
		return -1;
	}
	struct rtc_time t;
	struct tm tm;
	int ret;

	ret = rtc_get_time(this->rtc_device, &t);
	if (ret) {
		return -1;
	}

	tm.tm_sec = t.tm_sec;
	tm.tm_min = t.tm_min;
	tm.tm_hour = t.tm_hour;
	tm.tm_mday = t.tm_mday;
	tm.tm_mon = t.tm_mon;
	tm.tm_year = t.tm_year;
	tm.tm_wday = t.tm_wday;
	tm.tm_yday = t.tm_yday;
	tm.tm_isdst = t.tm_isdst;

	return timeutil_timegm64(&tm); // UTC -> POSIX seconds
}

bool RTCClass::setUnixTime(int64_t unix_time) {
    if (!this->rtc_device) {
		return false;
	}
	time_t tsec = (time_t)unix_time;
	struct tm tm;
	struct rtc_time r;

	if (gmtime_r(&tsec, &tm) == NULL) {
		return false;
	}

	r.tm_sec = tm.tm_sec;
	r.tm_min = tm.tm_min;
	r.tm_hour = tm.tm_hour;
	r.tm_mday = tm.tm_mday;
	r.tm_mon = tm.tm_mon;
	r.tm_year = tm.tm_year;
	r.tm_wday = tm.tm_wday;
	r.tm_yday = tm.tm_yday;
	r.tm_isdst = tm.tm_isdst;

	if (rtc_set_time(this->rtc_device, &r) != 0) {
		return false;
	}
	return true;
}

uint8_t RTCClass::getSecond() {
    if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_sec;
}

uint8_t RTCClass::getMinute() {
	if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_min;
}

uint8_t RTCClass::getHour() {
	if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_hour;
}

uint8_t RTCClass::getDay() {
	if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_mday;
}

String RTCClass::getDayName() {
	char dayname[20];
	if (!this->rtc_device) {
		return "";
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return "";
    }
    struct tm ts = {};
	ts.tm_year = t.tm_year;
	ts.tm_mon = t.tm_mon;
	ts.tm_mday = t.tm_mday;
	mktime(&ts);
	strftime(dayname, sizeof(dayname), "%A", &ts);
	return String(dayname);
}

uint8_t RTCClass::getMonth() {
	if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_mon + 1;
}

String RTCClass::getMonthName() {
	char monthname[20];
	if (!this->rtc_device) {
		return "";
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return "";
    }
	struct tm ts = {};
	ts.tm_year = t.tm_year;
	ts.tm_mon = t.tm_mon;
	ts.tm_mday = t.tm_mday;
	mktime(&ts);
	strftime(monthname, sizeof(monthname), "%B", &ts);
	return String(monthname);
}

uint16_t RTCClass::getYear() {
	if (!this->rtc_device) {
		return 0;
	}
	struct rtc_time t;
    if (rtc_get_time(this->rtc_device, &t) != 0) {
        return 0;
    }
    return t.tm_year + 1900;
}

RTCClass RTC;
