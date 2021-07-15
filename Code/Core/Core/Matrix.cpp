#include "Core/Matrix.h"

const Matrix4x4 Matrix4x4::Identity(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f);

const Matrix4x4 Matrix4x4::Zero(
	 0.f, 0.f, 0.f, 0.f,
	 0.f, 0.f, 0.f, 0.f,
	 0.f, 0.f, 0.f, 0.f,
	 0.f, 0.f, 0.f, 0.f);