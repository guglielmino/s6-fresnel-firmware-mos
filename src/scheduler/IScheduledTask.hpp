//
// Created by Fabrizio Guglielmino on 17/10/17.
//

class IScheduledTask {
public:
    virtual bool Execute(void *param) = 0;
};