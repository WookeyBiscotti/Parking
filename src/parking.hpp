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
    std::size_t inGatesCount_;
    std::size_t outGatesCount_;
    std::size_t parkingSpacesCount_;
    std::size_t usedSpaces_ = 0;
    std::size_t ticketCounter_ = 0;

    struct TicketData
    {
        CarInfo carInfo;
        TimePoint entryTime;
    };

    std::unordered_map<std::string, TicketData> activeTickets_;
    std::unordered_set<std::string> activeCarIds_;

public:
    Parking(std::size_t inGates, std::size_t outGates, std::size_t spaces)
        : inGatesCount_(inGates), outGatesCount_(outGates), parkingSpacesCount_(spaces) {}

    virtual std::tuple<std::unique_ptr<ITicket>, std::string> tryToEnter(const CarInfo &carInfo,
                                                                            std::size_t inGateIdx, TimePoint tp) override
    {
        if (inGateIdx >= inGatesCount_)
        {
            //return unexpected("Invalid in gate index");
            return std::make_tuple(std::make_unique<Ticket>(), "Invalid in gate index");
        }

        if (usedSpaces_ >= parkingSpacesCount_)
        {
            // return unexpected("No available spaces");
            return std::make_tuple(std::make_unique<Ticket>(), "No available spaces");
        }

        if (activeCarIds_.count(carInfo.id) > 0)
        {
            // return unexpected("Car already parked");
            return std::make_tuple(std::make_unique<Ticket>(), "Car already parked");
        }

        std::string ticketId = std::to_string(ticketCounter_++);
        activeTickets_[ticketId] = {carInfo, tp};
        activeCarIds_.insert(carInfo.id);
        ++usedSpaces_;

        // return std::make_unique<Ticket>(ticketId, carInfo, tp);
        return std::make_tuple(std::make_unique<Ticket>(ticketId, carInfo, tp), "");
    }

    virtual std::tuple<GoodbyeMessage, std::string /*error text*/> tryToExit(const CarInfo &carInfo,
                                                                                std::size_t outGateIdx, TimePoint tp, ITicket *ticket) override
    {
        if (outGateIdx >= outGatesCount_)
        {
            // return unexpected("Invalid out gate index");
            return std::make_tuple(GoodbyeMessage{}, "Invalid out gate index");
        }

        if (!ticket)
        {
            // return unexpected("Invalid ticket");
            return std::make_tuple(GoodbyeMessage{}, "Invalid ticket");
        }

        const std::string &ticketId = ticket->id();
        auto it = activeTickets_.find(ticketId);
        if (it == activeTickets_.end())
        {
            // return unexpected("Invalid ticket");
            return std::make_tuple(GoodbyeMessage{}, "Invalid ticket");
        }

        const TicketData &ticketData = it->second;
        const CarInfo &storedInfo = ticketData.carInfo;

        if (storedInfo.id != carInfo.id || storedInfo.type != carInfo.type || storedInfo.ownerId != carInfo.ownerId)
        {
            // return unexpected("Car info does not match ticket");
            return std::make_tuple(GoodbyeMessage{}, "Car info does not match ticket");
        }

        activeCarIds_.erase(storedInfo.id);
        activeTickets_.erase(it);
        --usedSpaces_;

        return std::make_tuple(GoodbyeMessage{}, "");
    };

    std::size_t getAvailableSpaces() const override
    {
        return parkingSpacesCount_ - usedSpaces_;
    };
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