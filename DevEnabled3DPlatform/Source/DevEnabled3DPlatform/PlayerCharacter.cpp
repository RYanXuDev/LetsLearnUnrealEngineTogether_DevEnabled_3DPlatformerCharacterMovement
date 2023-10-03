#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	JumpMaxHoldTime = 0.3f;
	JumpMaxCount = 2;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	MaskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask Mesh"));
	MaskMeshComponent->SetupAttachment(GetMesh(), FName("head_001Socket"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 20.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxAcceleration = 1500;
	GetCharacterMovement()->BrakingFrictionFactor = 0.1f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 1200.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->SetCrouchedHalfHeight(50.0f);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeProperties();
	
	LandedDelegate.AddDynamic(this, &APlayerCharacter::OnCharacterLanded);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAirJumping)
	{
		if (GetVelocity().Z < 0.0f)
		{
			IsAirJumping = false;
		}
	}
	
	WallSlideCheck();

	if (IsWallSliding)
	{
		WallSlide(DeltaTime);
	}
	else
	{
		HasZVelocityReset = false;
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
	}

	if (IsSliding)
	{
		AddMovementInput(SlideDirection);
	}

	FString HitWallText;
	
	if (IsHeadBlocked)
	{
		if (!IsHeadHitWall())
		{
			StopSliding();
		}
	}
}

void APlayerCharacter::MoveForward(const float InputValue)
{
	if (IsSliding) return;

	AddMovementInput(GetMovementDirection(FVector::ForwardVector), InputValue);
}

void APlayerCharacter::MoveRight(const float InputValue)
{
	if (IsSliding) return;

	AddMovementInput(GetMovementDirection(FVector::RightVector), InputValue);
}

void APlayerCharacter::GroundJump()
{
	Jump();
	IsJumping = true;
}

void APlayerCharacter::AirJump()
{
	ResetZVelocity();
	
	const FVector NewVelocity = GetVelocity() + FVector(0.0f, 0.0f, AirJumpForce);
	
	LaunchCharacter(NewVelocity, true, true);
	IsAirJumping = true;
}

void APlayerCharacter::WallJump()
{
	ResetZVelocity();

	const FVector ForwardVelocity = GetActorForwardVector() * WallJumpVelocity.X * -1.0f;
	const FVector UpwardsVelocity = WallJumpVelocity.Z * FVector::UpVector;
	const FVector NewVelocity = ForwardVelocity + UpwardsVelocity;
	
	LaunchCharacter(NewVelocity, true, true);

	FRotator NewRotation = NewVelocity.Rotation();
	NewRotation.Pitch = 0.0f;
	NewRotation.Roll = 0.0f;
	
	SetActorRotation(NewRotation);
	
	IsWallJumping = true;
}

void APlayerCharacter::DoJump()
{
	if (JumpCount >= JumpMaxCount || IsSliding) return;
	
	JumpCount++;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
	
	if (IsWallSliding)
	{
		WallJump();
		
		return;
	}
	
	if (GetMovementComponent()->IsFalling())
	{
		AirJump();
	
		return;
	}
	
	GroundJump();
}

void APlayerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed * WalkSpeedMultiplier;
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed * SprintSpeedMultiplier;
}

void APlayerCharacter::ResetZVelocity(const bool DoOnce)
{
	if (HasZVelocityReset) return;
	
	GetCharacterMovement()->Velocity = FVector(GetVelocity().X, GetVelocity().Y, 0.0f);
	HasZVelocityReset = DoOnce ? true : false;
}

void APlayerCharacter::WallSlide(const float DeltaTime)
{
	IsWallJumping = false;
	JumpCount = JumpMaxCount - 1;

	const FVector TargetVelocity = FVector(0.0f, 0.0f, GetVelocity().Z);
	const FVector NewVelocity = FMath::VInterpTo(GetVelocity(), TargetVelocity, DeltaTime, WallSlideDeceleration);

	GetCharacterMovement()->Velocity = NewVelocity;

	const FRotator WallHitRotation = UKismetMathLibrary::MakeRotFromX(WallCheckHitResult.Normal);
	const FRotator NewRotation = FRotator(0.0f, WallHitRotation.Yaw + 180.0f, 0.0f);

	SetActorRotation(NewRotation);

	if (GetVelocity().Z >= 0.0f) return;

	ResetZVelocity();
	GetCharacterMovement()->GravityScale = WallSlideGravityScale;
}

void APlayerCharacter::Slide()
{
	if (!GetMovementComponent()->IsMovingOnGround()) return;
		
	Crouch();
	SlideDirection = GetActorForwardVector();
	IsSliding = true;

	FTimerHandle SlideTimerHandle;
	
	GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &APlayerCharacter::StopSliding, SlideDuration);
}

void APlayerCharacter::StopSliding()
{
	IsHeadBlocked = IsHeadHitWall();
	
	if (IsHeadBlocked) return;
		
	UnCrouch();
	IsSliding = false;
}

bool APlayerCharacter::IsHeadHitWall() const
{
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector TraceEnd = GetActorLocation() + 100.0f * FVector::UpVector;
	
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation(),
		TraceEnd,
		ECC_Visibility,
		Params);

	return HitResult.bBlockingHit && HitResult.GetActor()->ActorHasTag(WallCheckTag);
}

void APlayerCharacter::ResetMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
}

void APlayerCharacter::OnCharacterLanded(const FHitResult& Hit)
{
	IsJumping = false;
	IsAirJumping = false;
	IsWallSliding = false;
	HasZVelocityReset = false;
	JumpCount = 0;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
}

void APlayerCharacter::InitializeProperties()
{
	DefaultMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultGravityScale = GetCharacterMovement()->GravityScale;

	WallCheckCapsule = FCollisionShape::MakeCapsule(
		GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.0f,
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	
	QueryParams.AddIgnoredActor(this);
}

void APlayerCharacter::WallSlideCheck()
{
	if (GetCharacterMovement()->IsMovingOnGround()) return;

	const bool HitWall = GetWorld()->SweepSingleByChannel(
		WallCheckHitResult,
		GetActorLocation(),
		GetActorLocation(),
		GetActorRotation().Quaternion(),
		ECC_Visibility,
		WallCheckCapsule,
		QueryParams);

	// DrawDebugCapsule(
	// 	GetWorld(),
	// 	GetActorLocation(),
	// 	WallCheckCapsule.GetCapsuleHalfHeight(),
	// 	WallCheckCapsule.GetCapsuleRadius(),
	// 	GetActorRotation().Quaternion(),
	// 	FColor::Red,
	// 	false,
	// 	5.0f);
	
	IsWallSliding = HitWall && WallCheckHitResult.GetActor()->ActorHasTag(WallCheckTag);
}

FVector APlayerCharacter::GetMovementDirection(const FVector& InVector) const
{
	return FRotator(0.0f, GetControlRotation().Yaw, 0.0f).RotateVector(InVector);
}

bool APlayerCharacter::GetIsFalling() const
{
	return GetCharacterMovement()->IsFalling() && GetVelocity().Z < 0.0f;
}

bool APlayerCharacter::GetIsGrounded() const
{
	return GetCharacterMovement()->IsMovingOnGround();
}