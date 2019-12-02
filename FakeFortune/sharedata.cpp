#include "stdafx.h"
#include "sharedata.h"

struct ShareData gShareData;

std::vector<CString> split(CString &str, const CString find, int limit)
{
	std::vector<CString> ret;
	int start = 0;
	int pos = str.Find(find, start);
	int len = find.GetLength();
	int i = 0;
	while (true) {
		if (pos < 0 || (limit > 0 && i + 1 == limit)) { //NO FIND
			ret.push_back(str.Mid(start));
			break;
		}
		else {
			ret.push_back(str.Mid(start, pos - start));
			start = pos + len;
			pos = str.Find(find, start);
		}
		i++;
	}
	return ret;
}