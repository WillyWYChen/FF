#pragma once

#include <string>
#include <set>
#include <vector>

#define COLOR_DELTA_LIGHT_BLUE	RGB(0,173,239)
#define COLOR_DELTA_DEEP_BLUE	RGB(0,135,220)
#define COLOR_DELTA_GREY		RGB(95,95,95)
#define COLOR_DELTA_GREEN		RGB(20,160,40)
#define COLOR_DELTA_ORANGE		RGB(240,130,0)


#define WM_MY_MSG (WM_USER+38)

#define WPARAM_UPDATE_GUI	(1)
#define WPARAM_TEXT_UP		(2)
#define WPARAM_TEXT_DOWN	(3)
#define WPARAM_TEXT_LEFT	(4)
#define WPARAM_TEXT_RIGHT	(5)

#define INIT_UPDATE_TIMER_ID		(5566)
#define DRAW_ANIMATION_TIMER_ID		(9527)

#define ANIMATION_BACKGROUND_COLOR	COLOR_DELTA_DEEP_BLUE
#define RESULT_BACKGROUND_COLOR		RGB(255,255,255)
#define ANIMATION_WAIT_MS			(3000)
#define TYPE2_SHOW_INTERVAL_MS		(1000)

#define RESULT_BACKGROUND			"res\\result.gif"
#define ANIMATION_BACKGROUND		"res\\background.gif"
#define TYPE1_DRAW_SOUND			"res\\show.wav"
#define TYPE2_DRAW_SOUND			"res\\show2.wav"

#define RESEULT_TEXT_FONT_SIZE		(1200)	// unit is 1/10 point
#define RESEULT_TEXT_FONT_COLOR		COLOR_DELTA_ORANGE
#define RESEULT_TEXT_FONT_NAME		"Britannic Bold"

#define RESEULT_TEXT_FONT2_SIZE		(850)	// unit is 1/10 point
#define RESEULT_TEXT_FONT2_COLOR	COLOR_DELTA_LIGHT_BLUE
#define RESEULT_TEXT_FONT2_NAME		"·L³n¥¿¶ÂÅé"

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
	int Paused;
	CString NextShowValue;
	CString NextShowName;
};

extern struct ShareData gShareData;

std::vector<CString> split(CString &str, const CString find, int limit = 0);