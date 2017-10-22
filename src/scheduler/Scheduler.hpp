//
// Created by Fabrizio Guglielmino on 17/10/17.
//

#pragma once

#include "./Schedule.hpp"
#include "IScheduledTask.hpp"
#include <vector>

class ScheduleItem {

public:
    IScheduledTask *_what;
    Schedule *_when;
    ScheduleItem(IScheduledTask *what, Schedule *when) : _what(what), _when(when) {

    };

};

class Scheduler {
private:
    std::vector<ScheduleItem> _items;
public:
    void add(IScheduledTask *what, Schedule *when) {
        _items.push_back(ScheduleItem(what, when));
    }

    void loop(time_t *time) {
        std::vector<ScheduleItem>::const_iterator i;
        for(i=_items.begin(); i!=_items.end(); ++i){
            if(i->_when->ShouldRun(time)) {
                i->_what->Execute(nullptr);
            }
        }
    }
};
