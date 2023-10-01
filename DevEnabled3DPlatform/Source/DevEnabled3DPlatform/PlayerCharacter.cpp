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

	MaskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask Mesh"));
	MaskMeshComponent->SetupAttachment(GetMesh(), FName("head_001Socket"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 600.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 3.0f;
	GetCharacterMovement()->JumpZVelocity = 850.0f;
	GetCharacterMovement()->AirControl = 0.7f;
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

	WallSlideCheck();

	if (IsWallSliding)
	{
		WallSlide(DeltaTime);
	}
	else
	{
		// HasZVelocityReset = false;
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
	}
}

void APlayerCharacter::MoveForward(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::ForwardVector), InputValue);
}

void APlayerCharacter::MoveRight(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::RightVector), InputValue);
}

void APlayerCharacter::GroundJump()
{
	Jump();
	IsJumping = true;
	JumpCount++;
}

void APlayerCharacter::AirJump()
{
	Jump();
	IsJumping = false;
	IsAirJumping = true;
	JumpCount++;
}

void APlayerCharacter::WallJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, __FUNCTION__);
}

void APlayerCharacter::DoJump()
{
	if (JumpCount >= JumpMaxCount) return;
	
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
	ResetZVelocity();
	JumpCount = JumpMaxCount - 1;

	const FVector TargetVelocity = FVector(0.0f, 0.0f, GetVelocity().Z);
	const FVector NewVelocity = FMath::VInterpTo(GetVelocity(), TargetVelocity, DeltaTime, WallSlideDeceleration);

	GetCharacterMovement()->Velocity = NewVelocity;

	const FRotator WallHitRotation = UKismetMathLibrary::MakeRotFromX(WallCheckHitResult.Normal);
	const FRotator NewRotation = FRotator(0.0f, WallHitRotation.Yaw + 180.0f, 0.0f);

	SetActorRotation(NewRotation);

	if (GetVelocity().Z < 0.0f)
	{
		GetCharacterMovement()->GravityScale = WallSlideGravityScale;
	}
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
}

void APlayerCharacter::InitializeProperties()
{
	DefaultMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultGravityScale = GetCharacterMovement()->GravityScale;

	WallCheckCapsule = FCollisionShape::MakeCapsule(
		GetCapsuleComponent()->GetScaledCapsuleRadius() + 1.0f,
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

bool APlayerCharacter::GetIsFalling()
{
	IsAirJumping = false;
	return GetCharacterMovement()->IsFalling() && GetVelocity().Z < 0.0f;
}

bool APlayerCharacter::GetIsGrounded() const
{
	return GetCharacterMovement()->IsMovingOnGround();
}