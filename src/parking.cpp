#include "parking.hpp"

Parking::Parking(std::size_t inGates, std::size_t outGates, std::size_t spaces)
    : _inGatesCount(inGates), _outGatesCount(outGates), _parkingSpacesCount(spaces) {}

std::tuple<std::unique_ptr<ITicket>, std::string> Parking::tryToEnter(const CarInfo &_carInfo,
                                                                      std::size_t inGateIdx, TimePoint tp)
{
    if (inGateIdx >= _inGatesCount)
    {
        // return std::unexpected("Invalid in gate index");
        return std::make_tuple(std::make_unique<Ticket>(), "Invalid in gate index");
    }

    if (_usedSpaces >= _parkingSpacesCount)
    {
        // return std::unexpected("No available spaces");
        return std::make_tuple(std::make_unique<Ticket>(), "No available spaces");
    }

    if (_activeCarIds.count(_carInfo.id) > 0)
    {
        // return std::unexpected("Car already parked");
        return std::make_tuple(std::make_unique<Ticket>(), "Car already parked");
    }

    std::string ticketId = std::to_string(_ticketCounter++);
    _activeTickets[ticketId] = {_carInfo, tp};
    _activeCarIds.insert(_carInfo.id);
    ++_usedSpaces;

    // return std::make_unique<Ticket>(ticketId, _carInfo, tp);
    return std::make_tuple(std::make_unique<Ticket>(ticketId, _carInfo, tp), "");
}

std::tuple<GoodbyeMessage, std::string /*error text*/> Parking::tryToExit(const CarInfo &_carInfo,
                                                                          std::size_t outGateIdx, TimePoint tp, ITicket *ticket)
{
    if (outGateIdx >= _outGatesCount)
    {
        // return std::unexpected("Invalid out gate index");
        return std::make_tuple(GoodbyeMessage{}, "Invalid out gate index");
    }

    if (!ticket)
    {
        // return std::unexpected("Invalid ticket");
        return std::make_tuple(GoodbyeMessage{}, "Invalid ticket");
    }

    const std::string &ticketId = ticket->id();
    auto it = _activeTickets.find(ticketId);
    if (it == _activeTickets.end())
    {
        // return std::unexpected("Invalid ticket");
        return std::make_tuple(GoodbyeMessage{}, "Invalid ticket");
    }

    const TicketData &ticketData = it->second;
    const CarInfo &storedInfo = ticketData._carInfo;

    if (storedInfo.id != _carInfo.id || storedInfo.type != _carInfo.type || storedInfo.ownerId != _carInfo.ownerId)
    {
        // return std::unexpected("Car info does not match ticket");
        return std::make_tuple(GoodbyeMessage{}, "Car info does not match ticket");
    }

    _activeCarIds.erase(storedInfo.id);
    _activeTickets.erase(it);
    --_usedSpaces;

    return std::make_tuple(GoodbyeMessage{}, "");
};

std::size_t Parking::getAvailableSpaces() const
{
    return _parkingSpacesCount - _usedSpaces;
};