#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const
{
	const Matrix4 rot = Matrix4::CreateFromQuaternion(rotation);
	const Matrix4 trans = Matrix4::CreateTranslation(translation);

	return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, const float f)
{
	BoneTransform ret;
	ret.rotation = Quaternion::Slerp(a.rotation, b.rotation, f);
	ret.translation = Vector3::Lerp(a.translation, b.translation, f);

	return ret;
}
