#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->AirControl = 1.0f;
}	

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::MoveForward(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::ForwardVector), InputValue);
}

void APlayerCharacter::MoveRight(const float InputValue)
{
	AddMovementInput(GetMovementDirection(FVector::RightVector), InputValue);
}

void APlayerCharacter::DoJump()
{
	Jump();
	IsJumping = true;
}

void APlayerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed * WalkSpeedMultiplier;
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed * SprintSpeedMultiplier;
}

void APlayerCharacter::ResetMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
}

FVector APlayerCharacter::GetMovementDirection(const FVector& InVector) const
{
	return FRotator(0.0f, GetControlRotation().Yaw, 0.0f).RotateVector(InVector);
}

bool APlayerCharacter::GetIsInAir() const
{
	return GetCharacterMovement()->IsFalling();
}