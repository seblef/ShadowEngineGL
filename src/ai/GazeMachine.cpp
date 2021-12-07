
#include "GazeMachine.h"
#include "GazeTrack.h"
#include "GazeIdle.h"

GazeMachine::GazeMachine() : AIMachine(GAZE_MACHINE_ID)
{
	_states[GAZE_IDLE_STATE_ID] = new GazeIdle(this);
	_states[GAZE_TRACK_STATE_ID] = new GazeTrack(this);
}

unsigned int GazeMachine::getStateID(const string& name)
{
    if (name == "idle")
        return GAZE_IDLE_STATE_ID;
    else if (name == "track")
        return GAZE_TRACK_STATE_ID;
    else
        return 0xffffffff;
}
