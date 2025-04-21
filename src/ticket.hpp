#include "parking_interface.hpp"
#include <string>

class Ticket : public ITicket
{
public:
    Ticket() = default;

    Ticket(std::string id, CarInfo carInfo, TimePoint entryTime)
        : _id(move(id)), _carInfo(std::move(carInfo)), _entryTime(entryTime) {}

    const std::string &id() const override
    {
        return _id;
    }

    const CarInfo &getCarInfo() const
    {
        return _carInfo;
    }

    TimePoint getEntryTime() const
    {
        return _entryTime;
    }

private:
    std::string _id;
    CarInfo _carInfo;
    TimePoint _entryTime;
};
