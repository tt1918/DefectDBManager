#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#pragma warning (disable: 4995)
#include <vector>
#pragma warning (default: 4995)
using namespace std;

// ㅏ형 2차원 배열
template <class T>
class _C2DVector
{
private:
	vector< vector <T> > _m_2DVector;
	unsigned int _m_dimRow;
	unsigned int _m_dimCol;

public:
	_C2DVector() : _m_dimRow(0), _m_dimCol(0)
	{}

	_C2DVector(unsigned int uRow, unsigned int uCol)
	{
		_m_dimRow = uRow;
		_m_dimCol = uCol;
		for (unsigned int i=0; i<uRow; i++)
		{
			vector<T> x(uCol);
			_m_2DVector.push_back(x);
		}
	}

	void _SetAt(unsigned int uRow, unsigned int uCol, const T& value)
	{
		if (uRow >= _m_dimRow || uCol >= _m_dimCol)
			throw out_of_range("Array out of bound");
		else
			_m_2DVector[uRow][uCol] = value;
	}

	T& _GetAt(unsigned int uRow, unsigned int uCol)
	{
		if (uRow >= _m_dimRow || uCol >= _m_dimCol)
			throw out_of_range("Array out of bound");
		else
			return _m_2DVector[uRow][uCol];
	}

	void _GrowRow(unsigned int uNewSize)
	{
		if (uNewSize <= _m_dimRow)
			return;
		for (unsigned int i=0; i<uNewSize-_m_dimRow; i++)
		{
			vector<T> x(_m_dimCol);
			_m_2DVector.push_back(x);
		}
		_m_dimRow = uNewSize;
	}

	void _GrowCol(unsigned int uNewSize)
	{
		if (uNewSize <= _m_dimCol)
			return;
		for (unsigned int i=0; i<_m_dimRow; i++)
			_m_2DVector[i].resize(uNewSize);
		_m_dimCol = uNewSize;
	}

	void _InsertRow(unsigned int uRow, const T& value)
	{
		if (uRow > _m_dimRow)
			throw out_of_range("Row out of bound");
		vector<T> x(_m_dimCol);
		_m_2DVector.insert(_m_2DVector.begin()+uRow, x);
		for (unsigned int i=0; i<_m_dimCol; i++)
			_m_2DVector[uRow][i] = value;
		_m_dimRow++;
	}

	void _InsertCol(unsigned int uCol, const T& value)
	{
		if (uCol > _m_dimCol)
			throw out_of_range("Col out of bound");
		for (unsigned int i=0; i<_m_dimRow; i++)
			_m_2DVector[i].insert(_m_2DVector[i].begin()+uCol, value);
		_m_dimCol++;
	}

	void _EraseRow(unsigned int uRow)
	{
		if (uRow >= _m_dimRow)
			throw out_of_range("Row out of bound");
		_m_2DVector[uRow].clear();
		_m_2DVector.erase(_m_2DVector.begin()+uRow);
		_m_dimRow--;
	}

	void _EraseCol(unsigned int uCol)
	{
		if (uCol >= _m_dimCol)
			throw out_of_range("Col out of bound");
		for (unsigned int i=0; i<_m_dimRow; i++)
			_m_2DVector[i].erase(_m_2DVector[i].begin()+uCol);
		_m_dimCol--;
	}

	void _Clear()
	{
		for (unsigned int i=0; i<_m_dimRow; i++)
			_m_2DVector[i].clear();
		_m_2DVector.clear();
		_m_dimRow = 0;
		_m_dimCol = 0;
	}

	void _Swap(unsigned int uRow1, unsigned int uCol1,
					unsigned int uRow2, unsigned int uCol2)
	{
		if (uRow1 >= _m_dimRow || uCol1 >= _m_dimCol
			|| uRow2 >= _m_dimRow || uCol2 >= _m_dimCol)
			throw out_of_range("Array out of bound");
		else
		{
			T T1 = _GetAt(uRow1, uCol1);
			T T2 = _GetAt(uRow2, uCol2);
			_SetAt(uRow1, uCol1, T2);
			_SetAt(uRow2, uCol2, T1);
		}
	}

	// _SetAt, _GetAt을 사용하지 않고도 외부에 선언된 변수에 [][]붙여 접근할 수 있다.
	// out_of_range 평가를 할 필요가 없이 빠른 속도가 필요할 때 사용.
	vector<T>& operator[](int x)
	{
		return _m_2DVector[x];
	}

	const unsigned int _GetRowSize() const
	{
		return _m_dimRow;
	}

	const unsigned int _GetColSize() const
	{
		return _m_dimCol;
	}
};
