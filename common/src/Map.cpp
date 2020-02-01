#include "Map.h"

Map* Map::GetInstance()
{
    static Map instance;
    return &instance;
}

void Map::Process()
{
    actualTrackType = trackDetect->GetTrackType();
    IsCarOverRoadsign();
    prevTrackType = actualTrackType;

    timeElapsed = clock->GetTime() - timeOrigo;

    actualPosition = navigation->GetPosition();

    actualDistance = encoder->GetDistance();

    switch (state)
    {
        case MapState::Drive:
        {
            if (overRoadsign == true)
            {
                state = MapState::OverRoadSign;
                decisionMade = false;
            }
            break;
        }
        case MapState::OverRoadSign:
        {
            if (IsCrosspoint() == true)
            {
                state = MapState::Decision;
            }
            else if (overRoadsign == false)
            {
                state = MapState::Drive;
            }
            else {}
            break;
        }
        case MapState::Decision:
        {
            DecideNextTurn();
            decisionMade = true;

            state = MapState::OverRoadSign;
            break;
        }
        default:
            break;
    }
}

TurnType Map::WhichWayToTurn()
{
    return nextTurn;
}

Map::Map()
{
    segments[32]            = {0U};
    foundSegmentCount       = 0U;
    discovSegmentCount      = 0U;
    exitSegmentId           = INVALID_SEGMENT_ID;
    adjacents_AtoB[32][3]   = {INVALID_SEGMENT_ID};
    adjacents_BtoA[32][3]   = {INVALID_SEGMENT_ID};
    route[32]               = {TurnType::NotSet};
    direction               = CarDirection::AtoB;
    shouldExit              = false;

    state                   = MapState::Drive;
    remainingTime           = TIME_START;
    nextTurn                = TurnType::NoTurn;
    decisionMade            = false;
    overRoadsign    = false;

    actualTrackType = TrackType::Single;
    prevTrackType   = TrackType::Single;
    timeElapsed     = 0;
    timeOrigo       = clock->GetTime();
    actualPosition  = navigation->GetPosition();
    actualDistance  = encoder->GetDistance();

    navigation  = Navigation::GetInstance();
    encoder     = Encoder::GetInstance();
    trackDetect = TrackDetector::GetInstance();
    clock       = Timepiece::GetInstance();
}

void Map::DecideNextTurn()
{
    if (RANDOM_STRATEGY_ON == false)
    {

    }
    else
    {
        nextTurn = RollDiceOnTurn();

        if (timeElapsed > TIME_GIVE_UP)
        {
            shouldExit = true;

            switch (actualTrackType)
            {
                case TrackType::Exit:
                {
                    nextTurn = TurnType::ExitRight;
                    break;
                }
                case TrackType::ExitReverse:
                {
                    nextTurn = TurnType::ExitLeft;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

TurnType Map::RollDiceOnTurn()
{
    uint8_t  randomNumber;
    TurnType turn;

    turn = TurnType::NoTurn;

    randomNumber = navigation->GetRandomNumber() + (clock->GetTime() % 10);
    if (randomNumber <= 85)
    {
        turn = TurnType::Left;
    }
    else if (randomNumber <= 170)
    {
        turn = TurnType::Right;
    }
    else
    {
        turn = TurnType::NoTurn;
    }

    return turn;
}

void Map::IsCarOverRoadsign()
{
    if (actualTrackType == TrackType::Single)
    {
        overRoadsign = false;
    }
    else
    {
        if ((prevTrackType == TrackType::Single)
             && ((actualTrackType != TrackType::Single) /*|| (actualDistance != TrackType::None)*/))
        {
            overRoadsign = true;
        }
    }
}

bool Map::IsCrosspoint()
{
    bool crosspointFound = false;

    if (((prevTrackType == TrackType::JunctionBoth) ||
         (prevTrackType == TrackType::JunctionLeft) ||
         (prevTrackType == TrackType::JunctionRight))
        &&
        ((actualTrackType == TrackType::ForkBoth)  ||
         (actualTrackType == TrackType::ForkLeft)  ||
         (actualTrackType == TrackType::ForkRight) ||
         (actualTrackType == TrackType::Single)))
    {
        crosspointFound = true;
    }

    if (prevTrackType == TrackType::Single
        &&
        ((actualTrackType == TrackType::ForkBoth)  ||
         (actualTrackType == TrackType::ForkLeft)  ||
         (actualTrackType == TrackType::ForkRight)))
    {
        crosspointFound = true;
    }

    // The crossing point is the middle of the junction. This is the position registered to the segment.
    return crosspointFound;
}

