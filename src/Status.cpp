#include "Status.h"
#include <cstdlib>

Status::Status(StatusType type) : type(type), duration(1) {
    switch (type) {
        case StatusType::Poisoned:
            name = "Poisoned";
            duration = 3;
            break;
        case StatusType::Frozen:
            name = "Frozen";
            duration = 1 + (rand() % 2); // 1-2 turns
            break;
        case StatusType::Paralyzed:
            name = "Paralyzed";
            duration = 3;
            break;
        case StatusType::Stunned:
            name = "Stunned";
            duration = 1;
            break;
        case StatusType::Cursed:
            name = "Cursed";
            duration = 3;
            break;
    }
}

StatusType Status::getType() const { return type; }
std::string Status::getName() const { return name; }
int Status::getDuration() const { return duration; }
void Status::decrementDuration() { if (duration > 0) duration--; }
bool Status::isExpired() const { return duration <= 0; }
