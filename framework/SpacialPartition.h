#pragma once

#include <vector>
#include <array>
#include <map>


template<class T> 
class SpacialPartition {
public:
	SpacialPartition(float xSize, float ySize, float zSize);
	SpacialPartition(std::array<float, 3> cellSize);

	int getCellIndexAtXloc(float x);
	int getCellIndexAtYloc(float y);
	int getCellIndexAtZloc(float z);
	std::array<int, 3> getCellIndexAtLoc(float x, float y, float z);
	std::array<int, 3> getCellIndexAtLoc(std::array<float, 3> location);

public:
	void addItemByIndex(int x, int y, int z, const T& item);
	void addItemByIndex(std::array<int, 3> index, const T& item);
	void addItemByLocation(float x, float y, float z, const T& item);
	void addItemByLocation(std::array<float, 3> location, const T& item);

	std::vector<T>& getCellByIndex(int x, int y, int z);
	std::vector<T>& getCellByIndex(std::array<int, 3> index);
	std::vector<T>& getCellByLocation(float x, float y, float z);
	std::vector<T>& getCellByLocation(std::array<float, 3> location);

	std::vector<std::array<int, 3>> getCellsInAreaRect(float x, float y, float z, float deltaX, float deltaY, float deltaZ);
	std::vector<std::array<int, 3>> getCellsInAreaCenteredRect(float x, float y, float z, float deltaX, float deltaY, float deltaZ);
	std::vector<std::array<int, 3>> getCellsInAreaSphere(float x, float y, float z, float radius);
	// std::vector<T*> getCellsInAreaFrustum(); Implement this later**** Look at https://pastebin.com/PEeDKLET

	std::map<std::array<int, 3>, std::vector<T>>& getCellMap();

public:
	void setCellXSize(float _xSize);
	void setCellYSize(float _ySize);
	void setCellZSize(float _zSize);
	void setCellSize(float _xSize, float _ySize, float _zSize);
	void setCellSize(std::array<float, 3> size);

public:
	void clear();

private:
	float cellXSize, cellYSize, cellZSize;
	std::map<std::array<int, 3>, std::vector<T>> cells;
};





template<class T>
inline SpacialPartition<T>::SpacialPartition(float xSize, float ySize, float zSize) :
	cellXSize(xSize),
	cellYSize(ySize),
	cellZSize(zSize),
	cells(std::map<std::array<int, 3>, std::vector<T>>())
{

}

template<class T>
inline SpacialPartition<T>::SpacialPartition(std::array<float, 3> cellSize) :
	cellXSize(cellSize[0]),
	cellYSize(cellSize[1]),
	cellZSize(cellSize[2]),
	cells(std::map<std::array<int, 3>, std::vector<T>>())
{

}


template<class T>
inline int SpacialPartition<T>::getCellIndexAtXloc(float x) {
	return (int)floor(x / cellXSize);
}

template<class T>
inline int SpacialPartition<T>::getCellIndexAtYloc(float y) {
	return (int)floor(y / cellYSize);
}

template<class T>
inline int SpacialPartition<T>::getCellIndexAtZloc(float z) {
	return (int)floor(z / cellZSize);
}

template<class T>
inline std::array<int, 3> SpacialPartition<T>::getCellIndexAtLoc(float x, float y, float z) {
	return std::array<int, 3>({ getCellIndexAtXloc(x), getCellIndexAtYloc(y), getCellIndexAtYloc(y) });
}

template<class T>
inline std::array<int, 3> SpacialPartition<T>::getCellIndexAtLoc(std::array<float, 3> location) {
	return getCellIndexAtLoc(location[0], location[1], location[2]);
}

template<class T>
inline void SpacialPartition<T>::addItemByIndex(int x, int y, int z, const T& item) {
	addItemByIndex({ x, y, z }, item);
}

template<class T>
inline void SpacialPartition<T>::addItemByIndex(std::array<int, 3> index, const T& item) {
	cells[index].push_back(item);
}

template<class T>
inline void SpacialPartition<T>::addItemByLocation(float x, float y, float z, const T& item) {
	addItemByLocation({ x, y, z }, item);
}

template<class T>
inline void SpacialPartition<T>::addItemByLocation(std::array<float, 3> location, const T& item) {
	addItemByIndex(getCellIndexAtLoc(location), item);
}

template<class T>
inline std::vector<T>& SpacialPartition<T>::getCellByIndex(int x, int y, int z) {
	return getCellByIndex({ x, y, z });
}

template<class T>
inline std::vector<T>& SpacialPartition<T>::getCellByIndex(std::array<int, 3> index) {
	return cells[index];
}

template<class T>
inline std::vector<T>& SpacialPartition<T>::getCellByLocation(float x, float y, float z) {
	return getCellByLocation({ x, y, z });
}

template<class T>
inline std::vector<T>& SpacialPartition<T>::getCellByLocation(std::array<float, 3> location) {
	return cells[getCellIndexAtLoc(location)];
}

template<class T>
inline std::vector<std::array<int, 3>> SpacialPartition<T>::getCellsInAreaRect(float x, float y, float z, float deltaX, float deltaY, float deltaZ) {
	std::vector<std::array<int, 3>> cellsInArea;
	std::array<int, 3> index = getCellIndexAtLoc(x, y, z);

	for (int x = 0; x < (int)(ceil(deltaX / cellXSize)); x++) 
		for (int y = 0; y < (int)(ceil(deltaY / cellYSize)); y++) 
			for (int z = 0; z < (int)(ceil(deltaZ / cellZSize)); z++) 
				cellsInArea.push_back({ index[0] + x, index[1] + y, index[2] + z });

	return cellsInArea;
}

template<class T>
inline std::vector<std::array<int, 3>> SpacialPartition<T>::getCellsInAreaCenteredRect(float x, float y, float z, float deltaX, float deltaY, float deltaZ) {
	return getCellsInAreaRect(x - deltaX/2, y - deltaY/2, z - deltaZ/2, deltaX, deltaY, deltaZ);
}

template<class T>
inline std::vector<std::array<int, 3>> SpacialPartition<T>::getCellsInAreaSphere(float x, float y, float z, float radius) {
	
	std::vector<std::_Array_iterator<float, 3>> cellsInArea = getCellsInAreaCenteredRect(x, y, z, radius*2, radius*2, radius*2);
	for (int i = 0; i < cellsInArea.size(); i++) {
		float x = cellsInArea[i][0] * cellXSize;
		float y = cellsInArea[i][1] * cellYSize;
		float z = cellsInArea[i][3] * cellZSize;

		if (x*x + y*y + z*z >= radius * radius) {
			cellsInArea.erase(cellsInArea.begin() + i);
			i--;
		}
	}

	return cellsInArea;
}

template<class T>
inline std::map<std::array<int, 3>, std::vector<T>>& SpacialPartition<T>::getCellMap() {
	return cells;
}

template<class T>
inline void SpacialPartition<T>::setCellXSize(float _xSize) {
	cellXSize = _xSize;
}

template<class T>
inline void SpacialPartition<T>::setCellYSize(float _ySize) {
	cellYSize = _ySize;
}

template<class T>
inline void SpacialPartition<T>::setCellZSize(float _zSize) {
	cellZSize = _zSize;
}

template<class T>
inline void SpacialPartition<T>::setCellSize(float xSize, float ySize, float zSize) {
	setCellXSize(xSize);
	setCellYSize(ySize);
	setCellZSize(zSize);
}

template<class T>
inline void SpacialPartition<T>::setCellSize(std::array<float, 3> size) {
	setCellSize(size[0], size[1], size[2]);
}

template<class T>
inline void SpacialPartition<T>::clear() {
	cells.clear();
}
