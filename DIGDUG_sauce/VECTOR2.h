#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);	// �����t���R���X�g���N�^

	~VECTOR2();
	int x;
	int y;

	// ������Z�q
	VECTOR2& operator = (const VECTOR2& vec);

	// �Y�������Z�q
	int& operator[](int i);

	// ��r���Z�q
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;

	// �P�����Z�q
	VECTOR2& operator += (const VECTOR2& vec);	// +=
	VECTOR2& operator -= (const VECTOR2& vec);	// -=
	VECTOR2& operator *= (int k);				// *=
	VECTOR2& operator /= (int k);				// /=


	VECTOR2 operator+() const;
	VECTOR2 operator-() const;

};

// �x�N�g���̉��Z
// �Ԃ�l�̌^ operator ���Z�q(�����P, ....);

// VECTOR2+VECTOR2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);
// VECTOR2-VECTOR2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
// VECTOR2*k && k*VECTOR2
VECTOR2 operator*(int k, const VECTOR2& v);
VECTOR2 operator*(const VECTOR2& v, int k);
// VECTOR2/k
VECTOR2 operator/(const VECTOR2& v, int k);

