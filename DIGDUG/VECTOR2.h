#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);	// 引数付きコンストラクタ

	~VECTOR2();
	int x;
	int y;

	// 代入演算子
	VECTOR2& operator = (const VECTOR2& vec);

	// 添え字演算子
	int& operator[](int i);

	// 比較演算子
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;

	// 単項演算子
	VECTOR2& operator += (const VECTOR2& vec);	// +=
	VECTOR2& operator -= (const VECTOR2& vec);	// -=
	VECTOR2& operator *= (int k);				// *=
	VECTOR2& operator /= (int k);				// /=


	VECTOR2 operator+() const;
	VECTOR2 operator-() const;

};

// ベクトルの演算
// 返り値の型 operator 演算子(引数１, ....);

// VECTOR2+VECTOR2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);
// VECTOR2-VECTOR2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
// VECTOR2*k && k*VECTOR2
VECTOR2 operator*(int k, const VECTOR2& v);
VECTOR2 operator*(const VECTOR2& v, int k);
// VECTOR2/k
VECTOR2 operator/(const VECTOR2& v, int k);

