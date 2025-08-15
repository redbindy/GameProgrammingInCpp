#include "SkeletalMeshComponent.h"

#include "Skeleton.h"
#include "Animation.h"
#include "Shader.h"
#include "Actor.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* const pActor)
	: MeshComponent(pActor, true)
	, mpSkeleton(nullptr)
	, mpAnimation(nullptr)
	, mAnimPlayRate(0.f)
	, mAnimTime(0.f)
	, mPalette()
{

}

void SkeletalMeshComponent::Draw(Shader* const pShader)
{
	if (mpMesh == nullptr)
	{
		return;
	}

	pShader->SetMatrixUniform("uWorldTransform", mpOwner->GetWorldTransform());
	pShader->SetMatrixUniforms("uMatrixPalette", &mPalette.entry[0], MAX_SKELETON_BONES);
	pShader->SetFloatUniform("uSpecPower", mpMesh->GetSpecPower());

	Texture* const pTexture = mpMesh->GetTexture(mTextureIndex);
	if (pTexture != nullptr)
	{
		pTexture->SetActive();
	}

	VertexArray* const pVA = mpMesh->GetVertexArray();
	pVA->SetActive();

	glDrawElements(GL_TRIANGLES, pVA->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void SkeletalMeshComponent::Update(const float deltaTime)
{
	if (mpAnimation != nullptr && mpSkeleton != nullptr)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		while (mAnimTime > mpAnimation->GetDuration())
		{
			mAnimTime -= mpAnimation->GetDuration();
		}

		computeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* pAnim, const float playRate)
{
	mpAnimation = pAnim;
	mAnimTime = 0.f;
	mAnimPlayRate = playRate;

	if (mpAnimation == nullptr)
	{
		return 0.f;
	}

	computeMatrixPalette();

	return mpAnimation->GetDuration();
}

void SkeletalMeshComponent::computeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mpSkeleton->GetGlobalInvBindPoses();

	std::vector<Matrix4> currentPoses;
	mpAnimation->GetGlobalPoseAtTime(currentPoses, mpSkeleton, mAnimTime);

	for (size_t i = 0; i < mpSkeleton->GetNumBones(); ++i)
	{
		mPalette.entry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
