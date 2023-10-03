#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;

UCLASS()
class DEVENABLED3DPLATFORM_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void NativeBeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsAirJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsWallSliding;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsSliding;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsWallJumping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsGrounded;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	void UpdateParameters();
};