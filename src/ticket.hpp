#include "parking_interface.hpp"
#include <string>

class Ticket : public ITicket
{
public:
    Ticket() = default;

    Ticket(std::string id, CarInfo carInfo, TimePoint entryTime)
        : id_(move(id)), carInfo_(std::move(carInfo)), entryTime_(entryTime) {}

    const std::string &id() const override
    {
        return id_;
    }

    const CarInfo &getCarInfo() const
    {
        return carInfo_;
    }

    TimePoint getEntryTime() const
    {
        return entryTime_;
    }

private:
    std::string id_;
    CarInfo carInfo_;
    TimePoint entryTime_;
};
