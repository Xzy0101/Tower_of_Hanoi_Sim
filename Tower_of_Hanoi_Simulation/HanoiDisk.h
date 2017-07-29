#pragma once

class HanoiDisk {
public:
	HanoiDisk(int w)
		: weight{ w }
	{}
	int getWeight() const { return weight; }
	bool movable(const HanoiDisk &d) const { return weight < d.getWeight(); }
private:
	int weight;
};