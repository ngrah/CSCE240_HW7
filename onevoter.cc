#include "onevoter.h"
/****************************************************************
* Implementation for the 'OneVoter' class.
*
* Author/copyright:  Duncan Buell. All rights reserved.
* Date: 6 October 2016
*
**/

static const string kTag = "ONEVOTER: ";

/****************************************************************
* Constructor.
**/
OneVoter::OneVoter() {
}
/****************************************************************
**/
OneVoter::OneVoter(int sequence, int arrival_seconds,
int duration_seconds) {
sequence_ = sequence;
time_arrival_seconds_ = arrival_seconds;
time_start_voting_seconds_ = 0;
time_vote_duration_seconds_ = duration_seconds;
which_station_ = -1;
}

/****************************************************************
**/
OneVoter::~OneVoter() {
}

/****************************************************************
* Accessors and mutators.
**/
/****************************************************************
**/
int OneVoter::GetTimeArrival() const {
return time_arrival_seconds_;
}

/****************************************************************
**/
int OneVoter::GetTimeWaiting() const {
return time_waiting_seconds_;
}

/****************************************************************
**/
int OneVoter::GetStationNumber() const {
return which_station_;
}

/****************************************************************
* General functions.
**/

/****************************************************************
**/
void OneVoter::AssignStation(int station_number,
int start_time_seconds) {
which_station_ = station_number;
time_start_voting_seconds_ = start_time_seconds;
time_done_voting_seconds_ = time_start_voting_seconds_
+ time_vote_duration_seconds_;
time_waiting_seconds_ = time_start_voting_seconds_
- time_arrival_seconds_;
}

/****************************************************************
**/
int OneVoter::GetTimeDoneVoting() const {
return time_start_voting_seconds_ + time_vote_duration_seconds_;
}

/****************************************************************
**/
int OneVoter::GetTimeInQ() const {
return time_start_voting_seconds_ - time_arrival_seconds_;
}

/****************************************************************
**/
string OneVoter::GetTOD(int time_in_seconds) const {
//  int offset_hours = 6;
int offset_hours = 0;
string s = "";
return this->ConvertTime(time_in_seconds + offset_hours*3600);
}

/****************************************************************
**/
string OneVoter::ConvertTime(int time_in_seconds) const {
int hours = 0;
int minutes = 0;
int seconds = 0;
string s = "";

hours = time_in_seconds / 3600;
minutes = (time_in_seconds - 3600*hours) / 60;
seconds = (time_in_seconds - 3600*hours - 60*minutes);

s += Utils::Format(time_in_seconds, 6);

if(hours < 0)
s += " 00";
else if(hours < 10)
s += " 0" + Utils::Format(hours, 1);
else
s += " " + Utils::Format(hours, 2);

if(minutes < 0)
s += ":00";
else if(minutes < 10)
s += ":0" + Utils::Format(minutes, 1);
else
s += ":" + Utils::Format(minutes, 2);

if(seconds < 0)
s += ":00";
else if(seconds < 10)
s += ":0" + Utils::Format(seconds, 1);
else
s += ":" + Utils::Format(seconds, 2);

return s;
} // string OneVoter::ConvertTime(int time_in_seconds) const

/****************************************************************
**/
string OneVoter::ToString() {
string s = kTag;

s += Utils::Format(sequence_, 7);
s += ": ";
s += Utils::Format(this->GetTOD(time_arrival_seconds_));
s += " ";
s += Utils::Format(this->GetTOD(time_start_voting_seconds_));
s += " ";
s += Utils::Format(this->ConvertTime(time_vote_duration_seconds_));
s += " ";
s += Utils::Format(this->GetTOD(time_start_voting_seconds_ + time_vote_duration_seconds_));
s += " ";
s += Utils::Format(this->ConvertTime(GetTimeInQ()));
s += ": ";
s += Utils::Format(which_station_, 4);

return s;
} // string OneVoter::toString()

/****************************************************************
* 
**/
string OneVoter::ToStringHeader() {
string s = kTag;
s += "    Seq        Arr           Start             Dur             End            Wait         Stn";
return s;
}

