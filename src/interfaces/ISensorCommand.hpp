//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

class ISensorCommand {
public:
    virtual void exec() = 0;
    virtual ~ISensorCommand() = default;
};