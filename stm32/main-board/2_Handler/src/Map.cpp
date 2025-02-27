#include "Map.h"

Map* Map::GetInstance()
{
    static Map instance;
    return &instance;
}

void Map::Process()
{
    if (active == true)
    {
        prevTrackType   = actualTrackType;
        actualTrackType = trackDetect->GetTrackType();
        IsCarOverRoadsign();

        timeElapsed = clock->GetTime() - timeOrigo;

        actualPosition = navigation->GetPosition();
        actualDistance = encoder->GetDistance();

        switch (state)
        {
            case MapState::Drive:
            {
                decisionMade = false;

                if (overRoadsign == true)
                {
                    if ((IsCrosspoint() == true)                    ||
                        (actualTrackType == TrackType::Exit)        ||
                        (actualTrackType == TrackType::ExitReverse))
                    {
                        state = MapState::Decision;
                        trace->Transmit("__MAP: Decision", 15);
                    }
                    else
                    {
                        state = MapState::OverRoadSign;
                        trace->Transmit("__MAP: OverRoadSign", 19);
                    }
                }
                break;
            }
            case MapState::OverRoadSign:
            {
                if (IsCrosspoint() == true){        state = MapState::Decision;     trace->Transmit("__MAP: Decision", 15);     }
                else if (overRoadsign == false){    state = MapState::Drive;        trace->Transmit("__MAP: Drive",    12);     decisionMade = false; }
                else {}
                break;
            }
            case MapState::Decision:
            {
                DecideNextTurn();
                state = MapState::OverRoadSign;
                trace->Transmit("__MAP: OverRoadSign", 19);
                decisionMade = true;
                break;
            }
            default:
                break;
        }
    }
}

TurnType Map::WhichWayToTurn()
{
    return nextTurn;
}

bool Map::isDecisionMade()
{
    return decisionMade;
}

bool Map::shouldExitMaze()
{
    return shouldExit;
}

MapState Map::GetState()
{
    return state;
}


void Map::TurnOn()
{
    active = true;
}

void Map::TurnOff()
{
    active = false;
}


Map::Map()
{
    encoder                 = Encoder::GetInstance();
    trackDetect             = TrackDetector::GetInstance();
    clock                   = Timepiece::GetInstance();
    navigation              = Navigation::GetInstance();
    trace                   = StringQueue::GetInstance(MapModule);

    active                  = true;

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
    overRoadsign            = false;

    actualTrackType         = TrackType::Single;
    prevTrackType           = TrackType::Single;
    timeElapsed             = 0;
    timeOrigo               = clock->GetTime();
    actualPosition          = navigation->GetPosition();
    actualDistance          = encoder->GetDistance();

    magic                   = 0;
}

void Map::DecideNextTurn()
{
    if (RANDOM_STRATEGY_ON == true)
    {
        // TODO Labyrinth algorithm
        // Add new segments
        // Update already know segments
        // Position can be set here if the end of the segment was found previously
        // Set adjacent segments
        // Plan route or discover new segment
    }
    else
    {
        nextTurn = RollDiceOnTurn();

        if (timeElapsed > TIME_GIVE_UP)       // TODO for exit maneuver
        {
            shouldExit = true;
        }
    }
}

TurnType Map::RollDiceOnTurn()
{
    uint8_t  randomNumber;
    TurnType turn;

    turn = TurnType::NoTurn;

    randomNumber = ( ((int)(encoder->GetDistance() * 10000) + (clock->GetTime()) ) % 255);
    if (randomNumber <= 85)
    {
        turn = TurnType::Left;
        trace->Transmit("__MAP: Left", 11);
    }
    else if (randomNumber <= 170)
    {
        turn = TurnType::Right;
        trace->Transmit("__MAP: Right", 12);
    }
    else
    {
        turn = TurnType::NoTurn;
        trace->Transmit("__MAP: NoTurn", 13);
    }

    // Magic
    /*if (magic <= 15)
    {
        turn = TurnType::Right;
        trace->Transmit("__MAP: Force Right", 18);
        magic++;
    }*/

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

    if ((trackDetect->IsJunction(prevTrackType) == true)
        &&
        ((trackDetect->IsFork(actualTrackType) == true) || (actualTrackType == TrackType::Single) == true))
    {
        crosspointFound = true;
        trace->Transmit("__MAP: CrossPoint", 17);
    }

    if (prevTrackType == TrackType::Single
        &&
        (trackDetect->IsFork(actualTrackType) == true))
    {
        crosspointFound = true;
        trace->Transmit("__MAP: CrossPoint", 17);
    }

    // The crossing point is the middle of the junction. This is the position registered to the segment.
    return crosspointFound;
}
