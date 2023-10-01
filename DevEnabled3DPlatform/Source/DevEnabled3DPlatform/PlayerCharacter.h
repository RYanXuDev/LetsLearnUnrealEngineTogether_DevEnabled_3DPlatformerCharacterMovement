#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPlayerCharacterAnimInstance;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DEVENABLED3DPLATFORM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool GetIsFalling();

	FORCEINLINE bool GetIsJumping() const { return IsJumping; }
	FORCEINLINE bool GetIsAirJumping() const { return IsAirJumping; }

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaskMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	int32 JumpCount;
	
	bool IsJumping;
	bool IsAirJumping;
	
	float DefaultMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta=(AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta=(AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier = 2.5f;
	
	UFUNCTION(BlueprintCallable)
	void MoveForward(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(const float InputValue);
	void AirJump();
	void GroundJump();

	UFUNCTION(BlueprintCallable)
	void DoJump();

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void ResetMoveSpeed();

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	
	FVector GetMovementDirection(const FVector& InVector) const;
};