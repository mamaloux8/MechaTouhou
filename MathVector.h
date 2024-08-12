#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

class MathVector
{
public:
	MathVector();
	~MathVector();
	MathVector(int angle, float length);
	MathVector(float dx, float dy);

	MathVector operator+(const MathVector& v) const
	{
		return { m_dx + v.m_dx, m_dy + v.m_dy };
	}

	void add(MathVector&);
	void scale(float);

	float getX() const{ return m_dx; };
	float getY() const { return m_dy; };
	float getAngle() const { return m_angle; };
	float getLength() const { return m_length; };

	void setX(float dx) { m_dx = dx; };
	void setY(float dy) { m_dy = dy; };
	void setAngle(float angle);
	void setLength(float length) { m_length = length; };

private:
	float m_dx{};
	float m_dy{};
	float m_angle{};
	float m_length{};

	void updatePolar();
	void updateCartesian();
};

#endif
