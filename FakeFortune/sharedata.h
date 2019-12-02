#pragma once

#include <string>
#include <set>
#include <vector>

#define WM_MY_MSG (WM_USER+38)

#define WPARAM_UPDATE_GUI	(1)

#define INIT_UPDATE_TIMER_ID		(5566)
#define DRAW_ANIMATION_TIMER_ID		(9527)

#define ANIMATION_BACKGROUND_COLOR	RGB(250,211,236)
#define ANIMATION_WAIT_MS			(3000)
#define TYPE2_SHOW_INTERVAL_MS		(1000)

#define RESULT_BACKGROUND			"res\\result.gif"
#define ANIMATION_BACKGROUND		"res\\background.gif"
#define TYPE1_DRAW_SOUND			"res\\show.wav"
#define TYPE2_DRAW_SOUND			"res\\show2.wav"

#define RESEULT_TEXT_FONT_SIZE		(600)	// unit is 1/10 point
#define RESEULT_TEXT_FONT_COLOR		RGB(255,0,0)
#define RESEULT_TEXT_FONT_NAME		"Britannic Bold"

class EmployeeRecord {
public:
	bool operator< (const EmployeeRecord &rhs) const
	{
		return ((this->id) < (rhs.id));
	};
public:
	int id;
	CString name;
};

class HistoryRecord {
public:
	CString time;
	EmployeeRecord emplyee;
};

struct ShareData {
	std::set<class EmployeeRecord > GlobalSet;
	std::vector<class HistoryRecord > HistoryRecord;

	int ShowType;
	int ShowCount;
	CString NextShowValue;
};

extern struct ShareData gShareData;

std::vector<CString> split(CString &str, const CString find, int limit = 0);