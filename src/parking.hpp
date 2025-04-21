#include "parking_interface.hpp"
#include "ticket.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

using namespace std::chrono;

class Parking : public IParking
{
private:
    std::size_t _inGatesCount;
    std::size_t _outGatesCount;
    std::size_t _parkingSpacesCount;
    std::size_t _usedSpaces = 0;
    std::size_t _ticketCounter = 0;

    struct TicketData
    {
        CarInfo _carInfo;
        TimePoint _entryTime;
    };

    std::unordered_map<std::string, TicketData> _activeTickets;
    std::unordered_set<std::string> _activeCarIds;

public:
    Parking(std::size_t inGates, std::size_t outGates, std::size_t spaces);

    virtual std::tuple<std::unique_ptr<ITicket>, std::string> tryToEnter(const CarInfo &_carInfo,
                                                                         std::size_t inGateIdx, TimePoint tp) override;

    virtual std::tuple<GoodbyeMessage, std::string /*error text*/> tryToExit(const CarInfo &_carInfo,
                                                                             std::size_t outGateIdx, TimePoint tp, ITicket *ticket) override;

    std::size_t getAvailableSpaces() const override;
};

extern "C"
{

    IParking *createParking(std::size_t inGatesCount, std::size_t outGatesCount, std::size_t parkingSpacesCount)
    {
        return new Parking(inGatesCount, outGatesCount, parkingSpacesCount);
    }

    void destroyParking(IParking *ptr)
    {
        delete ptr;
    }
}
