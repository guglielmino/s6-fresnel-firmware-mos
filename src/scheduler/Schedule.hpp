//
// Created by Fabrizio Guglielmino on 17/10/17.
//

#pragma once

#include <ctime>

class Schedule {
public:
    typedef enum {
        None, Mon, Tue, Wed, Thu, Fri, Sat, Sun
    } DaysOfWeek;
private:
    int8_t _hour;
    int8_t _min;

    DaysOfWeek _dayOfWeek;

    int8_t _month;
    int8_t _year;

    bool checkHour(struct tm *ptm, int8_t hour, int8_t min) {
        return (hour == ptm->tm_hour && min == ptm->tm_min);
    }

    bool checkDayOfWeek(struct tm *ptm, DaysOfWeek dayOfWeek) {
        return (_hour == ptm->tm_hour && _min == ptm->tm_min);
    }

public:

    Schedule(int8_t hour, int8_t min) : _hour(hour), _min(min), _dayOfWeek(None), _month(0), _year(0) {

    }

    Schedule(int8_t hour, int8_t min, DaysOfWeek dayOfWeek) : _hour(hour), _min(min), _dayOfWeek(dayOfWeek), _month(0),
                                                              _year(0) {

    }

    Schedule(int8_t hour, int8_t min, int8_t month, int8_t year) : _hour(hour), _min(min), _dayOfWeek(None),
                                                                   _month(month), _year(year) {

    }

    bool ShouldRun(time_t *time) {
        bool ret = false;

        struct tm *ptm;
        ptm = gmtime(time);
        return checkHour(ptm, _hour, _min);
    }
};
