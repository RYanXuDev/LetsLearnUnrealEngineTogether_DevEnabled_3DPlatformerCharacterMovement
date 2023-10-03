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

	bool GetIsFalling() const;
	bool GetIsGrounded() const;

	FORCEINLINE bool GetIsJumping() const { return IsJumping; }
	FORCEINLINE bool GetIsAirJumping() const { return IsAirJumping; }
	FORCEINLINE bool GetIsWallSliding() const { return IsWallSliding; }
	FORCEINLINE bool GetIsWallJumping() const { return IsWallJumping; }
	FORCEINLINE bool GetIsSliding() const { return IsSliding; }

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaskMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	FHitResult WallCheckHitResult;

	FCollisionShape WallCheckCapsule;

	FCollisionQueryParams QueryParams;

	FVector SlideDirection;
	
	int32 JumpCount;

	FName WallCheckTag = FName("Wall");
	
	bool IsJumping;
	bool IsAirJumping;
	bool IsWallSliding;
	bool IsWallJumping;
	bool IsSliding;
	bool IsHeadBlocked;
	bool HasZVelocityReset;
	
	float DefaultMovementSpeed;
	float DefaultGravityScale;

	UPROPERTY(EditAnywhere, Category = "Move", meta=(AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Move", meta=(AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Wall Slide", meta=(AllowPrivateAccess = "true"))
	float WallSlideGravityScale = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Wall Slide", meta=(AllowPrivateAccess = "true"))
	float WallSlideDeceleration = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Air Jump", meta=(AllowPrivateAccess = "true"))
	float AirJumpForce = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Slide", meta=(AllowPrivateAccess = "true"))
	float SlideDuration = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Wall Jump", meta=(AllowPrivateAccess = "true"))
	FVector WallJumpVelocity = FVector(600.0f, 0.0f, 1600.0f);

	UFUNCTION(BlueprintCallable)
	void MoveForward(const float InputValue);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(const float InputValue);
	
	void GroundJump();
	void AirJump();
	void WallJump();
	
	UFUNCTION(BlueprintCallable)
	void DoJump();

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Sprint();

	void WallSlide(const float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Slide();
	
	void StopSliding();

	UFUNCTION(BlueprintCallable)
	void ResetMoveSpeed();

	void ResetZVelocity(bool DoOnce = true);

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);

	void WallSlideCheck();

	bool IsHeadHitWall() const;
	
	FVector GetMovementDirection(const FVector& InVector) const;

	void InitializeProperties();
};