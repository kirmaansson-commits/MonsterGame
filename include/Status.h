#pragma once
#include <string>

enum class StatusType {
    Poisoned,
    Frozen,
    Paralyzed,
    Stunned,
    Cursed
};

class Status {
private:
    StatusType type;
    std::string name;
    int duration; 

public:
    Status(StatusType type);

    StatusType getType() const;
    std::string getName() const;
    int getDuration() const;
    void decrementDuration();
    bool isExpired() const;
};
