#pragma once

#include "axmol.h"
#include "RoundedRect.h"

class Peg : public RoundedRect
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Peg);

    unsigned char getTopDisk() const;
    void removeTopDisk();

    void addDisk(const unsigned char size) { _disks.push_back(size); }
    void clearDisks() { _disks.clear(); }
    int countDisks() const { return _disks.size(); }
    bool isEmpty() const { return _disks.empty(); }

private:
    std::deque<unsigned char> _disks; // Stack of disks on this peg
};
