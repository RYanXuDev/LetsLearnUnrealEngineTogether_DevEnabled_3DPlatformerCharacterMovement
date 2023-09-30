#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;

UCLASS()
class DEVENABLED3DPLATFORM_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

public:
	FORCEINLINE void SetIsJumping(const bool InValue) { IsJumping = InValue; }

private:
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	bool IsInAir;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
	void UpdateParameters();
};